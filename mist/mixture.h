// 
// Copyright (c) 2003-2010, MIST Project, Nagoya University
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
		typedef vector2< double > vector_type;

		double			weight;		///< �����d��
		vector_type		av;			///< ����
		double			v[ 4 ];		///< �����U�s��

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

	// dp�ŗ^�����鐳�K���z��(x,y)�ɂ�����l��Ԃ��B
	inline double gauss( const mist::mixture::distribution2 &dp, double x, double y )
	{
		double t = dp.v[ 0 ] * dp.v[ 3 ] - dp.v[ 1 ] * dp.v[ 2 ];
		double a = dp.v[ 3 ] / t;
		double b = -dp.v[ 1 ] / t;
		double c = -dp.v[ 2 ] / t;
		double d = dp.v[ 0 ] / t;
		x -= dp.av.x;
		y -= dp.av.y;
		const double pi = 3.1415926535897932384626433832795;
		const double _2pi = 2.0 * pi;
		double vvv = ( a * x + b * y ) * x + ( c * x + d * y ) * y;
		return ( 1.0 / ( _2pi * sqrt( t ) ) * std::exp( - vvv / 2.0 ) );
	}

	inline double gauss( const mist::mixture::distribution &dp, double x )
	{
		const double pi = 3.1415926535897932384626433832795;
		const double _2pi = std::sqrt( 2.0 * pi );
		double myu = x - dp.av;
		return ( 1.0 / ( _2pi * dp.sd ) * std::exp( - myu * myu / ( 2.0 * dp.sd * dp.sd ) ) );
	}
}


/// @brief �f�[�^�n�񂩂琳�K���z�̍������z�𐄒肷��
//! 
//! @attention ���͂ƂȂ�f�[�^�̔z��Ƃ��āC�ʏ�̃f�[�^�z��CMIST�Œ񋟂���R���e�i�CSTL�Œ񋟂���Ă���vector�Cdeque�R���e�i�����p�\�ł��D
//! 
//! @param[in]     rSamples      �c ���̓T���v��
//! @param[in,out] opdp          �c ���z�p�����[�^
//! @param[in]     nSamples      �c ���̓T���v����
//! @param[in]     nComponents   �c ���肷�鍬�����z�̐�
//! @param[in]     nMaxIteration �c �ő僋�[�v��
//! @param[in]     tolerance     �c �ΐ��ޓx�̑ł��؂苖�e���Ό덷
//! @param[out]    nIteration    �c ���ۂ̃��[�v��
//!
//! @retval true  �c �������z�̐���ɐ���
//! @retval false �c �������z�̐���Ɏ��s�C�������͓��̓f�[�^����
//! 
template < class Array >
bool estimate_mixture( const Array &rSamples, mixture::distribution *opdp, size_t nSamples, size_t nComponents, size_t nMaxIteration, double tolerance, size_t &nIteration )
{
	if( rSamples.empty( ) || nComponents == 0 )
	{
		return( false );
	}

	typedef size_t size_type;

	size_type k, m;
	//const double	pi = atan( 1.0f ) * 4.0f;
	const double pi = 3.1415926535897932384626433832795;
	const double _2pi = std::sqrt( 2.0 * pi );
	const double _log_2pi = std::log( _2pi );
	double fLastLikelihood = -1.0e30;

	array2< double > Weight( nSamples, nComponents );
	std::vector< mixture::distribution > pdp( nComponents );


#if defined( EMALGORITHM_DEBUG ) && EMALGORITHM_DEBUG == 1
	for( m = 0 ; m < nComponents ; m++ )
	{
		std::cerr << pdp[ m ] << std::endl;
	}
#endif

	// ���̓f�[�^����ƃf�[�^�ɃR�s�[����
	for( m = 0 ; m < nComponents ; m++ )
	{
		pdp[ m ] = opdp[ m ];
	}

	// �������z�f�[�^�̏d�݂̘a���P�ɐ��K������
	{
		double tmp = 0.0;
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
	}

	// EM�A���S���Y���̊J�n
	for( nIteration = 0 ; nIteration < nMaxIteration ; nIteration++ )
	{
		// E-step
		for( k = 0 ; k < nSamples ; k++ )
		{
			double tmp = 0.0;

			for( m = 0 ; m < nComponents ; m++ )
			{
				double myu = rSamples[ k ] - pdp[ m ].av;
				double v = pdp[ m ].weight * std::exp( - myu * myu / ( 2.0 * pdp[ m ].sd * pdp[ m ].sd ) ) / pdp[ m ].sd;
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
					double myu = rSamples[ k ] - pdp[ m ].av;
					variance += Weight( k, m ) * myu * myu;
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

		double fLikelihood = 0.0;

		for( k = 0 ; k < nSamples ; k++ )
		{
			double tmp = 0.0;

			for( m = 0 ; m < nComponents ; m++ )
			{
				double myu = rSamples[ k ] - pdp[ m ].av;
				tmp += pdp[ m ].weight * std::exp( - myu * myu / ( 2.0 * pdp[ m ].sd * pdp[ m ].sd ) ) / pdp[ m ].sd;
				//tmp += Weight( k, m ) * pdp[ m ].weight * std::exp( - myu * myu / ( 2.0 * pdp[ m ].sd * pdp[ m ].sd ) ) / pdp[ m ].sd;
			}

			if( tmp == 0.0 )
			{
				return( false );
			}

			fLikelihood += std::log( tmp ) - _log_2pi;
		}

#if defined( EMALGORITHM_DEBUG ) && EMALGORITHM_DEBUG == 1
		for( m = 0 ; m < nComponents ; m++ )
		{
			std::cerr << pdp[ m ] << std::endl;
		}
#elif defined( EMALGORITHM_DEBUG ) && EMALGORITHM_DEBUG == 2
		printf( "%f = ( %f, %f, %f )\n", fLikelihood, pdp[ 0 ].weight, pdp[ 0 ].av, pdp[ 0 ].sd );
#endif

		if( fLastLikelihood >= fLikelihood || 2.0 * std::abs( fLastLikelihood - fLikelihood ) < tolerance * ( std::abs( fLastLikelihood ) + std::abs( fLikelihood ) ) )
		{
			break;
		}

		// �o�͂ɍ�ƃf�[�^�𔽉f������
		for( m = 0 ; m < nComponents ; m++ )
		{
			opdp[ m ] = pdp[ m ];
		}

		fLastLikelihood = fLikelihood;
	}

	return( true );
}


/// @brief �f�[�^�n�񂩂琳�K���z�̍������z�𐄒肷��
//! 
//! @attention ���͂ƂȂ�f�[�^�̔z��Ƃ��āC�ʏ�̃f�[�^�z��CMIST�Œ񋟂���R���e�i�CSTL�Œ񋟂���Ă���vector�Cdeque�R���e�i�����p�\�ł��D
//! @attention ���̓f�[�^�́CMIST�Œ񋟂��� vector2 �^�C�v�ł���K�v������܂��D
//!
//! @param[in]     rSamples      �c ���̓T���v��
//! @param[in,out] opdp          �c ���z�p�����[�^
//! @param[in]     nSamples      �c ���̓T���v����
//! @param[in]     nComponents   �c ���肷�鍬�����z�̐�
//! @param[in]     nMaxIteration �c �ő僋�[�v��
//! @param[in,out] tolerance     �c �ΐ��ޓx�̑ł��؂苖�e���Ό덷
//! @param[out]    nIteration    �c ���ۂ̃��[�v��
//!
//! @retval true  �c �������z�̐���ɐ���
//! @retval false �c �������z�̐���Ɏ��s�C�������͓��̓f�[�^����
//! 
template < class Array >
bool estimate_mixture( const Array &rSamples, mixture::distribution2 *opdp, size_t nSamples, size_t nComponents, size_t nMaxIteration, double tolerance, size_t &nIteration )
{
	if( rSamples.empty( ) || nComponents == 0 )
	{
		return( false );
	}

	typedef size_t size_type;

	size_type k, m;
	//const double	pi = atan( 1.0f ) * 4.0f;
	const double pi = 3.1415926535897932384626433832795;
	const double _2pi = 2.0 * pi;
	const double _log_2pi = std::log( _2pi );
	double fLastLikelihood = -1.0e30;

	array2< double > Weight( nSamples, nComponents );
	std::vector< mixture::distribution2 > pdp( nComponents );


#if defined( EMALGORITHM_DEBUG ) && EMALGORITHM_DEBUG == 1
	for( m = 0 ; m < nComponents ; m++ )
	{
		std::cerr << pdp[ m ] << std::endl;
	}
#endif


	// ���̓f�[�^����ƃf�[�^�ɃR�s�[����
	for( m = 0 ; m < nComponents ; m++ )
	{
		pdp[ m ] = opdp[ m ];
	}


	// �������z�f�[�^�̏d�݂̘a���P�ɐ��K������
	{
		double tmp = 0.0;
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
	}

	// EM�A���S���Y���̊J�n
	for( nIteration = 0 ; nIteration < nMaxIteration ; nIteration++ )
	{
		// E-step

		for( k = 0 ; k < nSamples ; k++ )
		{
			double tmp = 0.0;

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

		double fLikelihood = 0.0;

		for( k = 0 ; k < nSamples ; k++ )
		{
			double tmp = 0.0;

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

			fLikelihood += std::log( tmp ) - _log_2pi;
		}

#if defined( EMALGORITHM_DEBUG ) && EMALGORITHM_DEBUG == 1
		for( m = 0 ; m < nComponents ; m++ )
		{
			std::cerr << pdp[ m ] << std::endl;
		}
#elif defined( EMALGORITHM_DEBUG ) && EMALGORITHM_DEBUG == 2
		printf( "%f = ( %f, %f )\n", fLikelihood, pdp[ 0 ].weight, pdp[ 0 ].av );
#endif

		if( fLastLikelihood >= fLikelihood || 2.0 * std::abs( fLastLikelihood - fLikelihood ) < tolerance * ( std::abs( fLastLikelihood ) + std::abs( fLikelihood ) ) )
		{
			break;
		}

		// �o�͂ɍ�ƃf�[�^�𔽉f������
		for( m = 0 ; m < nComponents ; m++ )
		{
			opdp[ m ] = pdp[ m ];
		}

		fLastLikelihood = fLikelihood;
	}

	return( true );
}


/// @brief �f�[�^�n�񂩂琳�K���z�̍������z�𐄒肷��
//! 
//! @attention ���͂ƂȂ�f�[�^�̔z��Ƃ��āCMIST�Œ񋟂���R���e�i��������STL�Œ񋟂���Ă���vector�Cdeque�R���e�i�����p�\�ł��D
//! @attention ���̓f�[�^�́CMIST�Œ񋟂��� vector2 �^�C�v�ŕ��z�� mixture::distribution2 �̏ꍇ��2�������K���z����ɂȂ�܂��D
//! 
//! @param[in]     rSamples      �c ���̓T���v��
//! @param[in,out] pdp           �c ���z�p�����[�^
//! @param[in]     nMaxIteration �c �ő僋�[�v��
//! @param[in]     tolerance     �c �ΐ��ޓx�̑ł��؂苖�e���Ό덷
//! @param[out]    nIteration    �c ���ۂ̃��[�v��
//!
//! @retval true  �c �������z�̐���ɐ���
//! @retval false �c �������z�̐���Ɏ��s�C�������͓��̓f�[�^����
//! 
template < class Array1, class Array2 >
bool estimate_mixture( const Array1 &rSamples, Array2 &pdp, typename Array1::size_type nMaxIteration, double tolerance, typename Array1::size_type &nIteration )
{
	return( estimate_mixture( rSamples, &pdp[ 0 ], rSamples.size( ), pdp.size( ), nMaxIteration, tolerance, nIteration ) );
}


/// @brief �f�[�^�n�񂩂�1�������K���z�̍������z�𐄒肷��
//! 
//! @attention ���͂ƂȂ�f�[�^�̔z��Ƃ��āCMIST�Œ񋟂���R���e�i��������STL�Œ񋟂���Ă���vector�Cdeque�R���e�i�����p�\�ł��D
//! @attention ���̓f�[�^�́CMIST�Œ񋟂��� vector2 �^�C�v�ŕ��z�� mixture::distribution2 �̏ꍇ��2�������K���z����ɂȂ�܂��D
//! 
//! @param[in]     rSamples      �c ���̓T���v��
//! @param[in,out] pdp           �c ���z�p�����[�^
//! @param[in]     nMaxIteration �c �ő僋�[�v��
//! @param[in]     tolerance     �c �ΐ��ޓx�̑ł��؂苖�e���Ό덷
//!
//! @retval true  �c �������z�̐���ɐ���
//! @retval false �c �������z�̐���Ɏ��s�C�������͓��̓f�[�^����
//! 
template < class Array1, class Array2 >
bool estimate_mixture( const Array1 &rSamples, Array2 &pdp, typename Array1::size_type nMaxIteration, double tolerance )
{
	typename Array1::size_type nIteration = 0;
	return( estimate_mixture( rSamples, &pdp[ 0 ], rSamples.size( ), pdp.size( ), nMaxIteration, tolerance, nIteration ) );
}


/// @brief �f�[�^�n�񂩂琳�K���z�̍������z�𐄒肷��
//! 
//! @attention ���͂ƂȂ�f�[�^�̔z��Ƃ��āCMIST�Œ񋟂���R���e�i��������STL�Œ񋟂���Ă���vector�Cdeque�R���e�i�����p�\�ł��D
//! 
//! @param[in]     rSamples      �c ���̓T���v��
//! @param[in,out] pdp           �c ���z�p�����[�^
//! @param[in]     nComponents   �c ���肷�鍬�����z�̐�
//! @param[in]     nMaxIteration �c �ő僋�[�v��
//! @param[in]     tolerance     �c �ΐ��ޓx�̑ł��؂苖�e���Ό덷
//!
//! @retval true  �c �������z�̐���ɐ���
//! @retval false �c �������z�̐���Ɏ��s�C�������͓��̓f�[�^����
//! 
template < class Array >
bool estimate_mixture( const Array &rSamples, mixture::distribution *pdp, typename Array::size_type nComponents, typename Array::size_type nMaxIteration, double tolerance )
{
	size_t nIteration = 0;
	return( estimate_mixture( rSamples, pdp, rSamples.size( ), nComponents, nMaxIteration, tolerance, nIteration ) );
}


/// @brief �f�[�^�n�񂩂�2�������K���z�̍������z�𐄒肷��
//! 
//! @attention ���͂ƂȂ�f�[�^�̔z��Ƃ��āCMIST�Œ񋟂���R���e�i��������STL�Œ񋟂���Ă���vector�Cdeque�R���e�i�����p�\�ł��D
//! @attention ���̓f�[�^�́CMIST�Œ񋟂��� vector2 �^�C�v����K�v������܂��D
//! 
//! @param[in]     rSamples      �c ���̓T���v��
//! @param[in,out] pdp           �c ���z�p�����[�^
//! @param[in]     nComponents   �c ���肷�鍬�����z�̐�
//! @param[in]     nMaxIteration �c �ő僋�[�v��
//! @param[in]     tolerance     �c �ΐ��ޓx�̑ł��؂苖�e���Ό덷
//!
//! @retval true  �c �������z�̐���ɐ���
//! @retval false �c �������z�̐���Ɏ��s�C�������͓��̓f�[�^����
//! 
template < class Array >
bool estimate_mixture( const Array &rSamples, mixture::distribution2 *pdp, typename Array::size_type nComponents, typename Array::size_type nMaxIteration, double tolerance )
{
	size_t nIteration = 0;
	return( estimate_mixture( rSamples, pdp, rSamples.size( ), nComponents, nMaxIteration, tolerance, nIteration ) );
}



/// @brief �q�X�g�O�������琳�K���z�̍������z�𐄒肷��֐��Q
namespace histogram
{
	/// @brief 1�����q�X�g�O�������琳�K���z�̍������z�𐄒肷��
	//! 
	//! @attention ���͂ƂȂ�f�[�^�̔z��Ƃ��āC�ʏ�̃f�[�^�z��CMIST�Œ񋟂���R���e�i�CSTL�Œ񋟂���Ă���vector�Cdeque�R���e�i�����p�\�ł��D
	//! 
	//! @param[in]     rSamples      �c ���̓T���v��
	//! @param[in,out] opdp          �c ���z�p�����[�^
	//! @param[in]     nSamples      �c ���̓T���v����
	//! @param[in]     nComponents   �c ���肷�鍬�����z�̐�
	//! @param[in]     minimum       �c �q�X�g�O�������쐬�����ۂ̍ŏ��l
	//! @param[in]     bin           �c �q�X�g�O�������쐬�����ۂ̃r����
	//! @param[in]     nMaxIteration �c �ő僋�[�v��
	//! @param[in]     tolerance     �c �ΐ��ޓx�̑ł��؂苖�e���Ό덷
	//! @param[out]    nIteration    �c ���ۂ̃��[�v��
	//!
	//! @retval true  �c �������z�̐���ɐ���
	//! @retval false �c �������z�̐���Ɏ��s�C�������͓��̓f�[�^����
	//! 
	template < class Array >
	bool estimate_mixture( const Array &rSamples, mixture::distribution *opdp, size_t nSamples, size_t nComponents, double minimum, double bin, size_t nMaxIteration, double tolerance, size_t &nIteration )
	{
		if( rSamples.empty( ) || nComponents == 0 || bin == 0.0 )
		{
			return( false );
		}

		typedef size_t size_type;

		size_type k, m;

		const double pi = 3.1415926535897932384626433832795;
		const double _2pi = std::sqrt( 2.0 * pi );
		const double _log_2pi = std::log( _2pi );
		double fLastLikelihood = -1.0e30;
		double tmp, number_of_samples;

		array2< double > Weight( nSamples, nComponents );
		std::vector< mixture::distribution > pdp( nComponents );


		// ���̓f�[�^����ƃf�[�^�ɃR�s�[����
		for( m = 0 ; m < nComponents ; m++ )
		{
			pdp[ m ] = opdp[ m ];
		}

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

		// ���ϒl��0�ɐݒ肵�C�d�݂̘a��1�ɂ���
		for( m = 0 ; m < nComponents ; m++ )
		{
			pdp[ m ].weight /= tmp;
			pdp[ m ].av -= minimum;
		}

		for( k = 0, number_of_samples = 0.0 ; k < nSamples ; k++ )
		{
			number_of_samples += rSamples[ k ];
		}


#if defined( EMALGORITHM_DEBUG ) && EMALGORITHM_DEBUG == 1
		for( m = 0 ; m < nComponents ; m++ )
		{
			std::cerr << pdp[ m ] << std::endl;
		}
#endif


		// EM�A���S���Y���̊J�n
		for( nIteration = 0 ; nIteration < nMaxIteration ; nIteration++ )
		{
			// E-step
			for( k = 0 ; k < nSamples ; k++ )
			{
				if( rSamples[ k ] == 0 )
				{
					continue;
				}

				tmp = 0.0;

				for( m = 0 ; m < nComponents ; m++ )
				{
					double myu = ( k + 0.5 ) * bin - pdp[ m ].av;
					double v = pdp[ m ].weight * std::exp( - myu * myu / ( 2.0 * pdp[ m ].sd * pdp[ m ].sd ) ) / pdp[ m ].sd;
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
					if( rSamples[ k ] == 0 )
					{
						continue;
					}
					double w = Weight( k, m ) * rSamples[ k ];
					weight_sum += w;
					average += ( k + 0.5 ) * bin * w;
				}

				if( weight_sum > 0.0 )
				{
					pdp[ m ].weight = weight_sum / number_of_samples;
					pdp[ m ].av = average / weight_sum;

					for( k = 0 ; k < nSamples ; k++ )
					{
						double myu = ( k + 0.5 ) * bin - pdp[ m ].av;
						variance += Weight( k, m ) * rSamples[ k ] * myu * myu;
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

			double fLikelihood = 0.0;

			for( k = 0 ; k < nSamples ; k++ )
			{
				if( rSamples[ k ] == 0 )
				{
					continue;
				}

				tmp = 0.0;

				for( m = 0 ; m < nComponents ; m++ )
				{
					double myu = ( k + 0.5 ) * bin - pdp[ m ].av;
					tmp += Weight( k, m ) * pdp[ m ].weight * std::exp( - myu * myu / ( 2.0 * pdp[ m ].sd * pdp[ m ].sd ) ) / pdp[ m ].sd;
				}

				if( tmp == 0.0 )
				{
					return( false );
				}

				fLikelihood += rSamples[ k ] * ( std::log( tmp ) - _log_2pi );
			}

#if defined( EMALGORITHM_DEBUG ) && EMALGORITHM_DEBUG == 1
			for( m = 0 ; m < nComponents ; m++ )
			{
				std::cerr << pdp[ m ] << std::endl;
			}
#elif defined( EMALGORITHM_DEBUG ) && EMALGORITHM_DEBUG == 2
		printf( "%f = ( %f, %f, %f )\n", fLikelihood, pdp[ 0 ].weight, pdp[ 0 ].av, pdp[ 0 ].sd );
#endif

			if( fLastLikelihood >= fLikelihood || 2.0 * std::abs( fLastLikelihood - fLikelihood ) < tolerance * ( std::abs( fLastLikelihood ) + std::abs( fLikelihood ) ) )
			{
				break;
			}

			// �o�͂ɍ�ƃf�[�^�𔽉f������
			for( m = 0 ; m < nComponents ; m++ )
			{
				opdp[ m ] = pdp[ m ];
			}

			fLastLikelihood = fLikelihood;
		}


		// ���ϒl�����ɖ߂�
		for( m = 0 ; m < nComponents ; m++ )
		{
			opdp[ m ].av += minimum;
		}

		return( true );
	}



	/// @brief 2�����q�X�g�O�������琳�K���z�̍������z�𐄒肷��
	//! 
	//! @attention ���͂ƂȂ�f�[�^�̔z��Ƃ��āCMIST�Œ񋟂���2�����R���e�i�����p�\�ł��D
	//!
	//! @param[in]     rSamples      �c ���̓T���v��
	//! @param[in,out] opdp          �c ���z�p�����[�^
	//! @param[in]     nComponents   �c ���肷�鍬�����z�̐�
	//! @param[in]     minimum1      �c �q�X�g�O�������쐬�����ۂ̑�1�������ł̍ŏ��l
	//! @param[in]     minimum2      �c �q�X�g�O�������쐬�����ۂ̑�2�������ł̍ŏ��l
	//! @param[in]     bin           �c �q�X�g�O�������쐬�����ۂ̃r����
	//! @param[in]     nMaxIteration �c �ő僋�[�v��
	//! @param[in]     tolerance      �c �ΐ��ޓx�̑ł��؂苖�e���Ό덷
	//! @param[out]    nIteration    �c ���ۂ̃��[�v��
	//!
	//! @retval true  �c �������z�̐���ɐ���
	//! @retval false �c �������z�̐���Ɏ��s�C�������͓��̓f�[�^����
	//! 
	template < class T, class Allocator >
	bool estimate_mixture( const array2< T, Allocator > &rSamples, mixture::distribution2 *opdp, size_t nComponents, double minimum1, double minimum2, double bin, size_t nMaxIteration, double tolerance, size_t &nIteration )
	{
		if( rSamples.empty( ) || nComponents == 0 )
		{
			return( false );
		}

		typedef size_t size_type;

		size_type i, j, k, m;

		const double pi = 3.1415926535897932384626433832795;
		const double _2pi = 2.0 * pi;
		const double _log_2pi = std::log( _2pi );
		double fLastLikelihood = -1.0e30;
		double number_of_samples;

		array3< double > Weight( rSamples.width( ), rSamples.height( ), nComponents );
		std::vector< mixture::distribution2 > pdp( nComponents );


		// ���̓f�[�^����ƃf�[�^�ɃR�s�[����
		for( m = 0 ; m < nComponents ; m++ )
		{
			pdp[ m ] = opdp[ m ];
		}

#if defined( EMALGORITHM_DEBUG ) && EMALGORITHM_DEBUG == 1
		for( m = 0 ; m < nComponents ; m++ )
		{
			mixture::distribution2 tmp = pdp[ m ];
			tmp.av.x += minimum1;
			tmp.av.y += minimum2;
			std::cerr << tmp << std::endl;
		}
#endif

		// �������z�f�[�^�̏d�݂̘a���P�ɐ��K������
		{
			double tmp = 0.0;
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
				pdp[ m ].av.x   -= minimum1;
				pdp[ m ].av.y   -= minimum2;
			}

			for( k = 0, number_of_samples = 0.0 ; k < rSamples.size( ) ; k++ )
			{
				number_of_samples += rSamples[ k ];
			}
		}


		// EM�A���S���Y���̊J�n
		for( nIteration = 0 ; nIteration < nMaxIteration ; nIteration++ )
		{
			// E-step

			for( j = 0 ; j < rSamples.height( ) ; j++ )
			{
				for( i = 0 ; i < rSamples.width( ) ; i++ )
				{
					if( rSamples( i, j ) == 0 )
					{
						continue;
					}

					double tmp = 0.0;

					for( m = 0 ; m < nComponents ; m++ )
					{
						double t = pdp[ m ].v[ 0 ] * pdp[ m ].v[ 3 ] - pdp[ m ].v[ 1 ] * pdp[ m ].v[ 2 ];
						double a = pdp[ m ].v[ 3 ];
						double b = -pdp[ m ].v[ 1 ];
						double c = -pdp[ m ].v[ 2 ];
						double d = pdp[ m ].v[ 0 ];
						double x = pdp[ m ].av.x - ( i + 0.5 ) * bin;
						double y = pdp[ m ].av.y - ( j + 0.5 ) * bin;
						double vvv = ( ( a * x + b * y ) * x + ( c * x + d * y ) * y ) / t;
						double v = pdp[ m ].weight * std::exp( - vvv / 2.0 ) / sqrt( t );
						Weight( i, j, m ) = v;
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
							Weight( i, j, m ) /= tmp;
						}
					}
				}
			}

			// M-step
			for( m = 0 ; m < nComponents ; m++ )
			{
				double	weight_sum = 0;
				double	avex = 0;
				double	avey = 0;
				double	v1 = 0;
				double	v2 = 0;
				double	v3 = 0;

				for( j = 0 ; j < rSamples.height( ) ; j++ )
				{
					for( i = 0 ; i < rSamples.width( ) ; i++ )
					{
						double w = Weight( i, j, m ) * rSamples( i, j );
						weight_sum += w;
						avex += static_cast< double >( ( i + 0.5 ) * bin ) * w;
						avey += static_cast< double >( ( j + 0.5 ) * bin ) * w;
					}
				}

				if( weight_sum > 0.0 )
				{
					pdp[ m ].weight = weight_sum / number_of_samples;
					pdp[ m ].av.x = avex / weight_sum;
					pdp[ m ].av.y = avey / weight_sum;

					for( j = 0 ; j < rSamples.height( ) ; j++ )
					{
						for( i = 0 ; i < rSamples.width( ) ; i++ )
						{
							double w   = Weight( i, j, m );
							double xx  = ( i + 0.5 ) * bin - pdp[ m ].av.x;
							double yy  = ( j + 0.5 ) * bin - pdp[ m ].av.y;
							double num = rSamples( i, j );
							v1 += w * xx * xx * num;
							v2 += w * yy * yy * num;
							v3 += w * xx * yy * num;
						}
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

			double fLikelihood = 0.0;

			for( j = 0 ; j < rSamples.height( ) ; j++ )
			{
				for( i = 0 ; i < rSamples.width( ) ; i++ )
				{
					if( rSamples( i, j ) == 0 )
					{
						continue;
					}

					double tmp = 0.0;

					for( m = 0 ; m < nComponents ; m++ )
					{
						double t = pdp[ m ].v[ 0 ] * pdp[ m ].v[ 3 ] - pdp[ m ].v[ 1 ] * pdp[ m ].v[ 2 ];
						double a = pdp[ m ].v[ 3 ];
						double b = -pdp[ m ].v[ 1 ];
						double c = -pdp[ m ].v[ 2 ];
						double d = pdp[ m ].v[ 0 ];
						double x = pdp[ m ].av.x - ( i + 0.5 ) * bin;
						double y = pdp[ m ].av.y - ( j + 0.5 ) * bin;
						double vvv = ( ( a * x + b * y ) * x + ( c * x + d * y ) * y ) / t;
						tmp += Weight( i, j, m ) * pdp[ m ].weight * std::exp( - vvv / 2.0 ) / std::sqrt( t );
					}

					if( tmp == 0.0 )
					{
						return( false );
					}

					fLikelihood += rSamples( i, j ) * ( std::log( tmp ) - _log_2pi );
				}
			}

#if defined( EMALGORITHM_DEBUG ) && EMALGORITHM_DEBUG == 1
			for( m = 0 ; m < nComponents ; m++ )
			{
				mixture::distribution2 tmp = pdp[ m ];
				tmp.av.x += minimum1;
				tmp.av.y += minimum2;
				std::cerr << tmp << std::endl;
			}
#elif defined( EMALGORITHM_DEBUG ) && EMALGORITHM_DEBUG == 2
			printf( "%f = ( %f, %f )\n", fLikelihood, pdp[ 0 ].weight, pdp[ 0 ].av );
#endif

			if( fLastLikelihood >= fLikelihood || 2.0 * std::abs( fLastLikelihood - fLikelihood ) < tolerance * ( std::abs( fLastLikelihood ) + std::abs( fLikelihood ) ) )
			{
				break;
			}

			// �o�͂ɍ�ƃf�[�^�𔽉f������
			for( m = 0 ; m < nComponents ; m++ )
			{
				opdp[ m ] = pdp[ m ];
			}

			fLastLikelihood = fLikelihood;
		}

		for( m = 0 ; m < nComponents ; m++ )
		{
			opdp[ m ].av.x += minimum1;
			opdp[ m ].av.y += minimum2;
		}

		return( true );
	}


	/// @brief �f�[�^�n�񂩂琳�K���z�̍������z�𐄒肷��
	//! 
	//! @attention ���͂ƂȂ�f�[�^�̔z��Ƃ��āCMIST�Œ񋟂���R���e�i��������STL�Œ񋟂���Ă���vector�Cdeque�R���e�i�����p�\�ł��D
	//! 
	//! @param[in]     rSamples      �c ���̓T���v��
	//! @param[in,out] pdp           �c ���z�p�����[�^
	//! @param[in]     minimum       �c �q�X�g�O�������쐬�����ۂ̍ŏ��l
	//! @param[in]     bin           �c �q�X�g�O�������쐬�����ۂ̃r����
	//! @param[in]     nMaxIteration �c �ő僋�[�v��
	//! @param[in]     tolerance      �c �ΐ��ޓx�̑ł��؂苖�e���Ό덷
	//! @param[out]    nIteration    �c ���ۂ̃��[�v��
	//!
	//! @retval true  �c �������z�̐���ɐ���
	//! @retval false �c �������z�̐���Ɏ��s�C�������͓��̓f�[�^����
	//! 
	template < class Array1, class Array2 >
	bool estimate_mixture( const Array1 &rSamples, Array2 &pdp, double minimum, double bin, typename Array1::size_type nMaxIteration, double tolerance, typename Array1::size_type &nIteration )
	{
		return( histogram::estimate_mixture( rSamples, &pdp[ 0 ], rSamples.size( ), pdp.size( ), minimum, bin, nMaxIteration, tolerance, nIteration ) );
	}


	/// @brief �f�[�^�n�񂩂�1�������K���z�̍������z�𐄒肷��
	//! 
	//! @attention ���͂ƂȂ�f�[�^�̔z��Ƃ��āCMIST�Œ񋟂���R���e�i��������STL�Œ񋟂���Ă���vector�Cdeque�R���e�i�����p�\�ł��D
	//! 
	//! @param[in]     rSamples      �c ���̓T���v��
	//! @param[in,out] pdp           �c ���z�p�����[�^
	//! @param[in]     minimum       �c �q�X�g�O�������쐬�����ۂ̍ŏ��l
	//! @param[in]     bin           �c �q�X�g�O�������쐬�����ۂ̃r����
	//! @param[in]     nMaxIteration �c �ő僋�[�v��
	//! @param[in]     tolerance     �c �ΐ��ޓx�̑ł��؂苖�e���Ό덷
	//!
	//! @retval true  �c �������z�̐���ɐ���
	//! @retval false �c �������z�̐���Ɏ��s�C�������͓��̓f�[�^����
	//! 
	template < class Array1, class Array2 >
	bool estimate_mixture( const Array1 &rSamples, Array2 &pdp, double minimum, double bin, typename Array1::size_type nMaxIteration, double tolerance )
	{
		typename Array1::size_type nIteration = 0;
		return( histogram::estimate_mixture( rSamples, &pdp[ 0 ], rSamples.size( ), pdp.size( ), minimum, bin, nMaxIteration, tolerance, nIteration ) );
	}


	/// @brief �f�[�^�n�񂩂琳�K���z�̍������z�𐄒肷��
	//! 
	//! @attention ���͂ƂȂ�f�[�^�̔z��Ƃ��āCMIST�Œ񋟂���R���e�i��������STL�Œ񋟂���Ă���vector�Cdeque�R���e�i�����p�\�ł��D
	//! 
	//! @param[in]     rSamples      �c ���̓T���v��
	//! @param[in,out] pdp           �c ���z�p�����[�^
	//! @param[in]     nComponents   �c ���肷�鍬�����z�̐�
	//! @param[in]     minimum       �c �q�X�g�O�������쐬�����ۂ̍ŏ��l
	//! @param[in]     bin           �c �q�X�g�O�������쐬�����ۂ̃r����
	//! @param[in]     nMaxIteration �c �ő僋�[�v��
	//! @param[in]     tolerance     �c �ΐ��ޓx�̑ł��؂苖�e���Ό덷
	//!
	//! @retval true  �c �������z�̐���ɐ���
	//! @retval false �c �������z�̐���Ɏ��s�C�������͓��̓f�[�^����
	//! 
	template < class Array >
	bool estimate_mixture( const Array &rSamples, mixture::distribution *pdp, typename Array::size_type nComponents, double minimum, double bin, typename Array::size_type nMaxIteration, double tolerance )
	{
		size_t nIteration = 0;
		return( histogram::estimate_mixture( rSamples, pdp, rSamples.size( ), nComponents, minimum, bin, nMaxIteration, tolerance, nIteration ) );
	}






	/// @brief �f�[�^�n�񂩂琳�K���z�̍������z�𐄒肷��
	//! 
	//! @attention ���͂ƂȂ�f�[�^�̔z��Ƃ��āCMIST�Œ񋟂���R���e�i��������STL�Œ񋟂���Ă���vector�Cdeque�R���e�i�����p�\�ł��D
	//! @attention ���̓f�[�^�� array2 �ŕ��z�� mixture::distribution2 �̏ꍇ��2�������K���z����ɂȂ�܂��D
	//! @attention ���������ϒl�����߂邽�߂ɂ́CEM�A���S���Y���̐��茋�ʂ̕��ϒl�ɁC���͂��ꂽ�q�X�g�O�����̃I�t�Z�b�g�𑫂��Ă��������D
	//! 
	//! @param[in]     rSamples      �c ���̓T���v��
	//! @param[in,out] pdp           �c ���z�p�����[�^
	//! @param[in]     minimum1      �c �q�X�g�O�������쐬�����ۂ̑�1�������ł̍ŏ��l
	//! @param[in]     minimum2      �c �q�X�g�O�������쐬�����ۂ̑�2�������ł̍ŏ��l
	//! @param[in]     bin           �c �q�X�g�O�������쐬�����ۂ̃r����
	//! @param[in]     nMaxIteration �c �ő僋�[�v��
	//! @param[in]     tolerance     �c �ΐ��ޓx�̑ł��؂苖�e���Ό덷
	//! @param[out]    nIteration    �c ���ۂ̃��[�v��
	//!
	//! @retval true  �c �������z�̐���ɐ���
	//! @retval false �c �������z�̐���Ɏ��s�C�������͓��̓f�[�^����
	//! 
	template < class T, class Allocator, class Array1 >
	bool estimate_mixture( const array2< T, Allocator > &rSamples, Array1 &pdp, double minimum1, double minimum2, double bin, typename Array1::size_type nMaxIteration, double tolerance, typename Array1::size_type &nIteration )
	{
		return( histogram::estimate_mixture( rSamples, &pdp[ 0 ], pdp.size( ), minimum1, minimum2, bin, nMaxIteration, tolerance, nIteration ) );
	}


	/// @brief �f�[�^�n�񂩂�1�������K���z�̍������z�𐄒肷��
	//! 
	//! @attention ���͂ƂȂ�f�[�^�̔z��Ƃ��āCMIST�Œ񋟂���R���e�i��������STL�Œ񋟂���Ă���vector�Cdeque�R���e�i�����p�\�ł��D
	//! @attention ���̓f�[�^�� array2 �ŕ��z�� mixture::distribution2 �̏ꍇ��2�������K���z����ɂȂ�܂��D
	//! @attention ���������ϒl�����߂邽�߂ɂ́CEM�A���S���Y���̐��茋�ʂ̕��ϒl�ɁC���͂��ꂽ�q�X�g�O�����̃I�t�Z�b�g�𑫂��Ă��������D
	//! 
	//! @param[in]     rSamples      �c ���̓T���v��
	//! @param[in,out] pdp           �c ���z�p�����[�^
	//! @param[in]     minimum1      �c �q�X�g�O�������쐬�����ۂ̑�1�������ł̍ŏ��l
	//! @param[in]     minimum2      �c �q�X�g�O�������쐬�����ۂ̑�2�������ł̍ŏ��l
	//! @param[in]     bin           �c �q�X�g�O�������쐬�����ۂ̃r����
	//! @param[in]     nMaxIteration �c �ő僋�[�v��
	//! @param[in]     tolerance     �c �ΐ��ޓx�̑ł��؂苖�e���Ό덷
	//!
	//! @retval true  �c �������z�̐���ɐ���
	//! @retval false �c �������z�̐���Ɏ��s�C�������͓��̓f�[�^����
	//! 
	template < class T, class Allocator, class Array1 >
	bool estimate_mixture( const array2< T, Allocator > &rSamples, Array1 &pdp, double minimum1, double minimum2, double bin, typename Array1::size_type nMaxIteration, double tolerance )
	{
		typename array< T, Allocator >::size_type nIteration = 0;
		return( histogram::estimate_mixture( rSamples, &pdp[ 0 ], pdp.size( ), minimum1, minimum2, bin, nMaxIteration, tolerance, nIteration ) );
	}


	/// @brief �f�[�^�n�񂩂�2�������K���z�̍������z�𐄒肷��
	//! 
	//! @attention ���͂ƂȂ�f�[�^�̔z��Ƃ��āCMIST�Œ񋟂���R���e�i��������STL�Œ񋟂���Ă���vector�Cdeque�R���e�i�����p�\�ł��D
	//! @attention ���̓f�[�^�́CMIST�Œ񋟂��� vector2 �^�C�v����K�v������܂��D
	//! @attention ���������ϒl�����߂邽�߂ɂ́CEM�A���S���Y���̐��茋�ʂ̕��ϒl�ɁC���͂��ꂽ�q�X�g�O�����̃I�t�Z�b�g�𑫂��Ă��������D
	//! 
	//! @param[in]     rSamples      �c ���̓T���v��
	//! @param[in,out] pdp           �c ���z�p�����[�^
	//! @param[in]     nComponents   �c ���肷�鍬�����z�̐�
	//! @param[in]     minimum1      �c �q�X�g�O�������쐬�����ۂ̑�1�������ł̍ŏ��l
	//! @param[in]     minimum2      �c �q�X�g�O�������쐬�����ۂ̑�2�������ł̍ŏ��l
	//! @param[in]     bin           �c �q�X�g�O�������쐬�����ۂ̃r����
	//! @param[in]     nMaxIteration �c �ő僋�[�v��
	//! @param[in]     tolerance     �c �ΐ��ޓx�̑ł��؂苖�e���Ό덷
	//!
	//! @retval true  �c �������z�̐���ɐ���
	//! @retval false �c �������z�̐���Ɏ��s�C�������͓��̓f�[�^����
	//! 
	template < class T, class Allocator >
	bool estimate_mixture( const array2< T, Allocator > &rSamples, mixture::distribution2 *pdp, typename array2< T, Allocator >::size_type nComponents, double minimum1, double minimum2, double bin, typename array2< T, Allocator >::size_type nMaxIteration, double tolerance )
	{
		size_t nIteration = 0;
		return( histogram::estimate_mixture( rSamples, pdp, nComponents, minimum1, minimum2, bin, nMaxIteration, tolerance, nIteration ) );
	}
}

/// @}
//  EM�A���S���Y����p�����������z�̐���O���[�v�̏I���



// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIXTURE__
