#ifndef __INCLUDE_MIST_DICOM__
#define __INCLUDE_MIST_DICOM__


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

// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


// ���̃}�N�����`����ƁC�R���\�[����ɓǂݍ��񂾃^�O����\������
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

		// DICOM�p�̃v���A���u�������݂��Ȃ����߁C�擪�̃|�C���^��Ԃ�
		return( p );
	}

	// DICOM�̃^�O��ǂݍ��݁C�e�[�u���ɓo�^����Ă�����̂Əƍ�����D
	// �e�[�u���ɓo�^����Ă��Ȃ��ꍇ�́C�ǂݔ�΂��D
	// �����C�K�؂�DICOM�t�@�C���łȂ��ꍇ��-2��Ԃ��C�f�[�^�̏I�[�������̓t�@�C���̓ǂݍ��݂Ɏ��s�����ꍇ��-1��Ԃ��D
	// �����āC�ǂݔ�΂����ꍇ��0��Ԃ��C�e�[�u���ɓo�^����Ă���ꍇ�͎��ɑ��݂���f�[�^�̃o�C�g����Ԃ��D
	// �f�[�^����������擪�̃|�C���^ 'p' �ƁC�f�[�^�̍Ō�{1�������|�C���^ 'e' ��^����
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
				// �����IVR�ł��邪�C�����ƈقȂ�ꍇ
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
					// �Î��IVR
					num_bytes = to_current_endian( byte_array< unsigned int >( data ), true ).get_value( );
				}
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


	// DICOM��UID��ϊ�����
	inline dicom_uid get_uid( const std::string &uid )
	{
		static dicom_uid_table uid_table;
		return( uid_table.get_uid( uid ) );
	}

	// DICOM��UID��ϊ�����
	inline dicom_uid get_uid( const unsigned char *str, difference_type numBytes )
	{
		return( get_uid( std::string( reinterpret_cast< const char * >( str ), str[ numBytes - 1 ] == 0 ? numBytes - 1 : numBytes ) ) );
	}


	// DICOM�̃^�O�ɑ΂��C�eVR���Ƃ̏������s���D
	// �o�^����Ă��Ȃ��^�O�̏ꍇ�� false ���������C�������������ꂽ�ꍇ�̂� true ��Ԃ��D
	inline bool process_dicom_tag( const dicom_tag &tag, unsigned char *byte, difference_type num_bytes )
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
				byte_array< float > tmp = to_current_endian( byte_array< float >( byte ), true );
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
				byte_array< signed int > tmp = to_current_endian( byte_array< signed int >( byte ), true );
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
				return( 0 );
			}

			{
				byte_array< signed short > tmp = to_current_endian( byte_array< signed short >( byte ), true );
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
				byte_array< unsigned int > tmp = to_current_endian( byte_array< unsigned int >( byte ), true );
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
				byte_array< unsigned short > tmp = to_current_endian( byte_array< unsigned short >( byte ), true );
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
				// �������邱�Ƃ��ł��Ȃ�DICOM�^�O�𔭌������̂ŏI������
				return( NULL );
			}
			else
			{
				dicom_tag_container::iterator ite;
				// �f�[�^���u�q�̗v���𖞂����ꍇ�̓��X�g�ɒǉ�
				switch( tag.vr )
				{
				case UI:
					{
						// DICOM�̌ŗLID�Ȃ̂ŁC�������ϊ�����
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


		unsigned char *end_pointer = buff + filesize;
		pointer = buff;
		// DICOM�f�[�^�̐擪�܂Ń|�C���^���ړ�
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



// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_DICOM__
