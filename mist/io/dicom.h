#ifndef __INCLUDE_MIST_DICOM__
#define __INCLUDE_MIST_DICOM__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

// �J���[�摜�̐ݒ��ǂݍ���
#ifndef __INCLUDE_MIST_COLOR_H__
#include "../config/color.h"
#endif

#include "./dicom_tag.h"

// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


#define __SHOW_DICOM_TAG__

namespace __dicom_controller__
{
	template < class T >
	union byte_array
	{
	private:
		typedef T value_type;
		value_type value;
		char byte[ sizeof( value_type ) ];

	public:
		byte_array( ) : value( 0 ){ }
		byte_array( const value_type v ) : value( v ){ }
		byte_array( const byte_array &v ) : value( v.value ){ }
		byte_array( const char *b )
		{
			for( size_t i = 0 ; i < sizeof( value_type ) ; i++ )
			{
				byte[ i ] = b[ i ];
			}
		}
		char &operator[]( size_t index ){ return( byte[index] ); }
		const char &operator[]( size_t index ) const { return( byte[index] ); }
		const value_type get_value( ) const { return( value ); }
		value_type set_value( const value_type &v ) { return( value = v ); }
	};

	inline bool _is_little_endian_( )
	{
		return( byte_array< unsigned short >( 1 )[ 0 ] == 1 );
	}

	inline bool _is_big_endian_( )
	{
		return( byte_array< unsigned short >( 1 )[ 0 ] == 0 );
	}

	template < class T >
	void swap_bytes( byte_array< T > &bytes )
	{
		byte_array< T > tmp( bytes );
		for( size_t i = 0 ; i < sizeof( T ) ; ++i )
		{
			bytes[ i ] = tmp[ sizeof( T ) - i - 1 ];
		}
	}

	template < class T >
	byte_array< T > to_current_endian( const byte_array< T > &bytes, bool is_little_endian )
	{
		static bool current_endian = _is_little_endian_( );
		if( current_endian != is_little_endian )
		{
			byte_array< T > tmp( bytes );
			swap_bytes( tmp );
			return( tmp );
		}
		else
		{
			return( bytes );
		}
	}

	template < class T >
	byte_array< T > from_current_endian( const byte_array< T > &bytes, bool is_little_endian )
	{
		static bool current_endian = _is_little_endian_( );
		if( current_endian != is_little_endian )
		{
			byte_array< T > tmp( bytes );
			swap_bytes( tmp );
			return( tmp );
		}
		else
		{
			return( bytes );
		}
	}

	inline bool check_dicom_file( FILE *fp )
	{
		if( fp == NULL ) return( false );
		if( fseek( fp, 128, SEEK_SET ) != 0 ) return( false );

		char dicom[12];
		if( fread( dicom, sizeof( char ), 4, fp ) == 4 )
		{
			dicom[4] = '\0';
			if( strcmp( dicom, "DICM" ) == 0 ) return( true );
		}

		// DICOM�p�̐U�ĂԂ邪���݂��Ȃ����߁C�t�@�C���̐擪�Ƀ|�C���^��߂�
		rewind( fp );
		return( false );
	}

	// DICOM�̃^�O��ǂݍ��݁C�e�[�u���ɓo�^����Ă�����̂Əƍ�����D
	// �e�[�u���ɓo�^����Ă��Ȃ��ꍇ�́C�ǂݔ�΂��D
	// �����C�K�؂�DICOM�t�@�C���łȂ��ꍇ��-2��Ԃ��C�t�@�C���̏I�[�������̓t�@�C���̓ǂݍ��݂Ɏ��s�����ꍇ��-1��Ԃ��D
	// �����āC�ǂݔ�΂����ꍇ��0��Ԃ��C�e�[�u���ɓo�^����Ă���ꍇ�͎��ɑ��݂���f�[�^�̃o�C�g����Ԃ��D
	inline long read_dicom_tag( FILE *fp, dicom_tag &tag )
	{
		if( fp == NULL ) return( -1 );

		char data[4];
		if( fread( data, sizeof( char ), 4, fp ) != 4 ) return( -1 );

		unsigned short group   = to_current_endian( byte_array< unsigned short >( data ), true ).get_value( );
		unsigned short element = to_current_endian( byte_array< unsigned short >( data + 2 ), true ).get_value( );
		unsigned int   num_bytes;

		if( fread( data, sizeof( char ), 4, fp ) != 4 ) return( -1 );
		char tmp[3];
		tmp[0] = data[0];
		tmp[1] = data[1];
		tmp[2] = '\0';
		dicom_vr vr = get_dicom_vr( tmp );

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
				case SQ:
				case UN:
					fread( data, sizeof( char ), 4, fp );
					num_bytes = to_current_endian( byte_array< unsigned int >( data ), true ).get_value( );
					break;

				default:
					num_bytes = to_current_endian( byte_array< unsigned short >( data + 2 ), true ).get_value( );
					break;
				}
			}
			else
			{
				switch( vr )
				{
				case OB:
				case OW:
				case SQ:
				case UN:
					fread( data, sizeof( char ), 4, fp );
					num_bytes = to_current_endian( byte_array< unsigned int >( data ), true ).get_value( );
					break;

				default:
					// �Î��IVR
					num_bytes = to_current_endian( byte_array< unsigned int >( data ), true ).get_value( );
				}
			}
			return( num_bytes );
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
			case SQ:
			case UN:
				fread( data, sizeof( char ), 4, fp );
				num_bytes = to_current_endian( byte_array< unsigned int >( data ), true ).get_value( );
				break;

			default:
				num_bytes = to_current_endian( byte_array< unsigned int >( data ), true ).get_value( );
				break;
			}

#ifdef __SHOW_DICOM_TAG__
			printf( "Unknown Tags( %04x, %04x )!!\n", group, element );
#endif
			if( fseek( fp, num_bytes, SEEK_CUR ) != 0 ) return( -2 );
		}

		return( 0 );
	}

	// DICOM�̃^�O�ɑ΂��C�eVR���Ƃ̏������s���D
	// �f�[�^��VR�̗v���𖞂����Ȃ��ꍇ��0��Ԃ��D
	// �o�^����Ă��Ȃ��^�O�̏ꍇ�� false ���������C�������������ꂽ�ꍇ�̂� true ��Ԃ��D
	inline bool process_dicom_tag( const dicom_tag &tag, char *byte, long &num_bytes )
	{
		switch( tag.vr )
		{
		case AE:
			// �`�d�@���p�G���e�B�e�B�@�P�U�o�C�g�ȉ�
			// �Ӗ��̂Ȃ��擪�Ɩ�����SPACE�i20H�j����������B
			// �g�w�肳�ꂽ���p�����Ȃ��h���Ƃ��Ӗ�����P�U�̃X�y�|�X�ł�����l�́C�g�p���Ȃ�
			if( tag.vm != -1 && num_bytes > 16 * tag.vm ) return( 0 );
			byte[ num_bytes ] = '\0';
			num_bytes++;
			break;

		case AS:
			// �`�r�@�N��@�@�@�@�@�@�@�S�o�C�g�Œ�
			// ���̏����̈���������� - nnnD, nnnW, nnnM, nnnY�G
			// ������ nnn �� D �ɑ΂��Ă͓��CW �ɑ΂��Ă͏T�CM �ɑ΂��Ă͌��CY �ɑ΂��Ă͔N�̐����܂ށB
			// ��F "018M" �� 18 ���̔N���\���B
			if( tag.vm != -1 && ( num_bytes % 4 != 0 || (int)( num_bytes / 4 ) > tag.vm ) ) return( 0 );
			byte[ num_bytes ] = '\0';
			num_bytes++;
			break;

		case AT:
			// �`�s�@�����^�O�@�@�@�@�@�S�o�C�g�Œ�
			// �f�[�^�v�f�^�O�̒l�ł���P�U�r�b�g�����Ȃ������̏����t����ꂽ�΁B
			// ��F (0018,00FF) �̃f�[�^�v�f�^�O�́C�S�o�C�g�̃V���[�Y�Ƃ���
			// ���g���G���f�B�A���]���\���ł� 18H, 00H, FFH, 00H �Ƃ��āC
			// �r�b�O�G���f�B�A���]���\���ł� 00H, 18H, 00H, FFH �Ƃ��ĕ����������ł��낤�B
			// ���F�`�s�l�̕������͐߂V�̒��Œ�`�����f�[�^�v�f�^�O�̕������Ɛ��m�ɓ���ł���B
			if( tag.vm != -1 && ( num_bytes % 4 != 0 || (int)( num_bytes / 4 ) > tag.vm ) ) return( 0 );
			byte[ num_bytes ] = '\0';
			num_bytes++;
			break;

		case CS:
			// �b�r�@�R�[�h��@�@�@�@�@�P�U�o�C�g�ȉ�
			// �Ӗ��̂Ȃ��擪�܂��͖����̃X�y�|�X�i20H�j����������B
			if( tag.vm != -1 && num_bytes > 16 * tag.vm ) return( 0 );
			byte[ num_bytes ] = '\0';
			num_bytes++;
			break;

		case DA:
			// �c�`�@���t�@�@�@�@�@�@�@�W�o�C�g�Œ�
			// ���� yyyymmdd �̕�����G
			// ������ yyyy �͔N�Cmm �͌��Cdd �͓����܂ށB
			// �����ANSI HISPP MSDS Date common data type�ɓK������B
			// ��F"19930822"�� 1993 �N 8 �� 22 ����\���B
			// ���F�P�DV 3.0 ���O�̂��̋K�i�̔łƂ̌���݊����̂��߂ɁC
			// �����́C���̂u�q�ɑ΂��Č`��yyyy.mm.dd �̕�����𓯗l�ɃT�|�[�g���邱�Ƃ𐄏������B
			if( tag.vm != -1 && ( num_bytes % 8 != 0 || (int)( num_bytes / 8 ) > tag.vm ) ) return( 0 );
			byte[ num_bytes ] = '\0';
			num_bytes++;
			break;

		case DS:
			// �c�r�@10�i���i������j�@�P�U�o�C�g�ȉ�
			// �Œ菬���_�����������_����\�����镶����B
			// �Œ菬���_���͕��� 0-9�C�C�ӂ̐擪�� "+" �܂��� "-"�C����я����_�������C�ӂ� "." �݂̂��܂ށB
			// ���������_���́CANSI X3.9 �̒��Œ�`�����Ƃ���C�w���̎n�܂������ "E" �� "e" �������ē`�B�����B
			// 10 �i����͐擪���邢�͖����X�y�[�X�ŏ[�Ă񂳂�邱�Ƃ�����B
			// �r���̃X�y�[�X�͋�����Ȃ��B
			if( tag.vm != -1 && num_bytes > 16 * tag.vm ) return( 0 );
			byte[ num_bytes ] = '\0';
			num_bytes++;
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
			if( tag.vm != -1 && num_bytes > 26 * tag.vm ) return( 0 );
			byte[ num_bytes ] = '\0';
			num_bytes++;
			break;

		case FL:
			// �e�k�@�S�o�C�g�����@�@�@�S�o�C�g�Œ�
			// �P���x�̂Q�i���������_�̐����ŁCIEEE 754: 1985, 32 �r�b�g���������_���`���ŕ\�������B
			if( tag.vm != -1 && ( num_bytes % 4 != 0 || (int)( num_bytes / 4 ) > tag.vm ) ) return( 0 );

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
				return( 0 );
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
			if( tag.vm != -1 && num_bytes > 16 * tag.vm ) return( 0 );
			byte[ num_bytes ] = '\0';
			num_bytes++;
			break;

		case LO:
			// �k�n�@����@�@�@�@�@�@�@�U�S�o�C�g�ȉ�
			// �擪����с^�܂��͖����̃X�y�|�X�Ŗ��߂��邱�Ƃ����镶����B
			// �������� 5CH �iISO-IR 6 �̒��̃o�b�N�X���b�V�� "\" �j�́C
			// �����l�f�|�^�v�f�̒��̒l�̊Ԃ̋�؂�L���Ƃ��Ďg�p�����̂ŁC���݂��Ȃ��B
			// ���̗�́C ESC �������C���䕶���������Ȃ��B
			if( tag.vm != -1 && num_bytes > 64 * tag.vm ) return( 0 );
			byte[ num_bytes ] = '\0';
			num_bytes++;
			break;

		case LT:
			// �k�s�@�e�L�X�g�@�@�@�@�@�P�O�Q�S�o�C�g�ȉ�
			// ��ȏ�̒i�����܂ނ��Ƃ����镶����B
			// �}�`�����W���Ɛ��䕶�� CR, LF, FF, ����� ESC ���܂ނ��Ƃ�����B
			// ��������邱�Ƃ����閖���̃X�y�[�X�����Ŗ��߂��邱�Ƃ�����C
			// �������擪�̃X�y�[�X�����͈Ӗ�������ƍl������B
			// ���̂u�q�����f�|�^�v�f�͕����l�ł͂Ȃ��C
			// �]���ĕ������� 5CH �iISO-IR 6 �̒��̃o�b�N�X���b�V�� "\" �j�͎g�p����邱�Ƃ�����B
			if( tag.vm != -1 && num_bytes > 1024 * tag.vm ) return( 0 );
			byte[ num_bytes ] = '\0';
			num_bytes++;
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
			if( tag.vm != -1 && num_bytes > 64 * tag.vm ) return( 0 );
			byte[ num_bytes ] = '\0';
			num_bytes++;
			break;

		case SH:
			// �r�g�@�Z��@�@�@�@�@�@�@�P�U�o�C�g�ȉ�
			// �擪����с^�܂��͖����̃X�y�|�X�Ŗ��߂��邱�Ƃ����镶����B
			// �������� 5CH �iISO-IR 6 �̒��̃o�b�N�X���b�V�� "\" �j�́C�����l�f�|�^�v�f�̂��߂̒l�̊Ԃ̋�؂�L���Ƃ��Ďg�p�����̂ŁC���݂��Ȃ��B
			// ���̗�� ESC ���������䕶���������Ȃ��B
			if( tag.vm != -1 && num_bytes > 16 * tag.vm ) return( 0 );
			byte[ num_bytes ] = '\0';
			num_bytes++;
			break;

		case SL:
			// �r�k�@���������@�@�@�@�S�o�C�g�Œ�
			// �Q�̕␔�`���̂R�Q�r�b�g�������t���Q�i�����B
			// ���͈̔͂̐��� n ��\���F -2^31 �� n �� (2^31-1�j 
			if( tag.vm != -1 && ( num_bytes % 4 != 0 || (int)( num_bytes / 4 ) > tag.vm ) )
			{
				return( 0 );
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
			if( tag.vm != -1 && num_bytes > 1024 * tag.vm ) return( 0 );
			byte[ num_bytes ] = '\0';
			num_bytes++;
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
			if( tag.vm != -1 && num_bytes > 16 * tag.vm ) return( 0 );
			byte[ num_bytes ] = '\0';
			num_bytes++;
			break;

		case UI:
			// �t�h�@�t�h�c�@�@�@�@�@�@�U�S�o�C�g�ȉ�
			// ���ڂ̍L����ނ�B��Ɏ��ʂ��邽�߂ɗp������t�h�c���܂ޕ�����B
			// �t�h�c�̓s���I�h "." �����ŕ�����ꂽ�����\���v�f�̃V���[�Y�ł���B
			// ��ȏ�̂t�h�c���܂ޒl�̈�̒�������o�C�g���̏ꍇ�C�l�̈悪�����o�C�g�̒����ł��邱�Ƃ��m�ۂ��邽�߂�
			// ��̖�����NULL �i00H�j�Ŗ��߂���B���S�Ȏd�l�Ɨ�͐߂X�ƕt�����a���Q�ƁB
			if( tag.vm != -1 && num_bytes > 64 * tag.vm ) return( 0 );
			byte[ num_bytes ] = '\0';
			num_bytes++;
			break;

		case UL:
			// �t�k�@�����Ȃ����@�@�@�@�S�o�C�g�Œ�
			// ���������R�Q�r�b�g���Q�i�����B
			// ���L�͈̔͂̐��� n ��\���F 0 ��n�� 2^32 
			if( tag.vm != -1 && ( num_bytes % 4 != 0 || (int)( num_bytes / 4 ) > tag.vm ) )
			{
				return( 0 );
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
			if( tag.vm != -1 && ( num_bytes % 2 != 0 || (int)( num_bytes / 2 ) > tag.vm ) ) return( 0 );

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
			byte[ num_bytes ] = '\0';
			num_bytes++;
			break;

		default:
			return( false );
		}

		return( true );
	}

	class dicom_element
	{
	public:
		dicom_tag tag;
		char *data;
		size_t num_bytes;

		void create( size_t nbytes )
		{
			if( num_bytes != nbytes )
			{
				release();
			}
			num_bytes = nbytes;
			data = new char[ num_bytes + 1 ];
		}

		void release()
		{
			delete [] data;
			data = NULL;
			num_bytes = 0;
		}

		bool operator <( const dicom_element &dicom ) const { return( tag < dicom.tag ); }
		const dicom_element &operator =( const dicom_element &dicom )
		{
			if( &dicom != this )
			{
				tag = dicom.tag;
				create( dicom.num_bytes );
				memcpy( data, dicom.data, sizeof( char ) * num_bytes );
			}
			return( *this );
		}

		// �ϊ��I�y���[�^
		double         to_double( ) const { return( ( tag.vr == FD && num_bytes == 8 )? byte_array< double >( data ).get_value( )        : static_cast< double >        ( atof( to_string( ).c_str( ) ) ) ); }
		float          to_float( )  const { return( ( tag.vr == FL && num_bytes == 4 )? byte_array< float >( data ).get_value( )         : static_cast< float >         ( atof( to_string( ).c_str( ) ) ) ); }
		signed int     to_int( )    const { return( ( tag.vr == SL && num_bytes == 4 )? byte_array< signed int >( data ).get_value( )    : static_cast< signed int >    ( atoi( to_string( ).c_str( ) ) ) ); }
		unsigned int   to_uint( )   const { return( ( tag.vr == UL && num_bytes == 4 )? byte_array< unsigned int >( data ).get_value( )  : static_cast< unsigned int >  ( atoi( to_string( ).c_str( ) ) ) ); }
		signed short   to_short( )  const { return( ( tag.vr == SS && num_bytes == 2 )? byte_array< signed short >( data ).get_value( )  : static_cast< signed short >  ( atoi( to_string( ).c_str( ) ) ) ); }
		unsigned short to_ushort( ) const { return( ( tag.vr == US && num_bytes == 2 )? byte_array< unsigned short >( data ).get_value( ): static_cast< unsigned short >( atoi( to_string( ).c_str( ) ) ) ); }
		std::string    to_string( ) const { return( ( data[ num_bytes - 1 ] == '\0' ) ? data: "" ); }

		dicom_element( ) : tag( ), data( NULL ), num_bytes( 0 )
		{
		}
		dicom_element( const dicom_element &dicom ) : tag( dicom.tag ), data( NULL ), num_bytes( 0 )
		{
			create( dicom.num_bytes );
			memcpy( data, dicom.data, sizeof( char ) * num_bytes );
		}
		dicom_element( unsigned short group, unsigned short element, const char *d = NULL, size_t nbytes = 0 ) : tag( dicom_tag( construct_dicom_tag( group, element ), "", 1, "" ) ), data( NULL ), num_bytes( 0 )
		{
			create( nbytes );
			memcpy( data, d, sizeof( char ) * num_bytes );
		}
		dicom_element( const dicom_tag &t, const char *d = NULL, size_t nbytes = 0 ) : tag( t ), data( NULL ), num_bytes( 0 )
		{
			create( nbytes );
			memcpy( data, d, sizeof( char ) * num_bytes );
		}
	};


	class dicom_tag_container : public std::set< dicom_element >
	{
	private:
		typedef std::set< dicom_element > base;

	public:
		bool add( const dicom_element &element )
		{
			std::pair< base::iterator, bool > ite = base::insert( element );
			return( ite.second );
		}

		void erase( const dicom_element &element )
		{
			base::erase( element );
		}

		base::iterator find( unsigned short group, unsigned short element )
		{
			return( base::find( dicom_element( construct_dicom_tag( group, element ) ) ) );
		}

		dicom_tag_container( )
		{
		}
		dicom_tag_container( const dicom_tag_container &dicom ) : base( dicom )
		{
		}
	};


	inline bool read_dicom_tags( dicom_tag_container &dicom, const std::string &filename )
	{
		FILE *fp = fopen( filename.c_str( ), "rb" );
		if( fp == NULL ) return( false );

		if( fseek( fp, 0, SEEK_END ) != 0 )
		{
			fclose( fp );
			return( false );
		}

		long nFileSize = ftell( fp );

		// �t�@�C���|�C���^��擪�ɖ߂�
		rewind( fp );

		long nPos = 0;
		long nBytes = 0;
		long ret = 0;

		// DICOM�f�[�^�̐擪�܂Ń|�C���^���ړ�
		check_dicom_file( fp );
//		bool is_dicom_file = check_dicom_file( fp );

		dicom.clear( );

		dicom_tag tag;
		char *data = NULL;
		long nbytes = 0;

		while( feof( fp ) == 0 )
		{
			nPos = ftell( fp );
			nBytes = read_dicom_tag( fp, tag );
			if( nBytes < -1 || nPos + nBytes > nFileSize )
			{
				delete [] data;
				fclose( fp );
				return( false );
			}
			else if( nBytes > 0 )
			{
				if( nBytes > nbytes )
				{
					delete [] data;
					data = new char[ nBytes + 1 ];
					nbytes = nBytes;
				}
				fread( data, sizeof( char ), nBytes, fp );
				data[ nBytes ] = '\0';

				ret = process_dicom_tag( tag, data, nBytes );
				if( ret < 0 )
				{
					// �������邱�Ƃ��ł��Ȃ�DICOM�^�O�𔭌������̂ŏI������
					delete [] data;
					fclose( fp );
					return( false );
				}
				else if( ret > 0 )
				{
					// �f�[�^���u�q�̗v���𖞂����ꍇ�̓��X�g�ɒǉ�
					dicom.add( dicom_element( tag, data, nBytes ) );
				}

#ifdef __SHOW_DICOM_TAG__
				switch( tag.vr )
				{
				case FL:
					printf( "( %04x, %04x, %s, %s ) = %f\n", (int)( 0x0000ffff & (tag.tag >> 16 ) ), (int)( 0x0000ffff & tag.tag ), get_dicom_vr( tag.vr ).c_str(), tag.comment.c_str(), byte_array< float >( data ).get_value( ) );
					break;
				case FD:
					printf( "( %04x, %04x, %s, %s ) = %f\n", (int)( 0x0000ffff & (tag.tag >> 16 ) ), (int)( 0x0000ffff & tag.tag ), get_dicom_vr( tag.vr ).c_str(), tag.comment.c_str(), byte_array< double >( data ).get_value( ) );
					break;
				case SL:
					printf( "( %04x, %04x, %s, %s ) = %d\n", (int)( 0x0000ffff & (tag.tag >> 16 ) ), (int)( 0x0000ffff & tag.tag ), get_dicom_vr( tag.vr ).c_str(), tag.comment.c_str(), byte_array< signed int >( data ).get_value( ) );
					break;
				case SS:
					printf( "( %04x, %04x, %s, %s ) = %d\n", (int)( 0x0000ffff & (tag.tag >> 16 ) ), (int)( 0x0000ffff & tag.tag ), get_dicom_vr( tag.vr ).c_str(), tag.comment.c_str(), byte_array< signed short >( data ).get_value( ) );
					break;
				case UL:
					printf( "( %04x, %04x, %s, %s ) = %d\n", (int)( 0x0000ffff & (tag.tag >> 16 ) ), (int)( 0x0000ffff & tag.tag ), get_dicom_vr( tag.vr ).c_str(), tag.comment.c_str(), byte_array< unsigned int >( data ).get_value( ) );
					break;
				case US:
					printf( "( %04x, %04x, %s, %s ) = %d\n", (int)( 0x0000ffff & (tag.tag >> 16 ) ), (int)( 0x0000ffff & tag.tag ), get_dicom_vr( tag.vr ).c_str(), tag.comment.c_str(), byte_array< unsigned short >( data ).get_value( ) );
					break;

				case OB:
				case OW:
				case SQ:
				case UN:
					printf( "( %04x, %04x, %s, %s ) = ...\n", (int)( 0x0000ffff & (tag.tag >> 16 ) ), (int)( 0x0000ffff & tag.tag ), get_dicom_vr( tag.vr ).c_str(), tag.comment.c_str() );
					break;

				default:
					printf( "( %04x, %04x, %s, %s ) = %s\n", (int)( 0x0000ffff & (tag.tag >> 16 ) ), (int)( 0x0000ffff & tag.tag ), get_dicom_vr( tag.vr ).c_str(), tag.comment.c_str(), data );
					break;
				}
#endif
			}
		}

		delete [] data;

		fclose( fp );

		return( true );
	}

}




template < class T, class Allocator >
bool read_dicom( array2< T, Allocator > &image, const std::string &filename )
{
	typedef typename array2< T, Allocator >::value_type value_type;
	typedef typename array2< T, Allocator >::size_type size_type;
	typedef _pixel_converter_< T > pixel_converter;
	typedef typename pixel_converter::color_type color_type;

	__dicom_controller__::dicom_tag_container dicom;
	__dicom_controller__::dicom_tag_container::iterator ite;
	if( !__dicom_controller__::read_dicom_tags( dicom, filename ) )
	{
		return( false );
	}

	size_type height = 0;
	size_type width  = 0;
	double window_level = -500.0;
	double window_width = 1500.0;
	double resoX = 0.625;
	double resoY = 0.625;

	ite = dicom.find( 0x0028, 0x1050 );
	if( ite != dicom.end( ) )
	{
		window_level = ite->to_double( );
	}

	ite = dicom.find( 0x0028, 0x1051 );
	if( ite != dicom.end( ) )
	{
		window_width = ite->to_double( );
	}

	ite = dicom.find( 0x0028, 0x0011 );
	if( ite != dicom.end( ) )
	{
		width = ite->to_ushort( );
	}
	else
	{
		return( false );
	}

	ite = dicom.find( 0x0028, 0x0010 );
	if( ite != dicom.end( ) )
	{
		height = ite->to_ushort( );
	}
	else
	{
		return( false );
	}

	ite = dicom.find( 0x0028, 0x0030 );
	if( ite != dicom.end() )
	{
		sscanf( ite->to_string( ).c_str( ), "%lf\\%lf", &resoX, &resoY );
	}

	ite = dicom.find( 0x7fe0, 0x0010 );
	if( ite != dicom.end( ) )
	{
		image.resize( width, height );
		image.reso1( resoX );
		image.reso2( resoY );
		char *data = ite->data;
		double pix;
		unsigned char pixel;
		for( size_type i = 0 ; i < image.size( ) ; i++ )
		{
			pix = __dicom_controller__::byte_array< short >( data + i * 2 ).get_value( );
			pix = ( ( pix - window_level ) / window_width + 0.5 ) * 255.0;
			pix = pix > 255.0 ? 255.0 : pix;
			pix = pix <   0.0 ?   0.0 : pix;
			pixel = static_cast< unsigned char >( pix );
			image[ i ] = pixel_converter::convert_to_pixel( pixel, pixel, pixel );
		}
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
