#include <stdio.h>

extern "C"
{
#include <f2c.h>
#include <clapack.h>
}

int main(void)
{
	/* LAPACK関数の引数（説明は後述） */
	long N=3, NRHS=1, LDA=3, IPIV[3], LDB=3, INFO;

	/* マトリックスAとB
	ここではBはベクトルとする*/
	double A[3*3], B[3];

	/* 後に結果出力で用いる繰り返し用整数（どうでもいい変数） */
	int i;




	/* Aに値を代入
	順番に要注意!!　1列目の上から下へ，2列目の上から下へ,,,の順になっている */
	A[0]= 1.0; A[3]= 1.0; A[6]= 1.0;
	A[1]= 3.0; A[4]= 1.0; A[7]=-3.0;
	A[2]= 1.0; A[5]=-2.0; A[8]=-5.0;

	/* Bに値を代入 */
	B[0]= 3.0;
	B[1]= 1.0;
	B[2]=-6.0;



	/* CLapackの連立一次方程式を解く関数
	【引数解説】
	long     N: マトリックスAの次数
	long  NRHS: マトリックスBの列数（ここではBがベクトルなので1）
	double*  A: マトリックスAの成分が並んだ配列（NxN要素）
	long   LDA: マトリックスAの第一次元(とりあえず N?)
	long* IPIV: ピボット選択に用いる配列（N要素）
	double*  B: マトリックスBの成分が並んだ配列（NxNRHS要素）
	long   LDB: マトリックスBの第一次元(とりあえず N?)
	long  INFO: 正常終了したか否かを知らせてくれる変数 */
	dgesv_(&N, &NRHS, A, &LDA, IPIV, B, &LDB, &INFO);



	/* 結果の出力
	BがXに変換されて出力されている */
	for(i=0; i<N; i++){ printf("%4.3f\n", B[i]); }

	/* 計算が成功したか否かをINFOで調べて知らせる */
	if( INFO==0 ){ fprintf( stderr, "OK\n" ); }
	else{ fprintf( stderr, "NG\n" ); }



	return 0;
}

