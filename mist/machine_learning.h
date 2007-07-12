// 
// Copyright (c) 2003-2007, MIST Project, Intelligent Media Integration COE, Nagoya University
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
// 
// 3. Neither the name of the Nagoya University nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
// FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
// IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 

/// @file mist/machine_learning.h
//!
//! @brief 機械学習を行うためのライブラリ
//!
//! マルチクラス対応の AdaBoost アルゴリズムが実装してあります．
//!
//! - 参考文献
//!   - Multiclass Adaboost
//!     - Thomas G. Dietterich, Ghulum Bakiri, ``Solving Multiclass Learning Problems via Error-Correcting Output Codes,'' Journal of Artificial Intelligence Research, 2, pp. 263--286, 1995
//!     - Venkatesan Guruswami, Amit Sahai, ``Multiclass Learning, Boosting, and Error-Correcting Codes,'' Proc. of 12th Annual Conference, Computational Learning Theory, pp. 145--155, 1999
//!     - Robert E. Schapire, Yoram Singer, ``Improved Boosting Algorithms Using Confidence-rated Predictions,'' Machine Learning, 37, pp. 297--336, 1999
//!     - Ling Li, ``Multiclass Boosting with Repartitioning,'' Proceedings of 23rd International Conference on Machine Learning, pp.569--576, 2006
//!
#ifndef __INCLUDE_MACHINE_LEARNING__
#define __INCLUDE_MACHINE_LEARNING__


#ifndef __INCLUDE_MIST_H__
	#include "mist.h"
#endif

#ifndef __INCLUDE_MIST_RANDOM__
	#include "random.h"
#endif


#include <ctime>
#include <vector>
#include <map>
#include <algorithm>

#ifdef _OPENMP
	#include <omp.h>
#endif


// mist名前空間の始まり
_MIST_BEGIN


#define __ASYMMETRIC_WEIGHTING__		1		///< AdaBoost の弱識別器に付与する重みを非対称にするかどうか
#define __ONE_PER_CLASS_CODE_WORD__		0		///< 1クラスに1つ一意の Code Word を割り当てるかどうか
#define __RANDOM_CODE_WORD__			1		///< ERPを実行する際に使用する初期 Code Word をランダムに生成するかどうか
#define __DEBUG_OUTPUT_LEVEL__			0		///< コンソールに学習の様子をデバッグ情報として出力するレベル（0は何も出力しない）
#define __NUMBER_OF_INNER_LOOPS__		10		///< Ling Li の方法を使って Code Word を更新する最大回数


/// @brief 機械学習
namespace machine_learning
{
	/// @brief 機械学習で利用する２カテゴリの特徴量を管理するクラス（内部でのみ使用）
	struct feature_one
	{
		typedef size_t size_type;			///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
		typedef ptrdiff_t difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ

		double value;		///< @brief 特徴量の値
		bool   category;	///< @brief 学習データのカテゴリ名（true か false）
		double weight;		///< @brief 学習データに対する重み

		/// @brief デフォルトのコンストラクタ
		feature_one( ) : value( 0.0 ), category( true ), weight( 0.0 )
		{
		}

		/// @brief 次元数を指定して特長量を初期化
		//! 
		//! @param[in]  val  … 特徴量
		//! @param[in]  cate … カテゴリ
		//! @param[in]  wei  … 重み
		//! 
		feature_one( double val, bool cate, double wei ) : value( val ), category( cate ), weight( wei )
		{
		}

		/// @brief コピーコンストラクタ
		feature_one( const feature_one &f ): value( f.value ), category( f.category ), weight( f.weight )
		{
		}

		/// @brief 他の特徴量をコピーする
		feature_one &operator =( const feature_one &f )
		{
			if( &f != this )
			{
				value  = f.value;
				category = f.category;
				weight = f.weight;
			}

			return( *this );
		}

		/// @brief 他の特徴量と値の大小を比較する
		const bool operator <( const feature_one &f ) const
		{
			return( value < f.value );
		}
	};

	/// @brief 機械学習で利用する多カテゴリの特徴量を管理するクラス
	struct feature : public std::vector< double >
	{
		typedef std::vector< double > base;
		typedef base::value_type value_type;			///< @brief MISTのコンテナ内に格納するデータ型．mist::array< data > の data と同じ
		typedef base::size_type size_type;				///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
		typedef base::difference_type difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ

		std::string category;	///< @brief 学習データのカテゴリ名
		double      weight;		///< @brief 学習データに対する重み
		bool        valid;		///< @brief 機械学習に利用するかどうか

		/// @brief デフォルトのコンストラクタ
		feature( ) : base( ), category( "unknown" ), weight( 0.0 ), valid( true )
		{
		}

		/// @brief 次元数を指定して特長量を初期化
		//! 
		//! @param[in]  dimension … 特徴量の次元
		//! 
		feature( size_type dimension ) : base( dimension ), category( "unknown" ), weight( 0.0 ), valid( true )
		{
		}

		/// @brief カテゴリ名を指定して特長量を初期化
		//! 
		//! @param[in]  cate      … カテゴリ名徴量の次元
		//! 
		feature( const std::string &cate ) : base( ), category( cate ), weight( 0.0 ), valid( true )
		{
		}

		/// @brief 次元数とカテゴリ名を指定して特長量を初期化
		//! 
		//! @param[in]  dimension … 特徴量の次元
		//! @param[in]  cate      … カテゴリ名徴量の次元
		//! 
		feature( size_type dimension, const std::string &cate ) : base( dimension, 0.0 ), category( cate ), weight( 0.0 ), valid( true )
		{
		}

		/// @brief コピーコンストラクタ
		feature( const feature &f ): base( f ), category( f.category ), weight( f.weight ), valid( f.valid )
		{
		}

		/// @brief 他の特徴量をコピーする
		feature &operator =( const feature &f )
		{
			if( &f != this )
			{
				base::operator =( f );
				category = f.category;
				weight = f.weight;
				valid  = f.valid;
			}

			return( *this );
		}
	};

	/// @brief 機械学習で利用する２カテゴリの特徴量を管理するクラス（内部でのみ使用）
	template < class T1, class T2 >
	struct pair
	{
		T1 key;
		T2 value;

		/// @brief 他の特徴量と値の大小を比較する
		const bool operator <( const pair &f ) const
		{
			return( key < f.key );
		}
	};


	//! @addtogroup machine_learning_group 機械学習を扱う
	//!
	//! @code 次のヘッダをインクルードする
	//! #include <mist/machine_learning.h>
	//! @endcode
	//!
	//!  @{

	/// @brief AdaBoost を用いた識別器
	namespace adaboost
	{
		inline size_t __power_of_two__( size_t x )
		{
			size_t val = 1;
			for( size_t i = 0 ; i < x ; i++ )
			{
				val *= 2;
			}
			return( val );
		}

		/// @brief AdaBoost で利用する弱識別器（しきい値処理）
		class weak_classifier
		{
		public:
			typedef feature feature_type;
			typedef feature_type::value_type value_type;			///< @brief MISTのコンテナ内に格納するデータ型．mist::array< data > の data と同じ
			typedef feature_type::size_type size_type;				///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
			typedef feature_type::difference_type difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ

		private:
			double sign_;		///< @brief しきい値の符号
			double threshold_;	///< @brief 分類に用いるしきい値
			size_type index_;	///< @brief 使用する特徴量の番号

		public:
			/// @brief デフォルトのコンストラクタ
			weak_classifier( ) : sign_( 1.0 ), threshold_( 0.0 ), index_( 0 )
			{
			}

			/// @brief パラメータを指定して弱識別器を初期化
			//! 
			//! @param[in]  _sign_      … しきい値の符号
			//! @param[in]  _threshold_ … 分類に用いるしきい値
			//! @param[in]  indx        …  使用する特徴量の番号
			//! 
			weak_classifier( double _sign_, double _threshold_, size_type indx = 0 ) : sign_( _sign_ ), threshold_( _threshold_ ), index_( indx )
			{
			}

			/// @brief コピーコンストラクタ
			weak_classifier( const weak_classifier& w ) : sign_( w.sign_ ), threshold_( w.threshold_ ), index_( w.index_ )
			{
			}

			/// @brief 他の識別器と同じパラメータの識別器となるようにデータをコピーする
			weak_classifier& operator =( const weak_classifier& other )
			{
				if( this != &other )
				{
					sign_      = other.sign_;
					threshold_ = other.threshold_;
					index_     = other.index_;
				}

				return( *this );
			}

		public:
			/// @brief 識別器の符号を取得する
			double sign( ) const { return( sign_ ); }

			/// @brief 識別器の符号を設定する
			void   sign( double s ){ sign_ = s; }

			/// @brief 識別器のしきい値を取得する
			double threshold( ) const { return( threshold_ ); }

			/// @brief 識別器のしきい値を設定する
			void   threshold( double th ){ threshold_ = th; }

			/// @brief 識別器が使用する特徴量の番号符号を取得する
			size_type index( ) const { return( index_ ); }

			/// @brief 識別器が使用する特徴量の番号符号を設定する
			void   index( size_type indx ){ index_ = indx; }

		public:
			/// @brief 教師データを用いて最適な弱識別器を構築する
			//! 
			//! @param[in]  features   … 学習に用いる教師データ
			//! @param[in]  categories … 学習データのカテゴリ（true もしくは false）
			//! 
			template < template < typename, typename > class FEATURE_LIST, template < typename, typename > class CATEGORY_LIST, class Allocator1, class Allocator2 >
			bool learn( const FEATURE_LIST< feature_type, Allocator1 > & features, const CATEGORY_LIST< bool, Allocator2 > &categories )
			{
				if( features.empty( ) )
				{
					return( false );
				}

				double _minimum_classification_error_ = 1.0e100;

				// Positive と Negative それぞれの重みの合計を計算する
				double overall_sum_of_positive_weights = 0.0;
				double overall_sum_of_negative_weights = 0.0;
				for( size_type i = 0 ; i < features.size( ) ; i++ )
				{
					const feature_type &f = features[ i ];
					if( categories[ i ] )
					{
						overall_sum_of_positive_weights += f.weight;
					}
					else
					{
						overall_sum_of_negative_weights += f.weight;
					}
				}

				int nfeatures = static_cast< int >( features[ 0 ].size( ) );

				// 特徴量のリストを作成する
				#pragma omp parallel for firstprivate( nfeatures ) schedule( guided )
				for( int index = 0 ; index < nfeatures ; index++ )
				{
					std::vector< feature_one > flist;
					flist.reserve( features.size( ) );
					for( size_type i = 0 ; i < features.size( ) ; i++ )
					{
						const feature_type &f = features[ i ];
						if( f.valid )
						{
							flist.push_back( feature_one( f[ index ], categories[ i ], f.weight ) );
						}
					}

					// 特徴量をソートする
					std::sort( flist.begin( ), flist.end( ) );

					// 各しきい値での重み付き誤差を計算し，誤差最小のしきい値を求める
					double min_error = 1.0e100, sgn = 1.0, th = 0.0;
					double sum_of_positive_weights = 0.0;
					double sum_of_negative_weights = 0.0;

					for( size_type i = 0 ; i < flist.size( ) ; i++ )
					{
						const feature_one &f = flist[ i ];
						if( f.category )
						{
							sum_of_positive_weights += f.weight;
						}
						else
						{
							sum_of_negative_weights += f.weight;
						}

						double e1 = sum_of_positive_weights + overall_sum_of_negative_weights - sum_of_negative_weights;
						double e2 = sum_of_negative_weights + overall_sum_of_positive_weights - sum_of_positive_weights;
						double error = e1 < e2 ? e1 : e2;

						if( error <= min_error )
						{
							min_error = error;
							th        = f.value;
							sgn       = e1 < e2 ? -1.0 : 1.0;

							if( i < flist.size( ) - 1 )
							{
								th = ( th + flist[ i + 1 ].value ) * 0.5;
							}
						}
					}

					double e = 0.0;
					for( size_t i = 0 ; i < features.size( ) ; i++ )
					{
						const feature_type &f = features[ i ];
						if( f.valid )
						{
							if( evaluate( f, index, sgn, th ) != categories[ i ] )
							{
								e += f.weight;
							}
						}
					}

					#pragma omp critical
					if( _minimum_classification_error_ > e )
					{
						_minimum_classification_error_ = e;
						index_ = index;
						sign_ = sgn;
						threshold_ = th;
					}
				}

				return( true );
			}

		public:
			/// @brief 学習済みの弱識別器を用いて特徴量を分類する
			//! 
			//! @param[in]  f … 分類する特徴量
			//! 
			template < class FEATURE >
			bool operator ()( const FEATURE &f ) const
			{
				return( evaluate( f ) );
			}

			/// @brief 学習済みの弱識別器を用いて特徴量を分類する
			//! 
			//! @param[in]  f … 分類する特徴量
			//! 
			template < class FEATURE >
			bool evaluate( const FEATURE &f ) const
			{
				return( evaluate( f, index_, sign_, threshold_ ) );
			}

			/// @brief 学習済みの弱識別器を用いて特徴量を分類する
			template < class FEATURE >
			bool evaluate( const FEATURE &f, size_type indx, double sgn, double th ) const
			{
				return( sgn * f[ indx ] <= sgn * th );
			}
		};

		/// @brief AdaBoost を用いた識別器（マルチクラス対応）
		class classifier
		{
		public:
			typedef weak_classifier weak_classifier_type;			///< @brief Boosting する弱識別器のクラス
			typedef feature feature_type;							///< @brief 学習に用いる特徴量を扱うクラス
			typedef feature_type::value_type value_type;			///< @brief MISTのコンテナ内に格納するデータ型．mist::array< data > の data と同じ
			typedef feature_type::size_type size_type;				///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
			typedef feature_type::difference_type difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ

		private:
			std::vector< std::string >          categories_;		///< @brief 分類に使用するカテゴリ名のリスト
			std::vector< weak_classifier_type > weak_classifiers_;	///< @brief 分類に使用する弱識別器のリスト
			std::vector< std::vector< bool > >  code_word_;			///< @brief 分類に使用する Code Word のリスト
			std::vector< double >               alpha_;				///< @brief 分類に使用する係数リスト
			std::vector< double >               beta_;				///< @brief 分類に使用する係数リスト

		public:
			/// @brief デフォルトのコンストラクタ
			classifier( )
			{
			}

			/// @brief コピーコンストラクタ
			classifier( const classifier &cls ) : categories_( cls.categories_ ), weak_classifiers_( cls.weak_classifiers_ ), code_word_( cls.code_word_ ), alpha_( cls.alpha_ ), beta_( cls.beta_ )
			{
			}

			/// @brief 他の識別器と同じパラメータの識別器となるようにデータをコピーする
			classifier& operator =( const classifier& other )
			{
				if( this != &other )
				{
					categories_        = other.categories_;
					weak_classifiers_  = other.weak_classifiers_;
					code_word_         = other.code_word_;
					alpha_             = other.alpha_;
					beta_              = other.beta_;
				}

				return( *this );
			}

		public:
			/// @brief 分類に使用する弱識別器のリストを取得する
			const std::vector< weak_classifier_type > &weak_classifiers( ) const
			{
				return( weak_classifiers_ );
			}

			/// @brief 分類に使用するカテゴリのリストを取得する
			const std::vector< std::string > &categories( ) const
			{
				return( categories_ );
			}

			/// @brief 分類に使用する Code Word のリストを取得する
			const std::vector< std::vector< bool > > &code_word( ) const
			{
				return( code_word_ );
			}

			/// @brief 分類に使用する係数のリストを取得する
			const std::vector< double > &alpha( ) const
			{
				return( alpha_ );
			}

			/// @brief 分類に使用する係数のリストを取得する
			const std::vector< double > &beta( ) const
			{
				return( beta_ );
			}

		public:
			/// @brief 教師データを用いて最適な弱識別器を構築する
			//! 
			//! 指定した number_of_iterations 回 Boosting を実行する．分類誤差が 0 となった場合は途中で終了する．
			//! 
			//! @param[in]  features             … 学習に用いる教師データ
			//! @param[in]  number_of_iterations … 学習を実行する最大ステップ数
			//! 
			template < template < typename, typename > class FEATURE_LIST, class Allocator >
			bool learn( FEATURE_LIST< feature_type, Allocator > & features, size_type number_of_iterations )
			{
				if( features.empty( ) )
				{
					std::cerr << "特徴データが指定されていません．" << std::endl;
					return( false );
				}
				else if( number_of_iterations == 0 )
				{
					// 少なくとも1回は実行する
					number_of_iterations++;
				}

				// 強識別器を初期化する
				weak_classifiers_.clear( );
				categories_.clear( );
				code_word_.clear( );
				alpha_.clear( );
				beta_.clear( );


				// カテゴリの数を数える
				std::map< std::string, size_type > category_map;
				for( size_type i = 0 ; i < features.size( ) ; i++ )
				{
					category_map[ features[ i ].category ] = 0;
				}

				// カテゴリの数を確定する
				categories_.reserve( category_map.size( ) );

				// カテゴリのリストを登録する
				std::map< std::string, size_type >::iterator ite = category_map.begin( );
				for( ; ite != category_map.end( ) ; ++ite )
				{
					ite->second = categories_.size( );
					categories_.push_back( ite->first );
				}

				// 重みを初期化する
				mist::matrix< double > D( features.size( ), categories_.size( ) );
				for( size_type i = 0 ; i < features.size( ) ; i++ )
				{
					const feature_type &f = features[ i ];
					size_type category_index = category_map[ f.category ];
					for( size_type l = 0 ; l < categories_.size( ) ; l++ )
					{
						if( l != category_index )
						{
							D( i, l ) = 1.0 / static_cast< double >( features.size( ) * ( categories_.size( ) - 1 ) );
						}
						else
						{
							D( i, l ) = 0.0;
						}
					}
				}

				std::vector< size_type > fcatemap( features.size( ) );
				std::vector< bool > fcategories( features.size( ) );
				std::vector< double > rpweight( categories_.size( ) );
				for( size_type i = 0 ; i < fcatemap.size( ) ; i++ )
				{
					fcatemap[ i ] = static_cast< typename std::vector< size_type >::value_type >( category_map[ features[ i ].category ] );
				}

#if defined( __ONE_PER_CLASS_CODE_WORD__ ) && __ONE_PER_CLASS_CODE_WORD__ == 1
				// 各クラス単位で code word を作る
				size_type nhypothesis = categories_.size( );
#elif defined( __RANDOM_CODE_WORD__ ) && __RANDOM_CODE_WORD__ == 1
				uniform::random rnd( std::clock( ) );
				// code word を ECC ベースで作る
				size_type nhypothesis = categories_.size( );
#else
				// code word を ECC ベースで作る
				size_type nhypothesis = categories_.size( ) == 2 ? 2 : __power_of_two__( categories_.size( ) - 1 ) - 1;
#endif

				// AdaBoost により強識別器を学習する
				for( size_type t = 0 ; t < number_of_iterations ; t++ )
				{
					// カラーリングを決定する
					code_word_.push_back( std::vector< bool >( categories_.size( ) ) );
					std::vector< bool > &myu = code_word_.back( );

#if defined( __ONE_PER_CLASS_CODE_WORD__ ) && __ONE_PER_CLASS_CODE_WORD__ == 1
					myu[ t % categories_.size( ) ] = true;
#elif defined( __RANDOM_CODE_WORD__ ) && __RANDOM_CODE_WORD__ == 1
					{
						std::vector< pair< double, size_type > > list( categories_.size( ) );
						for( size_type i = 0 ; i < list.size( ) ; i++ )
						{
							list[ i ].key = rnd.real1( );
							list[ i ].value = i;
						}
						std::sort( list.begin( ), list.end( ) );
						for( size_type i = 0 ; i < list.size( ) ; i++ )
						{
							myu[ list[ i ].value ] = i < list.size( ) / 2;
						}
					}
#else
					if( nhypothesis == 2 )
					{
						myu[ 0 ] = ( t % nhypothesis ) == 0;
						myu[ 1 ] = !myu[ 0 ];
					}
					else
					{
						myu[ 0 ] = true;
						for( size_type r = 1 ; r < myu.size( ) ; r++ )
						{
							// code word を ECC ベースで動的に作る
							size_type c = t % nhypothesis;
							size_type d = ( nhypothesis + 1 ) / __power_of_two__( r );
							myu[ r ] = ( ( c / d ) % 2 ) == 1;
						}
					}
#endif

#if defined( __DEBUG_OUTPUT_LEVEL__ ) && __DEBUG_OUTPUT_LEVEL__ >= 3
					// 弱識別器の学習用カテゴリデータを作る
					for( size_type i = 0 ; i < fcategories.size( ) ; i++ )
					{
						std::cout << myu[ fcatemap[ i ] ];
					}
					std::cout << std::endl;
#endif

					// 学習に使う弱識別器を用意する
					weak_classifiers_.push_back( weak_classifier_type( ) );
					weak_classifier_type &weak = weak_classifiers_.back( );

					for( size_type m = 0 ; m < __NUMBER_OF_INNER_LOOPS__ ; m++ )
					{
						// 弱識別器の学習用カテゴリデータを作る
						for( size_type i = 0 ; i < fcategories.size( ) ; i++ )
						{
							fcategories[ i ] = myu[ fcatemap[ i ] ];
						}

						// 重みを正規化する
						double Ut = 0.0;
						for( size_type i = 0 ; i < features.size( ) ; i++ )
						{
							const feature_type &f = features[ i ];
							if( f.valid )
							{
								bool myuY = fcategories[ i ];
								for( size_type l = 0 ; l < categories_.size( ) ; l++ )
								{
									if( myuY != myu[ l ] )
									{
										Ut += D( i, l );
									}
								}
							}
						}

						for( size_type i = 0 ; i < D.rows( ) ; i++ )
						{
							feature_type &f = features[ i ];
							if( f.valid )
							{
								bool myuY = fcategories[ i ];
								double sum = 0.0;
								for( size_type l = 0 ; l < categories_.size( ) ; l++ )
								{
									if( myuY != myu[ l ] )
									{
										sum += D( i, l );
									}
								}

								f.weight = sum / Ut;
							}
						}

						// 弱識別器を学習する
						weak.learn( features, fcategories );
						//std::cout << "しきい値: " << weak.threshold( ) << ", 符号: " << ( weak.sign( ) < 0.0 ? "-" : "+" ) << ", 番号: " << weak.index( ) << std::endl;

						if( m < __NUMBER_OF_INNER_LOOPS__ - 1 )
						{
							for( size_type i = 0 ; i < rpweight.size( ) ; i++ )
							{
								rpweight[ i ] = 0.0;
							}

							double sum = 0.0;
							for( size_type i = 0 ; i < features.size( ) ; i++ )
							{
								feature_type &f = features[ i ];
								if( f.valid )
								{
									double val = weak( f ) ? 1.0 : -1.0;

									double tmp = 0.0;
									for( size_type l = 0 ; l < D.cols( ) ; l++ )
									{
										tmp += D( i, l ) * val;
									}

									rpweight[ fcatemap[ i ] ] += tmp;

									for( size_type l = 0 ; l < D.cols( ) ; l++ )
									{
										rpweight[ l ] -= D( i, l ) * val;
									}
								}
							}

							bool isChanged = false;
							for( size_type i = 0 ; i < rpweight.size( ) ; i++ )
							{
								bool nmyu = rpweight[ i ] >= 0.0;
								isChanged = isChanged || nmyu != myu[ i ];
								myu[ i ] = nmyu;
							}

							if( !isChanged )
							{
								// 前回とまったく同じなので終了する
								break;
							}
						}
					}


#if defined( __DEBUG_OUTPUT_LEVEL__ ) && __DEBUG_OUTPUT_LEVEL__ >= 3
					// 学習した弱識別器の分類結果を表示する
					for( size_type i = 0 ; i < features.size( ) ; i++ )
					{
						const feature_type &f = features[ i ];
						if( f.valid )
						{
							std::cout << weak.evaluate( f );
						}
					}
					std::cout << std::endl;
#endif

#if defined( __ASYMMETRIC_WEIGHTING__ ) && __ASYMMETRIC_WEIGHTING__ == 1
					// 重みの更新を非対称に行う
					const double eps = 1.0e-16;
					double h1u1 = eps;
					double h1u0 = eps;
					double h0u0 = eps;
					double h0u1 = eps;

					for( size_type i = 0 ; i < features.size( ) ; i++ )
					{
						const feature_type &f = features[ i ];
						if( f.valid )
						{
							bool uY = fcategories[ i ];
							bool hX = weak( f );
							if( hX && uY )
							{
								h1u1 += f.weight;
							}
							else if( hX && !uY )
							{
								h1u0 += f.weight;
							}
							else if( !hX && !uY )
							{
								h0u0 += f.weight;
							}
							else
							{
								h0u1 += f.weight;
							}
						}
					}

					double alpha = 0.5 * std::log( h1u1 / h1u0 );
					double beta = -0.5 * std::log( h0u0 / h0u1 );
#else
					// 重みの更新を対称にする
					const double eps = 1.0e-16;
					double positives = eps;
					double negatives = eps;

					for( size_type i = 0 ; i < features.size( ) ; i++ )
					{
						const feature_type &f = features[ i ];
						if( f.valid )
						{
							bool uY = fcategories[ i ];
							bool hX = weak( f );
							if( hX == uY )
							{
								positives += f.weight;
							}
							else
							{
								negatives += f.weight;
							}
						}
					}

					double alpha = 0.5 * std::log( positives / negatives );
					double beta = -alpha;
#endif

					alpha_.push_back( alpha );
					beta_.push_back( beta );

					double Zt = 0.0;
					for( size_type i = 0 ; i < features.size( ) ; i++ )
					{
						const feature_type &f = features[ i ];
						if( f.valid )
						{
							bool myuY = fcategories[ i ];
							for( size_type l = 0 ; l < categories_.size( ) ; l++ )
							{
								double v = ( myu[ l ] - myuY ) * 0.5;
								if( weak( f ) )
								{
									v *= alpha;
								}
								else
								{
									v *= beta;
								}

								D( i, l ) *= std::exp( v );
								Zt += D( i, l );
							}
						}
					}

					for( size_type i = 0 ; i < D.size( ) ; i++ )
					{
						D[ i ] /= Zt;
					}

					if( ( ( t + 1 ) % 5 ) == 0 )
					{
						double __classification_error__ = error_rate( features );

#if defined( __DEBUG_OUTPUT_LEVEL__ ) && __DEBUG_OUTPUT_LEVEL__ >= 1
						// 1ループ終了
						std::cout << "識別器の学習ループ " << t + 1 << " / " << number_of_iterations << " が終了しました。";
						std::cout << "分類誤差: " << __classification_error__ << std::endl;
#endif

						if( __classification_error__ == 0.0 )
						{
							// 分類器の性能に変化が無かった，もしくは，すべて分類できたので終了する
							break;
						}
					}
				}

				return( true );
			}

		public:
			/// @brief 学習済みの識別器を用いて特徴量を分類する
			//! 
			//! @param[in]  f … 分類する特徴量
			//! 
			//! @return 分類結果（学習の際に指定したカテゴリ名）
			//! 
			template < class FEATURE >
			const std::string operator ()( const FEATURE &f ) const
			{
				return( evaluate( f ) );
			}

			/// @brief 学習済みの識別器を用いて特徴量を分類する
			//! 
			//! @param[in]  f … 分類する特徴量
			//! 
			//! @return 分類結果（学習の際に指定したカテゴリ名）
			//! 
			template < class FEATURE >
			const std::string evaluate( const FEATURE &f ) const
			{
#if 1
				std::vector< double > values( categories_.size( ), 0.0 );
				for( size_type t = 0 ; t < weak_classifiers_.size( ) ; t++ )
				{
					const std::vector< bool > &code = code_word_[ t ];
					double weight = weak_classifiers_[ t ]( f ) ? alpha_[ t ] : beta_[ t ];

					for( size_type l = 0 ; l < categories_.size( ) ; l++ )
					{
						values[ l ] += code[ l ] * weight;
					}
				}

				size_type category = 0;
				for( size_type l = 1 ; l < categories_.size( ) ; l++ )
				{
					if( values[ category ] < values[ l ] )
					{
						category = l;
					}
				}
#else
				double max = 0.0;
				size_type category = 0;
				for( size_type l = 0 ; l < categories_.size( ) ; l++ )
				{
					double sum = 0.0;
					for( size_type t = 0 ; t < weak_classifiers_.size( ) ; t++ )
					{
						double val = code_word_[ t ][ l ];
						if( weak_classifiers_[ t ]( f ) )
						{
							val *= alpha_[ t ];
						}
						else
						{
							val *= beta_[ t ];
						}

						sum += val;
					}

					if( sum > max )
					{
						max = sum;
						category = l;
					}
				}
#endif

				return( categories_[ category ] );
			}

			/// @brief 学習済みの識別器の分類誤差を計算する
			//! 
			//! @param[in] features … 分類する特徴量のリスト
			//! 
			//! @return 分類誤差
			//! 
			template < template < typename, typename > class FEATURE_LIST, class Allocator >
			double error_rate( const FEATURE_LIST< feature_type, Allocator > & features ) const
			{
				if( features.empty( ) )
				{
					return( 0.0 );
				}

				size_type error = 0;
				int nfeatures = static_cast< int >( features.size( ) );

				#pragma omp parallel for firstprivate( nfeatures ) reduction( +: error ) schedule( guided )
				for( int i = 0 ; i < nfeatures ; i++ )
				{
					const feature_type &f = features[ i ];
					if( f.valid )
					{
						std::string ret = evaluate( f );
#if defined( __DEBUG_OUTPUT_LEVEL__ ) && __DEBUG_OUTPUT_LEVEL__ >= 3
						std::cout << ret;
#endif
						if( f.category != ret )
						{
							error++;
						}
					}
				}

#if defined( __DEBUG_OUTPUT_LEVEL__ ) && __DEBUG_OUTPUT_LEVEL__ >= 3
				std::cout << std::endl;
#endif

				return( static_cast< double >( error ) / static_cast< double >( nfeatures ) );
			}

		private:
			static const char *get_value( const char *s, const char *e, std::string &val )
			{
				// 先頭の空白（改行やタブを含む）を飛ばす
				while( s < e )
				{
					if( s[ 0 ] == '\r' )
					{
						if( s + 1 != e && s[ 1 ] == '\n' )
						{
							s = s + 2;
						}
						else
						{
							s++;
						}
					}
					else if( s[ 0 ] == '\n' )
					{
						s++;
					}
					else if( s[ 0 ] == ' ' || s[ 0 ] == '\t' )
					{
						s++;
					}
					else
					{
						break;
					}
				}

				const char *sp = s;
				const char *ep = sp;

				// 次にコンマが来る前まで進める
				while( s < e )
				{
					if( s[ 0 ] == '\r' )
					{
						if( s + 1 != e && s[ 1 ] == '\n' )
						{
							s = s + 2;
						}
						else
						{
							s++;
						}
						break;
					}
					else if( s[ 0 ] == '\n' )
					{
						s++;
						break;
					}
					else if( s[ 0 ] == ',' )
					{
						s++;
						break;
					}

					ep++;
					s++;
				}

				if( sp < ep )
				{
					val = std::string( sp, ep );
				}

				return( s > e ? e : s );
			}

		public:
			/// @brief 学習済みの識別器のパラメータをファイルに保存する
			//! 
			//! @attention save と load はペアで使用してください．
			//! 
			//! @param[in] filename … パラメータを保存するファイル名
			//! 
			//! @return 保存に成功したかどうか
			//! 
			bool save( const std::string &filename ) const
			{
				FILE *fp = fopen( filename.c_str( ), "wt" );

				if( fp == NULL )
				{
					return( false );
				}

				// クラス数等の基本情報を書き込む
				fprintf( fp, "Category = %d\n", categories_.size( ) );
				fprintf( fp, "Stage    = %d\n", weak_classifiers_.size( ) );

				// クラス名を列挙する
				for( size_type i = 0 ; i < categories_.size( ) ; i++ )
				{
					fprintf( fp, "Class[%d] : %s\n", i + 1, categories_[ i ].c_str( ) );
				}

				// Code Word を列挙する
				for( size_type i = 0 ; i < code_word_.size( ) ; i++ )
				{
					const std::vector< bool > &code = code_word_[ i ];
					fprintf( fp, "%d", code[ 0 ] ? 1: 0 );
					for( size_type l = 1 ; l < code.size( ) ; l++ )
					{
						fprintf( fp, ",%d", code[ l ] ? 1: 0 );
					}
					fprintf( fp, "\n" );
				}

				// 弱識別器と Alpha と Beta を列挙する
				for( size_type i = 0 ; i < weak_classifiers_.size( ) ; i++ )
				{
					const weak_classifier_type &weak = weak_classifiers_[ i ];
					fprintf( fp, "%f,%f,%d,%f,%f\n", weak.sign( ), weak.threshold( ), weak.index( ), alpha_[ i ], beta_[ i ] );
				}

				fclose( fp );

				return( true );
			}

			/// @brief 学習済みの識別器のパラメータをファイルから読み込む
			//! 
			//! @attention save と load はペアで使用してください．
			//! 
			//! @param[in] filename … パラメータを保存してあるファイル名
			//! 
			//! @return 読込に成功したかどうか
			//! 
			bool load( const std::string &filename )
			{
				FILE *fp = fopen( filename.c_str( ), "rt" );

				if( fp == NULL )
				{
					return( false );
				}

				int numClasses = 0, numStages = 0, dmy;
				char line[ 4096 ], buff[ 4096 ];

				// クラス数等の基本情報を書き込む
				fgets( line, 4096, fp );
				sscanf( line, "Category  = %d", &numClasses );
				fgets( line, 4096, fp );
				sscanf( line, "Stage     = %d", &numStages );

				// 強識別器を初期化する
				weak_classifiers_.resize( numStages );
				alpha_.resize( numStages );
				beta_.resize( numStages );
				code_word_.resize( numStages );
				categories_.resize( numClasses );

				// クラス名を読み込む
				for( size_type i = 0 ; i < categories_.size( ) ; i++ )
				{
					fgets( line, 4096, fp );
					sscanf( line, "Class[%d] : %s", &dmy, buff );
					categories_[ i ] = buff;
				}

				// Code Word を読み込む
				for( size_type i = 0 ; i < code_word_.size( ) ; i++ )
				{
					std::vector< bool > &code = code_word_[ i ];
					code.resize( numClasses );

					fgets( line, 4096, fp );
					const char *p = line, *ep = line + 4096;

					for( size_type l = 0 ; l < code.size( ) ; l++ )
					{
						std::string val;
						p = get_value( p, ep, val );
						code[ l ] = val == "1" ? true : false;
					}
				}

				// 弱識別器と Alpha と Beta を列挙する
				int index;
				double sign, th, alpha, beta;
				for( size_type i = 0 ; i < weak_classifiers_.size( ) ; i++ )
				{
					fgets( line, 4096, fp );
					sscanf( line, "%lf,%lf,%d,%lf,%lf", &sign, &th, &index, &alpha, &beta );

					weak_classifier_type &weak = weak_classifiers_[ i ];
					weak.sign( sign );
					weak.threshold( th );
					weak.index( index );
					alpha_[ i ] = alpha;
					beta_[ i ] = beta;
				}

				return( true );
			}
		};
	}


	/// @}
	//  機械学習グループの終わり
}


// mist名前空間の終わり
_MIST_END



#endif // __INCLUDE_MACHINE_LEARNING__
