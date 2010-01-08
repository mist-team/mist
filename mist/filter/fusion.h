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

/// @file mist/filter/fusion.h
//!
//! @brief �e�����̉摜�ɑΉ������C�}�`�Z���A���S���Y��
//!

#ifndef __INCLUDE_MIST_FIGURE_FUSION__
#define __INCLUDE_MIST_FIGURE_FUSION__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#ifndef __INCLUDE_MIST_THREAD__
#include "../thread.h"
#endif

#ifndef __INCLUDE_MIST_LIMITS__
#include "../limits.h"
#endif



// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


// �}�`�Z���̏����֐�
namespace __fusion_controller__
{
	template < int DIMENSION >
	struct __access__
	{
		template < class Array >
		inline static typename Array::value_type &at( Array &in, typename Array::size_type _1, typename Array::size_type _2, typename Array::size_type _3 )
		{
			return( in( _1, _2, _3 ) );
		}

		template < class Array >
		inline static typename Array::size_type size1( const Array &in ){ return( in.size1( ) ); }

		template < class Array >
		inline static typename Array::size_type size2( const Array &in ){ return( in.size2( ) ); }

		template < class Array >
		inline static typename Array::size_type size3( const Array &in ){ return( in.size3( ) ); }
	};

	template < >
	struct __access__< 2 >
	{
		template < class Array >
		inline static typename Array::value_type &at( Array &in, typename Array::size_type _1, typename Array::size_type _2, typename Array::size_type _3 )
		{
			return( in( _2, _1, _3 ) );
		}

		template < class Array >
		inline static typename Array::size_type size1( const Array &in ){ return( in.size2( ) ); }

		template < class Array >
		inline static typename Array::size_type size2( const Array &in ){ return( in.size1( ) ); }

		template < class Array >
		inline static typename Array::size_type size3( const Array &in ){ return( in.size3( ) ); }
	};

	template < >
	struct __access__< 3 >
	{
		template < class Array >
		inline static typename Array::value_type &at( Array &in, typename Array::size_type _1, typename Array::size_type _2, typename Array::size_type _3 )
		{
			return( in( _2, _3, _1 ) );
		}

		template < class Array >
		inline static typename Array::size_type size1( const Array &in ){ return( in.size3( ) ); }

		template < class Array >
		inline static typename Array::size_type size2( const Array &in ){ return( in.size1( ) ); }

		template < class Array >
		inline static typename Array::size_type size3( const Array &in ){ return( in.size2( ) ); }
	};


	/// @brief �}���n�b�^�������i�V�e�B�u���b�N�����j�Ɋ�Â��}�`�̊g��
	struct __expand_mdt__
	{
		template < class Array1, class Array2 >
		static void pre_process( const Array1 &in, Array2 &out )
		{
			typedef typename Array2::size_type  size_type;
			typedef typename Array2::value_type value_type;

			value_type infinity = type_limits< value_type >::maximum( );
			for( size_type i = 0 ; i < in.size( ) ; i++ )
			{
				out[ i ] = in[ i ] > 0 ? infinity : 0;
			}
		}

		template < class Array >
		static void post_process( Array &in )
		{
			typedef typename Array::size_type  size_type;
			typedef typename Array::value_type value_type;

			for( size_type i = 0 ; i < in.size( ) ; i++ )
			{
				in[ i ] = in[ i ] > 0 ? 1 : 0;
			}
		}

		template < class Array, int DIMENSION >
		static void transform( Array &in, typename Array::size_type length, typename Array::size_type thread_id, typename Array::size_type thread_num, __access__< DIMENSION > dmy )
		{
			typedef typename Array::size_type  size_type;
			typedef typename Array::value_type value_type;
			typedef typename Array::difference_type difference_type;
			typedef __access__< DIMENSION > access;

			difference_type i1, i2, i3, l;

			difference_type _1 = access::size1( in );
			difference_type _2 = access::size2( in );
			difference_type _3 = access::size3( in );

			value_type infinity = type_limits< value_type >::maximum( );

			for( i3 = 0 ; i3 < _3 ; i3++ )
			{
				for( i2 = thread_id ; i2 < _2 ; i2 += thread_num )
				{
					for( i1 = 0, l = 0 ; i1 < _1 ; i1++ )
					{
						value_type &v = access::at( in, i1, i2, i3 );
						if( v == infinity )
						{
							l = length;
						}
						else if( l > v )
						{
							v = static_cast< value_type >( l );
							l--;
						}
						else if( v > 0 )
						{
							l = static_cast< difference_type >( v ) - 1;
						}
					}

					for( i1 = _1 - 1, l = 0 ; i1 >= 0 ; i1-- )
					{
						value_type &v = access::at( in, i1, i2, i3 );
						if( v == infinity )
						{
							l = length;
						}
						else if( l > v )
						{
							v = static_cast< value_type >( l );
							l--;
						}
						else if( v > 0 )
						{
							l = static_cast< difference_type >( v ) - 1;
						}
					}
				}
			}
		}
	};

	/// @brief �}���n�b�^�������i�V�e�B�u���b�N�����j�Ɋ�Â��}�`�̊g��
	struct __shrink_mdt__
	{
		template < class Array1, class Array2 >
		static void pre_process( const Array1 &in, Array2 &out )
		{
			typedef typename Array2::size_type  size_type;
			typedef typename Array2::value_type value_type;

			value_type infinity = type_limits< value_type >::maximum( );
			for( size_type i = 0 ; i < in.size( ) ; i++ )
			{
				out[ i ] = in[ i ] > 0 ? infinity : 0;
			}
		}

		template < class Array >
		static void post_process( Array &in )
		{
			typedef typename Array::size_type  size_type;
			typedef typename Array::value_type value_type;

			value_type infinity = type_limits< value_type >::maximum( );
			for( size_type i = 0 ; i < in.size( ) ; i++ )
			{
				in[ i ] = in[ i ] == infinity ? 1 : 0;
			}
		}

		template < class Array, int DIMENSION >
		static void transform( Array &in, typename Array::size_type length, typename Array::size_type thread_id, typename Array::size_type thread_num, __access__< DIMENSION > dmy )
		{
			typedef typename Array::size_type  size_type;
			typedef typename Array::value_type value_type;
			typedef typename Array::difference_type difference_type;
			typedef __access__< DIMENSION > access;

			difference_type i1, i2, i3, l;

			difference_type _1 = access::size1( in );
			difference_type _2 = access::size2( in );
			difference_type _3 = access::size3( in );

			value_type infinity = type_limits< value_type >::maximum( );

			for( i3 = 0 ; i3 < _3 ; i3++ )
			{
				for( i2 = thread_id ; i2 < _2 ; i2 += thread_num )
				{
					for( i1 = 0, l = 0 ; i1 < _1 ; i1++ )
					{
						value_type &v = access::at( in, i1, i2, i3 );
						if( v == 0 )
						{
							l = length;
						}
						else if( v == infinity )
						{
							if( l > 0 )
							{
								v = static_cast< value_type >( l );
								l--;
							}
						}
						else if( v < l )
						{
							v = static_cast< value_type >( l );
							l--;
						}
						else if( v > 0 )
						{
							l = static_cast< difference_type >( v ) - 1;
						}
					}

					for( i1 = _1 - 1, l = 0 ; i1 >= 0 ; i1-- )
					{
						value_type &v = access::at( in, i1, i2, i3 );
						if( v == 0 )
						{
							l = length;
						}
						else if( v == infinity )
						{
							if( l > 0 )
							{
								v = static_cast< value_type >( l );
								l--;
							}
						}
						else if( v < l )
						{
							v = static_cast< value_type >( l );
							l--;
						}
						else if( v > 0 )
						{
							l = static_cast< difference_type >( v ) - 1;
						}
					}
				}
			}
		}
	};


	// �}�`�Z�������ϊ��̃X���b�h����
	template < class T, class Fusion >
	class fusion_thread : public mist::thread< fusion_thread< T, Fusion > >
	{
	public:
		typedef mist::thread< fusion_thread< T, Fusion > > base;
		typedef typename base::thread_exit_type thread_exit_type;
		typedef typename T::size_type size_type;
		typedef typename T::value_type value_type;

	private:
		size_t thread_id_;
		size_t thread_num_;

		// ���o�͗p�̉摜�ւ̃|�C���^
		T *in_;
		size_type length_;
		size_type axis_;

	public:
		void setup_parameters( T &in, size_type axis, size_type len, size_type thread_id, size_type thread_num )
		{
			in_  = &in;
			axis_ = axis;
			length_ = len;
			thread_id_ = thread_id;
			thread_num_ = thread_num;
		}

		void setup_axis( size_type axis )
		{
			axis_ = axis;
		}

		fusion_thread( size_type id = 0, size_type num = 1 )
			: thread_id_( id ), thread_num_( num ), in_( NULL ), length_( 1 ), axis_( 0 )
		{
		}

	protected:
		// �p��������ŕK�����������X���b�h�֐�
		virtual thread_exit_type thread_function( )
		{
			switch( axis_ )
			{
			case 2:
				Fusion::transform( *in_, length_, thread_id_, thread_num_, __access__< 2 >( ) );
				break;

			case 3:
				Fusion::transform( *in_, length_, thread_id_, thread_num_, __access__< 3 >( ) );
				break;

			case 0:
			default:
				Fusion::transform( *in_, length_, thread_id_, thread_num_, __access__< 1 >( ) );
				break;
			}
			return( true );
		}
	};
}



//! @addtogroup fusion_group �}�`�Z���O���[�v
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/filter/fusion.h>
//! @endcode
//!
//!  @{




/// @brief �C�ӂ̋����^�C�v�ɑΉ������}�`�Z��
//! 
//! @attention ���͂Əo�͂́C����MIST�R���e�i�I�u�W�F�N�g�ł����������삷��
//! @attention �X���b�h����0���w�肵���ꍇ�́C�g�p�\��CPU���������I�Ɏ擾����
//! 
//! @param[in]  in         �c ���͉摜
//! @param[out] out        �c �o�͉摜
//! @param[in]  length     �c �g�傳����T�C�Y
//! @param[in]  thread_num �c �g�p����X���b�h��
//! @param[in]  fusion     �c �g�p���鋗���֐��Ɗg��k���̑g�ݍ��킹
//! 
template < class Array1, class Array2, class Fusion >
bool fusion( const Array1 &in, Array2 &out, typename Array1::size_type length, typename Array1::size_type thread_num, Fusion fusion )
{
	if( in.empty( ) )
	{
		return( false );
	}

	typedef typename Array2::size_type  size_type;
	typedef typename Array2::value_type value_type;
	typedef __fusion_controller__::fusion_thread< Array2, Fusion > fusion_thread;

	if( thread_num == 0 )
	{
		thread_num = static_cast< size_type >( get_cpu_num( ) );
	}

	out.resize( in.size1( ), in.size2( ), in.size3( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );
	out.reso3( in.reso3( ) );

	size_type i;

	Fusion::pre_process( in, out );

	fusion_thread *thread = new fusion_thread[ thread_num ];

	if( in.width( ) > 1 )
	{
		// X�������̏���
		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( out, 1, length, i, thread_num );
		}

		do_threads( thread, thread_num );
	}

	if( in.height( ) > 1 )
	{
		// Y�������̏���
		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( out, 2, length, i, thread_num );
		}

		do_threads( thread, thread_num );
	}

	if( in.depth( ) > 1 )
	{
		// Z�������̏���
		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( out, 3, length, i, thread_num );
		}

		do_threads( thread, thread_num );
	}

	delete [] thread;

	Fusion::post_process( out );

	return( true );
}



//! @addtogroup fusion_mdt_group �}���n�b�^�������O���[�v
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/filter/fusion.h>
//! @endcode
//!
//!  @{


/// @brief �}���n�b�^�������iL1�m�����C�V�e�B�[�u���b�N�����j�^�C�v�̐}�`�g��
//! 
//! @attention ���͂Əo�͂́C����MIST�R���e�i�I�u�W�F�N�g�ł����������삷��
//! @attention �X���b�h����0���w�肵���ꍇ�́C�g�p�\��CPU���������I�Ɏ擾����
//! 
//! @param[in]  in         �c ���͉摜
//! @param[out] out        �c �o�͉摜
//! @param[in]  length     �c �g�傳����T�C�Y
//! @param[in]  thread_num �c �g�p����X���b�h��
//! 
template < class Array1, class Array2 >
bool expand_mdt( const Array1 &in, Array2 &out, typename Array1::size_type length = 1, typename Array1::size_type thread_num = 0 )
{
	return( fusion( in, out, length, thread_num, __fusion_controller__::__expand_mdt__( ) ) );
}



/// @brief �}���n�b�^�������iL1�m�����C�V�e�B�[�u���b�N�����j�^�C�v�̐}�`�k��
//! 
//! @attention ���͂Əo�͂́C����MIST�R���e�i�I�u�W�F�N�g�ł����������삷��
//! @attention �X���b�h����0���w�肵���ꍇ�́C�g�p�\��CPU���������I�Ɏ擾����
//! 
//! @param[in]  in         �c ���͉摜
//! @param[out] out        �c �o�͉摜
//! @param[in]  length     �c �g�傳����T�C�Y
//! @param[in]  thread_num �c �g�p����X���b�h��
//! 
template < class Array1, class Array2 >
bool shrink_mdt( const Array1 &in, Array2 &out, typename Array1::size_type length = 1, typename Array1::size_type thread_num = 0 )
{
	return( fusion( in, out, length, thread_num, __fusion_controller__::__shrink_mdt__( ) ) );
}

/// @}
//  �}���n�b�^�������O���[�v�̏I���



/// @}
//  �}�`�Z���O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_FIGURE_FUSION__
