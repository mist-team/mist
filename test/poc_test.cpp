#include <mist/mist.h>
#include <mist/io/image.h>
#include <mist/registration.h>

int main()
{ 
	mist::array2< mist::rgb< unsigned char > > image1, image2;
	mist::array2< mist::rgb< unsigned char > > transform_image;
	if( !mist::read_bmp( image1, "image1.bmp" ) )
	{
		std::cout << " Can't read image1." << std::endl;
	}
	if( !mist::read_bmp( image2, "image2.bmp" ) )
	{
		std::cout << " Can't read image2." << std::endl;
	}
	// doubleバージョン
	double dx, dy;
	mist::poc::estimate( image2,image1,  dx, dy, 1.0, 5 );
	std::cout << "( dx, dy ) = ( " << dx << ", " << dy << " )" << std::endl; 

	// vetor< double >バージョン
	//mist::vector2< double > dvec;
	//mist::optical_flow::poc( image2,image1,  dvec, 1.0, 5 );
	//std::cout << "( dx, dy ) = ( " << dvec << " ) " << std::endl; 

	mist::poc::transform( image2, transform_image, dx, dy, 4 );
	mist::write_bmp( transform_image, "transform_image.bmp" );

	getchar();
	return 0;
}