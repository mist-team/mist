/// @file mist/filter/thinning.h
//!
//! @brief �e�����̉摜�ɑΉ������C�א����A���S���Y��
//!

#ifndef __INCLUDE_MIST_THINNING__
#define __INCLUDE_MIST_THINNING__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#ifndef __INCLUDE_MIST_LIMITS__
#include "../limits.h"
#endif

// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


// ���f�B�A���t�B���^

namespace __thinning_controller__
{
	template < class T, class Allocator >
	inline void val9( const array2< T, Allocator > &in, typename array2< T, Allocator >::value_type p[9],
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

	template < class T > inline T pixel( T p ){ return( ( p > 0 )? 1: 0 ); }
	template < class T > inline T pixel( T p[ 9 ], int index )
	{
		if( index <= 0 )
		{
			return( pixel( p[ index + 8 ] ) );
		}
		else if( index >= 9 )
		{
			return( pixel( p[ index - 8 ] ) );
		}
		else
		{
			return( pixel( p[ index ] ) );
		}
	}
	template < class T > inline T pixel_( T p[ 9 ], int index )
	{
		return( 1 - pixel( p, index ) );
	}
	template < class T > inline int Nc4( T p[ 9 ] )
	{
		int ret = 0, i;
		for( i = 1 ; i < 9 ; i += 2 )
		{
			ret += pixel( p, i ) - pixel( p, i ) * pixel( p, i + 1 ) * pixel( p, i + 2 );
		}
		return( ret );
	}
	template < class T > inline int Nc8( T p[ 9 ] )
	{
		int ret = 0, i;
		for( i = 1 ; i < 9 ; i += 2 )
		{
			ret += pixel_( p, i ) - pixel_( p, i ) * pixel_( p, i + 1 ) * pixel_( p, i + 2 );
		}
		return( ret );
	}

	template < class T, class Allocator >
	void thinning( array2< T, Allocator > &ia )
	{
		typedef typename array2< T, Allocator >::size_type  size_type;
		typedef typename array2< T, Allocator >::value_type value_type;

		bool flag;
		size_type w = ia.width( );
		size_type h = ia.height( );
		size_type i, j;
		char p[9], q[9], value;
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
					if( pixel( p[0] ) != 1 ) continue;

					// ����1 = ���͉摜F�ɂ����āCx0��4�ߖT�ɏ��Ȃ��Ƃ�1��0��f�����݂���
					if( p[1] == 1 && p[3] == 1 && p[5] == 1 && p[7] == 1 ) continue;

					// ����2 = ���͉摜F�ɂ����āCx0��8�ߖT��2�ȏ��1��f������(x0���[�_�łȂ�)
					value = pixel( p[1] ) + pixel( p[2] ) + pixel( p[3] ) + pixel( p[4] ) + pixel( p[5] ) + pixel( p[6] ) + pixel( p[7] ) + pixel( p[8] );
					if( value <= 1 ) continue;

					// ����3 = ��Ɖ摜F'�ɂ����āC-1�̉�f��0��f�Ƃ݂Ȃ������Cx0���Ǘ��_�ł͂Ȃ�
					value = pixel( q[1] ) + pixel( q[2] ) + pixel( q[3] ) + pixel( q[4] ) + pixel( q[5] ) + pixel( q[6] ) + pixel( q[7] ) + pixel( q[8] );
					if( value == 0 ) continue;

					// ����4 = ���͉摜F�ɂ����āCx0�ɂ�����Nc[8]�̘A������1�ł���
					if( Nc8( p ) != 1 ) continue;

					// ����5 = i=3,5�ɑ΂��Ď��̎��������藧��
					//         xi�����傤�ǂ��̔�����1����0�ɕς����Ă͂��Ȃ�(xi��-1�ɂȂ��Ă��Ȃ�)���C
					//         �܂��́C���ꂪ-1�ɂȂ��Ă��Ă��C-1��S��0�Ƃ݂Ȃ��Čv�Z����x0�̘A����Nc[8]��1�ɓ�����
					if( q[3] == -1 || q[5] == -1 )
					{
						if( Nc8( q ) != 1 ) continue;
					}

					// �S�Ă̏����𖞂�������f��-1�ɂ���
					id( i, j ) = -1;
					flag = true;
				}
			}
			for( i = 0 ; i < id.size( ) ; i++ )
			{
				id[i] = id[i] == -1? 0: id[i];
			}
		}
		for( i = 0 ; i < ia.size( ) ; i++ )
		{
			ia[ i ] = id[ i ] > 0 ? 1 : 0;
		}
	}
}


//! @addtogroup thinning_group �א���
//!  @{


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
void thinning( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out )
{
	typedef typename array2< T2, Allocator2 >::size_type  size_type;
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


//template < class T1, class T2, class Allocator1, class Allocator2 >
//typename array3< T2, Allocator2 >::size_type thinning6( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out, typename array3< T2, Allocator2 >::size_type max_label = __thinning_controller__::default_label_num3< T2 >::value )
//{
//	typedef typename array3< T2, Allocator2 >::size_type  size_type;
//	typedef typename array3< T2, Allocator2 >::value_type value_type;
//
//	if( max_label == 0 )
//	{
//		max_label = type_limits< value_type >::maximum( );
//	}
//
//	out.resize( in.size1( ), in.size2( ), in.size3( ) );
//	out.reso1( in.reso1( ) );
//	out.reso2( in.reso2( ) );
//	out.reso3( in.reso3( ) );
//
//	for( size_type i = 0 ; i < in.size( ) ; i++ )
//	{
//		out[ i ] = in[ i ] > 0 ? 1 : 0;
//	}
//	return( __thinning_controller__::thinning( out, max_label, __thinning_controller__::neighbors< 6 >( ) ) );
//}
//
//
//template < class T1, class T2, class Allocator1, class Allocator2 >
//typename array3< T2, Allocator2 >::size_type thinning26( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out, typename array3< T2, Allocator2 >::size_type max_label = __thinning_controller__::default_label_num3< T2 >::value )
//{
//	typedef typename array3< T2, Allocator2 >::size_type  size_type;
//	typedef typename array3< T2, Allocator2 >::value_type value_type;
//
//	if( max_label == 0 )
//	{
//		max_label = type_limits< value_type >::maximum( );
//	}
//
//	out.resize( in.size1( ), in.size2( ), in.size3( ) );
//	out.reso1( in.reso1( ) );
//	out.reso2( in.reso2( ) );
//	out.reso3( in.reso3( ) );
//
//	for( size_type i = 0 ; i < in.size( ) ; i++ )
//	{
//		out[ i ] = in[ i ] > 0 ? 1 : 0;
//	}
//	return( __thinning_controller__::thinning( out, max_label, __thinning_controller__::neighbors< 26 >( ) ) );
//}

/// @}
//  �א����O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_THINNING__
