#include <mist/mist.h>
#include <mist/io/image.h>
#include <mist/registration.h>
#include <mist/random.h>
#include <ctime>

int main()
{ 
	mist::array2< mist::rgb< unsigned char > > image1, image2;
	if( !mist::read_bmp( image1, "image1.bmp" ) )
	{
		std::cout << " Can't read image1." << std::endl;
	}
	image2.resize( image1.width(), image1.height() );
	
	mist::uniform::random rnd( std::clock( ) );// 乱数生成

	double cx, cy;
	cx = rnd.real1();
	cy = rnd.real1();
	std::cout << "correct  ( dx, dy ) = ( " << cx << ", " << cy << " )" << std::endl; 
	mist::cubic::transform( image1, image2, cx, cy );

	// doubleバージョン
	double ex, ey;
	mist::poc::estimate( image2,image1,  ex, ey, 0.85 );
	std::cout << "estimate ( dx, dy ) = ( " << ex << ", " << ey << " )" << std::endl; 
	std::cout << "error    ( dx, dy ) = ( " << ex - cx << ", " << ey - cy << " )" << std::endl; 
	// vetor< double >バージョン
	//mist::vector2< double > dvec;
	//mist::optical_flow::poc( image2,image1,  dvec, 1.0, 5 );
	//std::cout << "( dx, dy ) = ( " << dvec << " ) " << std::endl; 

	mist::array2< mist::rgb< unsigned char > > transform_image( image2.width(), image2.height() );
	mist::poc::transform( image2, transform_image, ex, ey );
	mist::write_bmp( transform_image, "transform_image.bmp" );
	
	getchar();
	return 0;
}