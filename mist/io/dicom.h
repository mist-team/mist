#ifndef __INCLUDE_MIST_DICOM__
#define __INCLUDE_MIST_DICOM__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#ifndef __INCLUDE_MIST_ENDIAN__
#include "../config/endian.h"
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

// mist名前空間の始まり
_MIST_BEGIN


// 次のマクロを定義すると，コンソール上に読み込んだタグ情報を表示する
// #define __SHOW_DICOM_TAG__
// #define __SHOW_DICOM_UNKNOWN_TAG__

namespace dicom_controller
{
	inline unsigned char *check_dicom_file( unsigned char *p, unsigned char *e )
	{
		if( p == NULL || p + 128 + 4 >= e )
		{
			return( NULL );
		}


		char *dicom = reinterpret_cast< char * >( p ) + 128;
		if( dicom[ 0 ] == 'D' && dicom[ 1 ] == 'I' && dicom[ 2 ] == 'C' && dicom[ 3 ] == 'M' )
		{
			return( p + 128 + 4 );
		}

		// DICOM用のプリアンブルが存在しないため，先頭のポインタを返す
		return( p );
	}

	// DICOMのタグを読み込み，テーブルに登録されているものと照合する．
	// テーブルに登録されていない場合は，読み飛ばす．
	// もし，適切なDICOMファイルでない場合は-2を返し，データの終端もしくはファイルの読み込みに失敗した場合は-1を返す．
	// そして，読み飛ばした場合は0を返し，テーブルに登録されている場合は次に存在するデータのバイト数を返す．
	// データを処理する先頭のポインタ 'p' と，データの最後＋1をさすポインタ 'e' を与える
	inline unsigned char *read_dicom_tag( unsigned char *p, unsigned char *e, dicom_tag &tag, difference_type &numBytes )
	{
		if( p == NULL || p + 8 >= e )
		{
			numBytes = -1;
			return( e );
		}

		unsigned char *data = p;

		unsigned short group   = to_current_endian( byte_array< unsigned short >( data ), true ).get_value( );
		unsigned short element = to_current_endian( byte_array< unsigned short >( data + 2 ), true ).get_value( );

		data += 4;

		char VR[ 3 ] = { static_cast< char >( data[ 0 ] ), static_cast< char >( data[ 1 ] ), '\0' };
		dicom_vr vr = get_dicom_vr( VR );

		difference_type num_bytes = 0;
		static dicom_tag_table dicom_table;
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
					data += 4;
					num_bytes = to_current_endian( byte_array< unsigned int >( data ), true ).get_value( );
					break;

				default:
					num_bytes = to_current_endian( byte_array< unsigned short >( data + 2 ), true ).get_value( );
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
					data += 4;
					num_bytes = to_current_endian( byte_array< unsigned int >( data ), true ).get_value( );
					break;

				default:
					num_bytes = to_current_endian( byte_array< unsigned short >( data + 2 ), true ).get_value( );
				}
			}
			else
			{
				switch( vr )
				{
				case OB:
				case OW:
				case UN:
				case SQ:
					data += 4;
					num_bytes = to_current_endian( byte_array< unsigned int >( data ), true ).get_value( );
					break;

				default:
					// 暗示的VR
					num_bytes = to_current_endian( byte_array< unsigned int >( data ), true ).get_value( );
				}
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
			case UT:
				{
					num_bytes = to_current_endian( byte_array< unsigned short >( data + 2 ), true ).get_value( );
				}
				break;

			case OB:
			case OW:
			case UN:
			case SQ:
				data += 4;
				num_bytes = to_current_endian( byte_array< unsigned int >( data ), true ).get_value( );
				break;

			default:
				num_bytes = to_current_endian( byte_array< unsigned int >( data ), true ).get_value( );
				break;
			}

#ifdef __SHOW_DICOM_UNKNOWN_TAG__
			printf( "( %04x, %04x, %s, % 8d ) = Unknown Tags!!\n", group, element, VR, num_bytes );
#endif
			if( data + 4 + num_bytes <= e )
			{
				numBytes = 0;
				return( reinterpret_cast< unsigned char * >( data + 4 + num_bytes ) );
			}
			else
			{
				numBytes = -2;
				return( p );
			}
		}
	}


	// DICOMのUIDを変換する
	inline dicom_uid get_uid( const std::string &uid )
	{
		static dicom_uid_table uid_table;
		return( uid_table.get_uid( uid ) );
	}

	// DICOMのUIDを変換する
	inline dicom_uid get_uid( const unsigned char *str, difference_type numBytes )
	{
		return( get_uid( std::string( reinterpret_cast< const char * >( str ), str[ numBytes - 1 ] == 0 ? numBytes - 1 : numBytes ) ) );
	}


	// DICOMのタグに対し，各VRごとの処理を行う．
	// 登録されていないタグの場合は false をかえし，正しく処理された場合のみ true を返す．
	inline bool process_dicom_tag( const dicom_tag &tag, unsigned char *byte, difference_type num_bytes )
	{
		switch( tag.vr )
		{
		case AE:
			// ＡＥ　応用エンティティ　１６バイト以下
			// 意味のない先頭と末尾のSPACE（20H）を持つ文字列。
			// “指定された応用名がない”ことを意味する１６のスペ－スでつくられる値は，使用しない
			if( tag.vm != -1 && num_bytes > 16 * tag.vm )
			{
				return( false );
			}
			break;

		case AS:
			// ＡＳ　年齢　　　　　　　４バイト固定
			// 次の書式の一つをもつ文字列 - nnnD, nnnW, nnnM, nnnY；
			// ここで nnn は D に対しては日，W に対しては週，M に対しては月，Y に対しては年の数を含む。
			// 例： "018M" は 18 月の年齢を表す。
			if( tag.vm != -1 && ( num_bytes % 4 != 0 || (int)( num_bytes / 4 ) > tag.vm ) )
			{
				return( false );
			}
			break;

		case AT:
			// ＡＴ　属性タグ　　　　　４バイト固定
			// データ要素タグの値である１６ビット符号なし整数の順序付けられた対。
			// 例： (0018,00FF) のデータ要素タグは，４バイトのシリーズとして
			// リトルエンディアン転送構文では 18H, 00H, FFH, 00H として，
			// ビッグエンディアン転送構文では 00H, 18H, 00H, FFH として符号化されるであろう。
			// 注：ＡＴ値の符号化は節７の中で定義されるデータ要素タグの符号化と正確に同一である。
			if( tag.vm != -1 && ( num_bytes % 4 != 0 || (int)( num_bytes / 4 ) > tag.vm ) )
			{
				return( false );
			}
			break;

		case CS:
			// ＣＳ　コード列　　　　　１６バイト以下
			// 意味のない先頭または末尾のスペ－ス（20H）をもつ文字列。
			if( tag.vm != -1 && num_bytes > 16 * tag.vm )
			{
				return( false );
			}
			break;

		case DA:
			// ＤＡ　日付　　　　　　　８バイト固定
			// 書式 yyyymmdd の文字列；
			// ここで yyyy は年，mm は月，dd は日を含む。
			// これはANSI HISPP MSDS Date common data typeに適合する。
			// 例："19930822"は 1993 年 8 月 22 日を表す。
			// 注：１．V 3.0 より前のこの規格の版との後方互換性のために，
			// 実装は，このＶＲに対して形式yyyy.mm.dd の文字列を同様にサポートすることを推奨される。
			if( tag.vm != -1 && ( num_bytes % 8 != 0 || (int)( num_bytes / 8 ) > tag.vm ) && ( num_bytes % 10 != 0 || (int)( num_bytes / 10 ) > tag.vm ) )
			{
				return( false );
			}
			break;

		case DS:
			// ＤＳ　10進数（文字列）　１６バイト以下
			// 固定小数点か浮動小数点数を表現する文字列。
			// 固定小数点数は文字 0-9，任意の先頭の "+" または "-"，および小数点を示す任意の "." のみを含む。
			// 浮動小数点数は，ANSI X3.9 の中で定義されるとおり，指数の始まりを示す "E" か "e" を持って伝達される。
			// 10 進数列は先頭あるいは末尾スペースで充てんされることがある。
			// 途中のスペースは許されない。
			if( tag.vm != -1 && num_bytes > 16 * tag.vm )
			{
				return( false );
			}
			break;

		case DT:
			// ＤＴ　日時　　　　　　　２６バイト以下
			// 日時共通データタイプ。
			// 書式 YYYYMMDDHHMMSS.FFFFFF&ZZZZ の連結日時 ASCII 列を示す。
			// この列の構成要素は，左から右へ YYYY = 年, MM = 月, DD = 日, HH = 時間, MM = 分, SS = 秒, FFFFFF = 秒の小数部分，
			// & = "+"または "-" ，および ZZZZ = オフセットの時間と分。
			// &ZZZZ は 協定世界時からのプラス／マイナスオフッセットに対する任意選択の接尾辞である。
			// 列から省かれた構成要素は空白構成要素と呼ばれる。
			// 日時の末尾の空白構成要素は無視される。
			// 末尾でない空白構成要素は禁止される，任意選択の接尾辞は構成要素と考えられない。
			// 注：	V 3.0 より前のこの規格の版との後方互換性のために，
			// 多くの存在するＤＩＣＯＭデータ要素は分離したＤＡおよびＴＭのＶＲを使用する。
			// 将来制定される標準および私的データ要素は ANSI HISPP MSDS により適合するために可能ならＤＴを使用する。
			if( tag.vm != -1 && num_bytes > 26 * tag.vm )
			{
				return( false );
			}
			break;

		case FL:
			// ＦＬ　４バイト実数　　　４バイト固定
			// 単精度の２進浮動小数点の数字で，IEEE 754: 1985, 32 ビット浮動小数点数形式で表現される。
			if( tag.vm != -1 && ( num_bytes % 4 != 0 || (int)( num_bytes / 4 ) > tag.vm ) )
			{
				return( false );
			}

			{
				byte_array< float > tmp = to_current_endian( byte_array< float >( byte ), true );
				byte[ 0 ] = tmp[ 0 ];
				byte[ 1 ] = tmp[ 1 ];
				byte[ 2 ] = tmp[ 2 ];
				byte[ 3 ] = tmp[ 3 ];
			}
			break;

		case FD:
			// ＦＤ　８バイト実質　　　８バイト固定
			// 倍精度の２進浮動小数点の数字で，IEEE 754: 1985, 64 ビット浮動小数点数形式で表現される。
			if( tag.vm != -1 && ( num_bytes % 8 != 0 || (int)( num_bytes / 8 ) > tag.vm ) )
			{
				return( false );
			}

			{
				byte_array< double > tmp = to_current_endian( byte_array< double >( byte ), true );
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
			// ＩＳ　整数（文字列）　　１６バイト以下
			// 10 を底とする整数（10 進数）を表わす文字列で，
			// 任意選択の先頭の "+", "-" をもつ文字 0-9 のみを含む。
			// これは先頭そして末尾のスペ－スで埋められることがある。
			// 途中のスペ－スは許されない。表現される整数 n は，下記の範囲である。
			// -231 ≦ n ≦ (231-1) 
			if( tag.vm != -1 && num_bytes > 16 * tag.vm )
			{
				return( false );
			}
			break;

		case LO:
			// ＬＯ　長列　　　　　　　６４バイト以下
			// 先頭および／または末尾のスペ－スで埋められることがある文字列。
			// 文字符号 5CH （ISO-IR 6 の中のバックスラッシュ "\" ）は，
			// 複数値デ－タ要素の中の値の間の区切り記号として使用されるので，存在しない。
			// この列は， ESC を除き，制御文字を持たない。
			if( tag.vm != -1 && num_bytes > 64 * tag.vm )
			{
				return( false );
			}
			break;

		case LT:
			// ＬＴ　テキスト　　　　　１０２４バイト以下
			// 一つ以上の段落を含むことがある文字列。
			// 図形文字集合と制御文字 CR, LF, FF, および ESC を含むことがある。
			// 無視されることがある末尾のスペース文字で埋められることがある，
			// しかし先頭のスペース文字は意味があると考えられる。
			// このＶＲを持つデ－タ要素は複数値ではない，
			// 従って文字符号 5CH （ISO-IR 6 の中のバックスラッシュ "\" ）は使用されることがある。
			if( tag.vm != -1 && num_bytes > 1024 * tag.vm )
			{
				return( false );
			}
			break;

		case OB:
			// ＯＢ　バイト列　　　　　無制限
			// 内容の符号化が折衝転送構文によって指定されるバイト列。
			// ＯＢはリトルエンディアン／ビッグエンディアンバイト順に影響されないＶＲである。
			// バイト列は偶数長にするために必要なとき，単一の末尾のNULLバイト値（00H）で埋められる。
			break;

		case OW:
			// ＯＷ　ワード列　　　　　無制限
			// 内容の符号化が折衝転送構文によって指定された１６ビットワード列。
			// ＯＷはリトルエンディアンおよびビッグエンディアンバイト順の間で変更するとき，
			// 各ワード内でバイトスワッピングを必要とするＶＲである。
			if( _is_big_endian_( ) )
			{
				char tmp;
				for( long i = 0 ; i < num_bytes / 2 ; i += 2 )
				{
					tmp = byte[ 2 * i ];
					byte[ 2 * i ] = byte[ 2 * i + 1 ];
					byte[ 2 * i + 1 ] = tmp;
				}
			}
			break;

		case PN:
			// ＰＮ　患者名　　　　　　６４バイト以下
			// ５構成要素規約を用いて符号化される文字列。
			// 文字符号 5CH （ISO-IR 6 の中のバックスラッシュ "\" ）は，複数値デ－タ要素の中の値の間の区切り記号として使用されるので，存在しない。
			// 列は，末尾スペースで埋められることがある。
			// ５構成要素はその発生順に：
			//       family name（姓）複合体，
			//       given name（名）複合体，
			//       middle name，name prefix（名前接頭辞），
			//       name suffix（名前接尾辞）。
			// ５構成要素の何れかが空の列のことがある。
			// 構成要素の区切り記号はキャラット文字 "^" (5EH) である。
			// 区切り記号は内部が空白の構成要素にも必要である。
			// 末尾の空白構成要素グループおよびそれらの区切り記号は省略されることがある。
			// 複数登録が，各構成要素の中で許され，そして自然文列として，その名前の人によって好まれる書式の中で符号化される。
			// これは ANSI HISPP MSDS 人名共通データタイプに適合する。
			// この５構成要素のグループは，人名構成要素グループとして参照される。
			// 名前を表意文字および表音文字で書く目的で，三つまでの構成要素グループが使用されることがある（付属書Ｈ例１および例２を参照）。
			// 構成要素グループのための区切り記号は，等号文字 "=" (3DH) である。三つの構成要素グループは出現順に，
			// １バイト文字表現，表意文字表現，表音文字表現である。
			//
			// 構成要素グループの何れもが，最初の構成要素グループを含んで，不在であることがある。
			// この場合，人の名前は，一つ以上の "=" 区切り記号から始まる。
			// 区切り記号は，内部の零の構成要素グループに対して必要である。
			// 末尾の零の構成要素グループおよびその区切り記号は，省略されることがある。
			// 詳細な意味が，各構成要素グループのために定義されている。
			// 例：Rev. John Robert Quincy Adams, B.A. M.Div."Adams^John Robert Quincy^^Rev.^B.A. M.Div."
			//      ［family name  1，given name  3，middle name 無，name prefix  1，name suffix  2］
			//     Susan Morrison-Jones, Ph.D., Chief Executive Officer"Morrison-Jones^Susan^^^Ph.D., Chief Executive Officer"
			//      ［family name  2，given name  1，middle name 無，name prefix 無，name suffix  2］
			//     John Doe"Doe^John"
			//      ［family name  1，given name  1，middle name 無，name prefix 無，name suffix 無。
			//           区切り記号は三つの末尾の零構成要素については省略されている。］
			//   （複数バイト文字集合を使用した人名の符号化の例については，付属書Ｈを参照。）
			// 注：	１．この５構成要素の規約は， ASTM E-1238-91の中で定義されANSI MSDSによってさらに特殊化され， HL7 によって同様に使用されている。
			//      ２．典型的なアメリカや欧州での使用法では"given name"の最初の部分が"first name"を表す。
			//          "given name"の二番目とそれに続く部分は普通"middle name"として扱われる。
			//          この"middle name"構成要素は，現存する規格との後方互換性の目的のために残されている。
			//      ３．ASTM E-1238-91の中に存在する「学位」構成要素は"suffix"構成要素の中に吸収された。
			//      ４．実装者は，"given name"を"first name"および"middle name"として表現する初期の使用形式と，
			//          そしてこの初期の典型的使用法への，またそれからの変換が必要とされることがあることに留意しなければならない。
			//      ５．版３．０より前のこの規格の版との後方互換性の理由のために，人名は，単一のfamily name（姓）複合体
			//         （区切り記号 "^" なしの単一の構成要素）と考えられることがある。
			if( tag.vm != -1 && num_bytes > 64 * tag.vm )
			{
				return( false );
			}
			break;

		case SH:
			// ＳＨ　短列　　　　　　　１６バイト以下
			// 先頭および／または末尾のスペ－スで埋められることがある文字列。
			// 文字符号 5CH （ISO-IR 6 の中のバックスラッシュ "\" ）は，複数値デ－タ要素のための値の間の区切り記号として使用されるので，存在しない。
			// この列は ESC を除き制御文字を持たない。
			if( tag.vm != -1 && num_bytes > 16 * tag.vm )
			{
				return( false );
			}
			break;

		case SL:
			// ＳＬ　符号つき長　　　　４バイト固定
			// ２の補数形式の３２ビット長符号付き２進整数。
			// 次の範囲の整数 n を表す： -2^31 ≦ n ≦ (2^31-1） 
			if( tag.vm != -1 && ( num_bytes % 4 != 0 || (int)( num_bytes / 4 ) > tag.vm ) )
			{
				return( false );
			}

			{
				byte_array< signed int > tmp = to_current_endian( byte_array< signed int >( byte ), true );
				byte[ 0 ] = tmp[ 0 ];
				byte[ 1 ] = tmp[ 1 ];
				byte[ 2 ] = tmp[ 2 ];
				byte[ 3 ] = tmp[ 3 ];
			}
			break;

		case SQ:
			// ＳＱ　項目シーケンス　　無制限
			// 値は，節７．５で定義される１つ以上の項目のシーケンスである。
			break;

		case SS:
			// ＳＳ　符号つき短　　　　２バイト固定
			// ２の補数形式の１６ビット長符号付き２進整。
			// 範囲 -2^15 ≦n≦ (2^15-1) の中の整数 n を表す。
			if( tag.vm != -1 && ( num_bytes % 2 != 0 || (int)( num_bytes / 2 ) > tag.vm ) )
			{
				return( 0 );
			}

			{
				byte_array< signed short > tmp = to_current_endian( byte_array< signed short >( byte ), true );
				byte[ 0 ] = tmp[ 0 ];
				byte[ 1 ] = tmp[ 1 ];
			}
			break;

		case ST:
			// ＳＴ　短テキスト　　　　１０２４バイト以下
			// 一つ以上の段落を含むことがある文字列。
			// 図形文字集合と制御文字 CR, LF, FF, および ESC を含むことがある。
			// 無視されることがある末尾のスペースで埋められることがある，しかし先頭のスペースは意味があると考えられる。
			// このＶＲを持つデ－タ要素は，複数値ではない，従って文字符号 5CH （ISO-IR 6 の中のバックスラッシュ "\" ）は使用されることがある。
			if( tag.vm != -1 && num_bytes > 1024 * tag.vm )
			{
				return( false );
			}
			break;

		case TM:
			// ＴＭ　時間　　　　　　　１６バイト以下
			// 書式 hhmmss.frac の文字列；
			//    hh は時間を含み（範囲 "00" - "23"），
			//    mm は分を含み（範囲 "00" - "59"），
			//    ss は秒を含み（範囲 "00" - "59"），
			//    frac は秒の１億分の１の単位の秒の部分を含む（範囲 "000000" - "999999"）。
			// ２４時間時刻が用いられる。真夜中は "2400" は時間の範囲を超すので， "0000" のみで表す。
			// 列は末尾のスペースで詰められることがある。
			// 先頭および途中のスペースは許されない。
			// 構成要素 mm, ss, frac の一つ以上は，明記されない構成要素の右側の何れの構成要素も同様に明記されない場合は，明記されないことがある。
			// そのフォーマットが ANSI HISPP MSDS 時間共通データタイプに適合するために，frac は，小数第６位またはそれ以下を持つ。
			// 例：１． "070907.0705"は7時9分7.0705秒の時間を表す。
			//     ２． "1010"は，10時と10分の時間を表す。
			//     ３． "021"は違反の値である。
			// 注：１．V 3.0 より前のこの規格の版との後方互換性のために実装は，このＶＲに書式 hh:mm:ss.frac の文字列をサポートすることを勧められる。
			//     ２．この表のＤＴ ＶＲを同様に参照。
			if( tag.vm != -1 && num_bytes > 16 * tag.vm )
			{
				return( false );
			}
			break;

		case UI:
			// ＵＩ　ＵＩＤ　　　　　　６４バイト以下
			// 項目の広い種類を唯一に識別するために用いられるＵＩＤを含む文字列。
			// ＵＩＤはピリオド "." 文字で分けられた数字構成要素のシリーズである。
			// 一つ以上のＵＩＤを含む値領域の長さが奇数バイト数の場合，値領域が偶数バイトの長さであることを確保するために
			// 一つの末尾のNULL （00H）で埋められる。完全な仕様と例は節９と付属書Ｂを参照。
			if( tag.vm != -1 && num_bytes > 64 * tag.vm )
			{
				return( false );
			}
			break;

		case UL:
			// ＵＬ　符号なし長　　　　４バイト固定
			// 符号無し３２ビット長２進整数。
			// 下記の範囲の整数 n を表す： 0 ≦n＜ 2^32 
			if( tag.vm != -1 && ( num_bytes % 4 != 0 || (int)( num_bytes / 4 ) > tag.vm ) )
			{
				return( false );
			}

			{
				byte_array< unsigned int > tmp = to_current_endian( byte_array< unsigned int >( byte ), true );
				byte[ 0 ] = tmp[ 0 ];
				byte[ 1 ] = tmp[ 1 ];
				byte[ 2 ] = tmp[ 2 ];
				byte[ 3 ] = tmp[ 3 ];
			}
			break;

		case UN:
			// 未知
			// 内容の符号化が不明であるバイトの列。（節６．２．２参照）
			break;

		case US:
			// ＵＳ　符号なし短　　　　２バイト固定
			// 符号無し１６ビット長２進整数。
			// 下記の範囲の整数 n を表す： 0 ≦n＜ 2^16 
			if( tag.vm != -1 && ( num_bytes % 2 != 0 || (int)( num_bytes / 2 ) > tag.vm ) )
			{
				return( false );
			}

			{
				byte_array< unsigned short > tmp = to_current_endian( byte_array< unsigned short >( byte ), true );
				byte[ 0 ] = tmp[ 0 ];
				byte[ 1 ] = tmp[ 1 ];
			}
			break;

		case UT:
			// ＵＴ　無制限テキスト
			// 一以上の段落を含んでいることがある文字列。
			// それは図形文字集合および制御文字 CR, LF, FF および ESC を含むことがある。
			// それは，無視されることがある末尾のスペースで埋められることがある。
			// しかし先頭のスペースは意味があると考えられる。
			// このＶＲをもつデータ要素は複数値ではない，したがって，文字コード 5CH （ISO-IR 6 におけるバックスラッシュ "\"）は使用されることがある。 
			break;

		default:
			return( false );
		}

		return( true );
	}


	inline bool is_sequence_separate_tag( const unsigned char *p, const unsigned char *e )
	{
		if( p + 4 >= e )
		{
			return( false );
		}
		return( p[ 0 ] == 0xfe && p[ 1 ] == 0xff && p[ 2 ] == 0x00 && p[ 3 ] == 0xe0 );
	}

	inline bool is_sequence_element_end( const unsigned char *p, const unsigned char *e )
	{
		if( p + 8 >= e )
		{
			return( false );
		}
		return( p[ 0 ] == 0xfe && p[ 1 ] == 0xff && p[ 2 ] == 0x0d && p[ 3 ] == 0xe0 && p[ 4 ] == 0x00 && p[ 5 ] == 0x00 && p[ 6 ] == 0x00 && p[ 7 ] == 0x00 );
	}

	inline bool is_sequence_tag_end( const unsigned char *p, const unsigned char *e )
	{
		if( p + 8 >= e )
		{
			return( false );
		}
		return( p[ 0 ] == 0xfe && p[ 1 ] == 0xff && p[ 2 ] == 0xdd && p[ 3 ] == 0xe0 && p[ 4 ] == 0x00 && p[ 5 ] == 0x00 && p[ 6 ] == 0x00 && p[ 7 ] == 0x00 );
	}

	inline unsigned char *process_dicom_tag( dicom_tag_container &dicom, unsigned char *pointer, unsigned char *end_pointer )
	{
		difference_type numBytes = 0;
		dicom_tag tag;

		pointer = read_dicom_tag( pointer, end_pointer, tag, numBytes );

		if( tag.vr == SQ )
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
					numBytes = to_current_endian( byte_array< unsigned int >( pointer ), true ).get_value( );
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
							pointer = process_dicom_tag( dicom, pointer, ep );
							if( pointer == NULL )
							{
								return( NULL );
							}
						}
					}
				}
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
							break;
						}
						else
						{
							if( !is_sequence_separate_tag( p, end_pointer ) )
							{
								return( NULL );
							}

							p += 4;
							numBytes = to_current_endian( byte_array< unsigned int >( p ), true ).get_value( );
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
						ite = dicom.append( dicom_element( tag, pointer, numBytes ) );
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
			if( ite != dicom.end( ) )
			{
				ite->second.show_tag( );
			}
#endif
		}
		else if( numBytes > 0 )
		{
			if( !process_dicom_tag( tag, pointer, numBytes ) )
			{
				// 処理することができないDICOMタグを発見したので終了する
				return( NULL );
			}
			else
			{
				dicom_tag_container::iterator ite;
				// データがＶＲの要件を満たす場合はリストに追加
				switch( tag.vr )
				{
				case UI:
					{
						// DICOMの固有IDなので，文字列を変換する
						dicom_uid uid = get_uid( pointer, numBytes );
						ite = dicom.append( dicom_element( tag, reinterpret_cast< const unsigned char * >( uid.name.c_str( ) ), uid.name.size( ) ) );
					}
					break;

				default:
					ite = dicom.append( dicom_element( tag, pointer, numBytes ) );
					break;
				}

				pointer += numBytes;

#ifdef __SHOW_DICOM_TAG__
				if( ite != dicom.end( ) )
				{
					ite->second.show_tag( );
				}
#endif
			}
		}
#ifdef __SHOW_DICOM_TAG__
		else if( tag.vr != UNKNOWN )
		{
			dicom_element( tag, NULL, 0 ).show_tag( );
		}
#endif

		return( pointer );
	}

	inline bool read_dicom_tags( dicom_tag_container &dicom, const std::string &filename )
	{
		size_type filesize;
		FILE *fp;
		if( ( fp = fopen( filename.c_str( ), "rb" ) ) == NULL ) return( false );
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


		unsigned char *end_pointer = buff + filesize;
		pointer = buff;
		// DICOMデータの先頭までポインタを移動
		pointer = check_dicom_file( pointer, end_pointer );

		dicom.clear( );

		bool ret = true;

		while( pointer < end_pointer )
		{
			pointer = process_dicom_tag( dicom, pointer, end_pointer );
			if( pointer == NULL )
			{
				ret = false;
				break;
			}
		}

		delete [] buff;

#ifdef __SHOW_DICOM_TAG__
		printf( "\n\n\n" );
#endif

		return( ret );
	}

}




template < class T, class Allocator >
bool read_dicom( array2< T, Allocator > &image, const std::string &filename )
{
	typedef typename array2< T, Allocator >::value_type value_type;
	typedef typename array2< T, Allocator >::size_type size_type;
	typedef _pixel_converter_< T > pixel_converter;
	typedef typename pixel_converter::color_type color_type;

	dicom_controller::dicom_tag_container dicom;
	dicom_controller::dicom_info info;

	if( !dicom_controller::read_dicom_tags( dicom, filename ) )
	{
		return( false );
	}

	get_dicom_info( dicom, info );

	double window_level = info.window_center;
	double window_width = info.window_width;

	if( dicom.contain( 0x7fe0, 0x0010 ) )
	{
		dicom_controller::dicom_element &element = dicom( 0x7fe0, 0x0010 );
		image.resize( info.cols, info.rows );
		image.reso1( info.pixel_spacing_x );
		image.reso2( info.pixel_spacing_y );
		if( info.bits_allocated == 8 )
		{
			unsigned char *data = element.data;
			double pix;
			unsigned char pixel;
			for( size_type i = 0 ; i < image.size( ) ; i++ )
			{
				pix = byte_array< short >( data[ i ] ).get_value( );
				pix = ( ( pix - window_level ) / window_width + 0.5 ) * 255.0;
				pix = pix > 255.0 ? 255.0 : pix;
				pix = pix <   0.0 ?   0.0 : pix;
				pixel = static_cast< unsigned char >( pix );
				image[ i ] = pixel_converter::convert_to( pixel, pixel, pixel );
			}
		}
		else if( info.bits_allocated == 16 )
		{
			short *data = reinterpret_cast< short * >( element.data );
			double pix;
			unsigned char pixel;
			for( size_type i = 0 ; i < image.size( ) ; i++ )
			{
				pix = byte_array< short >( data[ i ] ).get_value( );
				pix = ( ( pix - window_level ) / window_width + 0.5 ) * 255.0;
				pix = pix > 255.0 ? 255.0 : pix;
				pix = pix <   0.0 ?   0.0 : pix;
				pixel = static_cast< unsigned char >( pix );
				image[ i ] = pixel_converter::convert_to( pixel, pixel, pixel );
			}
		}
		element.release( );
	}
	else
	{
		return( false );
	}

	return( true );
}



// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_DICOM__
