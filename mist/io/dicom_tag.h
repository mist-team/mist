#ifndef __INCLUDE_MIST_DICOM_TAG__
#define __INCLUDE_MIST_DICOM_TAG__


#ifndef __INCLUDE_MIST_CONF_H__
#include "mist_conf.h"
#endif

#include <iostream>
#include <string>
#include <set>


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


namespace __dicom_controller__
{

	// DICOM�̃^�O�̓��C�L���ɂ���^�O�̎�ނ̂ݒ�`����
	// ���p������̂������Ȃ�ɂ�C�R���p�C�����Ԃ���������̂Œ��� 
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
		UNKNOWN,	// �s���Ȃu�q
		AE,			// ���p�G���e�B�e�B�@�P�U�o�C�g�ȉ�
		AS,			// �N��@�@�@�@�@�@�@�S�o�C�g�Œ�
		AT,			// �����^�O�@�@�@�@�@�S�o�C�g�Œ�
		CS,			// �R�[�h��@�@�@�@�@�P�U�o�C�g�ȉ�
		DA,			// ���t�@�@�@�@�@�@�@�W�o�C�g�Œ�
		DS,			// 10�i���i������j�@�P�U�o�C�g�ȉ�
		DT,			// �����@�@�@�@�@�@�@�Q�U�o�C�g�ȉ�
		FL,			// �S�o�C�g�����@�@�@�S�o�C�g�Œ�
		FD,			// �W�o�C�g�����@�@�@�W�o�C�g�Œ�
		IS,			// �����i������j�@�@�P�U�o�C�g�ȉ�
		LO,			// ����@�@�@�@�@�@�@�U�S�o�C�g�ȉ�
		LT,			// �e�L�X�g�@�@�@�@�@�P�O�Q�S�o�C�g�ȉ�
		OB,			// �o�C�g��@�@�@�@�@������
		OF,			// ���������_������@232-4 maximum
		OW,			// ���[�h��@�@�@�@�@������
		PN,			// ���Җ��@�@�@�@�@�@�U�S�o�C�g�ȉ�
		SH,			// �Z��@�@�@�@�@�@�@�P�U�o�C�g�ȉ�
		SL,			// ���������@�@�@�@�S�o�C�g�Œ�
		SQ,			// ���ڃV�[�P���X�@�@������
		SS,			// �������Z�@�@�@�@�Q�o�C�g�Œ�
		ST,			// �Z�e�L�X�g�@�@�@�@�P�O�Q�S�o�C�g�ȉ�
		TM,			// ���ԁ@�@�@�@�@�@�@�P�U�o�C�g�ȉ�
		UI,			// �t�h�c�@�@�@�@�@�@�U�S�o�C�g�ȉ�
		UL,			// �����Ȃ����@�@�@�@�S�o�C�g�Œ�
		UN,			// ���m�@�@�@�@�@�@�@�s��
		US,			// �����Ȃ��Z�@�@�@�@�Q�o�C�g�Œ�
		UT,			// �������e�L�X�g
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
			// �`�d�@���p�G���e�B�e�B�@�P�U�o�C�g�ȉ�
			// �Ӗ��̂Ȃ��擪�Ɩ�����SPACE�i20H�j����������B
			// �g�w�肳�ꂽ���p�����Ȃ��h���Ƃ��Ӗ�����P�U�̃X�y�|�X�ł�����l�́C�g�p���Ȃ�
			ret = AE;
		}
		else if( vr == "AS" )
		{
			// �`�r�@�N��@�@�@�@�@�@�@�S�o�C�g�Œ�
			// ���̏����̈���������� - nnnD, nnnW, nnnM, nnnY�G
			// ������ nnn �� D �ɑ΂��Ă͓��CW �ɑ΂��Ă͏T�CM �ɑ΂��Ă͌��CY �ɑ΂��Ă͔N�̐����܂ށB
			// ��F "018M" �� 18 ���̔N���\���B
			ret = AS;
		}
		else if( vr == "AT" )
		{
			// �`�s�@�����^�O�@�@�@�@�@�S�o�C�g�Œ�
			// �f�[�^�v�f�^�O�̒l�ł���P�U�r�b�g�����Ȃ������̏����t����ꂽ�΁B
			// ��F (0018,00FF) �̃f�[�^�v�f�^�O�́C�S�o�C�g�̃V���[�Y�Ƃ���
			// ���g���G���f�B�A���]���\���ł� 18H, 00H, FFH, 00H �Ƃ��āC
			// �r�b�O�G���f�B�A���]���\���ł� 00H, 18H, 00H, FFH �Ƃ��ĕ����������ł��낤�B
			// ���F�`�s�l�̕������͐߂V�̒��Œ�`�����f�[�^�v�f�^�O�̕������Ɛ��m�ɓ���ł���B
			ret = AT;
		}
		else if( vr == "CS" )
		{
			// �b�r�@�R�[�h��@�@�@�@�@�P�U�o�C�g�ȉ�
			// �Ӗ��̂Ȃ��擪�܂��͖����̃X�y�|�X�i20H�j����������B
			ret = CS;
		}
		else if( vr == "DA" )
		{
			// �c�`�@���t�@�@�@�@�@�@�@�W�o�C�g�Œ�
			// ���� yyyymmdd �̕�����G
			// ������ yyyy �͔N�Cmm �͌��Cdd �͓����܂ށB
			// �����ANSI HISPP MSDS Date common data type�ɓK������B
			// ��F"19930822"�� 1993 �N 8 �� 22 ����\���B
			// ���F�P�DV 3.0 ���O�̂��̋K�i�̔łƂ̌���݊����̂��߂ɁC
			// �����́C���̂u�q�ɑ΂��Č`��yyyy.mm.dd �̕�����𓯗l�ɃT�|�[�g���邱�Ƃ𐄏������B
			ret = DA;
		}
		else if( vr == "DS" )
		{
			// �c�r�@10�i���i������j�@�P�U�o�C�g�ȉ�
			// �Œ菬���_�����������_����\�����镶����B
			// �Œ菬���_���͕��� 0-9�C�C�ӂ̐擪�� "+" �܂��� "-"�C����я����_�������C�ӂ� "." �݂̂��܂ށB
			// ���������_���́CANSI X3.9 �̒��Œ�`�����Ƃ���C�w���̎n�܂������ "E" �� "e" �������ē`�B�����B
			// 10 �i����͐擪���邢�͖����X�y�[�X�ŏ[�Ă񂳂�邱�Ƃ�����B
			// �r���̃X�y�[�X�͋�����Ȃ��B
			ret = DS;
		}
		else if( vr == "DT" )
		{
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
			ret = DT;
		}
		else if( vr == "FL" )
		{
			// �e�k�@�S�o�C�g�����@�@�@�S�o�C�g�Œ�
			// �P���x�̂Q�i���������_�̐����ŁCIEEE 754: 1985, 32 �r�b�g���������_���`���ŕ\�������B
			ret = FL;
		}
		else if( vr == "FD" )
		{
			// �e�c�@�W�o�C�g�����@�@�@�W�o�C�g�Œ�
			// �{���x�̂Q�i���������_�̐����ŁCIEEE 754: 1985, 64 �r�b�g���������_���`���ŕ\�������B
			ret = FD;
		}
		else if( vr == "IS" )
		{
			// �h�r�@�����i������j�@�@�P�U�o�C�g�ȉ�
			// 10 ���Ƃ��鐮���i10 �i���j��\�킷������ŁC
			// �C�ӑI���̐擪�� "+", "-" �������� 0-9 �݂̂��܂ށB
			// ����͐擪�����Ė����̃X�y�|�X�Ŗ��߂��邱�Ƃ�����B
			// �r���̃X�y�|�X�͋�����Ȃ��B�\������鐮�� n �́C���L�͈̔͂ł���B
			// -231 �� n �� (231-1) 
			ret = IS;
		}
		else if( vr == "LO" )
		{
			// �k�n�@����@�@�@�@�@�@�@�U�S�o�C�g�ȉ�
			// �擪����с^�܂��͖����̃X�y�|�X�Ŗ��߂��邱�Ƃ����镶����B
			// �������� 5CH �iISO-IR 6 �̒��̃o�b�N�X���b�V�� "\" �j�́C
			// �����l�f�|�^�v�f�̒��̒l�̊Ԃ̋�؂�L���Ƃ��Ďg�p�����̂ŁC���݂��Ȃ��B
			// ���̗�́C ESC �������C���䕶���������Ȃ��B
			ret = LO;
		}
		else if( vr == "LT" )
		{
			// �k�s�@�e�L�X�g�@�@�@�@�@�P�O�Q�S�o�C�g�ȉ�
			// ��ȏ�̒i�����܂ނ��Ƃ����镶����B
			// �}�`�����W���Ɛ��䕶�� CR, LF, FF, ����� ESC ���܂ނ��Ƃ�����B
			// ��������邱�Ƃ����閖���̃X�y�[�X�����Ŗ��߂��邱�Ƃ�����C
			// �������擪�̃X�y�[�X�����͈Ӗ�������ƍl������B
			// ���̂u�q�����f�|�^�v�f�͕����l�ł͂Ȃ��C
			// �]���ĕ������� 5CH �iISO-IR 6 �̒��̃o�b�N�X���b�V�� "\" �j�͎g�p����邱�Ƃ�����B
			ret = LT;
		}
		else if( vr == "OB" )
		{
			// �n�a�@�o�C�g��@�@�@�@�@������
			// ���e�̕��������܏Փ]���\���ɂ���Ďw�肳���o�C�g��B
			// �n�a�̓��g���G���f�B�A���^�r�b�O�G���f�B�A���o�C�g���ɉe������Ȃ��u�q�ł���B
			// �o�C�g��͋������ɂ��邽�߂ɕK�v�ȂƂ��C�P��̖�����NULL�o�C�g�l�i00H�j�Ŗ��߂���B
			ret = OB;
		}
		else if( vr == "OF" )
		{
			// �n�e�@���������_������@�@�@�@�@������
			// A string of 32-bit IEEE 754:1985 floating point
			// words. OF is a VR which requires byte
			// swapping within each 32-bit word when
			// changing between Little Endian and Big
			// Endian byte ordering (see Section 7.3).
			ret = OF;
		}
		else if( vr == "OW" )
		{
			// �n�v�@���[�h��@�@�@�@�@������
			// ���e�̕��������܏Փ]���\���ɂ���Ďw�肳�ꂽ�P�U�r�b�g���[�h��B
			// �n�v�̓��g���G���f�B�A������уr�b�O�G���f�B�A���o�C�g���̊ԂŕύX����Ƃ��C
			// �e���[�h���Ńo�C�g�X���b�s���O��K�v�Ƃ���u�q�ł���B
			ret = OW;
		}
		else if( vr == "PN" )
		{
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
			ret = PN;
		}
		else if( vr == "SH" )
		{
			// �r�g�@�Z��@�@�@�@�@�@�@�P�U�o�C�g�ȉ�
			// �擪����с^�܂��͖����̃X�y�|�X�Ŗ��߂��邱�Ƃ����镶����B
			// �������� 5CH �iISO-IR 6 �̒��̃o�b�N�X���b�V�� "\" �j�́C�����l�f�|�^�v�f�̂��߂̒l�̊Ԃ̋�؂�L���Ƃ��Ďg�p�����̂ŁC���݂��Ȃ��B
			// ���̗�� ESC ���������䕶���������Ȃ��B
			ret = SH;
		}
		else if( vr == "SL" )
		{
			// �r�k�@���������@�@�@�@�S�o�C�g�Œ�
			// �Q�̕␔�`���̂R�Q�r�b�g�������t���Q�i�����B
			// ���͈̔͂̐��� n ��\���F -2^31 �� n �� (2^31-1�j 
			ret = SL;
		}
		else if( vr == "SQ" )
		{
			// �r�p�@���ڃV�[�P���X�@�@������
			// �l�́C�߂V�D�T�Œ�`�����P�ȏ�̍��ڂ̃V�[�P���X�ł���B
			ret = SQ;
		}
		else if( vr == "SS" )
		{
			// �r�r�@�������Z�@�@�@�@�Q�o�C�g�Œ�
			// �Q�̕␔�`���̂P�U�r�b�g�������t���Q�i���B
			// �͈� -2^15 ��n�� (2^15-1) �̒��̐��� n ��\���B
			ret = SS;
		}
		else if( vr == "ST" )
		{
			// �r�s�@�Z�e�L�X�g�@�@�@�@�P�O�Q�S�o�C�g�ȉ�
			// ��ȏ�̒i�����܂ނ��Ƃ����镶����B
			// �}�`�����W���Ɛ��䕶�� CR, LF, FF, ����� ESC ���܂ނ��Ƃ�����B
			// ��������邱�Ƃ����閖���̃X�y�[�X�Ŗ��߂��邱�Ƃ�����C�������擪�̃X�y�[�X�͈Ӗ�������ƍl������B
			// ���̂u�q�����f�|�^�v�f�́C�����l�ł͂Ȃ��C�]���ĕ������� 5CH �iISO-IR 6 �̒��̃o�b�N�X���b�V�� "\" �j�͎g�p����邱�Ƃ�����B
			ret = ST;
		}
		else if( vr == "TM" )
		{
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
			ret = TM;
		}
		else if( vr == "UI" )
		{
			// �t�h�@�t�h�c�@�@�@�@�@�@�U�S�o�C�g�ȉ�
			// ���ڂ̍L����ނ�B��Ɏ��ʂ��邽�߂ɗp������t�h�c���܂ޕ�����B
			// �t�h�c�̓s���I�h "." �����ŕ�����ꂽ�����\���v�f�̃V���[�Y�ł���B
			// ��ȏ�̂t�h�c���܂ޒl�̈�̒�������o�C�g���̏ꍇ�C�l�̈悪�����o�C�g�̒����ł��邱�Ƃ��m�ۂ��邽�߂�
			// ��̖�����NULL �i00H�j�Ŗ��߂���B���S�Ȏd�l�Ɨ�͐߂X�ƕt�����a���Q�ƁB
			ret = UI;
		}
		else if( vr == "UL" )
		{
			// �t�k�@�����Ȃ����@�@�@�@�S�o�C�g�Œ�
			// ���������R�Q�r�b�g���Q�i�����B
			// ���L�͈̔͂̐��� n ��\���F 0 ��n�� 2^32 
			ret = UL;
		}
		else if( vr == "UN" )
		{
			// ���m
			// ���e�̕��������s���ł���o�C�g�̗�B�i�߂U�D�Q�D�Q�Q�Ɓj
			ret = UN;
		}
		else if( vr == "US" )
		{
			// �t�r�@�����Ȃ��Z�@�@�@�@�Q�o�C�g�Œ�
			// ���������P�U�r�b�g���Q�i�����B
			// ���L�͈̔͂̐��� n ��\���F 0 ��n�� 2^16 
			ret = US;
		}
		else if( vr == "UT" )
		{
			// �t�s�@�������e�L�X�g
			// ��ȏ�̒i�����܂�ł��邱�Ƃ����镶����B
			// ����͐}�`�����W������ѐ��䕶�� CR, LF, FF ����� ESC ���܂ނ��Ƃ�����B
			// ����́C��������邱�Ƃ����閖���̃X�y�[�X�Ŗ��߂��邱�Ƃ�����B
			// �������擪�̃X�y�[�X�͈Ӗ�������ƍl������B
			// ���̂u�q�����f�[�^�v�f�͕����l�ł͂Ȃ��C���������āC�����R�[�h 5CH �iISO-IR 6 �ɂ�����o�b�N�X���b�V�� "\"�j�͎g�p����邱�Ƃ�����B 
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


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_DICOM_TAG__
