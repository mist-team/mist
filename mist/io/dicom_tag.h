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
//! @brief DICOM�摜��ǂݏ������邽�߂̃��C�u����
//!
#ifndef __INCLUDE_MIST_DICOM_TAG__
#define __INCLUDE_MIST_DICOM_TAG__


#ifndef __INCLUDE_MIST_CONF_H__
#include "../config/mist_conf.h"
#endif

#include <iostream>
#include <string>
#include <set>


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


//! @addtogroup image_group �摜���o��
//!  @{

//! @addtogroup dicom_group DICOM�摜���o��
//! @{


/// DICOM�t�@�C���𑀍삷��֐��E�N���X���܂ޖ��O���
namespace dicom
{
	typedef size_t    size_type;
	typedef ptrdiff_t difference_type;

	#ifdef __INCLUDE_DICOM_TAG_ALL__

		// �Ή����Ă���^�O��S�ăC���N���[�h����
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

		// �ʂɃC���N���[�h����^�O���w�肷��
		// DICOM�̃^�O�̓��C�L���ɂ���^�O�̎�ނ̂ݒ�`����
		// ���p������̂������Ȃ�ɂ�C�R���p�C�����Ԃ���������̂Œ��� 
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

		// DICOM�摜�݂̂�\������̂ł���΁C�ȉ���3�̃}�N���݂̂�L���ɂ���Ηǂ�
		#define __INCLUDE_DICOM_TAG_0002__
		#define __INCLUDE_DICOM_TAG_0028__
		#define __INCLUDE_DICOM_TAG_FFFF__

	#endif


	/// @brief DICOM��VR�^�O�̎��
	enum dicom_vr
	{
		UNKNOWN,	///< @brief �s���Ȃu�q
		AE,			///< @brief ���p�G���e�B�e�B�@�P�U�o�C�g�ȉ�
		AS,			///< @brief �N��@�@�@�@�@�@�@�S�o�C�g�Œ�
		AT,			///< @brief �����^�O�@�@�@�@�@�S�o�C�g�Œ�
		CS,			///< @brief �R�[�h��@�@�@�@�@�P�U�o�C�g�ȉ�
		DA,			///< @brief ���t�@�@�@�@�@�@�@�W�o�C�g�Œ�
		DS,			///< @brief 10�i���i������j�@�P�U�o�C�g�ȉ�
		DT,			///< @brief �����@�@�@�@�@�@�@�Q�U�o�C�g�ȉ�
		FL,			///< @brief �S�o�C�g�����@�@�@�S�o�C�g�Œ�
		FD,			///< @brief �W�o�C�g�����@�@�@�W�o�C�g�Œ�
		IS,			///< @brief �����i������j�@�@�P�U�o�C�g�ȉ�
		LO,			///< @brief ����@�@�@�@�@�@�@�U�S�o�C�g�ȉ�
		LT,			///< @brief �e�L�X�g�@�@�@�@�@�P�O�Q�S�o�C�g�ȉ�
		OB,			///< @brief �o�C�g��@�@�@�@�@������
		OF,			///< @brief ���������_������@232-4 maximum
		OW,			///< @brief ���[�h��@�@�@�@�@������
		PN,			///< @brief ���Җ��@�@�@�@�@�@�U�S�o�C�g�ȉ�
		SH,			///< @brief �Z��@�@�@�@�@�@�@�P�U�o�C�g�ȉ�
		SL,			///< @brief ���������@�@�@�@�S�o�C�g�Œ�
		SQ,			///< @brief ���ڃV�[�P���X�@�@������
		SS,			///< @brief �������Z�@�@�@�@�Q�o�C�g�Œ�
		ST,			///< @brief �Z�e�L�X�g�@�@�@�@�P�O�Q�S�o�C�g�ȉ�
		TM,			///< @brief ���ԁ@�@�@�@�@�@�@�P�U�o�C�g�ȉ�
		UI,			///< @brief �t�h�c�@�@�@�@�@�@�U�S�o�C�g�ȉ�
		UL,			///< @brief �����Ȃ����@�@�@�@�S�o�C�g�Œ�
		UN,			///< @brief ���m�@�@�@�@�@�@�@�s��
		US,			///< @brief �����Ȃ��Z�@�@�@�@�Q�o�C�g�Œ�
		UT,			///< @brief �������e�L�X�g
	};


	/// @brief (group, element) ��DICOM�^�O���ʎq���쐬����
	inline unsigned int construct_dicom_tag( unsigned short group, unsigned short element )
	{
		return( ( ( (unsigned int)group << 16 ) & 0xffff0000 ) + ( (unsigned int)element & 0x0000ffff ) );
	}


	/// @brief (group, element) ��DICOM�^�O���ʎq���쐬����
	inline unsigned short get_dicom_group( unsigned int group_element )
	{
		return( static_cast< unsigned short >( 0x0000ffff & ( group_element >> 16 ) ) );
	}

	/// @brief (group, element) ��DICOM�^�O���ʎq���쐬����
	inline unsigned short get_dicom_element( unsigned int group_element )
	{
		return( static_cast< unsigned short >( 0x0000ffff & group_element ) );
	}


	/// @brief �����񂩂�VR�^�O���쐬����
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


	/// @brief VR�^�O�𕶎���ɒ���
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


	/// @brief DICOM�^�O�iDICOM�f�[�^�̓��e��\�����́j
	class dicom_tag
	{
	public:
		unsigned int tag;			///< @brief DICOM�^�OID
		dicom_vr     vr;			///< @brief VR�^�O
		int          vm;			///< @brief VM�^�O
		std::string  comment;		///< @brief �R�����g
		bool         enable;		///< @brief DICOM�^�O��ۑ����邩�ǂ����i�l���Ɋւ���^�O�̓f�t�H���g�� false �j

	public:
		/// @brief �^�OID t�CVR�^�O vvr�CVM�^�O vvm�C�R�����g text ��DICOM�^�O���쐬����
		dicom_tag( unsigned int t = 0xffffffff, dicom_vr vvr = UNKNOWN, int vvm = 1, const std::string &text = "", bool b = true ) : tag( t ), vr( vvr ), vm( vvm ), comment( text ), enable( b ) { }

		/// @brief �^�OID t�CVR�^�O vvr�CVM�^�O vvm�C�R�����g text ��DICOM�^�O���쐬����
		dicom_tag( unsigned int t, const std::string &vvr, int vvm, const std::string &text, bool b ) : tag( t ), vr( get_dicom_vr( vvr ) ), vm( vvm ), comment( text ), enable( b ) { }

		/// @brief ����DICOM�^�O�ŏ���������
		dicom_tag( const dicom_tag &t ) : tag( t.tag ), vr( t.vr ), vm( t.vm ), comment( t.comment ), enable( t.enable ) { }


		/// @brief ����DICOM�^�O��������
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

		/// @brief DICOM�^�O��ID�ő召�֌W�����肷��
		bool operator <( const dicom_tag &t ) const
		{
			return( tag < t.tag );
		}

		/// @brief DICOM�^�O�̃O���[�vID���擾����
		unsigned short get_group( ) const { return( get_dicom_group( tag ) ); }

		/// @brief DICOM�^�O�̃G�������gID���擾����
		unsigned short get_element( ) const { return( get_dicom_element( tag ) ); }
	};


	/// @brief DICOM��UID�������N���X
	class dicom_uid
	{
	public:
		std::string  uid;		///< @brief UID
		std::string  name;		///< @brief UID�̖��O
		std::string  type;		///< @brief UID�̎��

	public:
		/// @brief UID id�C���O name�C�^�C�v type ��UID���쐬����
		dicom_uid( const std::string &id = "", const std::string &n = "", const std::string &t = "" ) : uid( id ), name( n ), type( t ){ }


		/// @brief ����UID��p���ď���������
		dicom_uid( const dicom_uid &id ) : uid( id.uid ), name( id.name ), type( id.type ){ }


		/// @brief ����UID��������
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


		/// @brief UID�Ԃ̑召�֌W��\��
		bool operator <( const dicom_uid &id ) const
		{
			return( uid < id.uid );
		}
	};



	/// @brief MIST�ň������Ƃ��ł���DICOM�^�O�̈ꗗ��o�^����N���X
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


		/// @brief �^�OID t�CVR�^�O vvr�CVM�^�O vvm�C�R�����g text ��DICOM�^�O��}������
		void insert_tag( unsigned short group, unsigned short element, const std::string &vr, int vm, const std::string &comment, bool b = true )
		{
			baseclass::insert( dicom_tag( construct_dicom_tag( group, element ), vr, vm, comment, b ) );
		}


		/// @brief �^�OID t�CVR�^�O vvr�CVM�^�O vvm�C�R�����g text ��DICOM�^�O��}������
		void insert_tag( unsigned short group, unsigned short element, dicom_vr vr, int vm, const std::string &comment, bool b = true )
		{
			baseclass::insert( dicom_tag( construct_dicom_tag( group, element ), vr, vm, comment, b ) );
		}


		/// @brief DICOM�^�O�̔z���}������
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
		/// @brief (group, element) �̃^�O���܂܂�Ă��邩�ǂ����𒲂ׂ�
		bool contain_tag( unsigned short group, unsigned short element ) const
		{
			baseclass::const_iterator ite = baseclass::find( dicom_tag( construct_dicom_tag( group, element ), UNKNOWN, 1, "" ) );
			return( ite != baseclass::end( ) );
		}

		/// @brief (group, element, vr) �̃^�O���擾����
		dicom_tag get_tag( unsigned short group, unsigned short element, const std::string &vr ) const
		{
			return( get_tag( group, element, get_dicom_vr( vr ) ) );
		}

		/// @brief (group, element, vr) �̃^�O���擾����
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

		/// @brief (group, element) �ň�ԍŏ��Ɍ��������^�O���擾����
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


		/// @brief �f�t�H���g�R���X�g���N�^
		//!
		//! �K�v��DICOM�^�O�݂̂��C���|�[�g����D
		//! �ȉ��̃}�N���̂����C1 �ƒ�`����Ă�����݂̂̂�L���ɂ���
		//! 
		//! - __INCLUDE_DICOM_TAG_0000__ �c �O���[�vID�� 0000 �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_0002__ �c �O���[�vID�� 0002 �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_0004__ �c �O���[�vID�� 0004 �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_0008__ �c �O���[�vID�� 0008 �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_0010__ �c �O���[�vID�� 0010 �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_0012__ �c �O���[�vID�� 0012 �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_0018__ �c �O���[�vID�� 0018 �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_0020__ �c �O���[�vID�� 0020 �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_0022__ �c �O���[�vID�� 0022 �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_0028__ �c �O���[�vID�� 0028 �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_0032__ �c �O���[�vID�� 0032 �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_0038__ �c �O���[�vID�� 0038 �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_003A__ �c �O���[�vID�� 003A �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_0040__ �c �O���[�vID�� 0040 �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_0042__ �c �O���[�vID�� 0042 �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_0050__ �c �O���[�vID�� 0050 �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_0054__ �c �O���[�vID�� 0054 �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_0060__ �c �O���[�vID�� 0060 �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_0062__ �c �O���[�vID�� 0062 �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_0064__ �c �O���[�vID�� 0064 �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_0070__ �c �O���[�vID�� 0070 �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_0072__ �c �O���[�vID�� 0072 �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_0088__ �c �O���[�vID�� 0088 �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_0100__ �c �O���[�vID�� 0100 �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_0400__ �c �O���[�vID�� 0400 �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_2000__ �c �O���[�vID�� 2000 �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_2010__ �c �O���[�vID�� 2010 �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_2020__ �c �O���[�vID�� 2020 �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_2030__ �c �O���[�vID�� 2030 �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_2040__ �c �O���[�vID�� 2040 �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_2050__ �c �O���[�vID�� 2050 �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_2100__ �c �O���[�vID�� 2100 �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_2110__ �c �O���[�vID�� 2110 �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_2120__ �c �O���[�vID�� 2120 �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_2130__ �c �O���[�vID�� 2130 �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_2200__ �c �O���[�vID�� 2200 �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_3002__ �c �O���[�vID�� 3002 �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_3004__ �c �O���[�vID�� 3004 �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_3006__ �c �O���[�vID�� 3006 �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_3008__ �c �O���[�vID�� 3008 �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_300A__ �c �O���[�vID�� 300A �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_300C__ �c �O���[�vID�� 300C �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_300E__ �c �O���[�vID�� 300E �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_4000__ �c �O���[�vID�� 4000 �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_4008__ �c �O���[�vID�� 4008 �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_4FFE__ �c �O���[�vID�� 4FFE �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_5000__ �c �O���[�vID�� 5000 �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_5200__ �c �O���[�vID�� 5200 �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_5400__ �c �O���[�vID�� 5400 �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_5600__ �c �O���[�vID�� 5600 �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_6000__ �c �O���[�vID�� 6000 �̃^�O��}������
		//! - __INCLUDE_DICOM_TAG_FFFF__ �c �O���[�vID�� FFFF �̃^�O��}������
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


	/// @brief DICOM������ UID �̈ꗗ���Ǘ�����N���X
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


		/// @brief �^�OID id�C���O n�C�^�C�v t �� UID ��}������
		void insert_uid( const std::string &id = "", const std::string &n = "", const std::string &t = "" )
		{
			baseclass::insert( dicom_uid( id, n, t ) );
		}


		/// @brief dicom_uids �� UID ��}������
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
		/// @brief uid ���܂܂�Ă��邩�ǂ����𒲂ׂ�
		bool contain_uid( const std::string &uid ) const
		{
			baseclass::const_iterator ite = baseclass::find( dicom_uid( uid ) );
			return( ite != baseclass::end( ) );
		}


		/// @brief uid ���擾����
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


		/// @brief �f�t�H���g�̃R���X�g���N�^
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
//  DICOM�摜���o�̓O���[�v�̏I���

/// @}
//  �摜���o�̓O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_DICOM_TAG__
