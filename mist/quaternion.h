#ifndef __INCLUDE_MIST_QUATERNION__
#define __INCLUDE_MIST_QUATERNION__


#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif

#ifndef __INCLUDE_MIST_VECTOR__
#include "vector.h"
#endif


#include <cmath>

// mist名前空間の始まり
_MIST_BEGIN


template < class T >
class quaternion
{
public:
	typedef T value_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

public:
	value_type w;
	value_type x;
	value_type y;
	value_type z;

	quaternion( ) : w( 0 ), x( 0 ), y( 0 ), z( 0 ){ }
	quaternion( const value_type &ww, const value_type &xx, const value_type &yy, const value_type &zz ) : w( ww ), x( xx ), y( yy ), z( zz ){ }

	template < class TT >
	quaternion( const quaternion< TT > &q ) : w( static_cast< value_type >( q.w ) ), x( static_cast< value_type >( q.x ) ), y( static_cast< value_type >( q.y ) ), z( static_cast< value_type >( q.z ) ){ }

	quaternion( const quaternion< T > &q ) : w( q.w ), x( q.x ), y( q.y ), z( q.z ){ }

	template < class TT >
	quaternion( value_type ww, const vector3< TT > &v ) : w( ww ), x( static_cast< value_type >( v.x ) ), y( static_cast< value_type >( v.y ) ), z( static_cast< value_type >( v.z ) ){ }

	// クォータニオンを用いた任意軸周りの回転
	// 右手系の場合は右ねじ回転，左手系の場合は左ねじ回転となるので注意！！
	template < class TT >
	quaternion( const vector3< TT > &axis, value_type theta )
	{
		double t = theta * 3.1415926535897932384626433832795 / 180.0 / 2.0;
		double c = std::cos( t );
		double s = std::sin( t );
		w = static_cast< value_type >( c );
		x = static_cast< value_type >( s * axis.x );
		y = static_cast< value_type >( s * axis.y );
		z = static_cast< value_type >( s * axis.z );
	}

	// オペレータのオーバーライド
	const quaternion &operator =( const quaternion &q )
	{
		if( &q != this )
		{
			w = q.w;
			x = q.x;
			y = q.y;
			z = q.z;
		}
		return ( *this );
	}

	const quaternion &operator +=( const quaternion &q )
	{
		w = w + q.w;
		x = x + q.x;
		y = y + q.y;
		z = z + q.z;
		return( *this );
	}

	const quaternion &operator +=( const value_type &a )
	{
		w += a;
		return( *this );
	}

	const quaternion &operator -=( const quaternion &q )
	{
		w = w - q.w;
		x = x - q.x;
		y = y - q.y;
		z = z - q.z;
		return( *this );
	}

	const quaternion &operator -=( const value_type &a )
	{
		w -= a;
		return( *this );
	}

	const quaternion &operator *=( const quaternion &q )
	{
		value_type ww = w * q.w - x * q.x - y * q.y - z * q.z;
		value_type xx = w * q.x + x * q.w + y * q.z - z * q.y;
		value_type yy = w * q.y + y * q.w + z * q.x - x * q.z;
		value_type zz = w * q.z + z * q.w + x * q.y - y * q.x;
		w = ww;
		x = xx;
		y = yy;
		z = zz;
		return( *this );
	}

	const quaternion &operator *=( const value_type &a )
	{
		w *= a;
		x *= a;
		y *= a;
		z *= a;
		return( *this );
	}

	const quaternion &operator /=( const quaternion &q )
	{
		return( this->operator *=( q.inv( ) ) );
	}

	const quaternion &operator /=( const value_type &a )
	{
		w /= a;
		x /= a;
		y /= a;
		z /= a;
		return( *this );
	}


	// 比較関数
	bool operator ==( const quaternion &q ) const { return( w == q.w && x == q.x && y == q.y && z == q.z ); }
	bool operator !=( const quaternion &q ) const { return( !( *this == q ) ); }
	bool operator < ( const quaternion &q ) const { return( !( *this >= q ) ); }
	bool operator <=( const quaternion &q ) const { return( q >= *this ); }
	bool operator > ( const quaternion &q ) const { return( q < *this ); }
	bool operator >=( const quaternion &q ) const { return( w >= q.w && x >= q.x && y >= q.y && z >= q.z ); }


public:	// その他の関数
	const quaternion conjugate( ) const	// 共役クォータニオン
	{
		return( quaternion( w, -x, -y, -z ) );
	}

	const quaternion inv( ) const
	{
		value_type length_ = length( );
		return( conjugate( ) / ( length_ * length_ ) );
	}

	const quaternion unit( ) const
	{
		value_type length_ = length( );
		return quaternion( w / length_, x / length_, y / length_, z / length_ );
	}

	value_type length( ) const { return ( static_cast< value_type >( sqrt( (double)( w * w + x * x + y * y + z * z ) ) ) ); }

	// ベクトルの回転
	template < class TT >
	const vector3< TT > rotate( const vector3< TT > &v ) const
	{
		quaternion q = ( *this ) * quaternion( 0, static_cast< value_type >( v.x ), static_cast< value_type >( v.y ), static_cast< value_type >( v.z ) ) * inv( );
		return( vector3< TT >( static_cast< TT >( q.x ), static_cast< TT >( q.y ), static_cast< TT >( q.z ) ) );
	}
};

template < class T > inline const quaternion< T > operator +( const quaternion< T > &q1, const quaternion< T > &q2 ){ return( quaternion< T >( q1 ) += q2 ); }
template < class T > inline const quaternion< T > operator -( const quaternion< T > &q1, const quaternion< T > &q2 ){ return( quaternion< T >( q1 ) -= q2 ); }
template < class T > inline const quaternion< T > operator /( const quaternion< T > &q1, const quaternion< T > &q2 ){ return( quaternion< T >( q1 ) /= q2 ); }
template < class T > inline const quaternion< T > operator *( const quaternion< T > &q1, const quaternion< T > &q2 ){ return( quaternion< T >( q1 ) *= q2 ); }

template < class T > inline const quaternion< T > operator *( const quaternion< T > &q, const typename quaternion< T >::value_type &a ){ return( quaternion< T >( q ) *= a ); }
template < class T > inline const quaternion< T > operator *( const typename quaternion< T >::value_type &a, const quaternion< T > &q ){ return( quaternion< T >( q ) *= a ); }
template < class T > inline const quaternion< T > operator /( const quaternion< T > &q, const typename quaternion< T >::value_type &a ){ return( quaternion< T >( q ) /= a ); }
template < class T > inline const quaternion< T > operator +( const quaternion< T > &q, const typename quaternion< T >::value_type &a ){ return( quaternion< T >( q ) += a ); }
template < class T > inline const quaternion< T > operator -( const quaternion< T > &q, const typename quaternion< T >::value_type &a ){ return( quaternion< T >( q ) -= a ); }

template < class T > inline std::ostream &operator <<( std::ostream &out, const quaternion< T > &q )
{
	out << "( ";
	out << q.w << ", ";
	out << q.x << ", ";
	out << q.y << ", ";
	out << q.z << " )";
	return( out );
}

// クォータニオンから行列へ変換する
//template < class T >
//matrix< T > rotate_matrix( const quaternion< T > &q )
//{
//	matrix< T > mat(  4, 4  );
//	mat( 0, 0 ) = q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z;
//	mat( 0, 1 ) = 2 * ( q.x * q.y - q.w * q.z );
//	mat( 0, 2 ) = 2 * ( q.x * q.z + q.w * q.y );
//	mat( 0, 3 ) = 0;
//	mat( 1, 0 ) = 2 * ( q.x * q.y + q.w * q.z );
//	mat( 1, 1 ) = q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z;
//	mat( 1, 2 ) = 2 * ( q.y * q.z - q.w * q.x );
//	mat( 1, 3 ) = 0;
//	mat( 2, 0 ) = 2 * ( q.x * q.z - q.w * q.y );
//	mat( 2, 1 ) = 2 * ( q.y * q.z + q.w * q.x );
//	mat( 2, 2 ) = q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z;
//	mat( 2, 3 ) = 0;
//	mat( 3, 0 ) = 0;
//	mat( 3, 1 ) = 0;
//	mat( 3, 2 ) = 0;
//	mat( 3, 3 ) = 1;
//	return( mat );
//}

// 仮想トラックボールの実装(左手座標系)
// 以下のソースコードを参考にした
//
//* Trackball code:
//*
//* Implementation of a virtual trackball.
//* Implemented by Gavin Bell, lots of ideas from Thant Tessman and
//*   the August '88 issue of Siggraph's "Computer Graphics," pp. 121-129.
//*
//* Vector manip code:
//*
//* Original code from:
//* David M. Ciemiewicz, Mark Grossman, Henry Moreton, and Paul Haeberli
//*
//* Much mucking with by:
//* Gavin Bell
//
template < class T >
const quaternion< T > track_ball( const vector2< T > &p1, const vector2< T > &p2, const vector3< T > &axisX, const vector3< T > axisY, const vector3< T > axisZ, const typename vector3< T >::value_type &trackball_size = 0.8 )
{
	typedef typename quaternion< T >::value_type value_type;

	if( p1 == p2 )
	{
		return( quaternion< T >( 1, 0, 0, 0 ) );
	}

	vector3< T > sp1( p1.x, p1.y, 0 ), sp2( p2.x, p2.y, 0 );
	value_type l, _2 = std::sqrt( value_type( 2.0 ) );

	// 点1の座標を仮想トラックボール上に投影
	l = p1.length( );
	if( l < trackball_size / _2 )
	{
		sp1.z = - std::sqrt( trackball_size * trackball_size - l * l );
	}
	else
	{
		sp1.z = - trackball_size * trackball_size / 2.0 / l;
	}

	// 点2の座標を仮想トラックボール上に投影
	l = p2.length( );
	if( l < trackball_size / _2 )
	{
		sp2.z = - std::sqrt( trackball_size * trackball_size - l * l );
	}
	else
	{
		sp2.z = - trackball_size * trackball_size / 2.0 / l;
	}

	//	sp1 = sp1.unit();
	//	sp2 = sp2.unit();

	//	Vector3<double> axis = (sp2 * sp1).unit();
	vector3< T > axis = ( sp1 * sp2 ).unit( );
	axis = ( axis.x * axisX + axis.y * axisY + axis.z * axisZ ).unit( );

	l = ( sp2 - sp1 ).length( ) / ( 2 * trackball_size );
	//	l = (l < -1.0)? -1.0: l;
	l = l > 1 ? 1: l;

	double phi = std::asin( l );
	//	fprintf(stdout, "axis(%.1f, %.1f, %.1f)   theta = %.1f\n", axis.x, axis.y, axis.z, phi * 180 / PAI);
	//	printf("%.1f\n", phi * 180 / PAI);
	return( quaternion< T >( std::cos( phi ), std::sin( phi ) * axis ) );
}

template < class T >
const quaternion< T > track_ball( const typename vector3< T >::value_type &x1, const typename vector3< T >::value_type &y1, const typename vector3< T >::value_type &x2, const typename vector3< T >::value_type &y2,
									const vector3< T > &axisX, const vector3< T > &axisY, const vector3< T > &axisZ, const typename vector3< T >::value_type &trackball_size = typename vector3< T >::value_type( 0.8 ) )
{
	return( track_ball( vector2< T >( x1, y1 ), vector2< T >( x2, y2 ), axisX, axisY, axisZ, trackball_size ) );
}


// mist名前空間の終わり
_MIST_END

#endif // __INCLUDE_MIST_QUATERNION__
