#include <stdio.h>

extern "C"
{
#include <f2c.h>
#include <clapack.h>
}

int main(void)
{
	/* LAPACK�֐��̈����i�����͌�q�j */
	long N=3, NRHS=1, LDA=3, IPIV[3], LDB=3, INFO;

	/* �}�g���b�N�XA��B
	�����ł�B�̓x�N�g���Ƃ���*/
	double A[3*3], B[3];

	/* ��Ɍ��ʏo�͂ŗp����J��Ԃ��p�����i�ǂ��ł������ϐ��j */
	int i;




	/* A�ɒl����
	���Ԃɗv����!!�@1��ڂ̏ォ�牺�ցC2��ڂ̏ォ�牺��,,,�̏��ɂȂ��Ă��� */
	A[0]= 1.0; A[3]= 1.0; A[6]= 1.0;
	A[1]= 3.0; A[4]= 1.0; A[7]=-3.0;
	A[2]= 1.0; A[5]=-2.0; A[8]=-5.0;

	/* B�ɒl���� */
	B[0]= 3.0;
	B[1]= 1.0;
	B[2]=-6.0;



	/* CLapack�̘A���ꎟ�������������֐�
	�y��������z
	long     N: �}�g���b�N�XA�̎���
	long  NRHS: �}�g���b�N�XB�̗񐔁i�����ł�B���x�N�g���Ȃ̂�1�j
	double*  A: �}�g���b�N�XA�̐��������񂾔z��iNxN�v�f�j
	long   LDA: �}�g���b�N�XA�̑�ꎟ��(�Ƃ肠���� N?)
	long* IPIV: �s�{�b�g�I���ɗp����z��iN�v�f�j
	double*  B: �}�g���b�N�XB�̐��������񂾔z��iNxNRHS�v�f�j
	long   LDB: �}�g���b�N�XB�̑�ꎟ��(�Ƃ肠���� N?)
	long  INFO: ����I���������ۂ���m�点�Ă����ϐ� */
	dgesv_(&N, &NRHS, A, &LDA, IPIV, B, &LDB, &INFO);



	/* ���ʂ̏o��
	B��X�ɕϊ�����ďo�͂���Ă��� */
	for(i=0; i<N; i++){ printf("%4.3f\n", B[i]); }

	/* �v�Z�������������ۂ���INFO�Œ��ׂĒm�点�� */
	if( INFO==0 ){ fprintf( stderr, "OK\n" ); }
	else{ fprintf( stderr, "NG\n" ); }



	return 0;
}

