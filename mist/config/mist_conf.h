/// @file mist/config/mist_conf.h
//!
//! @brief MIST�̐ݒ�t�@�C��
//!

#ifndef __INCLUDE_MIST_CONF_H__
#define __INCLUDE_MIST_CONF_H__


#include <iostream>


// MIST���O��Ԃ��`���邽�߂̃}�N��
#ifndef _MIST_BEGIN
#define _MIST_BEGIN		namespace mist{		///< @brief MIST���O��Ԃ̎n�܂�
#endif

#ifndef _MIST_END
#define _MIST_END		}					///< @brief MIST���O��Ԃ̏I���
#endif



// mist���O��Ԃ̎n�܂�
_MIST_BEGIN



// Microsoft Visual C++ �̃o�[�W�������`�F�b�N
#if _MSC_VER <= 1200
	// Visual C++6.0
	#define __MIST_MSVC__		6
#elif _MSC_VER > 1200
	// Visual Studio .NET�o�[�W����
	#define __MIST_MSVC__		7
#endif


// Microsoft Visual C++ 6.0 �ł́Cstatic const �����T�|�[�g�̂��߂� enum �ɂ���p
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ <= 6
	#define _MIST_CONST( type, name, value ) enum{ name = value }
#else
	#define _MIST_CONST( type, name, value ) static const type name = value
#endif


// �\���̂̃A���C�����g�����킹�邽�߂̃}�N��
#ifdef __MIST_MSVC__
	#define _MIST_ALIGN( type, alignment ) type
#else
	#define _MIST_ALIGN( type, alignment ) type __attribute__( ( aligned( alignment ) ) )
#endif



// MIST�w�b�_���ŗ��p����ݒ�
#define _MIST_ALLOCATE_TEST_			0

#ifdef __MIST_MSVC__
	#define _MIST_ALLOCATOR_MEMORY_TRIM_	0	///< VC�W����STL�ł̓T�|�[�g���Ă��Ȃ��̂ŃI�t�ɂ���
#else
	#define _MIST_ALLOCATOR_MEMORY_TRIM_	1	///< VC�ȊO��STL�ł̓T�|�[�g���Ă���̂ŃI���ɂ���
#endif


#ifndef NDEBUG

	// �f�o�b�O�p�̐ݒ��S�ăI���ɂ���
	#define _CHECK_ACCESS_VIOLATION_		1	///< �z��v�f�ւ̃A�N�Z�X�ᔽ���`�F�b�N���邩�ǂ���
	#define _CHECK_ARRAY_OPERATION_			1	///< 1�����z��̉��Z�̐��������`�F�b�N���邩�ǂ���
	#define _CHECK_ARRAY1_OPERATION_		1	///< 1�����摜�̉��Z�̐��������`�F�b�N���邩�ǂ���
	#define _CHECK_ARRAY2_OPERATION_		1	///< 2�����摜�̉��Z�̐��������`�F�b�N���邩�ǂ���
	#define _CHECK_ARRAY3_OPERATION_		1	///< 3�����摜�̉��Z�̐��������`�F�b�N���邩�ǂ���

	#define _CHECK_MATRIX_OPERATION_		1	///< �s�񉉎Z�̐��������`�F�b�N���邩�ǂ���
	#define _USE_EXPRESSION_TEMPLATE_		0	///< Expression Template�𗘗p���邩�ǂ���

#else

	// �f�o�b�O�p�̐ݒ��S�ăI�t�ɂ���
	#define _CHECK_ACCESS_VIOLATION_		0	///< �z��v�f�ւ̃A�N�Z�X�ᔽ���`�F�b�N���邩�ǂ���
	#define _CHECK_ARRAY_OPERATION_			0	///< 1�����z��̉��Z�̐��������`�F�b�N���邩�ǂ���
	#define _CHECK_ARRAY1_OPERATION_		0	///< 1�����摜�̉��Z�̐��������`�F�b�N���邩�ǂ���
	#define _CHECK_ARRAY2_OPERATION_		0	///< 2�����摜�̉��Z�̐��������`�F�b�N���邩�ǂ���
	#define _CHECK_ARRAY3_OPERATION_		0	///< 3�����摜�̉��Z�̐��������`�F�b�N���邩�ǂ���

	#define _CHECK_MATRIX_OPERATION_		0	/// <�s�񉉎Z�̐��������`�F�b�N���邩�ǂ���
	#define _USE_EXPRESSION_TEMPLATE_		1	///< Expression Template�𗘗p���邩�ǂ���

#endif


#define _MIST_THREAD_SUPPORT_				1	///< MIST�̊e�A���S���Y���ł̃X���b�h�T�|�[�g�����邩�ǂ����D�T�|�[�g����ꍇ�́CLINUX�n�̏ꍇ�� pthread ���C�u�������K�v

#define _MIST_VECTOR_SUPPORT_				0	///< MIST��VECTOR��L���ɂ���DMATRIX�Ƃ̊|���Z�����T�|�[�g����DSTL��vector�ƃN���X�������Ԃ邽�߁C���O��Ԃ𐳂����C���|�[�g����K�v������D


// �s�񉉎Z�̃I�v�V����
#define _USE_BALANCING_MATRIX_EIGEN_		1	///< �s��̑Ίp�����s�����ƂŌv�Z���x���グ��i�኱���������ڂɐH���j
#define _USE_DIVIDE_AND_CONQUER_SVD_		1	///< ���������@��p���������ȓ��ْl�����𗘗p����i�኱���������ڂɐH���j


/// MIST�̃A���S���Y���S�ʂŗ��p����_�~�[�R�[���o�b�N�t�@���N�^
//!
//! MIST�̃A���S���Y�����񋟂���R�[���o�b�N��0�`100�̊Ԃ̐���Ԃ��C�A���S���Y�����I������ۂ�100�����傫�����l��Ԃ�
//! �܂��C�R�[���o�b�N�t�@���N�^�̖߂�l�� false �̏ꍇ��MIST�̃A���S���Y���͏����𒆒f�ł���ꍇ�͒��f���C�����ɐ����Ԃ��܂��D
//! ���̍ۂɁC�r���̏������ʂ͎����邩�������͈Ӗ��̂Ȃ����ʂƂȂ�\��������܂�
//!
struct __mist_dmy_callback__
{
	/// MIST�̃A���S���Y���̐i�s�󋵂��C0�`100�p�[�Z���g�Ŏ󂯎��C�������Ȃ��֐��D
	//!
	//! @param percent [in] �A���S���Y���̐i�s��
	//! @return false ���[�U�[������̃L�����Z���ɂ��C�A���S���Y���̎��s�𒆎~�D
	//! @return true  �A���S���Y���̎��s���p��
	//!
	bool operator()( double percent ) const { return( true ); }
};


/// MIST�̃A���S���Y���S�ʂŗ��p�\�ȁC�W���o�͌^�_�~�[�R�[���o�b�N�t�@���N�^
//!
//! MIST�̃A���S���Y�����񋟂���R�[���o�b�N��0�`100�̊Ԃ̐���Ԃ��C�A���S���Y�����I������ۂ�100�����傫�����l��Ԃ�
//! �܂��C�R�[���o�b�N�t�@���N�^�̖߂�l�� false �̏ꍇ��MIST�̃A���S���Y���͏����𒆒f�ł���ꍇ�͒��f���C�����ɐ����Ԃ��܂��D
//! ���̍ۂɁC�r���̏������ʂ͎����邩�������͈Ӗ��̂Ȃ����ʂƂȂ�\��������܂�
//!
struct __mist_console_callback__
{
	/// MIST�̃A���S���Y���̐i�s�󋵂��C0�`100�p�[�Z���g�Ŏ󂯎��C�W���o�͂֏o�͂���D
	//!
	//! @param percent [in] �A���S���Y���̐i�s��
	//! @return false ���[�U�[������̃L�����Z���ɂ��C�A���S���Y���̎��s�𒆎~�D
	//! @return true  �A���S���Y���̎��s���p��
	//!
	bool operator()( double percent ) const
	{
		int k3 = static_cast< int >( percent / 100.0 );
		percent -= k3 * 100.0;
		int k2 = static_cast< int >( percent / 10.0 );
		percent -= k2 * 10.0;
		int k1 = static_cast< int >( percent );

		std::cout << "busy... ";
		if( k3 == 0 )
		{
			std::cout << " ";
			if( k2 == 0 )
			{
				std::cout << " " << k1;
			}
			else
			{
				std::cout << k2 << k1;
			}
		}
		else
		{
			std::cout << 1 << k2 << k1;
		}
		if( percent > 100.0 )
		{
			std::cout << "%" << std::endl;
		}
		else
		{
			std::cout << "%\r";
		}
		return( true );
	}
};




#if _CHECK_ACCESS_VIOLATION_

	/// DEBUG��MIST���r���h�����ۂɁC1�����A�N�Z�X�ł͈̔͊O�A�N�Z�X���`�F�b�N���C�G���[�̍ۂɌĂ΂��֐��D
	//!
	//! @param index [in] MIST�R���e�i�ɑ΂���A�N�Z�X�v���ʒu
	//!
	inline void mist_debug_assertion( ptrdiff_t index )
	{
		::std::cout << "Access Violation at ( " << static_cast< int >( index ) << " )" << ::std::endl;
	}

	/// DEBUG��MIST���r���h�����ۂɁC2�����A�N�Z�X�ł͈̔͊O�A�N�Z�X���`�F�b�N���C�G���[�̍ۂɌĂ΂��֐��D
	//!
	//! @param index1 [in] MIST�R���e�i�ɑ΂���X���ł̃A�N�Z�X�v���ʒu
	//! @param index2 [in] MIST�R���e�i�ɑ΂���Y���ł̃A�N�Z�X�v���ʒu
	//!
	inline void mist_debug_assertion( ptrdiff_t index1, ptrdiff_t index2 )
	{
		::std::cout << "Access Violation at ( " << static_cast< int >( index1 ) << ", " << static_cast< int >( index2 ) << " )" << ::std::endl;
	}

	/// DEBUG��MIST���r���h�����ۂɁC3�����A�N�Z�X�ł͈̔͊O�A�N�Z�X���`�F�b�N���C�G���[�̍ۂɌĂ΂��֐��D
	//!
	//! @param index1 [in] MIST�R���e�i�ɑ΂���X���ł̃A�N�Z�X�v���ʒu
	//! @param index2 [in] MIST�R���e�i�ɑ΂���Y���ł̃A�N�Z�X�v���ʒu
	//! @param index3 [in] MIST�R���e�i�ɑ΂���Z���ł̃A�N�Z�X�v���ʒu
	//!
	inline void mist_debug_assertion( ptrdiff_t index1, ptrdiff_t index2, ptrdiff_t index3 )
	{
		::std::cout << "Access Violation at ( " << static_cast< int >( index1 ) << ", " << static_cast< int >( index2 ) << ", " << static_cast< int >( index3 ) << " )" << ::std::endl;
	}


	#define _CHECK_ACCESS_VIOLATION1_( index ) \
		if( index < 0 || index >= size_ )\
		{\
			static value_type dmy;\
			mist_debug_assertion( index );\
			return( dmy );\
		}\

	#define _CHECK_ACCESS_VIOLATION2_( index1, index2 ) \
		if( index1 < 0 || index1 >= size1_ || index2 < 0 || index2 >= size2_ )\
		{\
			static value_type dmy;\
			mist_debug_assertion( index1, index2 );\
			return( dmy );\
		}\

	#define _CHECK_ACCESS_VIOLATION3_( index1, index2, index3 ) \
		if( index1 < 0 || index1 >= size1_ || index2 < 0 || index2 >= size2_ || index3 < 0 || index3 >= size3_ )\
		{\
			static value_type dmy;\
			mist_debug_assertion( index1, index2, index3 );\
			return( dmy );\
		}\

#else

	#define _CHECK_ACCESS_VIOLATION1_( index ) 
	#define _CHECK_ACCESS_VIOLATION2_( index1, index2 )
	#define _CHECK_ACCESS_VIOLATION3_( index1, index2, index3 )

#endif



/*** Doxygen �p�̃��C���y�[�W ***********************************************************************************************/
/*! @mainpage MIST ( Media Integration Standard Toolkit )
*
* @section class MIST�Œ񋟂����{�f�[�^�^
*
* - @ref mist::array "array"   : STL�ɏ���������������ŘA���ƂȂ�P�����z��ł���C�����E�摜�̑S�Ẵf�[�^�̊��N���X�D
* - @ref mist::array1 "array1" : �������������߂̂P�����z��D
* - @ref mist::array2 "array2" : �Q�����摜���������߂̉摜�N���X�D
* - @ref mist::array3 "array3" : �R�����摜���������߂̉摜�N���X�D
* - @ref mist::matrix "matrix" : �C�ӂ̍s����������߂̃N���X�iExpression template teqnique �𗘗p���������ȉ��Z���\�j�D
* - @ref mist::vector "vector" : �s��E�x�N�g�����Z���\�Ƃ���N���X�D
*
*
*
* @section algorithm MIST�Œ񋟂���A���S���Y��
*
* @subsection implement	�����⏕
* - @ref mist::type_limits					"�^�ɑ΂�����"
* - @ref mist::timer						"���Ԍv��"
* - @ref operator_group						"���Z�q�̎����⏕"
* - @ref thread_group						"�X���b�h"
*
* @subsection numeric �x�N�g���E�s�񉉎Z
* - @ref numeric_group "�s�񉉎Z"
*   - @ref mist::inverse					"�t�s����v�Z"
*   - @ref mist::solve						"�A��������������"
*   - @ref mist::qr_factorization			"QR ����"
*   - @ref mist::lu_factorization			"LU ����"
*   - @ref mist::eigen						"�ŗL�l�E�ŗL�x�N�g��"
*   - @ref mist::svd						"���ْl����"
*
* @subsection common ���ʂ̏���
* - @ref fourier_group						"�t�[���G�ϊ�"
*   - @ref fft_group						"�����t�[���G�ϊ� (FFT)"
*   - @ref dct_group						"���U�R�T�C���ϊ�(DCT)"
*   - @ref dst_group						"���U�T�C���ϊ� (DST) "
* - @ref a									"�E�F�[�u���b�g�ϊ�"
* - @ref a									"���v����"
* - @ref a									"�q�X�g�O��������"
* - @ref mist::random						"�[����������"
* - @ref a									"�O���t�`��(2����)"
* - @ref draw_group							"2�����摜�`��"
*
* @subsection image ��ɉ摜�ɑ΂��ēK�p����鏈��
*
* @subsubsection image-io ���o�͌n
* - @ref image_group						"�C�ӂ̉摜�f�[�^�̓ǂݍ��݁E�����o��"
*   - @ref image_raw_group					"RAW�f�[�^�̓ǂݍ��݁E�����o��"
*   - @ref image_bmp_group					"BMP�f�[�^�̓ǂݍ��݁E�����o��"
*   - @ref image_pnm_group					"PNM�f�[�^�̓ǂݍ��݁E�����o��"
*   - @ref image_jpeg_group					"JPEG�f�[�^�̓ǂݍ��݁E�����o��"
*   - @ref image_png_group					"PNG�f�[�^�̓ǂݍ��݁E�����o��"
*   - @ref image_tiff_group					"TIFF�f�[�^�̓ǂݍ��݁E�����o��"
*   - @ref image_dicom_group				"DICOM�f�[�^�̓ǂݍ��݁E�����o��"
*
* @subsubsection image-filter �t�B���^�n
* - @ref linear_group						"���`�t�B���^"
* - @ref median_group						"���f�B�A���t�B���^"
* - @ref interpolate_group					"�摜���"
* - @ref a									"�J���[�摜�ɑ΂��鏈��"
* - @ref threshold_group					"臒l�I��"
* - @ref a									"2�l�摜�ɑ΂��鏈��"
* - @ref morphology_group					"�����t�H���W�[���Z"
*
* @subsection audio ��ɉ����ɑ΂��ēK�p����鏈��
*
* @subsubsection audio-io ���o�͌n
* - @ref audio_group						"�����f�[�^�̓ǂݍ��݁E�����o��"
*   - @ref audio_wav_group					"WAV�f�[�^�̓ǂݍ��݁E�����o��"
*
* @subsubsection audio-filter �t�B���^�n
* - @ref a									"�T���v�����O���[�g�ύX"
* - @ref a									"�Đ�"
*
****************************************************************************************************************************/





// mist���O��Ԃ̏I���
_MIST_END


#endif
