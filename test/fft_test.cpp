#include <mist/mist.h>
#include <mist/fft/fft.h>
#include <mist/io/raw.h>
#include <stdio.h>
#include <time.h>

void main()
{
	int i;
	mist::array3< short > in;
	mist::array3< float > a3df;
	mist::array2< float > a2df;
	mist::array1< float > a1df;
	mist::array3< std::complex< float > > work3d;
	mist::array2< std::complex< float > > work2d;
	mist::array1< std::complex< float > > work1d;
	time_t start,end;

	mist::read_raw( in, "//eagle/data4/ctlung/haigeta/haigeta212.512.gz", 512, 512, 128 );

	///////////////////////////////////////////////////////////////////////////////////////////
	//1ŽŸŒ³FFT
	printf( "1D-FFT check( data size %d )\n", in.width() );
	a1df.resize( in.width() );
	for( i = 0 ; i < in.width() ; i++ )
	{
		a1df[i] = ( float )in[i];
	}

	printf("fft start\n");
	start = clock();

	mist::fft( a1df, work1d );

	end = clock();
	printf("fft end\n");
	printf("time %f sec.\n", (end - start )/(double)CLOCKS_PER_SEC );

	a1df.fill( 0.0f );

	printf("inverse_fft start\n");
	start = clock();	

	mist::inverse_fft( work1d, a1df );

	end = clock();
	printf("inverse_fft end\n");
	printf("time %f sec.\n", (end - start )/(double)CLOCKS_PER_SEC );

	for( i = 0 ; i < in.width() ; i++ )
	{
		if( in[i] != ( short )( a1df[i] +0.5f ) )
		{
			break;
		}
	}

	if( i == in.width() )
	{
		printf("result match\n\n");
	}else{
		printf("result miss-match\n\n");
	}

	a1df.clear();
	work1d.clear();

	///////////////////////////////////////////////////////////////////////////////////////////
	//2ŽŸŒ³FFT
	printf( "2D-FFT check( data size %d~%d )\n",in.width() ,in.height() );
	a2df.resize( in.width(), in.height() );
	for( i = 0 ; i < in.width() * in.height() ; i++ )
	{
		a2df[i] = ( float )in[i];
	}

	printf("fft start\n");
	start = clock();

	mist::fft( a2df, work2d );

	end = clock();
	printf("fft end\n");
	printf("time %f sec.\n", (end - start )/(double)CLOCKS_PER_SEC );

	a2df.fill( 0.0f );

	printf("inverse_fft start\n");
	start = clock();	

	mist::inverse_fft( work2d, a2df );

	end = clock();
	printf("inverse_fft end\n");
	printf("time %f sec.\n", (end - start )/(double)CLOCKS_PER_SEC );

	for( i = 0 ; i < in.width() * in.height() ; i++ )
	{
		if( in[i] != ( short )( a2df[i] +0.5f ) )
		{
			break;
		}
	}

	if( i == in.width() * in.height() )
	{
		printf("result match\n\n");
	}else{
		printf("result miss-match\n\n");
	}

	a2df.clear();
	work2d.clear();


	///////////////////////////////////////////////////////////////////////////////////////////
	//3ŽŸŒ³FFT
	printf( "3D-FFT check( data size %d~%d~%d )\n",in.width() ,in.height(),in.depth() );
	a3df.resize( in.width(), in.height(), in.depth() );
	for( i = 0 ; i < in.size() ; i++ )
	{
		a3df[i] = ( float )in[i];
	}

	printf("fft start\n");
	start = clock();

	mist::fft( a3df, work3d );

	end = clock();
	printf("fft end\n");
	printf("time %f sec.\n", (end - start )/(double)CLOCKS_PER_SEC );

	a3df.fill( 0.0f );

	printf("inverse_fft start\n");
	start = clock();	

	mist::inverse_fft( work3d, a3df );

	end = clock();
	printf("inverse_fft end\n");
	printf("time %f sec.\n", (end - start )/(double)CLOCKS_PER_SEC );

	for( i = 0 ; i < in.size() ; i++ )
	{
		if( in[i] != ( short )( a3df[i] +0.5f ) )
		{
			break;
		}
	}

	if( i == in.size() )
	{
		printf("result match\n\n");
	}else{
		printf("result miss-match\n\n");
	}

	a3df.clear();
	work3d.clear();
}
