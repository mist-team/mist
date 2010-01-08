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

/// @file mist/io/stl.h
//!
//! @brief STL形式のCADデータを読み書きするためのライブラリ
//!
#ifndef __INCLUDE_MIST_STL__
#define __INCLUDE_MIST_STL__


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


namespace __stl_controller__
{
	struct _facet_
	{
		enum{ bytes = 50 };
		float nX;
		float nY;
		float nZ;
		float p1X;
		float p1Y;
		float p1Z;
		float p2X;
		float p2Y;
		float p2Z;
		float p3X;
		float p3Y;
		float p3Z;
		unsigned short dmy;
	} _MIST_PACKED;

	template < class T >
	struct stl_controller
	{
		typedef facet_list< T > facet_list_type;
		typedef typename facet_list_type::facet_type facet_type;
		typedef typename facet_type::vector_type vector_type;
		typedef typename facet_type::size_type size_type;

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

		static bool convert_from_stl_ascii_data( facet_list_type &facets, const unsigned char *buff, size_type len )
		{
			const unsigned char *p = buff;
			const unsigned char *e = buff + len;
			std::string line = "";
			std::vector< std::string > elements;

			facet_type facet;

			int stage  = 0;
			int vcount = 0;
			bool eol = true;
			while( p < e )
			{
				p = get_value( p, e, line, eol );
				line = to_lower( line );

				switch( stage )
				{
				case 0:
					if( line == "solid" )
					{
						stage++;
						if( !eol )
						{
							p = get_line( p, e, facets.name );
							eol = true;
						}
					}
					else
					{
						std::cerr << "File does not start with 'solid'" << std::endl;
						return( false );
					}
					break;

				case 1:
					if( line == "endsolid" )
					{
						stage--;
						return( true );
					}
					else if( line == "facet" )
					{
						stage++;
						vcount = 0;
						facet.normal = vector_type( 0, 0, 1 );
						facet.p1 = facet.p2 = facet.p3 = vector_type( );

						if( !eol )
						{
							p = get_line( p, e, line );
							eol = true;
							std::vector< std::string > elements;
							split_string( line, ' ', elements );

							if( elements.size( ) == 4 )
							{
								if( to_lower( elements[ 0 ] ) == "normal" )
								{
									facet.normal.x = atof( elements[ 1 ].c_str( ) );
									facet.normal.y = atof( elements[ 2 ].c_str( ) );
									facet.normal.z = atof( elements[ 3 ].c_str( ) );
								}
								else
								{
									std::cerr << "'facet' does not have 'normal'" << std::endl;
								}
							}
							else
							{
								std::cerr << "'facet' does not have 'normal'" << std::endl;
							}
						}
						else
						{
							std::cerr << "'facet' does not have 'normal'" << std::endl;
							return( false );
						}
					}
					else
					{
						std::cerr << "Line does not start with 'facet'" << std::endl;
						return( false );
					}
					break;

				case 2:
					if( line == "endfacet" )
					{
						stage--;

						// ポリゴンを登録する
						if( vcount >= 3 )
						{
							if( facet.normal.inner( ( facet.p2 - facet.p1 ).outer( facet.p3 - facet.p1 ) ) < 0 )
							{
								vector_type tmp = facet.p2;
								facet.p2 = facet.p3;
								facet.p3 = tmp;
							}

							facets.push_back( facet );
						}
						else
						{
							std::cerr << "Facet does not have enough vertices" << std::endl;
							return( false );
						}
					}
					else if( line == "outer" )
					{
						stage++;

						// 次の単語を読み込む
						p = get_value( p, e, line, eol );
						line = to_lower( line );

						if( line != "loop" )
						{
							std::cerr << "Line does not start with 'outer loop'" << std::endl;
							return( false );
						}
					}
					else
					{
						std::cerr << "Line does not start with 'outer loop'" << std::endl;
						return( false );
					}
					break;

				case 3:
					if( line == "endloop" )
					{
						stage--;
					}
					else if( line == "vertex" )
					{
						if( !eol )
						{
							p = get_line( p, e, line );
							eol = true;
							std::vector< std::string > elements;
							split_string( line, ' ', elements );

							if( elements.size( ) == 3 )
							{
								switch( vcount )
								{
								case 0:
									facet.p1.x = atof( elements[ 0 ].c_str( ) );
									facet.p1.y = atof( elements[ 1 ].c_str( ) );
									facet.p1.z = atof( elements[ 2 ].c_str( ) );
									break;

								case 1:
									facet.p2.x = atof( elements[ 0 ].c_str( ) );
									facet.p2.y = atof( elements[ 1 ].c_str( ) );
									facet.p2.z = atof( elements[ 2 ].c_str( ) );
									break;

								case 2:
									facet.p3.x = atof( elements[ 0 ].c_str( ) );
									facet.p3.y = atof( elements[ 1 ].c_str( ) );
									facet.p3.z = atof( elements[ 2 ].c_str( ) );
									break;

								default:
									break;
								}

								vcount++;
							}
							else
							{
								std::cerr << "'facet' does not have 'vertex'" << std::endl;
							}
						}
						else
						{
							std::cerr << "'facet' does not have 'vertex'" << std::endl;
							return( false );
						}
					}
					else
					{
						// error
						std::cerr << "Line does not start with 'vertex'" << std::endl;
						return( false );
					}
					break;

				default:
					std::cerr << "Unknown error occured" << std::endl;
					break;
				}

				if( !eol )
				{
					// 行末までポインタを進める
					p = get_line( p, e, line );
				}
			}

			return( false );
		}

		static bool convert_from_stl_binary_data( facet_list_type &facets, const unsigned char *buff, size_type len )
		{
			const unsigned char *p = buff;
			const unsigned char *e = buff + len;

			if( len < 80 + 4 )
			{
				return( false );
			}

			size_type num_facets = ( ( unsigned int * )( buff + 80 ) )[ 0 ];

			if( len < 80 + 4 + num_facets * _facet_::bytes )
			{
				return( false );
			}

			facets.name = std::string( ( std::string::value_type * )buff, 80 );

			for( size_type i = 0 ; i < num_facets ; i++ )
			{
				_facet_ *F = ( _facet_ * )( buff + 80 + 4 + _facet_::bytes * i );
				facet_type f( vector_type( F->nX, F->nY, F->nZ ), vector_type( F->p1X, F->p1Y, F->p1Z ), vector_type( F->p2X, F->p2Y, F->p2Z ), vector_type( F->p3X, F->p3Y, F->p3Z ) );

				if( f.normal.length( ) == 0 )
				{
					f.normal = ( ( f.p2 - f.p1 ).outer( f.p3 - f.p1 ) ).unit( );
				}
				else if( f.normal.inner( ( f.p2 - f.p1 ).outer( f.p3 - f.p1 ) ) < 0 )
				{
					vector_type tmp = f.p2;
					f.p2 = f.p3;
					f.p3 = tmp;
				}

				facets.push_back( f );
			}

			return( true );
		}

		static bool read( facet_list_type &facets, const std::string &filename )
		{
			// データをクリアする
			facets.clear( );

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

			bool ret = false;

			if( filesize > 5 )
			{
				std::string sss = to_lower( std::string( ( char * )buff, 5 ) );
				if( sss == "solid" )
				{
					convert_from_stl_ascii_data( facets, buff, filesize );
				}
				else
				{
					convert_from_stl_binary_data( facets, buff, filesize );
				}
			}

			delete [] buff;

			return( ret );
		}

		static bool write( const facet_list_type &facets, const std::string &filename, bool use_ascii_format )
		{
			if( facets.empty( ) )
			{
				std::cerr << "There is no facet!" << std::endl;
				return( false );
			}

			if( use_ascii_format )
			{
				FILE *fp;
				if( ( fp = fopen( filename.c_str( ), "wt" ) ) == NULL )
				{
					return( false );
				}

				fprintf( fp, "solid %s\n", facets.name.c_str( ) );

				for( size_type i = 0 ; i < facets.size( ) ; i++ )
				{
					const facet_type &f = facets[ i ];
					fprintf( fp, "\tfacet normal %f %f %f\n", f.normal.x, f.normal.y, f.normal.z );
					fprintf( fp, "\t\touter loop\n" );

					if( f.normal.inner( ( f.p2 - f.p1 ).outer( f.p3 - f.p1 ) ) >= 0 )
					{
						fprintf( fp, "\t\t\tvertex %f %f %f\n", f.p1.x, f.p1.y, f.p1.z );
						fprintf( fp, "\t\t\tvertex %f %f %f\n", f.p2.x, f.p2.y, f.p2.z );
						fprintf( fp, "\t\t\tvertex %f %f %f\n", f.p3.x, f.p3.y, f.p3.z );
					}
					else
					{
						fprintf( fp, "\t\t\tvertex %f %f %f\n", f.p1.x, f.p1.y, f.p1.z );
						fprintf( fp, "\t\t\tvertex %f %f %f\n", f.p3.x, f.p3.y, f.p3.z );
						fprintf( fp, "\t\t\tvertex %f %f %f\n", f.p2.x, f.p2.y, f.p2.z );
					}

					fprintf( fp, "\t\tendloop\n" );
					fprintf( fp, "\tendfacet\n" );
				}

				fprintf( fp, "endsolid %s\n", facets.name.c_str( ) );

				fclose( fp );
			}
			else
			{
				FILE *fp;
				if( ( fp = fopen( filename.c_str( ), "wb" ) ) == NULL )
				{
					return( false );
				}

				unsigned int num_facets = static_cast< unsigned int >( facets.size( ) );
				unsigned char dmy[ 80 ];
				memset( dmy, 0, sizeof( unsigned char ) * 80 );

				{
					std::string tmp = std::string( "Binary STL output by MIST: " ) + facets.name;
					if( tmp.size( ) < 80 )
					{
						memcpy( dmy, tmp.c_str( ), tmp.size( ) );
					}
					else
					{
						memcpy( dmy, tmp.c_str( ), 80 );
					}
				}

				// ヘッダ情報を書き込む
				fwrite( dmy, sizeof( unsigned char ), 80, fp );
				fwrite( &num_facets, sizeof( unsigned int ), 1, fp );

				// ポリゴンデータを書き込む
				for( size_type i = 0 ; i < facets.size( ) ; i++ )
				{
					const facet_type &f = facets[ i ];
					_facet_ F;
					F.nX = ( float )f.normal.x;
					F.nY = ( float )f.normal.y;
					F.nZ = ( float )f.normal.z;
					F.p1X = ( float )f.p1.x;
					F.p1Y = ( float )f.p1.y;
					F.p1Z = ( float )f.p1.z;
					F.p2X = ( float )f.p2.x;
					F.p2Y = ( float )f.p2.y;
					F.p2Z = ( float )f.p2.z;
					F.p3X = ( float )f.p3.x;
					F.p3Y = ( float )f.p3.y;
					F.p3Z = ( float )f.p3.z;
					F.dmy = 0x0000;

					fwrite( &F, 1, _facet_::bytes, fp );
				}

				fclose( fp );
			}

			return( true );
		}
	};
}


//! @addtogroup cad_group CADデータ入出力
//!  @{

//! @addtogroup cad_stl_group STL形式のポリゴンデータの入出力
//!
//! STL（ASCII，バイナリ）形式のファイルからポリゴンデータを読み込む．
//! また，ポリゴンデータからSTL（ASCII，バイナリ）形式のファイルへの書き出しをサポート．
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/io/stl.h>
//! @endcode
//!
//!
//!  @{


/// @brief STL（ASCII，バイナリ）形式のファイルからポリゴンデータを読み込む
//! 
//! @param[out] facets   … ポリゴンのリスト
//! @param[in] filename … 出力ファイル名
//!
//! @retval true  … ポリゴンデータの読み込みに成功
//! @retval false … ポリゴンデータの読み込みに失敗
//! 
template < class T >
bool read_stl( facet_list< T > &facets, const std::wstring &filename )
{
	return( read_stl( facets, wstr2str( filename ) ) );
}


/// @brief STL（ASCII，バイナリ）形式のファイルからポリゴンデータを読み込む
//! 
//! @param[out] facets   … ポリゴンのリスト
//! @param[in] filename … 出力ファイル名
//!
//! @retval true  … ポリゴンデータの読み込みに成功
//! @retval false … ポリゴンデータの読み込みに失敗
//! 
template < class T >
bool read_stl( facet_list< T > &facets, const std::string &filename )
{
	return( __stl_controller__::stl_controller< T >::read( facets, filename ) );
}


/// @brief ポリゴンデータをSTL（ASCII，バイナリ）形式で出力する
//!
//! @param[in] facets           … ポリゴンのリスト
//! @param[in] filename         … 出力ファイル名
//! @param[in] use_ascii_format … ASCII形式で出力するかどうか
//!
//! @retval true  … ファイルへの書き込みに成功
//! @retval false … ファイルへの書き込みに失敗
//! 
template < class T >
inline bool write_stl( const facet_list< T > &facets, const std::wstring &filename, bool use_ascii_format = true )
{
	return( write_stl( facets, wstr2str( filename ), use_ascii_format ) );
}




/// @brief ポリゴンデータをSTL（ASCII，バイナリ）形式で出力する
//!
//! @param[in] facets           … ポリゴンのリスト
//! @param[in] filename         … 出力ファイル名
//! @param[in] use_ascii_format … ASCII形式で出力するかどうか
//!
//! @retval true  … ファイルへの書き込みに成功
//! @retval false … ファイルへの書き込みに失敗
//! 
template < class T >
inline bool write_stl( const facet_list< T > &facets, const std::string &filename, bool use_ascii_format = true )
{
	return( __stl_controller__::stl_controller< T >::write( facets, filename, use_ascii_format ) );
}



/// @}
//  STL形式のポリゴンデータの入出力グループの終わり

/// @}
//  CADデータ入出力グループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_STL__
