// 
// Copyright (c) 2003-2009, MIST Project, Intelligent Media Integration COE, Nagoya University
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

/// @file mist/io/mqo.h
//!
//! @brief Metasequoiaのデータを読み書きするためのライブラリ
//!
#ifndef __INCLUDE_MIST_MQO__
#define __INCLUDE_MIST_MQO__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

// ポリゴンオブジェクトの型を読み込む
#ifndef __INCLUDE_MIST_FACET__
#include "../facet.h"
#endif


#include <iostream>
#include <string>
#include <vector>



// mist名前空間の始まり
_MIST_BEGIN


namespace __mqo_controller__
{
	template < class T >
	struct mqo_controller
	{
		typedef facet_list< T > facet_list_type;
		typedef typename facet_list_type::facet_type facet_type;
		typedef typename facet_type::vector_type vector_type;
		typedef typename facet_type::size_type size_type;
		typedef vector3< size_type > ivector_type;

		static const unsigned char *get_line( const unsigned char *s, const unsigned char *e, std::string &line )
		{
			line = "";
			while( s < e )
			{
				if( s[ 0 ] == '\r' )
				{
					if( s + 1 != e && s[ 1 ] == '\n' )
					{
						s = s + 2;
					}
					else
					{
						s++;
					}
					break;
				}
				else if( s[ 0 ] == '\n' )
				{
					s = s + 1;
					break;
				}
				line += *s;
				s++;
			}
			return( s > e ? e : s );
		}

		static const unsigned char *get_value( const unsigned char *s, const unsigned char *e, std::string &line, bool &flag )
		{
			line = "";
			// 先頭の空白（改行やタブを含む）を飛ばす
			while( s < e )
			{
				if( flag && s[ 0 ] == '#' )
				{
					// コメント行なのですっ飛ばす
					while( s < e )
					{
						if( s[ 0 ] == '\r' )
						{
							if( s + 1 != e && s[ 1 ] == '\n' )
							{
								s = s + 2;
							}
							else
							{
								s++;
							}
							break;
						}
						else if( s[ 0 ] == '\n' )
						{
							s++;
							break;
						}
						s++;
					}
				}
				else if( s[ 0 ] == '\r' )
				{
					if( s + 1 != e && s[ 1 ] == '\n' )
					{
						s = s + 2;
					}
					else
					{
						s = s + 1;
					}
					flag = true;
				}
				else if( s[ 0 ] == '\n' )
				{
					s++;
					flag = true;
				}
				else if( s[ 0 ] == ' ' || s[ 0 ] == '\t' )
				{
					s++;
				}
				else
				{
					break;
				}
			}

			// 次に空白が来る前まで進める
			flag = false;
			while( s < e )
			{
				if( s[ 0 ] == '\r' )
				{
					if( s + 1 != e && s[ 1 ] == '\n' )
					{
						s = s + 2;
					}
					else
					{
						s++;
					}
					flag = true;
					break;
				}
				else if( s[ 0 ] == '\n' )
				{
					s++;
					flag = true;
					break;
				}
				else if( s[ 0 ] == ' ' || s[ 0 ] == '\t' )
				{
					s++;
					break;
				}
				line += *s;
				s++;
			}
			return( s > e ? e : s );
		}

		static size_type split_string( const std::string &line, const char ch, std::vector< std::string > &elements )
		{
			std::string str = "";
			size_type i = 0;
			elements.clear( );
			while( i < line.size( ) )
			{
				for( ; i < line.size( ) && line[ i ] == ch ; i++ );

				str = "";
				for( ; i < line.size( ) && line[ i ] != ch ; i++ )
				{
					str += line[ i ];
				}

				if( str != "" )
				{
					elements.push_back( str );
				}
			}
			return( elements.size( ) );
		}

		static std::string to_lower( std::string line )
		{
			for( size_t i = 0 ; i < line.size( ) ; i++ )
			{
				if( 'A' <= line[ i ] && line[ i ] <= 'Z' )
				{
					line[ i ] += 'a' - 'A';
				}
			}
			return( line );
		}

		static const unsigned char *skip_chunk( const unsigned char *p, const unsigned char *e, const std::string &ch )
		{
			std::string line = "";

			bool eol = true;
			while( p < e )
			{
				p = get_line( p, e, line );

				if( line.find_last_of( ch ) != std::string::npos )
				{
					return( p );
				}
			}

			return( NULL );
		}

		static const unsigned char *skip_chunk( const unsigned char *p, const unsigned char *e, const char ch )
		{
			std::string line = "";

			bool eol = true;
			while( p < e )
			{
				p = get_line( p, e, line );

				for( size_type i = 0 ; i < line.size( ) ; )
				{
					// SJISの2倍と文字はスキップする
					unsigned char v1 = line[ i ];
					unsigned char v2 = i + 1 < line.size( ) ? line[ i + 1 ] : 0x00;

					if( ( ( 0x81 <= v1 && v1 <= 0x9f ) || ( 0xe0 <= v1 && v1 <= 0xef ) ) && ( 0x40 <= v2 && v2 <= 0xfc && v2 != 0x7f ) )
					{
						i += 2;
					}
					else if( v1 == ch )
					{
						return( p );
					}
					else
					{
						i++;
					}
				}
			}

			return( NULL );
		}

		static const unsigned char *process_vertex_chunk( std::vector< vector_type > &vertices, const unsigned char *p, const unsigned char *e )
		{
			std::string line = "";
			std::vector< std::string > elements;

			bool eol = true;

			// 頂点数を読み込む
			p = get_value( p, e, line, eol );

			if( eol )
			{
				std::cerr << "EOL was found before vertex count appeared!!" << std::endl;
				return( NULL );
			}

			size_type num_vertices = atoi( line.c_str( ) );

			// 末尾を読み込む
			p = skip_chunk( p, e, "{" );

			if( p == NULL )
			{
				std::cerr << "'{' was not found before EOL appeared!!" << std::endl;
				return( NULL );
			}

			vertices.clear( );
			vertices.reserve( num_vertices );

			// vertex チャンクを処理する
			while( p < e )
			{
				// オブジェクト名を読み込む
				p = get_line( p, e, line );

				if( line.find_last_of( '}' ) != std::string::npos )
				{
					// vertex チャンクの終了
					break;
				}

				if( split_string( line, ' ', elements ) != 3 )
				{
					std::cerr << "Vertex must have three components." << std::endl;
					return( NULL );
				}

				vertices.push_back( vector_type( atof( elements[ 0 ].c_str( ) ), atof( elements[ 1 ].c_str( ) ), atof( elements[ 2 ].c_str( ) ) ) );
			}

			return( vertices.size( ) == num_vertices ? p : NULL );
		}

		static const unsigned char *process_face_chunk( std::vector< ivector_type > &faces, const unsigned char *p, const unsigned char *e )
		{
			std::string line = "";
			std::vector< std::string > elements;

			bool eol = true;

			// 頂点数を読み込む
			p = get_value( p, e, line, eol );

			if( eol )
			{
				std::cerr << "EOL was found before face count appeared!!" << std::endl;
				return( NULL );
			}

			size_type num_faces = atoi( line.c_str( ) );

			// 末尾を読み込む
			p = skip_chunk( p, e, "{" );

			if( p == NULL )
			{
				std::cerr << "'{' was not found before EOL appeared!!" << std::endl;
				return( NULL );
			}

			faces.clear( );
			faces.reserve( num_faces );

			// vertex チャンクを処理する
			while( p < e )
			{
				// オブジェクト名を読み込む
				p = get_line( p, e, line );

				if( line.find_last_of( '}' ) != std::string::npos )
				{
					// vertex チャンクの終了
					break;
				}

				int num = 0;
				sscanf( line.c_str( ), "%d", &num );

				size_type si = line.find( "V(" );
				if( si == std::string::npos )
				{
					std::cerr << "Incorrect face chunk is found." << std::endl;
					return( NULL );
				}
				else
				{
					si += 2;
				}

				size_type ei = line.find( ')', si + 1 );
				if( ei == std::string::npos )
				{
					std::cerr << "Incorrect face chunk is found." << std::endl;
					return( NULL );
				}
				else
				{
					ei -= 1;
				}

				line = line.substr( si, ei - si + 1 );

				if( split_string( line, ' ', elements ) != num )
				{
					std::cerr << "Incorrect face chunk is found." << std::endl;
					return( NULL );
				}
				else if( num < 3 )
				{
					// 未サポート
					std::cerr << "Face should have more than three vertices." << std::endl;
					continue;
				}
				else if( num > 4 )
				{
					// 未サポート
					std::cerr << "Face should have less than five vertices." << std::endl;
					continue;
				}

				if( num == 3 )
				{
					// 頂点が時計回りで並んでいるので，反時計回りに変換する
					faces.push_back( ivector_type( atoi( elements[ 0 ].c_str( ) ), atoi( elements[ 2 ].c_str( ) ), atoi( elements[ 1 ].c_str( ) ) ) );
				}
				else
				{
					// 頂点が時計回りで並んでいるので，反時計回りに変換する
					faces.push_back( ivector_type( atoi( elements[ 0 ].c_str( ) ), atoi( elements[ 3 ].c_str( ) ), atoi( elements[ 2 ].c_str( ) ) ) );
					faces.push_back( ivector_type( atoi( elements[ 0 ].c_str( ) ), atoi( elements[ 2 ].c_str( ) ), atoi( elements[ 1 ].c_str( ) ) ) );
					num_faces++;
				}
			}

			return( faces.size( ) == num_faces ? p : NULL );
		}

		static const unsigned char *process_object_chunk( facet_list_type &facets, const unsigned char *p, const unsigned char *e )
		{
			std::string line = "";

			bool eol = true;

			// オブジェクト名を読み込む
			p = get_value( p, e, line, eol );

			if( eol )
			{
				std::cerr << "EOL was found before object name appeared!!" << std::endl;
				return( NULL );
			}

			if( line.size( ) > 0 && line[ 0 ] == '\"' && line[ line.size( ) - 1 ] == '\"' )
			{
				facets.name = line.substr( 1, line.size( ) - 2 );
			}
			else
			{
				facets.name = line;
			}

			// 末尾を読み込む
			p = skip_chunk( p, e, "{" );

			if( p == NULL )
			{
				std::cerr << "'{' was not found before EOL appeared!!" << std::endl;
				return( NULL );
			}

			std::vector< vector_type > vertices;
			std::vector< ivector_type > faces;

			// object チャンクを処理する
			while( p != NULL && p < e )
			{
				// オブジェクト名を読み込む
				p = get_value( p, e, line, eol );
				line = to_lower( line );

				if( line == "}" )
				{
					// object チャンクの終了
					break;
				}
				else if( line == "depth" )
				{
				}
				else if( line == "folding" )
				{
				}
				else if( line == "scale" )
				{
				}
				else if( line == "rotation" )
				{
				}
				else if( line == "translation" )
				{
				}
				else if( line == "patch" )
				{
				}
				else if( line == "segment" )
				{
				}
				else if( line == "visible" )
				{
				}
				else if( line == "locking" )
				{
				}
				else if( line == "shading" )
				{
				}
				else if( line == "facet" )
				{
				}
				else if( line == "color" )
				{
				}
				else if( line == "color_type" )
				{
				}
				else if( line == "mirror" )
				{
				}
				else if( line == "mirror_axis" )
				{
				}
				else if( line == "mirror_dis" )
				{
				}
				else if( line == "lathe" )
				{
				}
				else if( line == "lathe_axis" )
				{
				}
				else if( line == "lathe_seg" )
				{
				}
				else if( line == "vertexattr" )
				{
					// 未サポート
					std::cerr << "vertexattr chunk is currently not supported."  << std::endl;
					return( NULL );
				}
				else if( line == "bvertex" )
				{
					// 未サポート
					std::cerr << "BVertex chunk is currently not supported."  << std::endl;
					return( NULL );
				}
				else if( line == "vertex" )
				{
					p = process_vertex_chunk( vertices, p, e );
					eol = true;
				}
				else if( line == "face" )
				{
					p = process_face_chunk( faces, p, e );
					eol = true;
				}
				else
				{
					// 未サポート
					std::cerr << "Unknown chunk is found!!"  << std::endl;
					return( NULL );
				}

				if( !eol )
				{
					p = get_line( p, e, line );
				}
			}

			if( p == NULL )
			{
				return( NULL );
			}

			facets.clear( );
			facets.reserve( faces.size( ) );

			for( size_type i = 0 ; i < faces.size( ) ; i++ )
			{
				const ivector_type &v = faces[ i ];

				facet_type f;
				if( 0 <= v.x && v.x < vertices.size( ) )
				{
					f.p1 = vertices[ v.x ];
				}
				else
				{
					std::cerr << "Incorrect face found: (" << v << ")" << std::endl;
				}

				if( 0 <= v.y && v.y < vertices.size( ) )
				{
					f.p2 = vertices[ v.y ];
				}
				else
				{
					std::cerr << "Incorrect face found: (" << v << ")" << std::endl;
				}

				if( 0 <= v.z && v.z < vertices.size( ) )
				{
					f.p3 = vertices[ v.z ];
				}
				else
				{
					std::cerr << "Incorrect face found: (" << v << ")" << std::endl;
				}

				f.normal = ( ( f.p2 - f.p1 ).outer( f.p3 - f.p1 ) ).unit( );

				facets.push_back( f );
			}

			return( p );
		}

		static const unsigned char * __process_object_chunk__( std::vector< facet_list_type > &facet_lists, const unsigned char *p, const unsigned char *e )
		{
			facet_lists.push_back( facet_list_type( ) );
			facet_list_type &facets = facet_lists.back( );
			p = process_object_chunk( facets, p, e );
			if( p == NULL )
			{
				facet_lists.pop_back( );
			}

			return( p );
		}

		static const unsigned char * __process_object_chunk__( facet_list_type &facets, const unsigned char *p, const unsigned char *e )
		{
			facet_list_type fff;
			p = process_object_chunk( fff, p, e );
			if( p != NULL )
			{
				facets.insert( facets.end( ), fff.begin( ), fff.end( ) );
			}

			return( p );
		}

		template < class FACETLIST >
		static bool convert_from_data( FACETLIST &facet_lists, const unsigned char *buff, size_type len )
		{
			const unsigned char *p = buff;
			const unsigned char *e = buff + len;
			std::string line = "";
			std::vector< std::string > elements;

			{
				// ヘッダ部分を解析
				// Metasequoia Document
				// Format %s Ver %.1f
				p = get_line( p, e, line );
				if( line != "Metasequoia Document" )
				{
					std::cerr << "This is not a Metasequoia Document." << std::endl;
					return( false );
				}

				char buff[ 4096 ];
				float ver = 0.0;
				p = get_line( p, e, line );
				if( sscanf( line.c_str( ), "Format %s Ver %.1f", buff, ver ) <= 0 )
				{
					std::cerr << "This is not a Metasequoia Document." << std::endl;
					return( false );
				}
			}

			bool eol = true;
			while( p != NULL && p < e )
			{
				p = get_value( p, e, line, eol );

				line = to_lower( line );

				if( line == "" )
				{
					continue;
				}
				else if( line == "eof" )
				{
					continue;
				}
				else if( line == "trialnoise" )
				{
					p = skip_chunk( p, e, '}' );
				}
				else if( line == "includexml" )
				{
					p = skip_chunk( p, e, '}' );
				}
				else if( line == "scene" )
				{
					p = skip_chunk( p, e, '}' );
				}
				else if( line == "backimage" )
				{
					p = skip_chunk( p, e, '}' );
				}
				else if( line == "material" )
				{
					p = skip_chunk( p, e, '}' );
				}
				else if( line == "blob" )
				{
					p = skip_chunk( p, e, '}' );
				}
				else if( line == "object" )
				{
					p = __process_object_chunk__( facet_lists, p, e );
				}
				else
				{
					std::cerr << "Unknown chunk found!!" << std::endl;
					return( false );
				}
			}

			return( true );
		}

		template < class FACETLIST >
		static bool read( FACETLIST &facet_lists, const std::string &filename )
		{
			// データをクリアする
			facet_lists.clear( );

			size_type filesize;
			FILE *fp;
			if( ( fp = fopen( filename.c_str( ), "rb" ) ) == NULL ) return( false );
			// ファイルサイズを取得
			fseek( fp, 0, SEEK_END );
			filesize = ftell( fp );
			fseek( fp, 0, SEEK_SET );

			// すべてのデータをファイルから読み込む
			unsigned char *buff = new unsigned char[ filesize + 1 ];
			unsigned char *pointer = buff;
			size_type read_size = 0;
			while( feof( fp ) == 0 )
			{
				read_size = fread( pointer, sizeof( unsigned char ), 1024, fp );
				if( read_size < 1024 )
				{
					break;
				}
				pointer += read_size;
			}
			fclose( fp );

			bool ret = convert_from_data( facet_lists, buff, filesize );

			delete [] buff;

			return( ret );
		}

		static bool write( const std::vector< facet_list_type > &facet_lists, const std::string &filename )
		{
			if( facet_lists.empty( ) )
			{
				std::cerr << "There is no object!" << std::endl;
				return( false );
			}

			FILE *fp;
			if( ( fp = fopen( filename.c_str( ), "wb" ) ) == NULL )
			{
				return( false );
			}

			fprintf( fp, "Metasequoia Document\r\n" );
			fprintf( fp, "Format Text Ver 1.0\r\n\r\n" );

			{
				fprintf( fp, "Scene {\r\n" );
				fprintf( fp, "\tpos 0.0000 0.0000 1000.0\r\n" );
				fprintf( fp, "\tlookat 0.0000 0.0000 0.0000\r\n" );
				fprintf( fp, "\thead 0.0000\r\n" );
				fprintf( fp, "\tpich 0.0000\r\n" );
				fprintf( fp, "\tortho 0\r\n" );
				fprintf( fp, "\tzoom2 5.0\r\n" );
				fprintf( fp, "\tamb 0.250 0.250 0.250\r\n" );
				fprintf( fp, "}\r\n" );
			}
			{
				fprintf( fp, "Material 1 {\r\n" );
				fprintf( fp, "\t\"default\" shader(3) col(1.000 1.000 1.000 1.000) dif(0.800) amb(0.600) emi(0.000) spc(0.000) power(5.00)\r\n" );
				fprintf( fp, "}\r\n" );
			}

			for( size_type l = 0 ; l < facet_lists.size( ) ; l++ )
			{
				const facet_list_type &facets = facet_lists[ l ];

				std::vector< vector_type > vertices;
				std::vector< ivector_type > faces;
				if( convert_to_vertex_face_list( facets, vertices, faces ) )
				{
					fprintf( fp, "Object \"%s\" {\r\n", facets.name.c_str( ) );
					fprintf( fp, "\tvisible 15\r\n" );
					fprintf( fp, "\tlocking 0\r\n" );
					fprintf( fp, "\tshading 1\r\n" );
					fprintf( fp, "\tcolor 1.000 1.000 1.000\r\n" );
					fprintf( fp, "\tcolor_type 0\r\n" );

					{
						fprintf( fp, "\tvertex %d {\r\n", static_cast< int >( vertices.size( ) ) );
						for( size_type i = 0 ; i < vertices.size( ) ; i++ )
						{
							const vector_type &f = vertices[ i ];
							double fx = ( f.x * 1e4 + 0.5 ) * 1e-4;
							double fy = ( f.y * 1e4 + 0.5 ) * 1e-4;
							double fz = ( f.z * 1e4 + 0.5 ) * 1e-4;
							fprintf( fp, "\t\t%.4f %.4f %.4f\r\n", f.x, f.y, f.z );
						}
						fprintf( fp, "\t}\r\n" );
					}

					{
						fprintf( fp, "\tface %d {\r\n", static_cast< int >( faces.size( ) ) );
						for( size_type i = 0 ; i < faces.size( ) ; i++ )
						{
							const ivector_type &f = faces[ i ];

							// 頂点は反時計回りに並んでいるため，時計回りで出力する
							fprintf( fp, "\t\t%d V(%d %d %d) M(0)\r\n", 3, f.x, f.z, f.y );
						}
						fprintf( fp, "\t}\r\n" );
					}

					fprintf( fp, "}\r\n" );
				}
			}

			fprintf( fp, "Eof" );

			fclose( fp );

			return( true );
		}
	};
}


//! @addtogroup cad_group CADデータ入出力
//!  @{

//! @addtogroup cad_mqo_group Metasequoia形式のポリゴンデータの入出力
//!
//! Metasequoia形式のファイルからポリゴンデータを読み込む．
//! また，ポリゴンデータからMetasequoia形式のファイルへの書き出しをサポート．
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/io/mqo.h>
//! @endcode
//!
//!
//!  @{


/// @brief Metasequoia形式のファイルからポリゴンデータを読み込む
//! 
//! @param[out] facet_lists … ポリゴンのリスト（オブジェクト単位のポリゴン集合リスト）
//! @param[in]  filename    … 出力ファイル名
//!
//! @retval true  … ポリゴンデータの読み込みに成功
//! @retval false … ポリゴンデータの読み込みに失敗
//! 
template < class T >
bool read_mqo( std::vector< facet_list< T > > &facet_lists, const std::string &filename )
{
	return( __mqo_controller__::mqo_controller< T >::read( facet_lists, filename ) );
}


/// @brief Metasequoia形式のファイルからポリゴンデータを読み込む
//! 
//! @param[out] facet_lists … ポリゴンのリスト（オブジェクト単位のポリゴン集合リスト）
//! @param[in]  filename    … 出力ファイル名
//!
//! @retval true  … ポリゴンデータの読み込みに成功
//! @retval false … ポリゴンデータの読み込みに失敗
//! 
template < class T >
bool read_mqo( std::vector< facet_list< T > > &facet_lists, const std::wstring &filename )
{
	return( read_mqo( facet_lists, wstr2str( filename ) ) );
}


/// @brief Metasequoia形式のファイルからポリゴンデータを読み込む
//! 
//! @param[out] facets   … ポリゴンの集合
//! @param[in]  filename … 出力ファイル名
//!
//! @retval true  … ポリゴンデータの読み込みに成功
//! @retval false … ポリゴンデータの読み込みに失敗
//! 
template < class T >
bool read_mqo( facet_list< T > &facets, const std::string &filename )
{
	return( __mqo_controller__::mqo_controller< T >::read( facets, filename ) );
}


/// @brief Metasequoia形式のファイルからポリゴンデータを読み込む
//! 
//! @param[out] facets   … ポリゴンの集合
//! @param[in]  filename … 出力ファイル名
//!
//! @retval true  … ポリゴンデータの読み込みに成功
//! @retval false … ポリゴンデータの読み込みに失敗
//! 
template < class T >
bool read_mqo( facet_list< T > &facets, const std::wstring &filename )
{
	return( read_mqo( facets, wstr2str( filename ) ) );
}


/// @brief ポリゴンデータをMetasequoia形式で出力する
//!
//! @param[in] facet_lists      … ポリゴンのリスト
//! @param[in] filename         … 出力ファイル名
//!
//! @retval true  … ファイルへの書き込みに成功
//! @retval false … ファイルへの書き込みに失敗
//! 
template < class T >
inline bool write_mqo( const std::vector< facet_list< T > > &facet_lists, const std::string &filename )
{
	return( __mqo_controller__::mqo_controller< T >::write( facet_lists, filename ) );
}

/// @brief ポリゴンデータをMetasequoia形式で出力する
//!
//! @param[in] facet_lists      … ポリゴンのリスト
//! @param[in] filename         … 出力ファイル名
//!
//! @retval true  … ファイルへの書き込みに成功
//! @retval false … ファイルへの書き込みに失敗
//! 
template < class T >
inline bool write_mqo( const std::vector< facet_list< T > > &facet_lists, const std::wstring &filename )
{
	return( write_mqo( facet_lists, wstr2str( filename ) ) );
}

/// @brief ポリゴンデータをMetasequoia形式で出力する
//!
//! @param[in] facets           … ポリゴンのリスト
//! @param[in] filename         … 出力ファイル名
//!
//! @retval true  … ファイルへの書き込みに成功
//! @retval false … ファイルへの書き込みに失敗
//! 
template < class T >
inline bool write_mqo( const facet_list< T > &facets, const std::string &filename )
{
	std::vector< facet_list< T > > tmp;
	tmp.push_back( facets );
	return( __mqo_controller__::mqo_controller< T >::write( tmp, filename ) );
}


/// @brief ポリゴンデータをMetasequoia形式で出力する
//!
//! @param[in] facets           … ポリゴンのリスト
//! @param[in] filename         … 出力ファイル名
//!
//! @retval true  … ファイルへの書き込みに成功
//! @retval false … ファイルへの書き込みに失敗
//! 
template < class T >
inline bool write_mqo( const facet_list< T > &facets, const std::wstring &filename )
{
	return( write_mqo( tmp, wstr2str( filename ) ) );
}



/// @}
//  Metasequoiaのポリゴンデータの入出力グループの終わり

/// @}
//  CADデータ入出力グループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_MQO__
