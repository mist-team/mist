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

/// @file mist/io/dicom.h
//!
//! @brief DICOM画像を読み書きするためのライブラリ
//!
#ifndef __INCLUDE_MIST_DICOM__
#define __INCLUDE_MIST_DICOM__


#ifndef __INCLUDE_MIST_CONF_H__
#include "../config/mist_conf.h"
#endif

#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#ifndef __INCLUDE_MIST_ENDIAN__
#include "../config/endian.h"
#endif

#ifndef __INCLUDE_MIST_LIMITS__
#include "../limits.h"
#endif

// カラー画像の設定を読み込む
#ifndef __INCLUDE_MIST_COLOR_H__
#include "../config/color.h"
#endif

#ifndef __INCLUDE_MIST_DICOM_TAG__
#include "./dicom_tag.h"
#endif

#ifndef __INCLUDE_MIST_DICOM_INFO__
#include "./dicom_info.h"
#endif

#ifndef __INCLUDE_MIST_SINGLETON__
#include "../singleton.h"
#endif

// mist名前空間の始まり
_MIST_BEGIN


//! @addtogroup image_group 画像入出力
//!  @{

//! @addtogroup dicom_group DICOM画像入出力
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/io/dicom.h>
//! @endcode
//!
//!  @{


// 次のマクロを定義すると，コンソール上に読み込んだタグ情報を表示する
// #define __SHOW_DICOM_TAG__
// #define __SHOW_DICOM_UNKNOWN_TAG__
// #define __SHOW_DICOM_ZEROBYTE_TAG__
// #define __CHECK_TAG_FORMAT__

// 次のマクロを定義すると，認識不能なタグは排除する
// #define __EXCLUDE_DICOM_UNKNOWN_TAG__
// #define __EXCLUDE_DICOM_ZEROBYTE_TAG__


/// DICOMファイルを操作する関数・クラスを含む名前空間
namespace dicom
{
	/// @brief 指定したメモリ領域がDICOM画像を現しているかどうかをチェックする
	//! 
	//! @attention メモリ領域は 132 バイト以上なくてはならない
	//! 
	//! @param[in] p … 先頭メモリ位置
	//! @param[in] e … 末尾メモリ位置
	//! 
	//! @return DICOMのプリアンブルの次を指すポインタ
	//! 
	inline unsigned char *check_dicom_file( unsigned char *p, unsigned char *e )
	{
		if( p == NULL || p + 4 >= e )
		{
			return( NULL );
		}

		char *dicm = reinterpret_cast< char * >( p ) + 128;
		if( dicm[ 0 ] == 'D' && dicm[ 1 ] == 'I' && dicm[ 2 ] == 'C' && dicm[ 3 ] == 'M' )
		{
			if( p + 4 + 128 >= e )
			{
				return( NULL );
			}
			else
			{
				return( p + 128 + 4 );
			}
		}

		// DICOM用のプリアンブルが存在しないため，先頭のポインタを返す
		return( p );
	}


	/// @brief シーケンスタグかどうかを判定する
	//! 
	//! @param[in] p … 先頭ポインタ
	//! @param[in] e … 末尾ポインタ
	//! 
	//! @retval true  … シーケンスタグの場合
	//! @retval false … それ以外
	//! 
	inline bool is_sequence_separate_tag( const unsigned char *p, const unsigned char *e )
	{
		if( p + 4 > e )
		{
			return( false );
		}
		return( p[ 0 ] == 0xfe && p[ 1 ] == 0xff && p[ 2 ] == 0x00 && p[ 3 ] == 0xe0 );
	}

	/// @brief シーケンス要素終了タグかどうか
	//! 
	//! @param[in] p … 先頭ポインタ
	//! @param[in] e … 末尾ポインタ
	//! 
	//! @retval true  … シーケンス要素終了タグの場合
	//! @retval false … それ以外
	//! 
	inline bool is_sequence_element_end( const unsigned char *p, const unsigned char *e )
	{
		if( p + 8 > e )
		{
			return( false );
		}
		return( p[ 0 ] == 0xfe && p[ 1 ] == 0xff && p[ 2 ] == 0x0d && p[ 3 ] == 0xe0 && p[ 4 ] == 0x00 && p[ 5 ] == 0x00 && p[ 6 ] == 0x00 && p[ 7 ] == 0x00 );
	}

	/// @brief シーケンス終了タグかどうか
	//! 
	//! @param[in] p … 先頭ポインタ
	//! @param[in] e … 末尾ポインタ
	//! 
	//! @retval true  … シーケンス終了タグの場合
	//! @retval false … それ以外
	//! 
	inline bool is_sequence_tag_end( const unsigned char *p, const unsigned char *e )
	{
		if( p + 8 > e )
		{
			return( false );
		}
		return( p[ 0 ] == 0xfe && p[ 1 ] == 0xff && p[ 2 ] == 0xdd && p[ 3 ] == 0xe0 && p[ 4 ] == 0x00 && p[ 5 ] == 0x00 && p[ 6 ] == 0x00 && p[ 7 ] == 0x00 );
	}

	template < bool _IS_SIGNED_ >
	struct __check_num_bytes_function__
	{
		template < class T >
		static ptrdiff_t check( const T &nbytes ) 
		{
			if( nbytes == type_limits< T >::maximum( ) )
			{
				return( -1 );
			}
			else
			{
				return( nbytes );
			}
		}
	};

	template < >
	struct __check_num_bytes_function__< true >
	{
		template < class T >
		static ptrdiff_t check( const T &nbytes ) 
		{
			return( nbytes );
		}
	};

	template < class T >
	inline ptrdiff_t __check_num_bytes__( const T &nbytes )
	{
		return( __check_num_bytes_function__< std::numeric_limits< T >::is_signed >::check( nbytes ) );
	}

	/// @brief DICOMのタグを読み込み，テーブルに登録されているものと照合する
	//! 
	//! テーブルに登録されていない場合は，読み飛ばす．
	//! もし，適切なDICOMファイルでない場合は-2を返し，データの終端もしくはファイルの読み込みに失敗した場合は-1を返す．
	//! そして，読み飛ばした場合は0を返し，テーブルに登録されている場合は次に存在するデータのバイト数を返す．
	//! データを処理する先頭のポインタ 'p' と，データの最後＋1をさすポインタ 'e' を与える
	//! 
	//! @param[in]  p        … 先頭メモリ位置
	//! @param[in]  e        … 末尾メモリ位置
	//! @param[out] tag      … 取得されたDICOMのタグ
	//! @param[out] numBytes … DICOMタグが使用しているメモリのバイト数
	//! @param[in]  from_little_endian … 入力データがリトルエンディアンかどうか
	//! 
	//! @return タグが示すデータ内容を指す先頭ポインタ
	//! 
	inline unsigned char *read_dicom_tag( unsigned char *p, unsigned char *e, dicom_tag &tag, difference_type &numBytes, bool from_little_endian = true )
	{
		if( p == NULL || p + 8 > e )
		{
			numBytes = -1;
			return( e );
		}

		unsigned char *data = p;

		unsigned short group   = to_current_endian( byte_array< unsigned short >( data ), from_little_endian ).get_value( );
		unsigned short element = to_current_endian( byte_array< unsigned short >( data + 2 ), from_little_endian ).get_value( );

		data += 4;

		char VR[ 3 ] = { static_cast< char >( data[ 0 ] ), static_cast< char >( data[ 1 ] ), '\0' };
		dicom_vr vr = get_dicom_vr( VR );

		difference_type num_bytes = 0;
		const dicom_tag_table &dicom_table = singleton< dicom_tag_table >::get_instance( );
		tag = dicom_table.get_tag( group, element, vr );

		if( tag.tag != 0xffffffff )
		{
			//タグがテーブルに登録されている場合
			if( tag.vr == vr )
			{
				// 明示的VR
				switch( vr )
				{
				case OB:
				case OW:
				case UN:
				case SQ:
				case OF:
				case UT:
					data += 4;
					num_bytes = __check_num_bytes__( to_current_endian( byte_array< unsigned int >( data ), from_little_endian ).get_value( ) );
					break;

				default:
					num_bytes = __check_num_bytes__( to_current_endian( byte_array< unsigned short >( data + 2 ), from_little_endian ).get_value( ) );
					break;
				}
			}
			else if( vr != UNKNOWN )
			{
				// 明示的VRであるが，辞書と異なる場合
				tag.vr = vr;
				switch( vr )
				{
				case OB:
				case OW:
				case UN:
				case SQ:
				case OF:
				case UT:
					data += 4;
					num_bytes = __check_num_bytes__( to_current_endian( byte_array< unsigned int >( data ), from_little_endian ).get_value( ) );
					break;

				default:
					num_bytes = __check_num_bytes__( to_current_endian( byte_array< unsigned short >( data + 2 ), from_little_endian ).get_value( ) );
				}
			}
			else
			{
				// 暗示的VR
				num_bytes = __check_num_bytes__( to_current_endian( byte_array< unsigned int >( data ), from_little_endian ).get_value( ) );
			}

			numBytes = num_bytes;
			return( reinterpret_cast< unsigned char * >( data + 4 ) );
		}
		else
		{
			//タグがテーブルに登録されていない場合はデータスキップ
			switch( vr )
			{
			case AE:
			case AS:
			case AT:
			case CS:
			case DA:
			case DS:
			case DT:
			case FL:
			case FD:
			case IS:
			case LO:
			case LT:
			case PN:
			case SH:
			case SL:
			case SS:
			case ST:
			case TM:
			case UI:
			case UL:
			case US:
				num_bytes = __check_num_bytes__( to_current_endian( byte_array< unsigned short >( data + 2 ), from_little_endian ).get_value( ) );
				break;

			case OB:
			case OW:
			case UN:
			case SQ:
			case OF:
			case UT:
				data += 4;
				num_bytes = __check_num_bytes__( to_current_endian( byte_array< unsigned int >( data ), from_little_endian ).get_value( ) );
				break;

			default:
				num_bytes = __check_num_bytes__( to_current_endian( byte_array< unsigned int >( data ), from_little_endian ).get_value( ) );
				break;
			}

#if defined( __SHOW_DICOM_ZEROBYTE_TAG__ ) && defined( __SHOW_DICOM_UNKNOWN_TAG__ )
			printf( "( %04x, %04x, %s, % 8d ) = Unknown Tags!!\n", group, element, VR, num_bytes );
#endif
			if( vr == SQ )
			{
				// 不明なタグだけどシーケンスタグということを確認
				tag = dicom_tag( construct_dicom_tag( group, element ), SQ, -1, "UNKNOWN" );
				numBytes = num_bytes;
				return( data + 4 );
			}
			else if( data + 4 + 4 <= e && data[ 0 ] == 0xff && data[ 1 ] == 0xff && data[ 2 ] == 0xff && data[ 3 ] == 0xff && is_sequence_separate_tag( data + 4, e ) )
			{
				// 不明なタグだけどシーケンスタグの可能性がある場合ということを確認
				tag = dicom_tag( construct_dicom_tag( group, element ), SQ, -1, "UNKNOWN" );
				numBytes = num_bytes;
				return( data + 4 );
			}
			else if( data + 4 + num_bytes <= e )
			{
				if( element == 0x0000 )
				{
					// Group Length タグ
					char str[ 50 ];
					sprintf( str, "Group %04d Length", group );
					tag = dicom_tag( construct_dicom_tag( group, element ), UL, 1, str );
					numBytes = num_bytes;
					return( reinterpret_cast< unsigned char * >( data + 4 ) );
				}
				else
				{
					tag = dicom_tag( construct_dicom_tag( group, element ), vr, -1, "UNKNOWN" );
					numBytes = num_bytes;
					return( reinterpret_cast< unsigned char * >( data + 4 ) );
				}
			}
			else
			{
				numBytes = -2;
				return( p );
			}
		}
	}



	/// @brief DICOMのタグに対し，各VRごとの処理を行う
	//! 
	//! @param[in]     tag       … 処理するDICOMタグ
	//! @param[in,out] byte      … 処理されるバイト列
	//! @param[in]     num_bytes … バイト列の長さ
	//! @param[in]     from_little_endian … 入力データがリトルエンディアンかどうか
	//! 
	//! @return 登録されていないタグの場合は false をかえし，正しく処理された場合のみ true を返す
	//! 
	inline bool process_dicom_tag( const dicom_tag &tag, unsigned char *byte, difference_type num_bytes, bool from_little_endian = true )
	{
		switch( tag.vr )
		{
		case AE:
			// Application Entity
			// 「16バイト以下」
			// 先頭と末尾にスペース（20H）を持つ可能性のある文字列
			// 文字列の長さを偶数に保つために，末尾にスペースを挿入する必要あり
			// スペースは特に意味なし
			if( tag.vm != -1 && num_bytes > 16 * tag.vm )
			{
				return( false );
			}
			break;

		case AS:
			// 年齢を表す文字列
			// 「4バイト固定」
			// 文字列の書式は nnnD, nnnW, nnnM, nnnY のいずれか
			// 使用可能な値は，0-9, D, W, M, Y のいずれか
			// 　D: nnn → 日
			// 　W: nnn → 週
			// 　M: nnn → 月
			// 　Y: nnn → 年
			//
			// 例： "018M" は生後18ヶ月を表す
			if( tag.vm != -1 && ( num_bytes % 4 != 0 || (int)( num_bytes / 4 ) > tag.vm ) )
			{
				return( false );
			}
			break;

		case AT:
			// Attribute Tag
			// 「4バイト固定」
			// 2つの16ビット符号なし整数の並び
			//
			// 例： (0018,00FF) のタグは，4バイトの並びで表現され，エンディアンの違いにより以下のように符号化される
			// リトルエンディアン転送構文 → 18H, 00H, FFH, 00H
			// ビッグエンディアン転送構文 → 00H, 18H, 00H, FFH
			if( tag.vm != -1 && ( num_bytes % 4 != 0 || (int)( num_bytes / 4 ) > tag.vm ) )
			{
				return( false );
			}

			if( _is_little_endian_( ) != from_little_endian )
			{
				for( long i = 0 ; i < num_bytes ; i += 4 )
				{
					unsigned char v0 = byte[ i + 0 ];
					unsigned char v1 = byte[ i + 1 ];
					unsigned char v2 = byte[ i + 2 ];
					unsigned char v3 = byte[ i + 3 ];
					byte[ i + 0 ] = v1;
					byte[ i + 1 ] = v0;
					byte[ i + 2 ] = v3;
					byte[ i + 3 ] = v2;
				}
			}
			break;

		case CS:
			// Code String
			// 「16バイト以下」
			// 先頭と末尾にスペース（20H）を持つ可能性のある文字列
			// 文字列の長さを偶数に保つために，末尾にスペースを挿入する必要あり
			// スペースは特に意味なし
			// 大文字の A-Z, 0-9, アンダーバー（_）のみを使用可能．
			if( tag.vm != -1 && num_bytes > 16 * tag.vm )
			{
				return( false );
			}
			break;

		case DA:
			// 日付を表す文字列
			// 「8バイト固定」ただし，DICOM2.0では「10バイト固定」のためどちらも扱えるようにする必要あり
			// yyyymmdd もしくは yyyy.mm.dd の書式で日付を符号化した文字列
			// これは，yyyy 年 mm 月 dd 日を表す
			// 0-9, ピリオド（.）のみを使用可能．
			//
			// 例：19930822 は 1993 年 8 月 22 日を表す
			if( tag.vm != -1 && ( num_bytes % 8 != 0 || (int)( num_bytes / 8 ) > tag.vm ) && ( num_bytes % 10 != 0 || (int)( num_bytes / 10 ) > tag.vm ) )
			{
				return( false );
			}
			break;

		case DS:
			// 10進数を表す文字列
			// 「16バイト以下」
			// 固定小数点もしくは浮動小数点数を表現する文字列であり，先頭と末尾にスペース（20H）を持つ可能性のある文字列
			// 固定小数点数の場合は 0-9 に加え，先頭に + もしくは -，さらに小数点を示す任意の . を含む
			// 浮動小数点数は，ANSI X3.9 に従い，指数を示す E もしくは e を含む
			// 文字列の長さを偶数に保つために，末尾にスペースを挿入する必要あり
			if( tag.vm != -1 && num_bytes > 16 * tag.vm )
			{
				return( false );
			}
			break;

		case DT:
			// 日時を表す文字列
			// 「26バイト以下」
			// YYYYMMDDHHMMSS.FFFFFF&ZZZZ の書式に従って日時を表し，先頭と末尾にスペース（20H）を持つ可能性のある文字列
			// これは，YYYY 年 MM 月 DD 日 HH 時 MM 分 SS.FFFFFF 秒（FFFFFFは病の小数部分）を表す
			// また，& は + もしくは - のどちらかであり，ZZZZ は時間と分のオフセットを表す
			// 文字列の長さを偶数に保つために，末尾にスペースを挿入する必要あり
			if( tag.vm != -1 && num_bytes > 26 * tag.vm )
			{
				return( false );
			}
			break;

		case FL:
			// 単精度浮動小数
			// 「4バイト固定」
			if( tag.vm != -1 && ( num_bytes % 4 != 0 || (int)( num_bytes / 4 ) > tag.vm ) )
			{
				return( false );
			}

			{
				byte_array< float > tmp = to_current_endian( byte_array< float >( byte ), from_little_endian );
				byte[ 0 ] = tmp[ 0 ];
				byte[ 1 ] = tmp[ 1 ];
				byte[ 2 ] = tmp[ 2 ];
				byte[ 3 ] = tmp[ 3 ];
			}
			break;

		case FD:
			// 倍精度浮動小数
			// 「8バイト固定」
			if( tag.vm != -1 && ( num_bytes % 8 != 0 || (int)( num_bytes / 8 ) > tag.vm ) )
			{
				return( false );
			}

			{
				byte_array< double > tmp = to_current_endian( byte_array< double >( byte ), from_little_endian );
				byte[ 0 ] = tmp[ 0 ];
				byte[ 1 ] = tmp[ 1 ];
				byte[ 2 ] = tmp[ 2 ];
				byte[ 3 ] = tmp[ 3 ];
				byte[ 4 ] = tmp[ 4 ];
				byte[ 5 ] = tmp[ 5 ];
				byte[ 6 ] = tmp[ 6 ];
				byte[ 7 ] = tmp[ 7 ];
			}
			break;

		case IS:
			// 整数を表す文字列
			// 「16バイト以下」
			// 10 を底とする整数（10進数）を表わす文字列で，先頭に + もしくは - を含んでも良く，先頭と末尾にスペース（20H）を持つ可能性のある文字列
			// 文字列の長さを偶数に保つために，末尾にスペースを挿入する必要あり
			// -2^31 ≦ n ≦ (2^31-1) の範囲を表現することが可能．
			if( tag.vm != -1 && num_bytes > 16 * tag.vm )
			{
				return( false );
			}
			break;

		case LO:
			// 長文字列
			// 「64バイト以下」
			// 先頭と末尾にスペース（20H）を持つ文字列
			// 文字列の長さを偶数に保つために，末尾にスペースを挿入する必要あり
			// 制御文字列は含まない
			if( tag.vm != -1 && num_bytes > 64 * tag.vm )
			{
				return( false );
			}
			break;

		case LT:
			// 長テキスト文字列
			// 「10240バイト以下」
			// 末尾にスペース（20H）を持つ可能性のある文字列
			// 文字列の長さを偶数に保つために，末尾にスペースを挿入する必要あり
			// 制御文字コードを含む
			if( tag.vm != -1 && num_bytes > 10240 * tag.vm )
			{
				return( false );
			}
			break;

		case OB:
			// バイト列
			// 「無制限」
			// 転送構文のエンディアン形式に依存しないデータ列
			// バイト列の長さを偶数に保つために，末尾にNULL値（00H）で埋められる可能性あり
			break;

		case OF:
			// 4バイト浮動小数のバイト列の並びワード列
			// 「無制限」
			// 転送構文のエンディアン形式に依存して，バイトの並びが変化するデータ列
			if( _is_little_endian_( ) != from_little_endian )
			{
				for( long i = 0 ; i < num_bytes ; i += 4 )
				{
					unsigned char v1 = byte[ i + 0 ];
					unsigned char v2 = byte[ i + 1 ];
					unsigned char v3 = byte[ i + 2 ];
					unsigned char v4 = byte[ i + 3 ];
					byte[ i + 0 ] = v4;
					byte[ i + 1 ] = v3;
					byte[ i + 2 ] = v2;
					byte[ i + 3 ] = v1;
				}
			}
			break;

		case OW:
			// ワード列
			// 「無制限」
			// 転送構文のエンディアン形式に依存して，バイトの並びが変化するデータ列
			if( _is_little_endian_( ) != from_little_endian )
			{
				for( long i = 0 ; i < num_bytes ; i += 2 )
				{
					unsigned char tmp = byte[ i ];
					byte[ i ] = byte[ i + 1 ];
					byte[ i + 1 ] = tmp;
				}
			}
			break;

		case PN:
			// 患者名を表す文字列
			// 「64バイト以下」
			// 制御文字列を一切含まず，末尾にスペース（20H）を持つ可能性のある文字列
			// 文字列の長さを偶数に保つために，末尾にスペースを挿入する必要あり
			// 姓と名の区切りには，^ (5EH) が用いられる
			if( tag.vm != -1 && num_bytes > 64 * tag.vm )
			{
				return( false );
			}
			break;

		case SH:
			// 短文字列
			// 「16バイト以下」
			// 先頭と末尾にスペース（20H）を持つ文字列
			// 文字列の長さを偶数に保つために，末尾にスペースを挿入する必要あり
			// 制御文字列は含まない
			if( tag.vm != -1 && num_bytes > 16 * tag.vm )
			{
				return( false );
			}
			break;

		case SL:
			// 符号付き4バイト整数
			// 「4バイト固定」
			if( tag.vm != -1 && ( num_bytes % 4 != 0 || (int)( num_bytes / 4 ) > tag.vm ) )
			{
				return( false );
			}

			{
				byte_array< signed int > tmp = to_current_endian( byte_array< signed int >( byte ), from_little_endian );
				byte[ 0 ] = tmp[ 0 ];
				byte[ 1 ] = tmp[ 1 ];
				byte[ 2 ] = tmp[ 2 ];
				byte[ 3 ] = tmp[ 3 ];
			}
			break;

		case SQ:
			// シーケンスを表すタグ
			// 「無制限」
			// DICOMタグのシーケンスを格納するタグ
			break;

		case SS:
			// 符号付き2バイト整数
			// 「2バイト固定」
			if( tag.vm != -1 && ( num_bytes % 2 != 0 || (int)( num_bytes / 2 ) > tag.vm ) )
			{
				return( false );
			}

			{
				byte_array< signed short > tmp = to_current_endian( byte_array< signed short >( byte ), from_little_endian );
				byte[ 0 ] = tmp[ 0 ];
				byte[ 1 ] = tmp[ 1 ];
			}
			break;

		case ST:
			// 短テキスト文字列
			// 末尾にスペース（20H）を持つ可能性のある文字列
			// 文字列の長さを偶数に保つために，末尾にスペースを挿入する必要あり
			// 制御文字コードを含む
			if( tag.vm != -1 && num_bytes > 1024 * tag.vm )
			{
				return( false );
			}
			break;

		case TM:
			// 時刻を表す文字列
			// 「16バイト以下」
			// hhmmss.frac の書式で時刻を符号化した文字列
			// これは，hh 時 mm 分 ss 秒 (frac は1億分の1秒単位) を表す
			// 24時間形式の時刻表記が用いられ，深夜24時は0時として扱われる
			// 文字列の長さを偶数に保つために，末尾にスペースを挿入する必要あり
			if( tag.vm != -1 && num_bytes > 16 * tag.vm )
			{
				return( false );
			}
			break;

		case UI:
			// UIDを表す文字列
			// 「64バイト以下」
			// 文字列の長さを偶数に保つために，末尾にNULL (00H) を挿入する必要あり
			if( tag.vm != -1 && num_bytes > 64 * tag.vm )
			{
				return( false );
			}
			break;

		case UL:
			// 符号無し4バイト整数
			// 「4バイト固定」
			if( tag.vm != -1 && ( num_bytes % 4 != 0 || (int)( num_bytes / 4 ) > tag.vm ) )
			{
				return( false );
			}

			{
				byte_array< unsigned int > tmp = to_current_endian( byte_array< unsigned int >( byte ), from_little_endian );
				byte[ 0 ] = tmp[ 0 ];
				byte[ 1 ] = tmp[ 1 ];
				byte[ 2 ] = tmp[ 2 ];
				byte[ 3 ] = tmp[ 3 ];
			}
			break;

		case UN:
			// 不明なタグ
			break;

		case US:
			// 符号無し2バイト整数
			// 「2バイト固定」
			if( tag.vm != -1 && ( num_bytes % 2 != 0 || (int)( num_bytes / 2 ) > tag.vm ) )
			{
				return( false );
			}

			{
				byte_array< unsigned short > tmp = to_current_endian( byte_array< unsigned short >( byte ), from_little_endian );
				byte[ 0 ] = tmp[ 0 ];
				byte[ 1 ] = tmp[ 1 ];
			}
			break;

		case UT:
			// 無制限のテキスト文字列
			// 末尾にスペース（20H）を持つ可能性のある文字列
			// 文字列の長さを偶数に保つために，末尾にスペースを挿入する必要あり
			// 制御文字コードを含む
			break;

		default:
			// VRがわかんないからとりあえず良しとする
			break;
		}

		return( true );
	}


	/// @brief DICOMのタグを処理する
	//! 
	//! シーケンスを表すDICOMタグが見つかった場合は，再帰的に本関数が適用される
	//! 
	//! @param[out] dicm        … DICOMデータの出力先
	//! @param[in]  pointer     … 先頭ポインタ
	//! @param[in]  end_pointer … 末尾先頭ポインタ
	//! @param[in]  from_little_endian … 入力データがリトルエンディアンかどうか
	//! @param[in]  is_in_sequence_tag … VRがSQ内のタグであるかどうか（SQタグは一つにまとめる）
	//! 
	//! @return 次のタグを指すポインタ
	//! 
	inline unsigned char *process_dicom_tag( dicom_tag_container &dicm, unsigned char *pointer, unsigned char *end_pointer, bool from_little_endian = true, bool is_in_sequence_tag = false )
	{
		difference_type numBytes = 0;
		dicom_tag tag;

		pointer = read_dicom_tag( pointer, end_pointer, tag, numBytes, from_little_endian );

		if( tag.vr == SQ )
		{
			unsigned char *sp = pointer;
			if( numBytes != -1 && pointer + numBytes <= end_pointer )
			{
				pointer += numBytes;
			}
			else
			{
				unsigned char *ep = numBytes == -1 ? end_pointer : pointer + numBytes;
				if( ep > end_pointer )
				{
					// 認識不能なシーケンスタグ発見
					return( NULL );
				}
				while( pointer + 8 <= ep )
				{
					if( is_sequence_tag_end( pointer, end_pointer ) )
					{
						pointer += 8;
						break;
					}
					else
					{
						if( !is_sequence_separate_tag( pointer, ep ) )
						{
							return( NULL );
						}

						pointer += 4;
						numBytes = __check_num_bytes__( to_current_endian( byte_array< unsigned int >( pointer ), from_little_endian ).get_value( ) );
						pointer += 4;

						unsigned char *epp = numBytes == -1 ? ep : pointer + numBytes;
						if( epp > ep )
						{
							return( NULL );
						}

						while( pointer + 8 <= epp )
						{
							if( is_sequence_element_end( pointer, ep ) )
							{
								pointer += 8;
								break;
							}
							else
							{
								pointer = process_dicom_tag( dicm, pointer, ep, from_little_endian, true );
								if( pointer == NULL )
								{
									return( NULL );
								}
							}
						}
					}
				}
			}

			if( !is_in_sequence_tag )
			{
				dicom_tag_container::iterator ite;
				ite = dicm.append( dicom_element( tag, sp, pointer - sp ) );

#ifdef __SHOW_DICOM_TAG__
				if( ite != dicm.end( ) )
				{
					ite->second.show_tag( );
				}
#endif
			}
		}
		else if( numBytes < -1 )
		{
			return( NULL );
		}
		else if( numBytes == -1 )
		{
			// 圧縮がかかっている可能性があるのでチェック
			dicom_tag_container::iterator ite;
			switch( tag.vr )
			{
			case OB:
			case OW:
				{
					unsigned char *p = pointer;
					while( p + 8 <= end_pointer )
					{
						if( is_sequence_tag_end( p, end_pointer ) )
						{
							p += 8;
							break;
						}
						else
						{
							if( !is_sequence_separate_tag( p, end_pointer ) )
							{
								return( NULL );
							}

							p += 4;
							numBytes = __check_num_bytes__( to_current_endian( byte_array< unsigned int >( p ), from_little_endian ).get_value( ) );
							p += 4 + numBytes;

							if( numBytes < 0 || p > end_pointer )
							{
								return( NULL );
							}
						}
					}
					if( p <= end_pointer )
					{
						numBytes = p - pointer;
						if( !is_in_sequence_tag )
						{
							ite = dicm.append( dicom_element( tag, pointer, numBytes ) );
						}
						pointer += 8;
					}
					else
					{
						// 圧縮シーケンスの終わりをうまく検出できなかった
						return( NULL );
					}
				}
				break;

			default:
				return( NULL );
			}

			pointer += numBytes;

#ifdef __SHOW_DICOM_TAG__
			if( !is_in_sequence_tag && ite != dicm.end( ) )
			{
				ite->second.show_tag( );
			}
#endif
		}
		else if( numBytes > 0 )
		{
			// 不適切なバイト数かどうかをチェックする
			if( pointer + numBytes > end_pointer )
			{
				return( NULL );
			}

#ifndef __CHECK_TAG_FORMAT__
			if( !process_dicom_tag( tag, pointer, numBytes, from_little_endian ) )
			{
				// 規格に従っていないタグを発見したが，とりあえず無視しておく
				std::cout << "Illegal DICOM tag is found!" << std::endl;
			}
#else
			if( !process_dicom_tag( tag, pointer, numBytes, from_little_endian ) )
			{
				// 処理することができないDICOMタグを発見したので終了する
				return( NULL );
			}
#endif
			if( !is_in_sequence_tag )
			{
				dicom_tag_container::iterator ite;
				ite = dicm.append( dicom_element( tag, pointer, numBytes ) );

#ifdef __SHOW_DICOM_TAG__
				if( ite != dicm.end( ) )
				{
					ite->second.show_tag( );
				}
#endif
			}

			pointer += numBytes;
		}
		else if( tag.vr != UNKNOWN && !is_in_sequence_tag )
		{
			// 不明なタグもリストに追加する
			dicom_tag_container::iterator ite;
			ite = dicm.append( dicom_element( tag, NULL, 0 ) );

#if defined( __SHOW_DICOM_ZEROBYTE_TAG__ ) && defined( __SHOW_DICOM_TAG__ )
			if( ite != dicm.end( ) )
			{
				ite->second.show_tag( );
			}
#endif
		}

		return( pointer );
	}


	/// @brief DICOMファイルを読み込んで，全てのDICOMタグを処理しテーブルに登録する
	//! 
	//! @param[out] dicm             … DICOMタグ毎にデータを登録するテーブル
	//! @param[in]  buff             … DICOMのデータが入ったバッファの先頭ポインタ
	//! @param[in]  numBytes         … バッファ内のデータのバイト数
	//! @param[in]  is_little_endian … データの中身をリトルエンディアンとして処理するかどうか
	//! 
	//! @retval true  … DICOMファイルの処理に成功
	//! @retval false … DICOMファイルではないか，処理できないタグ・データが存在する場合
	//! 
	inline bool read_dicom_tags( dicom_tag_container &dicm, unsigned char *buff, size_type numBytes, bool is_little_endian = true )
	{
		unsigned char *pointer = buff;
		unsigned char *end_pointer = buff + numBytes;

		// DICOMデータの先頭までポインタを移動
		pointer = check_dicom_file( pointer, end_pointer );

		dicm.clear( );

		unsigned char *group_end_pointer = NULL;
		bool ret = true, from_little_endian = is_little_endian, once = true;
		while( pointer < end_pointer )
		{
			pointer = process_dicom_tag( dicm, pointer, end_pointer, from_little_endian );
			if( pointer == NULL )
			{
				ret = false;
				break;
			}

			if( group_end_pointer == NULL && dicm.contain( 0x0002, 0x0000 ) )
			{
				// エンディアンの判定に用いる範囲を決定する
				group_end_pointer = pointer + find_tag( dicm, 0x0002, 0x0000, static_cast< unsigned int >( 0 ) );
			}

			if( group_end_pointer != NULL && once && dicm.contain( 0x0002, 0x0010 ) )
			{
				// 一度しか実行されないようにする
				once = false;

				// エンディアンの形式を設定する
				from_little_endian = find_tag( dicm, 0x0002, 0x0010, "" ) != "1.2.840.10008.1.2.2";

				// ビッグエンディアンの場合は，このブロックだけリトルエンディアンで処理する
				if( !from_little_endian )
				{
					while( pointer < group_end_pointer )
					{
						pointer = process_dicom_tag( dicm, pointer, group_end_pointer, true );
						if( pointer == NULL )
						{
							pointer = end_pointer;
							ret = false;
							break;
						}
					}
				}
			}
		}

#ifdef __SHOW_DICOM_TAG__
		printf( "\n\n\n" );
#endif

		return( ret );
	}


	/// @brief DICOMファイルを読み込んで，全てのDICOMタグを処理しテーブルに登録する
	//! 
	//! @param[out] dicm  … DICOMタグ毎にデータを登録するテーブル
	//! @param[in]  filename … 入力DICOM]ファイル名
	//! 
	//! @retval true  … DICOMファイルの処理に成功
	//! @retval false … DICOMファイルではないか，処理できないタグ・データが存在する場合
	//! 
	inline bool read_dicom_tags( dicom_tag_container &dicm, const std::string &filename )
	{
		size_type filesize;
		FILE *fp;
		if( ( fp = fopen( filename.c_str( ), "rb" ) ) == NULL )
		{
			return( false );
		}

		// ファイルサイズを取得
		fseek( fp, 0, SEEK_END );
		filesize = ftell( fp );
		fseek( fp, 0, SEEK_SET );

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

		bool ret = read_dicom_tags( dicm, buff, filesize, true );

		delete [] buff;

#ifdef __SHOW_DICOM_TAG__
		printf( "\n\n\n" );
#endif

		// 適切なDICOMファイルかどうかをチェックする
		if( ret )
		{
			dicom_tag_container::iterator ite = dicm.begin( );
			for( ; ite != dicm.end( ) ; ++ite )
			{
				if( ite->second.comment != "UNKNOWN" )
				{
					return( true );
				}
			}

			return( false );
		}
		else
		{
			return( false );
		}
	}


	/// @brief DICOMのタグ集合を Explicit VR 形式でファイルに書き出す
	//! 
	//! @param[in]  group     … グループID
	//! @param[in]  element   … エレメントID
	//! @param[in]  vr        … VRタグ
	//! @param[in]  data      … データ
	//! @param[in]  num_bytes … データのバイト数
	//! @param[out] fp    … 出力ファイルストリーム
	//! @param[in]  to_little_endian … 出力データの形式をリトルエンディアンにするかどうか
	//! 
	//! @retval true  … DICOMファイルの処理に成功
	//! @retval false … DICOMファイルではないか，処理できないタグ・データが存在する場合
	//! 
	inline bool write_dicom_tag_explicit_vr( unsigned short group, unsigned short element, dicom_vr vr, const unsigned char *data, size_t num_bytes, FILE *fp, bool to_little_endian = true )
	{
		unsigned char ZERO[] = { 0, 0, 0, 0 };
		unsigned char FFFF[] = { 0xff, 0xff, 0xff, 0xff };

		switch( vr )
		{
		case OW:
		case OF:
			fwrite( from_current_endian( byte_array< unsigned short >( group ), to_little_endian ).get_bytes( ), 1, 2, fp );
			fwrite( from_current_endian( byte_array< unsigned short >( element ), to_little_endian ).get_bytes( ), 1, 2, fp );
			fwrite( get_dicom_vr( vr ).c_str( ), 1, 2, fp );
			fwrite( ZERO, 1, 2, fp );

			// データがシーケンスになっているかを調べ，出力方法を切り替える
			if( num_bytes > 4 + 8 && is_sequence_separate_tag( data, data + num_bytes ) && is_sequence_tag_end( data + num_bytes - 8, data + num_bytes ) )
			{
				fwrite( FFFF, 1, 4, fp );
			}
			else
			{
				fwrite( from_current_endian( byte_array< unsigned int >( static_cast< unsigned int >( num_bytes ) ), to_little_endian ).get_bytes( ), 1, 4, fp );
			}

			if( _is_little_endian_( ) == to_little_endian )
			{
				fwrite( data, 1, num_bytes, fp );
			}
			else
			{
				switch( vr )
				{
				case OW:
					if( num_bytes % 2 == 0 )
					{
						for( size_t i = 0 ; i < num_bytes ; i += 2 )
						{
							fwrite( from_current_endian( byte_array< unsigned short >( data + i ), to_little_endian ).get_bytes( ), 1, 2, fp );
						}
					}
					else
					{
						// データのバイト数がおかしい
						return( false );
					}
					break;

				case OF:
				default:
					if( num_bytes % 4 == 0 )
					{
						for( size_t i = 0 ; i < num_bytes ; i += 4 )
						{
							fwrite( from_current_endian( byte_array< unsigned int >( data + i ), to_little_endian ).get_bytes( ), 1, 4, fp );
						}
					}
					else
					{
						// データのバイト数がおかしい
						return( false );
					}
					break;
				}
			}
			break;

		case US:
		case SS:
		case AT:
		case UL:
		case SL:
		case FL:
		case FD:
			fwrite( from_current_endian( byte_array< unsigned short >( group ), to_little_endian ).get_bytes( ), 1, 2, fp );
			fwrite( from_current_endian( byte_array< unsigned short >( element ), to_little_endian ).get_bytes( ), 1, 2, fp );
			fwrite( get_dicom_vr( vr ).c_str( ), 1, 2, fp );
			fwrite( from_current_endian( byte_array< unsigned short >( static_cast< unsigned short >( num_bytes ) ), to_little_endian ).get_bytes( ), 1, 2, fp );
			if( _is_little_endian_( ) == to_little_endian )
			{
				fwrite( data, 1, num_bytes, fp );
			}
			else
			{
				switch( vr )
				{
				case US:
				case SS:
				case AT:
					if( num_bytes % 2 == 0 )
					{
						for( size_t i = 0 ; i < num_bytes ; i += 2 )
						{
							fwrite( from_current_endian( byte_array< unsigned short >( data + i ), to_little_endian ).get_bytes( ), 1, 2, fp );
						}
					}
					else
					{
						// データのバイト数がおかしい
						return( false );
					}
					break;

				case UL:
				case SL:
				case FL:
					if( num_bytes % 4 == 0 )
					{
						for( size_t i = 0 ; i < num_bytes ; i += 4 )
						{
							fwrite( from_current_endian( byte_array< unsigned int >( data + i ), to_little_endian ).get_bytes( ), 1, 4, fp );
						}
					}
					else
					{
						// データのバイト数がおかしい
						return( false );
					}
					break;

				case FD:
				default:
					if( num_bytes % 8 == 0 )
					{
						for( size_t i = 0 ; i < num_bytes ; i += 8 )
						{
							fwrite( from_current_endian( byte_array< double >( data + i ), to_little_endian ).get_bytes( ), 1, 8, fp );
						}
					}
					else
					{
						// データのバイト数がおかしい
						return( false );
					}
					break;
				}
			}
			break;

		case OB:
		case UN:
		case UT:
		case SQ:
			fwrite( from_current_endian( byte_array< unsigned short >( group ), to_little_endian ).get_bytes( ), 1, 2, fp );
			fwrite( from_current_endian( byte_array< unsigned short >( element ), to_little_endian ).get_bytes( ), 1, 2, fp );
			fwrite( get_dicom_vr( vr ).c_str( ), 1, 2, fp );
			fwrite( ZERO, 1, 2, fp );
			// データがシーケンスになっているかを調べ，出力方法を切り替える
			if( num_bytes > 4 + 8 && is_sequence_separate_tag( data, data + num_bytes ) && is_sequence_tag_end( data + num_bytes - 8, data + num_bytes ) )
			{
				fwrite( FFFF, 1, 4, fp );
				fwrite( data, 1, num_bytes, fp );
			}
			else
			{
				fwrite( from_current_endian( byte_array< unsigned int >( static_cast< unsigned int >( num_bytes ) ), to_little_endian ).get_bytes( ), 1, 4, fp );
				fwrite( data, 1, num_bytes, fp );
			}
			break;

		case UNKNOWN:
			fwrite( from_current_endian( byte_array< unsigned short >( group ), to_little_endian ).get_bytes( ), 1, 2, fp );
			fwrite( from_current_endian( byte_array< unsigned short >( element ), to_little_endian ).get_bytes( ), 1, 2, fp );
			fwrite( "UN", 1, 2, fp );
			fwrite( get_dicom_vr( vr ).c_str( ), 1, 2, fp );
			fwrite( from_current_endian( byte_array< unsigned int >( static_cast< unsigned int >( num_bytes ) ), to_little_endian ).get_bytes( ), 1, 4, fp );
			fwrite( data, 1, num_bytes, fp );
			break;

		default:
			fwrite( from_current_endian( byte_array< unsigned short >( group ), to_little_endian ).get_bytes( ), 1, 2, fp );
			fwrite( from_current_endian( byte_array< unsigned short >( element ), to_little_endian ).get_bytes( ), 1, 2, fp );
			fwrite( get_dicom_vr( vr ).c_str( ), 1, 2, fp );
			fwrite( from_current_endian( byte_array< unsigned short >( static_cast< unsigned short >( num_bytes ) ), to_little_endian ).get_bytes( ), 1, 2, fp );
			fwrite( data, 1, num_bytes, fp );
			break;
		}

		return( true );
	}



	/// @brief DICOMのタグ集合を Implicit VR 形式でファイルに書き出す
	//! 
	//! @param[in]  group     … グループID
	//! @param[in]  element   … エレメントID
	//! @param[in]  vr        … VRタグ
	//! @param[in]  data      … データ
	//! @param[in]  num_bytes … データのバイト数
	//! @param[out] fp    … 出力ファイルストリーム
	//! @param[in]  to_little_endian … 出力データの形式をリトルエンディアンにするかどうか
	//! 
	//! @retval true  … DICOMファイルの処理に成功
	//! @retval false … DICOMファイルではないか，処理できないタグ・データが存在する場合
	//! 
	inline bool write_dicom_tag_implicit_vr( unsigned short group, unsigned short element, dicom_vr vr, const unsigned char *data, size_t num_bytes, FILE *fp, bool to_little_endian = true )
	{
		unsigned char FFFF[] = { 0xff, 0xff, 0xff, 0xff };

		switch( vr )
		{
		case OB:
		case UN:
		case UT:
		case SQ:
			// データがシーケンスになっているかを調べ，出力方法を切り替える
			if( num_bytes > 4 + 8 && is_sequence_separate_tag( data, data + num_bytes ) && is_sequence_tag_end( data + num_bytes - 8, data + num_bytes ) )
			{
				fwrite( from_current_endian( byte_array< unsigned short >( group ), to_little_endian ).get_bytes( ), 1, 2, fp );
				fwrite( from_current_endian( byte_array< unsigned short >( element ), to_little_endian ).get_bytes( ), 1, 2, fp );
				fwrite( FFFF, 1, 4, fp );
				fwrite( data, 1, num_bytes, fp );
			}
			else
			{
				fwrite( from_current_endian( byte_array< unsigned short >( group ), to_little_endian ).get_bytes( ), 1, 2, fp );
				fwrite( from_current_endian( byte_array< unsigned short >( element ), to_little_endian ).get_bytes( ), 1, 2, fp );
				fwrite( from_current_endian( byte_array< unsigned int >( static_cast< unsigned int >( num_bytes ) ), to_little_endian ).get_bytes( ), 1, 4, fp );
				fwrite( data, 1, num_bytes, fp );
			}
			break;

		default:
			fwrite( from_current_endian( byte_array< unsigned short >( group ), to_little_endian ).get_bytes( ), 1, 2, fp );
			fwrite( from_current_endian( byte_array< unsigned short >( element ), to_little_endian ).get_bytes( ), 1, 2, fp );
			fwrite( from_current_endian( byte_array< unsigned int >( static_cast< unsigned int >( num_bytes ) ), to_little_endian ).get_bytes( ), 1, 4, fp );
			fwrite( data, 1, num_bytes, fp );
			break;
		}

		return( true );
	}

	/// @brief DICOMのタグ集合を Explicit VR 形式でファイルに書き出す
	//! 
	//! @param[in]  e     … ファイルに出力するDICOMタグ
	//! @param[out] fp    … 出力ファイルストリーム
	//! @param[in]  to_little_endian … 出力データの形式をリトルエンディアンにするかどうか
	//! 
	//! @retval true  … DICOMファイルの処理に成功
	//! @retval false … DICOMファイルではないか，処理できないタグ・データが存在する場合
	//! 
	inline bool write_dicom_tag_explicit_vr( const dicom_element &e, FILE *fp, bool to_little_endian = true )
	{
		return( write_dicom_tag_explicit_vr( e.get_group( ), e.get_element( ), e.vr, e.data, e.num_bytes, fp, to_little_endian ) );
	}

	/// @brief DICOMのタグ集合を Implicit VR 形式でファイルに書き出す
	//! 
	//! @param[in]  e     … ファイルに出力するDICOMタグ
	//! @param[out] fp    … 出力ファイルストリーム
	//! @param[in]  to_little_endian … 出力データの形式をリトルエンディアンにするかどうか
	//! 
	//! @retval true  … DICOMファイルの処理に成功
	//! @retval false … DICOMファイルではないか，処理できないタグ・データが存在する場合
	//! 
	inline bool write_dicom_tag_implicit_vr( const dicom_element &e, FILE *fp, bool to_little_endian = true )
	{
		return( write_dicom_tag_implicit_vr( e.get_group( ), e.get_element( ), e.vr, e.data, e.num_bytes, fp, to_little_endian ) );
	}


	/// @brief DICOMのタグを出力する際に各タグが必要とするバイト数を計算する
	//! 
	//! @param[in] e          … DICOMタグ
	//! @param[in] implicitVR … Implicit VR 形式かどうか
	//! 
	//! @return タグのバイト数
	//! 
	inline size_t get_write_dicom_tag_size( const dicom_element &e, bool implicitVR )
	{
		if( implicitVR )
		{
			return( 8 + e.num_bytes );
		}
		else
		{
			size_t num_bytes = 0;

			switch( e.vr )
			{
			case OW:
			case OF:
			case OB:
			case UN:
			case SQ:
			case UT:
			case UNKNOWN:
				num_bytes = 12 + e.num_bytes;
				break;

			case US:
			case SS:
			case AT:
			case UL:
			case SL:
			case FL:
			case FD:
			default:
				num_bytes = 8 + e.num_bytes;
				break;
			}

			return( num_bytes );
		}
	}


	/// @brief DICOMの Group Length を計算し，タグに設定する
	//! 
	//! @param[in,out] dicm       … DICOMタグ毎にデータを登録するテーブル
	//! @param[in]     implicitVR … Implicit VR 形式かどうか
	//! 
	inline void compute_group_length( dicom_tag_container &dicm, bool implicitVR )
	{
		std::map< unsigned short, size_t > group_length;
		dicom_tag_container::iterator ite;
		for( ite = dicm.begin( ) ; ite != dicm.end( ) ; ++ite )
		{
			const dicom_element &e = ite->second;
			if( e.enable )
			{
				unsigned short group     = e.get_group( );
				unsigned short element   = e.get_element( );
				// Group 0002 だけは Explicit VR Little Endian Transfer Syntax でエンコードする必要あり
				size_t num_bytes = get_write_dicom_tag_size( e, group == 0x0002 ? false : implicitVR );

				if( element != 0x0000 )
				{
					// Group Length のタグはスキップする
					if( group_length.find( group ) != group_length.end( ) )
					{
						group_length[ group ] += num_bytes;
					}
					else
					{
						group_length[ group ] = num_bytes;
					}
				}
			}
		}

		for( ite = dicm.begin( ) ; ite != dicm.end( ) ; ++ite )
		{
			dicom_element &e = ite->second;
			unsigned short group   = e.get_group( );
			unsigned short element = e.get_element( );

			if( element == 0x0000 )
			{
				// Group Length のタグにデータを設定する
				size_t num_bytes = group_length[ group ];

				switch( e.num_bytes )
				{
				case 2:
					{
						byte_array< unsigned short > b( static_cast< unsigned short >( num_bytes ) );
						memcpy( e.data, b.get_bytes( ), e.num_bytes );
					}
					break;

				case 4:
				default:
					{
						byte_array< unsigned int > b( static_cast< unsigned int >( num_bytes ) );
						memcpy( e.data, b.get_bytes( ), e.num_bytes );
					}
					break;
				}
			}
		}
	}


	/// @brief DICOMのタグ集合をファイルに書き出す
	//! 
	//! @param[out] dicom    … DICOMタグ毎にデータを登録するテーブル
	//! @param[in]  filename … 出力DICOMファイル名
	//! 
	//! @retval true  … DICOMファイルの処理に成功
	//! @retval false … DICOMファイルではないか，処理できないタグ・データが存在する場合
	//! 
	inline bool write_dicom_tags( const dicom_tag_container &dicom, const std::string &filename )
	{
		FILE *fp;
		if( ( fp = fopen( filename.c_str( ), "wb" ) ) == NULL )
		{
			return( false );
		}

		// DICOMのプリアンブルのタグが存在するかどうかを調べる
		bool hasPreamble = false;
		{
			unsigned short elements[] = { 0x0001, 0x0002, 0x0003, 0x0010, 0x0012, 0x0013, 0x0016, 0x0100, 0x0102 };
			size_t num = sizeof( elements ) / sizeof( unsigned short );
			for( size_t i = 0 ; i < num ; i++ )
			{
				if( dicom.contain( 0x0002, elements[ i ] ) )
				{
					// データが存在するのでプリアンブルを埋め込む
					hasPreamble = true;
					break;
				}
			}
		}

		dicom_tag_container dicm( dicom );
		bool implicitVR = true;

		if( hasPreamble )
		{
			// DICOMのヘッダ情報を書き込む
			unsigned char ZERO[ 128 ];
			unsigned char DICM[ 4 ];
			memset( ZERO, 0, sizeof( unsigned char ) * 128 );
			DICM[ 0 ] = 'D';
			DICM[ 1 ] = 'I';
			DICM[ 2 ] = 'C';
			DICM[ 3 ] = 'M';
			fwrite( ZERO, sizeof( unsigned char ), 128, fp );
			fwrite( DICM, sizeof( unsigned char ), 4, fp );


			// 明示的VRの指定がない場合は追加する
			if( !dicm.contain( 0x0002, 0x0010 ) )
			{
				// Transfer Syntax UID が存在しない場合は，「Implicit VR Little Endian」を指定する
				std::string syntax = "1.2.840.10008.1.2";
				dicm.append( dicom_element( 0x0002, 0x0010, reinterpret_cast< const unsigned char * >( syntax.c_str( ) ), syntax.length( ) ) );
			}
			else if( find_tag( dicm, 0x0002, 0x0010, "" ) == "1.2.840.10008.1.2.1" )
			{
				// 「Explicit VR Little Endian」が指定されている場合は，「Explicit VR Little Endian」に変更する
				implicitVR = false;
			}
			else if( find_tag( dicm, 0x0002, 0x0010, "" ) == "1.2.840.10008.1.2.2" )
			{
				// 「Implicit VR Big Endian」が指定されている場合は，「Explicit VR Little Endian」に変更する
				implicitVR = false;
				std::string syntax = "1.2.840.10008.1.2.1";
				dicm( 0x0002, 0x0010 ).copy( reinterpret_cast< const unsigned char * >( syntax.c_str( ) ), syntax.length( ) );
			}
			else if( get_compress_type( find_tag( dicm, 0x0002, 0x0010, "" ) ) != RAW )
			{
				// 圧縮されている場合は「Explicit VR Little Endian」に変更する
				implicitVR = false;
			}

			// Group 0002 の長さが挿入されていない場合は挿入する
			if( !dicm.contain( 0x0002, 0x0000 ) )
			{
				byte_array< unsigned int > b( static_cast< unsigned int >( 0 ) );
				dicm.append( dicom_element( 0x0002, 0x0000, b.get_bytes( ), b.length( ) ) );
			}
		}

		// DICOM のタグが使用に準拠するようにする
		dicm.update( );

		// Group Length を正しく設定する
		compute_group_length( dicm, implicitVR );

		// VRに応じて出力する
		dicom_tag_container::const_iterator ite = dicm.begin( );
		for( ; ite != dicm.end( ) ; ++ite )
		{
			if( ite->second.enable )
			{
				// Group 0002 だけは Explicit VR Little Endian Transfer Syntax でエンコードする必要あり
				if( implicitVR && ite->second.get_group( ) != 0x0002 )
				{
					write_dicom_tag_implicit_vr( ite->second, fp, true );
				}
				else
				{
					write_dicom_tag_explicit_vr( ite->second, fp, true );
				}
			}
		}

		fclose( fp );

		return( true );
	}
}




/// @brief DICOMデータを画像コンテナに読み込む
//! 
//! @note 本処理では，不要なタグは全て取り除かれてしまうため，必要なデータがある場合は，本関数内を参考にして自作してください
//! 
//! @param[out] image    … 出力画像
//! @param[in]  filename … 入力ファイル名
//! 
//! @retval true  … DICOMファイルの処理に成功
//! @retval false … DICOMファイルではないか，処理できないタグ・データが存在する場合
//! 
template < class T, class Allocator >
bool read_dicom( array2< T, Allocator > &image, const std::string &filename )
{
	typedef typename array2< T, Allocator >::value_type value_type;
	typedef typename array2< T, Allocator >::size_type size_type;
	typedef _pixel_converter_< T > pixel_converter;
	typedef typename pixel_converter::color_type color_type;

	dicom::dicom_tag_container dicm;
	dicom::dicom_info info;

	if( !dicom::read_dicom_tags( dicm, filename ) )
	{
		return( false );
	}

	dicom::get_dicom_info( dicm, info );

	double window_level = info.window_center;
	double window_width = info.window_width;

	if( dicm.contain( 0x7fe0, 0x0010 ) )
	{
		switch( info.compression_type )
		{
		case dicom::JPEG:
		case dicom::JPEGLS:
		case dicom::JPEG2000:
			// 今のところ未サポート
			return( false );

		default:
			break;
		}

		dicom::dicom_element element = dicm( 0x7fe0, 0x0010 );
		if( !dicom::decode( element, info ) )
		{
			// 圧縮の解凍もしくはデータがおかしい
			return( false );
		}

		image.resize( info.cols, info.rows );
		image.reso1( info.pixel_spacing_x );
		image.reso2( info.pixel_spacing_y );

		switch( info.photometric_type )
		{
		case dicom::RGB:
			if( info.compression_type == dicom::RLE )
			{
				// RLEで圧縮する際は，色ごとに圧縮されるみたい
				for( size_type j = 0 ; j < image.height( ) ; j++ )
				{
					unsigned char *r = element.data + j * info.cols;
					unsigned char *g = element.data + j * info.cols + info.cols * info.rows;
					unsigned char *b = element.data + j * info.cols + info.cols * info.rows * 2;
					for( size_type i = 0 ; i < image.width( ) ; i++ )
					{
						image( i, j ) = pixel_converter::convert_to( r[ i ], g[ i ], b[ i ] );
					}
				}
			}
			else
			{
				for( size_type j = 0 ; j < image.height( ) ; j++ )
				{
					unsigned char *data = element.data + j * image.width( ) * 3;
					for( size_type i = 0 ; i < image.width( ) ; i++ )
					{
						size_type ii = i * 3;
						image( i, j ) = pixel_converter::convert_to( data[ ii + 2 ], data[ ii + 1 ], data[ ii + 0 ] );
					}
				}
			}
			break;

		case dicom::PALETTE_COLOR:
			if( dicm.contain( 0x0028, 0x1201 ) && dicm.contain( 0x0028, 0x1202 ) && dicm.contain( 0x0028, 0x1203 ) )
			{
				const dicom::dicom_element &red   = dicm( 0x0028, 0x1201 );
				const dicom::dicom_element &green = dicm( 0x0028, 0x1202 );
				const dicom::dicom_element &blue  = dicm( 0x0028, 0x1203 );

				const short *r = reinterpret_cast< const short * >( red.data );
				const short *g = reinterpret_cast< const short * >( green.data );
				const short *b = reinterpret_cast< const short * >( blue.data );

				bool is_big_endian = false;

				int max_value = ( 2 << info.bits_stored ) - 1;
				for( size_t i = 0 ; i < red.num_bytes / 2 ; i++ )
				{
					if( r[ i ] > max_value || g[ i ] > max_value || b[ i ] > max_value )
					{
						is_big_endian = true;
						break;
					}
				}

				if( is_big_endian )
				{
					// パレットのデータは，必ずビッグエンディアンでインプリメントされている
					for( size_type j = 0 ; j < image.height( ) ; j++ )
					{
						unsigned char *data = element.data + j * image.width( );
						for( size_type i = 0 ; i < image.width( ) ; i++ )
						{
							unsigned char R = static_cast< unsigned char >( from_current_endian( byte_array< short >( r[ data[ i ] ] ), false ).get_value( ) );
							unsigned char G = static_cast< unsigned char >( from_current_endian( byte_array< short >( g[ data[ i ] ] ), false ).get_value( ) );
							unsigned char B = static_cast< unsigned char >( from_current_endian( byte_array< short >( b[ data[ i ] ] ), false ).get_value( ) );
							image( i, j ) = pixel_converter::convert_to( R, G, B );
						}
					}
				}
				else
				{
					// パレットのデータは，必ずビッグエンディアンでインプリメントされている
					for( size_type j = 0 ; j < image.height( ) ; j++ )
					{
						unsigned char *data = element.data + j * image.width( );
						for( size_type i = 0 ; i < image.width( ) ; i++ )
						{
							unsigned char R = static_cast< unsigned char >( r[ data[ i ] ] );
							unsigned char G = static_cast< unsigned char >( g[ data[ i ] ] );
							unsigned char B = static_cast< unsigned char >( b[ data[ i ] ] );
							image( i, j ) = pixel_converter::convert_to( R, G, B );
						}
					}
				}
			}
			break;

		case dicom::MONOCHROME1:
		case dicom::MONOCHROME2:
		default:
			if( info.bits_allocated == 8 )
			{
				for( size_type j = 0 ; j < image.height( ) ; j++ )
				{
					unsigned char *data = element.data + j * image.width( );
					for( size_type i = 0 ; i < image.width( ) ; i++ )
					{
						image( i, j ) = data[ i ];
					}
				}
			}
			else if( info.bits_allocated == 16 )
			{
				// MONOCHROME1 では背景が白がデフォルト
				// MONOCHROME2 では背景が黒がデフォルト
				if( info.photometric_type == dicom::MONOCHROME1 )
				{
					window_width = -window_width;
				}

				double offset = info.rescale_intercept;
				if( dicm.contain( 0x0028, 0x1050 ) && dicm.contain( 0x0028, 0x1051 ) && dicm( 0x0028, 0x1050 ).num_bytes > 0 && dicm( 0x0028, 0x1051 ).num_bytes > 0 )
				{
					const short *bytes = reinterpret_cast< const short * >( element.data );
					for( size_type j = 0 ; j < image.height( ) ; j++ )
					{
						const short *data = bytes + j * image.width( );
						for( size_type i = 0 ; i < image.width( ) ; i++ )
						{
							double pix = ( ( data[ i ] + offset - window_level ) / window_width + 0.5 ) * 255.0;
							pix = pix > 255.0 ? 255.0 : pix;
							pix = pix <   0.0 ?   0.0 : pix;
							image( i, j ) = static_cast< unsigned char >( pix );
						}
					}
				}
				else
				{
					// まず，描画に用いる範囲を決定する
					const unsigned short *bytes = reinterpret_cast< const unsigned short * >( element.data );
					double min = bytes[ 0 ], max = bytes[ 0 ];
					for( size_t l = 1 ; l < element.num_bytes / 2 ; l++ )
					{
						if( bytes[ l ] < min )
						{
							min = bytes[ l ];
						}
						else if( max < bytes[ l ] )
						{
							max = bytes[ l ];
						}
					}

					double ww = max - min + 1;
					double wl = ( max + min ) / 2.0;

					// MONOCHROME1 では背景が白がデフォルト
					// MONOCHROME2 では背景が黒がデフォルト
					if( info.photometric_type == dicom::MONOCHROME1 )
					{
						ww = -ww;
					}

					for( size_type j = 0 ; j < image.height( ) ; j++ )
					{
						const unsigned short *data = bytes + j * image.width( );
						for( size_type i = 0 ; i < image.width( ) ; i++ )
						{
							double pix = ( ( data[ i ] - wl ) / ww + 0.5 ) * 255.0;
							pix = pix > 255.0 ? 255.0 : pix;
							pix = pix <   0.0 ?   0.0 : pix;
							image( i, j ) = static_cast< unsigned char >( pix );
						}
					}
				}
			}
			else
			{
				return( false );
			}
			break;
		}
	}
	else
	{
		return( false );
	}

	return( true );
}


/// @}
//  DICOM画像入出力グループの終わり

/// @}
//  画像入出力グループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_DICOM__
