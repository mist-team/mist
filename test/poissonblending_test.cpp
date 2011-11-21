#include <iostream>
#include <mist/mist.h>
#include <mist/timer.h>
#include <mist/io/image.h>
#include <mist/utility/options.h>

// poisson image editing
#include <mist/poisson_blending.h>

int main( int argc, char* argv[] )
{
	std::string header;
	header += "[ Useage ]\n";
	header += "./poissonblending [source image] [target image] [mask image] [out image]\n";
	header += "[ Options ]\n";

	mist::options op( header );
	op.add( "offset_x", "coodinate x of the offset of mask image", 0 );
	op.add( "offset_y", "coodinate y of the offset of mask image", 0 );
	op.add( "using_mixture", "if you'd like to use gradient mixture, set this flag" );

	if ( !op.parse_args( argc, argv ) )
	{
		op.show_help();		return 1;
	}
	else if ( op.size() < 4 )
	{
		std::cerr << "arguments error." << std::endl << std::endl;
		op.show_help();		return 1;
	}

	std::string src_file	= op[ 0 ];
	std::string target_file	= op[ 1 ];
	std::string mask_file	= op[ 2 ];
	std::string out_file	= op[ 3 ];
	int offx = op.get_int( "offset_x" );
	int offy = op.get_int( "offset_y" );
	bool mix = op.isset( "using_mixture" );

	//mist::array2< unsigned char > src_img;
	mist::array2< mist::rgb< unsigned char > > src_img;
	if ( !mist::read_image( src_img, src_file ) )
	{
		std::cerr << "src image read error." << std::endl;
		return -1;
	}

	//mist::array2< unsigned char > target_img;
	mist::array2< mist::rgb< unsigned char > > target_img;
	if ( !mist::read_image( target_img, target_file ) )
	{
		std::cerr << "target image read error." << std::endl;
		return -1;
	}

	mist::array2< unsigned char > mask_img;
	if ( !mist::read_image( mask_img, mask_file ) ) 
	{
		std::cerr << "mask image read error." << std::endl;
		return -1;
	}

	//mist::array2< unsigned char > dst_img;
	mist::array2< mist::rgb< unsigned char > > dst_img;
	mist::timer t;

	/* seamless Cloning */
	if ( !mist::seamlessCloning( src_img, target_img, dst_img, mask_img, offx, offy, mix  ) )
	{
		std::cerr << "cant make seamless clone image." << std::endl;
		return -1;
	}

	/* localColorChange (Grayscale background ver.) */
	//if ( !mist::localColorChange( src_img, dst_img, mask_img ) )
	//{
	//	std::cerr << "cant make seamless clone image." << std::endl;
	//	return -1;
	//}

	/* localColorChange (Change object color ver.) */
	//if ( !mist::localColorChange( src_img, dst_img, mask_img, mist::rgb<double>( 1.5, 0.5, 0.5 ) ) ) 
	//{
	//	std::cerr << "cant make seamless clone image." << std::endl;
	//	return -1;
	//}

	// output
	std::cout << t.elapse() <<" [ms]"<<std::endl;
	mist::write_image( dst_img, out_file );

	return 0;
}
