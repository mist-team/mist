/// @file mist/filter/distance.h
//!
//! @brief �e�����̉摜�ɑΉ������C�e�틗���ϊ��E�t�����ϊ��A���S���Y��
//!
//! @section ���[�N���b�h�����ϊ�
//! -# �V���L��, ���e����Y, "3�����f�B�W�^���摜�ɑ΂��郆�[�N���b�h�����ϊ�," �d�q���ʐM�w��_����, J76-D-II, No. 3, pp.445-453, 1993
//!

#ifndef __INCLUDE_MIST_DISTANCE_TRANSFORM__
#define __INCLUDE_MIST_DISTANCE_TRANSFORM__

#include <cmath>

#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#ifndef __INCLUDE_MIST_THREAD__
#include "../thread.h"
#endif

#ifndef __INCLUDE_MIST_LIMITS__
#include "../limits.h"
#endif



// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


// ���[�N���b�h�^�����ϊ�
namespace __euclidean_distance_transform__
{
	template < class T >
	void euclidean_distance_transform_x( T &in, double max_length = -1.0, typename T::size_type thread_id = 0, typename T::size_type thread_num = 1 )
	{
		typedef typename T::difference_type difference_type;
		typedef typename T::value_type value_type;

		difference_type i, j, k;
		value_type nd;

		const difference_type w = in.width( );
		const difference_type h = in.height( );
		const difference_type d = in.depth( );

		value_type max = static_cast< value_type >( max_length <= 0 ? sqrt( static_cast< double >( type_limits< value_type >::maximum( ) ) ) : max_length );

		for( k = 0 ; k < d ; k++ )
		{
			for( j = thread_id ; j < h ; j += thread_num )
			{
				if( in( 0, j, k ) != 0 )
				{
					nd = static_cast< value_type >( w ) < max ? static_cast< value_type >( w ) : max;
					in( 0, j, k ) = nd * nd;
				}
				else
				{
					nd = 0;
				}

				for( i = 1 ; i < w ; i++ )
				{
					if( in( i, j, k ) != 0 )
					{
						nd = nd + 1 < max ? nd + 1 : nd;
						in( i, j, k ) = nd * nd;
					}
					else
					{
						nd = 0;
					}
				}

				if( in( w - 1, j, k ) != 0 )
				{
					nd = static_cast< value_type >( w ) < max ? static_cast< value_type >( w ) : max;
					in( w - 1, j, k ) = nd * nd;
				}
				else
				{
					nd = 0;
				}

				for( i = w - 2 ; i >= 0 ; i-- )
				{
					if( in( i, j, k ) != 0 )
					{
						nd = nd + 1 < max ? nd + 1 : nd;
						in( i, j, k ) = in( i, j, k ) < nd * nd ? in( i, j, k ) : nd * nd;
					}
					else
					{
						nd = 0;
					}
				}
			}
		}
	}

	template < class T >
	void euclidean_distance_transform_y( T &in, double max_length = -1.0, typename T::size_type thread_id = 0, typename T::size_type thread_num = 1 )
	{
		typedef typename T::size_type  size_type;
		typedef typename T::value_type value_type;
		typedef typename T::difference_type difference_type;

		size_type i, j, k;
		difference_type n;

		double wmin, wtmp;
		size_type irange;
		difference_type irange1, irange2;
		double *work;
		double vy, vyvy;

		size_type w = in.width( );
		size_type h = in.height( );
		size_type d = in.depth( );

		value_type max = static_cast< value_type >( max_length <= 0 ? sqrt( static_cast< double >( type_limits< value_type >::maximum( ) ) ) : max_length );

		vy = in.reso2( ) / in.reso1( );
		vyvy = vy * vy;
		work = new double[ h ];

		for( k = 0 ; k < d ; k++ )
		{
			for( i = thread_id ; i < w ; i += thread_num )
			{
				for( j = 0 ; j < h ; j++ )
				{
					work[ j ] = static_cast< double >( in( i, j, k ) );
				}

				for( j = 0 ; j < h ; j++ )
				{
					wmin = work[ j ];

					if( wmin != 0.0 )
					{
						irange = static_cast< size_type >( sqrt( wmin ) / vy ) + 1;
						irange = static_cast< size_type >( static_cast< value_type >( irange ) < max ? irange : max );

						irange1 = j < irange  ? j : irange;
						irange2 = j + irange >= h ? h - 1 - j : irange;
						for( n = -irange1 ; n <= irange2 ; n++ )
						{
							wtmp = work[ j + n ] + static_cast< double >( n * n * vyvy );
							wmin = wmin > wtmp ? wtmp : wmin;
						}

						in( i, j, k ) = static_cast< value_type >( wmin );
					}
				}
			}
		}

		delete [] work;
	}


	template < class T >
	void euclidean_distance_transform_z( T &in, double max_length = -1.0, typename T::size_type thread_id = 0, typename T::size_type thread_num = 1 )
	{
		typedef typename T::size_type  size_type;
		typedef typename T::value_type value_type;
		typedef typename T::difference_type difference_type;

		size_type i, j, k;
		difference_type n;
		double wmin, wtmp;
		size_type irange;
		difference_type irange1, irange2;
		double *work;
		double vz, vzvz;

		size_type w = in.width( );
		size_type h = in.height( );
		size_type d = in.depth( );

		vz = in.reso3( ) / in.reso1( );
		vzvz = vz * vz;
		work = new double[ d ];

		value_type max = static_cast< value_type >( max_length <= 0 ? sqrt( static_cast< double >( type_limits< value_type >::maximum( ) ) ) : max_length );

		for( j = 0 ; j < h ; j++ )
		{
			for( i = thread_id ; i < w ; i += thread_num )
			{
				for( k = 0 ; k < d ; k++ )
				{
					work[ k ] = static_cast< double >( in( i, j, k ) );
				}

				for( k = 0 ; k < d ; k++ )
				{
					wmin = work[ k ];

					if( wmin != 0.0 )
					{
						irange = static_cast< size_type >( sqrt( wmin ) / vz ) + 1;
						irange = static_cast< size_type >( static_cast< value_type >( irange ) < max ? irange : max );

						irange1 = k < irange ? k : irange;
						irange2 = k + irange >= d ? d - 1 - k : irange;
						for( n = -irange1 ; n <= irange2 ; n++ )
						{
							wtmp = work[ k + n ] + static_cast< double >( n * n * vzvz );
							wmin = wmin > wtmp ? wtmp : wmin;
						}

						in( i, j, k ) = static_cast< value_type >( wmin );
					}
				}
			}
		}

		delete [] work;
	}
}


// ���[�N���b�h�^�����ϊ��̃X���b�h����
namespace __distance_transform_controller__
{
	template < class T >
	class euclidean_distance_transform_thread : public mist::thread< euclidean_distance_transform_thread< T > >
	{
	public:
		typedef mist::thread< euclidean_distance_transform_thread< T > > base;
		typedef typename base::thread_exit_type thread_exit_type;
		typedef typename T::size_type size_type;
		typedef typename T::value_type value_type;

	private:
		size_t thread_id_;
		size_t thread_num_;

		// ���o�͗p�̉摜�ւ̃|�C���^
		T *in_;
		double max_length_;
		size_type axis_;

	public:
		void setup_parameters( T &in, double max_length, size_type axis, size_type thread_id, size_type thread_num )
		{
			in_  = &in;
			max_length_ = max_length;
			axis_ = axis;
			thread_id_ = thread_id;
			thread_num_ = thread_num;
		}

		void setup_axis( size_type axis )
		{
			axis_ = axis;
		}

		const euclidean_distance_transform_thread& operator =( const euclidean_distance_transform_thread &p )
		{
			if( &p != this )
			{
				base::operator =( p );
				thread_id_ = p.thread_id_;
				thread_num_ = p.thread_num_;
				in_ = p.in_;
				max_length_ = p.max_length;
				axis_ = p.axis_;
			}
			return( *this );
		}

		euclidean_distance_transform_thread( size_type id = 0, size_type num = 1 )
			: thread_id_( id ), thread_num_( num ), in_( NULL ), max_length_( -1.0 ), axis_( 0 )
		{
		}
		euclidean_distance_transform_thread( const euclidean_distance_transform_thread &p )
			: base( p ), thread_id_( p.thread_id_ ), thread_num_( p.thread_num_ ), in_( NULL ), max_length_( -1.0 ), axis_( p.axis_ )
		{
		}

	protected:
		// �p��������ŕK�����������X���b�h�֐�
		virtual thread_exit_type thread_function( )
		{
			switch( axis_ )
			{
			case 1:
				__euclidean_distance_transform__::euclidean_distance_transform_y( *in_, max_length_, thread_id_, thread_num_ );
				break;

			case 2:
				__euclidean_distance_transform__::euclidean_distance_transform_z( *in_, max_length_, thread_id_, thread_num_ );
				break;

			case 0:
			default:
				__euclidean_distance_transform__::euclidean_distance_transform_x( *in_, max_length_, thread_id_, thread_num_ );
				break;
			}
			return( true );
		}
	};
}



//! @addtogroup distance_group �����ϊ�
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/filter/distance.h>
//! @endcode
//!
//!  @{


//! @addtogroup euclidean_distance_group ���[�N���b�h�����ϊ�
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/filter/distance.h>
//! @endcode
//!
//!  @{

/// @brief �ē��搶�ɂ�郆�[�N���b�h�����ϊ�
namespace euclidean
{
	/// @brief ���[�N���b�h�����ϊ�
	//! 
	//! @attention ���͂Əo�͂́C����MIST�R���e�i�I�u�W�F�N�g�ł����������삷��
	//! @attention �X���b�h����0���w�肵���ꍇ�́C�g�p�\��CPU���������I�Ɏ擾����
	//! 
	//! @param[in]  in         �c ���͉摜
	//! @param[out] out        �c �o�͉摜
	//! @param[in]  max_length �c �`�d�����鋗���̍ő�l
	//! @param[in]  thread_num �c �g�p����X���b�h��
	//! 
	template < class Array1, class Array2 >
	void distance_transform( const Array1 &in, Array2 &out, double max_length = -1.0, typename Array1::size_type thread_num = 0 )
	{
		typedef typename Array2::size_type  size_type;
		typedef typename Array2::value_type value_type;
		typedef __distance_transform_controller__::euclidean_distance_transform_thread< Array2 > euclidean_distance_transform_thread;

		if( thread_num == 0 )
		{
			thread_num = static_cast< size_type >( get_cpu_num( ) );
		}

		out.resize( in.size1( ), in.size2( ), in.size3( ) );
		out.reso1( in.reso1( ) );
		out.reso2( in.reso2( ) );
		out.reso3( in.reso3( ) );

		size_type i;

		for( i = 0 ; i < in.size( ) ; i++ )
		{
			out[ i ] = in[ i ] != 0 ? 1 : 0;
		}

		euclidean_distance_transform_thread *thread = new euclidean_distance_transform_thread[ thread_num ];

		if( in.width( ) > 1 )
		{
			// X�������̏���
			for( i = 0 ; i < thread_num ; i++ )
			{
				thread[ i ].setup_parameters( out, max_length, 0, i, thread_num );
			}

			do_threads( thread, thread_num );
		}

		if( in.height( ) > 1 )
		{
			// Y�������̏���
			for( i = 0 ; i < thread_num ; i++ )
			{
				thread[ i ].setup_parameters( out, max_length, 1, i, thread_num );
			}

			do_threads( thread, thread_num );
		}

		if( in.depth( ) > 1 )
		{
			// Y�������̏���
			for( i = 0 ; i < thread_num ; i++ )
			{
				thread[ i ].setup_parameters( out, max_length, 2, i, thread_num );
			}

			do_threads( thread, thread_num );
		}

		delete [] thread;
	}
}



/// @brief Calvin�ɂ�郆�[�N���b�h�����ϊ�
namespace calvin
{
	inline bool remove_edt( const double uR, const double vR, const double wR, const double ud, const double vd, const double wd )
	{
		const double a = vd - ud;
		const double b = wd - vd;
		const double c = a + b;
		return( c * vR - b * uR - a * wR - a * b * c > 0 );
	}


	template < int DIMENSION >
	struct __access__
	{
		template < class Array >
		inline static typename Array::value_type &at( Array &in, typename Array::size_type _1, typename Array::size_type _2, typename Array::size_type _3 )
		{
			return( in( _1, _2, _3 ) );
		}

		template < class Array >
		inline static typename Array::size_type size1( const Array &in ){ return( in.size1( ) ); }

		template < class Array >
		inline static typename Array::size_type size2( const Array &in ){ return( in.size2( ) ); }

		template < class Array >
		inline static typename Array::size_type size3( const Array &in ){ return( in.size3( ) ); }

		template < class Array >
		inline static double aspect( const Array &in ){ return( 1.0 ); }
	};

	template < >
	struct __access__< 2 >
	{
		template < class Array >
		inline static typename Array::value_type &at( Array &in, typename Array::size_type _1, typename Array::size_type _2, typename Array::size_type _3 )
		{
			return( in( _2, _1, _3 ) );
		}

		template < class Array >
		inline static typename Array::size_type size1( const Array &in ){ return( in.size2( ) ); }

		template < class Array >
		inline static typename Array::size_type size2( const Array &in ){ return( in.size1( ) ); }

		template < class Array >
		inline static typename Array::size_type size3( const Array &in ){ return( in.size3( ) ); }

		template < class Array >
		inline static double aspect( const Array &in ){ return( in.reso2( ) / in.reso1( ) ); }
	};

	template < >
	struct __access__< 3 >
	{
		template < class Array >
		inline static typename Array::value_type &at( Array &in, typename Array::size_type _1, typename Array::size_type _2, typename Array::size_type _3 )
		{
			return( in( _2, _3, _1 ) );
		}

		template < class Array >
		inline static typename Array::size_type size1( const Array &in ){ return( in.size3( ) ); }

		template < class Array >
		inline static typename Array::size_type size2( const Array &in ){ return( in.size1( ) ); }

		template < class Array >
		inline static typename Array::size_type size3( const Array &in ){ return( in.size2( ) ); }

		template < class Array >
		inline static double aspect( const Array &in ){ return( in.reso3( ) / in.reso1( ) ); }
	};

	/// @brief Y,Z�������p�̋����`�d�֐��i1���ȊO�̑S�Ă̎����j
	template < int DIMENSION >
	struct __distance_transform__
	{
		template < class Array >
		static void distance_transform( Array &in, typename Array::size_type thread_id = 0, typename Array::size_type thread_num = 1 )
		{
			typedef typename Array::size_type  size_type;
			typedef typename Array::value_type value_type;
			typedef typename Array::difference_type difference_type;
			typedef __access__< DIMENSION > access;

			size_type i1, i2, i3;

			size_type _1 = access::size1( in );
			size_type _2 = access::size2( in );
			size_type _3 = access::size3( in );

			value_type infinity = type_limits< value_type >::maximum( );

			double as = access::aspect( in );

			double *f = new double[ _1 + 1 ];
			double *g = new double[ _1 + 1 ];
			double *h = new double[ _1 + 1 ];

			for( i3 = 0 ; i3 < _3 ; i3++ )
			{
				for( i2 = thread_id ; i2 < _2 ; i2 += thread_num )
				{
					difference_type l = 0;

					for( i1 = 0 ; i1 < _1 ; i1++ )
					{
						difference_type n = i1 + 1;
						double nd = static_cast< double >( n ) * as;

						f[ n ] = access::at( in, i1, i2, i3 );
						if( f[ n ] == infinity )
						{
							continue;
						}

						if( l < 2 )
						{
							l++;
							g[ l ] = f[ n ];
							h[ l ] = nd;
						}
						else
						{
							while( l >= 2 && remove_edt( g[ l - 1 ], g[ l ], f[ n ], h[ l - 1 ], h[ l ], nd ) )
							{
								l--;
							}
							l++;
							g[ l ] = f[ n ];
							h[ l ] = nd;
						}
					}

					if( l == 0 )
					{
						continue;
					}

					difference_type ns = l;
					l = 1;

					for( i1 = 0 ; i1 < _1 ; i1++ )
					{
						double n = i1 + 1;
						double nd = n * as;
						double len = g[ l ] + ( h[ l ] - nd ) * ( h[ l ] - nd ), len_;
						for( ; l < ns ; l++ )
						{
							len_ = g[ l + 1 ] + ( h[ l + 1 ] - nd ) * ( h[ l + 1 ] - nd );
							if( len > len_ )
							{
								len = len_;
							}
							else
							{
								break;
							}
						}
						access::at( in, i1, i2, i3 ) = static_cast< value_type >( len );
					}
				}
			}

			delete [] f;
			delete [] g;
			delete [] h;
		}
	};


	/// @brief X��������p�̋����`�d�֐�
	template < >
	struct __distance_transform__< 1 >
	{
		template < class Array >
			static void distance_transform( Array &in, typename Array::size_type thread_id = 0, typename Array::size_type thread_num = 1 )
		{
			typedef typename Array::size_type  size_type;
			typedef typename Array::value_type value_type;
			typedef typename Array::difference_type difference_type;

			difference_type i, j, k;
			value_type len;

			const difference_type w = in.width( );
			const difference_type h = in.height( );
			const difference_type d = in.depth( );

			const value_type max = type_limits< value_type >::maximum( );

			for( k = 0 ; k < d ; k++ )
			{
				for( j = thread_id ; j < h ; j += thread_num )
				{
					if( in( 0, j, k ) != 0 )
					{
						len = static_cast< value_type >( w ) < max ? static_cast< value_type >( w ) : max;
						in( 0, j, k ) = len * len;
					}
					else
					{
						len = 0;
					}

					for( i = 1 ; i < w ; i++ )
					{
						if( in( i, j, k ) != 0 )
						{
							len = len + 1 < max ? len + 1 : len;
							in( i, j, k ) = len * len;
						}
						else
						{
							len = 0;
						}
					}

					if( in( w - 1, j, k ) != 0 )
					{
						len = static_cast< value_type >( w ) < max ? static_cast< value_type >( w ) : max;
						in( w - 1, j, k ) = len * len;
					}
					else
					{
						len = 0;
					}

					for( i = w - 2 ; i >= 0 ; i-- )
					{
						if( in( i, j, k ) != 0 )
						{
							len = len + 1 < max ? len + 1 : len;
							in( i, j, k ) = in( i, j, k ) < len * len ? in( i, j, k ) : len * len;
						}
						else
						{
							len = 0;
						}
					}
				}
			}
		}
	};


	/// @brief ���[�N���b�h�����ϊ�
	//! 
	//! @attention ���͂Əo�͂́C����MIST�R���e�i�I�u�W�F�N�g�ł����������삷��
	//! @attention �X���b�h����0���w�肵���ꍇ�́C�g�p�\��CPU���������I�Ɏ擾����
	//! 
	//! @section �Q�l����
	//! - Calvin R. Maurer, Jr., Rensheng Qi, and Vijay Raghavan, "A Linear Time Algorithm for Computing Exact Euclidean Distance Transforms of Binary Images in Arbitrary Dimensions", IEEE Transactions on Pattern Analysis and Machine Intelligence, Vol. 25, No. 2, February 2003
	//! 
	//! @param[in]  in         �c ���͉摜
	//! @param[out] out        �c �o�͉摜
	//! @param[in]  thread_num �c �g�p����X���b�h��
	//! 
	template < class T1, class T2, class Allocator1, class Allocator2 >
	void distance_transform( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out, typename array2< T1, Allocator1 >::size_type thread_num = 0 )
	{
		typedef typename array2< T2, Allocator2 >::size_type  size_type;
		typedef typename array2< T2, Allocator2 >::value_type value_type;
		typedef __distance_transform_controller__::euclidean_distance_transform_thread< array2< T2, Allocator2 > > euclidean_distance_transform_thread;

		if( thread_num == 0 )
		{
			thread_num = static_cast< size_type >( get_cpu_num( ) );
		}

		out.resize( in.size1( ), in.size2( ) );
		out.reso1( in.reso1( ) );
		out.reso2( in.reso2( ) );

		size_type i;

		value_type infinity = type_limits< value_type >::maximum( );
		for( i = 0 ; i < in.size( ) ; i++ )
		{
			out[ i ] = in[ i ] != 0 ? infinity : 0;
		}

		__distance_transform__< 1 >::distance_transform( out, 0, 1 );
		__distance_transform__< 2 >::distance_transform( out, 0, 1 );
	}

	/// @brief ���[�N���b�h�����ϊ�
	//! 
	//! @attention ���͂Əo�͂́C����MIST�R���e�i�I�u�W�F�N�g�ł����������삷��
	//! @attention �X���b�h����0���w�肵���ꍇ�́C�g�p�\��CPU���������I�Ɏ擾����
	//! 
	//! @section �Q�l����
	//! - Calvin R. Maurer, Jr., Rensheng Qi, and Vijay Raghavan, "A Linear Time Algorithm for Computing Exact Euclidean Distance Transforms of Binary Images in Arbitrary Dimensions", IEEE Transactions on Pattern Analysis and Machine Intelligence, Vol. 25, No. 2, February 2003
	//! 
	//! @param[in]  in         �c ���͉摜
	//! @param[out] out        �c �o�͉摜
	//! @param[in]  thread_num �c �g�p����X���b�h��
	//! 
	template < class T1, class T2, class Allocator1, class Allocator2 >
	void distance_transform( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out, typename array3< T1, Allocator1 >::size_type thread_num = 0 )
	{
		typedef typename array3< T2, Allocator2 >::size_type  size_type;
		typedef typename array3< T2, Allocator2 >::value_type value_type;
		typedef __distance_transform_controller__::euclidean_distance_transform_thread< array3< T2, Allocator2 > > euclidean_distance_transform_thread;

		if( thread_num == 0 )
		{
			thread_num = static_cast< size_type >( get_cpu_num( ) );
		}

		out.resize( in.size1( ), in.size2( ), in.size3( ) );
		out.reso1( in.reso1( ) );
		out.reso2( in.reso2( ) );
		out.reso3( in.reso3( ) );

		size_type i;

		value_type infinity = type_limits< value_type >::maximum( );
		for( i = 0 ; i < in.size( ) ; i++ )
		{
			out[ i ] = in[ i ] != 0 ? infinity : 0;
		}

		__distance_transform__< 1 >::distance_transform( out, 0, 1 );
		__distance_transform__< 2 >::distance_transform( out, 0, 1 );
		__distance_transform__< 3 >::distance_transform( out, 0, 1 );
	}
}


/// @}
//  ���[�N���b�h�����ϊ��O���[�v�̏I���

/// @}
//  �����ϊ��O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_DISTANCE_TRANSFORM__
