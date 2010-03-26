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

/// @file mist/config/mist_conf.h
//!
//! @brief MIST�̐ݒ�t�@�C��
//!

#ifndef __INCLUDE_MIST_CONF_H__
#define __INCLUDE_MIST_CONF_H__

#include <iostream>
#include <cstdlib>
#include <string>


// MIST���O��Ԃ��`���邽�߂̃}�N��
#ifndef _MIST_BEGIN
/// @namespace mist
//!
//! MIST�v���W�F�N�g�̖��O��ԁD
//! ���ׂĂ̊֐��E�N���X�� mist ���O��ԂɊ܂܂��D
//! 
#define _MIST_BEGIN		namespace mist{		///< @brief MIST���O��Ԃ̎n�܂�
#endif

#ifndef _MIST_END
#define _MIST_END		}					///< @brief MIST���O��Ԃ̏I���
#endif



// mist���O��Ԃ̎n�܂�
_MIST_BEGIN



// Microsoft Visual C++ �̃o�[�W�������`�F�b�N
#if defined( _MSC_VER )
	#if _MSC_VER <= 1200
		// Visual C++6.0
		#define __MIST_MSVC__		6
	#elif _MSC_VER < 1400
		// Visual Studio .NET 2002 - 2003�o�[�W����
		#define __MIST_MSVC__		7
	#else
		// Visual Studio 2005 �o�[�W����
		#define __MIST_MSVC__		8
	#endif
#endif


#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ >= 8
	// �Z�L�����e�B��̗��R�ŁAsprintf �� fopen �Ȃǂ̊֐��Ăяo�����x���ƂȂ���ւ̑Ώ�
	// �����ł̐ݒ�́AMIST���C���N���[�h���邷�ׂẴt�@�C���ɉe��������܂�
	//
	// ���̂����ꂩ��I������

	// �������s���Ȃ̂ŁA���̂Ƃ���x���𖳎�����
	#pragma warning( disable : 4996 )

	//#undef _CRT_SECURE_CPP_OVERLOAD_SECURE_NAMES
	//#define _CRT_SECURE_CPP_OVERLOAD_SECURE_NAMES		0
	////#define _CRT_SECURE_NO_DEPRECATE					1	// �Z�L�����e�B���ア�Â��֐��𗘗p����
	//#undef _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES
	//#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES		1	// �Z�L�����e�B���������ꂽ�֐��𗘗p����i���s�Ɏ��Ԃ��K�v�ƂȂ�\������j
#endif

// Microsoft Windows ���ǂ������`�F�b�N
#if defined( WIN64 ) || defined( _WIN64 )
	// 64bit Windows
	#define __MIST_WINDOWS__	2
#elif defined( WIN32 ) || defined( _WIN32 )
	// 32bit Windows
	#define __MIST_WINDOWS__	1
#endif

#if defined( _WIN64 ) || defined( __x86_64__ ) || defined( __AMD64__ )
	// 64bit OS
	#define __MIST64__	64
#else
	// 32bit OS
	#define __MIST32__	32
#endif



// Microsoft Visual C++ 6.0 �ł́Cstatic const �𖢃T�|�[�g�̂��߁Cenum �ő�p����
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ <= 6
	#define _MIST_CONST( type, name, value ) enum{ name = value }
#else
	#define _MIST_CONST( type, name, value ) static const type name = value
#endif


// �\���̂̃A���C�����g�����킹�邽�߂̃}�N��
#if defined(__MIST_MSVC__) || defined(__INTEL_COMPILER)
	#define _MIST_PACKED
#else
	#define _MIST_PACKED __attribute__( ( packed ) )
#endif



// MIST�w�b�_���ŗ��p����ݒ�
#define _MIST_ALLOCATE_TEST_			0

//#ifdef __MIST_MSVC__
//	#define _MIST_ALLOCATOR_MEMORY_TRIM_	0		///< VC�W����STL�ł̓T�|�[�g���Ă��Ȃ��̂ŃI�t�ɂ���
//#else
//	#define _MIST_ALLOCATOR_MEMORY_TRIM_	1		///< VC�ȊO��STL�ł̓T�|�[�g���Ă���̂ŃI���ɂ���
//#endif

#define _MIST_ALLOCATOR_MEMORY_TRIM_	0			///< ���łɊm�ۂ���Ă��郁�����̈�̐؂�l�߂��s���A���P�[�^���T�|�[�g����ꍇ�̓I���ɂ���

#define _MIST_UNUSED_( arg )	( void )( arg )		///< ���g�p�̕ϐ��Ɋւ���x����\�����Ȃ����邽�߂̃}�N��

#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ <= 6


// mist���O��Ԃ̏I���
_MIST_END

	#include <cmath>

	// VC6�ł́ASTD���O��Ԃ�ABS�֐����������Ă��Ȃ������C��
	#define STD_CMATH_FUNC1( name, func, value ) inline value name ( value v ){ return( ::func( v ) ); }
	#define STD_CMATH_FUNC2( name, func, value ) inline value name ( value v1, value v2 ){ return( ::func( v1, v2 ) ); }
	namespace std
	{
		STD_CMATH_FUNC1(   abs,    abs, int )
		STD_CMATH_FUNC1(   abs,   labs, long )
		STD_CMATH_FUNC1(   abs,   fabs, double )
		STD_CMATH_FUNC1(  sqrt,   sqrt, double )
		STD_CMATH_FUNC1(   sin,    sin, double )
		STD_CMATH_FUNC1(   cos,    cos, double )
		STD_CMATH_FUNC1(   log,    log, double )
		STD_CMATH_FUNC1( log10,  log10, double )
		STD_CMATH_FUNC1(   exp,    exp, double )
		STD_CMATH_FUNC2(   pow,    pow, double )
	}
	#undef STD_CMATH_FUNC1
	#undef STD_CMATH_FUNC2

// mist���O��Ԃ̎n�܂�
_MIST_BEGIN

#endif


//! @addtogroup config_group MIST�S�ʂŗ��p�����ݒ�
//!
//! @code ���̃w�b�_���Q��
//! #include <mist/config/mist_conf.h>
//! @endcode
//!
//! @{


#ifndef _MIST_USER_CONFIG_

#if !defined( NDEBUG ) || defined( DEBUG )

	// �f�o�b�O�p�̐ݒ��S�ăI���ɂ���
	#define _CHECK_ACCESS_VIOLATION_		1	///< �z��v�f�ւ̃A�N�Z�X�ᔽ���`�F�b�N���邩�ǂ���
	#define _CHECK_ARRAY_OPERATION_			1	///< 1�����z��̉��Z�̐��������`�F�b�N���邩�ǂ���
	#define _CHECK_ARRAY1_OPERATION_		1	///< 1�����摜�̉��Z�̐��������`�F�b�N���邩�ǂ���
	#define _CHECK_ARRAY2_OPERATION_		1	///< 2�����摜�̉��Z�̐��������`�F�b�N���邩�ǂ���
	#define _CHECK_ARRAY3_OPERATION_		1	///< 3�����摜�̉��Z�̐��������`�F�b�N���邩�ǂ���

	#define _CHECK_MATRIX_OPERATION_		1	///< �s�񉉎Z�̐��������`�F�b�N���邩�ǂ���

#else

	// �f�o�b�O�p�̐ݒ��S�ăI�t�ɂ���
	#define _CHECK_ACCESS_VIOLATION_		0	///< �z��v�f�ւ̃A�N�Z�X�ᔽ���`�F�b�N���邩�ǂ���
	#define _CHECK_ARRAY_OPERATION_			0	///< 1�����z��̉��Z�̐��������`�F�b�N���邩�ǂ���
	#define _CHECK_ARRAY1_OPERATION_		0	///< 1�����摜�̉��Z�̐��������`�F�b�N���邩�ǂ���
	#define _CHECK_ARRAY2_OPERATION_		0	///< 2�����摜�̉��Z�̐��������`�F�b�N���邩�ǂ���
	#define _CHECK_ARRAY3_OPERATION_		0	///< 3�����摜�̉��Z�̐��������`�F�b�N���邩�ǂ���

	#define _CHECK_MATRIX_OPERATION_		0	/// <�s�񉉎Z�̐��������`�F�b�N���邩�ǂ���

#endif


#define __CHECK_HARD_CASE__					0	///< CONDOR �A���S���Y���ɂ����āCTrust Region �������ۂ� Hard Case �̃`�F�b�N�����邩�ǂ����D�`�F�b�N������ق������肵���œK�����\�ɂȂ�ꍇ������܂��D

#define _ARRAY_BIND_OPERATION_SUPPORT_		1	///< MIST�� array  �R���e�i���m�̎l�����Z��L���ɂ��邩�ǂ���
#define _ARRAY1_BIND_OPERATION_SUPPORT_		1	///< MIST�� array1 �R���e�i���m�̎l�����Z��L���ɂ��邩�ǂ���
#define _ARRAY2_BIND_OPERATION_SUPPORT_		1	///< MIST�� array2 �R���e�i���m�̎l�����Z��L���ɂ��邩�ǂ���
#define _ARRAY3_BIND_OPERATION_SUPPORT_		1	///< MIST�� array3 �R���e�i���m�̎l�����Z��L���ɂ��邩�ǂ���


#define _MIST_THREAD_SUPPORT_				1	///< MIST�̊e�A���S���Y���ł̃X���b�h�T�|�[�g�����邩�ǂ����D�T�|�[�g����ꍇ�́CLINUX�n�̏ꍇ�� pthread ���C�u�������K�v

#define _MIST_VECTOR_SUPPORT_				0	///< MIST��VECTOR��L���ɂ���DMATRIX�Ƃ̊|���Z�����T�|�[�g����DSTL��vector�ƃN���X�������Ԃ邽�߁C���O��Ԃ𐳂����C���|�[�g����K�v������D

#define _LZW_COMPRESSION_SUPPORT_		false	///< TIFF�̏o�͂ŁCLZW���k��L���ɂ���I�v�V�������f�t�H���g�ɂ��邩�ǂ���

#define _COMPRESSED_CSV_SUPPORT_			0	///< ZLIB���k���ꂽCSV�t�@�C���̓ǂݍ��݂��T�|�[�g���邩�ǂ���


// �s�񉉎Z�̃I�v�V����
#define _USE_INTEL_MATH_KERNEL_LIBRARY_		0	///< �C���e�����񋟂��Ă��� Math Kernel Library �𗘗p����ꍇ�� 1 �ɂ���
#define _DESCENDING_ORDER_EIGEN_VALUE_		0	///< �ŗL�l�E�ŗL�x�N�g�����v�Z�������ɁC�~���ɕ��ׂ鎞�� 1�C�����ɕ��ׂ鎞�� 0 �ɂ���
#define _USE_BALANCING_MATRIX_EIGEN_		1	///< �s��̑Ίp�����s�����ƂŌv�Z���x���グ��i�኱���������ڂɐH���j
#define _USE_DIVIDE_AND_CONQUER_SVD_		1	///< ���������@��p���������ȓ��ْl�����𗘗p����i�኱���������ڂɐH���j

#endif


/// @}
//  MIST�S�ʂŗ��p�����ݒ�̏I���


/// @brief ���͂��ꂽ2�̃I�u�W�F�N�g���������́i��������œ����ʒu�ɂ���j���ǂ����𔻒肷��
//!
//! @param[in] o1 �c �I�u�W�F�N�g1
//! @param[in] o2 �c �I�u�W�F�N�g2
//!
//! @return true  �c ���͂��ꂽ2�̃I�u�W�F�N�g�́C��������œ����ʒu�ɑ��݂��Ă���
//! @return false �c ���͂��ꂽ2�̃I�u�W�F�N�g�́C�ʂ̃�������ɑ��݂��Ă���
//!
template < class Object1, class Object2 >
inline bool is_same_object( const Object1 &o1, const Object2 &o2 )
{
	return( reinterpret_cast< const void * >( &o1 ) == reinterpret_cast< const void * >( &o2 ) );
}

/// @brief UNICODE��������}���`�o�C�g������ɕϊ�����
//!
//! @param[in] src �c UNICODE������
//!
//! @return �}���`�o�C�g������
//!
inline const std::string wstr2str( const std::wstring &src )
{
	static bool is_initialized = false;
	if( !is_initialized )
	{
		is_initialized = true;
		setlocale( LC_CTYPE, "" );
	}

	size_t nbytes = wcstombs( NULL, src.c_str( ), 0 );
	char *buff = new char[ nbytes + 1 ];
	wcstombs( buff, src.c_str( ), nbytes + 1 );
	std::string dst( buff );
	delete [] buff;
	return( dst );
}



//! @addtogroup callback_group MIST�S�ʂŗ��p�ł���R�[���o�b�N�֐�
//!
//! @code ���̃w�b�_���Q��
//! #include <mist/config.h>
//! @endcode
//!
//! @{


/// @brief MIST�̃A���S���Y���S�ʂŗ��p����_�~�[�R�[���o�b�N�t�@���N�^
//!
//! MIST�̃A���S���Y�����񋟂���R�[���o�b�N��0�`100�̊Ԃ̐���Ԃ��C�A���S���Y�����I������ۂ�100�����傫�����l��Ԃ�
//! �܂��C�R�[���o�b�N�t�@���N�^�̖߂�l�� false �̏ꍇ��MIST�̃A���S���Y���͏����𒆒f�ł���ꍇ�͒��f���C�����ɐ����Ԃ��܂��D
//! ���̍ۂɁC�r���̏������ʂ͎����邩�������͈Ӗ��̂Ȃ����ʂƂȂ�\��������܂�
//!
struct __mist_dmy_callback__
{
	/// @brief MIST�̃A���S���Y���̐i�s�󋵂��C0�`100�p�[�Z���g�Ŏ󂯎��C�������Ȃ��֐��D
	//!
	//! @param[in] percent �c �A���S���Y���̐i�s��
	//!
	//! @return true  �c �A���S���Y���̎��s���p��
	//! @return false �c ���[�U�[������̃L�����Z���ɂ��C�A���S���Y���̎��s�𒆎~
	//!
	bool operator()( long double /* percent */ ) const { return( true ); }
};


/// @brief �w�肵���R�[���o�b�N�̖߂�l [0,100] ���w�肵����Ԃɕϊ�����
//!
//! MIST�̃A���S���Y�����񋟂���R�[���o�b�N��0�`100�̊Ԃ̐���Ԃ��C�A���S���Y�����I������ۂ�100�����傫�����l��Ԃ�
//! �܂��C�R�[���o�b�N�t�@���N�^�̖߂�l�� false �̏ꍇ��MIST�̃A���S���Y���͏����𒆒f�ł���ꍇ�͒��f���C�����ɐ����Ԃ��܂��D
//! ���̍ۂɁC�r���̏������ʂ͎����邩�������͈Ӗ��̂Ȃ����ʂƂȂ�\��������܂�
//!
template < class Functor >
struct __mist_convert_callback__
{
	Functor f_;
	double lower_;
	double upper_;

	/// @brief MIST�̃A���S���Y���̐i�s�󋵂��C0�`100�p�[�Z���g�Ŏ󂯎��C�w�肵����Ԃɕϊ�����֐��D
	//!
	//! @param[in] percent �c �A���S���Y���̐i�s��
	//!
	//! @return true  �c �A���S���Y���̎��s���p��
	//! @return false �c ���[�U�[������̃L�����Z���ɂ��C�A���S���Y���̎��s�𒆎~
	//!
	bool operator()( long double percent )
	{
		percent = lower_ + percent / 100.0 * ( upper_ - lower_ );
		return( f_( percent ) );
	}

	__mist_convert_callback__( ) : lower_( 0.0 ), upper_( 100.0 ){ }
	__mist_convert_callback__( Functor f, double l = 0.0, double u = 100.0 ) : f_( f ), lower_( l ), upper_( u ){ }
};


/// @brief MIST�̃A���S���Y���S�ʂŗ��p�\�ȁC�W���o�͌^�R�[���o�b�N�t�@���N�^
//!
//! @code �i�s�󋵂̕\����
//! busy... 99%
//! @endcode
//!
struct __mist_console_callback__
{
private:
	int __percent__;

public:
	/// @brief �W���o�͌^�R�[���o�b�N�t�@���N�^�̃f�t�H���g�R���X�g���N�^�D
	__mist_console_callback__( ) : __percent__( -1 )
	{
	}

	/// @brief MIST�̃A���S���Y���̐i�s�󋵂��C0�`100�p�[�Z���g�Ŏ󂯎��C�W���o�͂֏o�͂���D
	//!
	//! @param[in] percent �c �A���S���Y���̐i�s��
	//!
	//! @return true  �c �A���S���Y���̎��s���p��
	//! @return false �c ���[�U�[������̃L�����Z���ɂ��C�A���S���Y���̎��s�𒆎~
	//!
	bool operator()( long double percent )
	{
		int ppp = static_cast< int >( percent );
		if( __percent__ != ppp )
		{
			__percent__ = ppp;

			int k3 = static_cast< int >( percent / 100.0 );
			percent -= k3 * 100.0;
			int k2 = static_cast< int >( percent / 10.0 );
			percent -= k2 * 10.0;
			int k1 = static_cast< int >( percent );

			std::cerr << "busy... ";
			if( k3 == 0 )
			{
				std::cerr << " ";
				if( k2 == 0 )
				{
					std::cerr << " " << k1;
				}
				else
				{
					std::cerr << k2 << k1;
				}
			}
			else
			{
				std::cerr << 1 << k2 << k1;
			}
			if( percent > 100.0 )
			{
				std::cerr << "%" << std::endl;
			}
			else
			{
				std::cerr << "%\r";
			}
		}
		return( true );
	}
};




/// @brief MIST�̃A���S���Y���S�ʂŗ��p�\�ȁC�i�s�󋵕\���^�_�~�[�R�[���o�b�N�t�@���N�^
//!
//! �i�s�󋵂̃o�[�́C�R���X�g���N�^���Ăяo���ꂽ�ۂɁC�R���\�[���ɕ\�������
//!
//! @code �i�s�󋵂̕\����
//! 0%   10   20   30   40   50   60   70   80   90   100%
//! |----|----|----|----|----|----|----|----|----|----|
//! *********************************
//! @endcode
//!
struct __mist_progress_callback__
{
	__mist_progress_callback__( )
	{
		std::cerr << "0%   10   20   30   40   50   60   70   80   90   100%" << std::endl;
		std::cerr << "|----|----|----|----|----|----|----|----|----|----|" << std::endl;
	}

	/// @brief MIST�̃A���S���Y���̐i�s�󋵂��C0�`100�p�[�Z���g�Ŏ󂯎��C�W���o�͂֏o�͂���D
	//!
	//! @param[in] percent �c �A���S���Y���̐i�s��
	//!
	//! @return true  �c �A���S���Y���̎��s���p��
	//! @return false �c ���[�U�[������̃L�����Z���ɂ��C�A���S���Y���̎��s�𒆎~
	//!
	bool operator()( long double percent ) const
	{
		if( percent > 100.0 )
		{
			std::cerr << "***************************************************" << std::endl;
		}
		else
		{
			int i, p = static_cast< int >( percent / 2.0 );

			for( i = 0 ; i < p ; i++ )
			{
				std::cerr << "*";
			}
			for( ; i < 51 ; i++ )
			{
				std::cerr << " ";
			}

			std::cerr << "\r";
		}

		return( true );
	}
};



/// @}
//  MIST�S�ʂŗ��p�ł���R�[���o�b�N�֐�



#if _CHECK_ACCESS_VIOLATION_

	/// @brief DEBUG��MIST���r���h�����ۂɁC1�����A�N�Z�X�ł͈̔͊O�A�N�Z�X���`�F�b�N���C�G���[�̍ۂɌĂ΂��֐��D
	//!
	//! @param[in] index �c MIST�R���e�i�ɑ΂���A�N�Z�X�v���ʒu
	//!
	inline void mist_debug_assertion( ptrdiff_t index )
	{
		::std::cerr << "Access Violation at ( " << static_cast< int >( index ) << " )" << ::std::endl;
	}


	/// @brief DEBUG��MIST���r���h�����ۂɁC2�����A�N�Z�X�ł͈̔͊O�A�N�Z�X���`�F�b�N���C�G���[�̍ۂɌĂ΂��֐��D
	//!
	//! @param[in] index1 �c MIST�R���e�i�ɑ΂���X���ł̃A�N�Z�X�v���ʒu
	//! @param[in] index2 �c MIST�R���e�i�ɑ΂���Y���ł̃A�N�Z�X�v���ʒu
	//!
	inline void mist_debug_assertion( ptrdiff_t index1, ptrdiff_t index2 )
	{
		::std::cerr << "Access Violation at ( " << static_cast< int >( index1 ) << ", " << static_cast< int >( index2 ) << " )" << ::std::endl;
	}


	/// @brief DEBUG��MIST���r���h�����ۂɁC3�����A�N�Z�X�ł͈̔͊O�A�N�Z�X���`�F�b�N���C�G���[�̍ۂɌĂ΂��֐��D
	//!
	//! @param[in] index1 �c MIST�R���e�i�ɑ΂���X���ł̃A�N�Z�X�v���ʒu
	//! @param[in] index2 �c MIST�R���e�i�ɑ΂���Y���ł̃A�N�Z�X�v���ʒu
	//! @param[in] index3 �c MIST�R���e�i�ɑ΂���Z���ł̃A�N�Z�X�v���ʒu
	//!
	inline void mist_debug_assertion( ptrdiff_t index1, ptrdiff_t index2, ptrdiff_t index3 )
	{
		::std::cerr << "Access Violation at ( " << static_cast< int >( index1 ) << ", " << static_cast< int >( index2 ) << ", " << static_cast< int >( index3 ) << " )" << ::std::endl;
	}


	#define _CHECK_ACCESS_VIOLATION1U_( index ) \
		if( index >= size( ) )\
		{\
			static value_type dmy;\
			mist_debug_assertion( index );\
			return( dmy );\
		}

	#define _CHECK_ACCESS_VIOLATION1S_( index ) \
		if( index < 0 || index >= size( ) )\
		{\
			static value_type dmy;\
			mist_debug_assertion( index );\
			return( dmy );\
		}

	#define _CHECK_ACCESS_VIOLATION2U_( index1, index2 ) \
		if( index1 >= size1( ) || index2 >= size2( ) )\
		{\
			static value_type dmy;\
			mist_debug_assertion( index1, index2 );\
			return( dmy );\
		}

	#define _CHECK_ACCESS_VIOLATION2S_( index1, index2 ) \
		if( index1 < 0 || index1 >= size1( ) || index2 < 0 || index2 >= size2( ) )\
		{\
			static value_type dmy;\
			mist_debug_assertion( index1, index2 );\
			return( dmy );\
		}

	#define _CHECK_ACCESS_VIOLATION3U_( index1, index2, index3 ) \
		if( index1 >= size1( ) || index2 >= size2( ) || index3 >= size3( ) )\
		{\
			static value_type dmy;\
			mist_debug_assertion( index1, index2, index3 );\
			return( dmy );\
		}

	#define _CHECK_ACCESS_VIOLATION3S_( index1, index2, index3 ) \
		if( index1 < 0 || index1 >= size1( ) || index2 < 0 || index2 >= size2( ) || index3 < 0 || index3 >= size3( ) )\
		{\
			static value_type dmy;\
			mist_debug_assertion( index1, index2, index3 );\
			return( dmy );\
		}

#else

	#define _CHECK_ACCESS_VIOLATION1U_( index ) 
	#define _CHECK_ACCESS_VIOLATION1S_( index ) 
	#define _CHECK_ACCESS_VIOLATION2U_( index1, index2 )
	#define _CHECK_ACCESS_VIOLATION2S_( index1, index2 )
	#define _CHECK_ACCESS_VIOLATION3U_( index1, index2, index3 )
	#define _CHECK_ACCESS_VIOLATION3S_( index1, index2, index3 )

#endif



/*** Doxygen �p�̃��C���y�[�W ***********************************************************************************************/
/*! @mainpage MIST ( Media Integration Standard Toolkit )
*
*
* @section config MIST�̊�{�ݒ�
*
* - @ref config_group			"��{�ݒ�"
* - @ref callback_group			"MIST�S�ʂŗ��p�ł���R�[���o�b�N�֐�
*
* @section class MIST�Œ񋟂����{�R���e�i
*
* - @ref mist::array			"array"          : STL�ɏ���������������ŘA���ƂȂ�1�����z��ł���C�����E�摜�̑S�Ẵf�[�^�̊��N���X
* - @ref mist::array1			"array1"         : �������������߂�1�����z��
* - @ref mist::array2			"array2"         : 2�����摜���������߂̉摜�N���X
* - @ref mist::array3			"array3"         : 3�����摜���������߂̉摜�N���X
* - @ref mist::bitmap			"bitmap"         : �r�b�g�}�b�v�摜���������߂̃N���X
* - @ref mist::marray			"marray"         : 1�E2�E3�����摜�̉��Ƀ}�[�W�����������摜�������N���X
* - @ref mist::buffered_array	"buffered_array" : �摜�̈�ӂ�2�̎w����ƂȂ�摜�������N���X
* - @ref mist::matrix			"matrix"         : �C�ӂ̍s����������߂̃N���X�iExpression template �𗘗p���������ȉ��Z���\�j
* - @ref vector_group			"�x�N�g�����Z���\�Ƃ���N���X"
*   - @ref mist::vector2		"vector2"        : 2�����x�N�g���������N���X
*   - @ref mist::vector3		"vector3"        : 3�����x�N�g���������N���X
*   - @ref mist::vector			"vector"         : N�����x�N�g���������N���X
*
*
* @section element MIST�Œ񋟂����{�f�[�^�^
* - @ref mist::binary						"�o�C�i����f"
* - @ref color_group						"�F��Ԃ������N���X"
*   - @ref mist::rgb						"�J���[��f"
*   - @ref mist::bgr						"Windows�p�̃r�b�g�}�b�v�ŗ��p�����J���[��f"
*   - @ref mist::rgba						"�J���[��f�i�A���t�@�t���j"
* - @ref mist::stereo						"�X�e���I����"
* - @ref mist::quaternion					"�N�H�[�^�j�I���i�l�����j"
* - @ref set_group							"�W��"
* - @ref facet_group						"�|���S���֘A"
*   - @ref mist::facet						"�|���S���������N���X"
*   - @ref mist::facet_list					"�|���S���̏W���������N���X"
*   - @ref mist::surface_simplification		"�|���S�����̍팸"
*
*
* @section algorithm MIST�Œ񋟂���A���S���Y��
*
*
* @subsection implement	�����⏕
* - @ref limits_group						"�^�ɑ΂�����"
* - @ref pointer_group						"�����I�Ƀ������Ǘ����s���|�C���^"
* - @ref mist::timer						"���Ԍv��"
* - @ref environment_group					"�v�Z�@�����"
* - @ref operator_group						"���Z�q�̎����⏕"
* - @ref thread_group						"�}���`�X���b�h"
*   - @ref mist::thread_pool				"�X���b�h�v�[��"
*   - @ref mist::singleton					"�V���O���g��"
* - @ref free_form_group					"���R�Ȑ��E�Ȗ�"
*   - @ref bspline_group					"B�X�v���C���Ȑ�"
*   - @ref spline_group						"3���X�v���C���Ȑ�"
* - @ref mist::crc							"CRC"
* - @ref hash_group							"�n�b�V���֐�"
*   - @ref mist::md2						"MD2"
*   - @ref mist::md4						"MD4"
*   - @ref mist::md5						"MD5"
*   - @ref mist::sha1						"SHA-1"
*   - @ref mist::sha256						"SHA-256"
*   - @ref mist::sha384						"SHA-384"
*   - @ref mist::sha512						"SHA-512"
*
*
* @subsection numeric �x�N�g���E�s�񉉎Z
* - @ref numeric_group "�s�񉉎Z"
*   - @ref mist::trace						"�g���[�X"
*   - @ref mist::det						"�s��"
*   - @ref mist::inverse					"�t�s��"
*   - @ref mist::solve						"�A��������������"
*   - @ref mist::qr_factorization			"QR ����"
*   - @ref mist::lu_factorization			"LU ����"
*   - @ref mist::eigen						"�ŗL�l�E�ŗL�x�N�g��"
*   - @ref mist::svd						"���ْl����"
*
*
* @subsection common ���ʂ̏���
* - @ref fourier_group							"�t�[���G�ϊ�"
*   - @ref fft_group							"�����t�[���G�ϊ� (FFT)"
*   - @ref dct_group							"���U�R�T�C���ϊ�(DCT)"
*   - @ref dst_group							"���U�T�C���ϊ� (DST) "
* - @ref a										"�E�F�[�u���b�g�ϊ�"
* - @ref statistics_group						"���v����"
*   - @ref mist::statistics::average			"���ϒl�v�Z"
*   - @ref mist::statistics::variance			"���U�v�Z"
*   - @ref mist::statistics::generate_histogram	"�q�X�g�O�����쐬"
*   - @ref mixture_group						"�������K���z�̐���"
* - @ref random_group							"�[����������"
*   - @ref mist::uniform::random				"��l����"
*   - @ref mist::gauss::random					"���K����"
* - @ref a										"�O���t�`��(2����)"
* - �摜���{�I�u�W�F�N�g�̕`��
*   - @ref image_draw_group						"OpenGL��p����2�����摜�`��"
*   - @ref image_drawing_group					"�����E�~�̕`��"
* - @ref minimization_group						"�֐��̍ŏ���"
* - @ref registration_group						"���W�X�g���[�V����"
*   - @ref mist::non_rigid						"�񍄑̃��W�X�g���[�V����"
* - @ref visualization_group					"����"
*   - @ref volumerendering_group				"�{�����[�������_�����O"
*   - @ref mist::marching_cubes					"���Z�x�ʕ\��"
* - �L�����u���[�V����
*   - @ref calibration_group					"�J�����L�����u���[�V����"
* - @ref machine_learning_group					"�@�B�w�K"
*   - @ref mist::machine_learning::adaboost		"Multiclass AdaBoost"
* - @ref hht_group								"�o���I���[�h��������уq���x���g�ϊ�"
*   - @ref mist::hilbert						"�q���x���g�ϊ�"
*   - @ref mist::emd							"�o���I���[�h����"
*
*
* @subsection image ��ɉ摜�ɑ΂��ēK�p����鏈��
*
*
* @subsubsection image-io ���o�͌n
* - @ref image_group						"�C�ӂ̉摜�f�[�^�̓ǂݍ��݁E�����o��"
*   - @ref image_raw_group					"RAW�f�[�^�̓ǂݍ��݁E�����o��"
*   - @ref image_bmp_group					"BMP�f�[�^�̓ǂݍ��݁E�����o��"
*   - @ref image_pnm_group					"PNM�f�[�^�̓ǂݍ��݁E�����o��"
*   - @ref image_jpeg_group					"JPEG�f�[�^�̓ǂݍ��݁E�����o��"
*   - @ref image_png_group					"PNG�f�[�^�̓ǂݍ��݁E�����o��"
*   - @ref image_gif_group					"GIF�f�[�^�̓ǂݍ���"
*   - @ref image_tiff_group					"TIFF�f�[�^�̓ǂݍ��݁E�����o��"
*   - @ref image_tga_group					"TGA�f�[�^�̓ǂݍ��݁E�����o��"
*   - @ref dicom_group						"DICOM�f�[�^�̓ǂݍ��݁E�����o��"
*
*
* @subsubsection image-filter �t�B���^�n
* - @ref linear_group								"���`�t�B���^"
* - @ref median_group								"���f�B�A���t�B���^"
* - @ref region_growing_group						"�̈�g���@"
* - @ref interpolate_group							"�摜���"
*   - @ref mist::nearest							"�ŋߖT�^���"
*   - @ref mist::mean								"���ϒl�^���"
*   - @ref mist::linear								"���`���"
*   - @ref mist::cubic								"3����ԁisinc�֐���3���ߎ��𗘗p������ԁj"
*   - @ref mist::BSpline							"3����B�X�v���C�����"
*   - @ref mist::sinc								"sinc�֐���ԁi�S��f����ݍ��ނ̂Ōv�Z�R�X�g��j"
* - @ref edge_group									"�G�b�W���o"
*   - @ref mist::canny								"Canny�G�b�W���o��"
* - @ref corner_group								"�R�[�i�[���o"
*   - @ref mist::harris								"Harris�R�[�i�[���o��"
* - �J���[�摜�ɑ΂��鏈��
*   - @ref interlace_group							"�C���^�[���X����"
* - @ref threshold_group							"臒l�I��"
* - @ref morphology_group							"�����t�H���W�[���Z"
* - 2�l�摜�ɑ΂��鏈��
*   - @ref labeling_group							"���x�����O"
*   - @ref decomposition_group						"�}�`����"
*   - @ref boundary_group							"���E��f���o"
*   - @ref mode_group								"�ŕp�l�t�B���^"
*   - @ref thinning_group							"�א���"
*   - @ref skeleton_group							"�X�P���g�����o"
*   - @ref distance_group									"�����ϊ�"
*     - @ref voronoi_distance_group							"�{���m�C����"
*     - @ref euclidean_distance_group						"���[�N���b�h2�拗���ϊ�"
*     - @ref mist::euclidean::inverse_distance_transform	"���[�N���b�h2��t�����ϊ�"
*     - @ref mist::manhattan::distance_transform			"�}���n�b�^�������i�V�e�B�[�u���b�N�����j�ϊ�"
*     - @ref mist::chessboard::distance_transform			"�`�F�X�{�[�h�����ϊ�"
*   - @ref fusion_group										"�}�`�Z��"
*     - @ref fusion_mdt_group								"�}���n�b�^�������iL1�m�����C�V�e�B�[�u���b�N�����j"
*
*
* @subsection audio ��ɉ����ɑ΂��ēK�p����鏈��
*
*
* @subsubsection audio-io ���o�͌n
* - @ref audio_group						"�����f�[�^�̓ǂݍ��݁E�����o��"
*   - @ref audio_wav_group					"WAV�f�[�^�̓ǂݍ��݁E�����o��"
*
*
* @subsubsection audio-filter �t�B���^�n
* - @ref a									"�T���v�����O���[�g�ύX"
* - @ref a									"�Đ�"
*
*
****************************************************************************************************************************/





// mist���O��Ԃ̏I���
_MIST_END


#endif
