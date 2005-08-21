/// @file mist/volumerender.h
//!
//! @brief �R�����摜�̃{�����[�������_�����O�G���W��
//!
#ifndef __INCLUDE_MIST_VOLUMERENDER__
#define __INCLUDE_MIST_VOLUMERENDER__


#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif

#ifndef __INCLUDE_MIST_H__
#include "mist.h"
#endif

#ifndef __INCLUDE_MIST_VECTOR__
#include "vector.h"
#endif

#ifndef __INCLUDE_MIST_LIMITS__
#include "limits.h"
#endif


#ifndef __INCLUDE_MIST_THREAD__
#include "thread.h"
#endif

#ifndef __INCLUDE_MIST_DISTANCE_TRANSFORM__
#include "filter/distance.h"
#endif


#include <vector>


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN




//! @addtogroup visualization_group ����
//!
//!  @{


//! @addtogroup volumerendering_group �{�����[�������_�����O
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/volumerender.h>
//! @endcode
//!
//! @{


namespace volumerender
{
	template < class T >
	struct attribute
	{
		typedef T value_type;

		value_type pixel;
		double alpha;
		bool   has_alpha;

		attribute( ) : pixel( ), alpha( 0 ), has_alpha( false ){ }

		//template < class TT >
		//attribute( const attribute &a ) : pixel( a.pixel ), alpha( a.alpha ), has_alpha( a.has_alpha ){ }

		attribute( const attribute &a ) : pixel( a.pixel ), alpha( a.alpha ), has_alpha( a.has_alpha ){ }

		attribute( const value_type &pix, double a ) : pixel( pix ), alpha( a ), has_alpha( a > 0.0 ){ }
	
	};

	template < class T >
	struct attribute_element
	{
		typedef T value_type;
		typedef ptrdiff_t difference_type;

		value_type pixel;
		difference_type value_lower;
		difference_type value_upper;
		double alpha_lower;
		double alpha_upper;

		attribute_element( ) : pixel( ), value_lower( 0 ), value_upper( 0 ), alpha_lower( 0 ), alpha_upper( 0 ){ }
		attribute_element( const value_type &pixel, difference_type vs, difference_type ve, double as, double ae )
			: pixel( pixel ), value_lower( vs ), value_upper( ve ), alpha_lower( as ), alpha_upper( ae ){ }

		attribute_element( const attribute_element &a ) : pixel( a.pixel ), value_lower( a.value_lower ), value_upper( a.value_upper ), alpha_lower( a.alpha_lower ), alpha_upper( a.alpha_upper ){ }
	
	};

	template < class T >
	class attribute_table : protected std::vector< attribute< T > >
	{
	public:
		typedef std::vector< attribute< T > > base;
		typedef attribute< T > attribute_type;
		typedef attribute_element< T > attribute_element_type;
		typedef typename base::allocator_type allocator_type;	///< @brief MIST�R���e�i�����p����A���P�[�^�^
		typedef typename base::reference reference;				///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�̎Q�ƁDmist::array< data > �̏ꍇ�Cdata & �ƂȂ�
		typedef typename base::const_reference const_reference;	///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�� const �Q�ƁDmist::array< data > �̏ꍇ�Cconst data & �ƂȂ�
		typedef typename base::value_type value_type;			///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�Dmist::array< data > �� data �Ɠ���
		typedef typename base::size_type size_type;				///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
		typedef typename base::difference_type difference_type;	///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���
		typedef typename base::pointer pointer;					///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�̃|�C���^�[�^�Dmist::array< data > �̏ꍇ�Cdata * �ƂȂ�
		typedef typename base::const_pointer const_pointer;		///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�� const �|�C���^�[�^�Dmist::array< data > �̏ꍇ�Cconst data * �ƂȂ�

		typedef T pixel_type;
		typedef std::vector< attribute_element_type > attribute_element_list;

	protected:
		difference_type sindex_;
		difference_type eindex_;
		difference_type zero_index_;

		attribute_element_list attribute_element_table;

	public:
		void append( const pixel_type &pixel, difference_type vs, difference_type ve, double as, double ae )
		{
			if( base::empty( ) )
			{
				return;
			}
			else if( vs > ve )
			{
				difference_type tmp = vs;
				vs = ve;
				ve = tmp;
			}
			vs = vs < sindex_ ? sindex_: vs;
			vs = vs > eindex_ ? eindex_: vs;
			ve = ve < sindex_ ? sindex_: ve;
			ve = ve > eindex_ ? eindex_: ve;

			if( as > ae )
			{
				double tmp = as;
				as = ae;
				ae = tmp;
			}

			attribute_element_table.push_back( attribute_element_type( pixel, vs, ve, as, ae ) );

			double step = ( ae - as ) / static_cast< double >( ve - vs + 1 );
			for( difference_type i = vs ; i <= ve ; i++ )
			{
				value_type &p = at( i );
				p.pixel = pixel;
				p.alpha = as + static_cast< double >( i - vs ) * step;
				p.has_alpha = p.alpha > 0.0;
			}
		}

	public:
		const difference_type minimum( ) const
		{
			return( sindex_ );
		}

		const difference_type maximum( ) const
		{
			return( eindex_ );
		}

		const bool empty( ) const
		{
			return( eindex_ < sindex_ );
		}

		value_type &operator []( difference_type index )
		{
			return( base::operator []( zero_index_ + index ) );
		}

		value_type &at( difference_type index )
		{
			return( base::at( zero_index_ + index ) );
		}

		const value_type &operator []( difference_type index ) const
		{
			return( base::operator []( zero_index_ + index ) );
		}

		const value_type &at( difference_type index ) const
		{
			return( base::at( zero_index_ + index ) );
		}

		bool has_alpha( difference_type index ) const
		{
			return( operator []( index ).has_alpha );
		}

		const attribute_table &operator =( const attribute_table &a )
		{
			if( &a != this )
			{
				base::operator =( a );
				sindex_ = a.sindex_;
				eindex_ = a.eindex_;
				zero_index_ = a.zero_index_;
				attribute_element_table = a.attribute_element_table;
			}
			return( *this );
		}

		const attribute_element_list &attribute_elements( ) const { return( attribute_element_table ); }


	public:
		void create( difference_type si, difference_type ei )
		{
			sindex_ = si;
			eindex_ = ei;

			base::resize( ei - si + 1 );
			zero_index_ = - si;

			fill( );
			attribute_element_table.clear( );
		}

		void clear( )
		{
			base::clear( );
			sindex_ = 0;
			eindex_ = -1;
			zero_index_ = 0;
			attribute_element_table.clear( );
		}

		void fill( )
		{
			for( size_type i = 0 ; i < base::size( ) ; i++ )
			{
				base::operator []( i ) = attribute_type( );
			}
			attribute_element_table.clear( );
		}

		attribute_table( ) : sindex_( 0 ), eindex_( -1 ), zero_index_( 0 ){ }

		attribute_table( difference_type si, difference_type ei ) : base( ei - si + 1 ), sindex_( si ), eindex_( ei ), zero_index_( 0 )
		{
			zero_index_ = - si;
		}

		attribute_table( const attribute_table &a ) : base( a ), sindex_( a.sindex_ ), eindex_( a.eindex_ ), zero_index_( a.zero_index_ ), attribute_element_table( a.attribute_element_table )
		{
		}
	};

	struct boundingbox
	{
	public:
		double a;
		double b;
		double c;
		double d;

		void clear( )
		{
			a = 0;
			b = 0;
			c = 0;
			d = 0;
		}

		boundingbox( ) : a( 0 ), b( 0 ), c( 0 ), d( 0 ){ }
		boundingbox( double aa, double bb, double cc, double dd ) : a( aa ), b( bb ), c( cc ), d( dd ){ }
	};


	template < class T >
	inline bool check_intersection( vector3< T > &p1, vector3< T > &p2, const boundingbox &box )
	{
		typedef vector3< T > vector_type;
		typedef typename vector3< T >::value_type value_type;

		vector_type n( box.a, box.b, box.c );

		value_type d1 = p1.inner( n );
		value_type d2 = p2.inner( n );
		bool c1 = box.d + d1 >= 0;
		bool c2 = box.d + d2 >= 0;

		if( c1 && c2 )
		{
			return( true );
		}
		else if( !c1 && !c2 )
		{
			return( false );
		}
		vector_type v1 = p1;
		vector_type v2 = p2;
		if( !c1 )
		{
			p1 = v2 + ( v1 - v2 ) * ( ( box.d + d2 ) / ( d2 - d1 ) );
		}
		if( !c2 )
		{
			p2 = v1 + ( v2 - v1 ) * ( ( box.d + d1 ) / ( d1 - d2 ) );
		}
		return( true );
	}

	struct parameter
	{
		typedef vector3< double > vector_type;

		vector_type	pos;
		vector_type	dir;
		vector_type	up;
		vector_type	offset;
		bool	perspective_view;
		bool	value_interpolation;
		double	fovy;
		double	ambient_ratio;
		double	diffuse_ratio;
		double	light_attenuation;
		double	sampling_step;
		double	termination;
		double	specular;
		double	distortion;

		boundingbox box[ 6 ];

		parameter( ) : perspective_view( true ), value_interpolation( true ), fovy( 80.0 ), ambient_ratio( 0.4 ), diffuse_ratio( 0.6 ), light_attenuation( 0.0 ),
						sampling_step( 1.0 ), termination( 0.01 ), specular( 1.0 ), distortion( 0.0 )
		{
		}
	};

	inline std::ostream &operator <<( std::ostream &out, const parameter &p )
	{
		out << "Pos = ( " << p.pos << " )" << std::endl;
		out << "Dir = ( " << p.dir << " )" << std::endl;
		out << "Up  = ( " << p.up  << " )" << std::endl;

		out << "Center = ( " << p.offset  << " )" << std::endl;

		out << "Perspective = " << p.perspective_view << std::endl;
		out << "ValueInterpolation = " << p.value_interpolation << std::endl;
		out << "Fovy = " << p.fovy << std::endl;
		out << "Ambient = " << p.ambient_ratio << std::endl;
		out << "Diffuse = " << p.diffuse_ratio << std::endl;
		out << "Specular = " << p.specular << std::endl;
		out << "LightAtten = " << p.light_attenuation << std::endl;
		out << "SamplingStep = " << p.sampling_step << std::endl;
		out << "Termination = " << p.termination << std::endl;
		out << "Distortion = " << p.distortion << std::endl;

		return( out );
	}

	// �{���x���������𐮐��Ɋۂ߂�
	// ���̐��̏ꍇ�́C�ʏ�� int �L���X�g�Ɠ������������
	// ���̐��̂Ƃ��́C�l�̏������ق��Ɋۂ߂���
	inline long to_integer( double val )
	{
		return( static_cast< long >( val ) );
		//val += 68719476736.0 * 1.5;
		//return( ( ( long * )&val )[ 0 ] >> 16 );
	}

	template < class T >
	inline void normalize_vector( vector3< T > &v )
	{
		double len = std::sqrt( v.x * v.x + v.y * v.y + v.z * v.z ) + 1.0e-10;
		v.x /= len;
		v.y /= len;
		v.z /= len;
	}


	struct no_depth_map
	{
		typedef ptrdiff_t difference_type;
		double operator()( difference_type i, difference_type j, difference_type k ) const
		{
			return( 2.0 );
		}
	};

	template < class DepthMap >
	struct depth_map
	{
		typedef DepthMap depth_map_type;
		typedef typename depth_map_type::difference_type difference_type;

		const depth_map_type &depth_map_;

		depth_map( const depth_map_type &dmap ) : depth_map_( dmap )
		{
		}

		double operator()( difference_type i, difference_type j, difference_type k ) const
		{
			double l = depth_map_( i >> 1, j >> 1, k >> 1 );
			return( l < 1.0 ? 2.0 : l * 2.0 );
		}
	};

	inline double _3( double v )
	{
		if( v < 0.0 )
		{
			return( - std::pow( -v, 1.0 / 3.0 ) );
		}
		else if( v > 0.0 )
		{
			return( std::pow( v, 1.0 / 3.0 ) );
		}
		else
		{
			return( 0.0 );
		}
	}

	inline double __maximum__( double v1, double v2 )
	{
		return( v1 > v2 ? v1 : v2 );
	}

	inline double __maximum__( double v1, double v2, double v3 )
	{
		if( v1 > v2 )
		{
			if( v1 > v3 )
			{
				return( v1 );
			}
			else
			{
				return( v3 );
			}
		}
		else if( v2 > v3 )
		{
			return( v2 );
		}
		else
		{
			return( v3 );
		}
	}

	inline double __minimum__( double v1, double v2 )
	{
		return( v1 < v2 ? v1 : v2 );
	}

	inline double __minimum__( double v1, double v2, double v3 )
	{
		if( v1 < v2 )
		{
			if( v1 < v3 )
			{
				return( v1 );
			}
			else
			{
				return( v3 );
			}
		}
		else if( v2 < v3 )
		{
			return( v2 );
		}
		else
		{
			return( v3 );
		}
	}

	inline size_t solve_equation( double p, double q, double &v1, double &v2, double &v3 )
	{
		const double pai = 3.1415926535897932384626433832795;
		double p_3 = p / 3.0;
		double q_2 = q / 2.0;
		double D = - ( q_2 * q_2 + p_3 * p_3 * p_3 );

		if( D > 0.0 )
		{
			double r  = std::sqrt( - 4.0 * p / 3.0 );
			double t1 = 1.0 / ( 3.0 * std::cos( 3.0 * q / ( p * r ) ) );
			double t2 = 2.0 * pai / 3.0 + t1;
			double t3 = 2.0 * pai / 3.0 - t1;
			v1 = r * std::cos( t1 );
			v2 = r * std::cos( t2 );
			v3 = r * std::cos( t3 );
			return( 3 );
		}
		else if( D == 0.0 )
		{
			if( p == 0.0 )
			{
				v1 = v2 = v3 = 0.0;
				return( 1 );
			}
			else
			{
				double r  = std::sqrt( - 4.0 * p / 3.0 );
				v1 = r;
				v2 = r * std::cos( 2.0 * pai / 3.0 );
				v3 = 0.0;
				return( 2 );
			}
		}
		//else if( D < 0.0 && p < 0.0 )
		//{
		//	double _D = std::sqrt( -D );
		//	v1 = _3( - q / 2.0 + _D ) + _3( - q / 2.0 - _D );
		//	v2 = v3 = 0.0;
		//	return( 1 );
		//}
		else
		{
			double _D = std::sqrt( -D );
			v1 = _3( - q / 2.0 + _D ) + _3( - q / 2.0 - _D );
			v2 = v3 = 0.0;
			return( 1 );
		}
	}

	inline size_t solve_equation( double a, double b, double c, double d, double &v1, double &v2, double &v3 )
	{
		if( a == 0.0 )
		{
			return( 0 );
		}
		else
		{
			return( solve_equation( ( - b * b / ( 3.0 * a ) + c ) / a, ( 2.0 * b * b * b / ( 27.0 * a * a ) - b * c / ( 3.0 * a ) + d ) / a, v1, v2, v3 ) );
		}
	}
}


/// @}
// �{�����[�������_�����O�O���[�v�̏I���

/// @}
//  �����O���[�v�̏I���



// �l��ԃ^�C�v�̃{�����[�������_�����O
namespace value_interpolation
{
	template < class Array1, class Array2, class DepthMap, class T >
	bool volumerendering( const Array1 &in, Array2 &out, const DepthMap &depth_map, const volumerender::parameter &p, const volumerender::attribute_table< T > &table, typename Array1::size_type thread_id, typename Array1::size_type thread_num )
	{
		typedef typename volumerender::parameter::vector_type vector_type;
		typedef typename volumerender::attribute_table< T >::attribute_type attribute_type;
		typedef typename volumerender::attribute_table< T >::pixel_type pixel_type;
		typedef typename Array1::size_type size_type;
		typedef typename Array1::difference_type difference_type;
		typedef typename Array1::value_type value_type;
		typedef typename Array1::const_pointer const_pointer;
		typedef typename Array2::value_type out_value_type;

		vector_type pos = p.pos;
		vector_type dir = p.dir.unit( );
		vector_type up = p.up.unit( );
		vector_type offset = p.offset;
		double fovy = p.fovy;
		double ambient_ratio = p.ambient_ratio;
		double diffuse_ratio = p.diffuse_ratio;
		double specular = p.specular;
		bool   bSpecular = specular > 0.0;
		const volumerender::boundingbox *box = p.box;
		double lightAtten = p.light_attenuation;
		double sampling_step = p.sampling_step;
		double termination = p.termination;
		double distortion = p.distortion;
		bool bdistortion = distortion != 0.0;
		bool bperspective = p.perspective_view;

		const size_type w = in.width( );
		const size_type h = in.height( );
		const size_type d = in.depth( );

		const size_type image_width  = out.width( );
		const size_type image_height = out.height( );

		difference_type d0, d1, d2, d3, d4, d5, d6, d7, _1, _2, _3;
		{
			difference_type cx = in.width( ) / 2;
			difference_type cy = in.height( ) / 2;
			difference_type cz = in.depth( ) / 2;
			const_pointer ppp = &in( cx, cy, cz );
			d0 = 0;
			d1 = &in( cx    , cy + 1, cz     ) - ppp;
			d2 = &in( cx + 1, cy + 1, cz     ) - ppp;
			d3 = &in( cx + 1, cy    , cz     ) - ppp;
			d4 = &in( cx    , cy    , cz + 1 ) - ppp;
			d5 = &in( cx    , cy + 1, cz + 1 ) - ppp;
			d6 = &in( cx + 1, cy + 1, cz + 1 ) - ppp;
			d7 = &in( cx + 1, cy    , cz + 1 ) - ppp;
			_1 = &in( cx + 1, cy    , cz     ) - ppp;
			_2 = &in( cx    , cy + 1, cz     ) - ppp;
			_3 = &in( cx    , cy    , cz + 1 ) - ppp;
		}

		// �X���C�X���W�n�̎��������[���h�ƍl����
		vector_type normal, n0, n1, n2, n3, n4, n5, n6, n7;
		vector_type casting_start, casting_end;

		const double pai = 3.1415926535897932384626433832795;
		double focal = ( static_cast< double >( image_height ) / 2.0 ) / std::tan( fovy * pai / 180.0 / 2.0 );

		double cx = static_cast< double >( image_width ) / 2.0;
		double cy = static_cast< double >( image_height ) / 2.0;
		double ax = in.reso1( );
		double ay = in.reso2( );
		double az = in.reso3( );

		double asp = out.reso2( ) / out.reso1( );

		double masp = ax < ay ? ax : ay;
		masp = masp < az ? masp : az;

		vector_type yoko = ( dir * up ).unit( );

		if( out.reso1( ) < out.reso2( ) )
		{
			yoko *= out.reso1( ) / out.reso2( );
		}
		else
		{
			up *= out.reso2( ) / out.reso1( );
			focal *= out.reso2( ) / out.reso1( );
		}

		double max_distance = pos.length( ) + std::sqrt( static_cast< double >( w * w + h * h + d * d ) );

		for( size_type j = thread_id ; j < image_height ; j += thread_num )
		{
			for( size_type i = 0 ; i < image_width ; i++ )
			{
				// ���e�ʏ�̓_���J�������W�n�ɕϊ�
				vector_type Pos( static_cast< double >( i ) - cx, cy - static_cast< double >( j ), focal );

				// �c�֐���K�p����
				if( bdistortion )
				{
					double x = Pos.x / cx;
					double y = Pos.y / cy * asp;
					double ll = x * x + y * y;
					double r  = 1.0 + distortion * ll;
					Pos.x *= r;
					Pos.y *= r;
				}

				// ���C�������J�������W�n���烏�[���h���W�n�ɕϊ�
				vector_type light;
				if( bperspective )
				{
					light = ( yoko * Pos.x + up * Pos.y + dir * Pos.z ).unit( );
				}
				else
				{
					pos = p.pos + yoko * Pos.x + up * Pos.y;
					light = dir;
				}

				pixel_type add_intensity( 0 );
				double add_opacity = 1;

				casting_start = pos;
				casting_end = pos + light * max_distance;

				// ���̂Ƃ̏Փ˔���
				if( volumerender::check_intersection( casting_start, casting_end, box[ 0 ] )
					&& volumerender::check_intersection( casting_start, casting_end, box[ 1 ] )
					&& volumerender::check_intersection( casting_start, casting_end, box[ 2 ] )
					&& volumerender::check_intersection( casting_start, casting_end, box[ 3 ] )
					&& volumerender::check_intersection( casting_start, casting_end, box[ 4 ] )
					&& volumerender::check_intersection( casting_start, casting_end, box[ 5 ] ) )
				{
					// ���̌������������邽�߂ɁC�J��������̋����𑪂�
					Pos.x = (  pos.x + offset.x ) / ax;
					Pos.y = ( -pos.y + offset.y ) / ay;
					Pos.z = (  pos.z + offset.z ) / az;

					// ���[���h���W�n�i����j����X���C�X���W�n�i�E��j�ɕϊ�
					// �ȍ~�́C�S�ăX���C�X���W�n�Ōv�Z����
					casting_start.x = (  casting_start.x + offset.x ) / ax;
					casting_start.y = ( -casting_start.y + offset.y ) / ay;
					casting_start.z = (  casting_start.z + offset.z ) / az;
					casting_end.x   = (  casting_end.x   + offset.x ) / ax;
					casting_end.y   = ( -casting_end.y   + offset.y ) / ay;
					casting_end.z   = (  casting_end.z   + offset.z ) / az;

					vector_type spos = casting_start;
					vector_type ray = ( casting_end - casting_start ).unit( );

					// ���̌����̋����������ɒ������߂̃p�����[�^
					double dlen = vector_type( ray.x * ax, ray.y * ay, ray.z * az ).length( );

					// �����̉�f�̉摜��ł͕����ɂ���ăT���v�����O�Ԋu���ς���Ă��܂����ɑΉ�
					double ray_sampling_step = sampling_step * masp / dlen;

					vector_type ray_step = ray * ray_sampling_step;

					double n = ( casting_end - casting_start ).length( );
					double l = 0, of = ( Pos - casting_start ).length( );

					while( l < n )
					{
						difference_type si = volumerender::to_integer( spos.x );
						difference_type sj = volumerender::to_integer( spos.y );
						difference_type sk = volumerender::to_integer( spos.z );

						const_pointer p = &in( si, sj, sk );

						// ���̈ʒu�ɂ����镨�̂��s�����̏ꍇ�͎��̃X�e�b�v�ֈڍs����
						if( table.has_alpha( p[ d0 ] ) || table.has_alpha( p[ d1 ] ) || table.has_alpha( p[ d2 ] ) ||
							table.has_alpha( p[ d3 ] ) || table.has_alpha( p[ d4 ] ) || table.has_alpha( p[ d5 ] ) ||
							table.has_alpha( p[ d6 ] ) || table.has_alpha( p[ d7 ] ) )
						{
							if( l > 0 )
							{
								spos.x -= ray.x;
								spos.y -= ray.y;
								spos.z -= ray.z;
								l -= 1.0;
							}
							break;
						}

						double current_step = depth_map( si, sj, sk );
						l += current_step;
						spos.x += ray.x * current_step;
						spos.y += ray.y * current_step;
						spos.z += ray.z * current_step;
					}

					while( l < n )
					{
						difference_type si = volumerender::to_integer( spos.x );
						difference_type sj = volumerender::to_integer( spos.y );
						difference_type sk = volumerender::to_integer( spos.z );

						double xx = spos.x - si;
						double yy = spos.y - sj;
						double zz = spos.z - sk;

						const_pointer p = &in( si, sj, sk );
						double ct;

						// CT�l�ɑΉ�����F�ƕs�����x���擾
						ct = ( p[ d0 ] + ( p[ d3 ] - p[ d0 ] ) * xx ) + ( p[ d1 ] - p[ d0 ] + ( p[ d0 ] - p[ d1 ] + p[ d2 ] - p[ d3 ] ) * xx ) * yy;
						ct += ( ( p[ d4 ] + ( p[ d7 ] - p[ d4 ] ) * xx ) + ( p[ d5 ] - p[ d4 ] + ( p[ d4 ] - p[ d5 ] + p[ d6 ] - p[ d7 ] ) * xx ) * yy - ct ) * zz;

						const attribute_type &oc = table[ volumerender::to_integer( ct ) ];

						// ���̈ʒu�ɂ����镨�̂������̏ꍇ�͎��̃X�e�b�v�ֈڍs����
						if( !oc.has_alpha )
						{
							spos += ray_step;
							l += ray_sampling_step;

							size_t count = 0;
							while( l < n )
							{
								difference_type si = volumerender::to_integer( spos.x );
								difference_type sj = volumerender::to_integer( spos.y );
								difference_type sk = volumerender::to_integer( spos.z );

								const_pointer p = &in( si, sj, sk );

								// ���̈ʒu�ɂ����镨�̂��s�����̏ꍇ�͎��̃X�e�b�v�ֈڍs����
								if( table.has_alpha( p[ d0 ] ) || table.has_alpha( p[ d1 ] ) || table.has_alpha( p[ d2 ] ) ||
									table.has_alpha( p[ d3 ] ) || table.has_alpha( p[ d4 ] ) || table.has_alpha( p[ d5 ] ) ||
									table.has_alpha( p[ d6 ] ) || table.has_alpha( p[ d7 ] ) )
								{
									if( count > 0 )
									{
										spos.x -= ray.x;
										spos.y -= ray.y;
										spos.z -= ray.z;
										l -= 1.0;
									}
									break;
								}

								double current_step = depth_map( si, sj, sk );
								l += current_step;
								spos.x += ray.x * current_step;
								spos.y += ray.y * current_step;
								spos.z += ray.z * current_step;

								count++;
							}

							continue;
						}

						const_pointer p0 = p;
						const_pointer p1 = p0 + d1;
						const_pointer p2 = p0 + d2;
						const_pointer p3 = p0 + d3;
						const_pointer p4 = p0 + d4;
						const_pointer p5 = p0 + d5;
						const_pointer p6 = p0 + d6;
						const_pointer p7 = p0 + d7;

						n0.x = p3[  0  ] - p0[ -_1 ];
						n0.y = p0[ -_2 ] - p1[  0  ];
						n0.z = p4[  0  ] - p0[ -_3 ];
						n1.x = p2[  0  ] - p1[ -_1 ];
						n1.y = p0[  0  ] - p1[  _2 ];
						n1.z = p5[  0  ] - p1[ -_3 ];
						n2.x = p2[  _1 ] - p1[  0  ];
						n2.y = p3[  0  ] - p2[  _2 ];
						n2.z = p6[  0  ] - p2[ -_3 ];
						n3.x = p3[  _1 ] - p0[  0  ];
						n3.y = p3[ -_2 ] - p2[  0  ];
						n3.z = p7[  0  ] - p3[ -_3 ];
						n4.x = p7[  0  ] - p4[ -_1 ];
						n4.y = p4[ -_2 ] - p5[  0  ];
						n4.z = p4[  _3 ] - p0[  0  ];
						n5.x = p6[  0  ] - p5[ -_1 ];
						n5.y = p4[  0  ] - p5[  _2 ];
						n5.z = p5[  _3 ] - p1[  0  ];
						n6.x = p6[  _1 ] - p5[  0  ];
						n6.y = p7[  0  ] - p6[  _2 ];
						n6.z = p6[  _3 ] - p2[  0  ];
						n7.x = p7[  _1 ] - p4[  0  ];
						n7.y = p7[ -_2 ] - p6[  0  ];
						n7.z = p7[  _3 ] - p3[  0  ];

						normal.x  = ( n0.x + ( n3.x - n0.x ) * xx ) + ( n1.x - n0.x + ( n0.x - n1.x + n2.x - n3.x ) * xx ) * yy;
						normal.x += ( ( n4.x + ( n7.x - n4.x ) * xx ) + ( n5.x - n4.x + ( n4.x - n5.x + n6.x - n7.x ) * xx ) * yy - normal.x ) * zz;
						normal.y  = ( n0.y + ( n3.y - n0.y ) * xx ) + ( n1.y - n0.y + ( n0.y - n1.y + n2.y - n3.y ) * xx ) * yy;
						normal.y += ( ( n4.y + ( n7.y - n4.y ) * xx ) + ( n5.y - n4.y + ( n4.y - n5.y + n6.y - n7.y ) * xx ) * yy - normal.y ) * zz;
						normal.z  = ( n0.z + ( n3.z - n0.z ) * xx ) + ( n1.z - n0.z + ( n0.z - n1.z + n2.z - n3.z ) * xx ) * yy;
						normal.z += ( ( n4.z + ( n7.z - n4.z ) * xx ) + ( n5.z - n4.z + ( n4.z - n5.z + n6.z - n7.z ) * xx ) * yy - normal.z ) * zz;

						normal.x /= ax;
						normal.y /= ay;
						normal.z /= az;
						double len = std::sqrt( normal.x * normal.x + normal.y * normal.y + normal.z * normal.z ) + type_limits< double >::minimum( );
						normal.x /= len;
						normal.y /= len;
						normal.z /= len;

						double lAtten = 1.0;
						if( lightAtten > 0.0 )
						{
							double len = ( l + of ) * dlen;
							lAtten /= 1.0 + lightAtten * ( len * len );
						}

						double c = light.inner( normal );
						c = c < 0.0 ? -c : c;

						double spec = 0.0;
						if( bSpecular )
						{
							spec = 2.0 * c * c - 1.0;

							if( spec <= 0.0 )
							{
								spec = 0;
							}
							else
							{
								spec *= spec;	//  2��
								spec *= spec;	//  4��
								spec *= spec;	//  8��
								spec *= spec;	// 16��
								spec *= spec;	// 32��
								spec *= spec;	// 64��
								//spec *= spec;	// 128��
								spec *= specular * 255.0;
							}
						}

						c = c * diffuse_ratio + ambient_ratio;

						double alpha = oc.alpha * sampling_step;
						add_intensity += alpha * add_opacity * ( oc.pixel * c + spec ) * lAtten;
						add_opacity *= ( 1.0 - alpha );

						// ��f�������_�����O���ʂɗ^����e�����������l�ȉ��ɂȂ����ꍇ�͏I��
						if( add_opacity < termination )
						{
							break;
						}

						spos.x += ray_step.x;
						spos.y += ray_step.y;
						spos.z += ray_step.z;
						l += ray_sampling_step;
					}
					out( i, j ) = static_cast< out_value_type >( mist::limits_0_255( add_intensity ) );
				}
				else
				{
					out( i, j ) = 0;
				}
			}
		}
		return( true );
	}

	template < class Array1, class Array2, class Array3, class DepthMap, class T >
	bool volumerendering( const Array1 &in, const Array2 &mk, Array3 &out, const DepthMap &depth_map, const volumerender::parameter &p, const volumerender::attribute_table< T > &table, const volumerender::attribute_table< T > &mktable, typename Array1::size_type thread_id, typename Array1::size_type thread_num )
	{
		typedef typename volumerender::parameter::vector_type vector_type;
		typedef typename volumerender::attribute_table< T >::attribute_type attribute_type;
		typedef typename volumerender::attribute_table< T >::pixel_type pixel_type;
		typedef typename Array1::size_type size_type;
		typedef typename Array1::difference_type difference_type;
		typedef typename Array1::value_type value_type;
		typedef typename Array1::const_pointer const_pointer;
		typedef typename Array2::const_pointer const_mk_pointer;
		typedef typename Array3::value_type out_value_type;

		vector_type pos = p.pos;
		vector_type dir = p.dir.unit( );
		vector_type up = p.up.unit( );
		vector_type offset = p.offset;
		double fovy = p.fovy;
		double ambient_ratio = p.ambient_ratio;
		double diffuse_ratio = p.diffuse_ratio;
		double specular = p.specular;
		bool   bSpecular = specular > 0.0;
		const volumerender::boundingbox *box = p.box;
		double lightAtten = p.light_attenuation;
		double sampling_step = p.sampling_step;
		double termination = p.termination;
		double distortion = p.distortion;
		bool bdistortion = distortion != 0.0;
		bool bperspective = p.perspective_view;

		const size_type w = in.width( );
		const size_type h = in.height( );
		const size_type d = in.depth( );

		const size_type image_width  = out.width( );
		const size_type image_height = out.height( );

		difference_type d0, d1, d2, d3, d4, d5, d6, d7, _1, _2, _3;
		{
			difference_type cx = in.width( ) / 2;
			difference_type cy = in.height( ) / 2;
			difference_type cz = in.depth( ) / 2;
			const_pointer ppp = &in( cx, cy, cz );
			d0 = 0;
			d1 = &in( cx    , cy + 1, cz     ) - ppp;
			d2 = &in( cx + 1, cy + 1, cz     ) - ppp;
			d3 = &in( cx + 1, cy    , cz     ) - ppp;
			d4 = &in( cx    , cy    , cz + 1 ) - ppp;
			d5 = &in( cx    , cy + 1, cz + 1 ) - ppp;
			d6 = &in( cx + 1, cy + 1, cz + 1 ) - ppp;
			d7 = &in( cx + 1, cy    , cz + 1 ) - ppp;
			_1 = &in( cx + 1, cy    , cz     ) - ppp;
			_2 = &in( cx    , cy + 1, cz     ) - ppp;
			_3 = &in( cx    , cy    , cz + 1 ) - ppp;
		}

		// �X���C�X���W�n�̎��������[���h�ƍl����
		vector_type normal, n0, n1, n2, n3, n4, n5, n6, n7;
		vector_type casting_start, casting_end;

		const double pai = 3.1415926535897932384626433832795;
		double focal = ( static_cast< double >( image_height ) / 2.0 ) / std::tan( fovy * pai / 180.0 / 2.0 );

		double cx = static_cast< double >( image_width ) / 2.0;
		double cy = static_cast< double >( image_height ) / 2.0;
		double ax = in.reso1( );
		double ay = in.reso2( );
		double az = in.reso3( );

		double asp = out.reso2( ) / out.reso1( );

		double masp = ax < ay ? ax : ay;
		masp = masp < az ? masp : az;

		vector_type yoko = ( dir * up ).unit( );

		if( out.reso1( ) < out.reso2( ) )
		{
			yoko *= out.reso1( ) / out.reso2( );
		}
		else
		{
			up *= out.reso2( ) / out.reso1( );
			focal *= out.reso2( ) / out.reso1( );
		}

		double max_distance = pos.length( ) + std::sqrt( static_cast< double >( w * w + h * h + d * d ) );

		for( size_type j = thread_id ; j < image_height ; j += thread_num )
		{
			for( size_type i = 0 ; i < image_width ; i++ )
			{
				// ���e�ʏ�̓_���J�������W�n�ɕϊ�
				vector_type Pos( static_cast< double >( i ) - cx, cy - static_cast< double >( j ), focal );

				// �c�֐���K�p����
				if( bdistortion )
				{
					double x = Pos.x / cx;
					double y = Pos.y / cy * asp;
					double ll = x * x + y * y;
					double r  = 1.0 + distortion * ll;
					Pos.x *= r;
					Pos.y *= r;
				}

				// ���C�������J�������W�n���烏�[���h���W�n�ɕϊ�
				vector_type light;
				if( bperspective )
				{
					light = ( yoko * Pos.x + up * Pos.y + dir * Pos.z ).unit( );
				}
				else
				{
					pos = p.pos + yoko * Pos.x + up * Pos.y;
					light = dir;
				}

				pixel_type add_intensity( 0 );
				double add_opacity = 1;

				casting_start = pos;
				casting_end = pos + light * max_distance;

				// ���̂Ƃ̏Փ˔���
				if( volumerender::check_intersection( casting_start, casting_end, box[ 0 ] )
					&& volumerender::check_intersection( casting_start, casting_end, box[ 1 ] )
					&& volumerender::check_intersection( casting_start, casting_end, box[ 2 ] )
					&& volumerender::check_intersection( casting_start, casting_end, box[ 3 ] )
					&& volumerender::check_intersection( casting_start, casting_end, box[ 4 ] )
					&& volumerender::check_intersection( casting_start, casting_end, box[ 5 ] ) )
				{
					// ���̌������������邽�߂ɁC�J��������̋����𑪂�
					Pos.x = (  pos.x + offset.x ) / ax;
					Pos.y = ( -pos.y + offset.y ) / ay;
					Pos.z = (  pos.z + offset.z ) / az;

					// ���[���h���W�n�i����j����X���C�X���W�n�i�E��j�ɕϊ�
					// �ȍ~�́C�S�ăX���C�X���W�n�Ōv�Z����
					casting_start.x = (  casting_start.x + offset.x ) / ax;
					casting_start.y = ( -casting_start.y + offset.y ) / ay;
					casting_start.z = (  casting_start.z + offset.z ) / az;
					casting_end.x   = (  casting_end.x   + offset.x ) / ax;
					casting_end.y   = ( -casting_end.y   + offset.y ) / ay;
					casting_end.z   = (  casting_end.z   + offset.z ) / az;

					vector_type spos = casting_start;
					vector_type ray = ( casting_end - casting_start ).unit( );

					// ���̌����̋����������ɒ������߂̃p�����[�^
					double dlen = vector_type( ray.x * ax, ray.y * ay, ray.z * az ).length( );

					// �����̉�f�̉摜��ł͕����ɂ���ăT���v�����O�Ԋu���ς���Ă��܂����ɑΉ�
					double ray_sampling_step = sampling_step * masp / dlen;

					vector_type ray_step = ray * ray_sampling_step;

					double n = ( casting_end - casting_start ).length( );
					double l = 0, of = ( Pos - casting_start ).length( );

					while( l < n )
					{
						difference_type si = volumerender::to_integer( spos.x );
						difference_type sj = volumerender::to_integer( spos.y );
						difference_type sk = volumerender::to_integer( spos.z );

						const_pointer p = &in( si, sj, sk );

						// ���̈ʒu�ɂ����镨�̂��s�����̏ꍇ�͎��̃X�e�b�v�ֈڍs����
						if( table.has_alpha( p[ d0 ] ) || table.has_alpha( p[ d1 ] ) || table.has_alpha( p[ d2 ] ) ||
							table.has_alpha( p[ d3 ] ) || table.has_alpha( p[ d4 ] ) || table.has_alpha( p[ d5 ] ) ||
							table.has_alpha( p[ d6 ] ) || table.has_alpha( p[ d7 ] ) )
						{
							if( l > 0 )
							{
								spos.x -= ray.x;
								spos.y -= ray.y;
								spos.z -= ray.z;
								l -= 1.0;
							}
							break;
						}

						double current_step = depth_map( si, sj, sk );
						l += current_step;
						spos.x += ray.x * current_step;
						spos.y += ray.y * current_step;
						spos.z += ray.z * current_step;
					}

					while( l < n )
					{
						difference_type si = volumerender::to_integer( spos.x );
						difference_type sj = volumerender::to_integer( spos.y );
						difference_type sk = volumerender::to_integer( spos.z );

						double xx = spos.x - si;
						double yy = spos.y - sj;
						double zz = spos.z - sk;

						const_pointer p  = &in( si, sj, sk );
						double ct;

						// CT�l�ɑΉ�����F�ƕs�����x���擾
						ct = ( p[ d0 ] + ( p[ d3 ] - p[ d0 ] ) * xx ) + ( p[ d1 ] - p[ d0 ] + ( p[ d0 ] - p[ d1 ] + p[ d2 ] - p[ d3 ] ) * xx ) * yy;
						ct += ( ( p[ d4 ] + ( p[ d7 ] - p[ d4 ] ) * xx ) + ( p[ d5 ] - p[ d4 ] + ( p[ d4 ] - p[ d5 ] + p[ d6 ] - p[ d7 ] ) * xx ) * yy - ct ) * zz;

						attribute_type oc = table[ volumerender::to_integer( ct ) ];

						// ���̈ʒu�ɂ����镨�̂������̏ꍇ�͎��̃X�e�b�v�ֈڍs����
						if( !oc.has_alpha )
						{
							spos += ray_step;
							l += ray_sampling_step;

							size_t count = 0;
							while( l < n )
							{
								difference_type si = volumerender::to_integer( spos.x );
								difference_type sj = volumerender::to_integer( spos.y );
								difference_type sk = volumerender::to_integer( spos.z );

								const_pointer p = &in( si, sj, sk );

								// ���̈ʒu�ɂ����镨�̂��s�����̏ꍇ�͎��̃X�e�b�v�ֈڍs����
								if( table.has_alpha( p[ d0 ] ) || table.has_alpha( p[ d1 ] ) || table.has_alpha( p[ d2 ] ) ||
									table.has_alpha( p[ d3 ] ) || table.has_alpha( p[ d4 ] ) || table.has_alpha( p[ d5 ] ) ||
									table.has_alpha( p[ d6 ] ) || table.has_alpha( p[ d7 ] ) )
								{
									if( count > 0 )
									{
										spos.x -= ray.x;
										spos.y -= ray.y;
										spos.z -= ray.z;
										l -= 1.0;
									}
									break;
								}

								double current_step = depth_map( si, sj, sk );
								l += current_step;
								spos.x += ray.x * current_step;
								spos.y += ray.y * current_step;
								spos.z += ray.z * current_step;

								count++;
							}

							continue;
						}

						// �}�[�L���O���ʂ��d�˂�
						const_mk_pointer pm = &mk( si, sj, sk );

						// �ߖT��8�_�ɑΉ�����F�ƕs�����x���擾
						attribute_type mc0 = mktable[ volumerender::to_integer( pm[ d0 ] ) ];
						attribute_type mc1 = mktable[ volumerender::to_integer( pm[ d1 ] ) ];
						attribute_type mc2 = mktable[ volumerender::to_integer( pm[ d2 ] ) ];
						attribute_type mc3 = mktable[ volumerender::to_integer( pm[ d3 ] ) ];
						attribute_type mc4 = mktable[ volumerender::to_integer( pm[ d4 ] ) ];
						attribute_type mc5 = mktable[ volumerender::to_integer( pm[ d5 ] ) ];
						attribute_type mc6 = mktable[ volumerender::to_integer( pm[ d6 ] ) ];
						attribute_type mc7 = mktable[ volumerender::to_integer( pm[ d7 ] ) ];

						int number_of_visible_voxels = mc0.has_alpha;
						number_of_visible_voxels += mc1.has_alpha;
						number_of_visible_voxels += mc2.has_alpha;
						number_of_visible_voxels += mc3.has_alpha;
						number_of_visible_voxels += mc4.has_alpha;
						number_of_visible_voxels += mc5.has_alpha;
						number_of_visible_voxels += mc6.has_alpha;
						number_of_visible_voxels += mc7.has_alpha;

						if( number_of_visible_voxels > 0 )
						{
							attribute_type mc;
							// �܂����ϓI�ȐF�����肷��
							mc.pixel = ( mc0.pixel * mc0.has_alpha + mc1.pixel * mc1.has_alpha
										+ mc2.pixel * mc2.has_alpha + mc3.pixel * mc3.has_alpha
										+ mc4.pixel * mc4.has_alpha + mc5.pixel * mc5.has_alpha
										+ mc6.pixel * mc6.has_alpha + mc7.pixel * mc7.has_alpha ) / static_cast< double >( number_of_visible_voxels );

							// �������̂��������ꍇ�́C����̕s�������̂̐F�Œu�������邱�Ƃł���������
							if( !mc0.has_alpha ){ mc0.pixel = mc.pixel; }
							if( !mc1.has_alpha ){ mc1.pixel = mc.pixel; }
							if( !mc2.has_alpha ){ mc2.pixel = mc.pixel; }
							if( !mc3.has_alpha ){ mc3.pixel = mc.pixel; }
							if( !mc4.has_alpha ){ mc4.pixel = mc.pixel; }
							if( !mc5.has_alpha ){ mc5.pixel = mc.pixel; }
							if( !mc6.has_alpha ){ mc6.pixel = mc.pixel; }
							if( !mc7.has_alpha ){ mc7.pixel = mc.pixel; }

							mc.pixel = ( mc0.pixel + ( mc3.pixel - mc0.pixel ) * xx ) + ( mc1.pixel - mc0.pixel + ( mc0.pixel - mc1.pixel + mc2.pixel - mc3.pixel ) * xx ) * yy;
							mc.pixel = mc.pixel + ( ( mc4.pixel + ( mc7.pixel - mc4.pixel ) * xx ) + ( mc5.pixel - mc4.pixel + ( mc4.pixel - mc5.pixel + mc6.pixel - mc7.pixel ) * xx ) * yy - mc.pixel ) * zz;
							mc.alpha = ( mc0.alpha + ( mc3.alpha - mc0.alpha ) * xx ) + ( mc1.alpha - mc0.alpha + ( mc0.alpha - mc1.alpha + mc2.alpha - mc3.alpha ) * xx ) * yy;
							mc.alpha = mc.alpha + ( ( mc4.alpha + ( mc7.alpha - mc4.alpha ) * xx ) + ( mc5.alpha - mc4.alpha + ( mc4.alpha - mc5.alpha + mc6.alpha - mc7.alpha ) * xx ) * yy - mc.alpha ) * zz;

							// �}�[�L���O�̐F����������
							oc.pixel = oc.pixel * ( 1.0 - mc.alpha ) + mc.pixel * mc.alpha;
						}

						// �@���x�N�g�����v�Z
						const_pointer p0 = p;
						const_pointer p1 = p0 + d1;
						const_pointer p2 = p0 + d2;
						const_pointer p3 = p0 + d3;
						const_pointer p4 = p0 + d4;
						const_pointer p5 = p0 + d5;
						const_pointer p6 = p0 + d6;
						const_pointer p7 = p0 + d7;

						n0.x = p3[  0  ] - p0[ -_1 ];
						n0.y = p0[ -_2 ] - p1[  0  ];
						n0.z = p4[  0  ] - p0[ -_3 ];
						n1.x = p2[  0  ] - p1[ -_1 ];
						n1.y = p0[  0  ] - p1[  _2 ];
						n1.z = p5[  0  ] - p1[ -_3 ];
						n2.x = p2[  _1 ] - p1[  0  ];
						n2.y = p3[  0  ] - p2[  _2 ];
						n2.z = p6[  0  ] - p2[ -_3 ];
						n3.x = p3[  _1 ] - p0[  0  ];
						n3.y = p3[ -_2 ] - p2[  0  ];
						n3.z = p7[  0  ] - p3[ -_3 ];
						n4.x = p7[  0  ] - p4[ -_1 ];
						n4.y = p4[ -_2 ] - p5[  0  ];
						n4.z = p4[  _3 ] - p0[  0  ];
						n5.x = p6[  0  ] - p5[ -_1 ];
						n5.y = p4[  0  ] - p5[  _2 ];
						n5.z = p5[  _3 ] - p1[  0  ];
						n6.x = p6[  _1 ] - p5[  0  ];
						n6.y = p7[  0  ] - p6[  _2 ];
						n6.z = p6[  _3 ] - p2[  0  ];
						n7.x = p7[  _1 ] - p4[  0  ];
						n7.y = p7[ -_2 ] - p6[  0  ];
						n7.z = p7[  _3 ] - p3[  0  ];

						normal.x  = ( n0.x + ( n3.x - n0.x ) * xx ) + ( n1.x - n0.x + ( n0.x - n1.x + n2.x - n3.x ) * xx ) * yy;
						normal.x += ( ( n4.x + ( n7.x - n4.x ) * xx ) + ( n5.x - n4.x + ( n4.x - n5.x + n6.x - n7.x ) * xx ) * yy - normal.x ) * zz;
						normal.y  = ( n0.y + ( n3.y - n0.y ) * xx ) + ( n1.y - n0.y + ( n0.y - n1.y + n2.y - n3.y ) * xx ) * yy;
						normal.y += ( ( n4.y + ( n7.y - n4.y ) * xx ) + ( n5.y - n4.y + ( n4.y - n5.y + n6.y - n7.y ) * xx ) * yy - normal.y ) * zz;
						normal.z  = ( n0.z + ( n3.z - n0.z ) * xx ) + ( n1.z - n0.z + ( n0.z - n1.z + n2.z - n3.z ) * xx ) * yy;
						normal.z += ( ( n4.z + ( n7.z - n4.z ) * xx ) + ( n5.z - n4.z + ( n4.z - n5.z + n6.z - n7.z ) * xx ) * yy - normal.z ) * zz;

						normal.x /= ax;
						normal.y /= ay;
						normal.z /= az;
						double len = std::sqrt( normal.x * normal.x + normal.y * normal.y + normal.z * normal.z ) + type_limits< double >::minimum( );
						normal.x /= len;
						normal.y /= len;
						normal.z /= len;

						double lAtten = 1.0;
						if( lightAtten > 0.0 )
						{
							double len = ( l + of ) * dlen;
							lAtten /= 1.0 + lightAtten * ( len * len );
						}

						double c = light.inner( normal );
						c = c < 0.0 ? -c : c;

						double spec = 0.0;
						if( bSpecular )
						{
							spec = 2.0 * c * c - 1.0;

							if( spec <= 0.0 )
							{
								spec = 0;
							}
							else
							{
								spec *= spec;	//  2��
								spec *= spec;	//  4��
								spec *= spec;	//  8��
								spec *= spec;	// 16��
								spec *= spec;	// 32��
								spec *= spec;	// 64��
								//spec *= spec;	// 128��
								spec *= specular * 255.0;
							}
						}

						c = c * diffuse_ratio + ambient_ratio;

						double alpha = oc.alpha * sampling_step;
						add_intensity += alpha * add_opacity * ( oc.pixel * c + spec ) * lAtten;
						add_opacity *= ( 1.0 - alpha );

						// ��f�������_�����O���ʂɗ^����e�����������l�ȉ��ɂȂ����ꍇ�͏I��
						if( add_opacity < termination )
						{
							break;
						}

						spos.x += ray_step.x;
						spos.y += ray_step.y;
						spos.z += ray_step.z;
						l += ray_sampling_step;
					}
					out( i, j ) = static_cast< out_value_type >( mist::limits_0_255( add_intensity ) );
				}
				else
				{
					out( i, j ) = 0;
				}
			}
		}
		return( true );
	}



	template < class Array1, class Array2, class Array3, class DepthMap, class T >
	bool volumerendering( const Array1 &in, const Array2 &mk, Array3 &out, const DepthMap &depth_map, const volumerender::parameter &p, const volumerender::attribute_table< T > &table, const volumerender::attribute_table< T > &mktable, bool apply_and_operation, typename Array1::size_type thread_id, typename Array1::size_type thread_num )
	{
		typedef typename volumerender::parameter::vector_type vector_type;
		typedef typename volumerender::attribute_table< T >::attribute_type attribute_type;
		typedef typename volumerender::attribute_table< T >::pixel_type pixel_type;
		typedef typename Array1::size_type size_type;
		typedef typename Array1::difference_type difference_type;
		typedef typename Array1::value_type value_type;
		typedef typename Array1::const_pointer const_pointer;
		typedef typename Array2::const_pointer const_mk_pointer;
		typedef typename Array3::value_type out_value_type;

		vector_type pos = p.pos;
		vector_type dir = p.dir.unit( );
		vector_type up = p.up.unit( );
		vector_type offset = p.offset;
		double fovy = p.fovy;
		double ambient_ratio = p.ambient_ratio;
		double diffuse_ratio = p.diffuse_ratio;
		double specular = p.specular;
		bool   bSpecular = specular > 0.0;
		const volumerender::boundingbox *box = p.box;
		double lightAtten = p.light_attenuation;
		double sampling_step = p.sampling_step;
		double termination = p.termination;
		double distortion = p.distortion;
		bool bdistortion = distortion != 0.0;
		bool bperspective = p.perspective_view;

		const size_type w = in.width( );
		const size_type h = in.height( );
		const size_type d = in.depth( );

		const size_type image_width  = out.width( );
		const size_type image_height = out.height( );

		difference_type d0, d1, d2, d3, d4, d5, d6, d7, _1, _2, _3;
		{
			difference_type cx = in.width( ) / 2;
			difference_type cy = in.height( ) / 2;
			difference_type cz = in.depth( ) / 2;
			const_pointer ppp = &in( cx, cy, cz );
			d0 = 0;
			d1 = &in( cx    , cy + 1, cz     ) - ppp;
			d2 = &in( cx + 1, cy + 1, cz     ) - ppp;
			d3 = &in( cx + 1, cy    , cz     ) - ppp;
			d4 = &in( cx    , cy    , cz + 1 ) - ppp;
			d5 = &in( cx    , cy + 1, cz + 1 ) - ppp;
			d6 = &in( cx + 1, cy + 1, cz + 1 ) - ppp;
			d7 = &in( cx + 1, cy    , cz + 1 ) - ppp;
			_1 = &in( cx + 1, cy    , cz     ) - ppp;
			_2 = &in( cx    , cy + 1, cz     ) - ppp;
			_3 = &in( cx    , cy    , cz + 1 ) - ppp;
		}

		// �X���C�X���W�n�̎��������[���h�ƍl����
		vector_type normal, n0, n1, n2, n3, n4, n5, n6, n7;
		vector_type casting_start, casting_end;

		const double pai = 3.1415926535897932384626433832795;
		double focal = ( static_cast< double >( image_height ) / 2.0 ) / std::tan( fovy * pai / 180.0 / 2.0 );

		double cx = static_cast< double >( image_width ) / 2.0;
		double cy = static_cast< double >( image_height ) / 2.0;
		double ax = in.reso1( );
		double ay = in.reso2( );
		double az = in.reso3( );

		double asp = out.reso2( ) / out.reso1( );

		double masp = ax < ay ? ax : ay;
		masp = masp < az ? masp : az;

		vector_type yoko = ( dir * up ).unit( );

		if( out.reso1( ) < out.reso2( ) )
		{
			yoko *= out.reso1( ) / out.reso2( );
		}
		else
		{
			up *= out.reso2( ) / out.reso1( );
			focal *= out.reso2( ) / out.reso1( );
		}

		double max_distance = pos.length( ) + std::sqrt( static_cast< double >( w * w + h * h + d * d ) );

		for( size_type j = thread_id ; j < image_height ; j += thread_num )
		{
			for( size_type i = 0 ; i < image_width ; i++ )
			{
				// ���e�ʏ�̓_���J�������W�n�ɕϊ�
				vector_type Pos( static_cast< double >( i ) - cx, cy - static_cast< double >( j ), focal );

				// �c�֐���K�p����
				if( bdistortion )
				{
					double x = Pos.x / cx;
					double y = Pos.y / cy * asp;
					double ll = x * x + y * y;
					double r  = 1.0 + distortion * ll;
					Pos.x *= r;
					Pos.y *= r;
				}

				// ���C�������J�������W�n���烏�[���h���W�n�ɕϊ�
				vector_type light;
				if( bperspective )
				{
					light = ( yoko * Pos.x + up * Pos.y + dir * Pos.z ).unit( );
				}
				else
				{
					pos = p.pos + yoko * Pos.x + up * Pos.y;
					light = dir;
				}

				pixel_type add_intensity( 0 );
				double add_opacity = 1;

				casting_start = pos;
				casting_end = pos + light * max_distance;

				// ���̂Ƃ̏Փ˔���
				if( volumerender::check_intersection( casting_start, casting_end, box[ 0 ] )
					&& volumerender::check_intersection( casting_start, casting_end, box[ 1 ] )
					&& volumerender::check_intersection( casting_start, casting_end, box[ 2 ] )
					&& volumerender::check_intersection( casting_start, casting_end, box[ 3 ] )
					&& volumerender::check_intersection( casting_start, casting_end, box[ 4 ] )
					&& volumerender::check_intersection( casting_start, casting_end, box[ 5 ] ) )
				{
					// ���̌������������邽�߂ɁC�J��������̋����𑪂�
					Pos.x = (  pos.x + offset.x ) / ax;
					Pos.y = ( -pos.y + offset.y ) / ay;
					Pos.z = (  pos.z + offset.z ) / az;

					// ���[���h���W�n�i����j����X���C�X���W�n�i�E��j�ɕϊ�
					// �ȍ~�́C�S�ăX���C�X���W�n�Ōv�Z����
					casting_start.x = (  casting_start.x + offset.x ) / ax;
					casting_start.y = ( -casting_start.y + offset.y ) / ay;
					casting_start.z = (  casting_start.z + offset.z ) / az;
					casting_end.x   = (  casting_end.x   + offset.x ) / ax;
					casting_end.y   = ( -casting_end.y   + offset.y ) / ay;
					casting_end.z   = (  casting_end.z   + offset.z ) / az;

					vector_type spos = casting_start;
					vector_type ray = ( casting_end - casting_start ).unit( );

					// ���̌����̋����������ɒ������߂̃p�����[�^
					double dlen = vector_type( ray.x * ax, ray.y * ay, ray.z * az ).length( );

					// �����̉�f�̉摜��ł͕����ɂ���ăT���v�����O�Ԋu���ς���Ă��܂����ɑΉ�
					double ray_sampling_step = sampling_step * masp / dlen;

					vector_type ray_step = ray * ray_sampling_step;

					double n = ( casting_end - casting_start ).length( );
					double l = 0, of = ( Pos - casting_start ).length( );

					if( apply_and_operation )
					{
						while( l < n )
						{
							difference_type si = volumerender::to_integer( spos.x );
							difference_type sj = volumerender::to_integer( spos.y );
							difference_type sk = volumerender::to_integer( spos.z );

							const_pointer p = &in( si, sj, sk );
							const_mk_pointer pm = &mk( si, sj, sk );

							// ���̈ʒu�ɂ����镨�̂��s�����̏ꍇ�͎��̃X�e�b�v�ֈڍs����
							if( ( table.has_alpha( p[ d0 ] ) || table.has_alpha( p[ d1 ] ) || table.has_alpha( p[ d2 ] ) ||
								  table.has_alpha( p[ d3 ] ) || table.has_alpha( p[ d4 ] ) || table.has_alpha( p[ d5 ] ) ||
								  table.has_alpha( p[ d6 ] ) || table.has_alpha( p[ d7 ] ) ) && 
								( mktable.has_alpha( pm[ d0 ] ) || mktable.has_alpha( pm[ d1 ] ) || mktable.has_alpha( pm[ d2 ] ) ||
								  mktable.has_alpha( pm[ d3 ] ) || mktable.has_alpha( pm[ d4 ] ) || mktable.has_alpha( pm[ d5 ] ) ||
								  mktable.has_alpha( pm[ d6 ] ) || mktable.has_alpha( pm[ d7 ] ) ) )
							{
								if( l > 0 )
								{
									spos.x -= ray.x;
									spos.y -= ray.y;
									spos.z -= ray.z;
									l -= 1.0;
								}
								break;
							}

							double current_step = depth_map( si, sj, sk );
							l += current_step;
							spos.x += ray.x * current_step;
							spos.y += ray.y * current_step;
							spos.z += ray.z * current_step;
						}
					}
					else
					{
						while( l < n )
						{
							difference_type si = volumerender::to_integer( spos.x );
							difference_type sj = volumerender::to_integer( spos.y );
							difference_type sk = volumerender::to_integer( spos.z );

							const_pointer p = &in( si, sj, sk );
							const_mk_pointer pm = &mk( si, sj, sk );

							// ���̈ʒu�ɂ����镨�̂��s�����̏ꍇ�͎��̃X�e�b�v�ֈڍs����
							if( ( table.has_alpha( p[ d0 ] ) || table.has_alpha( p[ d1 ] ) || table.has_alpha( p[ d2 ] ) ||
								  table.has_alpha( p[ d3 ] ) || table.has_alpha( p[ d4 ] ) || table.has_alpha( p[ d5 ] ) ||
								  table.has_alpha( p[ d6 ] ) || table.has_alpha( p[ d7 ] ) ) || 
								( mktable.has_alpha( pm[ d0 ] ) || mktable.has_alpha( pm[ d1 ] ) || mktable.has_alpha( pm[ d2 ] ) ||
								  mktable.has_alpha( pm[ d3 ] ) || mktable.has_alpha( pm[ d4 ] ) || mktable.has_alpha( pm[ d5 ] ) ||
								  mktable.has_alpha( pm[ d6 ] ) || mktable.has_alpha( pm[ d7 ] ) ) )
							{
								if( l > 0 )
								{
									spos.x -= ray.x;
									spos.y -= ray.y;
									spos.z -= ray.z;
									l -= 1.0;
								}
								break;
							}

							double current_step = depth_map( si, sj, sk );
							l += current_step;
							spos.x += ray.x * current_step;
							spos.y += ray.y * current_step;
							spos.z += ray.z * current_step;
						}
					}

					while( l < n )
					{
						difference_type si = volumerender::to_integer( spos.x );
						difference_type sj = volumerender::to_integer( spos.y );
						difference_type sk = volumerender::to_integer( spos.z );

						double xx = spos.x - si;
						double yy = spos.y - sj;
						double zz = spos.z - sk;

						const_pointer p  = &in( si, sj, sk );
						double ct;

						// CT�l�ɑΉ�����F�ƕs�����x���擾
						ct = ( p[ d0 ] + ( p[ d3 ] - p[ d0 ] ) * xx ) + ( p[ d1 ] - p[ d0 ] + ( p[ d0 ] - p[ d1 ] + p[ d2 ] - p[ d3 ] ) * xx ) * yy;
						ct += ( ( p[ d4 ] + ( p[ d7 ] - p[ d4 ] ) * xx ) + ( p[ d5 ] - p[ d4 ] + ( p[ d4 ] - p[ d5 ] + p[ d6 ] - p[ d7 ] ) * xx ) * yy - ct ) * zz;

						attribute_type oc = table[ volumerender::to_integer( ct ) ];

						// �}�[�L���O���ʂ��d�˂�
						const_mk_pointer pm = &mk( si, sj, sk );

						// �ߖT��8�_�ɑΉ�����F�ƕs�����x���擾
						attribute_type mc0 = mktable[ volumerender::to_integer( pm[ d0 ] ) ];
						attribute_type mc1 = mktable[ volumerender::to_integer( pm[ d1 ] ) ];
						attribute_type mc2 = mktable[ volumerender::to_integer( pm[ d2 ] ) ];
						attribute_type mc3 = mktable[ volumerender::to_integer( pm[ d3 ] ) ];
						attribute_type mc4 = mktable[ volumerender::to_integer( pm[ d4 ] ) ];
						attribute_type mc5 = mktable[ volumerender::to_integer( pm[ d5 ] ) ];
						attribute_type mc6 = mktable[ volumerender::to_integer( pm[ d6 ] ) ];
						attribute_type mc7 = mktable[ volumerender::to_integer( pm[ d7 ] ) ];

						int number_of_visible_voxels = mc0.has_alpha;
						number_of_visible_voxels += mc1.has_alpha;
						number_of_visible_voxels += mc2.has_alpha;
						number_of_visible_voxels += mc3.has_alpha;
						number_of_visible_voxels += mc4.has_alpha;
						number_of_visible_voxels += mc5.has_alpha;
						number_of_visible_voxels += mc6.has_alpha;
						number_of_visible_voxels += mc7.has_alpha;

						// ���̈ʒu�ɂ����镨�̂������̏ꍇ�͎��̃X�e�b�v�ֈڍs����
						if( apply_and_operation )
						{
							if( !oc.has_alpha || number_of_visible_voxels == 0 )
							{
								spos += ray_step;
								l += ray_sampling_step;

								size_t count = 0;
								while( l < n )
								{
									difference_type si = volumerender::to_integer( spos.x );
									difference_type sj = volumerender::to_integer( spos.y );
									difference_type sk = volumerender::to_integer( spos.z );

									const_pointer p = &in( si, sj, sk );
									const_mk_pointer pm = &mk( si, sj, sk );

									// ���̈ʒu�ɂ����镨�̂��s�����̏ꍇ�͎��̃X�e�b�v�ֈڍs����
									if( ( table.has_alpha( p[ d0 ] ) || table.has_alpha( p[ d1 ] ) || table.has_alpha( p[ d2 ] ) ||
										table.has_alpha( p[ d3 ] ) || table.has_alpha( p[ d4 ] ) || table.has_alpha( p[ d5 ] ) ||
										table.has_alpha( p[ d6 ] ) || table.has_alpha( p[ d7 ] ) ) && 
										( mktable.has_alpha( pm[ d0 ] ) || mktable.has_alpha( pm[ d1 ] ) || mktable.has_alpha( pm[ d2 ] ) ||
										mktable.has_alpha( pm[ d3 ] ) || mktable.has_alpha( pm[ d4 ] ) || mktable.has_alpha( pm[ d5 ] ) ||
										mktable.has_alpha( pm[ d6 ] ) || mktable.has_alpha( pm[ d7 ] ) ) )
									{
										if( count > 0 )
										{
											spos.x -= ray.x;
											spos.y -= ray.y;
											spos.z -= ray.z;
											l -= 1.0;
										}
										break;
									}

									double current_step = depth_map( si, sj, sk );
									l += current_step;
									spos.x += ray.x * current_step;
									spos.y += ray.y * current_step;
									spos.z += ray.z * current_step;

									count++;
								}

								continue;
							}
						}
						else
						{
							if( !oc.has_alpha && number_of_visible_voxels == 0 )
							{
								spos += ray_step;
								l += ray_sampling_step;

								size_t count = 0;
								while( l < n )
								{
									difference_type si = volumerender::to_integer( spos.x );
									difference_type sj = volumerender::to_integer( spos.y );
									difference_type sk = volumerender::to_integer( spos.z );

									const_pointer p = &in( si, sj, sk );
									const_mk_pointer pm = &mk( si, sj, sk );

									// ���̈ʒu�ɂ����镨�̂��s�����̏ꍇ�͎��̃X�e�b�v�ֈڍs����
									if( ( table.has_alpha( p[ d0 ] ) || table.has_alpha( p[ d1 ] ) || table.has_alpha( p[ d2 ] ) ||
										table.has_alpha( p[ d3 ] ) || table.has_alpha( p[ d4 ] ) || table.has_alpha( p[ d5 ] ) ||
										table.has_alpha( p[ d6 ] ) || table.has_alpha( p[ d7 ] ) ) || 
										( mktable.has_alpha( pm[ d0 ] ) || mktable.has_alpha( pm[ d1 ] ) || mktable.has_alpha( pm[ d2 ] ) ||
										mktable.has_alpha( pm[ d3 ] ) || mktable.has_alpha( pm[ d4 ] ) || mktable.has_alpha( pm[ d5 ] ) ||
										mktable.has_alpha( pm[ d6 ] ) || mktable.has_alpha( pm[ d7 ] ) ) )
									{
										if( count > 0 )
										{
											spos.x -= ray.x;
											spos.y -= ray.y;
											spos.z -= ray.z;
											l -= 1.0;
										}
										break;
									}

									double current_step = depth_map( si, sj, sk );
									l += current_step;
									spos.x += ray.x * current_step;
									spos.y += ray.y * current_step;
									spos.z += ray.z * current_step;

									count++;
								}

								continue;
							}
						}

						if( number_of_visible_voxels > 0 )
						{
							attribute_type mc;
							// �܂����ϓI�ȐF�����肷��
							mc.pixel = ( mc0.pixel * mc0.has_alpha + mc1.pixel * mc1.has_alpha
										+ mc2.pixel * mc2.has_alpha + mc3.pixel * mc3.has_alpha
										+ mc4.pixel * mc4.has_alpha + mc5.pixel * mc5.has_alpha
										+ mc6.pixel * mc6.has_alpha + mc7.pixel * mc7.has_alpha ) / static_cast< double >( number_of_visible_voxels );

							// �������̂��������ꍇ�́C����̕s�������̂̐F�Œu�������邱�Ƃł���������
							if( !mc0.has_alpha ){ mc0.pixel = mc.pixel; }
							if( !mc1.has_alpha ){ mc1.pixel = mc.pixel; }
							if( !mc2.has_alpha ){ mc2.pixel = mc.pixel; }
							if( !mc3.has_alpha ){ mc3.pixel = mc.pixel; }
							if( !mc4.has_alpha ){ mc4.pixel = mc.pixel; }
							if( !mc5.has_alpha ){ mc5.pixel = mc.pixel; }
							if( !mc6.has_alpha ){ mc6.pixel = mc.pixel; }
							if( !mc7.has_alpha ){ mc7.pixel = mc.pixel; }

							mc.pixel = ( mc0.pixel + ( mc3.pixel - mc0.pixel ) * xx ) + ( mc1.pixel - mc0.pixel + ( mc0.pixel - mc1.pixel + mc2.pixel - mc3.pixel ) * xx ) * yy;
							mc.pixel = mc.pixel + ( ( mc4.pixel + ( mc7.pixel - mc4.pixel ) * xx ) + ( mc5.pixel - mc4.pixel + ( mc4.pixel - mc5.pixel + mc6.pixel - mc7.pixel ) * xx ) * yy - mc.pixel ) * zz;
							mc.alpha = ( mc0.alpha + ( mc3.alpha - mc0.alpha ) * xx ) + ( mc1.alpha - mc0.alpha + ( mc0.alpha - mc1.alpha + mc2.alpha - mc3.alpha ) * xx ) * yy;
							mc.alpha = mc.alpha + ( ( mc4.alpha + ( mc7.alpha - mc4.alpha ) * xx ) + ( mc5.alpha - mc4.alpha + ( mc4.alpha - mc5.alpha + mc6.alpha - mc7.alpha ) * xx ) * yy - mc.alpha ) * zz;

							// �}�[�L���O�̐F����������
							oc.pixel = oc.pixel * ( 1.0 - mc.alpha ) + mc.pixel * mc.alpha;
							oc.alpha = ( oc.alpha + mc.alpha );
						}

						// �@���x�N�g�����v�Z
						if( oc.has_alpha )
						{
							const_pointer p0 = p;
							const_pointer p1 = p0 + d1;
							const_pointer p2 = p0 + d2;
							const_pointer p3 = p0 + d3;
							const_pointer p4 = p0 + d4;
							const_pointer p5 = p0 + d5;
							const_pointer p6 = p0 + d6;
							const_pointer p7 = p0 + d7;

							n0.x = p3[  0  ] - p0[ -_1 ];
							n0.y = p0[ -_2 ] - p1[  0  ];
							n0.z = p4[  0  ] - p0[ -_3 ];
							n1.x = p2[  0  ] - p1[ -_1 ];
							n1.y = p0[  0  ] - p1[  _2 ];
							n1.z = p5[  0  ] - p1[ -_3 ];
							n2.x = p2[  _1 ] - p1[  0  ];
							n2.y = p3[  0  ] - p2[  _2 ];
							n2.z = p6[  0  ] - p2[ -_3 ];
							n3.x = p3[  _1 ] - p0[  0  ];
							n3.y = p3[ -_2 ] - p2[  0  ];
							n3.z = p7[  0  ] - p3[ -_3 ];
							n4.x = p7[  0  ] - p4[ -_1 ];
							n4.y = p4[ -_2 ] - p5[  0  ];
							n4.z = p4[  _3 ] - p0[  0  ];
							n5.x = p6[  0  ] - p5[ -_1 ];
							n5.y = p4[  0  ] - p5[  _2 ];
							n5.z = p5[  _3 ] - p1[  0  ];
							n6.x = p6[  _1 ] - p5[  0  ];
							n6.y = p7[  0  ] - p6[  _2 ];
							n6.z = p6[  _3 ] - p2[  0  ];
							n7.x = p7[  _1 ] - p4[  0  ];
							n7.y = p7[ -_2 ] - p6[  0  ];
							n7.z = p7[  _3 ] - p3[  0  ];
						}
						else
						{
							const_mk_pointer p0 = pm;
							const_mk_pointer p1 = p0 + d1;
							const_mk_pointer p2 = p0 + d2;
							const_mk_pointer p3 = p0 + d3;
							const_mk_pointer p4 = p0 + d4;
							const_mk_pointer p5 = p0 + d5;
							const_mk_pointer p6 = p0 + d6;
							const_mk_pointer p7 = p0 + d7;

							n0.x = p3[  0  ] - p0[ -_1 ];
							n0.y = p0[ -_2 ] - p1[  0  ];
							n0.z = p4[  0  ] - p0[ -_3 ];
							n1.x = p2[  0  ] - p1[ -_1 ];
							n1.y = p0[  0  ] - p1[  _2 ];
							n1.z = p5[  0  ] - p1[ -_3 ];
							n2.x = p2[  _1 ] - p1[  0  ];
							n2.y = p3[  0  ] - p2[  _2 ];
							n2.z = p6[  0  ] - p2[ -_3 ];
							n3.x = p3[  _1 ] - p0[  0  ];
							n3.y = p3[ -_2 ] - p2[  0  ];
							n3.z = p7[  0  ] - p3[ -_3 ];
							n4.x = p7[  0  ] - p4[ -_1 ];
							n4.y = p4[ -_2 ] - p5[  0  ];
							n4.z = p4[  _3 ] - p0[  0  ];
							n5.x = p6[  0  ] - p5[ -_1 ];
							n5.y = p4[  0  ] - p5[  _2 ];
							n5.z = p5[  _3 ] - p1[  0  ];
							n6.x = p6[  _1 ] - p5[  0  ];
							n6.y = p7[  0  ] - p6[  _2 ];
							n6.z = p6[  _3 ] - p2[  0  ];
							n7.x = p7[  _1 ] - p4[  0  ];
							n7.y = p7[ -_2 ] - p6[  0  ];
							n7.z = p7[  _3 ] - p3[  0  ];
						}

						normal.x  = ( n0.x + ( n3.x - n0.x ) * xx ) + ( n1.x - n0.x + ( n0.x - n1.x + n2.x - n3.x ) * xx ) * yy;
						normal.x += ( ( n4.x + ( n7.x - n4.x ) * xx ) + ( n5.x - n4.x + ( n4.x - n5.x + n6.x - n7.x ) * xx ) * yy - normal.x ) * zz;
						normal.y  = ( n0.y + ( n3.y - n0.y ) * xx ) + ( n1.y - n0.y + ( n0.y - n1.y + n2.y - n3.y ) * xx ) * yy;
						normal.y += ( ( n4.y + ( n7.y - n4.y ) * xx ) + ( n5.y - n4.y + ( n4.y - n5.y + n6.y - n7.y ) * xx ) * yy - normal.y ) * zz;
						normal.z  = ( n0.z + ( n3.z - n0.z ) * xx ) + ( n1.z - n0.z + ( n0.z - n1.z + n2.z - n3.z ) * xx ) * yy;
						normal.z += ( ( n4.z + ( n7.z - n4.z ) * xx ) + ( n5.z - n4.z + ( n4.z - n5.z + n6.z - n7.z ) * xx ) * yy - normal.z ) * zz;

						normal.x /= ax;
						normal.y /= ay;
						normal.z /= az;
						double len = std::sqrt( normal.x * normal.x + normal.y * normal.y + normal.z * normal.z ) + type_limits< double >::minimum( );
						normal.x /= len;
						normal.y /= len;
						normal.z /= len;

						double lAtten = 1.0;
						if( lightAtten > 0.0 )
						{
							double len = ( l + of ) * dlen;
							lAtten /= 1.0 + lightAtten * ( len * len );
						}

						double c = light.inner( normal );
						c = c < 0.0 ? -c : c;

						double spec = 0.0;
						if( bSpecular )
						{
							spec = 2.0 * c * c - 1.0;

							if( spec <= 0.0 )
							{
								spec = 0;
							}
							else
							{
								spec *= spec;	//  2��
								spec *= spec;	//  4��
								spec *= spec;	//  8��
								spec *= spec;	// 16��
								spec *= spec;	// 32��
								spec *= spec;	// 64��
								//spec *= spec;	// 128��
								spec *= specular * 255.0;
							}
						}

						c = c * diffuse_ratio + ambient_ratio;

						double alpha = oc.alpha * sampling_step;
						add_intensity += alpha * add_opacity * ( oc.pixel * c + spec ) * lAtten;
						add_opacity *= ( 1.0 - alpha );

						// ��f�������_�����O���ʂɗ^����e�����������l�ȉ��ɂȂ����ꍇ�͏I��
						if( add_opacity < termination )
						{
							break;
						}

						spos.x += ray_step.x;
						spos.y += ray_step.y;
						spos.z += ray_step.z;
						l += ray_sampling_step;
					}
					out( i, j ) = static_cast< out_value_type >( mist::limits_0_255( add_intensity ) );
				}
				else
				{
					out( i, j ) = 0;
				}
			}
		}
		return( true );
	}
}



// �F��ԃ^�C�v�̃{�����[�������_�����O
namespace color_interpolation
{
	template < class Array1, class Array2, class DepthMap, class T >
	bool volumerendering( const Array1 &in, Array2 &out, const DepthMap &depth_map, const volumerender::parameter &p, const volumerender::attribute_table< T > &table, typename Array1::size_type thread_id, typename Array1::size_type thread_num )
	{
		typedef typename volumerender::parameter::vector_type vector_type;
		typedef typename volumerender::attribute_table< T >::attribute_type attribute_type;
		typedef typename volumerender::attribute_table< T >::pixel_type pixel_type;
		typedef typename Array1::size_type size_type;
		typedef typename Array1::difference_type difference_type;
		typedef typename Array1::value_type value_type;
		typedef typename Array1::const_pointer const_pointer;
		typedef typename Array2::value_type out_value_type;

		vector_type pos = p.pos;
		vector_type dir = p.dir.unit( );
		vector_type up = p.up.unit( );
		vector_type offset = p.offset;
		double fovy = p.fovy;
		double ambient_ratio = p.ambient_ratio;
		double diffuse_ratio = p.diffuse_ratio;
		double specular = p.specular;
		bool   bSpecular = specular > 0.0;
		const volumerender::boundingbox *box = p.box;
		double lightAtten = p.light_attenuation;
		double sampling_step = p.sampling_step;
		double termination = p.termination;
		double distortion = p.distortion;
		bool bdistortion = distortion != 0.0;
		bool bperspective = p.perspective_view;

		const size_type w = in.width( );
		const size_type h = in.height( );
		const size_type d = in.depth( );

		const size_type image_width  = out.width( );
		const size_type image_height = out.height( );

		difference_type d0, d1, d2, d3, d4, d5, d6, d7, _1, _2, _3;
		{
			difference_type cx = in.width( ) / 2;
			difference_type cy = in.height( ) / 2;
			difference_type cz = in.depth( ) / 2;
			const_pointer ppp = &in( cx, cy, cz );
			d0 = 0;
			d1 = &in( cx    , cy + 1, cz     ) - ppp;
			d2 = &in( cx + 1, cy + 1, cz     ) - ppp;
			d3 = &in( cx + 1, cy    , cz     ) - ppp;
			d4 = &in( cx    , cy    , cz + 1 ) - ppp;
			d5 = &in( cx    , cy + 1, cz + 1 ) - ppp;
			d6 = &in( cx + 1, cy + 1, cz + 1 ) - ppp;
			d7 = &in( cx + 1, cy    , cz + 1 ) - ppp;
			_1 = &in( cx + 1, cy    , cz     ) - ppp;
			_2 = &in( cx    , cy + 1, cz     ) - ppp;
			_3 = &in( cx    , cy    , cz + 1 ) - ppp;
		}

		// �X���C�X���W�n�̎��������[���h�ƍl����
		vector_type normal, n0, n1, n2, n3, n4, n5, n6, n7;
		vector_type casting_start, casting_end;

		const double pai = 3.1415926535897932384626433832795;
		double focal = ( static_cast< double >( image_height ) / 2.0 ) / std::tan( fovy * pai / 180.0 / 2.0 );

		double cx = static_cast< double >( image_width ) / 2.0;
		double cy = static_cast< double >( image_height ) / 2.0;
		double ax = in.reso1( );
		double ay = in.reso2( );
		double az = in.reso3( );

		double asp = out.reso2( ) / out.reso1( );

		double masp = ax < ay ? ax : ay;
		masp = masp < az ? masp : az;

		vector_type yoko = ( dir * up ).unit( );

		if( out.reso1( ) < out.reso2( ) )
		{
			yoko *= out.reso1( ) / out.reso2( );
		}
		else
		{
			up *= out.reso2( ) / out.reso1( );
			focal *= out.reso2( ) / out.reso1( );
		}

		double max_distance = pos.length( ) + std::sqrt( static_cast< double >( w * w + h * h + d * d ) );

		for( size_type j = thread_id ; j < image_height ; j += thread_num )
		{
			for( size_type i = 0 ; i < image_width ; i++ )
			{
				// ���e�ʏ�̓_���J�������W�n�ɕϊ�
				vector_type Pos( static_cast< double >( i ) - cx, cy - static_cast< double >( j ), focal );

				// �c�֐���K�p����
				if( bdistortion )
				{
					double x = Pos.x / cx;
					double y = Pos.y / cy * asp;
					double ll = x * x + y * y;
					double r  = 1.0 + distortion * ll;
					Pos.x *= r;
					Pos.y *= r;
				}

				// ���C�������J�������W�n���烏�[���h���W�n�ɕϊ�
				vector_type light;
				if( bperspective )
				{
					light = ( yoko * Pos.x + up * Pos.y + dir * Pos.z ).unit( );
				}
				else
				{
					pos = p.pos + yoko * Pos.x + up * Pos.y;
					light = dir;
				}

				pixel_type add_intensity( 0 );
				double add_opacity = 1;

				casting_start = pos;
				casting_end = pos + light * max_distance;

				// ���̂Ƃ̏Փ˔���
				if( volumerender::check_intersection( casting_start, casting_end, box[ 0 ] )
					&& volumerender::check_intersection( casting_start, casting_end, box[ 1 ] )
					&& volumerender::check_intersection( casting_start, casting_end, box[ 2 ] )
					&& volumerender::check_intersection( casting_start, casting_end, box[ 3 ] )
					&& volumerender::check_intersection( casting_start, casting_end, box[ 4 ] )
					&& volumerender::check_intersection( casting_start, casting_end, box[ 5 ] ) )
				{
					// ���̌������������邽�߂ɁC�J��������̋����𑪂�
					Pos.x = (  pos.x + offset.x ) / ax;
					Pos.y = ( -pos.y + offset.y ) / ay;
					Pos.z = (  pos.z + offset.z ) / az;

					// ���[���h���W�n�i����j����X���C�X���W�n�i�E��j�ɕϊ�
					// �ȍ~�́C�S�ăX���C�X���W�n�Ōv�Z����
					casting_start.x = (  casting_start.x + offset.x ) / ax;
					casting_start.y = ( -casting_start.y + offset.y ) / ay;
					casting_start.z = (  casting_start.z + offset.z ) / az;
					casting_end.x   = (  casting_end.x   + offset.x ) / ax;
					casting_end.y   = ( -casting_end.y   + offset.y ) / ay;
					casting_end.z   = (  casting_end.z   + offset.z ) / az;

					vector_type spos = casting_start;
					vector_type ray = ( casting_end - casting_start ).unit( );

					// ���̌����̋����������ɒ������߂̃p�����[�^
					double dlen = vector_type( ray.x * ax, ray.y * ay, ray.z * az ).length( );

					// �����̉�f�̉摜��ł͕����ɂ���ăT���v�����O�Ԋu���ς���Ă��܂����ɑΉ�
					double ray_sampling_step = sampling_step * masp / dlen;

					vector_type ray_step = ray * ray_sampling_step;

					double n = ( casting_end - casting_start ).length( );
					double l = 0, of = ( Pos - casting_start ).length( );

					while( l < n )
					{
						difference_type si = volumerender::to_integer( spos.x );
						difference_type sj = volumerender::to_integer( spos.y );
						difference_type sk = volumerender::to_integer( spos.z );

						const_pointer p = &in( si, sj, sk );

						// ���̈ʒu�ɂ����镨�̂��s�����̏ꍇ�͎��̃X�e�b�v�ֈڍs����
						if( table.has_alpha( p[ d0 ] ) || table.has_alpha( p[ d1 ] ) || table.has_alpha( p[ d2 ] ) ||
							table.has_alpha( p[ d3 ] ) || table.has_alpha( p[ d4 ] ) || table.has_alpha( p[ d5 ] ) ||
							table.has_alpha( p[ d6 ] ) || table.has_alpha( p[ d7 ] ) )
						{
							if( l > 0 )
							{
								spos.x -= ray.x;
								spos.y -= ray.y;
								spos.z -= ray.z;
								l -= 1.0;
							}
							break;
						}

						double current_step = depth_map( si, sj, sk );
						l += current_step;
						spos.x += ray.x * current_step;
						spos.y += ray.y * current_step;
						spos.z += ray.z * current_step;
					}

					while( l < n )
					{
						difference_type si = volumerender::to_integer( spos.x );
						difference_type sj = volumerender::to_integer( spos.y );
						difference_type sk = volumerender::to_integer( spos.z );

						double xx = spos.x - si;
						double yy = spos.y - sj;
						double zz = spos.z - sk;

						const_pointer p = &in( si, sj, sk );

						// �ߖT��8�_�ɑΉ�����F�ƕs�����x���擾
						attribute_type oc0 = table[ volumerender::to_integer( p[ d0 ] ) ];
						attribute_type oc1 = table[ volumerender::to_integer( p[ d1 ] ) ];
						attribute_type oc2 = table[ volumerender::to_integer( p[ d2 ] ) ];
						attribute_type oc3 = table[ volumerender::to_integer( p[ d3 ] ) ];
						attribute_type oc4 = table[ volumerender::to_integer( p[ d4 ] ) ];
						attribute_type oc5 = table[ volumerender::to_integer( p[ d5 ] ) ];
						attribute_type oc6 = table[ volumerender::to_integer( p[ d6 ] ) ];
						attribute_type oc7 = table[ volumerender::to_integer( p[ d7 ] ) ];

						int number_of_visible_voxels = oc0.has_alpha;
						number_of_visible_voxels += oc1.has_alpha;
						number_of_visible_voxels += oc2.has_alpha;
						number_of_visible_voxels += oc3.has_alpha;
						number_of_visible_voxels += oc4.has_alpha;
						number_of_visible_voxels += oc5.has_alpha;
						number_of_visible_voxels += oc6.has_alpha;
						number_of_visible_voxels += oc7.has_alpha;

						// ���̈ʒu�ɂ����镨�̂������̏ꍇ�͎��̃X�e�b�v�ֈڍs����
						if( number_of_visible_voxels == 0 )
						{
							spos += ray_step;
							l += ray_sampling_step;

							size_t count = 0;
							while( l < n )
							{
								difference_type si = volumerender::to_integer( spos.x );
								difference_type sj = volumerender::to_integer( spos.y );
								difference_type sk = volumerender::to_integer( spos.z );

								const_pointer p = &in( si, sj, sk );

								// ���̈ʒu�ɂ����镨�̂��s�����̏ꍇ�͎��̃X�e�b�v�ֈڍs����
								if( table.has_alpha( p[ d0 ] ) || table.has_alpha( p[ d1 ] ) || table.has_alpha( p[ d2 ] ) ||
									table.has_alpha( p[ d3 ] ) || table.has_alpha( p[ d4 ] ) || table.has_alpha( p[ d5 ] ) ||
									table.has_alpha( p[ d6 ] ) || table.has_alpha( p[ d7 ] ) )
								{
									if( count > 0 )
									{
										spos.x -= ray.x;
										spos.y -= ray.y;
										spos.z -= ray.z;
										l -= 1.0;
									}
									break;
								}

								double current_step = depth_map( si, sj, sk );
								l += current_step;
								spos.x += ray.x * current_step;
								spos.y += ray.y * current_step;
								spos.z += ray.z * current_step;

								count++;
							}

							continue;
						}

						attribute_type oc;
						// �܂����ϓI�ȐF�����肷��
						oc.pixel = ( oc0.pixel * oc0.has_alpha + oc1.pixel * oc1.has_alpha
									+ oc2.pixel * oc2.has_alpha + oc3.pixel * oc3.has_alpha
									+ oc4.pixel * oc4.has_alpha + oc5.pixel * oc5.has_alpha
									+ oc6.pixel * oc6.has_alpha + oc7.pixel * oc7.has_alpha ) / static_cast< double >( number_of_visible_voxels );

						// �������̂��������ꍇ�́C����̕s�������̂̐F�Œu�������邱�Ƃł���������
						if( !oc0.has_alpha ){ oc0.pixel = oc.pixel; }
						if( !oc1.has_alpha ){ oc1.pixel = oc.pixel; }
						if( !oc2.has_alpha ){ oc2.pixel = oc.pixel; }
						if( !oc3.has_alpha ){ oc3.pixel = oc.pixel; }
						if( !oc4.has_alpha ){ oc4.pixel = oc.pixel; }
						if( !oc5.has_alpha ){ oc5.pixel = oc.pixel; }
						if( !oc6.has_alpha ){ oc6.pixel = oc.pixel; }
						if( !oc7.has_alpha ){ oc7.pixel = oc.pixel; }

						oc.pixel = ( oc0.pixel + ( oc3.pixel - oc0.pixel ) * xx ) + ( oc1.pixel - oc0.pixel + ( oc0.pixel - oc1.pixel + oc2.pixel - oc3.pixel ) * xx ) * yy;
						oc.pixel = oc.pixel + ( ( oc4.pixel + ( oc7.pixel - oc4.pixel ) * xx ) + ( oc5.pixel - oc4.pixel + ( oc4.pixel - oc5.pixel + oc6.pixel - oc7.pixel ) * xx ) * yy - oc.pixel ) * zz;
						oc.alpha = ( oc0.alpha + ( oc3.alpha - oc0.alpha ) * xx ) + ( oc1.alpha - oc0.alpha + ( oc0.alpha - oc1.alpha + oc2.alpha - oc3.alpha ) * xx ) * yy;
						oc.alpha = oc.alpha + ( ( oc4.alpha + ( oc7.alpha - oc4.alpha ) * xx ) + ( oc5.alpha - oc4.alpha + ( oc4.alpha - oc5.alpha + oc6.alpha - oc7.alpha ) * xx ) * yy - oc.alpha ) * zz;

						const_pointer p0 = p;
						const_pointer p1 = p0 + d1;
						const_pointer p2 = p0 + d2;
						const_pointer p3 = p0 + d3;
						const_pointer p4 = p0 + d4;
						const_pointer p5 = p0 + d5;
						const_pointer p6 = p0 + d6;
						const_pointer p7 = p0 + d7;

						n0.x = p3[  0  ] - p0[ -_1 ];
						n0.y = p0[ -_2 ] - p1[  0  ];
						n0.z = p4[  0  ] - p0[ -_3 ];
						n1.x = p2[  0  ] - p1[ -_1 ];
						n1.y = p0[  0  ] - p1[  _2 ];
						n1.z = p5[  0  ] - p1[ -_3 ];
						n2.x = p2[  _1 ] - p1[  0  ];
						n2.y = p3[  0  ] - p2[  _2 ];
						n2.z = p6[  0  ] - p2[ -_3 ];
						n3.x = p3[  _1 ] - p0[  0  ];
						n3.y = p3[ -_2 ] - p2[  0  ];
						n3.z = p7[  0  ] - p3[ -_3 ];
						n4.x = p7[  0  ] - p4[ -_1 ];
						n4.y = p4[ -_2 ] - p5[  0  ];
						n4.z = p4[  _3 ] - p0[  0  ];
						n5.x = p6[  0  ] - p5[ -_1 ];
						n5.y = p4[  0  ] - p5[  _2 ];
						n5.z = p5[  _3 ] - p1[  0  ];
						n6.x = p6[  _1 ] - p5[  0  ];
						n6.y = p7[  0  ] - p6[  _2 ];
						n6.z = p6[  _3 ] - p2[  0  ];
						n7.x = p7[  _1 ] - p4[  0  ];
						n7.y = p7[ -_2 ] - p6[  0  ];
						n7.z = p7[  _3 ] - p3[  0  ];

						normal.x  = ( n0.x + ( n3.x - n0.x ) * xx ) + ( n1.x - n0.x + ( n0.x - n1.x + n2.x - n3.x ) * xx ) * yy;
						normal.x += ( ( n4.x + ( n7.x - n4.x ) * xx ) + ( n5.x - n4.x + ( n4.x - n5.x + n6.x - n7.x ) * xx ) * yy - normal.x ) * zz;
						normal.y  = ( n0.y + ( n3.y - n0.y ) * xx ) + ( n1.y - n0.y + ( n0.y - n1.y + n2.y - n3.y ) * xx ) * yy;
						normal.y += ( ( n4.y + ( n7.y - n4.y ) * xx ) + ( n5.y - n4.y + ( n4.y - n5.y + n6.y - n7.y ) * xx ) * yy - normal.y ) * zz;
						normal.z  = ( n0.z + ( n3.z - n0.z ) * xx ) + ( n1.z - n0.z + ( n0.z - n1.z + n2.z - n3.z ) * xx ) * yy;
						normal.z += ( ( n4.z + ( n7.z - n4.z ) * xx ) + ( n5.z - n4.z + ( n4.z - n5.z + n6.z - n7.z ) * xx ) * yy - normal.z ) * zz;

						normal.x /= ax;
						normal.y /= ay;
						normal.z /= az;
						double len = std::sqrt( normal.x * normal.x + normal.y * normal.y + normal.z * normal.z ) + type_limits< double >::minimum( );
						normal.x /= len;
						normal.y /= len;
						normal.z /= len;

						double lAtten = 1.0;
						if( lightAtten > 0.0 )
						{
							double len = ( l + of ) * dlen;
							lAtten /= 1.0 + lightAtten * ( len * len );
						}

						double c = light.inner( normal );
						c = c < 0.0 ? -c : c;

						double spec = 0.0;
						if( bSpecular )
						{
							spec = 2.0 * c * c - 1.0;

							if( spec <= 0.0 )
							{
								spec = 0;
							}
							else
							{
								spec *= spec;	//  2��
								spec *= spec;	//  4��
								spec *= spec;	//  8��
								spec *= spec;	// 16��
								spec *= spec;	// 32��
								spec *= spec;	// 64��
								//spec *= spec;	// 128��
								spec *= specular * 255.0;
							}
						}

						c = c * diffuse_ratio + ambient_ratio;

						double alpha = oc.alpha * sampling_step;
						add_intensity += alpha * add_opacity * ( oc.pixel * c + spec ) * lAtten;
						add_opacity *= ( 1.0 - alpha );

						// ��f�������_�����O���ʂɗ^����e�����������l�ȉ��ɂȂ����ꍇ�͏I��
						if( add_opacity < termination )
						{
							break;
						}

						spos.x += ray_step.x;
						spos.y += ray_step.y;
						spos.z += ray_step.z;
						l += ray_sampling_step;
					}
					out( i, j ) = static_cast< out_value_type >( mist::limits_0_255( add_intensity ) );
				}
				else
				{
					out( i, j ) = 0;
				}
			}
		}
		return( true );
	}
}


// �{�����[�������_�����O�̃X���b�h����
namespace __volumerendering_controller__
{
	template < class Array1, class Array2, class DepthMap, class T >
	class volumerendering_thread : public mist::thread< volumerendering_thread< Array1, Array2, DepthMap, T > >
	{
	public:
		typedef mist::thread< volumerendering_thread< Array1, Array2, DepthMap, T > > base;
		typedef typename base::thread_exit_type thread_exit_type;
		typedef typename Array1::size_type size_type;
		typedef typename Array1::value_type value_type;

	private:
		size_t thread_id_;
		size_t thread_num_;

		// ���o�͗p�̉摜�ւ̃|�C���^
		const Array1 *in_;
		Array2 *out_;
		const DepthMap *depth_map_;
		const volumerender::parameter *param_;
		const volumerender::attribute_table< T > *table_;

	public:
		void setup_parameters( const Array1 &in, Array2 &out, const DepthMap &depth_map, const volumerender::parameter &p, const volumerender::attribute_table< T > &t, size_type thread_id, size_type thread_num )
		{
			in_  = &in;
			out_ = &out;
			depth_map_ = &depth_map;
			param_ = &p;
			table_ = &t;
			thread_id_ = thread_id;
			thread_num_ = thread_num;
		}

		volumerendering_thread( size_type id = 0, size_type num = 1 ) : thread_id_( id ), thread_num_( num ),
													in_( NULL ), out_( NULL ), depth_map_( NULL ), param_( NULL ), table_( NULL )
		{
		}
		volumerendering_thread( const volumerendering_thread &p ) : base( p ), thread_id_( p.thread_id_ ), thread_num_( p.thread_num_ ),
																		in_( p.in_ ), out_( p.out_ ), depth_map_( p.depth_map_ ),
																		param_( p.param_ ), table_( p.table_ )
		{
		}

	protected:
		// �p��������ŕK�����������X���b�h�֐�
		virtual thread_exit_type thread_function( )
		{
			if( param_->value_interpolation )
			{
				value_interpolation::volumerendering( *in_, *out_, *depth_map_, *param_, *table_, thread_id_, thread_num_ );
			}
			else
			{
				color_interpolation::volumerendering( *in_, *out_, *depth_map_, *param_, *table_, thread_id_, thread_num_ );
			}
			return( true );
		}
	};

	template < class Array1, class Array2, class Array3, class DepthMap, class T >
	class volumerendering_with_mark_thread : public mist::thread< volumerendering_with_mark_thread< Array1, Array2, Array3, DepthMap, T > >
	{
	public:
		typedef mist::thread< volumerendering_with_mark_thread< Array1, Array2, Array3, DepthMap, T > > base;
		typedef typename base::thread_exit_type thread_exit_type;
		typedef typename Array1::size_type size_type;
		typedef typename Array1::value_type value_type;

	private:
		size_t thread_id_;
		size_t thread_num_;

		// ���o�͗p�̉摜�ւ̃|�C���^
		const Array1 *in_;
		const Array2 *mk_;
		Array3 *out_;
		int mode_;
		const DepthMap *depth_map_;
		const volumerender::parameter *param_;
		const volumerender::attribute_table< T > *table_;
		const volumerender::attribute_table< T > *table_mk_;

	public:
		void setup_parameters( const Array1 &in, const Array2 &mk, Array3 &out, int mode, const DepthMap &depth_map, const volumerender::parameter &p, const volumerender::attribute_table< T > &t, const volumerender::attribute_table< T > &tmk, size_type thread_id, size_type thread_num )
		{
			in_  = &in;
			mk_  = &mk;
			out_ = &out;
			mode_ = mode;
			depth_map_ = &depth_map;
			param_ = &p;
			table_ = &t;
			table_mk_ = &tmk;
			thread_id_ = thread_id;
			thread_num_ = thread_num;
		}

		volumerendering_with_mark_thread( size_type id = 0, size_type num = 1 ) : thread_id_( id ), thread_num_( num ),
													in_( NULL ), out_( NULL ), depth_map_( NULL ), param_( NULL ), table_( NULL )
		{
		}
		volumerendering_with_mark_thread( const volumerendering_thread &p ) : base( p ), thread_id_( p.thread_id_ ), thread_num_( p.thread_num_ ),
																			in_( p.in_ ), mk_( p.mk_ ), out_( p.out_ ), mode_( p.mode_ ), depth_map_( p.depth_map_ ),
																			param_( p.param_ ), table_( p.table_ ), table_mk_( p.table_mk_ )
		{
		}

	protected:
		// �p��������ŕK�����������X���b�h�֐�
		virtual thread_exit_type thread_function( )
		{
			switch( mode_ )
			{
			case 1:
				value_interpolation::volumerendering( *in_, *mk_, *out_, *depth_map_, *param_, *table_, *table_mk_, true, thread_id_, thread_num_ );
				break;

			case 2:
				value_interpolation::volumerendering( *in_, *mk_, *out_, *depth_map_, *param_, *table_, *table_mk_, false, thread_id_, thread_num_ );
				break;

			case 0:
			default:
				value_interpolation::volumerendering( *in_, *mk_, *out_, *depth_map_, *param_, *table_, *table_mk_, thread_id_, thread_num_ );
				break;

			}
			return( true );
		}
	};
}



//! @addtogroup visualization_group
//!
//!  @{


//! @addtogroup volumerendering_group
//!
//! @{

/// @brief �{�����[�������_�����O
//! 
//! @attention ���͂Əo�͂́C�ʂ�MIST�R���e�i�I�u�W�F�N�g�łȂ��Ă͂Ȃ�Ȃ�
//! @attention �X���b�h����0���w�肵���ꍇ�́C�g�p�\��CPU���������I�Ɏ擾����
//!
//! @param[in]  in         �c ���͉摜
//! @param[out] out        �c �o�͉摜
//! @param[in]  dmap       �c �����_�����O�����������邽�߂̋����摜
//! @param[in]  param      �c �{�����[�������_�����O�̃p�����[�^
//! @param[in]  table      �c �{�����[�������_�����O�̐F�|�l�e�[�u��
//! @param[in]  thread_num �c �g�p����X���b�h��
//! 
//! @retval true  �c �{�����[�������_�����O�ɐ���
//! @retval false �c ���͂Əo�͂������I�u�W�F�N�g���w�肵���ꍇ
//! 
template < class Array1, class Array2, class DepthMap, class ATTRIBUTETYPE >
bool volumerendering( const Array1 &in, Array2 &out, const DepthMap &dmap, const volumerender::parameter &param, const volumerender::attribute_table< ATTRIBUTETYPE > &table, typename Array1::size_type thread_num = 0 )
{
	if( is_same_object( in, out ) || in.empty( ) )
	{
		return( false );
	}

	typedef typename Array1::size_type size_type;
	typedef __volumerendering_controller__::volumerendering_thread< Array1, Array2, DepthMap, ATTRIBUTETYPE > volumerendering_thread;

	if( thread_num == 0 )
	{
		thread_num = static_cast< size_type >( get_cpu_num( ) );
	}

	volumerendering_thread *thread = new volumerendering_thread[ thread_num ];

	size_type i;
	for( i = 0 ; i < thread_num ; i++ )
	{
		thread[ i ].setup_parameters( in, out, dmap, param, table, i, thread_num );
	}

	// �X���b�h�����s���āC�I���܂őҋ@����
	do_threads_( thread, thread_num );

	delete [] thread;
	
	return( true );
}


/// @brief �{�����[�������_�����O
//! 
//! @attention ���͂Əo�͂́C�ʂ�MIST�R���e�i�I�u�W�F�N�g�łȂ��Ă͂Ȃ�Ȃ�
//! @attention �X���b�h����0���w�肵���ꍇ�́C�g�p�\��CPU���������I�Ɏ擾����
//!
//! @param[in]  in         �c ���͉摜
//! @param[out] out        �c �o�͉摜
//! @param[in]  param      �c �{�����[�������_�����O�̃p�����[�^
//! @param[in]  table      �c �{�����[�������_�����O�̐F�|�l�e�[�u��
//! @param[in]  thread_num �c �g�p����X���b�h��
//! 
//! @retval true  �c �{�����[�������_�����O�ɐ���
//! @retval false �c ���͂Əo�͂������I�u�W�F�N�g���w�肵���ꍇ
//! 
template < class Array1, class Array2, class ATTRIBUTETYPE >
bool volumerendering( const Array1 &in, Array2 &out, const volumerender::parameter &param, const volumerender::attribute_table< ATTRIBUTETYPE > &table, typename Array1::size_type thread_num = 0 )
{
	return( volumerendering( in, out, volumerender::no_depth_map( ), param, table, thread_num ) );
}



/// @brief �{�����[�������_�����O
//! 
//! @attention ���͂Əo�͂́C�ʂ�MIST�R���e�i�I�u�W�F�N�g�łȂ��Ă͂Ȃ�Ȃ�
//! @attention �X���b�h����0���w�肵���ꍇ�́C�g�p�\��CPU���������I�Ɏ擾����
//!
//! @param[in]  in         �c ���͉摜
//! @param[in]  mk         �c �}�[�N�摜
//! @param[out] out        �c �o�͉摜
//! @param[in]  dmap       �c �����_�����O�����������邽�߂̋����摜
//! @param[in]  param      �c �{�����[�������_�����O�̃p�����[�^
//! @param[in]  table      �c �{�����[�������_�����O�̐F�|�l�e�[�u��
//! @param[in]  mktable    �c �}�[�N�f�[�^�p�̐F�|�l�e�[�u��
//! @param[in]  thread_num �c �g�p����X���b�h��
//! 
//! @retval true  �c �{�����[�������_�����O�ɐ���
//! @retval false �c ���͂Əo�͂������I�u�W�F�N�g���w�肵���ꍇ
//! 
template < class Array1, class Array2, class Array3, class DepthMap, class ATTRIBUTETYPE >
bool volumerendering( const Array1 &in, const Array2 &mk, Array3 &out, const DepthMap &dmap, const volumerender::parameter &param, const volumerender::attribute_table< ATTRIBUTETYPE > &table, const volumerender::attribute_table< ATTRIBUTETYPE > &mktable, typename Array1::size_type thread_num = 0 )
{
	if( is_same_object( in, out ) || in.empty( ) )
	{
		return( false );
	}
	else if( in.width( ) != mk.width( ) || in.height( ) != mk.height( ) || in.depth( ) != mk.depth( ) )
	{
	}

	typedef typename Array1::size_type size_type;
	typedef __volumerendering_controller__::volumerendering_with_mark_thread< Array1, Array2, Array3, DepthMap, ATTRIBUTETYPE > volumerendering_thread;

	if( thread_num == 0 )
	{
		thread_num = static_cast< size_type >( get_cpu_num( ) );
	}

	volumerendering_thread *thread = new volumerendering_thread[ thread_num ];

	size_type i;
	for( i = 0 ; i < thread_num ; i++ )
	{
		thread[ i ].setup_parameters( in, mk, out, 0, dmap, param, table, mktable, i, thread_num );
	}

	// �X���b�h�����s���āC�I���܂őҋ@����
	do_threads_( thread, thread_num );

	delete [] thread;
	
	return( true );
}



/// @brief �{�����[�������_�����O
//! 
//! @attention ���͂Əo�͂́C�ʂ�MIST�R���e�i�I�u�W�F�N�g�łȂ��Ă͂Ȃ�Ȃ�
//! @attention �X���b�h����0���w�肵���ꍇ�́C�g�p�\��CPU���������I�Ɏ擾����
//!
//! @param[in]  in                  �c ���͉摜
//! @param[in]  mk                  �c �}�[�N�摜
//! @param[out] out                 �c �o�͉摜
//! @param[in]  dmap                �c �����_�����O�����������邽�߂̋����摜
//! @param[in]  param               �c �{�����[�������_�����O�̃p�����[�^
//! @param[in]  table               �c �{�����[�������_�����O�̐F�|�l�e�[�u��
//! @param[in]  mktable             �c �}�[�N�f�[�^�p�̐F�|�l�e�[�u��
//! @param[in]  apply_and_operation �c CT�ƃ}�[�N�̋��ʕ����݂̂������_�����O���邩�C�a�W���������_�����O���邩
//! @param[in]  thread_num          �c �g�p����X���b�h��
//! 
//! @retval true  �c �{�����[�������_�����O�ɐ���
//! @retval false �c ���͂Əo�͂������I�u�W�F�N�g���w�肵���ꍇ
//! 
template < class Array1, class Array2, class Array3, class DepthMap, class ATTRIBUTETYPE >
bool volumerendering( const Array1 &in, const Array2 &mk, Array3 &out, const DepthMap &dmap, const volumerender::parameter &param, const volumerender::attribute_table< ATTRIBUTETYPE > &table, const volumerender::attribute_table< ATTRIBUTETYPE > &mktable, bool apply_and_operation, typename Array1::size_type thread_num = 0 )
{
	if( is_same_object( in, out ) || in.empty( ) )
	{
		return( false );
	}
	else if( in.width( ) != mk.width( ) || in.height( ) != mk.height( ) || in.depth( ) != mk.depth( ) )
	{
	}

	typedef typename Array1::size_type size_type;
	typedef __volumerendering_controller__::volumerendering_with_mark_thread< Array1, Array2, Array3, DepthMap, ATTRIBUTETYPE > volumerendering_thread;

	if( thread_num == 0 )
	{
		thread_num = static_cast< size_type >( get_cpu_num( ) );
	}

	volumerendering_thread *thread = new volumerendering_thread[ thread_num ];

	size_type i;
	for( i = 0 ; i < thread_num ; i++ )
	{
		thread[ i ].setup_parameters( in, mk, out, apply_and_operation ? 1 : 2, dmap, param, table, mktable, i, thread_num );
	}

	// �X���b�h�����s���āC�I���܂őҋ@����
	do_threads_( thread, thread_num );

	delete [] thread;
	
	return( true );
}


/// @brief �{�����[�������_�����O
//! 
//! @attention ���͂Əo�͂́C�ʂ�MIST�R���e�i�I�u�W�F�N�g�łȂ��Ă͂Ȃ�Ȃ�
//! @attention �X���b�h����0���w�肵���ꍇ�́C�g�p�\��CPU���������I�Ɏ擾����
//!
//! @param[in]  in         �c ���͉摜
//! @param[in]  mk         �c �}�[�N�摜
//! @param[out] out        �c �o�͉摜
//! @param[in]  param      �c �{�����[�������_�����O�̃p�����[�^
//! @param[in]  table      �c �{�����[�������_�����O�̐F�|�l�e�[�u��
//! @param[in]  mktable    �c �}�[�N�f�[�^�p�̐F�|�l�e�[�u��
//! @param[in]  thread_num �c �g�p����X���b�h��
//! 
//! @retval true  �c �{�����[�������_�����O�ɐ���
//! @retval false �c ���͂Əo�͂������I�u�W�F�N�g���w�肵���ꍇ
//! 
template < class Array1, class Array2, class Array3, class ATTRIBUTETYPE >
bool volumerendering( const Array1 &in, const Array2 &mk, Array3 &out, const volumerender::parameter &param, const volumerender::attribute_table< ATTRIBUTETYPE > &table, const volumerender::attribute_table< ATTRIBUTETYPE > &mktable, typename Array1::size_type thread_num = 0 )
{
	return( volumerendering( in, mk, out, volumerender::no_depth_map( ), param, table, mktable, thread_num ) );
}


/// @brief �{�����[�������_�����O
//! 
//! @attention ���͂Əo�͂́C�ʂ�MIST�R���e�i�I�u�W�F�N�g�łȂ��Ă͂Ȃ�Ȃ�
//! @attention �X���b�h����0���w�肵���ꍇ�́C�g�p�\��CPU���������I�Ɏ擾����
//!
//! @param[in]  in                  �c ���͉摜
//! @param[in]  mk                  �c �}�[�N�摜
//! @param[out] out                 �c �o�͉摜
//! @param[in]  param               �c �{�����[�������_�����O�̃p�����[�^
//! @param[in]  table               �c �{�����[�������_�����O�̐F�|�l�e�[�u��
//! @param[in]  mktable             �c �}�[�N�f�[�^�p�̐F�|�l�e�[�u��
//! @param[in]  apply_and_operation �c CT�ƃ}�[�N�̋��ʕ����݂̂������_�����O���邩�C�a�W���������_�����O���邩
//! @param[in]  thread_num          �c �g�p����X���b�h��
//! 
//! @retval true  �c �{�����[�������_�����O�ɐ���
//! @retval false �c ���͂Əo�͂������I�u�W�F�N�g���w�肵���ꍇ
//! 
template < class Array1, class Array2, class Array3, class ATTRIBUTETYPE >
bool volumerendering( const Array1 &in, const Array2 &mk, Array3 &out, const volumerender::parameter &param, const volumerender::attribute_table< ATTRIBUTETYPE > &table, const volumerender::attribute_table< ATTRIBUTETYPE > &mktable, bool apply_and_operation, typename Array1::size_type thread_num = 0 )
{
	return( volumerendering( in, mk, out, volumerender::no_depth_map( ), param, table, mktable, apply_and_operation, thread_num ) );
}



/// @brief �{�����[�������_�����O
//! 
//! @attention ���͂Əo�͂́C�ʂ�MIST�R���e�i�I�u�W�F�N�g�łȂ��Ă͂Ȃ�Ȃ�
//! @attention �X���b�h����0���w�肵���ꍇ�́C�g�p�\��CPU���������I�Ɏ擾����
//!
//! @param[in]  in         �c ���͉摜
//! @param[out] dmap       �c �����_�����O�����������邽�߂̋����摜�̌v�Z����
//! @param[in]  table      �c �{�����[�������_�����O�̐F�|�l�e�[�u��
//! @param[in]  thread_num �c �g�p����X���b�h��
//! 
//! @retval true  �c �{�����[�������_�����O�ɐ���
//! @retval false �c ���͂Əo�͂������I�u�W�F�N�g���w�肵���ꍇ
//! 
template < class Array, class DepthMap, class ATTRIBUTETYPE >
bool generate_depth_map( const Array &in, DepthMap &dmap, const volumerender::attribute_table< ATTRIBUTETYPE > &table, typename Array::size_type thread_num = 0 )
{
	if( is_same_object( in, dmap ) || in.empty( ) )
	{
		return( false );
	}

	typedef typename Array::size_type size_type;

	dmap.resize( in.width( ) / 2, in.height( ) / 2, in.depth( ) / 2 );
	dmap.reso( 1.0, 1.0, 1.0 );

	for( size_type k = 0 ; k < dmap.depth( ) ; k++ )
	{
		size_type _3 = k * 2;
		for( size_type j = 0 ; j < dmap.height( ) ; j++ )
		{
			size_type _2 = j * 2;
			for( size_type i = 0 ; i < dmap.width( ) ; i++ )
			{
				size_type _1 = i * 2;
				bool b1 = table.has_alpha( in( _1    , _2    , _3     ) );
				bool b2 = table.has_alpha( in( _1 + 1, _2    , _3     ) );
				bool b3 = table.has_alpha( in( _1    , _2 + 1, _3     ) );
				bool b4 = table.has_alpha( in( _1 + 1, _2 + 1, _3     ) );
				bool b5 = table.has_alpha( in( _1    , _2    , _3 + 1 ) );
				bool b6 = table.has_alpha( in( _1 + 1, _2    , _3 + 1 ) );
				bool b7 = table.has_alpha( in( _1    , _2 + 1, _3 + 1 ) );
				bool b8 = table.has_alpha( in( _1 + 1, _2 + 1, _3 + 1 ) );

				if( b1 || b2 || b3 || b4 || b5 || b6 || b7 || b8 )
				{
					dmap( i, j, k ) = 0;
				}
				else
				{
					dmap( i, j, k ) = 1;
				}
			}
		}
	}

	euclidean::distance_transform( dmap, dmap, thread_num );

	for( size_type i = 0 ; i < dmap.size( ) ; i++ )
	{
		dmap[ i ] = std::sqrt( dmap[ i ] );
	}
	
	return( true );
}


/// @}
// �{�����[�������_�����O�O���[�v�̏I���

/// @}
//  �����O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_VOLUMERENDER__
