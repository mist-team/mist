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

/// @file mist/io/dicom_tag.h
//!
//! @brief DICOM画像を読み書きするためのライブラリ
//!
#ifndef __INCLUDE_MIST_DICOM_TAG__
#define __INCLUDE_MIST_DICOM_TAG__


#ifndef __INCLUDE_MIST_CONF_H__
#include "../config/mist_conf.h"
#endif

#include <iostream>
#include <string>
#include <set>


// mist名前空間の始まり
_MIST_BEGIN


//! @addtogroup image_group 画像入出力
//!  @{

//! @addtogroup dicom_group DICOM画像入出力
//! @{


/// DICOMファイルを操作する関数・クラスを含む名前空間
namespace dicom
{
	typedef size_t    size_type;
	typedef ptrdiff_t difference_type;

	#ifdef __INCLUDE_DICOM_TAG_ALL__

		// 対応しているタグを全てインクルードする
		#define __INCLUDE_DICOM_TAG_0000__
		#define __INCLUDE_DICOM_TAG_0002__
		#define __INCLUDE_DICOM_TAG_0004__
		#define __INCLUDE_DICOM_TAG_0008__
		#define __INCLUDE_DICOM_TAG_0010__
		#define __INCLUDE_DICOM_TAG_0012__
		#define __INCLUDE_DICOM_TAG_0018__
		#define __INCLUDE_DICOM_TAG_0020__
		#define __INCLUDE_DICOM_TAG_0022__
		#define __INCLUDE_DICOM_TAG_0028__
		#define __INCLUDE_DICOM_TAG_0032__
		#define __INCLUDE_DICOM_TAG_0038__
		#define __INCLUDE_DICOM_TAG_003A__
		#define __INCLUDE_DICOM_TAG_0040__
		#define __INCLUDE_DICOM_TAG_0042__
		#define __INCLUDE_DICOM_TAG_0044__
		#define __INCLUDE_DICOM_TAG_0050__
		#define __INCLUDE_DICOM_TAG_0054__
		#define __INCLUDE_DICOM_TAG_0060__
		#define __INCLUDE_DICOM_TAG_0062__
		#define __INCLUDE_DICOM_TAG_0064__
		#define __INCLUDE_DICOM_TAG_0070__
		#define __INCLUDE_DICOM_TAG_0072__
		#define __INCLUDE_DICOM_TAG_0074__
		#define __INCLUDE_DICOM_TAG_0088__
		#define __INCLUDE_DICOM_TAG_0100__
		#define __INCLUDE_DICOM_TAG_0400__
		#define __INCLUDE_DICOM_TAG_2000__
		#define __INCLUDE_DICOM_TAG_2010__
		#define __INCLUDE_DICOM_TAG_2020__
		#define __INCLUDE_DICOM_TAG_2030__
		#define __INCLUDE_DICOM_TAG_2040__
		#define __INCLUDE_DICOM_TAG_2050__
		#define __INCLUDE_DICOM_TAG_2100__
		#define __INCLUDE_DICOM_TAG_2110__
		#define __INCLUDE_DICOM_TAG_2120__
		#define __INCLUDE_DICOM_TAG_2130__
		#define __INCLUDE_DICOM_TAG_2200__
		#define __INCLUDE_DICOM_TAG_3002__
		#define __INCLUDE_DICOM_TAG_3004__
		#define __INCLUDE_DICOM_TAG_3006__
		#define __INCLUDE_DICOM_TAG_3008__
		#define __INCLUDE_DICOM_TAG_300A__
		#define __INCLUDE_DICOM_TAG_300C__
		#define __INCLUDE_DICOM_TAG_300E__
		#define __INCLUDE_DICOM_TAG_4000__
		#define __INCLUDE_DICOM_TAG_4008__
		#define __INCLUDE_DICOM_TAG_4FFE__
		#define __INCLUDE_DICOM_TAG_5000__
		#define __INCLUDE_DICOM_TAG_5200__
		#define __INCLUDE_DICOM_TAG_5400__
		#define __INCLUDE_DICOM_TAG_5600__
		#define __INCLUDE_DICOM_TAG_6000__
		#define __INCLUDE_DICOM_TAG_FFFF__

	#else

		// 個別にインクルードするタグを指定する
		// DICOMのタグの内，有効にするタグの種類のみ定義する
		// 利用するものが多くなるにつれ，コンパイル時間が増加するので注意 
		//#define __INCLUDE_DICOM_TAG_0000__
		//#define __INCLUDE_DICOM_TAG_0004__
		//#define __INCLUDE_DICOM_TAG_0008__
		//#define __INCLUDE_DICOM_TAG_0010__
		//#define __INCLUDE_DICOM_TAG_0012__
		//#define __INCLUDE_DICOM_TAG_0018__
		//#define __INCLUDE_DICOM_TAG_0020__
		//#define __INCLUDE_DICOM_TAG_0022__
		//#define __INCLUDE_DICOM_TAG_0032__
		//#define __INCLUDE_DICOM_TAG_0038__
		//#define __INCLUDE_DICOM_TAG_003A__
		//#define __INCLUDE_DICOM_TAG_0040__
		//#define __INCLUDE_DICOM_TAG_0042__
		//#define __INCLUDE_DICOM_TAG_0044__
		//#define __INCLUDE_DICOM_TAG_0050__
		//#define __INCLUDE_DICOM_TAG_0054__
		//#define __INCLUDE_DICOM_TAG_0060__
		//#define __INCLUDE_DICOM_TAG_0062__
		//#define __INCLUDE_DICOM_TAG_0064__
		//#define __INCLUDE_DICOM_TAG_0070__
		//#define __INCLUDE_DICOM_TAG_0072__
		//#define __INCLUDE_DICOM_TAG_0074__
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
		//#define __INCLUDE_DICOM_TAG_2200__
		//#define __INCLUDE_DICOM_TAG_3002__
		//#define __INCLUDE_DICOM_TAG_3004__
		//#define __INCLUDE_DICOM_TAG_3006__
		//#define __INCLUDE_DICOM_TAG_3008__
		//#define __INCLUDE_DICOM_TAG_300A__
		//#define __INCLUDE_DICOM_TAG_300C__
		//#define __INCLUDE_DICOM_TAG_300E__
		//#define __INCLUDE_DICOM_TAG_4000__
		//#define __INCLUDE_DICOM_TAG_4008__
		//#define __INCLUDE_DICOM_TAG_4FFE__
		//#define __INCLUDE_DICOM_TAG_5000__
		//#define __INCLUDE_DICOM_TAG_5200__
		//#define __INCLUDE_DICOM_TAG_5400__
		//#define __INCLUDE_DICOM_TAG_5600__
		//#define __INCLUDE_DICOM_TAG_6000__

		// DICOM画像のみを表示するのであれば，以下の3つのマクロのみを有効にすれば良い
		#define __INCLUDE_DICOM_TAG_0002__
		#define __INCLUDE_DICOM_TAG_0028__
		#define __INCLUDE_DICOM_TAG_FFFF__

	#endif


	/// @brief DICOMのVRタグの種類
	enum dicom_vr
	{
		UNKNOWN,	///< @brief 不明なＶＲ
		AE,			///< @brief 応用エンティティ　１６バイト以下
		AS,			///< @brief 年齢　　　　　　　４バイト固定
		AT,			///< @brief 属性タグ　　　　　４バイト固定
		CS,			///< @brief コード列　　　　　１６バイト以下
		DA,			///< @brief 日付　　　　　　　８バイト固定
		DS,			///< @brief 10進数（文字列）　１６バイト以下
		DT,			///< @brief 日時　　　　　　　２６バイト以下
		FL,			///< @brief ４バイト実数　　　４バイト固定
		FD,			///< @brief ８バイト実数　　　８バイト固定
		IS,			///< @brief 整数（文字列）　　１６バイト以下
		LO,			///< @brief 長列　　　　　　　６４バイト以下
		LT,			///< @brief テキスト　　　　　１０２４バイト以下
		OB,			///< @brief バイト列　　　　　無制限
		OF,			///< @brief 浮動小数点文字列　232-4 maximum
		OW,			///< @brief ワード列　　　　　無制限
		PN,			///< @brief 患者名　　　　　　６４バイト以下
		SH,			///< @brief 短列　　　　　　　１６バイト以下
		SL,			///< @brief 符号つき長　　　　４バイト固定
		SQ,			///< @brief 項目シーケンス　　無制限
		SS,			///< @brief 符号つき短　　　　２バイト固定
		ST,			///< @brief 短テキスト　　　　１０２４バイト以下
		TM,			///< @brief 時間　　　　　　　１６バイト以下
		UI,			///< @brief ＵＩＤ　　　　　　６４バイト以下
		UL,			///< @brief 符号なし長　　　　４バイト固定
		UN,			///< @brief 未知　　　　　　　不明
		US,			///< @brief 符号なし短　　　　２バイト固定
		UT,			///< @brief 無制限テキスト
	};


	/// @brief (group, element) のDICOMタグ識別子を作成する
	inline unsigned int construct_dicom_tag( unsigned short group, unsigned short element )
	{
		return( ( ( (unsigned int)group << 16 ) & 0xffff0000 ) + ( (unsigned int)element & 0x0000ffff ) );
	}


	/// @brief (group, element) のDICOMタグ識別子を作成する
	inline unsigned short get_dicom_group( unsigned int group_element )
	{
		return( static_cast< unsigned short >( 0x0000ffff & ( group_element >> 16 ) ) );
	}

	/// @brief (group, element) のDICOMタグ識別子を作成する
	inline unsigned short get_dicom_element( unsigned int group_element )
	{
		return( static_cast< unsigned short >( 0x0000ffff & group_element ) );
	}


	/// @brief 文字列からVRタグを作成する
	inline dicom_vr get_dicom_vr( const std::string &vr )
	{
		dicom_vr ret = UNKNOWN;

		if( vr == "AE" )
		{
			ret = AE;
		}
		else if( vr == "AS" )
		{
			ret = AS;
		}
		else if( vr == "AT" )
		{
			ret = AT;
		}
		else if( vr == "CS" )
		{
			ret = CS;
		}
		else if( vr == "DA" )
		{
			ret = DA;
		}
		else if( vr == "DS" )
		{
			ret = DS;
		}
		else if( vr == "DT" )
		{
			ret = DT;
		}
		else if( vr == "FL" )
		{
			ret = FL;
		}
		else if( vr == "FD" )
		{
			ret = FD;
		}
		else if( vr == "IS" )
		{
			ret = IS;
		}
		else if( vr == "LO" )
		{
			ret = LO;
		}
		else if( vr == "LT" )
		{
			ret = LT;
		}
		else if( vr == "OB" )
		{
			ret = OB;
		}
		else if( vr == "OF" )
		{
			ret = OF;
		}
		else if( vr == "OW" )
		{
			ret = OW;
		}
		else if( vr == "PN" )
		{
			ret = PN;
		}
		else if( vr == "SH" )
		{
			ret = SH;
		}
		else if( vr == "SL" )
		{
			ret = SL;
		}
		else if( vr == "SQ" )
		{
			ret = SQ;
		}
		else if( vr == "SS" )
		{
			ret = SS;
		}
		else if( vr == "ST" )
		{
			ret = ST;
		}
		else if( vr == "TM" )
		{
			ret = TM;
		}
		else if( vr == "UI" )
		{
			ret = UI;
		}
		else if( vr == "UL" )
		{
			ret = UL;
		}
		else if( vr == "UN" )
		{
			ret = UN;
		}
		else if( vr == "US" )
		{
			ret = US;
		}
		else if( vr == "UT" )
		{
			ret = UT;
		}

		return( ret );
	}


	/// @brief VRタグを文字列に直す
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


	/// @brief DICOMタグ（DICOMデータの内容を表すもの）
	class dicom_tag
	{
	public:
		unsigned int tag;			///< @brief DICOMタグID
		dicom_vr     vr;			///< @brief VRタグ
		int          vm;			///< @brief VMタグ
		std::string  comment;		///< @brief コメント
		bool         enable;		///< @brief DICOMタグを保存するかどうか（個人情報に関するタグはデフォルトで false ）

	public:
		/// @brief タグID t，VRタグ vvr，VMタグ vvm，コメント text のDICOMタグを作成する
		dicom_tag( unsigned int t = 0xffffffff, dicom_vr vvr = UNKNOWN, int vvm = 1, const std::string &text = "", bool b = true ) : tag( t ), vr( vvr ), vm( vvm ), comment( text ), enable( b ) { }

		/// @brief タグID t，VRタグ vvr，VMタグ vvm，コメント text のDICOMタグを作成する
		dicom_tag( unsigned int t, const std::string &vvr, int vvm, const std::string &text, bool b ) : tag( t ), vr( get_dicom_vr( vvr ) ), vm( vvm ), comment( text ), enable( b ) { }

		/// @brief 他のDICOMタグで初期化する
		dicom_tag( const dicom_tag &t ) : tag( t.tag ), vr( t.vr ), vm( t.vm ), comment( t.comment ), enable( t.enable ) { }


		/// @brief 他のDICOMタグを代入する
		const dicom_tag &operator =( const dicom_tag &t )
		{
			if( &t != this )
			{
				tag = t.tag;
				vr = t.vr;
				vm = t.vm;
				comment = t.comment;
				enable = t.enable;
			}
			return( *this );
		}

		/// @brief DICOMタグのIDで大小関係を決定する
		bool operator <( const dicom_tag &t ) const
		{
			return( tag < t.tag );
		}

		/// @brief DICOMタグのグループIDを取得する
		unsigned short get_group( ) const { return( get_dicom_group( tag ) ); }

		/// @brief DICOMタグのエレメントIDを取得する
		unsigned short get_element( ) const { return( get_dicom_element( tag ) ); }
	};


	/// @brief DICOMのUIDを扱うクラス
	class dicom_uid
	{
	public:
		std::string  uid;		///< @brief UID
		std::string  name;		///< @brief UIDの名前
		std::string  type;		///< @brief UIDの種類

	public:
		/// @brief UID id，名前 name，タイプ type のUIDを作成する
		dicom_uid( const std::string &id = "", const std::string &n = "", const std::string &t = "" ) : uid( id ), name( n ), type( t ){ }


		/// @brief 他のUIDを用いて初期化する
		dicom_uid( const dicom_uid &id ) : uid( id.uid ), name( id.name ), type( id.type ){ }


		/// @brief 他のUIDを代入する
		const dicom_uid &operator =( const dicom_uid &id )
		{
			if( &id != this )
			{
				uid = id.uid;
				name = id.name;
				type = id.type;
			}
			return( *this );
		}


		/// @brief UID間の大小関係を表す
		bool operator <( const dicom_uid &id ) const
		{
			return( uid < id.uid );
		}
	};



	/// @brief MISTで扱うことができるDICOMタグの一覧を登録するクラス
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


		/// @brief タグID t，VRタグ vvr，VMタグ vvm，コメント text のDICOMタグを挿入する
		void insert_tag( unsigned short group, unsigned short element, const std::string &vr, int vm, const std::string &comment, bool b = true )
		{
			baseclass::insert( dicom_tag( construct_dicom_tag( group, element ), vr, vm, comment, b ) );
		}


		/// @brief タグID t，VRタグ vvr，VMタグ vvm，コメント text のDICOMタグを挿入する
		void insert_tag( unsigned short group, unsigned short element, dicom_vr vr, int vm, const std::string &comment, bool b = true )
		{
			baseclass::insert( dicom_tag( construct_dicom_tag( group, element ), vr, vm, comment, b ) );
		}


		/// @brief DICOMタグの配列を挿入する
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
		/// @brief (group, element) のタグが含まれているかどうかを調べる
		bool contain_tag( unsigned short group, unsigned short element ) const
		{
			baseclass::const_iterator ite = baseclass::find( dicom_tag( construct_dicom_tag( group, element ), UNKNOWN, 1, "" ) );
			return( ite != baseclass::end( ) );
		}

		/// @brief (group, element, vr) のタグを取得する
		dicom_tag get_tag( unsigned short group, unsigned short element, const std::string &vr ) const
		{
			return( get_tag( group, element, get_dicom_vr( vr ) ) );
		}

		/// @brief (group, element, vr) のタグを取得する
		dicom_tag get_tag( unsigned short group, unsigned short element, dicom_vr vr ) const
		{
			dicom_tag tag( construct_dicom_tag( group, element ), UNKNOWN, 1, "" );
			baseclass::const_iterator ite = baseclass::find( tag );
			if( ite == baseclass::end( ) )
			{
				return( dicom_tag( 0xffffffff, UNKNOWN, 1, "no tag exists." ) );
			}

			baseclass::const_iterator upper_ite = baseclass::upper_bound( tag );
			baseclass::const_iterator iite = ite;
			do
			{
				if( iite->vr == vr ) return( *iite );
				iite++;
			} while( iite != upper_ite );
			return( *ite );
		}

		/// @brief (group, element) で一番最初に見つかったタグを取得する
		dicom_tag get_tag( unsigned short group, unsigned short element ) const
		{
			dicom_tag tag( construct_dicom_tag( group, element ), UNKNOWN, 1, "" );
			baseclass::const_iterator ite = baseclass::find( tag );
			if( ite == baseclass::end( ) )
			{
				return( dicom_tag( 0xffffffff, UNKNOWN, 1, "no tag exists." ) );
			}
			return( *ite );
		}


		/// @brief デフォルトコンストラクタ
		//!
		//! 必要なDICOMタグのみをインポートする．
		//! 以下のマクロのうち，1 と定義されているもののみを有効にする
		//! 
		//! - __INCLUDE_DICOM_TAG_0000__ … グループIDが 0000 のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_0002__ … グループIDが 0002 のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_0004__ … グループIDが 0004 のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_0008__ … グループIDが 0008 のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_0010__ … グループIDが 0010 のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_0012__ … グループIDが 0012 のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_0018__ … グループIDが 0018 のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_0020__ … グループIDが 0020 のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_0022__ … グループIDが 0022 のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_0028__ … グループIDが 0028 のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_0032__ … グループIDが 0032 のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_0038__ … グループIDが 0038 のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_003A__ … グループIDが 003A のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_0040__ … グループIDが 0040 のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_0042__ … グループIDが 0042 のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_0050__ … グループIDが 0050 のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_0054__ … グループIDが 0054 のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_0060__ … グループIDが 0060 のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_0062__ … グループIDが 0062 のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_0064__ … グループIDが 0064 のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_0070__ … グループIDが 0070 のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_0072__ … グループIDが 0072 のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_0088__ … グループIDが 0088 のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_0100__ … グループIDが 0100 のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_0400__ … グループIDが 0400 のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_2000__ … グループIDが 2000 のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_2010__ … グループIDが 2010 のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_2020__ … グループIDが 2020 のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_2030__ … グループIDが 2030 のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_2040__ … グループIDが 2040 のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_2050__ … グループIDが 2050 のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_2100__ … グループIDが 2100 のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_2110__ … グループIDが 2110 のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_2120__ … グループIDが 2120 のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_2130__ … グループIDが 2130 のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_2200__ … グループIDが 2200 のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_3002__ … グループIDが 3002 のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_3004__ … グループIDが 3004 のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_3006__ … グループIDが 3006 のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_3008__ … グループIDが 3008 のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_300A__ … グループIDが 300A のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_300C__ … グループIDが 300C のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_300E__ … グループIDが 300E のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_4000__ … グループIDが 4000 のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_4008__ … グループIDが 4008 のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_4FFE__ … グループIDが 4FFE のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_5000__ … グループIDが 5000 のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_5200__ … グループIDが 5200 のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_5400__ … グループIDが 5400 のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_5600__ … グループIDが 5600 のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_6000__ … グループIDが 6000 のタグを挿入する
		//! - __INCLUDE_DICOM_TAG_FFFF__ … グループIDが FFFF のタグを挿入する
		//!
		dicom_tag_table( )
		{
#ifdef __INCLUDE_DICOM_TAG_0000__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag0000.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_0002__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag0002.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_0004__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag0004.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_0008__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag0008.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_0010__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag0010.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_0012__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag0012.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_0018__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag0018.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_0020__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag0020.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_0022__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag0022.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_0028__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag0028.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_0032__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag0032.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_0038__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag0038.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_003A__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag003A.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_0040__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag0040.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_0042__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag0042.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_0044__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag0044.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_0050__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag0050.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_0054__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag0054.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_0060__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag0060.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_0062__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag0062.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_0064__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag0064.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_0070__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag0070.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_0072__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag0072.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_0074__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag0074.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_0088__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag0088.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_0100__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag0100.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_0400__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag0400.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_2000__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag2000.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_2010__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag2010.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_2020__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag2020.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_2030__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag2030.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_2040__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag2040.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_2050__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag2050.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_2100__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag2100.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_2110__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag2110.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_2120__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag2120.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_2130__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag2130.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_2200__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag2200.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_3002__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag3002.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_3004__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag3004.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_3006__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag3006.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_3008__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag3008.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_300A__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag300A.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_300C__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag300C.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_300E__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag300E.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_4000__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag4000.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_4008__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag4008.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_4FFE__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag4FFE.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_5000__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag5000.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_5200__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag5200.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_5400__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag5400.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_5600__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag5600.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_6000__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tag6000.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
#ifdef __INCLUDE_DICOM_TAG_FFFF__
			{
				dicom_tag_element dicom_tag_elements[] = {
					#include "dicom_tag/dicom_tagFFFF.csv"
				};
				insert_tag( dicom_tag_elements );
			}
#endif
		}
	};


	/// @brief DICOMが扱う UID の一覧を管理するクラス
	class dicom_uid_table : public std::set< dicom_uid >
	{
	protected:
		typedef std::set< dicom_uid > baseclass;
		typedef struct
		{
			unsigned char  dmy;
			const char *   uid;
			const char *   name;
			const char *   type;
		} dicom_uid_element;


		/// @brief タグID id，名前 n，タイプ t の UID を挿入する
		void insert_uid( const std::string &id = "", const std::string &n = "", const std::string &t = "" )
		{
			baseclass::insert( dicom_uid( id, n, t ) );
		}


		/// @brief dicom_uids の UID を挿入する
		void insert_uid( const dicom_uid_element *dicom_uids )
		{
			const dicom_uid_element *uid = dicom_uids;
			while( !( uid->uid == NULL && uid->name == NULL && uid->type == NULL ) )
			{
				insert_uid( uid->uid, uid->name, uid->type );
				uid++;
			}
		}

	public:
		/// @brief uid が含まれているかどうかを調べる
		bool contain_uid( const std::string &uid ) const
		{
			baseclass::const_iterator ite = baseclass::find( dicom_uid( uid ) );
			return( ite != baseclass::end( ) );
		}


		/// @brief uid を取得する
		dicom_uid get_uid( const std::string &uid )
		{
			dicom_uid id( uid );
			baseclass::iterator ite = baseclass::find( id );
			if( ite == baseclass::end( ) )
			{
				return( dicom_uid( uid, uid, "no uid exists." ) );
			}
			return( *ite );
		}


		/// @brief デフォルトのコンストラクタ
		dicom_uid_table( )
		{
			dicom_uid_element dicom_uid_elements[] = {
				#include "dicom_tag/dicom_uid.csv"
			};
			insert_uid( dicom_uid_elements );
		}
	};
}


/// @}
//  DICOM画像入出力グループの終わり

/// @}
//  画像入出力グループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_DICOM_TAG__
