// 
// Copyright (c) 2003-2006, MIST Project, Intelligent Media Integration COE, Nagoya University
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
//! @brief DICOM�摜��ǂݏ������邽�߂̃��C�u����
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

// �J���[�摜�̐ݒ��ǂݍ���
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

// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


//! @addtogroup image_group �摜���o��
//!  @{

//! @addtogroup dicom_group DICOM�摜���o��
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/io/dicom.h>
//! @endcode
//!
//!  @{


// ���̃}�N�����`����ƁC�R���\�[����ɓǂݍ��񂾃^�O����\������
// #define __SHOW_DICOM_TAG__
// #define __SHOW_DICOM_UNKNOWN_TAG__
// #define __SHOW_DICOM_ZEROBYTE_TAG__
// #define __CHECK_TAG_FORMAT__

// ���̃}�N�����`����ƁC�F���s�\�ȃ^�O�͔r������
// #define __EXCLUDE_DICOM_UNKNOWN_TAG__
// #define __EXCLUDE_DICOM_ZEROBYTE_TAG__


/// DICOM�t�@�C���𑀍삷��֐��E�N���X���܂ޖ��O���
namespace dicom
{
	/// @brief �w�肵���������̈悪DICOM�摜�������Ă��邩�ǂ������`�F�b�N����
	//! 
	//! @attention �������̈�� 132 �o�C�g�ȏ�Ȃ��Ă͂Ȃ�Ȃ�
	//! 
	//! @param[in] p �c �擪�������ʒu
	//! @param[in] e �c �����������ʒu
	//! 
	//! @return DICOM�̃v���A���u���̎����w���|�C���^
	//! 
	inline unsigned char *check_dicom_file( unsigned char *p, unsigned char *e )
	{
		if( p == NULL || p + 128 + 4 >= e )
		{
			return( NULL );
		}


		char *dicm = reinterpret_cast< char * >( p ) + 128;
		if( dicm[ 0 ] == 'D' && dicm[ 1 ] == 'I' && dicm[ 2 ] == 'C' && dicm[ 3 ] == 'M' )
		{
			return( p + 128 + 4 );
		}

		// DICOM�p�̃v���A���u�������݂��Ȃ����߁C�擪�̃|�C���^��Ԃ�
		return( p );
	}


	/// @brief �V�[�P���X�^�O���ǂ����𔻒肷��
	//! 
	//! @param[in] p �c �擪�|�C���^
	//! @param[in] e �c �����|�C���^
	//! 
	//! @retval true  �c �V�[�P���X�^�O�̏ꍇ
	//! @retval false �c ����ȊO
	//! 
	inline bool is_sequence_separate_tag( const unsigned char *p, const unsigned char *e )
	{
		if( p + 4 > e )
		{
			return( false );
		}
		return( p[ 0 ] == 0xfe && p[ 1 ] == 0xff && p[ 2 ] == 0x00 && p[ 3 ] == 0xe0 );
	}

	/// @brief �V�[�P���X�v�f�I���^�O���ǂ���
	//! 
	//! @param[in] p �c �擪�|�C���^
	//! @param[in] e �c �����|�C���^
	//! 
	//! @retval true  �c �V�[�P���X�v�f�I���^�O�̏ꍇ
	//! @retval false �c ����ȊO
	//! 
	inline bool is_sequence_element_end( const unsigned char *p, const unsigned char *e )
	{
		if( p + 8 > e )
		{
			return( false );
		}
		return( p[ 0 ] == 0xfe && p[ 1 ] == 0xff && p[ 2 ] == 0x0d && p[ 3 ] == 0xe0 && p[ 4 ] == 0x00 && p[ 5 ] == 0x00 && p[ 6 ] == 0x00 && p[ 7 ] == 0x00 );
	}

	/// @brief �V�[�P���X�I���^�O���ǂ���
	//! 
	//! @param[in] p �c �擪�|�C���^
	//! @param[in] e �c �����|�C���^
	//! 
	//! @retval true  �c �V�[�P���X�I���^�O�̏ꍇ
	//! @retval false �c ����ȊO
	//! 
	inline bool is_sequence_tag_end( const unsigned char *p, const unsigned char *e )
	{
		if( p + 8 > e )
		{
			return( false );
		}
		return( p[ 0 ] == 0xfe && p[ 1 ] == 0xff && p[ 2 ] == 0xdd && p[ 3 ] == 0xe0 && p[ 4 ] == 0x00 && p[ 5 ] == 0x00 && p[ 6 ] == 0x00 && p[ 7 ] == 0x00 );
	}


	/// @brief DICOM�̃^�O��ǂݍ��݁C�e�[�u���ɓo�^����Ă�����̂Əƍ�����
	//! 
	//! �e�[�u���ɓo�^����Ă��Ȃ��ꍇ�́C�ǂݔ�΂��D
	//! �����C�K�؂�DICOM�t�@�C���łȂ��ꍇ��-2��Ԃ��C�f�[�^�̏I�[�������̓t�@�C���̓ǂݍ��݂Ɏ��s�����ꍇ��-1��Ԃ��D
	//! �����āC�ǂݔ�΂����ꍇ��0��Ԃ��C�e�[�u���ɓo�^����Ă���ꍇ�͎��ɑ��݂���f�[�^�̃o�C�g����Ԃ��D
	//! �f�[�^����������擪�̃|�C���^ 'p' �ƁC�f�[�^�̍Ō�{1�������|�C���^ 'e' ��^����
	//! 
	//! @param[in]  p        �c �擪�������ʒu
	//! @param[in]  e        �c �����������ʒu
	//! @param[out] tag      �c �擾���ꂽDICOM�̃^�O
	//! @param[out] numBytes �c DICOM�^�O���g�p���Ă��郁�����̃o�C�g��
	//! @param[in]  from_little_endian �c ���̓f�[�^�����g���G���f�B�A�����ǂ���
	//! 
	//! @return �^�O�������f�[�^���e���w���擪�|�C���^
	//! 
	inline unsigned char *read_dicom_tag( unsigned char *p, unsigned char *e, dicom_tag &tag, difference_type &numBytes, bool from_little_endian = true )
	{
		if( p == NULL || p + 8 >= e )
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
			//�^�O���e�[�u���ɓo�^����Ă���ꍇ
			if( tag.vr == vr )
			{
				// �����IVR
				switch( vr )
				{
				case OB:
				case OW:
				case UN:
				case SQ:
				case OF:
				case UT:
					data += 4;
					num_bytes = to_current_endian( byte_array< unsigned int >( data ), from_little_endian ).get_value( );
					break;

				default:
					num_bytes = to_current_endian( byte_array< unsigned short >( data + 2 ), from_little_endian ).get_value( );
					break;
				}
			}
			else if( vr != UNKNOWN )
			{
				// �����IVR�ł��邪�C�����ƈقȂ�ꍇ
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
					num_bytes = to_current_endian( byte_array< unsigned int >( data ), from_little_endian ).get_value( );
					break;

				default:
					num_bytes = to_current_endian( byte_array< unsigned short >( data + 2 ), from_little_endian ).get_value( );
				}
			}
			else
			{
				// �Î��IVR
				num_bytes = to_current_endian( byte_array< unsigned int >( data ), from_little_endian ).get_value( );
			}
			numBytes = num_bytes;
			return( reinterpret_cast< unsigned char * >( data + 4 ) );
		}
		else
		{
			//�^�O���e�[�u���ɓo�^����Ă��Ȃ��ꍇ�̓f�[�^�X�L�b�v
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
				{
					num_bytes = to_current_endian( byte_array< unsigned short >( data + 2 ), from_little_endian ).get_value( );
				}
				break;

			case OB:
			case OW:
			case UN:
			case SQ:
			case OF:
			case UT:
				data += 4;
				num_bytes = to_current_endian( byte_array< unsigned int >( data ), from_little_endian ).get_value( );
				break;

			default:
				num_bytes = to_current_endian( byte_array< unsigned int >( data ), from_little_endian ).get_value( );
				break;
			}

#if defined( __SHOW_DICOM_ZEROBYTE_TAG__ ) && defined( __SHOW_DICOM_UNKNOWN_TAG__ )
			printf( "( %04x, %04x, %s, % 8d ) = Unknown Tags!!\n", group, element, VR, num_bytes );
#endif
			if( vr == SQ )
			{
				// �s���ȃ^�O�����ǃV�[�P���X�^�O�Ƃ������Ƃ��m�F
				tag = dicom_tag( construct_dicom_tag( group, element ), SQ, -1, "UNKNOWN" );
				numBytes = num_bytes;
				return( data + 4 );
			}
			else if( data + 4 + 4 <= e && data[ 0 ] == 0xff && data[ 1 ] == 0xff && data[ 2 ] == 0xff && data[ 3 ] == 0xff && is_sequence_separate_tag( data + 4, e ) )
			{
				// �s���ȃ^�O�����ǃV�[�P���X�^�O�̉\��������ꍇ�Ƃ������Ƃ��m�F
				tag = dicom_tag( construct_dicom_tag( group, element ), SQ, -1, "UNKNOWN" );
				numBytes = num_bytes;
				return( data + 4 );
			}
			else if( data + 4 + num_bytes <= e )
			{
				if( element == 0x0000 )
				{
					// Group Length �^�O
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



	/// @brief DICOM�̃^�O�ɑ΂��C�eVR���Ƃ̏������s��
	//! 
	//! @param[in]     tag       �c ��������DICOM�^�O
	//! @param[in,out] byte      �c ���������o�C�g��
	//! @param[in]     num_bytes �c �o�C�g��̒���
	//! @param[in]     from_little_endian �c ���̓f�[�^�����g���G���f�B�A�����ǂ���
	//! 
	//! @return �o�^����Ă��Ȃ��^�O�̏ꍇ�� false ���������C�������������ꂽ�ꍇ�̂� true ��Ԃ�
	//! 
	inline bool process_dicom_tag( const dicom_tag &tag, unsigned char *byte, difference_type num_bytes, bool from_little_endian = true )
	{
		switch( tag.vr )
		{
		case AE:
			// �`�d�@���p�G���e�B�e�B�@�P�U�o�C�g�ȉ�
			// �Ӗ��̂Ȃ��擪�Ɩ�����SPACE�i20H�j����������B
			// �g�w�肳�ꂽ���p�����Ȃ��h���Ƃ��Ӗ�����P�U�̃X�y�|�X�ł�����l�́C�g�p���Ȃ�
			if( tag.vm != -1 && num_bytes > 16 * tag.vm )
			{
				return( false );
			}
			break;

		case AS:
			// �`�r�@�N��@�@�@�@�@�@�@�S�o�C�g�Œ�
			// ���̏����̈���������� - nnnD, nnnW, nnnM, nnnY�G
			// ������ nnn �� D �ɑ΂��Ă͓��CW �ɑ΂��Ă͏T�CM �ɑ΂��Ă͌��CY �ɑ΂��Ă͔N�̐����܂ށB
			// ��F "018M" �� 18 ���̔N���\���B
			if( tag.vm != -1 && ( num_bytes % 4 != 0 || (int)( num_bytes / 4 ) > tag.vm ) )
			{
				return( false );
			}
			break;

		case AT:
			// �`�s�@�����^�O�@�@�@�@�@�S�o�C�g�Œ�
			// �f�[�^�v�f�^�O�̒l�ł���P�U�r�b�g�����Ȃ������̏����t����ꂽ�΁B
			// ��F (0018,00FF) �̃f�[�^�v�f�^�O�́C�S�o�C�g�̃V���[�Y�Ƃ���
			// ���g���G���f�B�A���]���\���ł� 18H, 00H, FFH, 00H �Ƃ��āC
			// �r�b�O�G���f�B�A���]���\���ł� 00H, 18H, 00H, FFH �Ƃ��ĕ����������ł��낤�B
			// ���F�`�s�l�̕������͐߂V�̒��Œ�`�����f�[�^�v�f�^�O�̕������Ɛ��m�ɓ���ł���B
			if( tag.vm != -1 && ( num_bytes % 4 != 0 || (int)( num_bytes / 4 ) > tag.vm ) )
			{
				return( false );
			}
			break;

		case CS:
			// �b�r�@�R�[�h��@�@�@�@�@�P�U�o�C�g�ȉ�
			// �Ӗ��̂Ȃ��擪�܂��͖����̃X�y�|�X�i20H�j����������B
			if( tag.vm != -1 && num_bytes > 16 * tag.vm )
			{
				return( false );
			}
			break;

		case DA:
			// �c�`�@���t�@�@�@�@�@�@�@�W�o�C�g�Œ�
			// ���� yyyymmdd �̕�����G
			// ������ yyyy �͔N�Cmm �͌��Cdd �͓����܂ށB
			// �����ANSI HISPP MSDS Date common data type�ɓK������B
			// ��F"19930822"�� 1993 �N 8 �� 22 ����\���B
			// ���F�P�DV 3.0 ���O�̂��̋K�i�̔łƂ̌���݊����̂��߂ɁC
			// �����́C���̂u�q�ɑ΂��Č`��yyyy.mm.dd �̕�����𓯗l�ɃT�|�[�g���邱�Ƃ𐄏������B
			if( tag.vm != -1 && ( num_bytes % 8 != 0 || (int)( num_bytes / 8 ) > tag.vm ) && ( num_bytes % 10 != 0 || (int)( num_bytes / 10 ) > tag.vm ) )
			{
				return( false );
			}
			break;

		case DS:
			// �c�r�@10�i���i������j�@�P�U�o�C�g�ȉ�
			// �Œ菬���_�����������_����\�����镶����B
			// �Œ菬���_���͕��� 0-9�C�C�ӂ̐擪�� "+" �܂��� "-"�C����я����_�������C�ӂ� "." �݂̂��܂ށB
			// ���������_���́CANSI X3.9 �̒��Œ�`�����Ƃ���C�w���̎n�܂������ "E" �� "e" �������ē`�B�����B
			// 10 �i����͐擪���邢�͖����X�y�[�X�ŏ[�Ă񂳂�邱�Ƃ�����B
			// �r���̃X�y�[�X�͋�����Ȃ��B
			if( tag.vm != -1 && num_bytes > 16 * tag.vm )
			{
				return( false );
			}
			break;

		case DT:
			// �c�s�@�����@�@�@�@�@�@�@�Q�U�o�C�g�ȉ�
			// �������ʃf�[�^�^�C�v�B
			// ���� YYYYMMDDHHMMSS.FFFFFF&ZZZZ �̘A������ ASCII ��������B
			// ���̗�̍\���v�f�́C������E�� YYYY = �N, MM = ��, DD = ��, HH = ����, MM = ��, SS = �b, FFFFFF = �b�̏��������C
			// & = "+"�܂��� "-" �C����� ZZZZ = �I�t�Z�b�g�̎��Ԃƕ��B
			// &ZZZZ �� ���萢�E������̃v���X�^�}�C�i�X�I�t�b�Z�b�g�ɑ΂���C�ӑI���̐ڔ����ł���B
			// �񂩂�Ȃ��ꂽ�\���v�f�͋󔒍\���v�f�ƌĂ΂��B
			// �����̖����̋󔒍\���v�f�͖��������B
			// �����łȂ��󔒍\���v�f�͋֎~�����C�C�ӑI���̐ڔ����͍\���v�f�ƍl�����Ȃ��B
			// ���F	V 3.0 ���O�̂��̋K�i�̔łƂ̌���݊����̂��߂ɁC
			// �����̑��݂���c�h�b�n�l�f�[�^�v�f�͕��������c�`����тs�l�̂u�q���g�p����B
			// �������肳���W������ю��I�f�[�^�v�f�� ANSI HISPP MSDS �ɂ��K�����邽�߂ɉ\�Ȃ�c�s���g�p����B
			if( tag.vm != -1 && num_bytes > 26 * tag.vm )
			{
				return( false );
			}
			break;

		case FL:
			// �e�k�@�S�o�C�g�����@�@�@�S�o�C�g�Œ�
			// �P���x�̂Q�i���������_�̐����ŁCIEEE 754: 1985, 32 �r�b�g���������_���`���ŕ\�������B
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
			// �e�c�@�W�o�C�g�����@�@�@�W�o�C�g�Œ�
			// �{���x�̂Q�i���������_�̐����ŁCIEEE 754: 1985, 64 �r�b�g���������_���`���ŕ\�������B
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
			// �h�r�@�����i������j�@�@�P�U�o�C�g�ȉ�
			// 10 ���Ƃ��鐮���i10 �i���j��\�킷������ŁC
			// �C�ӑI���̐擪�� "+", "-" �������� 0-9 �݂̂��܂ށB
			// ����͐擪�����Ė����̃X�y�|�X�Ŗ��߂��邱�Ƃ�����B
			// �r���̃X�y�|�X�͋�����Ȃ��B�\������鐮�� n �́C���L�͈̔͂ł���B
			// -231 �� n �� (231-1) 
			if( tag.vm != -1 && num_bytes > 16 * tag.vm )
			{
				return( false );
			}
			break;

		case LO:
			// �k�n�@����@�@�@�@�@�@�@�U�S�o�C�g�ȉ�
			// �擪����с^�܂��͖����̃X�y�|�X�Ŗ��߂��邱�Ƃ����镶����B
			// �������� 5CH �iISO-IR 6 �̒��̃o�b�N�X���b�V�� "\" �j�́C
			// �����l�f�|�^�v�f�̒��̒l�̊Ԃ̋�؂�L���Ƃ��Ďg�p�����̂ŁC���݂��Ȃ��B
			// ���̗�́C ESC �������C���䕶���������Ȃ��B
			if( tag.vm != -1 && num_bytes > 64 * tag.vm )
			{
				return( false );
			}
			break;

		case LT:
			// �k�s�@�e�L�X�g�@�@�@�@�@�P�O�Q�S�o�C�g�ȉ�
			// ��ȏ�̒i�����܂ނ��Ƃ����镶����B
			// �}�`�����W���Ɛ��䕶�� CR, LF, FF, ����� ESC ���܂ނ��Ƃ�����B
			// ��������邱�Ƃ����閖���̃X�y�[�X�����Ŗ��߂��邱�Ƃ�����C
			// �������擪�̃X�y�[�X�����͈Ӗ�������ƍl������B
			// ���̂u�q�����f�|�^�v�f�͕����l�ł͂Ȃ��C
			// �]���ĕ������� 5CH �iISO-IR 6 �̒��̃o�b�N�X���b�V�� "\" �j�͎g�p����邱�Ƃ�����B
			if( tag.vm != -1 && num_bytes > 1024 * tag.vm )
			{
				return( false );
			}
			break;

		case OB:
			// �n�a�@�o�C�g��@�@�@�@�@������
			// ���e�̕��������܏Փ]���\���ɂ���Ďw�肳���o�C�g��B
			// �n�a�̓��g���G���f�B�A���^�r�b�O�G���f�B�A���o�C�g���ɉe������Ȃ��u�q�ł���B
			// �o�C�g��͋������ɂ��邽�߂ɕK�v�ȂƂ��C�P��̖�����NULL�o�C�g�l�i00H�j�Ŗ��߂���B
			break;

		case OW:
			// �n�v�@���[�h��@�@�@�@�@������
			// ���e�̕��������܏Փ]���\���ɂ���Ďw�肳�ꂽ�P�U�r�b�g���[�h��B
			// �n�v�̓��g���G���f�B�A������уr�b�O�G���f�B�A���o�C�g���̊ԂŕύX����Ƃ��C
			// �e���[�h���Ńo�C�g�X���b�s���O��K�v�Ƃ���u�q�ł���B
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
			// �o�m�@���Җ��@�@�@�@�@�@�U�S�o�C�g�ȉ�
			// �T�\���v�f�K���p���ĕ���������镶����B
			// �������� 5CH �iISO-IR 6 �̒��̃o�b�N�X���b�V�� "\" �j�́C�����l�f�|�^�v�f�̒��̒l�̊Ԃ̋�؂�L���Ƃ��Ďg�p�����̂ŁC���݂��Ȃ��B
			// ��́C�����X�y�[�X�Ŗ��߂��邱�Ƃ�����B
			// �T�\���v�f�͂��̔������ɁF
			//       family name�i���j�����́C
			//       given name�i���j�����́C
			//       middle name�Cname prefix�i���O�ړ����j�C
			//       name suffix�i���O�ڔ����j�B
			// �T�\���v�f�̉��ꂩ����̗�̂��Ƃ�����B
			// �\���v�f�̋�؂�L���̓L�����b�g���� "^" (5EH) �ł���B
			// ��؂�L���͓������󔒂̍\���v�f�ɂ��K�v�ł���B
			// �����̋󔒍\���v�f�O���[�v����т����̋�؂�L���͏ȗ�����邱�Ƃ�����B
			// �����o�^���C�e�\���v�f�̒��ŋ�����C�����Ď��R����Ƃ��āC���̖��O�̐l�ɂ���čD�܂�鏑���̒��ŕ����������B
			// ����� ANSI HISPP MSDS �l�����ʃf�[�^�^�C�v�ɓK������B
			// ���̂T�\���v�f�̃O���[�v�́C�l���\���v�f�O���[�v�Ƃ��ĎQ�Ƃ����B
			// ���O��\�ӕ�������ѕ\�������ŏ����ړI�ŁC�O�܂ł̍\���v�f�O���[�v���g�p����邱�Ƃ�����i�t�����g��P����ї�Q���Q�Ɓj�B
			// �\���v�f�O���[�v�̂��߂̋�؂�L���́C�������� "=" (3DH) �ł���B�O�̍\���v�f�O���[�v�͏o�����ɁC
			// �P�o�C�g�����\���C�\�ӕ����\���C�\�������\���ł���B
			//
			// �\���v�f�O���[�v�̉�������C�ŏ��̍\���v�f�O���[�v���܂�ŁC�s�݂ł��邱�Ƃ�����B
			// ���̏ꍇ�C�l�̖��O�́C��ȏ�� "=" ��؂�L������n�܂�B
			// ��؂�L���́C�����̗�̍\���v�f�O���[�v�ɑ΂��ĕK�v�ł���B
			// �����̗�̍\���v�f�O���[�v����т��̋�؂�L���́C�ȗ�����邱�Ƃ�����B
			// �ڍׂȈӖ����C�e�\���v�f�O���[�v�̂��߂ɒ�`����Ă���B
			// ��FRev. John Robert Quincy Adams, B.A. M.Div."Adams^John Robert Quincy^^Rev.^B.A. M.Div."
			//      �mfamily name  1�Cgiven name  3�Cmiddle name ���Cname prefix  1�Cname suffix  2�n
			//     Susan Morrison-Jones, Ph.D., Chief Executive Officer"Morrison-Jones^Susan^^^Ph.D., Chief Executive Officer"
			//      �mfamily name  2�Cgiven name  1�Cmiddle name ���Cname prefix ���Cname suffix  2�n
			//     John Doe"Doe^John"
			//      �mfamily name  1�Cgiven name  1�Cmiddle name ���Cname prefix ���Cname suffix ���B
			//           ��؂�L���͎O�̖����̗�\���v�f�ɂ��Ă͏ȗ�����Ă���B�n
			//   �i�����o�C�g�����W�����g�p�����l���̕������̗�ɂ��ẮC�t�����g���Q�ƁB�j
			// ���F	�P�D���̂T�\���v�f�̋K��́C ASTM E-1238-91�̒��Œ�`����ANSI MSDS�ɂ���Ă���ɓ��ꉻ����C HL7 �ɂ���ē��l�Ɏg�p����Ă���B
			//      �Q�D�T�^�I�ȃA�����J�≢�B�ł̎g�p�@�ł�"given name"�̍ŏ��̕�����"first name"��\���B
			//          "given name"�̓�ԖڂƂ���ɑ��������͕���"middle name"�Ƃ��Ĉ�����B
			//          ����"middle name"�\���v�f�́C��������K�i�Ƃ̌���݊����̖ړI�̂��߂Ɏc����Ă���B
			//      �R�DASTM E-1238-91�̒��ɑ��݂���u�w�ʁv�\���v�f��"suffix"�\���v�f�̒��ɋz�����ꂽ�B
			//      �S�D�����҂́C"given name"��"first name"�����"middle name"�Ƃ��ĕ\�����鏉���̎g�p�`���ƁC
			//          �����Ă��̏����̓T�^�I�g�p�@�ւ́C�܂����ꂩ��̕ϊ����K�v�Ƃ���邱�Ƃ����邱�Ƃɗ��ӂ��Ȃ���΂Ȃ�Ȃ��B
			//      �T�D�łR�D�O���O�̂��̋K�i�̔łƂ̌���݊����̗��R�̂��߂ɁC�l���́C�P���family name�i���j������
			//         �i��؂�L�� "^" �Ȃ��̒P��̍\���v�f�j�ƍl�����邱�Ƃ�����B
			if( tag.vm != -1 && num_bytes > 64 * tag.vm )
			{
				return( false );
			}
			break;

		case SH:
			// �r�g�@�Z��@�@�@�@�@�@�@�P�U�o�C�g�ȉ�
			// �擪����с^�܂��͖����̃X�y�|�X�Ŗ��߂��邱�Ƃ����镶����B
			// �������� 5CH �iISO-IR 6 �̒��̃o�b�N�X���b�V�� "\" �j�́C�����l�f�|�^�v�f�̂��߂̒l�̊Ԃ̋�؂�L���Ƃ��Ďg�p�����̂ŁC���݂��Ȃ��B
			// ���̗�� ESC ���������䕶���������Ȃ��B
			if( tag.vm != -1 && num_bytes > 16 * tag.vm )
			{
				return( false );
			}
			break;

		case SL:
			// �r�k�@���������@�@�@�@�S�o�C�g�Œ�
			// �Q�̕␔�`���̂R�Q�r�b�g�������t���Q�i�����B
			// ���͈̔͂̐��� n ��\���F -2^31 �� n �� (2^31-1�j 
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
			// �r�p�@���ڃV�[�P���X�@�@������
			// �l�́C�߂V�D�T�Œ�`�����P�ȏ�̍��ڂ̃V�[�P���X�ł���B
			break;

		case SS:
			// �r�r�@�������Z�@�@�@�@�Q�o�C�g�Œ�
			// �Q�̕␔�`���̂P�U�r�b�g�������t���Q�i���B
			// �͈� -2^15 ��n�� (2^15-1) �̒��̐��� n ��\���B
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
			// �r�s�@�Z�e�L�X�g�@�@�@�@�P�O�Q�S�o�C�g�ȉ�
			// ��ȏ�̒i�����܂ނ��Ƃ����镶����B
			// �}�`�����W���Ɛ��䕶�� CR, LF, FF, ����� ESC ���܂ނ��Ƃ�����B
			// ��������邱�Ƃ����閖���̃X�y�[�X�Ŗ��߂��邱�Ƃ�����C�������擪�̃X�y�[�X�͈Ӗ�������ƍl������B
			// ���̂u�q�����f�|�^�v�f�́C�����l�ł͂Ȃ��C�]���ĕ������� 5CH �iISO-IR 6 �̒��̃o�b�N�X���b�V�� "\" �j�͎g�p����邱�Ƃ�����B
			if( tag.vm != -1 && num_bytes > 1024 * tag.vm )
			{
				return( false );
			}
			break;

		case TM:
			// �s�l�@���ԁ@�@�@�@�@�@�@�P�U�o�C�g�ȉ�
			// ���� hhmmss.frac �̕�����G
			//    hh �͎��Ԃ��܂݁i�͈� "00" - "23"�j�C
			//    mm �͕����܂݁i�͈� "00" - "59"�j�C
			//    ss �͕b���܂݁i�͈� "00" - "59"�j�C
			//    frac �͕b�̂P�����̂P�̒P�ʂ̕b�̕������܂ށi�͈� "000000" - "999999"�j�B
			// �Q�S���Ԏ������p������B�^�钆�� "2400" �͎��Ԃ͈̔͂𒴂��̂ŁC "0000" �݂̂ŕ\���B
			// ��͖����̃X�y�[�X�ŋl�߂��邱�Ƃ�����B
			// �擪����ѓr���̃X�y�[�X�͋�����Ȃ��B
			// �\���v�f mm, ss, frac �̈�ȏ�́C���L����Ȃ��\���v�f�̉E���̉���̍\���v�f�����l�ɖ��L����Ȃ��ꍇ�́C���L����Ȃ����Ƃ�����B
			// ���̃t�H�[�}�b�g�� ANSI HISPP MSDS ���ԋ��ʃf�[�^�^�C�v�ɓK�����邽�߂ɁCfrac �́C������U�ʂ܂��͂���ȉ������B
			// ��F�P�D "070907.0705"��7��9��7.0705�b�̎��Ԃ�\���B
			//     �Q�D "1010"�́C10����10���̎��Ԃ�\���B
			//     �R�D "021"�͈ᔽ�̒l�ł���B
			// ���F�P�DV 3.0 ���O�̂��̋K�i�̔łƂ̌���݊����̂��߂Ɏ����́C���̂u�q�ɏ��� hh:mm:ss.frac �̕�������T�|�[�g���邱�Ƃ����߂���B
			//     �Q�D���̕\�̂c�s �u�q�𓯗l�ɎQ�ƁB
			if( tag.vm != -1 && num_bytes > 16 * tag.vm )
			{
				return( false );
			}
			break;

		case UI:
			// �t�h�@�t�h�c�@�@�@�@�@�@�U�S�o�C�g�ȉ�
			// ���ڂ̍L����ނ�B��Ɏ��ʂ��邽�߂ɗp������t�h�c���܂ޕ�����B
			// �t�h�c�̓s���I�h "." �����ŕ�����ꂽ�����\���v�f�̃V���[�Y�ł���B
			// ��ȏ�̂t�h�c���܂ޒl�̈�̒�������o�C�g���̏ꍇ�C�l�̈悪�����o�C�g�̒����ł��邱�Ƃ��m�ۂ��邽�߂�
			// ��̖�����NULL �i00H�j�Ŗ��߂���B���S�Ȏd�l�Ɨ�͐߂X�ƕt�����a���Q�ƁB
			if( tag.vm != -1 && num_bytes > 64 * tag.vm )
			{
				return( false );
			}
			break;

		case UL:
			// �t�k�@�����Ȃ����@�@�@�@�S�o�C�g�Œ�
			// ���������R�Q�r�b�g���Q�i�����B
			// ���L�͈̔͂̐��� n ��\���F 0 ��n�� 2^32 
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
			// ���m
			// ���e�̕��������s���ł���o�C�g�̗�B�i�߂U�D�Q�D�Q�Q�Ɓj
			break;

		case US:
			// �t�r�@�����Ȃ��Z�@�@�@�@�Q�o�C�g�Œ�
			// ���������P�U�r�b�g���Q�i�����B
			// ���L�͈̔͂̐��� n ��\���F 0 ��n�� 2^16 
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
			// �t�s�@�������e�L�X�g
			// ��ȏ�̒i�����܂�ł��邱�Ƃ����镶����B
			// ����͐}�`�����W������ѐ��䕶�� CR, LF, FF ����� ESC ���܂ނ��Ƃ�����B
			// ����́C��������邱�Ƃ����閖���̃X�y�[�X�Ŗ��߂��邱�Ƃ�����B
			// �������擪�̃X�y�[�X�͈Ӗ�������ƍl������B
			// ���̂u�q�����f�[�^�v�f�͕����l�ł͂Ȃ��C���������āC�����R�[�h 5CH �iISO-IR 6 �ɂ�����o�b�N�X���b�V�� "\"�j�͎g�p����邱�Ƃ�����B 
			break;

		default:
			// VR���킩��Ȃ�����Ƃ肠�����ǂ��Ƃ���
			break;
		}

		return( true );
	}


	/// @brief DICOM�̃^�O����������
	//! 
	//! �V�[�P���X��\��DICOM�^�O�����������ꍇ�́C�ċA�I�ɖ{�֐����K�p�����
	//! 
	//! @param[out] dicm        �c DICOM�f�[�^�̏o�͐�
	//! @param[in]  pointer     �c �擪�|�C���^
	//! @param[in]  end_pointer �c �����擪�|�C���^
	//! @param[in]  from_little_endian �c ���̓f�[�^�����g���G���f�B�A�����ǂ���
	//! @param[in]  is_in_sequence_tag �c VR��SQ���̃^�O�ł��邩�ǂ����iSQ�^�O�͈�ɂ܂Ƃ߂�j
	//! 
	//! @return ���̃^�O���w���|�C���^
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
					// �F���s�\�ȃV�[�P���X�^�O����
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
						numBytes = to_current_endian( byte_array< unsigned int >( pointer ), from_little_endian ).get_value( );
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
			// ���k���������Ă���\��������̂Ń`�F�b�N
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
							numBytes = to_current_endian( byte_array< unsigned int >( p ), from_little_endian ).get_value( );
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
						// ���k�V�[�P���X�̏I�������܂����o�ł��Ȃ�����
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
			// �s�K�؂ȃo�C�g�����ǂ������`�F�b�N����
			if( pointer + numBytes > end_pointer )
			{
				return( NULL );
			}

#ifndef __CHECK_TAG_FORMAT__
			if( !process_dicom_tag( tag, pointer, numBytes, from_little_endian ) )
			{
				// �K�i�ɏ]���Ă��Ȃ��^�O�𔭌��������C�Ƃ肠�����������Ă���
				std::cout << "Illegal DICOM tag is found!" << std::endl;
			}
#else
			if( !process_dicom_tag( tag, pointer, numBytes, from_little_endian ) )
			{
				// �������邱�Ƃ��ł��Ȃ�DICOM�^�O�𔭌������̂ŏI������
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
			// �s���ȃ^�O�����X�g�ɒǉ�����
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


	/// @brief DICOM�t�@�C����ǂݍ���ŁC�S�Ă�DICOM�^�O���������e�[�u���ɓo�^����
	//! 
	//! @param[out] dicm  �c DICOM�^�O���Ƀf�[�^��o�^����e�[�u��
	//! @param[in]  filename �c ����DICOM]�t�@�C����
	//! 
	//! @retval true  �c DICOM�t�@�C���̏����ɐ���
	//! @retval false �c DICOM�t�@�C���ł͂Ȃ����C�����ł��Ȃ��^�O�E�f�[�^�����݂���ꍇ
	//! 
	inline bool read_dicom_tags( dicom_tag_container &dicm, unsigned char *buff, size_type numBytes, bool is_little_endian = true )
	{
		unsigned char *pointer = buff;
		unsigned char *end_pointer = buff + numBytes;

		// DICOM�f�[�^�̐擪�܂Ń|�C���^���ړ�
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
				// �G���f�B�A���̔���ɗp����͈͂����肷��
				group_end_pointer = pointer + find_tag( dicm, 0x0002, 0x0000, static_cast< unsigned int >( 0 ) );
			}

			if( group_end_pointer != NULL && once && dicm.contain( 0x0002, 0x0010 ) )
			{
				// ��x�������s����Ȃ��悤�ɂ���
				once = false;

				// �G���f�B�A���̌`����ݒ肷��
				from_little_endian = find_tag( dicm, 0x0002, 0x0010, "" ) != "1.2.840.10008.1.2.2";

				// �r�b�O�G���f�B�A���̏ꍇ�́C���̃u���b�N�������g���G���f�B�A���ŏ�������
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


	/// @brief DICOM�t�@�C����ǂݍ���ŁC�S�Ă�DICOM�^�O���������e�[�u���ɓo�^����
	//! 
	//! @param[out] dicm  �c DICOM�^�O���Ƀf�[�^��o�^����e�[�u��
	//! @param[in]  filename �c ����DICOM]�t�@�C����
	//! 
	//! @retval true  �c DICOM�t�@�C���̏����ɐ���
	//! @retval false �c DICOM�t�@�C���ł͂Ȃ����C�����ł��Ȃ��^�O�E�f�[�^�����݂���ꍇ
	//! 
	inline bool read_dicom_tags( dicom_tag_container &dicm, const std::string &filename )
	{
		size_type filesize;
		FILE *fp;
		if( ( fp = fopen( filename.c_str( ), "rb" ) ) == NULL )
		{
			return( false );
		}

		// �t�@�C���T�C�Y���擾
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

		// �K�؂�DICOM�t�@�C�����ǂ������`�F�b�N����
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


	/// @brief DICOM�̃^�O�W���� Explicit VR �`���Ńt�@�C���ɏ����o��
	//! 
	//! @param[in]  group     �c �O���[�vID
	//! @param[in]  element   �c �G�������gID
	//! @param[in]  vr        �c VR�^�O
	//! @param[in]  data      �c �f�[�^
	//! @param[in]  num_bytes �c �f�[�^�̃o�C�g��
	//! @param[out] fp    �c �o�̓t�@�C���X�g���[��
	//! @param[in]  to_little_endian �c �o�̓f�[�^�̌`�������g���G���f�B�A���ɂ��邩�ǂ���
	//! 
	//! @retval true  �c DICOM�t�@�C���̏����ɐ���
	//! @retval false �c DICOM�t�@�C���ł͂Ȃ����C�����ł��Ȃ��^�O�E�f�[�^�����݂���ꍇ
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
			fwrite( from_current_endian( byte_array< unsigned int >( static_cast< unsigned int >( num_bytes ) ), to_little_endian ).get_bytes( ), 1, 4, fp );
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
						// �f�[�^�̃o�C�g������������
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
						// �f�[�^�̃o�C�g������������
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
						// �f�[�^�̃o�C�g������������
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
						// �f�[�^�̃o�C�g������������
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
						// �f�[�^�̃o�C�g������������
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
			// �f�[�^���V�[�P���X�ɂȂ��Ă��邩�𒲂ׁC�o�͕��@��؂�ւ���
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



	/// @brief DICOM�̃^�O�W���� Implicit VR �`���Ńt�@�C���ɏ����o��
	//! 
	//! @param[in]  group     �c �O���[�vID
	//! @param[in]  element   �c �G�������gID
	//! @param[in]  vr        �c VR�^�O
	//! @param[in]  data      �c �f�[�^
	//! @param[in]  num_bytes �c �f�[�^�̃o�C�g��
	//! @param[out] fp    �c �o�̓t�@�C���X�g���[��
	//! @param[in]  to_little_endian �c �o�̓f�[�^�̌`�������g���G���f�B�A���ɂ��邩�ǂ���
	//! 
	//! @retval true  �c DICOM�t�@�C���̏����ɐ���
	//! @retval false �c DICOM�t�@�C���ł͂Ȃ����C�����ł��Ȃ��^�O�E�f�[�^�����݂���ꍇ
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
			// �f�[�^���V�[�P���X�ɂȂ��Ă��邩�𒲂ׁC�o�͕��@��؂�ւ���
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

	/// @brief DICOM�̃^�O�W���� Explicit VR �`���Ńt�@�C���ɏ����o��
	//! 
	//! @param[in]  e     �c �t�@�C���ɏo�͂���DICOM�^�O
	//! @param[out] fp    �c �o�̓t�@�C���X�g���[��
	//! @param[in]  to_little_endian �c �o�̓f�[�^�̌`�������g���G���f�B�A���ɂ��邩�ǂ���
	//! 
	//! @retval true  �c DICOM�t�@�C���̏����ɐ���
	//! @retval false �c DICOM�t�@�C���ł͂Ȃ����C�����ł��Ȃ��^�O�E�f�[�^�����݂���ꍇ
	//! 
	inline bool write_dicom_tag_explicit_vr( const dicom_element &e, FILE *fp, bool to_little_endian = true )
	{
		return( write_dicom_tag_explicit_vr( e.get_group( ), e.get_element( ), e.vr, e.data, e.num_bytes, fp, to_little_endian ) );
	}

	/// @brief DICOM�̃^�O�W���� Implicit VR �`���Ńt�@�C���ɏ����o��
	//! 
	//! @param[in]  e     �c �t�@�C���ɏo�͂���DICOM�^�O
	//! @param[out] fp    �c �o�̓t�@�C���X�g���[��
	//! @param[in]  to_little_endian �c �o�̓f�[�^�̌`�������g���G���f�B�A���ɂ��邩�ǂ���
	//! 
	//! @retval true  �c DICOM�t�@�C���̏����ɐ���
	//! @retval false �c DICOM�t�@�C���ł͂Ȃ����C�����ł��Ȃ��^�O�E�f�[�^�����݂���ꍇ
	//! 
	inline bool write_dicom_tag_implicit_vr( const dicom_element &e, FILE *fp, bool to_little_endian = true )
	{
		return( write_dicom_tag_implicit_vr( e.get_group( ), e.get_element( ), e.vr, e.data, e.num_bytes, fp, to_little_endian ) );
	}


	/// @brief DICOM�̃^�O���o�͂���ۂɊe�^�O���K�v�Ƃ���o�C�g�����v�Z����
	//! 
	//! @param[in] e          �c DICOM�^�O
	//! @param[in] implicitVR �c Implicit VR �`�����ǂ���
	//! 
	//! @return �^�O�̃o�C�g��
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


	/// @brief DICOM�� Group Length ���v�Z���C�^�O�ɐݒ肷��
	//! 
	//! @param[in,out] dicm       �c DICOM�^�O���Ƀf�[�^��o�^����e�[�u��
	//! @param[in]     implicitVR �c Implicit VR �`�����ǂ���
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
				size_t num_bytes = get_write_dicom_tag_size( e, implicitVR );

				if( element != 0x0000 )
				{
					// Group Length �̃^�O�̓X�L�b�v����
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
				// Group Length �̃^�O�Ƀf�[�^��ݒ肷��
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


	/// @brief DICOM�̃^�O�W�����t�@�C���ɏ����o��
	//! 
	//! @param[out] dicom    �c DICOM�^�O���Ƀf�[�^��o�^����e�[�u��
	//! @param[in]  filename �c �o��DICOM�t�@�C����
	//! 
	//! @retval true  �c DICOM�t�@�C���̏����ɐ���
	//! @retval false �c DICOM�t�@�C���ł͂Ȃ����C�����ł��Ȃ��^�O�E�f�[�^�����݂���ꍇ
	//! 
	inline bool write_dicom_tags( const dicom_tag_container &dicom, const std::string &filename )
	{
		FILE *fp;
		if( ( fp = fopen( filename.c_str( ), "wb" ) ) == NULL )
		{
			return( false );
		}

		// DICOM�̃v���A���u���̃^�O�����݂��邩�ǂ����𒲂ׂ�
		bool hasPreamble = false;
		{
			unsigned short elements[] = { 0x0001, 0x0002, 0x0003, 0x0010, 0x0012, 0x0013, 0x0016, 0x0100, 0x0102 };
			size_t num = sizeof( elements ) / sizeof( unsigned short );
			for( size_t i = 0 ; i < num ; i++ )
			{
				if( dicom.contain( 0x0002, elements[ i ] ) )
				{
					// �f�[�^�����݂���̂Ńv���A���u���𖄂ߍ���
					hasPreamble = true;
					break;
				}
			}
		}

		dicom_tag_container dicm( dicom );
		bool implicitVR = true;

		if( hasPreamble )
		{
			// DICOM�̃w�b�_������������
			unsigned char ZERO[ 128 ];
			unsigned char DICM[ 4 ];
			memset( ZERO, 0, sizeof( unsigned char ) * 128 );
			DICM[ 0 ] = 'D';
			DICM[ 1 ] = 'I';
			DICM[ 2 ] = 'C';
			DICM[ 3 ] = 'M';
			fwrite( ZERO, sizeof( unsigned char ), 128, fp );
			fwrite( DICM, sizeof( unsigned char ), 4, fp );


			// �����IVR�̎w�肪�Ȃ��ꍇ�͒ǉ�����
			if( !dicm.contain( 0x0002, 0x0010 ) )
			{
				// Transfer Syntax UID �����݂��Ȃ��ꍇ�́C�uImplicit VR Little Endian�v���w�肷��
				std::string syntax = "1.2.840.10008.1.2";
				dicm.append( dicom_element( 0x0002, 0x0010, reinterpret_cast< const unsigned char * >( syntax.c_str( ) ), syntax.length( ) ) );
			}
			else if( find_tag( dicm, 0x0002, 0x0010, "" ) == "1.2.840.10008.1.2.1" )
			{
				// �uExplicit VR Little Endian�v���w�肳��Ă���ꍇ�́C�uExplicit VR Little Endian�v�ɕύX���邷��
				implicitVR = false;
			}
			else if( find_tag( dicm, 0x0002, 0x0010, "" ) == "1.2.840.10008.1.2.2" )
			{
				// �uImplicit VR Big Endian�v���w�肳��Ă���ꍇ�́C�uExplicit VR Little Endian�v�ɕύX���邷��
				implicitVR = false;
				std::string syntax = "1.2.840.10008.1.2.1";
				dicm( 0x0002, 0x0010 ).copy( reinterpret_cast< const unsigned char * >( syntax.c_str( ) ), syntax.length( ) );
			}

			// Group 0002 �̒������}������Ă��Ȃ��ꍇ�͑}������
			if( !dicm.contain( 0x0002, 0x0000 ) )
			{
				byte_array< unsigned int > b( static_cast< unsigned int >( 0 ) );
				dicm.append( dicom_element( 0x0002, 0x0000, b.get_bytes( ), b.length( ) ) );
			}
		}

		// Group Length �𐳂����ݒ肷��
		compute_group_length( dicm, implicitVR );

		// ���ׂĖ����IVR�ŋL�q����
		dicom_tag_container::const_iterator ite = dicm.begin( );
		for( ; ite != dicm.end( ) ; ++ite )
		{
			if( ite->second.enable )
			{
				// Group 0002 ������ Explicit VR Little Endian Transfer Syntax �ŃG���R�[�h����K�v����
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




/// @brief DICOM�f�[�^���摜�R���e�i�ɓǂݍ���
//! 
//! @note �{�����ł́C�s�v�ȃ^�O�͑S�Ď�菜����Ă��܂����߁C�K�v�ȃf�[�^������ꍇ�́C�{�֐������Q�l�ɂ��Ď��삵�Ă�������
//! 
//! @param[out] image    �c �o�͉摜
//! @param[in]  filename �c ���̓t�@�C����
//! 
//! @retval true  �c DICOM�t�@�C���̏����ɐ���
//! @retval false �c DICOM�t�@�C���ł͂Ȃ����C�����ł��Ȃ��^�O�E�f�[�^�����݂���ꍇ
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
		dicom::dicom_element &element = dicm( 0x7fe0, 0x0010 );
		if( !dicom::decode( element, info ) )
		{
			// ���k�̉𓀂������̓f�[�^����������
			return( false );
		}

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


/// @}
//  DICOM�摜���o�̓O���[�v�̏I���

/// @}
//  �摜���o�̓O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_DICOM__
