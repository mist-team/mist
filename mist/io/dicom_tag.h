#ifndef __INCLUDE_MIST_DICOM_TAG__
#define __INCLUDE_MIST_DICOM_TAG__


#ifndef __INCLUDE_MIST_CONF_H__
#include "mist_conf.h"
#endif

#include <iostream>
#include <string>
#include <set>


// mist名前空間の始まり
_MIST_BEGIN


namespace __dicom_controller__
{

	// DICOMのタグの内，有効にするタグの種類のみ定義する
	// 利用するものが多くなるにつれ，コンパイル時間が増加するので注意 
	//#define __INCLUDE_DICOM_TAG_0000__
	//#define __INCLUDE_DICOM_TAG_0002__
	//#define __INCLUDE_DICOM_TAG_0004__
	//#define __INCLUDE_DICOM_TAG_0008__
	//#define __INCLUDE_DICOM_TAG_0010__
	//#define __INCLUDE_DICOM_TAG_0012__
	//#define __INCLUDE_DICOM_TAG_0018__
	//#define __INCLUDE_DICOM_TAG_0020__
	#define __INCLUDE_DICOM_TAG_0028__
	//#define __INCLUDE_DICOM_TAG_0032__
	//#define __INCLUDE_DICOM_TAG_0038__
	//#define __INCLUDE_DICOM_TAG_003A__
	//#define __INCLUDE_DICOM_TAG_0040__
	//#define __INCLUDE_DICOM_TAG_0050__
	//#define __INCLUDE_DICOM_TAG_0054__
	//#define __INCLUDE_DICOM_TAG_0060__
	//#define __INCLUDE_DICOM_TAG_0070__
	//#define __INCLUDE_DICOM_TAG_0088__
	//#define __INCLUDE_DICOM_TAG_0100__
	//#define __INCLUDE_DICOM_TAG_0400__
	//#define __INCLUDE_DICOM_TAG_2000__
	//#define __INCLUDE_DICOM_TAG_2010__
	//#define __INCLUDE_DICOM_TAG_2020__
	//#define __INCLUDE_DICOM_TAG_2030__
	//#define __INCLUDE_DICOM_TAG_2040__
	//#define __INCLUDE_DICOM_TAG_2050__
	//#define __INCLUDE_DICOM_TAG_2100__
	//#define __INCLUDE_DICOM_TAG_2110__
	//#define __INCLUDE_DICOM_TAG_2120__
	//#define __INCLUDE_DICOM_TAG_2130__
	//#define __INCLUDE_DICOM_TAG_3002__
	//#define __INCLUDE_DICOM_TAG_3004__
	//#define __INCLUDE_DICOM_TAG_3006__
	//#define __INCLUDE_DICOM_TAG_3008__
	//#define __INCLUDE_DICOM_TAG_300A__
	//#define __INCLUDE_DICOM_TAG_300C__
	//#define __INCLUDE_DICOM_TAG_300E__
	//#define __INCLUDE_DICOM_TAG_4000__
	//#define __INCLUDE_DICOM_TAG_4008__
	//#define __INCLUDE_DICOM_TAG_5000__
	//#define __INCLUDE_DICOM_TAG_5200__
	//#define __INCLUDE_DICOM_TAG_5400__
	//#define __INCLUDE_DICOM_TAG_5600__
	//#define __INCLUDE_DICOM_TAG_6000__
	#define __INCLUDE_DICOM_TAG_FFFF__


	enum dicom_vr
	{
		UNKNOWN,	// 不明なＶＲ
		AE,			// 応用エンティティ　１６バイト以下
		AS,			// 年齢　　　　　　　４バイト固定
		AT,			// 属性タグ　　　　　４バイト固定
		CS,			// コード列　　　　　１６バイト以下
		DA,			// 日付　　　　　　　８バイト固定
		DS,			// 10進数（文字列）　１６バイト以下
		DT,			// 日時　　　　　　　２６バイト以下
		FL,			// ４バイト実数　　　４バイト固定
		FD,			// ８バイト実数　　　８バイト固定
		IS,			// 整数（文字列）　　１６バイト以下
		LO,			// 長列　　　　　　　６４バイト以下
		LT,			// テキスト　　　　　１０２４バイト以下
		OB,			// バイト列　　　　　無制限
		OF,			// 浮動小数点文字列　232-4 maximum
		OW,			// ワード列　　　　　無制限
		PN,			// 患者名　　　　　　６４バイト以下
		SH,			// 短列　　　　　　　１６バイト以下
		SL,			// 符号つき長　　　　４バイト固定
		SQ,			// 項目シーケンス　　無制限
		SS,			// 符号つき短　　　　２バイト固定
		ST,			// 短テキスト　　　　１０２４バイト以下
		TM,			// 時間　　　　　　　１６バイト以下
		UI,			// ＵＩＤ　　　　　　６４バイト以下
		UL,			// 符号なし長　　　　４バイト固定
		UN,			// 未知　　　　　　　不明
		US,			// 符号なし短　　　　２バイト固定
		UT,			// 無制限テキスト
	};


	inline unsigned int construct_dicom_tag( unsigned short group, unsigned short element )
	{
		return( ( ( (unsigned int)group << 16 ) & 0xffff0000 ) + ( (unsigned int)element & 0x0000ffff ) );
	}


	inline dicom_vr get_dicom_vr( const std::string &vr )
	{
		dicom_vr ret = UNKNOWN;

		if( vr == "AE" )
		{
			// ＡＥ　応用エンティティ　１６バイト以下
			// 意味のない先頭と末尾のSPACE（20H）を持つ文字列。
			// “指定された応用名がない”ことを意味する１６のスペ－スでつくられる値は，使用しない
			ret = AE;
		}
		else if( vr == "AS" )
		{
			// ＡＳ　年齢　　　　　　　４バイト固定
			// 次の書式の一つをもつ文字列 - nnnD, nnnW, nnnM, nnnY；
			// ここで nnn は D に対しては日，W に対しては週，M に対しては月，Y に対しては年の数を含む。
			// 例： "018M" は 18 月の年齢を表す。
			ret = AS;
		}
		else if( vr == "AT" )
		{
			// ＡＴ　属性タグ　　　　　４バイト固定
			// データ要素タグの値である１６ビット符号なし整数の順序付けられた対。
			// 例： (0018,00FF) のデータ要素タグは，４バイトのシリーズとして
			// リトルエンディアン転送構文では 18H, 00H, FFH, 00H として，
			// ビッグエンディアン転送構文では 00H, 18H, 00H, FFH として符号化されるであろう。
			// 注：ＡＴ値の符号化は節７の中で定義されるデータ要素タグの符号化と正確に同一である。
			ret = AT;
		}
		else if( vr == "CS" )
		{
			// ＣＳ　コード列　　　　　１６バイト以下
			// 意味のない先頭または末尾のスペ－ス（20H）をもつ文字列。
			ret = CS;
		}
		else if( vr == "DA" )
		{
			// ＤＡ　日付　　　　　　　８バイト固定
			// 書式 yyyymmdd の文字列；
			// ここで yyyy は年，mm は月，dd は日を含む。
			// これはANSI HISPP MSDS Date common data typeに適合する。
			// 例："19930822"は 1993 年 8 月 22 日を表す。
			// 注：１．V 3.0 より前のこの規格の版との後方互換性のために，
			// 実装は，このＶＲに対して形式yyyy.mm.dd の文字列を同様にサポートすることを推奨される。
			ret = DA;
		}
		else if( vr == "DS" )
		{
			// ＤＳ　10進数（文字列）　１６バイト以下
			// 固定小数点か浮動小数点数を表現する文字列。
			// 固定小数点数は文字 0-9，任意の先頭の "+" または "-"，および小数点を示す任意の "." のみを含む。
			// 浮動小数点数は，ANSI X3.9 の中で定義されるとおり，指数の始まりを示す "E" か "e" を持って伝達される。
			// 10 進数列は先頭あるいは末尾スペースで充てんされることがある。
			// 途中のスペースは許されない。
			ret = DS;
		}
		else if( vr == "DT" )
		{
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
			ret = DT;
		}
		else if( vr == "FL" )
		{
			// ＦＬ　４バイト実数　　　４バイト固定
			// 単精度の２進浮動小数点の数字で，IEEE 754: 1985, 32 ビット浮動小数点数形式で表現される。
			ret = FL;
		}
		else if( vr == "FD" )
		{
			// ＦＤ　８バイト実質　　　８バイト固定
			// 倍精度の２進浮動小数点の数字で，IEEE 754: 1985, 64 ビット浮動小数点数形式で表現される。
			ret = FD;
		}
		else if( vr == "IS" )
		{
			// ＩＳ　整数（文字列）　　１６バイト以下
			// 10 を底とする整数（10 進数）を表わす文字列で，
			// 任意選択の先頭の "+", "-" をもつ文字 0-9 のみを含む。
			// これは先頭そして末尾のスペ－スで埋められることがある。
			// 途中のスペ－スは許されない。表現される整数 n は，下記の範囲である。
			// -231 ≦ n ≦ (231-1) 
			ret = IS;
		}
		else if( vr == "LO" )
		{
			// ＬＯ　長列　　　　　　　６４バイト以下
			// 先頭および／または末尾のスペ－スで埋められることがある文字列。
			// 文字符号 5CH （ISO-IR 6 の中のバックスラッシュ "\" ）は，
			// 複数値デ－タ要素の中の値の間の区切り記号として使用されるので，存在しない。
			// この列は， ESC を除き，制御文字を持たない。
			ret = LO;
		}
		else if( vr == "LT" )
		{
			// ＬＴ　テキスト　　　　　１０２４バイト以下
			// 一つ以上の段落を含むことがある文字列。
			// 図形文字集合と制御文字 CR, LF, FF, および ESC を含むことがある。
			// 無視されることがある末尾のスペース文字で埋められることがある，
			// しかし先頭のスペース文字は意味があると考えられる。
			// このＶＲを持つデ－タ要素は複数値ではない，
			// 従って文字符号 5CH （ISO-IR 6 の中のバックスラッシュ "\" ）は使用されることがある。
			ret = LT;
		}
		else if( vr == "OB" )
		{
			// ＯＢ　バイト列　　　　　無制限
			// 内容の符号化が折衝転送構文によって指定されるバイト列。
			// ＯＢはリトルエンディアン／ビッグエンディアンバイト順に影響されないＶＲである。
			// バイト列は偶数長にするために必要なとき，単一の末尾のNULLバイト値（00H）で埋められる。
			ret = OB;
		}
		else if( vr == "OF" )
		{
			// ＯＦ　浮動小数点文字列　　　　　無制限
			// A string of 32-bit IEEE 754:1985 floating point
			// words. OF is a VR which requires byte
			// swapping within each 32-bit word when
			// changing between Little Endian and Big
			// Endian byte ordering (see Section 7.3).
			ret = OF;
		}
		else if( vr == "OW" )
		{
			// ＯＷ　ワード列　　　　　無制限
			// 内容の符号化が折衝転送構文によって指定された１６ビットワード列。
			// ＯＷはリトルエンディアンおよびビッグエンディアンバイト順の間で変更するとき，
			// 各ワード内でバイトスワッピングを必要とするＶＲである。
			ret = OW;
		}
		else if( vr == "PN" )
		{
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
			ret = PN;
		}
		else if( vr == "SH" )
		{
			// ＳＨ　短列　　　　　　　１６バイト以下
			// 先頭および／または末尾のスペ－スで埋められることがある文字列。
			// 文字符号 5CH （ISO-IR 6 の中のバックスラッシュ "\" ）は，複数値デ－タ要素のための値の間の区切り記号として使用されるので，存在しない。
			// この列は ESC を除き制御文字を持たない。
			ret = SH;
		}
		else if( vr == "SL" )
		{
			// ＳＬ　符号つき長　　　　４バイト固定
			// ２の補数形式の３２ビット長符号付き２進整数。
			// 次の範囲の整数 n を表す： -2^31 ≦ n ≦ (2^31-1） 
			ret = SL;
		}
		else if( vr == "SQ" )
		{
			// ＳＱ　項目シーケンス　　無制限
			// 値は，節７．５で定義される１つ以上の項目のシーケンスである。
			ret = SQ;
		}
		else if( vr == "SS" )
		{
			// ＳＳ　符号つき短　　　　２バイト固定
			// ２の補数形式の１６ビット長符号付き２進整。
			// 範囲 -2^15 ≦n≦ (2^15-1) の中の整数 n を表す。
			ret = SS;
		}
		else if( vr == "ST" )
		{
			// ＳＴ　短テキスト　　　　１０２４バイト以下
			// 一つ以上の段落を含むことがある文字列。
			// 図形文字集合と制御文字 CR, LF, FF, および ESC を含むことがある。
			// 無視されることがある末尾のスペースで埋められることがある，しかし先頭のスペースは意味があると考えられる。
			// このＶＲを持つデ－タ要素は，複数値ではない，従って文字符号 5CH （ISO-IR 6 の中のバックスラッシュ "\" ）は使用されることがある。
			ret = ST;
		}
		else if( vr == "TM" )
		{
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
			ret = TM;
		}
		else if( vr == "UI" )
		{
			// ＵＩ　ＵＩＤ　　　　　　６４バイト以下
			// 項目の広い種類を唯一に識別するために用いられるＵＩＤを含む文字列。
			// ＵＩＤはピリオド "." 文字で分けられた数字構成要素のシリーズである。
			// 一つ以上のＵＩＤを含む値領域の長さが奇数バイト数の場合，値領域が偶数バイトの長さであることを確保するために
			// 一つの末尾のNULL （00H）で埋められる。完全な仕様と例は節９と付属書Ｂを参照。
			ret = UI;
		}
		else if( vr == "UL" )
		{
			// ＵＬ　符号なし長　　　　４バイト固定
			// 符号無し３２ビット長２進整数。
			// 下記の範囲の整数 n を表す： 0 ≦n＜ 2^32 
			ret = UL;
		}
		else if( vr == "UN" )
		{
			// 未知
			// 内容の符号化が不明であるバイトの列。（節６．２．２参照）
			ret = UN;
		}
		else if( vr == "US" )
		{
			// ＵＳ　符号なし短　　　　２バイト固定
			// 符号無し１６ビット長２進整数。
			// 下記の範囲の整数 n を表す： 0 ≦n＜ 2^16 
			ret = US;
		}
		else if( vr == "UT" )
		{
			// ＵＴ　無制限テキスト
			// 一以上の段落を含んでいることがある文字列。
			// それは図形文字集合および制御文字 CR, LF, FF および ESC を含むことがある。
			// それは，無視されることがある末尾のスペースで埋められることがある。
			// しかし先頭のスペースは意味があると考えられる。
			// このＶＲをもつデータ要素は複数値ではない，したがって，文字コード 5CH （ISO-IR 6 におけるバックスラッシュ "\"）は使用されることがある。 
			ret = UT;
		}

		return( ret );
	}

	inline std::string get_dicom_vr( const dicom_vr &vr )
	{
		std::string ret = "UNKNOWN";
		switch( vr )
		{
		case AE:
			ret = "AE";
			break;
		case AS:
			ret = "AS";
			break;
		case AT:
			ret = "AT";
			break;
		case CS:
			ret = "CS";
			break;
		case DA:
			ret = "DA";
			break;
		case DS:
			ret = "DS";
			break;
		case DT:
			ret = "DT";
			break;
		case FL:
			ret = "FL";
			break;
		case FD:
			ret = "FD";
			break;
		case IS:
			ret = "IS";
			break;
		case LO:
			ret = "LO";
			break;
		case LT:
			ret = "LT";
			break;
		case OB:
			ret = "OB";
			break;
		case OW:
			ret = "OW";
			break;
		case PN:
			ret = "PN";
			break;
		case SH:
			ret = "SH";
			break;
		case SL:
			ret = "SL";
			break;
		case SQ:
			ret = "SQ";
			break;
		case SS:
			ret = "SS";
			break;
		case ST:
			ret = "ST";
			break;
		case TM:
			ret = "TM";
			break;
		case UI:
			ret = "UI";
			break;
		case UL:
			ret = "UL";
			break;
		case UN:
			ret = "UN";
			break;
		case US:
			ret = "US";
			break;
		case UT:
			ret = "UT";
			break;
		default:
			break;
		}
		return( ret );
	}


	class dicom_tag
	{
	public:
		unsigned int tag;
		dicom_vr     vr;
		int          vm;
		std::string  comment;

	public:
		dicom_tag( unsigned int t = 0xffffffff, dicom_vr vvr = UNKNOWN, int vvm = 1, const std::string &text = "" )
		{
			tag = t;
			vr = vvr;
			vm = vvm;
			comment = text;
		}

		dicom_tag( unsigned int t, const std::string &vvr, int vvm, const std::string &text )
		{
			tag = t;
			vr = get_dicom_vr( vvr );
			vm = vvm;
			comment = text;
		}

		dicom_tag( const dicom_tag &t )
		{
			tag = t.tag;
			vr = t.vr;
			vm = t.vm;
			comment = t.comment;
		}

		const dicom_tag &operator =( const dicom_tag &t )
		{
			if( &t != this )
			{
				tag = t.tag;
				vr = t.vr;
				vm = t.vm;
				comment = t.comment;
			}
			return( *this );
		}

		bool operator <( const dicom_tag &t ) const
		{
			return( this->tag < t.tag );
		}
	};


	class dicom_tag_table : public std::multiset< dicom_tag >
	{
	protected:
		typedef std::multiset< dicom_tag > baseclass;
		typedef struct
		{
			unsigned char  dmy;
			unsigned short group;
			unsigned short element;
			dicom_vr       vr;
			signed   short vm;
			const char *   comment;
		} dicom_tag_element;

		void insert_tag( unsigned short group, unsigned short element, const std::string &vr, int vm, const std::string &comment )
		{
			baseclass::insert( dicom_tag( construct_dicom_tag( group, element ), vr, vm, comment ) );
		}

		void insert_tag( unsigned short group, unsigned short element, dicom_vr vr, int vm, const std::string &comment )
		{
			baseclass::insert( dicom_tag( construct_dicom_tag( group, element ), vr, vm, comment ) );
		}

		void insert_tag( const dicom_tag_element *dicom_tags )
		{
			const dicom_tag_element *tag = dicom_tags;
			while( !( tag->group == 0x0000 && tag->element == 0x0000 && tag->vr == UNKNOWN ) )
			{
				insert_tag( tag->group, tag->element, tag->vr, tag->vm, tag->comment );
				tag++;
			}
		}

	public:
		bool contain_tag( unsigned short group, unsigned short element )
		{
			baseclass::iterator ite = baseclass::find( dicom_tag( construct_dicom_tag( group, element ), UNKNOWN, 1, "" ) );
			return( ite != baseclass::end( ) );
		}

		dicom_tag get_tag( unsigned short group, unsigned short element, const std::string &vr )
		{
			return( get_tag( group, element, get_dicom_vr( vr ) ) );
		}

		dicom_tag get_tag( unsigned short group, unsigned short element, dicom_vr vr )
		{
			dicom_tag tag( construct_dicom_tag( group, element ), UNKNOWN, 1, "" );
			baseclass::iterator ite = baseclass::find( tag );
			if( ite == baseclass::end( ) )
			{
				return( dicom_tag( 0xffffffff, UNKNOWN, 1, "no tag exists." ) );
			}

			baseclass::iterator upper_ite = baseclass::upper_bound( tag );
			baseclass::iterator iite = ite;
			do
			{
				if( iite->vr == vr ) return( *iite );
				iite++;
			} while( iite != upper_ite );
			return( *ite );
		}

		dicom_tag_table( )
		{
#ifdef __INCLUDE_DICOM_TAG_0000__
			{
				dicom_tag_element dicom_tag_element[] = {
					#include "dicom_tag/dicom_tag0000.csv"
				};
				insert_tag( dicom_tag_element );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_0002__
			{
				dicom_tag_element dicom_tag_element[] = {
					#include "dicom_tag/dicom_tag0002.csv"
				};
				insert_tag( dicom_tag_element );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_0004__
			{
				dicom_tag_element dicom_tag_element[] = {
					#include "dicom_tag/dicom_tag0004.csv"
				};
				insert_tag( dicom_tag_element );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_0000__
			{
				dicom_tag_element dicom_tag_element[] = {
					#include "dicom_tag/dicom_tag0000.csv"
				};
				insert_tag( dicom_tag_element );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_0008__
			{
				dicom_tag_element dicom_tag_element[] = {
					#include "dicom_tag/dicom_tag0008.csv"
				};
				insert_tag( dicom_tag_element );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_0010__
			{
				dicom_tag_element dicom_tag_element[] = {
					#include "dicom_tag/dicom_tag0010.csv"
				};
				insert_tag( dicom_tag_element );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_0012__
			{
				dicom_tag_element dicom_tag_element[] = {
					#include "dicom_tag/dicom_tag0012.csv"
				};
				insert_tag( dicom_tag_element );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_0018__
			{
				dicom_tag_element dicom_tag_element[] = {
					#include "dicom_tag/dicom_tag0018.csv"
				};
				insert_tag( dicom_tag_element );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_0020__
			{
				dicom_tag_element dicom_tag_element[] = {
					#include "dicom_tag/dicom_tag0020.csv"
				};
				insert_tag( dicom_tag_element );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_0028__
			{
				dicom_tag_element dicom_tag_element[] = {
					#include "dicom_tag/dicom_tag0028.csv"
				};
				insert_tag( dicom_tag_element );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_0032__
			{
				dicom_tag_element dicom_tag_element[] = {
					#include "dicom_tag/dicom_tag0032.csv"
				};
				insert_tag( dicom_tag_element );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_0038__
			{
				dicom_tag_element dicom_tag_element[] = {
					#include "dicom_tag/dicom_tag0038.csv"
				};
				insert_tag( dicom_tag_element );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_003A__
			{
				dicom_tag_element dicom_tag_element[] = {
					#include "dicom_tag/dicom_tag003A.csv"
				};
				insert_tag( dicom_tag_element );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_0040__
			{
				dicom_tag_element dicom_tag_element[] = {
					#include "dicom_tag/dicom_tag0040.csv"
				};
				insert_tag( dicom_tag_element );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_0050__
			{
				dicom_tag_element dicom_tag_element[] = {
					#include "dicom_tag/dicom_tag0050.csv"
				};
				insert_tag( dicom_tag_element );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_0060__
			{
				dicom_tag_element dicom_tag_element[] = {
					#include "dicom_tag/dicom_tag0060.csv"
				};
				insert_tag( dicom_tag_element );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_0070__
			{
				dicom_tag_element dicom_tag_element[] = {
					#include "dicom_tag/dicom_tag0070.csv"
				};
				insert_tag( dicom_tag_element );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_0088__
			{
				dicom_tag_element dicom_tag_element[] = {
					#include "dicom_tag/dicom_tag0088.csv"
				};
				insert_tag( dicom_tag_element );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_0100__
			{
				dicom_tag_element dicom_tag_element[] = {
					#include "dicom_tag/dicom_tag0100.csv"
				};
				insert_tag( dicom_tag_element );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_0400__
			{
				dicom_tag_element dicom_tag_element[] = {
					#include "dicom_tag/dicom_tag0400.csv"
				};
				insert_tag( dicom_tag_element );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_2000__
			{
				dicom_tag_element dicom_tag_element[] = {
					#include "dicom_tag/dicom_tag2000.csv"
				};
				insert_tag( dicom_tag_element );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_2010__
			{
				dicom_tag_element dicom_tag_element[] = {
					#include "dicom_tag/dicom_tag2010.csv"
				};
				insert_tag( dicom_tag_element );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_2020__
			{
				dicom_tag_element dicom_tag_element[] = {
					#include "dicom_tag/dicom_tag2020.csv"
				};
				insert_tag( dicom_tag_element );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_2030__
			{
				dicom_tag_element dicom_tag_element[] = {
					#include "dicom_tag/dicom_tag2030.csv"
				};
				insert_tag( dicom_tag_element );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_2040__
			{
				dicom_tag_element dicom_tag_element[] = {
					#include "dicom_tag/dicom_tag2040.csv"
				};
				insert_tag( dicom_tag_element );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_2050__
			{
				dicom_tag_element dicom_tag_element[] = {
					#include "dicom_tag/dicom_tag2050.csv"
				};
				insert_tag( dicom_tag_element );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_2100__
			{
				dicom_tag_element dicom_tag_element[] = {
					#include "dicom_tag/dicom_tag2100.csv"
				};
				insert_tag( dicom_tag_element );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_2110__
			{
				dicom_tag_element dicom_tag_element[] = {
					#include "dicom_tag/dicom_tag2110.csv"
				};
				insert_tag( dicom_tag_element );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_2120__
			{
				dicom_tag_element dicom_tag_element[] = {
					#include "dicom_tag/dicom_tag2120.csv"
				};
				insert_tag( dicom_tag_element );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_2130__
			{
				dicom_tag_element dicom_tag_element[] = {
					#include "dicom_tag/dicom_tag2130.csv"
				};
				insert_tag( dicom_tag_element );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_2110__
			{
				dicom_tag_element dicom_tag_element[] = {
					#include "dicom_tag/dicom_tag2110.csv"
				};
				insert_tag( dicom_tag_element );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_3002__
			{
				dicom_tag_element dicom_tag_element[] = {
					#include "dicom_tag/dicom_tag3002.csv"
				};
				insert_tag( dicom_tag_element );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_3004__
			{
				dicom_tag_element dicom_tag_element[] = {
					#include "dicom_tag/dicom_tag3004.csv"
				};
				insert_tag( dicom_tag_element );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_3006__
			{
				dicom_tag_element dicom_tag_element[] = {
					#include "dicom_tag/dicom_tag3006.csv"
				};
				insert_tag( dicom_tag_element );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_3008__
			{
				dicom_tag_element dicom_tag_element[] = {
					#include "dicom_tag/dicom_tag3008.csv"
				};
				insert_tag( dicom_tag_element );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_300A__
			{
				dicom_tag_element dicom_tag_element[] = {
					#include "dicom_tag/dicom_tag300A.csv"
				};
				insert_tag( dicom_tag_element );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_300C__
			{
				dicom_tag_element dicom_tag_element[] = {
					#include "dicom_tag/dicom_tag300C.csv"
				};
				insert_tag( dicom_tag_element );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_300E__
			{
				dicom_tag_element dicom_tag_element[] = {
					#include "dicom_tag/dicom_tag300E.csv"
				};
				insert_tag( dicom_tag_element );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_4000__
			{
				dicom_tag_element dicom_tag_element[] = {
					#include "dicom_tag/dicom_tag4000.csv"
				};
				insert_tag( dicom_tag_element );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_4008__
			{
				dicom_tag_element dicom_tag_element[] = {
					#include "dicom_tag/dicom_tag4008.csv"
				};
				insert_tag( dicom_tag_element );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_5000__
			{
				dicom_tag_element dicom_tag_element[] = {
					#include "dicom_tag/dicom_tag5000.csv"
				};
				insert_tag( dicom_tag_element );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_5200__
			{
				dicom_tag_element dicom_tag_element[] = {
					#include "dicom_tag/dicom_tag5200.csv"
				};
				insert_tag( dicom_tag_element );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_5400__
			{
				dicom_tag_element dicom_tag_element[] = {
					#include "dicom_tag/dicom_tag5400.csv"
				};
				insert_tag( dicom_tag_element );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_5600__
			{
				dicom_tag_element dicom_tag_element[] = {
					#include "dicom_tag/dicom_tag5600.csv"
				};
				insert_tag( dicom_tag_element );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_6000__
			{
				dicom_tag_element dicom_tag_element[] = {
					#include "dicom_tag/dicom_tag6000.csv"
				};
				insert_tag( dicom_tag_element );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_FFFF__
			{
				dicom_tag_element dicom_tag_element[] = {
					#include "dicom_tag/dicom_tagFFFF.csv"
				};
				insert_tag( dicom_tag_element );
			}
#endif
		}
	};
}


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_DICOM_TAG__
