#ifndef __INCLUDE_MIST_CONF_H__
#define __INCLUDE_MIST_CONF_H__


#ifndef _MIST_BEGIN
#define _MIST_BEGIN		namespace mist{
#endif

#ifndef _MIST_END
#define _MIST_END		}
#endif

// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


// �^�����𒲂ׂ�
#if _MSC_VER <= 1200

// char ����
// T���P�o�C�g�̕�����^�ł���ΐ^�ɕ]������
template< class T > struct is_char{ enum{ value = false }; };
template<> struct is_char< unsigned char >{ enum{ value = true }; };
template<> struct is_char< signed char >{ enum{ value = true }; };
template<> struct is_char< char >{ enum{ value = true }; };

// arithmetic ����
// T ���Z�p�^�ł���ΐ^�ɕ]������B�Đ����^�����������_�^�̂����ꂩ������ɂ�����
template< class T > struct is_arithmetic{ enum{ value = false }; };
template<> struct is_arithmetic< unsigned char >{ enum{ value = true }; };
template<> struct is_arithmetic< unsigned short >{ enum{ value = true }; };
template<> struct is_arithmetic< unsigned int >{ enum{ value = true }; };
template<> struct is_arithmetic< unsigned long >{ enum{ value = true }; };
template<> struct is_arithmetic< signed char >{ enum{ value = true }; };
template<> struct is_arithmetic< signed short >{ enum{ value = true }; };
template<> struct is_arithmetic< signed int >{ enum{ value = true }; };
template<> struct is_arithmetic< signed long >{ enum{ value = true }; };
template<> struct is_arithmetic< bool >{ enum{ value = true }; };
template<> struct is_arithmetic< char >{ enum{ value = true }; };
template<> struct is_arithmetic< float >{ enum{ value = true }; };
template<> struct is_arithmetic< double >{ enum{ value = true }; };
template<> struct is_arithmetic< long double >{ enum{ value = true }; };

#else


// char ����
// T���P�o�C�g�̕�����^�ł���ΐ^�ɕ]������
template< class T > struct is_char{ static const bool value = false; };
template<> struct is_char< unsigned char >{ static const bool value = true; };
template<> struct is_char< signed char >{ static const bool value = true; };
template<> struct is_char< char >{ static const bool value = true; };

// arithmetic ����
// T ���Z�p�^�ł���ΐ^�ɕ]������B�Đ����^�����������_�^�̂����ꂩ������ɂ�����
template< class T > struct is_arithmetic{ static const bool value = false; };
template<> struct is_arithmetic< unsigned char >{ static const bool value = true; };
template<> struct is_arithmetic< unsigned short >{ static const bool value = true; };
template<> struct is_arithmetic< unsigned int >{ static const bool value = true; };
template<> struct is_arithmetic< unsigned long >{ static const bool value = true; };
template<> struct is_arithmetic< signed char >{ static const bool value = true; };
template<> struct is_arithmetic< signed short >{ static const bool value = true; };
template<> struct is_arithmetic< signed int >{ static const bool value = true; };
template<> struct is_arithmetic< signed long >{ static const bool value = true; };
template<> struct is_arithmetic< bool >{ static const bool value = true; };
template<> struct is_arithmetic< char >{ static const bool value = true; };
template<> struct is_arithmetic< float >{ static const bool value = true; };
template<> struct is_arithmetic< double >{ static const bool value = true; };
template<> struct is_arithmetic< long double >{ static const bool value = true; };


#endif


// �f�[�^�^�̂Ɋւ�����
template< class T >
struct type_trait{ typedef T value_type; };



#define _MIST_ALLOCATE_TEST_			0

#ifdef WIN32

#define _MIST_ALLOCATOR_MEMORY_TRIM__	0	// VC�W����STL�ł̓T�|�[�g���Ă��Ȃ��̂ŃI�t�ɂ���

#else

#define _MIST_ALLOCATOR_MEMORY_TRIM__	1	// VC�W����STL�ł̓T�|�[�g���Ă��Ȃ��̂ŃI�t�ɂ���

#endif


#ifndef NDEBUG

#define _CHECK_ACCESS_VIOLATION_		1	// �z��v�f�ւ̃A�N�Z�X�ᔽ���`�F�b�N���邩�ǂ���
#define _CHECK_ARRAY_OPERATION_			1	// 1�����z��̉��Z�̐��������`�F�b�N���邩�ǂ���
#define _CHECK_ARRAY1_OPERATION_		1	// 1�����摜�̉��Z�̐��������`�F�b�N���邩�ǂ���
#define _CHECK_ARRAY2_OPERATION_		1	// 2�����摜�̉��Z�̐��������`�F�b�N���邩�ǂ���
#define _CHECK_ARRAY3_OPERATION_		1	// 3�����摜�̉��Z�̐��������`�F�b�N���邩�ǂ���

#define _CHECK_MATRIX_OPERATION_		1	// �s�񉉎Z�̐��������`�F�b�N���邩�ǂ���
#define _USE_EXPRESSION_TEMPLATE_		0	// Expression Template�𗘗p���邩�ǂ���

#else

#define _CHECK_ACCESS_VIOLATION_		0	// �z��v�f�ւ̃A�N�Z�X�ᔽ���`�F�b�N���邩�ǂ���
#define _CHECK_ARRAY_OPERATION_			0
#define _CHECK_ARRAY1_OPERATION_		0
#define _CHECK_ARRAY2_OPERATION_		0
#define _CHECK_ARRAY3_OPERATION_		0

#define _CHECK_MATRIX_OPERATION_		0
#define _USE_EXPRESSION_TEMPLATE_		1

#endif



// mist���O��Ԃ̏I���
_MIST_END


#endif