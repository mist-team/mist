#ifndef __INCLUDE_MIST_CONF_H__
#define __INCLUDE_MIST_CONF_H__



// MIST���O��Ԃ��`���邽�߂̃}�N��
#ifndef _MIST_BEGIN
#define _MIST_BEGIN		namespace mist{
#endif

#ifndef _MIST_END
#define _MIST_END		}
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
	#define _MIST_ALLOCATOR_MEMORY_TRIM_	0	// VC�W����STL�ł̓T�|�[�g���Ă��Ȃ��̂ŃI�t�ɂ���
#else
	#define _MIST_ALLOCATOR_MEMORY_TRIM_	1	// VC�ȊO��STL�ł̓T�|�[�g���Ă���̂ŃI���ɂ���
#endif


#ifndef NDEBUG

	// �f�o�b�O�p�̐ݒ��S�ăI���ɂ���
	#define _CHECK_ACCESS_VIOLATION_		1	// �z��v�f�ւ̃A�N�Z�X�ᔽ���`�F�b�N���邩�ǂ���
	#define _CHECK_ARRAY_OPERATION_			1	// 1�����z��̉��Z�̐��������`�F�b�N���邩�ǂ���
	#define _CHECK_ARRAY1_OPERATION_		1	// 1�����摜�̉��Z�̐��������`�F�b�N���邩�ǂ���
	#define _CHECK_ARRAY2_OPERATION_		1	// 2�����摜�̉��Z�̐��������`�F�b�N���邩�ǂ���
	#define _CHECK_ARRAY3_OPERATION_		1	// 3�����摜�̉��Z�̐��������`�F�b�N���邩�ǂ���

	#define _CHECK_MATRIX_OPERATION_		1	// �s�񉉎Z�̐��������`�F�b�N���邩�ǂ���
	#define _USE_EXPRESSION_TEMPLATE_		0	// Expression Template�𗘗p���邩�ǂ���

#else

	// �f�o�b�O�p�̐ݒ��S�ăI�t�ɂ���
	#define _CHECK_ACCESS_VIOLATION_		0	// �z��v�f�ւ̃A�N�Z�X�ᔽ���`�F�b�N���邩�ǂ���
	#define _CHECK_ARRAY_OPERATION_			0
	#define _CHECK_ARRAY1_OPERATION_		0
	#define _CHECK_ARRAY2_OPERATION_		0
	#define _CHECK_ARRAY3_OPERATION_		0

	#define _CHECK_MATRIX_OPERATION_		0
	#define _USE_EXPRESSION_TEMPLATE_		1

#endif

#define _MIST_THREAD_SUPPORT_				1	// MIST�̊e�A���S���Y���ł̃X���b�h�T�|�[�g�����邩�ǂ���
												// �T�|�[�g����ꍇ�́CLINUX�n�̏ꍇ�� pthread ���C�u�������K�v


// �s�񉉎Z�̃I�v�V����
#define _USE_BALANCING_MATRIX_EIGEN_		1	// �s��̑Ίp�����s�����ƂŌv�Z���x���グ��i�኱���������ڂɐH���j
#define _USE_DIVIDE_AND_CONQUER_SVD_		1	// ���������@��p���������ȓ��ْl�����𗘗p����i�኱���������ڂɐH���j


#if _CHECK_ACCESS_VIOLATION_

	#define _CHECK_ACCESS_VIOLATION1_( index ) \
		if( index < 0 || index >= size_ )\
		{\
			static value_type dmy;\
			std::cout << "Access Violation at ( " << index << " )" << std::endl;\
			return( dmy );\
		}\

	#define _CHECK_ACCESS_VIOLATION2_( index1, index2 ) \
		if( index1 < 0 || index1 >= size1_ || index2 < 0 || index2 >= size2_ )\
		{\
			static value_type dmy;\
			std::cout << "Access Violation at ( " << index1 << ", " << index2 << " )" << std::endl;\
			return( dmy );\
		}\

	#define _CHECK_ACCESS_VIOLATION3_( index1, index2, index3 ) \
		if( index1 < 0 || index1 >= size1_ || index2 < 0 || index2 >= size2_ || index3 < 0 || index3 >= size3_ )\
		{\
			static value_type dmy;\
			std::cout << "Access Violation at ( " << index1 << ", " << index2 << ", " << index3 << " )" << std::endl;\
			return( dmy );\
		}\

#else

	#define _CHECK_ACCESS_VIOLATION1_( index ) 
	#define _CHECK_ACCESS_VIOLATION2_( index1, index2 )
	#define _CHECK_ACCESS_VIOLATION3_( index1, index2, index3 )

#endif


// mist���O��Ԃ̏I���
_MIST_END


#endif
