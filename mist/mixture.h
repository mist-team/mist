/// @file mist/mixture.h
//!
//! @brief EM�A���S���Y����p�����������z�̐���
//!
//! - �Q�l����
//!   - �E�E�E
//!
#ifndef __INCLUDE_MIXTURE__
#define __INCLUDE_MIXTURE__


#ifndef __INCLUDE_MIST_H__
#include "mist.h"
#endif

#ifndef __INCLUDE_MIST_VECTOR__
#include "vector.h"
#endif

#include <cmath>
#include <vector>


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


#define EMALGORITHM_DEBUG	0


//! @defgroup statistics_group ���v����
//!
//!  @{


//! @addtogroup mixture_group EM�A���S���Y����p�����������z�̐���
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/mixture.h>
//! @endcode
//!
//! @{


/// @brief �������z����ɗp����p�����[�^
namespace mixture
{
	/// @brief �������z�𐄒肷�邽�߂̕��z�p�����[�^
	struct distribution
	{
		double	weight; 	///< �����d��
		double	av; 		///< ����
		double	sd; 		///< �W���΍�

		distribution( ) : weight( 1.0 ), av( 0.0 ), sd( 1.0 ){ }
	};

	inline std::ostream &operator <<( std::ostream &out, const distribution &a )
	{
		out << "( " << a.weight << ", " << a.av << ", " << a.sd << " )";
		return( out );
	}

	/// @brief �������z�𐄒肷�邽�߂̕��z�p�����[�^
	struct distribution2
	{
		double				weight;		///< �����d��
		vector2< double >	av;			///< ����
		double				v[ 4 ];		///< �����U�s��

		distribution2( ) : weight( 1.0 )
		{
			v[ 0 ] = v[ 3 ] = 1.0;
			v[ 1 ] = v[ 2 ] = 0.0;
		}
	};

	inline std::ostream &operator <<( std::ostream &out, const distribution2 &a )
	{
		out << "( " << a.weight << ", " << a.av << ", < " << a.v[ 0 ] << ", " << a.v[ 1 ] << ", " << a.v[ 2 ] << ", " << a.v[ 3 ] << " > )";
		return( out );
	}
}

/// @brief �f�[�^�n�񂩂琳�K���z�̍������z�𐄒肷��
//! 
//! @param[in]     rSamples      �c ���̓T���v��
//! @param[in]     pdp           �c ���z�p�����[�^
//! @param[in]     nComponents   �c ���肷�鍬�����z�̐�
//! @param[in]     nMaxIteration �c �ő僋�[�v��
//! @param[in,out] fEpsilon      �c �ΐ��ޓx�̑ł��؂萸�x
//! @param[out]    nIteration    �c ���ۂ̃��[�v��
//!
//! @retval true  �c �������z�̐���ɐ���
//! @retval false �c �������z�̐���Ɏ��s�C�������͓��̓f�[�^����
//! 
template < class T, class Allocator >
bool estimate_mixture(
							const array< T, Allocator > &rSamples,
							mixture::distribution *pdp,
							typename array< T, Allocator >::size_type nComponents,
							typename array< T, Allocator >::size_type nMaxIteration,
							double	fEpsilon, typename array< T, Allocator >::size_type &nIteration
						)
{
	if( rSamples.empty( ) || nComponents == 0 )
	{
		return( false );
	}

	typedef typename array< T, Allocator >::size_type size_type;

	size_type nSamples = rSamples.size( );
	size_type k, m, n;
	//const double	pi = atan( 1.0f ) * 4.0f;
	const double pi = 3.1415926535897932384626433832795;
	const double _2pi = std::sqrt( 2.0 * pi );
	double fLikelihood;
	double fLastLikelihood = -1.0e30;
	double tmp;

#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	// VC6�ł�STL�̃A���P�[�^�̒�`���A�W���ɏ������Ă��Ȃ��̂ŁA�f�t�H���g�ő�p����
	array2< double > Weight( nSamples, nComponents );
#else
	array2< double, typename Allocator::template rebind< double >::other > Weight( nSamples, nComponents );
#endif



#if defined( EMALGORITHM_DEBUG ) && EMALGORITHM_DEBUG == 1
	printf( "%d, %f, %f, %f, %f, %f, %f\n", 0, pdp[ 0 ].av, pdp[ 0 ].sd, pdp[ 0 ].weight, pdp[ 1 ].av, pdp[ 1 ].sd, pdp[ 1 ].weight );
#endif


	// �������z�f�[�^�̏d�݂̘a���P�ɐ��K������
	tmp = 0.0;
	for( m = 0 ; m < nComponents ; m++ )
	{
		tmp += pdp[ m ].weight;
	}

	if( tmp <= 0.0 )
	{
		return( false );
	}

	for( m = 0 ; m < nComponents ; m++ )
	{
		pdp[ m ].weight /= tmp;
	}


	// EM�A���S���Y���̊J�n
	for( n = 0 ; n < nMaxIteration ; n++ )
	{
		// E-step

		for( k = 0 ; k < nSamples ; k++ )
		{
			tmp = 0.0;

			for( m = 0 ; m < nComponents ; m++ )
			{
				double myu = rSamples[ k ] - pdp[ m ].av;
				double v = pdp[ m ].weight * ( 1.0 / pdp[ m ].sd ) * std::exp( - myu * myu / ( 2.0 * pdp[ m ].sd * pdp[ m ].sd ) );
				Weight( k, m ) = v;
				tmp += v;
			}

			if( tmp == 0.0 )
			{
				// �d�݂̍��v���P�ɂȂ�Ȃ��G���[
				return( false );
			}
			else
			{
				for( size_type m = 0 ; m < nComponents ; m++ )
				{
					Weight( k, m ) /= tmp;
				}
			}
		}

		// M-step
		for( m = 0 ; m < nComponents ; m++ )
		{
			double	weight_sum = 0;
			double	average = 0;
			double	variance = 0;

			for( k = 0 ; k < nSamples ; k++ )
			{
				weight_sum += Weight( k, m );
				average += static_cast< double >( rSamples[ k ] ) * Weight( k, m );
			}

			if( weight_sum > 0.0 )
			{
				pdp[ m ].weight = weight_sum / static_cast< double >( nSamples );
				pdp[ m ].av = average / weight_sum;

				for( k = 0 ; k < nSamples ; k++ )
				{
					// ���U�̌v�Z���@���Ԉ���Ă�C������
					variance += Weight( k, m ) * ( rSamples[ k ] - pdp[ m ].av ) * ( rSamples[ k ] - pdp[ m ].av );
				}

				variance /= weight_sum;
			}
			else
			{
				// �d�݂̍��v���P�ɂȂ�Ȃ��G���[
				return( false );
			}


			pdp[ m ].sd = std::sqrt( variance );
		}


		double weight_sum = 0;
		for( m = 0 ; m < nComponents ; m++ )
		{
			weight_sum += pdp[ m ].weight;
		}

		if( std::abs( weight_sum - 1.0 ) > 0.1 )
		{
			// �d�݂̍��v���P�ɂȂ�Ȃ��G���[
			return( false );
		}

		fLikelihood = 0.0;

		for( k = 0 ; k < nSamples ; k++ )
		{
			tmp = 0.0;

			for( m = 0 ; m < nComponents ; m++ )
			{
				double myu = rSamples[ k ] - pdp[ m ].av;
				tmp += Weight( k, m ) * pdp[ m ].weight * ( 1.0 / pdp[ m ].sd ) * std::exp( - myu * myu / ( 2.0 * pdp[ m ].sd * pdp[ m ].sd ) );
			}

			if( tmp == 0.0 )
			{
				return( false );
			}

			fLikelihood += std::log( tmp / _2pi );
		}

#if defined( EMALGORITHM_DEBUG ) && EMALGORITHM_DEBUG == 1
		printf( "%d (%f %f %f) (%f %f %f) %f\n", n, pdp[ 0 ].av, pdp[ 0 ].sd, pdp[ 0 ].weight, pdp[ 1 ].av, pdp[ 1 ].sd, pdp[ 1 ].weight, fLikelihood );
#endif

		if( fLastLikelihood > fLikelihood || std::abs( fLastLikelihood - fLikelihood ) < fEpsilon )
		{
			break;
		}

		fLastLikelihood = fLikelihood;
	}

	return( true );
}




/// @brief �f�[�^�n�񂩂琳�K���z�̍������z�𐄒肷��
//! 
//! @param[in]     rSamples      �c ���̓T���v��
//! @param[in]     pdp           �c ���z�p�����[�^
//! @param[in]     nComponents   �c ���肷�鍬�����z�̐�
//! @param[in]     nMaxIteration �c �ő僋�[�v��
//! @param[in,out] fEpsilon      �c �ΐ��ޓx�̑ł��؂萸�x
//! @param[out]    nIteration    �c ���ۂ̃��[�v��
//!
//! @retval true  �c �������z�̐���ɐ���
//! @retval false �c �������z�̐���Ɏ��s�C�������͓��̓f�[�^����
//! 
template < class T, class Allocator >
bool estimate_mixture(
							const array< vector2< T >, Allocator > &rSamples,
							mixture::distribution2 *pdp,
							typename array< T, Allocator >::size_type nComponents,
							typename array< T, Allocator >::size_type nMaxIteration,
							double	fEpsilon, typename array< T, Allocator >::size_type &nIteration
						)
{
	if( rSamples.empty( ) || nComponents == 0 )
	{
		return( false );
	}

	typedef typename array< T, Allocator >::size_type size_type;

	size_type nSamples = rSamples.size( );
	size_type k, m, n;
	//const double	pi = atan( 1.0f ) * 4.0f;
	const double pi = 3.1415926535897932384626433832795;
	const double _2pi = 2.0 * pi;
	double fLikelihood;
	double fLastLikelihood = -1.0e30;
	double tmp;

#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	// VC6�ł�STL�̃A���P�[�^�̒�`���A�W���ɏ������Ă��Ȃ��̂ŁA�f�t�H���g�ő�p����
	array2< double > Weight( nSamples, nComponents );
#else
	array2< double, typename Allocator::template rebind< double >::other > Weight( nSamples, nComponents );
#endif



#if defined( EMALGORITHM_DEBUG ) && EMALGORITHM_DEBUG == 1
	printf( "%d, %f, %f, %f, %f, %f, %f\n", 0, pdp[ 0 ].av, pdp[ 0 ].sd, pdp[ 0 ].weight, pdp[ 1 ].av, pdp[ 1 ].sd, pdp[ 1 ].weight );
#endif


	// �������z�f�[�^�̏d�݂̘a���P�ɐ��K������
	tmp = 0.0;
	for( m = 0 ; m < nComponents ; m++ )
	{
		tmp += pdp[ m ].weight;
	}

	if( tmp <= 0.0 )
	{
		return( false );
	}

	for( m = 0 ; m < nComponents ; m++ )
	{
		pdp[ m ].weight /= tmp;
	}


	// EM�A���S���Y���̊J�n
	for( n = 0 ; n < nMaxIteration ; n++ )
	{
		// E-step

		for( k = 0 ; k < nSamples ; k++ )
		{
			tmp = 0.0;

			for( m = 0 ; m < nComponents ; m++ )
			{
				double t = pdp[ m ].v[ 0 ] * pdp[ m ].v[ 3 ] - pdp[ m ].v[ 1 ] * pdp[ m ].v[ 2 ];
				double a = pdp[ m ].v[ 3 ];
				double b = -pdp[ m ].v[ 1 ];
				double c = -pdp[ m ].v[ 2 ];
				double d = pdp[ m ].v[ 0 ];
				double x = rSamples[ k ].x - pdp[ m ].av.x;
				double y = rSamples[ k ].y - pdp[ m ].av.y;
				double vvv = ( ( a * x + b * y ) * x + ( c * x + d * y ) * y ) / t;
				double v = pdp[ m ].weight * ( 1.0 / sqrt( t ) ) * std::exp( - vvv / 2.0 );
				Weight( k, m ) = v;
				tmp += v;
			}
			if( tmp == 0.0 )
			{
				// �d�݂̍��v���P�ɂȂ�Ȃ��G���[
				return( false );
			}
			else
			{
				for( size_type m = 0 ; m < nComponents ; m++ )
				{
					Weight( k, m ) /= tmp;
				}
			}
		}

		// M-step
		for( m = 0 ; m < nComponents ; m++ )
		{
			double	weight_sum = 0;
			vector2< double > average( 0, 0 );
			double	v1 = 0;
			double	v2 = 0;
			double	v3 = 0;

			for( k = 0 ; k < nSamples ; k++ )
			{
				weight_sum += Weight( k, m );
				average += rSamples[ k ] * Weight( k, m );
			}

			if( weight_sum > 0.0 )
			{
				pdp[ m ].weight = weight_sum / static_cast< double >( nSamples );
				pdp[ m ].av = average / weight_sum;

				for( k = 0 ; k < nSamples ; k++ )
				{
					double w  = Weight( k, m );
					double xx = rSamples[ k ].x - pdp[ m ].av.x;
					double yy = rSamples[ k ].y - pdp[ m ].av.y;
					v1 += w * xx * xx;
					v2 += w * yy * yy;
					v3 += w * xx * yy;
				}

				v1 /= weight_sum;
				v2 /= weight_sum;
				v3 /= weight_sum;

				if( v1 * v2 < v3 * v3 )
				{
					v3 = std::sqrt( v1 * v2 ) - 1.0e-10;
				}
			}
			else
			{
				// �d�݂̍��v���P�ɂȂ�Ȃ��G���[
				return( false );
			}


			pdp[ m ].v[ 0 ] = v1;
			pdp[ m ].v[ 3 ] = v2;
			pdp[ m ].v[ 1 ] = pdp[ m ].v[ 2 ] = v3;
		}


		double weight_sum = 0;
		for( m = 0 ; m < nComponents ; m++ )
		{
			weight_sum += pdp[ m ].weight;
		}

		if( std::abs( weight_sum - 1.0 ) > 0.1 )
		{
			// �d�݂̍��v���P�ɂȂ�Ȃ��G���[
			return( false );
		}

		fLikelihood = 0.0;

		for( k = 0 ; k < nSamples ; k++ )
		{
			tmp = 0.0;

			for( m = 0 ; m < nComponents ; m++ )
			{
				double t = pdp[ m ].v[ 0 ] * pdp[ m ].v[ 3 ] - pdp[ m ].v[ 1 ] * pdp[ m ].v[ 2 ];
				double a = pdp[ m ].v[ 3 ];
				double b = -pdp[ m ].v[ 1 ];
				double c = -pdp[ m ].v[ 2 ];
				double d = pdp[ m ].v[ 0 ];
				double x = rSamples[ k ].x - pdp[ m ].av.x;
				double y = rSamples[ k ].y - pdp[ m ].av.y;
				double vvv = ( ( a * x + b * y ) * x + ( c * x + d * y ) * y ) / t;
				tmp += Weight( k, m ) * pdp[ m ].weight / sqrt( t ) * std::exp( - vvv / 2.0 );
			}

			if( tmp == 0.0 )
			{
				return( false );
			}

			fLikelihood += std::log( tmp / _2pi );
		}

#if defined( EMALGORITHM_DEBUG ) && EMALGORITHM_DEBUG == 1
		printf( "%d (%f %f %f) (%f %f %f) %f\n", n, pdp[ 0 ].av, pdp[ 0 ].sd, pdp[ 0 ].weight, pdp[ 1 ].av, pdp[ 1 ].sd, pdp[ 1 ].weight, fLikelihood );
#endif

		if( fLastLikelihood > fLikelihood || std::abs( fLastLikelihood - fLikelihood ) < fEpsilon )
		{
			break;
		}

		fLastLikelihood = fLikelihood;

		//static int count = 0;
		//std::cout << count++ << "                                           \r";
	}

	return( true );
}


/// @brief �f�[�^�n�񂩂琳�K���z�̍������z�𐄒肷��
//! 
//! @param[in]     rSamples      �c ���̓T���v��
//! @param[in]     pdp           �c ���z�p�����[�^
//! @param[in]     nMaxIteration �c �ő僋�[�v��
//! @param[in,out] fEpsilon      �c �ΐ��ޓx�̑ł��؂萸�x
//! @param[out]    nIteration    �c ���ۂ̃��[�v��
//!
//! @retval true  �c �������z�̐���ɐ���
//! @retval false �c �������z�̐���Ɏ��s�C�������͓��̓f�[�^����
//! 
template < class T, class Allocator >
bool estimate_mixture(
							const mist::array< T, Allocator >& rSamples, std::vector< mixture::distribution > &pdp,
							typename array< T, Allocator >::size_type nMaxIteration,
							double	fEpsilon, typename array< T, Allocator >::size_type &nIteration
						)
{
	return( estimate_mixture( rSamples, &pdp[ 0 ], pdp.size( ), nMaxIteration, fEpsilon, nIteration ) );
}


/// @brief �f�[�^�n�񂩂琳�K���z�̍������z�𐄒肷��
//! 
//! @param[in]     rSamples      �c ���̓T���v��
//! @param[in]     pdp           �c ���z�p�����[�^
//! @param[in]     nMaxIteration �c �ő僋�[�v��
//! @param[in,out] fEpsilon      �c �ΐ��ޓx�̑ł��؂萸�x
//!
//! @retval true  �c �������z�̐���ɐ���
//! @retval false �c �������z�̐���Ɏ��s�C�������͓��̓f�[�^����
//! 
template < class T, class Allocator >
bool estimate_mixture(
							const mist::array< T, Allocator >& rSamples,
							std::vector< mixture::distribution > &pdp,
							typename array< T, Allocator >::size_type nMaxIteration,
							double	fEpsilon
						)
{
	typename array< T, Allocator >::size_type nIteration = 0;
	return( estimate_mixture( rSamples, &pdp[ 0 ], pdp.size( ), nMaxIteration, fEpsilon, nIteration ) );
}


/// @brief �f�[�^�n�񂩂琳�K���z�̍������z�𐄒肷��
//! 
//! @param[in]     rSamples      �c ���̓T���v��
//! @param[in]     pdp           �c ���z�p�����[�^
//! @param[in]     nComponents   �c ���肷�鍬�����z�̐�
//! @param[in]     nMaxIteration �c �ő僋�[�v��
//! @param[in,out] fEpsilon      �c �ΐ��ޓx�̑ł��؂萸�x
//!
//! @retval true  �c �������z�̐���ɐ���
//! @retval false �c �������z�̐���Ɏ��s�C�������͓��̓f�[�^����
//! 
template < class T, class Allocator >
bool estimate_mixture(
							const mist::array< T, Allocator > &rSamples,
							mixture::distribution *pdp,
							typename array< T, Allocator >::size_type nComponents,
							typename array< T, Allocator >::size_type nMaxIteration,
							double	fEpsilon
						)
{
	typename array< T, Allocator >::size_type nIteration = 0;
	return( estimate_mixture( rSamples, pdp, nComponents, nMaxIteration, fEpsilon, nIteration ) );
}





/// @brief �f�[�^�n�񂩂琳�K���z�̍������z�𐄒肷��
//! 
//! @param[in]     rSamples      �c ���̓T���v��
//! @param[in]     pdp           �c ���z�p�����[�^
//! @param[in]     nMaxIteration �c �ő僋�[�v��
//! @param[in,out] fEpsilon      �c �ΐ��ޓx�̑ł��؂萸�x
//! @param[out]    nIteration    �c ���ۂ̃��[�v��
//!
//! @retval true  �c �������z�̐���ɐ���
//! @retval false �c �������z�̐���Ɏ��s�C�������͓��̓f�[�^����
//! 
template < class T, class Allocator >
bool estimate_mixture(
							const mist::array< T, Allocator >& rSamples, std::vector< mixture::distribution2 > &pdp,
							typename array< T, Allocator >::size_type nMaxIteration,
							double	fEpsilon, typename array< T, Allocator >::size_type &nIteration
						)
{
	return( estimate_mixture( rSamples, &pdp[ 0 ], pdp.size( ), nMaxIteration, fEpsilon, nIteration ) );
}


/// @brief �f�[�^�n�񂩂琳�K���z�̍������z�𐄒肷��
//! 
//! @param[in]     rSamples      �c ���̓T���v��
//! @param[in]     pdp           �c ���z�p�����[�^
//! @param[in]     nMaxIteration �c �ő僋�[�v��
//! @param[in,out] fEpsilon      �c �ΐ��ޓx�̑ł��؂萸�x
//!
//! @retval true  �c �������z�̐���ɐ���
//! @retval false �c �������z�̐���Ɏ��s�C�������͓��̓f�[�^����
//! 
template < class T, class Allocator >
bool estimate_mixture(
							const mist::array< T, Allocator >& rSamples,
							std::vector< mixture::distribution2 > &pdp,
							typename array< T, Allocator >::size_type nMaxIteration,
							double	fEpsilon
						)
{
	typename array< T, Allocator >::size_type nIteration = 0;
	return( estimate_mixture( rSamples, &pdp[ 0 ], pdp.size( ), nMaxIteration, fEpsilon, nIteration ) );
}


/// @brief �f�[�^�n�񂩂琳�K���z�̍������z�𐄒肷��
//! 
//! @param[in]     rSamples      �c ���̓T���v��
//! @param[in]     pdp           �c ���z�p�����[�^
//! @param[in]     nComponents   �c ���肷�鍬�����z�̐�
//! @param[in]     nMaxIteration �c �ő僋�[�v��
//! @param[in,out] fEpsilon      �c �ΐ��ޓx�̑ł��؂萸�x
//!
//! @retval true  �c �������z�̐���ɐ���
//! @retval false �c �������z�̐���Ɏ��s�C�������͓��̓f�[�^����
//! 
template < class T, class Allocator >
bool estimate_mixture(
							const mist::array< T, Allocator > &rSamples,
							mixture::distribution2 *pdp,
							typename array< T, Allocator >::size_type nComponents,
							typename array< T, Allocator >::size_type nMaxIteration,
							double	fEpsilon
						)
{
	typename array< T, Allocator >::size_type nIteration = 0;
	return( estimate_mixture( rSamples, pdp, nComponents, nMaxIteration, fEpsilon, nIteration ) );
}



/// @}
//  EM�A���S���Y����p�����������z�̐���O���[�v�̏I���

/// @}
//  ���v�����O���[�v�̏I���



// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIXTURE__
