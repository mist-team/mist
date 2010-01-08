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

#include <iostream>
#include <mist/mist.h>
#include <mist/io/bmp.h>
#include <mist/filter/region_growing.h>
#include <mist/timer.h>

int main( int argc, char *argv[] )
{
	using namespace std;

	typedef mist::array2< unsigned int > image_type;
	typedef image_type::size_type size_type;
	typedef image_type::difference_type difference_type;

	image_type input( 100, 100 ), output;
	difference_type w = input.width( );
	difference_type h = input.height( );

	input.fill( 255 );

	for( difference_type j = 0 ; j < h ; j++ )
	{
		double ry = j - h / 2.0;

		for( difference_type i = 0 ; i < w ; i++ )
		{
			double rx = i - w / 2.0;

			if( std::abs( rx ) + std::abs( ry ) <= w * 0.4 )
			//if( rx * rx + ry * ry <= w * w  / 5.0 )
			{
				input( i, j ) = 0;
			}
		}
	}

	mist::timer t;

	typedef mist::vector2< int >								point_type;     // �̈�g���̊J�n�_���w�肷��^�imist::vector2 �� mist::vector3 �ő�p�j
//	typedef mist::region_growing_utility::pixel					component_type; // �̈�g���ɗp����\���v�f
	typedef mist::region_growing_utility::circle				component_type; // �̈�g���ɗp����\���v�f
	typedef mist::region_growing_utility::less< unsigned int >	condition_type; // �̈�g���̂̊g������

	mist::region_growing(
							input,							// ���͉摜
							output,							// �̈�g�����s�������ʂ��i�[����摜	
							point_type( (int)w / 2, (int)h / 2 ),		// �g���J�n�_�i���X�g�ɂ��邱�Ƃŕ����w��\�j
							128,							// �̈�g���̌��ʂɑ������l
							component_type( 20 ),			// �̈�g���ɗp����\���v�f�i��f�C�~�C���Ȃǁj
							condition_type( 128 )			// �̈�g���̂̊g�������i�����C�ȏ�C�͈͂Ȃǁj
						);

	cout << "Calculation Time: " << t << " (sec)" << endl;

	mist::write_bmp( output, "hoge.bmp" );

	return( 0 );
}
