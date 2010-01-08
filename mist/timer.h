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

/// @file mist/timer.h
//!
//! @brief �o�ߎ��Ԃ��v�Z����N���X
//!
#ifndef __INCLUDE_MIST_TIMER__
#define __INCLUDE_MIST_TIMER__


#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif

#if defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0

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
private:
#if defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
	union timeval
	{
		LARGE_INTEGER largeint;
		unsigned long long int uint64;
	};

	// Windows �̏ꍇ�ŁC������\�J�E���^�����p�\�Ȃ痘�p����
	static timeval __QueryPerformanceFrequency__( )
	{
		timeval dmy;
		QueryPerformanceFrequency( &( dmy.largeint ) );
		return( dmy );
	}

	static timeval _QueryPerformanceFrequency_( )
	{
		static timeval dmy = __QueryPerformanceFrequency__( );
		return( dmy );
	}

	static double _timeGetTime_( )
	{
		timeval dmy;
		if( QueryPerformanceCounter( &( dmy.largeint ) ) )
		{
			return( static_cast< double >( dmy.uint64 ) / static_cast< double >( _QueryPerformanceFrequency_( ).uint64 ) );
		}
		else
		{
			return( static_cast< double >( timeGetTime( ) ) / 1000.0 );
		}
	}
#else
	static double _timeGetTime_( )
	{
		timeval dmy;
		gettimeofday( &dmy, NULL );
		return( static_cast< double >( dmy.tv_sec ) + static_cast< double >( dmy.tv_usec ) / 1000000.0 );
	}
#endif

public:
	/// @brief ���Ԍv�����s�����߂̃N���X�̃R���X�g���N�^�D
	//! 
	//! �������ɁC�����I�Ƀ^�C�}������������D
	//! 
	timer( ) : _start_time( _timeGetTime_( ) )
	{
		_start_time = _timeGetTime_( );
	}

	/// @brief �����I�ɁC�^�C�}�[���N���A����D
	void reset( )
	{
		_start_time = _timeGetTime_( );
	}

	/// @brief ���Ԍv���J�n������̌o�ߎ��Ԃ��~���b�P�ʂŕԂ��D
	//! 
	//! @return �~���b�P�ʂł̌o�ߎ���
	//! 
	double elapse( ) const
	{
		return( _timeGetTime_( ) - _start_time );
	}

private:
	double	_start_time;	///< @brief ���Ԍv���J�n������ێ�����ϐ�
};


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
class stopwatch
{
private:
	timer t_;
	double sec_;
	bool   run_;

public:
	/// @brief ���Ԍv�����s�����߂̃N���X�̃R���X�g���N�^�D
	//! 
	//! �������ɁC�����I�Ƀ^�C�}������������D
	//! 
	stopwatch( ) : sec_( 0.0 ), run_( false )
	{
	}

	/// @brief �����I�ɁC�^�C�}�[���N���A����D
	void reset( )
	{
		t_.reset( );
		sec_ = 0.0;
	}

	/// @brief ���Ԍv���J�n������̌o�ߎ��Ԃ��~���b�P�ʂŕԂ��D
	//! 
	//! @return �~���b�P�ʂł̌o�ߎ���
	//! 
	double elapse( ) const
	{
		if( run_ )
		{
			return( sec_ + t_.elapse( ) );
		}
		else
		{
			return( sec_ );
		}
	}

	/// @brief �v�����~�߂�D
	//! 
	//! @return �~���b�P�ʂł̌o�ߎ���
	//! 
	void stop( )
	{
		sec_ += t_.elapse( );
		run_ = false;
	}

	/// @brief �v�����ĊJ����D
	//! 
	//! @return �~���b�P�ʂł̌o�ߎ���
	//! 
	void start( )
	{
		t_.reset( );
		run_ = true;
	}
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

/// @brief �o�ߎ��Ԃ�W���o�͂ɏo�͂���D
//! 
//! @param[in,out] out �c �����̐���
//! @param[in]     t   �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
inline ::std::ostream &operator <<( ::std::ostream &out, const stopwatch &t )
{
	out << t.elapse( );
	return( out );
}


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_TIMER__
