/// @file mist/registration.h
//!
//! @brief ���́E�񍄑̃��W�X�g���[�V����
//!
#ifndef __INCLUDE_MIST_REGISTRATION__
#define __INCLUDE_MIST_REGISTRATION__


#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif


#ifndef __INCLUDE_MIST_H__
#include "mist.h"
#endif

#ifndef __INCLUDE_MIST_MINIMIZATION__
#include "minimization.h"
#endif

#ifndef __INCLUDE_MIST_DRAWING__
#include "drawing.h"
#endif


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN

namespace __non_rigid_registration_utility__
{
	inline void FFD( double v, double &f0, double &f1, double &f2, double &f3 )
	{
		double v_ = 1.0 - v;
		double v2 = v * v;
		double v3 = v2 * v;
		f0 = v_ * v_ * v_ / 6.0;
		f1 = 0.5 * v3 - v2 + 4.0 / 6.0;
		f2 = ( v2 - v3 + v ) * 0.5 + 1.0 / 6.0;
		f3 = v3 / 6.0;
	}


	// ����_����p���āC�\�[�X�摜���^�[�Q�b�g�摜�ɕό`����
	template < class TARGETTYPE, class TARGETTYPEA, class SOURCETYPE, class SOURCETYPEA, class CONTROLMESH >
	static void non_rigid_transformation_( array2< TARGETTYPE, TARGETTYPEA > &target, const array2< SOURCETYPE, SOURCETYPEA > &source, const CONTROLMESH &control_mesh,
											typename CONTROLMESH::difference_type mx = -1,
											typename CONTROLMESH::difference_type my = -1,
											typename CONTROLMESH::difference_type mz = -1,
											typename CONTROLMESH::size_type thread_id = 0,
											typename CONTROLMESH::size_type thread_num = 1
										)
	{
		typedef array2< TARGETTYPE, TARGETTYPEA >  target_image_type;
		typedef array2< SOURCETYPE, SOURCETYPEA >  source_image_type;
		typedef CONTROLMESH control_mesh_type;

		typedef typename target_image_type::size_type size_type;
		typedef typename target_image_type::difference_type difference_type;

		difference_type i, j;
		double u, v;
		double sx[ 4 ], sy[ 4 ];
		double stepW = control_mesh.width( ) == 1 ? 1.0 : target.width( ) / static_cast< double >( control_mesh.width( ) - 1 );
		double stepH = control_mesh.height( ) == 1 ? 1.0 : target.height( ) / static_cast< double >( control_mesh.height( ) - 1 );
		double _1_stepW = 1.0 / stepW;
		double _1_stepH = 1.0 / stepH;
		double _1_ax = 1.0 / source.reso1( );
		double _1_ay = 1.0 / source.reso2( );

		//stepW *= target.reso1( );
		//stepH *= target.reso2( );

		difference_type d0, d1, d2, d3;
		{
			difference_type cx = source.width( ) / 2;
			difference_type cy = source.height( ) / 2;
			typename source_image_type::const_pointer ppp = &source( cx, cy );
			d0 = 0;
			d1 = &source( cx    , cy + 1 ) - ppp;
			d2 = &source( cx + 1, cy + 1 ) - ppp;
			d3 = &source( cx + 1, cy     ) - ppp;
		}

		difference_type isx, isy, iex, iey;
		difference_type mw = control_mesh.width( );
		difference_type mh = control_mesh.height( );
		if( mx < 0 || mw <= mx || my < 0 || mh <= my )
		{
			isx = 0;
			iex = target.width( ) - 1;
			isy = 0;
			iey = target.height( ) - 1;
		}
		else
		{
			isx = static_cast< difference_type >( ( mx - 2 ) * stepW );
			iex = static_cast< difference_type >( ( mx + 2 ) * stepW );
			isy = static_cast< difference_type >( ( my - 2 ) * stepH );
			iey = static_cast< difference_type >( ( my + 2 ) * stepH );

			difference_type tw = target.width( );
			difference_type th = target.height( );
			isx = isx > 0  ? isx : 0;
			isy = isy > 0  ? isy : 0;
			iex = iex < tw ? iex : tw - 1;
			iey = iey < th ? iey : th - 1;
		}


		for( difference_type y = isy + thread_id ; y <= iey ; y += thread_num )
		{
			v = y * _1_stepH;
			j = static_cast< difference_type >( v );
			v -= j;
			j--;

			FFD( v, sy[ 0 ], sy[ 1 ], sy[ 2 ], sy[ 3 ] );

			for( difference_type x = isx ; x <= iex ; x++ )
			{
				u = x * _1_stepW;
				i = static_cast< difference_type >( u );
				u -= i;
				i--;

				FFD( u, sx[ 0 ], sx[ 1 ], sx[ 2 ], sx[ 3 ] );

				double xx = 0.0, yy = 0.0;
				for( difference_type m = 0 ; m <= 3 ; m++ )
				{
					typename control_mesh_type::const_pointer p = &control_mesh( i, j + m, 0 );
					for( difference_type l = 0 ; l <= 3 ; l++ )
					{
						double val = sx[ l ] * sy[ m ];
						xx += val * p[ l ].x;
						yy += val * p[ l ].y;
					}
				}

				xx *= _1_ax;
				yy *= _1_ay;

				double ct = -2000.0;
				if( xx < 0 || source.width( ) <= xx + 1 || yy < 0 || source.height( ) <= yy + 1 )
				{
				}
				else
				{
					difference_type ixx = static_cast< size_type >( xx );
					difference_type iyy = static_cast< size_type >( yy );
					typename source_image_type::const_pointer p = &source( ixx, iyy, 0 );
					xx -= ixx;
					yy -= iyy;

					ct = ( p[ d0 ] + ( p[ d3 ] - p[ d0 ] ) * xx ) + ( p[ d1 ] - p[ d0 ] + ( p[ d0 ] - p[ d1 ] + p[ d2 ] - p[ d3 ] ) * xx ) * yy;
				}
				target( x, y ) = static_cast< typename target_image_type::value_type >( ct );
			}
		}
	}


	// ����_����p���āC�\�[�X�摜���^�[�Q�b�g�摜�ɕό`����
	template < class TARGETTYPE, class TARGETTYPEA, class SOURCETYPE, class SOURCETYPEA, class CONTROLMESH >
	static void non_rigid_transformation_( array3< TARGETTYPE, TARGETTYPEA > &target, const array3< SOURCETYPE, SOURCETYPEA > &source, const CONTROLMESH &control_mesh,
											typename CONTROLMESH::difference_type mx = -1,
											typename CONTROLMESH::difference_type my = -1,
											typename CONTROLMESH::difference_type mz = -1,
											typename CONTROLMESH::size_type thread_id = 0,
											typename CONTROLMESH::size_type thread_num = 1
										)
	{
		typedef array3< TARGETTYPE, TARGETTYPEA >  target_image_type;
		typedef array3< SOURCETYPE, SOURCETYPEA >  source_image_type;
		typedef CONTROLMESH control_mesh_type;

		typedef typename target_image_type::size_type size_type;
		typedef typename target_image_type::difference_type difference_type;

		difference_type i, j, k;
		double u, v, w;
		double sx[ 4 ], sy[ 4 ], sz[ 4 ];
		double stepW = control_mesh.width( )  == 1 ? 1.0 : target.width( ) / static_cast< double >( control_mesh.width( ) - 1 );
		double stepH = control_mesh.height( ) == 1 ? 1.0 : target.height( ) / static_cast< double >( control_mesh.height( ) - 1 );
		double stepD = control_mesh.depth( )  == 1 ? 1.0 : target.depth( ) / static_cast< double >( control_mesh.depth( ) - 1 );
		double _1_stepW = 1.0 / stepW;
		double _1_stepH = 1.0 / stepH;
		double _1_stepD = 1.0 / stepH;
		double _1_ax = 1.0 / source.reso1( );
		double _1_ay = 1.0 / source.reso2( );
		double _1_az = 1.0 / source.reso2( );

		//stepW *= target.reso1( );
		//stepH *= target.reso2( );
		//stepD *= target.reso3( );

		difference_type d0, d1, d2, d3, d4, d5, d6, d7;
		{
			difference_type cx = source.width( ) / 2;
			difference_type cy = source.height( ) / 2;
			difference_type cz = source.depth( ) / 2;
			typename source_image_type::const_pointer ppp = &source( cx, cy, cz );
			d0 = 0;
			d1 = &source( cx    , cy + 1, cz     ) - ppp;
			d2 = &source( cx + 1, cy + 1, cz     ) - ppp;
			d3 = &source( cx + 1, cy    , cz     ) - ppp;
			d4 = &source( cx    , cy    , cz + 1 ) - ppp;
			d5 = &source( cx    , cy + 1, cz + 1 ) - ppp;
			d6 = &source( cx + 1, cy + 1, cz + 1 ) - ppp;
			d7 = &source( cx + 1, cy    , cz + 1 ) - ppp;
		}

		difference_type isx, isy, isz, iex, iey, iez;
		difference_type mw = control_mesh.width( );
		difference_type mh = control_mesh.height( );
		difference_type md = control_mesh.depth( );
		if( mx < 0 || mw <= mx || my < 0 || mh <= my || mz < 0 || md <= mz )
		{
			isx = 0;
			iex = target.width( ) - 1;
			isy = 0;
			iey = target.height( ) - 1;
			isz = 0;
			iez = target.depth( ) - 1;
		}
		else
		{
			isx = static_cast< difference_type >( ( mx - 2 ) * stepW );
			iex = static_cast< difference_type >( ( mx + 2 ) * stepW );
			isy = static_cast< difference_type >( ( my - 2 ) * stepH );
			iey = static_cast< difference_type >( ( my + 2 ) * stepH );
			isz = static_cast< difference_type >( ( mz - 2 ) * stepD );
			iez = static_cast< difference_type >( ( mz + 2 ) * stepD );

			difference_type tw = target.width( );
			difference_type th = target.height( );
			difference_type td = target.depth( );
			isx = isx > 0  ? isx : 0;
			isy = isy > 0  ? isy : 0;
			isz = isz > 0  ? isz : 0;
			iex = iex < tw ? iex : tw - 1;
			iey = iey < th ? iey : th - 1;
			iez = iez < td ? iez : td - 1;
		}


		for( difference_type z = isz + thread_id ; z <= iez ; z += thread_num )
		{
			w = z * _1_stepD;
			k = static_cast< difference_type >( w );
			w -= k;
			k--;

			FFD( w, sz[ 0 ], sz[ 1 ], sz[ 2 ], sz[ 3 ] );

			for( difference_type y = isy ; y <= iey ; y++ )
			{
				v = y * _1_stepH;
				j = static_cast< difference_type >( v );
				v -= j;
				j--;

				FFD( v, sy[ 0 ], sy[ 1 ], sy[ 2 ], sy[ 3 ] );

				for( difference_type x = isx ; x <= iex ; x++ )
				{
					u = x * _1_stepW;
					i = static_cast< difference_type >( u );
					u -= i;
					i--;

					FFD( u, sx[ 0 ], sx[ 1 ], sx[ 2 ], sx[ 3 ] );

					double xx = 0.0, yy = 0.0, zz = 0.0;
					for( difference_type n = 0 ; n <= 3 ; n++ )
					{
						for( difference_type m = 0 ; m <= 3 ; m++ )
						{
							typename control_mesh_type::const_pointer p = &control_mesh( i, j + m, k + n );
							double vvv = sy[ m ] * sz[ n ];
							for( difference_type l = 0 ; l <= 3 ; l++ )
							{
								double vv = sx[ l ] * vvv;
								xx += vv * p[ l ].x;
								yy += vv * p[ l ].y;
								zz += vv * p[ l ].z;
							}
						}
					}

					xx *= _1_ax;
					yy *= _1_ay;
					zz *= _1_az;

					double ct = -2000.0;
					if( xx < 0 || source.width( ) <= xx + 1 || yy < 0 || source.height( ) <= yy + 1 || zz < 0 || source.depth( ) <= zz + 1 )
					{
					}
					else
					{
						difference_type ixx = static_cast< size_type >( xx );
						difference_type iyy = static_cast< size_type >( yy );
						difference_type izz = static_cast< size_type >( zz );
						typename source_image_type::const_pointer p = &source( ixx, iyy, izz );
						xx -= ixx;
						yy -= iyy;
						zz -= izz;

						ct = ( p[ d0 ] + ( p[ d3 ] - p[ d0 ] ) * xx ) + ( p[ d1 ] - p[ d0 ] + ( p[ d0 ] - p[ d1 ] + p[ d2 ] - p[ d3 ] ) * xx ) * yy;
						ct += ( ( p[ d4 ] + ( p[ d7 ] - p[ d4 ] ) * xx ) + ( p[ d5 ] - p[ d4 ] + ( p[ d4 ] - p[ d5 ] + p[ d6 ] - p[ d7 ] ) * xx ) * yy - ct ) * zz;
					}
					target( x, y, z ) = static_cast< typename target_image_type::value_type >( ct );
				}
			}
		}
	}



	template < class TARGETTYPE, class SOURCETYPE, class CONTROLMESH >
	class non_rigid_registration_thread : public thread< non_rigid_registration_thread< TARGETTYPE, SOURCETYPE, CONTROLMESH > >
	{
	public:
		typedef TARGETTYPE  target_image_type;
		typedef SOURCETYPE  source_image_type;
		typedef CONTROLMESH control_mesh_type;
		typedef thread< non_rigid_registration_thread< TARGETTYPE, SOURCETYPE, CONTROLMESH > > base;
		typedef typename base::thread_exit_type thread_exit_type;
		typedef typename TARGETTYPE::size_type size_type;

	private:
		size_t thread_id_;
		size_t thread_num_;

		// ���o�͗p�̉摜�ւ̃|�C���^
		target_image_type *target_;
		const source_image_type *source_;
		const control_mesh_type *control_mesh_;
		size_type mx_;
		size_type my_;
		size_type mz_;

	private:
		const non_rigid_registration_thread& operator =( const non_rigid_registration_thread &p );

	public:
		void setup_parameters( target_image_type &target, const source_image_type &source, const control_mesh_type &control_mesh,
													size_t mx, size_type my, size_type mz, size_type thread_id, size_type thread_num )
		{
			target_  = &target;
			source_ = &source;
			control_mesh_ = &control_mesh;
			mx_ = mx;
			my_ = my;
			mz_ = mz;
			thread_id_ = thread_id;
			thread_num_ = thread_num;
		}

		non_rigid_registration_thread( size_type id = 0, size_type num = 1 ) : thread_id_( id ), thread_num_( num ),
			target_( NULL ), source_( NULL ), control_mesh_( NULL ), mx_( -1 ), my_( -1 ), mz_( -1 )
		{
		}

		non_rigid_registration_thread( const non_rigid_registration_thread &p ) : base( p ), thread_id_( p.thread_id_ ), thread_num_( p.thread_num_ ),
			target_( p.target_ ), source_( p.source_ ), control_mesh_( p.control_mesh_ ), mx_( p.mx_ ), my_( p.my_ ), mz_( p.mz_ )
		{
		}

	protected:
		// �p��������ŕK�����������X���b�h�֐�
		virtual thread_exit_type thread_function( )
		{
			non_rigid_transformation_( *target_, *source_, *control_mesh_, mx_, my_, mz_, thread_id_, thread_num_ );
			return( true );
		}
	};

	// ����_����p���āC�\�[�X�摜���^�[�Q�b�g�摜�ɕό`����
	template < class TARGETTYPE, class SOURCETYPE, class CONTROLMESH >
	static bool transformation( TARGETTYPE &target, const SOURCETYPE &source, const CONTROLMESH &control_mesh,
									typename CONTROLMESH::difference_type mx = -1,
									typename CONTROLMESH::difference_type my = -1,
									typename CONTROLMESH::difference_type mz = -1,
									typename CONTROLMESH::size_type thread_num = 0
								)
	{
		if( is_same_object( target, source ) || target.empty( ) || source.empty( ) )
		{
			return( false );
		}

		typedef TARGETTYPE target_image_type;
		typedef SOURCETYPE source_image_type;
		typedef CONTROLMESH control_mesh_type;
		typedef typename target_image_type::size_type  size_type;
		typedef non_rigid_registration_thread< target_image_type, source_image_type, control_mesh_type > non_rigid_registration_thread;

		if( thread_num == 0 )
		{
			thread_num = static_cast< size_type >( get_cpu_num( ) );
		}

		non_rigid_registration_thread *thread = new non_rigid_registration_thread[ thread_num ];

		size_type i;
		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( target, source, control_mesh, mx, my, mz, i, thread_num );
		}

		// �X���b�h�����s���āC�I���܂őҋ@����
		do_threads_( thread, thread_num );

		delete [] thread;
		
		return( true );
	}


	template < class TARGETTYPE, class SOURCETYPE, class CONTROLMESH >
	struct registration_functor
	{
		typedef TARGETTYPE  target_image_type;
		typedef SOURCETYPE  source_image_type;
		typedef CONTROLMESH control_mesh_type;
		typedef typename TARGETTYPE::size_type size_type;
		typedef typename TARGETTYPE::difference_type difference_type;

		array< unsigned int * > target;
		target_image_type &transformed_image;
		const source_image_type &source;
		const control_mesh_type &control_mesh;
		control_mesh_type control_mesh_tmp;
		size_type x, y, z;

		array2< unsigned int > h;
		array< unsigned int > hh;

		difference_type minimum;
		difference_type maximum;
		size_type BIN;

		unsigned int __no_data_is_associated__;
		double H1;

		template < class IMAGETYPE >
		static difference_type get_minimum( const IMAGETYPE &image )
		{
			typedef typename IMAGETYPE::value_type value_type;
			value_type min = image[ 0 ];
			for( size_type i = 1 ; i < image.size( ) ; i++ )
			{
				if( min > image[ i ] )
				{
					min = image[ i ];
				}
			}
			return( static_cast< difference_type >( min ) );
		}

		template < class T >
		static const T get_minimum( const T &v1, const T &v2 )
		{
			return( v1 < v2 ? v1 : v2 );
		}

		template < class IMAGETYPE >
		static difference_type get_maximum( const IMAGETYPE &image )
		{
			typedef typename IMAGETYPE::value_type value_type;
			value_type max = image[ 0 ];
			for( size_type i = 1 ; i < image.size( ) ; i++ )
			{
				if( max < image[ i ] )
				{
					max = image[ i ];
				}
			}
			return( static_cast< difference_type >( max ) );
		}

		template < class T >
		static const T get_maximum( const T &v1, const T &v2 )
		{
			return( v1 > v2 ? v1 : v2 );
		}

		registration_functor( const target_image_type &tgt, target_image_type &tmp, const source_image_type &src, const control_mesh_type &cmesh, size_type bin )
			: target( tgt.size( ) ), transformed_image( tmp ), source( src ), control_mesh ( cmesh ), control_mesh_tmp( cmesh ), x( 0 ), y( 0 ), z( 0 ), BIN( bin )
		{
			// �����摜���쐬����
			transformation( transformed_image, source, control_mesh );

			minimum = get_maximum( get_minimum( get_minimum( tgt ), get_minimum( src ) ), -1000 );
			maximum = get_maximum( get_maximum( tgt ), get_maximum( src ) );
			h.resize( ( maximum - minimum ) / BIN + 1, ( maximum - minimum ) / BIN + 1 );
			hh.resize( ( maximum - minimum ) / BIN + 1, 1 );

			size_type count = 0;
			difference_type upper = h.width( );
			double _1_bin = 1.0 / BIN;
			for( size_t i = 0 ; i < tgt.size( ) ; i++ )
			{
				difference_type v1 = static_cast< difference_type >( ( tgt[ i ] - minimum ) * _1_bin );
				if( v1 < 0 || upper <= v1 )
				{
					target[ i ] = &__no_data_is_associated__;
					continue;
				}
				target[ i ] = &h( 0, v1 );
				hh[ v1 ]++;
				count++;
			}

			H1 = 0.0;

			// �q�X�g�O�����̐��K��
			if( count == 0.0 )
			{
				count = 1;
			}

			for( size_type i = 0 ; i < hh.size( ) ; i++ )
			{
				if( hh[ i ] > 0 )
				{
					double v = hh[ i ] / static_cast< double >( count );
					H1 -= v * std::log( v );
				}
			}
		}

		void initialize( size_t i, size_t j, size_t k )
		{
			x = i;
			y = j;
			z = k;
			control_mesh_tmp = control_mesh;
		}

		template < class PARAMETER >
		double operator ()( const PARAMETER &p )
		{
			control_mesh_tmp( x, y, z ).x = control_mesh( x, y, z ).x + p[ 0 ];
			control_mesh_tmp( x, y, z ).y = control_mesh( x, y, z ).y + p[ 1 ];
			control_mesh_tmp( x, y, z ).z = control_mesh( x, y, z ).z + p[ 2 ];

			__non_rigid_registration_utility__::transformation( transformed_image, source, control_mesh_tmp, x, y, z );

			// �f�[�^������������
			__no_data_is_associated__ = 0;
			h.fill( );
			hh.fill( );
			difference_type upper = h.width( );
			size_type count = 0;
			double _1_bin = 1.0 / BIN;

			for( size_t i = 0 ; i < target.size( ) ; i++ )
			{
				difference_type v2 = static_cast< difference_type >( ( transformed_image[ i ] - minimum ) * _1_bin );
				if( v2 < 0 )
				{
					continue;
				}

				target[ i ][ v2 ]++;
				hh[ v2 ]++;
				count++;
			}

			// �q�X�g�O�����̐��K��
			if( count == 0 )
			{
				return ( 5.0 );
			}

			double H2 = 0.0, H12 = 0.0, _1_count = 1.0 / static_cast< double >( count );

			for( size_type i = 0 ; i < hh.size( ) ; i++ )
			{
				if( hh[ i ] > 0 )
				{
					double v = hh[ i ] * _1_count;
					H2 -= v * std::log( v );

					array2< unsigned int >::const_pointer ph = &h( 0, i );
					for( size_type j = 0 ; j < hh.size( ) ; j++ )
					{
						if( ph[ j ] > 0 )
						{
							double v = ph[ j ] * _1_count;
							H12 -= v * std::log( v );
						}
					}
				}
			}

			double nmi = ( H1 + H2 ) / H12;
			//double nmi = H1 + H2 - H12;
			//{
			//	static int count = 0;
			//	printf( "%d                        \r", count++ );
			//}

			return ( - nmi );
		}
	};
}



//! @addtogroup registration_group �摜�̃��W�X�g���[�V����
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/registration.h>
//! @endcode
//!
//!  @{



/// @brief FFD��p�����񍄑̃��W�X�g���[�V����
namespace non_rigid
{
	/// @brief �񍄑̃��W�X�g���[�V�����G���W��
	//! 
	//! @param TARGETTYPE  �c ���W�X�g���[�V��������ڕW�摜�̌^
	//! 
	//! @code
	//! mist::non_rigid::registration< �ڕW�摜�̌^����� > r( �ڕW�摜, X�������̐���_��, Y�������̐���_��, Z�������̐���_��, NMI���v�Z����ۂ̃r���� );
	//! 
	//! r.apply( �\�[�X�摜, ���W�X�g���[�V�����̏I������ɗp���鋖�e���Ό덷 );
	//! 
	//! r.transform( �\�[�X�摜, �񍄑̃��W�X�g���[�V�����̌��ʂ��i�[����摜 );
	//! @endcode
	//! 
	template < class TARGETTYPE >
	class registration
	{
	public:
		typedef TARGETTYPE image_type;									///< @brief ���W�X�g���[�V��������ڕW�摜�̌^
		typedef vector3< double > vector_type;							///< @brief �����ŗ��p����3�����x�N�g���̌^
		typedef marray< array3< vector_type > > control_mesh_type;		///< @brief ����_�̔z��
		typedef matrix< double > matrix_type;							///< @brief �����ŗ��p����s��̌^
		typedef typename image_type::size_type size_type;				///< @brief �����ŗ��p���镄���Ȃ������^
		typedef typename image_type::difference_type difference_type;	///< @brief �����ŗ��p���镄�����萮���^

	protected:
		const image_type &target;			///< @brief ���W�X�g���[�V��������ڕW�摜
		image_type transformed_source;		///< @brief ���W�X�g���[�V�������ɗ��p���C����_����p���ă\�[�X��ڕW�摜�֕ϊ������ꎞ�摜
		control_mesh_type control_mesh;		///< @brief ����_�z��
		size_type BIN;

	public:
		/// @brief �񍄑̃��W�X�g���[�V�����G���W���̏�����
		//! 
		//! @param[in] tgt  �c �ڕW�摜
		//! @param[in] divW �c ����_�̏�����������X�������̌�
		//! @param[in] divH �c ����_�̏�����������Y�������̌�
		//! @param[in] divD �c ����_�̏�����������Z�������̌�
		//! @param[in] bin  �c NMI���v�Z����ۂ̃r����
		//! 
		registration( const image_type &tgt, size_type divW, size_type divH, size_type divD, size_type bin )
			: target( tgt ), transformed_source( tgt ), control_mesh( divW + 1, divH + 1, divD + 1, 2 ), BIN( bin )
		{
		}


		/// @brief �񍄑̃��W�X�g���[�V�����̎��s
		//! 
		//! @param[in] source    �c �ڕW�摜�Ɍ����ĕό`����\�[�X�摜
		//! @param[in] tolerance �c ���W�X�g���[�V�����̏I������ɗp���鋖�e���Ό덷
		//! @param[in] max_loop  �c �œK�������̍ő唽����
		//! 
		template < class SOURCETYPE >
		void apply( const SOURCETYPE &source, double tolerance, size_type max_loop = 3 )
		{
			typedef __non_rigid_registration_utility__::registration_functor< TARGETTYPE, SOURCETYPE, control_mesh_type > non_rigid_registration_functor_type;
			typedef __minimization_utility__::__no_copy_constructor_functor__< non_rigid_registration_functor_type > no_constructor_functor_type;

			size_type divW = control_mesh.width( ) - 1;
			size_type divH = control_mesh.height( ) - 1;
			size_type divD = control_mesh.depth( ) - 1;

			// �܂��C����_�̏�����
			double stepW = divW == 0 ? 0 : source.width( ) / static_cast< double >( divW );
			double stepH = divH == 0 ? 0 : source.height( ) / static_cast< double >( divH );
			double stepD = divD == 0 ? 0 : source.depth( ) / static_cast< double >( divD );
			difference_type w = control_mesh.width( );
			difference_type h = control_mesh.height( );
			difference_type d = control_mesh.depth( );
			for( difference_type k = -2 ; k <= d + 1 ; k++ )
			{
				for( difference_type j = -2 ; j <= h + 1 ; j++ )
				{
					for( difference_type i = -2 ; i <= w + 1 ; i++ )
					{
						vector_type &v = control_mesh( i, j, k );
						v.x = i * stepW * source.reso1( );
						v.y = j * stepH * source.reso2( );
						v.z = k * stepD * source.reso3( );
					}
				}
			}

			difference_type isx = 0;
			difference_type iex = w - 1;
			difference_type isy = 0;
			difference_type iey = h - 1;
			difference_type isz = 0;
			difference_type iez = d - 1;

			if( iex < isx )
			{
				iex = isx = 0;
			}
			if( iey < isy )
			{
				iey = isy = 0;
			}
			if( iez < isz )
			{
				iez = isz = 0;
			}

			// �m�����W�b�h���W�X�g���[�V�������s���t�@���N�^
			non_rigid_registration_functor_type f( target, transformed_source, source, control_mesh, BIN );

			double err = f( matrix_type::zero( 3, 1 ) );
			double old_err = err;
			size_type loop = 0;
			while( loop++ < max_loop )
			{
				// �e����_�����Ԃɕό`�����Ȃ���œK�����s��
				for( difference_type k = isz ; k <= iez ; k++ )
				{
					for( difference_type j = isy ; j <= iey ; j++ )
					{
						for( difference_type i = isx ; i <= iex ; i++ )
						{
							// �ړ������鐧��_���w��
							f.initialize( i, j, k );

							// �ŏ������J�n
							matrix_type p( 3, 1 ), dirs = matrix_type::identity( 3, 3 ), bound( 3, 2 );
							bound( 0, 0 ) = -( control_mesh( i - 1, j    , k     ) - control_mesh( i, j, k ) ).length( ) * 0.5;
							bound( 0, 1 ) =  ( control_mesh( i + 1, j    , k     ) - control_mesh( i, j, k ) ).length( ) * 0.5;
							bound( 1, 0 ) = -( control_mesh( i    , j - 1, k     ) - control_mesh( i, j, k ) ).length( ) * 0.5;
							bound( 1, 1 ) =  ( control_mesh( i    , j + 1, k     ) - control_mesh( i, j, k ) ).length( ) * 0.5;
							bound( 2, 0 ) = -( control_mesh( i    , j    , k - 1 ) - control_mesh( i, j, k ) ).length( ) * 0.5;
							bound( 2, 1 ) =  ( control_mesh( i    , j    , k + 1 ) - control_mesh( i, j, k ) ).length( ) * 0.5;

	//						err = gradient::minimization( p, no_constructor_functor_type( f ), 0.1 );
							err = gradient::minimization( p, bound, no_constructor_functor_type( f ), 1.0 );
//							err = gradient::minimization( p, bound, no_constructor_functor_type( f ), tolerance );
	//						err = powell::minimization( p, dirs, bound, no_constructor_functor_type( f ), 0.1 );

							// ���ʂ𔽉f
							vector_type &v = control_mesh( i, j, k );
							v.x += p[ 0 ];
							v.y += p[ 1 ];
							v.z += p[ 2 ];

							std::cout << "( " << i << ", " << j << ", " << k << " ) = " << "( " << v << " )" << std::endl;
							__non_rigid_registration_utility__::transformation( transformed_source, source, control_mesh );
						}
					}
				}

				if( 2.0 * std::abs( old_err - err ) < tolerance * ( std::abs( old_err ) + std::abs( err ) ) )
				{
					break;
				}
			}
		}


		/// @brief �񍄑̃��W�X�g���[�V�������ʂ��摜�Ɋi�[����
		//!
		//! �o�͉摜�̉𑜓x�ɍ��킹�āC�񍄑̃��W�X�g���[�V�������ʂ̉摜�𐶐�����
		//!
		//! @attention �o�͉摜�̃T�C�Y�Ɖ𑜓x�͎��O�ɐݒ肵�Ă����K�v����
		//! 
		//! @param[in]  source �c �ڕW�摜�Ɍ����ĕό`����\�[�X�摜
		//! @param[out] out    �c �o�͌��ʂ̉摜
		//! 
		template < class SOURCETYPE, class SOURCETYPEA, class OUTTYPE, class OUTTYPEA >
		void transform( const array2< SOURCETYPE, SOURCETYPEA > &source, array2< OUTTYPE, OUTTYPEA > &out )
		{
			__non_rigid_registration_utility__::transformation( out, source, control_mesh );
		}


		/// @brief �񍄑̃��W�X�g���[�V�������ʂ��摜�Ɋi�[����
		//!
		//! �o�͉摜�̉𑜓x�ɍ��킹�āC�񍄑̃��W�X�g���[�V�������ʂ̉摜�𐶐�����
		//!
		//! @attention �o�͉摜�̃T�C�Y�Ɖ𑜓x�͎��O�ɐݒ肵�Ă����K�v����
		//! 
		//! @param[in]  source �c �ڕW�摜�Ɍ����ĕό`����\�[�X�摜
		//! @param[out] out    �c �o�͌��ʂ̉摜
		//! 
		template < class SOURCETYPE, class SOURCETYPEA, class OUTTYPE, class OUTTYPEA >
		void transform( const array3< SOURCETYPE, SOURCETYPEA > &source, array3< OUTTYPE, OUTTYPEA > &out )
		{
			__non_rigid_registration_utility__::transformation( out, source, control_mesh );
		}


		/// @brief �񍄑̃��W�X�g���[�V�������ʂ��摜�Ɋi�[����
		//!
		//! �o�͉摜�̉𑜓x�ɍ��킹�āC�񍄑̃��W�X�g���[�V�������ʂ̉摜�𐶐����C����_�̃��b�V����`�悷��
		//!
		//! @attention �o�͉摜�̃T�C�Y�Ɖ𑜓x�͎��O�ɐݒ肵�Ă����K�v����
		//! 
		//! @param[in]  source �c �ڕW�摜�Ɍ����ĕό`����\�[�X�摜
		//! @param[out] out    �c �o�͌��ʂ̉摜
		//! @param[in]  value  �c ����_���b�V���̒l
		//! 
		template < class SOURCETYPE, class SOURCETYPEA, class OUTTYPE, class OUTTYPEA >
		void transform( const array2< SOURCETYPE, SOURCETYPEA > &source, array2< OUTTYPE, OUTTYPEA > &out, typename array2< OUTTYPE, OUTTYPEA >::value_type value )
		{
			__non_rigid_registration_utility__::transformation( out, source, control_mesh );

			difference_type w = control_mesh.width( );
			difference_type h = control_mesh.height( );
			difference_type d = control_mesh.depth( );
			difference_type i, j;
			for( j = 0 ; j < h - 1 ; j++ )
			{
				for( i = 0 ; i < w - 1 ; i++ )
				{
					vector_type &vec0 = control_mesh( i    , j    , 0 );
					vector_type &vec1 = control_mesh( i + 1, j    , 0 );
					vector_type &vec2 = control_mesh( i    , j + 1, 0 );

					size_type x0 = static_cast< size_type >( vec0.x );
					size_type y0 = static_cast< size_type >( vec0.y );
					size_type x1 = static_cast< size_type >( vec1.x );
					size_type y1 = static_cast< size_type >( vec1.y );
					size_type x2 = static_cast< size_type >( vec2.x );
					size_type y2 = static_cast< size_type >( vec2.y );
					draw_line( out, x0, y0, x1, y1, value );
					draw_line( out, x0, y0, x2, y2, value );
				}

				vector_type &vec0 = control_mesh( i    , j    , 0 );
				vector_type &vec2 = control_mesh( i    , j + 1, 0 );
				size_type x0 = static_cast< size_type >( vec0.x );
				size_type y0 = static_cast< size_type >( vec0.y );
				size_type x2 = static_cast< size_type >( vec2.x );
				size_type y2 = static_cast< size_type >( vec2.y );
				draw_line( out, x0, y0, x2, y2, value );
			}
			for( i = 0 ; i < w - 1 ; i++ )
			{
				vector_type &vec0 = control_mesh( i    , j    , 0 );
				vector_type &vec1 = control_mesh( i + 1, j    , 0 );

				size_type x0 = static_cast< size_type >( vec0.x );
				size_type y0 = static_cast< size_type >( vec0.y );
				size_type x1 = static_cast< size_type >( vec1.x );
				size_type y1 = static_cast< size_type >( vec1.y );
				draw_line( out, x0, y0, x1, y1, value );
			}
		}


		/// @brief �񍄑̃��W�X�g���[�V�������ʂ��摜�Ɋi�[����
		//!
		//! �o�͉摜�̉𑜓x�ɍ��킹�āC�񍄑̃��W�X�g���[�V�������ʂ̉摜�𐶐����C����_�̃��b�V����`�悷��
		//!
		//! @attention �o�͉摜�̃T�C�Y�Ɖ𑜓x�͎��O�ɐݒ肵�Ă����K�v����
		//! 
		//! @param[in]  source �c �ڕW�摜�Ɍ����ĕό`����\�[�X�摜
		//! @param[out] out    �c �o�͌��ʂ̉摜
		//! @param[in]  value  �c ����_���b�V���̒l
		//! 
		template < class SOURCETYPE, class SOURCETYPEA, class OUTTYPE, class OUTTYPEA >
		void transform( const array3< SOURCETYPE, SOURCETYPEA > &source, array3< OUTTYPE, OUTTYPEA > &out, typename array2< OUTTYPE, OUTTYPEA >::value_type value )
		{
			__non_rigid_registration_utility__::transformation( out, source, control_mesh );

			difference_type w = control_mesh.width( );
			difference_type h = control_mesh.height( );
			difference_type d = control_mesh.depth( );
			for( difference_type k = 0 ; k < d ; k++ )
			{
				for( difference_type j = 0 ; j < h ; j++ )
				{
					for( difference_type i = 0 ; i < w ; i++ )
					{
						const vector_type &vec0 = control_mesh( i, j, k );

						size_type x0 = static_cast< size_type >( vec0.x );
						size_type y0 = static_cast< size_type >( vec0.y );
						size_type z0 = static_cast< size_type >( vec0.z );

						if( i < w - 1 )
						{
							const vector_type &vec1 = control_mesh( i + 1, j, k );
							size_type x1 = static_cast< size_type >( vec1.x );
							size_type y1 = static_cast< size_type >( vec1.y );
							size_type z1 = static_cast< size_type >( vec1.z );
							draw_line( out, x0, y0, z0, x1, y1, z1, value );
						}

						if( j < h - 1 )
						{
							const vector_type &vec2 = control_mesh( i, j + 1, k );
							size_type x2 = static_cast< size_type >( vec2.x );
							size_type y2 = static_cast< size_type >( vec2.y );
							size_type z2 = static_cast< size_type >( vec2.z );
							draw_line( out, x0, y0, z0, x2, y2, z2, value );
						}

						if( k < d - 1 )
						{
							const vector_type &vec3 = control_mesh( i, j, k + 1 );
							size_type x3 = static_cast< size_type >( vec3.x );
							size_type y3 = static_cast< size_type >( vec3.y );
							size_type z3 = static_cast< size_type >( vec3.z );
							draw_line( out, x0, y0, z0, x3, y3, z3, value );
						}
					}
				}
			}
		}
	};

} // ���O��� non_rigid �̏I���


/// @}
//  �摜�̃��W�X�g���[�V�����O���[�v�̏I���



// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_REGISTRATION__
