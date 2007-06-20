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
//!
#ifndef __INCLUDE_MACHINE_LEARNING__
#define __INCLUDE_MACHINE_LEARNING__


#ifndef __INCLUDE_MIST_H__
#include "mist.h"
#endif


#include <vector>
#include <map>
#include <algorithm>
 
#ifdef _OPENMP
	#include <omp.h>
#endif


#define __ASYMMETRIC_WEIGHTING__		1
#define __ONE_PER_CLASS_CODE_WORD__		0
#define __DEBUG_OUTPUT_LEVEL__			0

// mist名前空間の始まり
_MIST_BEGIN

namespace machine_learning
{
	struct feature_one
	{
		typedef size_t size_type;			///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
		typedef ptrdiff_t difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ

		double value;
		bool   category;
		double weight;

		feature_one( ) : value( 0.0 ), category( true ), weight( 0.0 )
		{
		}

		feature_one( double val ) : value( val ), category( true ), weight( 0.0 )
		{
		}

		feature_one( double val, bool cate, double wei ) : value( val ), category( cate ), weight( wei )
		{
		}

		feature_one( const feature_one &f ): value( f.value ), category( f.category ), weight( f.weight )
		{
		}

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

		const bool operator <( const feature_one &f ) const
		{
			return( value < f.value );
		}
	};

	struct feature : public std::vector< double >
	{
		typedef std::vector< double > base;
		typedef base::value_type value_type;			///< @brief MISTのコンテナ内に格納するデータ型．mist::array< data > の data と同じ
		typedef base::size_type size_type;				///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
		typedef base::difference_type difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ

		std::string category;
		double      weight;
		bool        valid;

		feature( ) : base( ), category( "unknown" ), weight( 0.0 ), valid( true )
		{
		}

		feature( size_type dimension ) : base( dimension ), category( "unknown" ), weight( 0.0 ), valid( true )
		{
		}

		feature( size_type dimension, const std::string &cate ) : base( dimension, 0.0 ), category( cate ), weight( 0.0 ), valid( true )
		{
		}

		feature( const feature &f ): base( f ), category( f.category ), weight( f.weight ), valid( f.valid )
		{
		}

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

		class weak_classifier
		{
		public:
			typedef feature feature_type;
			typedef feature_type::value_type value_type;			///< @brief MISTのコンテナ内に格納するデータ型．mist::array< data > の data と同じ
			typedef feature_type::size_type size_type;				///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
			typedef feature_type::difference_type difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ

		private:
			double sign_;
			double threshold_;
			size_type index_;

		public:
			weak_classifier( ) : sign_( 1.0 ), threshold_( 0.0 ), index_( 0 )
			{
			}

			weak_classifier( double _sign_, double _threshold_, size_type indx = 0 ) : sign_( _sign_ ), threshold_( _threshold_ ), index_( indx )
			{
			}

			weak_classifier( const weak_classifier& w ) : sign_( w.sign_ ), threshold_( w.threshold_ ), index_( w.index_ )
			{
			}

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
			double sign( ) const { return( sign_ ); }
			void   sign( double s ){ sign_ = s; }
			double threshold( ) const { return( threshold_ ); }
			void   threshold( double th ){ threshold_ = th; }
			size_type index( ) const { return( index_ ); }
			void   index( size_type indx ){ index_ = indx; }

		public:
			template < class FEATURE_LIST, class CATEGORY_LIST >
			bool learn( const FEATURE_LIST & features, const CATEGORY_LIST &categories )
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
					size_type mindex = 0;
					double e1, e2, error, min_error = 1.0e100, sgn, th;
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

						e1 = sum_of_positive_weights + overall_sum_of_negative_weights - sum_of_negative_weights;
						e2 = sum_of_negative_weights + overall_sum_of_positive_weights - sum_of_positive_weights;
						error = e1 < e2 ? e1 : e2;

						if( error <= min_error )
						{
							min_error = error;
							mindex    = i;
							th        = f.value;
							sgn       = e1 < e2 ? -1.0 : 1.0;
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
			template < class FEATURE >
			bool operator ()( const FEATURE &f ) const
			{
				return( evaluate( f ) );
			}

			template < class FEATURE >
			bool evaluate( const FEATURE &f ) const
			{
				return( evaluate( f, index_, sign_, threshold_ ) );
			}

			template < class FEATURE >
			bool evaluate( const FEATURE &f, size_type indx, double sgn, double th ) const
			{
				return( sgn * f[ indx ] <= sgn * th );
			}
		};

		class classifier
		{
		public:
			typedef weak_classifier weak_classifier_type;
			typedef feature feature_type;
			typedef feature_type::value_type value_type;			///< @brief MISTのコンテナ内に格納するデータ型．mist::array< data > の data と同じ
			typedef feature_type::size_type size_type;				///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
			typedef feature_type::difference_type difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ

		private:
			std::vector< std::string >          categories_;
			std::vector< weak_classifier_type > weak_classifiers_;
			std::vector< std::vector< bool > >  code_word_;
			std::vector< double >               alpha_;
			std::vector< double >               beta_;

		public:
			classifier( )
			{
			}

			classifier( const classifier &cls ) : categories_( cls.categories_ ), weak_classifiers_( cls.weak_classifiers_ ), code_word_( cls.code_word_ ), alpha_( cls.alpha_ ), beta_( cls.beta_ )
			{
			}

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
			const std::vector< weak_classifier_type > &weak_classifier( ) const
			{
				return( weak_classifiers_ );
			}

			const std::vector< std::string > &categories( ) const
			{
				return( categories_ );
			}

			const std::vector< std::vector< bool > > &code_word( ) const
			{
				return( code_word_ );
			}

			const std::vector< double > &alpha( ) const
			{
				return( alpha_ );
			}

			const std::vector< double > &beta( ) const
			{
				return( beta_ );
			}

		public:
			template < class FEATURE_LIST >
			bool learn( FEATURE_LIST & features, size_type number_of_iterations )
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
				for( size_type i = 0 ; i < fcatemap.size( ) ; i++ )
				{
					fcatemap[ i ] = static_cast< typename std::vector< size_type >::value_type >( category_map[ features[ i ].category ] );
				}

#if defined( __ONE_PER_CLASS_CODE_WORD__ ) && __ONE_PER_CLASS_CODE_WORD__ == 1
				// 各クラス単位で code word を作る
				size_type nhypothesis = categories_.size( );
#else
				// code word を ECC ベースで作る
				size_type nhypothesis = categories_.size( ) == 2 ? 2 : __power_of_two__( categories_.size( ) - 1 ) - 1;
				mist::matrix< bool > code_word_pool( categories_.size( ), nhypothesis );

				if( nhypothesis == 2 )
				{
					code_word_pool( 0, 0 ) = true;
					code_word_pool( 1, 1 ) = true;
				}
				else
				{
					for( size_type c = 0 ; c < code_word_pool.cols( ) ; c++ )
					{
						code_word_pool( 0, c ) = true;
					}

					for( size_type r = 1 ; r < code_word_pool.rows( ); r++ )
					{
						size_type d = ( code_word_pool.cols( ) + 1 ) / __power_of_two__( r );

						for( size_type c = 0 ; c < code_word_pool.cols( ) ; c++ )
						{
							if( ( c / d ) % 2 == 1 )
							{
								code_word_pool( r, c ) = true;
							}
						}
					}
				}
#endif

				double __old_classification_error__ = 1.0;
				for( size_type loop = 0 ; loop < number_of_iterations ; loop++ )
				{
					// AdaBoost により強識別器を学習する
					for( size_type t = 0 ; t < nhypothesis ; t++ )
					{
						// カラーリングを決定する
						code_word_.push_back( std::vector< bool >( categories_.size( ) ) );
						std::vector< bool > &myu = code_word_.back( );

#if defined( __ONE_PER_CLASS_CODE_WORD__ ) && __ONE_PER_CLASS_CODE_WORD__ == 1
						myu[ t % categories_.size( ) ] = true;
#else
						for( size_type c = 0 ; c < code_word_pool.rows( ); c++ )
						{
							myu[ c ] = code_word_pool( c, t % code_word_pool.cols( ) );
						}
#endif
						// 弱識別器の学習用カテゴリデータを作る
						for( size_type i = 0 ; i < fcategories.size( ) ; i++ )
						{
							fcategories[ i ] = myu[ fcatemap[ i ] ];
#if defined( __DEBUG_OUTPUT_LEVEL__ ) && __DEBUG_OUTPUT_LEVEL__ >= 2
							std::cout << myu[ fcatemap[ i ] ];
#endif
						}
#if defined( __DEBUG_OUTPUT_LEVEL__ ) && __DEBUG_OUTPUT_LEVEL__ >= 2
						std::cout << std::endl;
#endif

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

						// 学習に使う弱識別器を用意する
						weak_classifiers_.push_back( weak_classifier_type( ) );
						weak_classifier_type &weak = weak_classifiers_.back( );

						// 弱識別器を学習する
						weak.learn( features, fcategories );


#if defined( __DEBUG_OUTPUT_LEVEL__ ) && __DEBUG_OUTPUT_LEVEL__ >= 2
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

#if defined( __DEBUG_OUTPUT_LEVEL__ ) && __DEBUG_OUTPUT_LEVEL__ >= 2
						// この段階でAdaBoostにより学習した識別器の性能を表示する
						std::cout << "分類誤差: " << error_rate( features ) << std::endl;
#endif
					}

					double __classification_error__ = error_rate( features );

#if defined( __DEBUG_OUTPUT_LEVEL__ ) && __DEBUG_OUTPUT_LEVEL__ >= 1
					// 1ループ終了
					std::cout << "識別器の学習ループ " << loop + 1 << " / " << number_of_iterations << " が終了しました。" << std::endl;
					std::cout << "分類誤差: " << __classification_error__ << std::endl << std::endl;
#endif

					if( __classification_error__ == 0.0 || __classification_error__ == __old_classification_error__ )
					{
						// 分類器の性能に変化が無かった，もしくは，すべて分類できたので終了する
						break;
					}
					else
					{
						__old_classification_error__ = __classification_error__;
					}
				}

				return( true );
			}

		public:
			template < class FEATURE >
			const std::string operator ()( const FEATURE &f ) const
			{
				return( evaluate( f ) );
			}

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

			template < class FEATURE_LIST >
			double error_rate( const FEATURE_LIST & features ) const
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
#if defined( __DEBUG_OUTPUT_LEVEL__ ) && __DEBUG_OUTPUT_LEVEL__ >= 2
						std::cout << evaluate( f );
#endif
						if( f.category != evaluate( f ) )
						{
							error++;
						}
					}
				}

#if defined( __DEBUG_OUTPUT_LEVEL__ ) && __DEBUG_OUTPUT_LEVEL__ >= 2
				std::cout << std::endl;
#endif

				return( static_cast< double >( error ) / static_cast< double >( nfeatures ) );
			}
		};
	}
}


//! @addtogroup machine_learning_group 機械学習を扱う
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/machine_learning.h>
//! @endcode
//!
//!  @{



/// @}
//  機械学習グループの終わり


// mist名前空間の終わり
_MIST_END



#endif // __INCLUDE_MACHINE_LEARNING__
