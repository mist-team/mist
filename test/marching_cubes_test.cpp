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

#include <mist/marching_cubes.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <glut.h>
// gcc でコンパイルする場合には，glut.h と libglut の設定が必要です（ 例：g++ -Wall -DNDEBUG marching_cubes.cpp -Ihoge -Lhoge -lglut ）
// vc7 の場合には，glut.h，glut32.lib，glut32.dll の入手・設定が必要です．

typedef mist::array3< double >						 image_type;
typedef mist::marching_cubes< image_type, double >	marching_cubes_type;
typedef marching_cubes_type::vector_type			vector_type;

marching_cubes_type mcs;							// marching_cubes オブジェクト
image_type			va( 64, 32, 32 );				// 入力用ボリュームデータ
std::vector< marching_cubes_type::vector_type >	pv;	// 等値面の頂点座標格納用
std::vector< marching_cubes_type::vector_type >	nv;	// 等値面の頂点の法線ベクトル格納用
std::vector< marching_cubes_type::size_type >	sv;	// 等値面の各ポリゴンの頂点数格納用
double									th = 31.5;	// 等値面生成の閾値
int										sign = -1;

inline double distance( const vector_type &p0, const vector_type &p1 )
{
	double L = ( p0.x - p1.x ) * ( p0.x - p1.x ) + ( p0.y - p1.y ) * ( p0.y - p1.y ) + ( p0.z - p1.z ) * ( p0.z - p1.z );
	return( std::sqrt( L ) );
}

inline double minimum( const double &v0, const double &v1 )
{
	return ( v0 < v1 ) ? v0 : v1; 
}

void disp( void );
void idle( void );

int main( int argc , char *argv[ ] )
{
	// 入力ボリュームデータの作成
	const vector_type c( ( va.width( ) - 1 ) / 2.0, ( va.height( ) - 1 ) / 2.0, ( va.depth( ) - 1 ) / 2.0 );
	vector_type c0( c ), c1( c );
	c0.x -= 8.0;
	c1.x += 8.0;
	for( size_t k = 0 ; k < va.depth( ) ; k ++ )
	{
		for( size_t j = 0 ; j < va.height( ) ; j ++ )
		{
			for( size_t i = 0 ; i < va.width( ) ; i ++ )
			{
				const vector_type p( static_cast< double >( i ), static_cast< double >( j ), static_cast< double >( k ) );
				const double d0 = distance( p, c0 );
				const double d1 = distance( p, c1 );
				va( i, j, k ) = 31.5 - minimum( d0, d1 );
				if( va( i, j, k ) < 0.0 )
				{
					va( i, j, k ) = 0.0;
				}
			}
		}
	}


	// 等値面生成の前処理（入力ボリュームデータを渡す）
	// 本サンプルのように描画の度に閾値を動的に変化させながら等値面生成を行う場合に生成処理時間を短縮できる
	// この行をコメントアウトしたものとしないものを比較すると処理速度の違いがわかる
	mcs.preprocess( va );
	
	// 等値面生成パラメータ設定	
	mcs.offset( -31.5, -15.5, -15.5 );
	mcs.scale( 0.1, 0.1, 0.1 );
	
	// 等値面生成結果格納のため，大きめの領域を確保しておくことで
	// ベクタの要素サイズの増加に伴うメモリ再確保の発生を抑制
	pv.reserve( 32768 );
	nv.reserve( 32768 );
	sv.reserve( 32768 );
	

	glutInit( &argc , argv );
	glutInitWindowPosition( 100 , 100 );
	glutInitWindowSize( 400 , 400 );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
	glutCreateWindow( "mist::marching_cubes" );
	glutDisplayFunc( disp );
	glutIdleFunc( idle );
	glMatrixMode( GL_PROJECTION );
	glFrustum( -1 , 1 , -1 , 1 , 1 , 5 );
	gluLookAt( 0.0, 0.0, 4.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0 );
	GLfloat lpos[ ] = { 0 , 0 , 4 , 1 };
	glLightfv( GL_LIGHT0 , GL_POSITION , lpos );
	glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0 );
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );
	glMatrixMode( GL_MODELVIEW );
	glutMainLoop( );

	return 0;	
}


static GLfloat mamb[ ] = { 1.0, 1.0, 1.0, 0.6 };
static GLfloat mdif[ ] = { 1.0, 1.0, 1.0, 0.6 }; 
static GLfloat mspc[ ] = { 1.0, 1.0, 1.0, 0.6 }; 
static GLfloat mshn = 5.0; 
static GLfloat r_ang = 0.0;

void disp( void )
{	
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glPushMatrix( );
	glRotated( r_ang, 0.0, 1.0, 0.0 );
	glMaterialfv( GL_FRONT, GL_AMBIENT,   mamb );
	glMaterialfv( GL_FRONT, GL_DIFFUSE,   mdif );
	glMaterialfv( GL_FRONT, GL_SPECULAR,  mspc );
	glMaterialf(  GL_FRONT, GL_SHININESS, mshn );
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_NORMAL_ARRAY );

	
	// 閾値を設定して等値面を生成する
	mcs.threshold( th );
	mcs.isosurfacing( va, pv, nv, sv );

	if( pv.size( ) > 0 )
	{
		// 生成結果として得られた頂点座標と法線ベクトルをGLの頂点配列に設定
		glVertexPointer( 3, GL_DOUBLE, 0, ( double * )&pv[ 0 ] );
		glNormalPointer(    GL_DOUBLE, 0, ( double * )&nv[ 0 ] );
		for( size_t i = 0, begin = 0; i < sv.size( ) ; i++ )
		{
			// 生成結果として得られた各ポリゴンの頂点数を利用
			glDrawArrays( GL_TRIANGLE_FAN, ( GLint )begin, ( GLsizei )sv[ i ] );
			begin += sv[ i ];
		}
	}

	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	glPopMatrix( );
	glutSwapBuffers();
}

void idle( void )
{
	r_ang += 1.0;
	if( r_ang >= 360.0 )
	{
		r_ang = 0.0;
	}
	th += sign * 0.1;
	if( th >= 31.5 || th <= 16.5  )
	{
		sign *= -1;
	}
	disp( ); // モデルの回転角度と等値面生成の閾値を変化させながらポリゴンモデルを表示
}
