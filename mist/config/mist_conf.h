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

#if _MSC_VER <= 1200

#define _MIST_CONST( type, name, value ) enum{ name = value }

#else

#define _MIST_CONST( type, name, value ) static const type name = value

#endif


// �^�����𒲂ׂ�

// char ����
// T���P�o�C�g�̕�����^�ł���ΐ^�ɕ]������
template< class T > struct is_char        { _MIST_CONST( bool, value, false ); };
template<> struct is_char< unsigned char >{ _MIST_CONST( bool, value, true  ); };
template<> struct is_char< signed char >  { _MIST_CONST( bool, value, true  ); };
template<> struct is_char< char >         { _MIST_CONST( bool, value, true  ); };

// arithmetic ����
// T ���Z�p�^�ł���ΐ^�ɕ]������B�Đ����^�����������_�^�̂����ꂩ������ɂ�����
template< class T > struct is_arithmetic                  { _MIST_CONST( bool, value, false ); };
template<>          struct is_arithmetic< unsigned char > { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< unsigned short >{ _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< unsigned int >  { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< unsigned long > { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< signed char >   { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< signed short >  { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< signed int >    { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< signed long >   { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< bool >          { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< char >          { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< float >         { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< double >        { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< long double >   { _MIST_CONST( bool, value, true  ); };

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