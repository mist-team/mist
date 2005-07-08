/// @file mist/filter/thinning.h
//!
//! @brief �e�����̉摜�ɑΉ������C�א����A���S���Y��
//!
//! - �Q�l����
//!   - ���e����Y, ``�摜�����̂��߂̃f�B�W�^���摜�����kI�l�kII�l�C'' ���W���C1988
//!   - ��ؒq, �����\��, ``�����ϊ��̌��ʂ𗘗p������l�摜�̒����א���,'' �d�q���ʐM�w��_����D, vol.68-D, no.4, pp.473-480, 1985
//!
#ifndef __INCLUDE_MIST_THINNING__
#define __INCLUDE_MIST_THINNING__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#ifndef __INCLUDE_MIST_LIMITS__
#include "../limits.h"
#endif

#ifndef __INCLUDE_MIST_MATRIX__
#include "../matrix.h"
#endif

#ifndef __INCLUDE_MIST_LIMITS__
#include "distance.h"
#endif

#include <deque>

// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


namespace __thinning_controller__
{
	template < class T, class Allocator >
		inline void val9( const array2< T, Allocator > &in, typename array2< T, Allocator >::value_type p[ 9 ],
		typename array2< T, Allocator >::size_type i, typename array2< T, Allocator >::size_type j )
	{
		p[ 0 ] = in( i , j );
		p[ 1 ] = in( i + 1, j );
		p[ 2 ] = in( i + 1, j - 1 );
		p[ 3 ] = in( i , j - 1 );
		p[ 4 ] = in( i - 1, j - 1 );
		p[ 5 ] = in( i - 1, j );
		p[ 6 ] = in( i - 1, j + 1 );
		p[ 7 ] = in( i , j + 1 );
		p[ 8 ] = in( i + 1, j + 1 );
	}

	template < class T > inline T pixel( T p )
	{
		return ( ( p > 0 ) ? 1 : 0 );
	}

	template < class T > inline T pixel( T p[ 9 ], int index )
	{
		if( index <= 0 )
		{
			return ( pixel( p[ index + 8 ] ) );
		}
		else if( index >= 9 )
		{
			return ( pixel( p[ index - 8 ] ) );
		}
		else
		{
			return ( pixel( p[ index ] ) );
		}
	}

	template < class T > inline T pixel_( T p[ 9 ], int index )
	{
		return ( 1 - pixel( p, index ) );
	}

	template < class T > inline int Nc4( T p[ 9 ] )
	{
		T ret = 0;

		for( int i = 1 ; i < 9 ; i += 2 )
		{
			ret += pixel( p, i ) - pixel( p, i ) * pixel( p, i + 1 ) * pixel( p, i + 2 );
		}

		return ( ret );
	}

	template < class T > inline int Nc8( T p[ 9 ] )
	{
		T ret = 0;

		for( int i = 1 ; i < 9 ; i += 2 )
		{
			ret += pixel_( p, i ) - pixel_( p, i ) * pixel_( p, i + 1 ) * pixel_( p, i + 2 );
		}

		return ( ret );
	}

	template < class T, class Allocator >
		void thinning( array2< T, Allocator > &ia )
	{
		typedef typename array2< T, Allocator >::size_type size_type;
		typedef typename array2< T, Allocator >::value_type value_type;

		bool flag;
		size_type w = ia.width( );
		size_type h = ia.height( );
		size_type i, j;
		char p[ 9 ], q[ 9 ], value;
		array2< char > ic( w, h );
		array2< char > id( w, h );

		for( i = 0 ; i < ia.size( ) ; i++ )
		{
			id[ i ] = ia[ i ] == 0 ? 0 : 1;
		}

		flag = true;

		while( flag )
		{
			flag = false;
			ic = id;

			for( j = 1 ; j < h - 1 ; j++ )
			{
				for( i = 1 ; i < w - 1 ; i++ )
				{
					val9( ic, p, i, j );
					val9( id, q, i, j );

					if( pixel( p[ 0 ] ) != 1 )
					{
						continue;
					}

					// ����1 = ���͉摜F�ɂ����āCx0��4�ߖT�ɏ��Ȃ��Ƃ�1��0��f�����݂���
					if( p[ 1 ] == 1 && p[ 3 ] == 1 && p[ 5 ] == 1 && p[ 7 ] == 1 )
					{
						continue;
					}

					// ����2 = ���͉摜F�ɂ����āCx0��8�ߖT��2�ȏ��1��f������(x0���[�_�łȂ�)
					value = pixel( p[ 1 ] ) + pixel( p[ 2 ] ) + pixel( p[ 3 ] ) + pixel( p[ 4 ] ) + pixel( p[ 5 ] ) + pixel( p[ 6 ] ) + pixel( p[ 7 ] ) + pixel( p[ 8 ] );

					if( value <= 1 )
					{
						continue;
					}

					// ����3 = ��Ɖ摜F'�ɂ����āC-1�̉�f��0��f�Ƃ݂Ȃ������Cx0���Ǘ��_�ł͂Ȃ�
					value = pixel( q[ 1 ] ) + pixel( q[ 2 ] ) + pixel( q[ 3 ] ) + pixel( q[ 4 ] ) + pixel( q[ 5 ] ) + pixel( q[ 6 ] ) + pixel( q[ 7 ] ) + pixel( q[ 8 ] );

					if( value == 0 )
					{
						continue;
					}

					// ����4 = ���͉摜F�ɂ����āCx0�ɂ�����Nc[8]�̘A������1�ł���
					if( Nc8( p ) != 1 )
					{
						continue;
					}

					// ����5 = i=3,5�ɑ΂��Ď��̎��������藧��
					//         xi�����傤�ǂ��̔�����1����0�ɕς����Ă͂��Ȃ�(xi��-1�ɂȂ��Ă��Ȃ�)���C
					//         �܂��́C���ꂪ-1�ɂȂ��Ă��Ă��C-1��S��0�Ƃ݂Ȃ��Čv�Z����x0�̘A����Nc[8]��1�ɓ�����
					if( q[ 3 ] == -1 || q[ 5 ] == -1 )
					{
						if( Nc8( q ) != 1 )
						{
							continue;
						}
					}

					// �S�Ă̏����𖞂�������f��-1�ɂ���
					id( i, j ) = -1;

					flag = true;
				}
			}

			for( i = 0 ; i < id.size( ) ; i++ )
			{
				id[ i ] = id[ i ] == -1 ? 0 : id[ i ];
			}
		}

		for( i = 0 ; i < ia.size( ) ; i++ )
		{
			ia[ i ] = id[ i ] > 0 ? 1 : 0;
		}
	}
}


//! @addtogroup thinning_group �א���
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/filter/thinning.h>
//! @endcode
//!
//!  @{


/// @brief 2�����摜�ɑ΂���א����A���S���Y��
//!
//! �א������ʂ�8�A���ƂȂ�
//!
//! @param[in]  in  �c ���͉摜
//! @param[out] out �c �o�͉摜
//!
template < class T1, class T2, class Allocator1, class Allocator2 >
void thinning( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out )
{
	typedef typename array2< T2, Allocator2 >::size_type size_type;
	typedef typename array2< T2, Allocator2 >::value_type value_type;

	out.resize( in.size1( ), in.size2( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );

	for( size_type i = 0 ; i < in.size( ) ; i++ )
	{
		out[ i ] = in[ i ] > 0 ? 1 : 0;
	}

	__thinning_controller__::thinning( out );
}


namespace euclidean
{
	namespace __utility__
	{
		template < class T, class Allocator >
			inline void val9_1( const array2< T, Allocator > &in, typename array2< T, Allocator >::value_type p[ 9 ],
			typename array2< T, Allocator >::size_type i, typename array2< T, Allocator >::size_type j )
		{
			p[ 0 ] = in( i    , j     );
			p[ 1 ] = in( i + 1, j     );
			p[ 2 ] = in( i + 1, j - 1 );
			p[ 3 ] = in( i    , j - 1 );
			p[ 4 ] = in( i - 1, j - 1 );
			p[ 5 ] = in( i - 1, j     );
			p[ 6 ] = in( i - 1, j + 1 );
			p[ 7 ] = in( i    , j + 1 );
			p[ 8 ] = in( i + 1, j + 1 );
		}

		template < class T, class Allocator >
			inline void val9_2( const array2< T, Allocator > &in, typename array2< T, Allocator >::value_type p[ 9 ],
			typename array2< T, Allocator >::size_type i, typename array2< T, Allocator >::size_type j )
		{
			p[ 0 ] = in( i    , j     );
			p[ 1 ] = in( i - 1, j     );
			p[ 2 ] = in( i - 1, j + 1 );
			p[ 3 ] = in( i    , j + 1 );
			p[ 4 ] = in( i + 1, j + 1 );
			p[ 5 ] = in( i + 1, j     );
			p[ 6 ] = in( i + 1, j - 1 );
			p[ 7 ] = in( i    , j - 1 );
			p[ 8 ] = in( i - 1, j - 1 );
		}

		template < class T, class Allocator >
			inline void val9_3( const array2< T, Allocator > &in, typename array2< T, Allocator >::value_type p[ 9 ],
			typename array2< T, Allocator >::size_type i, typename array2< T, Allocator >::size_type j )
		{
			p[ 0 ] = in( i    , j     );
			p[ 1 ] = in( i + 1, j     );
			p[ 2 ] = in( i + 1, j + 1 );
			p[ 3 ] = in( i    , j + 1 );
			p[ 4 ] = in( i - 1, j + 1 );
			p[ 5 ] = in( i - 1, j     );
			p[ 6 ] = in( i - 1, j - 1 );
			p[ 7 ] = in( i    , j - 1 );
			p[ 8 ] = in( i + 1, j - 1 );
		}

		template < class T > inline T max_pixel( T p[ 9 ] )
		{
			T max = 0;

			for( int i = 1 ; i < 9 ; i++ )
			{
				if( std::abs( p[ i ] ) > max )
				{
					max = std::abs( p[ i ] );
				}
			}

			return ( max );
		}

		template < class T > inline T plus_pixel_num( T p[ 9 ] )
		{
			int num = 0;

			for( int i = 1 ; i < 9 ; i++ )
			{
				if( p[ i ] > 0 )
				{
					num++;
				}
			}

			return ( num );
		}

		template < class T > inline T pixel( T p )
		{
			return ( ( p > 0 ) ? 1 : 0 );
		}

		template < class T > inline T pixel( T p[ 9 ], int index )
		{
			if( index <= 0 )
			{
				return ( pixel( p[ index + 8 ] ) );
			}
			else if( index >= 9 )
			{
				return ( pixel( p[ index - 8 ] ) );
			}
			else
			{
				return ( pixel( p[ index ] ) );
			}
		}

		template < class T > inline T pixel_( T p[ 9 ], int index )
		{
			return ( 1 - pixel( p, index ) );
		}

		template < class T > inline int Nc4( T p[ 9 ] )
		{
			T ret = 0;

			for( int i = 1 ; i < 9 ; i += 2 )
			{
				ret += pixel( p, i ) - pixel( p, i ) * pixel( p, i + 1 ) * pixel( p, i + 2 );
			}

			return ( ret );
		}

		template < class T > inline T Nc8( T p[ 9 ] )
		{
			T ret = 0;

			for( int i = 1 ; i < 9 ; i += 2 )
			{
				ret += pixel_( p, i ) - pixel_( p, i ) * pixel_( p, i + 1 ) * pixel_( p, i + 2 );
			}

			return ( ret );
		}

		/// @brief ���[�N���b�h������p�����א����̎��� by �т���
		//!
		//! - �Q�l����
		//!   - ��ؒq, �����\��, ``�����ϊ��̌��ʂ𗘗p������l�摜�̒����א���,'' �d�q���ʐM�w��_����D, vol.68-D, no.4, pp.473-480, 1985.
		//!
		template < class T, class Allocator >
			void thinning( array2< T, Allocator > &ia )
		{
			typedef typename array2< T, Allocator >::size_type size_type;
			typedef typename array2< T, Allocator >::value_type value_type;

			size_type w = ia.width( );
			size_type h = ia.height( );
			size_type i, j;
			double p[ 9 ], q[ 9 ];
			array2< double > ic( w, h, ia.reso1( ), ia.reso2( ) );
			array2< double > id( w, h, ia.reso1( ), ia.reso2( ) );

			for( i = 0 ; i < ia.size( ) ; i++ )
			{
				id[ i ] = ia[ i ] == 0 ? 0 : 1;
			}

			//�����ϊ�
			euclidean::distance_transform( id, id );

			//�ׂߏ����@1���
			ic = id;
			for( j = 1 ; j < h - 1 ; j++ )
			{
				for( i = 1 ; i < w - 1 ; i++ )
				{
					val9_1( ic, p, i, j );
					val9_1( id, q, i, j );

					if( q[ 0 ] <= 0 )
					{
						continue;
					}

					if( p[ 3 ] > 0 && p[ 5 ] > 0 )
					{
						continue;
					}

					if( Nc8( p ) != 1 )
					{
						continue;
					}

					if( max_pixel( p ) <= std::abs( q[ 0 ] ) )
					{
						continue;
					}

					if( ( q[ 0 ] > q[ 1 ] && q[ 1 ] > 0 ) && ( ( p[ 2 ] > 0 ) || ( p[ 3 ] > 0 ) ) && ( ( q[ 7 ] > 0 ) || ( q[ 8 ] > 0 ) ) )
					{
						continue;
					}

					if( ( q[ 0 ] > q[ 7 ] && q[ 7 ] > 0 ) && ( ( p[ 5 ] > 0 ) || ( q[ 6 ] > 0 ) ) && ( ( q[ 8 ] > 0 ) || ( q[ 1 ] > 0 ) ) )
					{
						continue;
					}

					ic( i, j ) = - id( i, j );
				}
			}

			//�ׂߏ����@2���
			id = ic;
			for( j = h - 2 ; j > 0 ; j-- )
			{
				for( i = w - 2 ; i > 0 ; i-- )
				{
					val9_2( id, p, i, j );
					val9_2( ic, q, i, j );

					if( q[ 0 ] <= 0 )
					{
						continue;
					}

					if( p[ 3 ] > 0 && p[ 5 ] > 0 )
					{
						continue;
					}

					val9_1( id, p, i, j );

					if( Nc8( p ) != 1 )
					{
						continue;
					}

					val9_2( id, p, i, j );

					if( max_pixel( p ) <= std::abs( q[ 0 ] ) )
					{
						continue;
					}

					if( ( q[ 0 ] > q[ 1 ] && q[ 1 ] > 0 ) && ( ( p[ 2 ] > 0 ) || ( p[ 3 ] > 0 ) ) && ( ( q[ 7 ] > 0 ) || ( q[ 8 ] > 0 ) ) )
					{
						continue;
					}

					if( ( q[ 0 ] > q[ 7 ] && q[ 7 ] > 0 ) && ( ( p[ 5 ] > 0 ) || ( q[ 6 ] >= q[ 0 ] ) ) && ( ( q[ 8 ] > 0 ) || ( q[ 1 ] > 0 ) ) )
					{
						continue;
					}

					id( i, j ) = - ic( i, j );
				}
			}

			//�ׂߏ����@3���
			ic = id;
			for( j = h - 2 ; j > 0 ; j-- )
			{
				for( i = 1 ; i < w - 1 ; i++ )
				{
					val9_3( ic, p, i, j );
					val9_3( id, q, i, j );

					if( q[ 0 ] <= 0 )
					{
						continue;
					}

					if( p[ 3 ] > 0 && p[ 5 ] > 0 )
					{
						continue;
					}

					val9_1( ic, p, i, j );

					if( Nc8( p ) != 1 )
					{
						continue;
					}

					val9_3( ic, p, i, j );

					if( max_pixel( p ) <= std::abs( q[ 0 ] ) )
					{
						continue;
					}

					ic( i, j ) = - id( i, j );
				}
			}

			//�㏈��
			id = ic;
			for( j = 1 ; j < h - 1 ; j++ )
			{
				for( i = 1 ; i < w - 1 ; i++ )
				{
					val9_1( id, p, i, j );
					val9_1( ic, q, i, j );

					if( q[ 0 ] <= 0 )
					{
						id( i, j ) = 0;
						continue;
					}

					if( ( p[ 1 ] > 0 ) && ( p[ 3 ] > 0 ) && ( p[ 5 ] > 0 ) && ( p[ 7 ] > 0 ) )
					{
						continue;
					}

					if( Nc8( p ) != 1 )
					{
						continue;
					}

					if( plus_pixel_num( p ) < 2 )
					{
						continue;
					}

					if( plus_pixel_num( p ) == 2 && ( q[ 4 ] > 0 && p[ 4 ] == 0 ) )
					{
						p[ 4 ] = 1;

						if( Nc8( p ) != 1 )
						{
							continue;
						}
					}

					id( i, j ) = 0;
				}
			}

			for( i = 0 ; i < ia.size( ) ; i++ )
			{
				ia[ i ] = id[ i ] != 0 ? 1 : 0;
			}
		}

		//3�����摜�̍א���
		template < size_t Nc >
		struct neighbor
		{
			typedef size_t size_type;			///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
			typedef ptrdiff_t difference_type;	///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���

			template < class T >
				static T P1( T p[ 4 ][ 9 ], difference_type i, difference_type j )
			{
				if( j >= 9 )
				{
					return( p[ i ][ j - 8 ] );
				}
				else
				{
					return( p[ i ][ j ] );
				}
			}

			template < class T >
				static T P2( T p[ 4 ][ 9 ], difference_type i, difference_type j )
			{
				if( j >= 9 )
				{
					return( p[ i ][ j - 8 ] );
				}
				else if( j <= 0 )
				{
					return( p[ i ][ j + 8 ] );
				}
				else
				{
					return( p[ i ][ j ] );
				}
			}

			//�A���� 6�A��
			template < class T >
			static difference_type Nc6( T p[ 3 ][ 9 ] )
			{
				static size_type S0[] = { 1, 3 };
				static size_type S1[] = { 1, 3, 5, 7 };

				difference_type ret = 0;

				for( size_type h = 0 ; h < 2 ; h++ )
				{
					difference_type tmp = 0;
					for( size_type k = 0 ; k < 4 ; k++ )
					{
						tmp += P1( p, S0[ h ], S1[ k ] ) * P1( p, 2, S1[ k ] );
					}
					ret += P1( p, S0[ h ], 0 ) * ( 1 - tmp );
				}

				for( size_type k = 0 ; k < 4 ; k++ )
				{
					difference_type tmp = 0;
					for( size_type h = 0 ; h < 2 ; h++ )
					{
						tmp += P1( p, S0[ h ], 0 ) * P1( p, S0[ h ], S1[ k ] ) * P1( p, S0[ h ], S1[ k ] + 1 ) * P1( p, S0[ h ], S1[ k ] + 2 );
					}
					ret += P1( p, 2, S1[ k ] ) * ( 1 - P1( p, 2, S1[ k ] + 1 ) * P1( p, 2, S1[ k ] + 2 ) * ( 1 - tmp ) );
				}

				return( ret );
			}

			//�A���� 6�A��
			template < class T >
			static difference_type Sx( T p[ 3 ][ 9 ] )
			{
				static size_type S0[] = { 1, 3 };
				static size_type S1[] = { 1, 3, 5, 7 };

				difference_type sx = 0;

				for( size_type k = 0 ; k < 4 ; k++ )
				{
					difference_type tmp = 0;
					for( size_type h = 0 ; h < 2 ; h++ )
					{
						sx += ( 1 - P1( p, S0[ h ], S1[ k ] + 1 ) ) * P1( p, S0[ h ], S1[ k ] ) * P1( p, S0[ h ], S1[ k ] + 2 ) * P1( p, S0[ h ], 0 ) * P1( p, 2, S1[ k ] ) * P1( p, 2, S1[ k ] + 1 ) * P1( p, 2, S1[ k ] + 2 );
					}
				}

				return( sx );
			}


			template < class Array >
			static bool is_deletable( const Array &in, size_type i, size_type j, size_type k )
			{
				int p[ 4 ][ 9 ];

				// �ߖT�����擾
				p[ 1 ][ 0 ] = in( i    , j    , k - 1 ) > 0 ? 1 : 0;
				p[ 1 ][ 1 ] = in( i    , j + 1, k - 1 ) > 0 ? 1 : 0;
				p[ 1 ][ 2 ] = in( i - 1, j + 1, k - 1 ) > 0 ? 1 : 0;
				p[ 1 ][ 3 ] = in( i - 1, j    , k - 1 ) > 0 ? 1 : 0;
				p[ 1 ][ 4 ] = in( i - 1, j - 1, k - 1 ) > 0 ? 1 : 0;
				p[ 1 ][ 5 ] = in( i    , j - 1, k - 1 ) > 0 ? 1 : 0;
				p[ 1 ][ 6 ] = in( i + 1, j - 1, k - 1 ) > 0 ? 1 : 0;
				p[ 1 ][ 7 ] = in( i + 1, j    , k - 1 ) > 0 ? 1 : 0;
				p[ 1 ][ 8 ] = in( i + 1, j + 1, k - 1 ) > 0 ? 1 : 0;

				p[ 2 ][ 0 ] = in( i    , j    , k     ) > 0 ? 1 : 0;
				p[ 2 ][ 1 ] = in( i    , j + 1, k     ) > 0 ? 1 : 0;
				p[ 2 ][ 2 ] = in( i - 1, j + 1, k     ) > 0 ? 1 : 0;
				p[ 2 ][ 3 ] = in( i - 1, j    , k     ) > 0 ? 1 : 0;
				p[ 2 ][ 4 ] = in( i - 1, j - 1, k     ) > 0 ? 1 : 0;
				p[ 2 ][ 5 ] = in( i    , j - 1, k     ) > 0 ? 1 : 0;
				p[ 2 ][ 6 ] = in( i + 1, j - 1, k     ) > 0 ? 1 : 0;
				p[ 2 ][ 7 ] = in( i + 1, j    , k     ) > 0 ? 1 : 0;
				p[ 2 ][ 8 ] = in( i + 1, j + 1, k     ) > 0 ? 1 : 0;

				p[ 3 ][ 0 ] = in( i    , j    , k + 1 ) > 0 ? 1 : 0;
				p[ 3 ][ 1 ] = in( i    , j + 1, k + 1 ) > 0 ? 1 : 0;
				p[ 3 ][ 2 ] = in( i - 1, j + 1, k + 1 ) > 0 ? 1 : 0;
				p[ 3 ][ 3 ] = in( i - 1, j    , k + 1 ) > 0 ? 1 : 0;
				p[ 3 ][ 4 ] = in( i - 1, j - 1, k + 1 ) > 0 ? 1 : 0;
				p[ 3 ][ 5 ] = in( i    , j - 1, k + 1 ) > 0 ? 1 : 0;
				p[ 3 ][ 6 ] = in( i + 1, j - 1, k + 1 ) > 0 ? 1 : 0;
				p[ 3 ][ 7 ] = in( i + 1, j    , k + 1 ) > 0 ? 1 : 0;
				p[ 3 ][ 8 ] = in( i + 1, j + 1, k + 1 ) > 0 ? 1 : 0;


				//�P�D�A����
				if( Nc6( p ) != 1 )
				{
					return( false );
				}

				//�Q�D6�ߖT�ɂ���1-�v�f�̌�
				difference_type num = p[ 1 ][ 0 ] + p[ 2 ][ 1 ] + p[ 2 ][ 3 ] + p[ 2 ][ 5 ] + p[ 2 ][ 7 ] + p[ 3 ][ 0 ];
				if( num <= 3 )
				{
					return( true );
				}
				//�R�D
				else if( num == 4 )
				{
					if( Sx( p ) == 1 )
					{
						return( false );
					}
					else
					{
						return( true );
					}
				}
				//�S�D
				else if( num == 5 )
				{
					if( Sx( p ) == 1 )
					{
						return( false );
					}
					else
					{
						//�ˉe�O���t�ɌǗ��_�����邩
						if( p[ 2 ][ 0 ] == 0 )
						{
							return( false );
						}

						static size_type S1[] = { 1, 3, 5, 7 };
						difference_type hole = 0;
						if( p[ 1 ][ 0 ] != 0 )
						{
							for( size_type h = 0 ; h < 4 ; h++ )
							{
								hole += p[ 1 ][ S1[ h ] ] * p[ 2 ][ S1[ h ] ];
							}
							if( hole == 0 )
							{
								return( false );
							}
						}

						hole = 0;
						if( p[ 3 ][ 0 ] != 0 )
						{
							for( size_type h = 0 ; h < 4 ; h++ )
							{
								hole += p[ 3 ][ S1[ h ] ] * p[ 2 ][ S1[ h ] ];
							}
							if( hole == 0 )
							{
								return( false );
							}
						}

						for( size_type h = 0 ; h < 4 ; h++ )
						{
							hole = 0;
							if( p[ 2 ][ S1[ h ] ] != 0 )
							{
								hole = p[ 1 ][ 0 ] * p[ 1 ][ S1[ h ] ] + p[ 3 ][ 0 ] * p[ 3 ][ S1[ h ] ] + P2( p, 2, S1[ h ] - 1 ) * P2( p, 2, S1[ h ] - 2 ) + P2( p, 2, S1[ h ] + 1 ) * P2( p, 2, S1[ h ] + 2 );
								if( hole == 0 )
								{
									return( false );
								}
							}
						}
						return( true );
					}
				}
				//�T�D
				else if( num == 6 )
				{
					typedef matrix< difference_type > matrix_type;

					//�אڍs��
					matrix_type M( 6, 6 );

					M( 0, 0 ) = 0          ; M( 0, 1 ) = p[ 1 ][ 1 ]; M( 0, 2 ) = p[ 1 ][ 3 ]; M( 0, 3 ) = p[ 1 ][ 7 ]; M( 0, 4 ) = p[ 1 ][ 5 ]; M( 0, 5 ) = 0;
					M( 1, 0 ) = p[ 1 ][ 1 ]; M( 1, 1 ) = 0          ; M( 1, 2 ) = p[ 2 ][ 2 ]; M( 1, 3 ) = p[ 2 ][ 8 ]; M( 1, 4 ) = 0          ; M( 1, 5 ) = p[ 3 ][ 1 ];
					M( 2, 0 ) = p[ 1 ][ 3 ]; M( 2, 1 ) = p[ 2 ][ 2 ]; M( 2, 2 ) = 0          ; M( 2, 3 ) = 0          ; M( 2, 4 ) = p[ 2 ][ 4 ]; M( 2, 5 ) = p[ 3 ][ 3 ];
					M( 3, 0 ) = p[ 1 ][ 7 ]; M( 3, 1 ) = p[ 2 ][ 8 ]; M( 3, 2 ) = 0          ; M( 3, 3 ) = 0          ; M( 3, 4 ) = p[ 2 ][ 6 ]; M( 3, 5 ) = p[ 3 ][ 7 ];
					M( 4, 0 ) = p[ 1 ][ 5 ]; M( 4, 1 ) = 0          ; M( 4, 2 ) = p[ 2 ][ 4 ]; M( 4, 3 ) = p[ 2 ][ 6 ]; M( 4, 4 ) = 0          ; M( 4, 5 ) = p[ 3 ][ 5 ];
					M( 5, 0 ) = 0          ; M( 5, 1 ) = p[ 3 ][ 1 ]; M( 5, 2 ) = p[ 3 ][ 3 ]; M( 5, 3 ) = p[ 3 ][ 7 ]; M( 5, 4 ) = p[ 3 ][ 5 ]; M( 5, 5 ) = 0;

					matrix_type I = matrix_type::identity( 6, 6 );
					matrix_type N = M * ( I + M * ( I + M * ( I + M * ( I + M ) ) ));

					for( size_type r = 0 ; r < N.size( ) ; r++ )
					{
						if( N[ r ] == 0 )
						{
							return( false );
						}
					}
					return( true );
				}
				else
				{
					std::cout << "Error" << std::endl;
				}

				return( true );
			}
		};

		template < >
		struct neighbor< 26 >
		{
			typedef size_t size_type;			///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
			typedef ptrdiff_t difference_type;	///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���

			template < class T >
			static T P1( T p[ 4 ][ 9 ], difference_type i, difference_type j )
			{
				if( j >= 9 )
				{
					return( p[ i ][ j - 8 ] );
				}
				else
				{
					return( p[ i ][ j ] );
				}
			}

			template < class T >
			static T P2( T p[ 4 ][ 9 ], difference_type i, difference_type j )
			{
				if( j >= 9 )
				{
					return( p[ i ][ j - 8 ] );
				}
				else if( j <= 0 )
				{
					return( p[ i ][ j + 8 ] );
				}
				else
				{
					return( p[ i ][ j ] );
				}
			}

			template < class T >
			static T P1_( T p[ 4 ][ 9 ], difference_type i, difference_type j )
			{
				if( j >= 9 )
				{
					return( 1 - p[ i ][ j - 8 ] );
				}
				else
				{
					return( 1 - p[ i ][ j ] );
				}
			}

			template < class T >
			static T P2_( T p[ 4 ][ 9 ], difference_type i, difference_type j )
			{
				if( j >= 9 )
				{
					return( 1 - p[ i ][ j - 8 ] );
				}
				else if( j <= 0 )
				{
					return( 1 - p[ i ][ j + 8 ] );
				}
				else
				{
					return( 1 - p[ i ][ j ] );
				}
			}

			//�A���� 26�A��
			template < class T >
			static difference_type Nc26( T p[ 3 ][ 9 ] )
			{
				static size_type S0[] = { 1, 3 };
				static size_type S1[] = { 1, 3, 5, 7 };

				difference_type ret = 0;

				for( size_type h = 0 ; h < 2 ; h++ )
				{
					difference_type tmp = 0;
					for( size_type k = 0 ; k < 4 ; k++ )
					{
						tmp += P1_( p, S0[ h ], S1[ k ] ) * P1_( p, 2, S1[ k ] );
					}
					ret += P1_( p, S0[ h ], 0 ) * ( 1 - tmp );
				}

				for( size_type k = 0 ; k < 4 ; k++ )
				{
					difference_type tmp = 0;
					for( size_type h = 0 ; h < 2 ; h++ )
					{
						tmp += P1_( p, S0[ h ], 0 ) * P1_( p, S0[ h ], S1[ k ] ) * P1_( p, S0[ h ], S1[ k ] + 1 ) * P1_( p, S0[ h ], S1[ k ] + 2 );
					}
					ret += P1_( p, 2, S1[ k ] ) * ( 1 - P1_( p, 2, S1[ k ] + 1 ) * P1_( p, 2, S1[ k ] + 2 ) * ( 1 - tmp ) );
				}

				ret = 2 - ret;

				return( ret );
			}

			//
			template < class T >
			static difference_type Sx( T p[ 3 ][ 9 ] )
			{
				static size_type S0[] = { 1, 3 };
				static size_type S1[] = { 1, 3, 5, 7 };

				difference_type sx = 0;

				for( size_type k = 0 ; k < 4 ; k++ )
				{
					difference_type tmp = 0;
					for( size_type h = 0 ; h < 2 ; h++ )
					{
						sx += ( 1 - P1( p, S0[ h ], S1[ k ] + 1 ) ) * P1( p, S0[ h ], S1[ k ] ) * P1( p, S0[ h ], S1[ k ] + 2 ) * P1( p, S0[ h ], 0 ) * P1( p, 2, S1[ k ] ) * P1( p, 2, S1[ k ] + 1 ) * P1( p, 2, S1[ k ] + 2 );
					}
				}

				return( sx );
			}


			template < class Array >
			static bool is_deletable( const Array &in, size_type i, size_type j, size_type k )
			{
				int p[ 4 ][ 9 ];

				// �ߖT�����擾
				p[ 1 ][ 0 ] = in( i    , j    , k - 1 ) > 0 ? 1 : 0;
				p[ 1 ][ 1 ] = in( i    , j + 1, k - 1 ) > 0 ? 1 : 0;
				p[ 1 ][ 2 ] = in( i - 1, j + 1, k - 1 ) > 0 ? 1 : 0;
				p[ 1 ][ 3 ] = in( i - 1, j    , k - 1 ) > 0 ? 1 : 0;
				p[ 1 ][ 4 ] = in( i - 1, j - 1, k - 1 ) > 0 ? 1 : 0;
				p[ 1 ][ 5 ] = in( i    , j - 1, k - 1 ) > 0 ? 1 : 0;
				p[ 1 ][ 6 ] = in( i + 1, j - 1, k - 1 ) > 0 ? 1 : 0;
				p[ 1 ][ 7 ] = in( i + 1, j    , k - 1 ) > 0 ? 1 : 0;
				p[ 1 ][ 8 ] = in( i + 1, j + 1, k - 1 ) > 0 ? 1 : 0;

				p[ 2 ][ 0 ] = in( i    , j    , k     ) > 0 ? 1 : 0;
				p[ 2 ][ 1 ] = in( i    , j + 1, k     ) > 0 ? 1 : 0;
				p[ 2 ][ 2 ] = in( i - 1, j + 1, k     ) > 0 ? 1 : 0;
				p[ 2 ][ 3 ] = in( i - 1, j    , k     ) > 0 ? 1 : 0;
				p[ 2 ][ 4 ] = in( i - 1, j - 1, k     ) > 0 ? 1 : 0;
				p[ 2 ][ 5 ] = in( i    , j - 1, k     ) > 0 ? 1 : 0;
				p[ 2 ][ 6 ] = in( i + 1, j - 1, k     ) > 0 ? 1 : 0;
				p[ 2 ][ 7 ] = in( i + 1, j    , k     ) > 0 ? 1 : 0;
				p[ 2 ][ 8 ] = in( i + 1, j + 1, k     ) > 0 ? 1 : 0;

				p[ 3 ][ 0 ] = in( i    , j    , k + 1 ) > 0 ? 1 : 0;
				p[ 3 ][ 1 ] = in( i    , j + 1, k + 1 ) > 0 ? 1 : 0;
				p[ 3 ][ 2 ] = in( i - 1, j + 1, k + 1 ) > 0 ? 1 : 0;
				p[ 3 ][ 3 ] = in( i - 1, j    , k + 1 ) > 0 ? 1 : 0;
				p[ 3 ][ 4 ] = in( i - 1, j - 1, k + 1 ) > 0 ? 1 : 0;
				p[ 3 ][ 5 ] = in( i    , j - 1, k + 1 ) > 0 ? 1 : 0;
				p[ 3 ][ 6 ] = in( i + 1, j - 1, k + 1 ) > 0 ? 1 : 0;
				p[ 3 ][ 7 ] = in( i + 1, j    , k + 1 ) > 0 ? 1 : 0;
				p[ 3 ][ 8 ] = in( i + 1, j + 1, k + 1 ) > 0 ? 1 : 0;


				//�P�D�A����
				if( Nc26( p ) != 1 )
				{
					return( false );
				}

				// �ߖT�����擾 0-1���]
				p[ 1 ][ 0 ] = in( i    , j    , k - 1 ) > 0 ? 0 : 1;
				p[ 1 ][ 1 ] = in( i    , j + 1, k - 1 ) > 0 ? 0 : 1;
				p[ 1 ][ 2 ] = in( i - 1, j + 1, k - 1 ) > 0 ? 0 : 1;
				p[ 1 ][ 3 ] = in( i - 1, j    , k - 1 ) > 0 ? 0 : 1;
				p[ 1 ][ 4 ] = in( i - 1, j - 1, k - 1 ) > 0 ? 0 : 1;
				p[ 1 ][ 5 ] = in( i    , j - 1, k - 1 ) > 0 ? 0 : 1;
				p[ 1 ][ 6 ] = in( i + 1, j - 1, k - 1 ) > 0 ? 0 : 1;
				p[ 1 ][ 7 ] = in( i + 1, j    , k - 1 ) > 0 ? 0 : 1;
				p[ 1 ][ 8 ] = in( i + 1, j + 1, k - 1 ) > 0 ? 0 : 1;

				p[ 2 ][ 0 ] = in( i    , j    , k     ) > 0 ? 1 : 0;
				p[ 2 ][ 1 ] = in( i    , j + 1, k     ) > 0 ? 0 : 1;
				p[ 2 ][ 2 ] = in( i - 1, j + 1, k     ) > 0 ? 0 : 1;
				p[ 2 ][ 3 ] = in( i - 1, j    , k     ) > 0 ? 0 : 1;
				p[ 2 ][ 4 ] = in( i - 1, j - 1, k     ) > 0 ? 0 : 1;
				p[ 2 ][ 5 ] = in( i    , j - 1, k     ) > 0 ? 0 : 1;
				p[ 2 ][ 6 ] = in( i + 1, j - 1, k     ) > 0 ? 0 : 1;
				p[ 2 ][ 7 ] = in( i + 1, j    , k     ) > 0 ? 0 : 1;
				p[ 2 ][ 8 ] = in( i + 1, j + 1, k     ) > 0 ? 0 : 1;

				p[ 3 ][ 0 ] = in( i    , j    , k + 1 ) > 0 ? 0 : 1;
				p[ 3 ][ 1 ] = in( i    , j + 1, k + 1 ) > 0 ? 0 : 1;
				p[ 3 ][ 2 ] = in( i - 1, j + 1, k + 1 ) > 0 ? 0 : 1;
				p[ 3 ][ 3 ] = in( i - 1, j    , k + 1 ) > 0 ? 0 : 1;
				p[ 3 ][ 4 ] = in( i - 1, j - 1, k + 1 ) > 0 ? 0 : 1;
				p[ 3 ][ 5 ] = in( i    , j - 1, k + 1 ) > 0 ? 0 : 1;
				p[ 3 ][ 6 ] = in( i + 1, j - 1, k + 1 ) > 0 ? 0 : 1;
				p[ 3 ][ 7 ] = in( i + 1, j    , k + 1 ) > 0 ? 0 : 1;
				p[ 3 ][ 8 ] = in( i + 1, j + 1, k + 1 ) > 0 ? 0 : 1;

				//�Q�D6�ߖT�ɂ���1-�v�f�̌�
				difference_type num = p[ 1 ][ 0 ] + p[ 2 ][ 1 ] + p[ 2 ][ 3 ] + p[ 2 ][ 5 ] + p[ 2 ][ 7 ] + p[ 3 ][ 0 ];
				if( num <= 3 )
				{
					return( true );
				}
				//�R�D
				else if( num == 4 )
				{
					if( Sx( p ) == 1 )
					{
						return( false );
					}
					else
					{
						return( true );
					}
				}
				//�S�D
				else if( num == 5 )
				{
					if( Sx( p ) == 1 )
					{
						return( false );
					}
					else
					{
						//�ˉe�O���t�ɌǗ��_�����邩
						if( p[ 2 ][ 0 ] == 0 )
						{
							return( false );
						}

						static size_type S1[] = { 1, 3, 5, 7 };
						difference_type hole = 0;
						if( p[ 1 ][ 0 ] != 0 )
						{
							for( size_type h = 0 ; h < 4 ; h++ )
							{
								hole += p[ 1 ][ S1[ h ] ] * p[ 2 ][ S1[ h ] ];
							}
							if( hole == 0 )
							{
								return( false );
							}
						}

						hole = 0;
						if( p[ 3 ][ 0 ] != 0 )
						{
							for( size_type h = 0 ; h < 4 ; h++ )
							{
								hole += p[ 3 ][ S1[ h ] ] * p[ 2 ][ S1[ h ] ];
							}
							if( hole == 0 )
							{
								return( false );
							}
						}

						for( size_type h = 0 ; h < 4 ; h++ )
						{
							hole = 0;
							if( p[ 2 ][ S1[ h ] ] != 0 )
							{
								hole = p[ 1 ][ 0 ] * p[ 1 ][ S1[ h ] ] + p[ 3 ][ 0 ] * p[ 3 ][ S1[ h ] ] + P2( p, 2, S1[ h ] - 1 ) * P2( p, 2, S1[ h ] - 2 ) + P2( p, 2, S1[ h ] + 1 ) * P2( p, 2, S1[ h ] + 2 );
								if( hole == 0 )
								{
									return( false );
								}
							}
						}
						return( true );
					}
				}
				//�T�D
				else if( num == 6 )
				{
					typedef matrix< int > matrix_type;

					//�אڍs��
					matrix_type M( 6, 6 );

					M( 0, 0 ) = 0          ; M( 0, 1 ) = p[ 1 ][ 1 ]; M( 0, 2 ) = p[ 1 ][ 3 ]; M( 0, 3 ) = p[ 1 ][ 7 ]; M( 0, 4 ) = p[ 1 ][ 5 ]; M( 0, 5 ) = 0;
					M( 1, 0 ) = p[ 1 ][ 1 ]; M( 1, 1 ) = 0          ; M( 1, 2 ) = p[ 2 ][ 2 ]; M( 1, 3 ) = p[ 2 ][ 8 ]; M( 1, 4 ) = 0          ; M( 1, 5 ) = p[ 3 ][ 1 ];
					M( 2, 0 ) = p[ 1 ][ 3 ]; M( 2, 1 ) = p[ 2 ][ 2 ]; M( 2, 2 ) = 0          ; M( 2, 3 ) = 0          ; M( 2, 4 ) = p[ 2 ][ 4 ]; M( 2, 5 ) = p[ 3 ][ 3 ];
					M( 3, 0 ) = p[ 1 ][ 7 ]; M( 3, 1 ) = p[ 2 ][ 8 ]; M( 3, 2 ) = 0          ; M( 3, 3 ) = 0          ; M( 3, 4 ) = p[ 2 ][ 6 ]; M( 3, 5 ) = p[ 3 ][ 7 ];
					M( 4, 0 ) = p[ 1 ][ 5 ]; M( 4, 1 ) = 0          ; M( 4, 2 ) = p[ 2 ][ 4 ]; M( 4, 3 ) = p[ 2 ][ 6 ]; M( 4, 4 ) = 0          ; M( 4, 5 ) = p[ 3 ][ 5 ];
					M( 5, 0 ) = 0          ; M( 5, 1 ) = p[ 3 ][ 1 ]; M( 5, 2 ) = p[ 3 ][ 3 ]; M( 5, 3 ) = p[ 3 ][ 7 ]; M( 5, 4 ) = p[ 3 ][ 5 ]; M( 5, 5 ) = 0;

					matrix_type I = matrix_type::identity( 6, 6 );
					matrix_type N = M * ( I + M * ( I + M * ( I + M * ( I + M ) ) ));

					for( size_type r = 0 ; r < N.size( ) ; r++ )
					{
						if( N[ r ] == 0 )
						{
							return( false );
						}
					}
					return( true );
				}
				else
				{
					std::cout << "Error" << std::endl;
				}

				return( true );
			}
		};


		template < class T, class Allocator, class Neighbor >
		void shrink_skelton( array3< T, Allocator > &in, Neighbor __dmy__ )
		{
			typedef typename array3< T, Allocator >::size_type size_type;
			typedef Neighbor neighbor_type;

			bool loop;

			do
			{
				loop = false;
				for( size_type k = 0 ; k < in.depth( ) ; k++ )
				{
					for( size_type j = 0 ; j < in.height( ) ; j++ )
					{
						for( size_type i = 0 ; i < in.width( ) ; i++ )
						{
							if( in( i, j, k ) != 0 )
							{
								if( neighbor_type::is_deletable( in, i, j, k ) )
								{
									in( i, j, k ) = 0;
									loop = true;
								}
							}
						}
					}
				}
			} while( loop );
		}


		template< class T >
		struct border
		{
			typedef ptrdiff_t difference_type;

			difference_type i;
			difference_type j;
			difference_type k;
			T value;
			T distance;

			border( difference_type ii, difference_type jj, difference_type kk, T val ) : i( ii ), j( jj ), k( kk ), value( val ), distance( val ) {}
		};

		template < class T, class Allocator, class Neighbor >
		void thinning( array3< T, Allocator > &in, Neighbor __dmy__ )
		{
			typedef typename array3< T, Allocator >::size_type size_type;
			typedef typename array3< T, Allocator >::difference_type difference_type;
			typedef T value_type;
			typedef Neighbor neighbor_type;
			typedef border< T > border_type;
			typedef std::deque< border_type > border_list_type;

			size_type num, loop = 0;
			border_list_type blist;

			// Step1 �����ϊ�
			euclidean::distance_transform( in, in );

			// �}�`�̒[�͂O
			for( size_type j = 0 ; j < in.height( ) ; j++ )
			{
				for( size_type i = 0 ; i < in.width( ) ; i++ )
				{
					in( i, j, 0 ) = 0;
					in( i, j, in.depth( ) - 1 ) = 0;
				}
			}

			for( size_type k = 0 ; k < in.depth( ) ; k++ )
			{
				for( size_type j = 0 ; j < in.height( ) ; j++ )
				{
					in( 0, j, k ) = 0;
					in( in.width( ) - 1, j, k ) = 0;
				}
			}

			for( size_type k = 0 ; k < in.depth( ) ; k++ )
			{
				for( size_type i = 0 ; i < in.width( ) ; i++ )
				{
					in( i, 0, k ) = 0;
					in( i, in.height( ) - 1, k ) = 0;
				}
			}

			value_type min = type_limits< value_type >::maximum( ), max = type_limits< value_type >::minimum( );
			for( size_type i = 0 ; i < in.size( ) ; i++ )
			{
				value_type &v = in[ i ];
				if( v != 0 )
				{
					v += 20;
					if( v > max ) max = v;
					if( v < min ) min = v;
				}
			}

			// Step2 ���E��f�̌��o
			for( size_type k = 0 ; k < in.depth( ) ; k++ )
			{
				for( size_type j = 0 ; j < in.height( ) ; j++ )
				{
					for( size_type i = 0 ; i < in.width( ) ; i++ )
					{
						if( in( i, j, k ) > 20 )
						{
							if( in( i - 1, j, k ) == 0 || in( i + 1, j, k ) == 0 ||
								in( i, j - 1, k ) == 0 || in( i, j + 1, k ) == 0 ||
								in( i, j, k - 1 ) == 0 || in( i, j, k + 1 ) == 0 )
							{
								blist.push_back( border_type( i, j, k, in( i, j, k ) ) );
								in( i, j, k ) = 1;
							}
						}
					}
				}
			}

			do
			{
				//Step3 ���C���T�C�N��
				typename border_list_type::iterator ite = blist.begin( );
				for( ; ite != blist.end( ) ; )
				{
					if( ite->value <= min )
					{
						difference_type i = ite->i;
						difference_type j = ite->j;
						difference_type k = ite->k;

						//�����s�\�Ȃ�ꎞ�ۑ��_
						if( !neighbor_type::is_deletable( in, i, j, k ) )
						{
							ite->value = 16;
						}
						else
						{
							num = 0;
							if( in( i    , j    , k - 1 ) > 0 ) num++;
							if( in( i    , j + 1, k - 1 ) > 0 ) num++;
							if( in( i - 1, j + 1, k - 1 ) > 0 ) num++;
							if( in( i - 1, j    , k - 1 ) > 0 ) num++;
							if( in( i - 1, j - 1, k - 1 ) > 0 ) num++;
							if( in( i    , j - 1, k - 1 ) > 0 ) num++;
							if( in( i + 1, j - 1, k - 1 ) > 0 ) num++;
							if( in( i + 1, j    , k - 1 ) > 0 ) num++;
							if( in( i + 1, j + 1, k - 1 ) > 0 ) num++;

							if( in( i    , j + 1, k     ) > 0 ) num++;
							if( in( i - 1, j + 1, k     ) > 0 ) num++;
							if( in( i - 1, j    , k     ) > 0 ) num++;
							if( in( i - 1, j - 1, k     ) > 0 ) num++;
							if( in( i    , j - 1, k     ) > 0 ) num++;
							if( in( i + 1, j - 1, k     ) > 0 ) num++;
							if( in( i + 1, j    , k     ) > 0 ) num++;
							if( in( i + 1, j + 1, k     ) > 0 ) num++;

							if( in( i    , j    , k + 1 ) > 0 ) num++;
							if( in( i    , j + 1, k + 1 ) > 0 ) num++;
							if( in( i - 1, j + 1, k + 1 ) > 0 ) num++;
							if( in( i - 1, j    , k + 1 ) > 0 ) num++;
							if( in( i - 1, j - 1, k + 1 ) > 0 ) num++;
							if( in( i    , j - 1, k + 1 ) > 0 ) num++;
							if( in( i + 1, j - 1, k + 1 ) > 0 ) num++;
							if( in( i + 1, j    , k + 1 ) > 0 ) num++;
							if( in( i + 1, j + 1, k + 1 ) > 0 ) num++;

							//�[�_�Ȃ�i�v�ۑ��_
							if( num == 1 )
							{
								ite = blist.erase( ite );
								continue;
							}
							else
							{
								ite->value = (int)( num / 3 ) + 7;
							}
						}
					}
					++ite;
				}

				//Step4 �T�u�T�C�N��
				for( difference_type bordertype = 7 ; bordertype < 16 ; bordertype++ )
				{
					for( ite = blist.begin( ) ; ite != blist.end( ) ; )
					{
						if( ite->value == bordertype )
						{
							difference_type i = ite->i;
							difference_type j = ite->j;
							difference_type k = ite->k;

							//�����s�\�Ȃ�ꎞ�ۑ��_
							if( !neighbor_type::is_deletable( in, i, j, k ) )
							{
								ite->value = 16;
							}
							else
							{
								num = 0;
								if( in( i    , j    , k - 1 ) > 0 ) num++;
								if( in( i    , j + 1, k - 1 ) > 0 ) num++;
								if( in( i - 1, j + 1, k - 1 ) > 0 ) num++;
								if( in( i - 1, j    , k - 1 ) > 0 ) num++;
								if( in( i - 1, j - 1, k - 1 ) > 0 ) num++;
								if( in( i    , j - 1, k - 1 ) > 0 ) num++;
								if( in( i + 1, j - 1, k - 1 ) > 0 ) num++;
								if( in( i + 1, j    , k - 1 ) > 0 ) num++;
								if( in( i + 1, j + 1, k - 1 ) > 0 ) num++;

								if( in( i    , j + 1, k     ) > 0 ) num++;
								if( in( i - 1, j + 1, k     ) > 0 ) num++;
								if( in( i - 1, j    , k     ) > 0 ) num++;
								if( in( i - 1, j - 1, k     ) > 0 ) num++;
								if( in( i    , j - 1, k     ) > 0 ) num++;
								if( in( i + 1, j - 1, k     ) > 0 ) num++;
								if( in( i + 1, j    , k     ) > 0 ) num++;
								if( in( i + 1, j + 1, k     ) > 0 ) num++;

								if( in( i    , j    , k + 1 ) > 0 ) num++;
								if( in( i    , j + 1, k + 1 ) > 0 ) num++;
								if( in( i - 1, j + 1, k + 1 ) > 0 ) num++;
								if( in( i - 1, j    , k + 1 ) > 0 ) num++;
								if( in( i - 1, j - 1, k + 1 ) > 0 ) num++;
								if( in( i    , j - 1, k + 1 ) > 0 ) num++;
								if( in( i + 1, j - 1, k + 1 ) > 0 ) num++;
								if( in( i + 1, j    , k + 1 ) > 0 ) num++;
								if( in( i + 1, j + 1, k + 1 ) > 0 ) num++;

								//�[�_�Ȃ�i�v�ۑ��_
								if( num == 1 )
								{
									ite = blist.erase( ite );
									continue;
								}
								//��f�̏���
								else
								{
									in( i, j, k ) = 0;
									ite = blist.erase( ite );

									if( in( i - 1, j, k ) > 20 )
									{
										blist.push_back( border_type( i - 1, j, k, in( i - 1, j, k ) ) );
										in( i - 1, j, k ) = 1;
									}
									if( in( i + 1, j, k ) > 20 )
									{
										blist.push_back( border_type( i + 1, j, k, in( i + 1, j, k ) ) );
										in( i + 1, j, k ) = 1;
									}
									if( in( i, j - 1, k ) > 20 )
									{
										blist.push_back( border_type( i, j - 1, k, in( i, j - 1, k ) ) );
										in( i, j - 1, k ) = 1;
									}
									if( in( i, j + 1, k ) > 20 )
									{
										blist.push_back( border_type( i, j + 1, k, in( i, j + 1, k ) ) );
										in( i, j + 1, k ) = 1;
									}
									if( in( i, j, k - 1 ) > 20 )
									{
										blist.push_back( border_type( i, j, k - 1, in( i, j, k - 1 ) ) );
										in( i, j, k - 1) = 1;
									}
									if( in( i, j, k + 1 ) > 20 )
									{
										blist.push_back( border_type( i, j, k + 1, in( i, j, k + 1 ) ) );
										in( i, j, k + 1 ) = 1;
									}

									continue;
								}
							}
						}
						++ite;
					}
				}

				for( ite = blist.begin( ) ; ite != blist.end( ) ; ++ite )
				{
					if( ite->value == 16 )
					{
						if( neighbor_type::is_deletable( in, ite->i, ite->j, ite->k ) )
						{
							ite->value = ite->distance;
						}
					}
				}

				//Step5 �I������
				min = max;
				num = 0;
				for( ite = blist.begin( ) ; ite != blist.end( ) ; ++ite )
				{
					if( ite->value < min && ite->value > 20) min = ite->value;
					if( ite->value == 16) num++;
				}

			} while( min < max || num != blist.size( ) );

			//Step6 �㏈��
			for( size_type k = 0 ; k < in.depth( ) ; k++ )
			{
				for( size_type j = 0 ; j < in.height( ) ; j++ )
				{
					for( size_type i = 0 ; i < in.width( ) ; i++ )
					{
						if( in( i, j, k ) != 0 )
						{
							in( i ,j, k ) = 1;
						}
					}
				}
			}
		}
	}


	/// @brief ���[�N���b�h������p����2�����摜�ɑ΂���א����A���S���Y��
	//!
	//! �א������ʂ�8�A���ƂȂ�
	//!
	//! @attention ���͂Əo�͂������摜�I�u�W�F�N�g�ł����������x�����O���邱�Ƃ��\�ł�
	//!
	//! @param[in]  in  �c ���͉摜
	//! @param[out] out �c �o�͉摜
	//!
	template < class T1, class T2, class Allocator1, class Allocator2 >
	void thinning( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out )
	{
		typedef typename array2< T2, Allocator2 >::size_type size_type;
		typedef typename array2< T2, Allocator2 >::value_type value_type;

		out.resize( in.size1( ), in.size2( ) );
		out.reso1( in.reso1( ) );
		out.reso2( in.reso2( ) );

		for( size_type i = 0 ; i < in.size( ) ; i++ )
		{
			out[ i ] = in[ i ] > 0 ? 1 : 0;
		}

		__utility__::thinning( out );
	}


	/// @brief 3�����摜�ɑ΂�����k�X�P���g���A���S���Y��
	//!
	//! @param[in]  in  �c ���͉摜
	//! @param[out] out �c �o�͉摜
	//!
	template < class T1, class T2, class Allocator1, class Allocator2 >
	void shrink_skelton6( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out )
	{
		typedef typename array3< T2, Allocator2 >::size_type  size_type;
		typedef typename array3< T2, Allocator2 >::value_type value_type;

		out.resize( in.size1( ), in.size2( ), in.size3( ) );
		out.reso1( in.reso1( ) );
		out.reso2( in.reso2( ) );
		out.reso3( in.reso3( ) );

		for( size_type i = 0 ; i < in.size( ) ; i++ )
		{
			out[ i ] = in[ i ] > 0 ? 1 : 0;
		}
		__utility__::shrink_skelton( out, __utility__::neighbor< 6 >( ) );
	}


	template < class T1, class T2, class Allocator1, class Allocator2 >
	void shrink_skelton26( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out )
	{
		typedef typename array3< T2, Allocator2 >::size_type  size_type;
		typedef typename array3< T2, Allocator2 >::value_type value_type;

		out.resize( in.size1( ), in.size2( ), in.size3( ) );
		out.reso1( in.reso1( ) );
		out.reso2( in.reso2( ) );
		out.reso3( in.reso3( ) );

		for( size_type i = 0 ; i < in.size( ) ; i++ )
		{
			out[ i ] = in[ i ] > 0 ? 1 : 0;
		}
		__utility__::shrink_skelton( out, __utility__::neighbor< 26 >( ) );
	}


	/// @brief ���[�N���b�h������p����3�����摜�ɑ΂���א����A���S���Y��
	//!
	//! �א������ʂ�8�A���ƂȂ�
	//!
	//! @attention ���͂Əo�͂������摜�I�u�W�F�N�g�ł��������א������邱�Ƃ��\�ł�
	//!
	//! @param[in]  in  �c ���͉摜
	//! @param[out] out �c �o�͉摜
	//!
	template < class T1, class T2, class Allocator1, class Allocator2 >
	void thinning6( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out )
	{
		typedef typename array3< T2, Allocator2 >::size_type  size_type;
		typedef typename array3< T2, Allocator2 >::value_type value_type;

		out.resize( in.size1( ), in.size2( ), in.size3( ) );
		out.reso1( in.reso1( ) );
		out.reso2( in.reso2( ) );
		out.reso3( in.reso3( ) );

		for( size_type i = 0 ; i < in.size( ) ; i++ )
		{
			out[ i ] = in[ i ] > 0 ? 1 : 0;
		}
		__utility__::thinning( out, __utility__::neighbor< 6 >( ) );
	}


	template < class T1, class T2, class Allocator1, class Allocator2 >
	void thinning26( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out )
	{
		typedef typename array3< T2, Allocator2 >::size_type  size_type;
		typedef typename array3< T2, Allocator2 >::value_type value_type;

		out.resize( in.size1( ), in.size2( ), in.size3( ) );
		out.reso1( in.reso1( ) );
		out.reso2( in.reso2( ) );
		out.reso3( in.reso3( ) );

		for( size_type i = 0 ; i < in.size( ) ; i++ )
		{
			out[ i ] = in[ i ] > 0 ? 1 : 0;
		}
		__utility__::thinning( out, __utility__::neighbor< 26 >( ) );
	}
}


/// @}
//  �א����O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_THINNING__
