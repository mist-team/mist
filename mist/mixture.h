/// @file mist/mixture.h
//!
//! @brief EM�A���S���Y����p�����������z�̐���
//!
//! - �Q�l����
//!   - �E�E�E
//!
#ifndef __INCLUDE_MIXTURE__
#define __INCLUDE_MIXTURE__

#include <stdio.h>
#include <math.h>

#include "mist/mist.h"
#include "mist/random.h"

// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


namespace __mixture__
{
	typedef struct	tagNORMDIST_PARAM
	{
		float	weight;	// �����d��
		float	av;		// ����
		float	sd;		// �W���΍�
	}NORMDIST_PARAM, *PNORMDIST_PARAM;

	int EstimateMixture(
		const mist::array<float>&		rSamples,		// [in]			���̓T���v��	
		mist::array<float>::size_type	nSamples,		// [in]			�T���v����
		int								nComponents,	// [in]			�������z�̕��z�v�f��
		int								nMaxIteration,	// [in]			�ő僋�[�v��
		float							fEpsilon,		// [in]			�ΐ��ޓx�̑ł��؂萸�x
		PNORMDIST_PARAM					pdp,			// [in, out]	���z�p�����[�^
		float*							pfLikelihood	// [out]		�ΐ��ޓx
		)
	{
		int	n;
		const float	pi = atan(1.0f) * 4.0f;
		float	fLikelihood;
		float	fLastLikelihood = -1.0e30f;
		float	tmp;
		float*	pWeight = new float[nSamples * nComponents];

#ifdef	_DEBUG
		printf("%d, %f, %f, %f, %f, %f, %f\n", 0, pdp[0].av, pdp[0].sd, pdp[0].weight, pdp[1].av, pdp[1].sd, pdp[1].weight);
#endif

		for(n = 0; n < nMaxIteration; n ++)
		{
			// E-step
			for(int k = 0; k < nSamples; k ++)
			{
				tmp = 0.0f;
				for(int m = 0; m < nComponents; m ++)
				{
					pWeight[k * nComponents + m] =
						pdp[m].weight *
						(1.0f / (sqrt(2.0f * pi) * pdp[m].sd)) *
						exp(- pow(rSamples[k] - pdp[m].av, 2) / (2.0f * pdp[m].sd * pdp[m].sd));

					tmp += pWeight[k * nComponents + m];
				}
				if(tmp == 0.0f)
				{
					return 0;

					printf("�޶!! ��٧!!\n");

					for(int m = 0; m < nComponents; m ++)
					{
						pWeight[k * nComponents + m] = 1.0f / nComponents;
					}
				}
				else
				{
					for(int m = 0; m < nComponents; m ++)
					{
						pWeight[k * nComponents + m] /= tmp;
					}
				}
			}

			// M-step
			for(int m = 0; m < nComponents; m ++)
			{
				float	weight_sum = 0;
				float	average = 0;
				float	variance = 0;

				for(int k = 0; k < nSamples; k ++)
				{
					weight_sum += pWeight[k * nComponents + m];
					average += rSamples[k] * pWeight[k * nComponents + m];
				}

				if(weight_sum > 0.0)
				{
					pdp[m].weight = weight_sum / nSamples;
					pdp[m].av = average / weight_sum;

					for(int k = 0; k < nSamples; k ++)
					{
						variance += pWeight[k * nComponents + m] * ((rSamples[k] * rSamples[k]) - (pdp[m].av * pdp[m].av));
					}
					variance /= weight_sum;
				}
				else
				{
					return 0;

					printf("����ځ[��\n");

					pdp[m].weight = 0.0f;

					for(int k = 0; k < nSamples; k ++)
					{
						average += rSamples[k];
					}
					pdp[m].av = average / nSamples;

					for(int k = 0; k < nSamples; k ++)
					{
						variance += ((rSamples[k] * rSamples[k]) - (pdp[m].av * pdp[m].av));
					}
					variance /= nSamples;
				}


				pdp[m].sd = sqrt(variance);
			}

			float	weight_sum = 0;
			for(int m = 0; m < nComponents; m ++)
			{
				weight_sum += pdp[m].weight;
			}
			if(fabs(weight_sum - 1.0f) > 0.1f)
			{
				return 0;

				printf("�����Ă�1�ɂȂ�Ȃ���\n");
			}

			fLikelihood = 0.0f;
			for(int k = 0; k < nSamples; k ++){
				tmp = 0.0f;
				for(int m = 0; m < nComponents; m ++)
				{
					tmp +=
						pWeight[k * nComponents + m] * 
						pdp[m].weight *
						(1.0f / (sqrt(2.0f * pi) * pdp[m].sd)) *
						exp(- pow(rSamples[k] - pdp[m].av, 2) / (2.0f * pdp[m].sd * pdp[m].sd));
				}
				if(tmp == 0.0f)
				{
					return 0;
				}
				fLikelihood += log(tmp);
			}

#ifdef	_DEBUG
			printf("%d (%f %f %f) (%f %f %f) %f\n", n, pdp[0].av, pdp[0].sd, pdp[0].weight, pdp[1].av, pdp[1].sd, pdp[1].weight, fLikelihood);
#endif

#ifdef	_DEBUG
			if(fLastLikelihood > fLikelihood)
			{
				printf("Error! Likelihood must not be decrease.\n");
				//break;
			}
			else
			{
				memcpy(dp_best, pdp, sizeof(NORMDIST_PARAM) * nComponents);
				fLikelihood_best = fLikelihood;
			}
#endif

			if(fabs(fLikelihood - fLastLikelihood) < fEpsilon)
			{
				break;
			}

			fLastLikelihood = fLikelihood;
		}

		delete [] pWeight;

		*pfLikelihood = fLikelihood;

		return n;
	}

}


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIXTURE__
