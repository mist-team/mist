/// @file mist/timer.h
//!
//! @brief �o�ߎ��Ԃ��v�Z����N���X
//!
#ifndef __INCLUDE_MIST_TIMER__
#define __INCLUDE_MIST_TIMER__


#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif

#ifdef WIN32

#include <windows.h>
#include <mmsystem.h>
#pragma comment ( lib, "winmm.lib" )

#else

//#include <time.h>
#include <sys/time.h>

#endif


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


/// ���Ԍv�����s�����߂̃N���X
//! 
//! timer�N���X�𐶐������玞�Ԍv�����J�n����D
//! 
//! @par �����ӏ��Ŏ��Ԍv�����s���ꍇ�́C�ȉ��̂悤�ɋL�q����
//! @code �^�C�}�[�I�u�W�F�N�g�̗��p���@
//! {
//!     mist::timer t;
//! 
//!     ... ���炩�̌v�Z ...
//! 
//!     std::cout << "�v�Z����: " << t << std::endl;
//! }
//! @endcode
//!
class timer
{
public:
	/// @brief ���Ԍv�����s�����߂̃N���X�̃R���X�g���N�^�D
	//! 
	//! �������ɁC�����I�Ƀ^�C�}������������D
	//! 
	timer( )
	{
#ifdef WIN32
		_start_time = timeGetTime( );
#else
		gettimeofday( &_start_time, NULL );
#endif
	} // postcondition: elapsed()==0

	/// @brief �����I�ɁC�^�C�}�[���N���A����D
	void reset( )
	{
#ifdef WIN32
		_start_time = timeGetTime( );
#else
		gettimeofday( &_start_time, NULL );
#endif
	}

	/// @brief ���Ԍv���J�n������̌o�ߎ��Ԃ��~���b�P�ʂŕԂ��D
	//! 
	//! @return �~���b�P�ʂł̌o�ߎ���
	//! 
	double elapse( ) const
	{
#ifdef WIN32
		return( static_cast< double >( timeGetTime( ) - _start_time ) / 1000.0 );
#else
		timeval  _end_time;
		gettimeofday( &_end_time, NULL );
		return( static_cast< double >( _end_time.tv_sec - _start_time.tv_sec ) + static_cast< double >( _end_time.tv_usec - _start_time.tv_usec ) / 1000000.0 );
#endif
	}

private:
#ifdef WIN32
	DWORD		_start_time;	///< @brief ���Ԍv���J�n������ێ�����ϐ��iWindows�p�j
#else
	timeval		_start_time;	///< @brief ���Ԍv���J�n������ێ�����ϐ��iLinux�p�j
#endif

};


/// @brief �o�ߎ��Ԃ�W���o�͂ɏo�͂���D
//! 
//! @param[in,out] out �c �����̐���
//! @param[in]     t   �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
inline ::std::ostream &operator <<( ::std::ostream &out, const timer &t )
{
	out << t.elapse( );
	return( out );
}


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_TIMER__
