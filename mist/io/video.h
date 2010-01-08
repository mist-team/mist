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

/// @file mist/io/video.h
//!
//! @brief ���܂��܂ȃr�f�I�X�g���[���̓��o�̓��C�u����
//!

#ifndef __INCLUDE_IO_VIDEO_H__
#define __INCLUDE_IO_VIDEO_H__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#ifndef __INCLUDE_MIST_COLOR_H__
#include "../config/color.h"
#endif

#ifndef __INCLUDE_MIST_SINGLETON__
#include "../singleton.h"
#endif

#ifndef __INCLUDE_MIST_LIMITS__
#include "../limits.h"
#endif

#include <string>

extern "C"
{
#ifdef __MIST_WINDOWS__
	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>
	#include <libswscale/swscale.h>
#else
	#include <ffmpeg/avcodec.h>
	#include <ffmpeg/avformat.h>
	#include <ffmpeg/swscale.h>
#endif
}


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN



//! @defgroup video_group �r�f�I�t�@�C���̓��o�͂��s�����C�u����
//!
//! �{���C�u�����́Chttp://ffmpeg.sourceforge.net/index.php �ŊJ�����s���Ă��� ffmpeg �� libavcodec, libavformat �𗘗p���Ă���
//! �G���R�[�h�ƃf�R�[�h�͂��̃��C�u������API��p���Ď�������Ă��邽�߁C�ʓr���C�u������p�ӂ���K�v������
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/io/video.h>
//! @endcode
//!
//! @code �g�p��(�r�f�I�X�g���[������A��bmp�t�@�C�����쐬)
//! mist::array2< mist::rgb< unsigned char > > img;
//! mist::video::decoder iv( "input.avi" );
//! 
//! iv.dump( );
//! char fname[ 256 ];
//! int i = 0;
//! while( !iv.is_eof( ) )
//! {
//!     iv >> img;
//!     sprintf( fname, "%04d.bmp", i++ );
//!     mist::write_bmp( img, fname );
//! }
//! iv.close( );
//! @endcode
//!
//! @code �g�p��(���̓r�f�I��mpeg1�r�f�I�ɕϊ�)
//! mist::array2< mist::rgb< unsigned char > > img;
//! mist::video::decoder iv( "input.avi" );
//! 
//! // �R���X�g���N�^�̈�����ύX���Ĉ��k�p�����[�^�̕ύX
//! // �i�y���ӁI�z�X�g���[�����J���O�ɕύX���Ȃ���΂Ȃ�Ȃ��j
//! mist::video::mpeg1::encoder ov( "output.mpg", 320, 240 );
//! ov.dump( );
//! 
//! while( !iv.is_eof( ) )
//! {
//!     std::cout << iv.time( ) << std::endl;
//!     iv >> img;
//!     /* ������ img �ɉ��炩�̉摜�������{�����Ƃ��\ */
//!     ov << img;
//! }
//! 
//! ov.close( );
//! iv.close( );
//! @endcode
//!
//!  @{


/// @brief �r�f�I�t�@�C�����������C�u����
namespace video
{
	class video_io_vase
	{
	public:
		typedef size_t		size_type;
		typedef ptrdiff_t	difference_type;

	public:
		virtual ~video_io_vase( ){ }

		bool open( const std::wstring &filename ){ return( open( wstr2str( filename ) ) ); }	///< @brief 

		virtual bool open( const std::string &filename ) = 0;		///< @brief 
		virtual bool close( ) = 0;									///< @brief 
		virtual bool is_open( ) const = 0;							///< @brief �r�f�I�X�g���[�����J���Ă��邩�ǂ�����Ԃ�
		virtual bool is_eof( ) const = 0;							///< @brief �r�f�I�X�g���[���̏I���ɗ������ǂ�����Ԃ�
		virtual bool dump( ) const = 0;								///< @brief �X�g���[���̃t�H�[�}�b�g��W���o�͂Ƀ_���v����
		virtual long double time( ) const = 0;						///< @brief ���݂̃r�f�I�X�g���[����ł̍Đ��ʒu��\���b��
		virtual long double duration( ) const = 0;					///< @brief �r�f�I�X�g���[���̑��b����Ԃ�
		virtual const std::string filename( ) const = 0;			///< @brief �t�@�C������Ԃ�
		virtual size_type bit_rate( ) const = 0;					///< @brief �r�b�g���[�g��Ԃ�
		virtual size_type width( ) const = 0;						///< @brief �t���[���̕���Ԃ�
		virtual size_type height( ) const = 0;						///< @brief �t���[���̍�����Ԃ�
		virtual long double frame_rate_numerator( ) const = 0;		///< @brief �t���[�����[�g�iA��B�j�̕��q������A��Ԃ�
		virtual long double frame_rate_denominator( ) const = 0;	///< @brief �t���[�����[�g�iA��B�j�̕��ꕔ����B��Ԃ�
		virtual long double frame_aspect_ratio( ) const = 0;		///< @brief �t���[���̃A�X�y�N�g��𓾂�i�E�B���h�E��/�E�B���h�E�����j

		/// @brief �t���[�����[�g��Ԃ�
		virtual long double frame_rate( ) const
		{
			return( frame_rate_denominator( ) / frame_rate_numerator( ) );
		}

		/// @brief 1�t���[��������̎��Ԃ�Ԃ�
		virtual long double seconds_per_frame( ) const
		{
			return( frame_rate_numerator( ) / frame_rate_denominator( ) );
		}

	protected:
		AVOutputFormat *guess_video_format( const std::string &filename, const std::string &video_type, const std::string &mime_type )
		{
			AVOutputFormat *fmt = NULL;

			const char *fname = filename.empty( )   ? NULL : filename.c_str( );
			const char *sname = video_type.empty( ) ? NULL : video_type.c_str( );
			const char *mime  = mime_type.empty( )  ? NULL : mime_type.c_str( );

			if( fname != NULL || sname != NULL || mime != NULL )
			{
				// �t�@�C���̊g���q����t�H�[�}�b�g�𐄑�����
				fmt = guess_format( sname, fname, mime );
				if( fmt == NULL )
				{
					std::cout << "Could not determine output format from file extension. MPEG is used as default." << std::endl;
					fmt = guess_format( "mpeg", NULL, NULL );
					if( fmt == NULL )
					{
						std::cerr << "Could not find MPEG encoder." << std::endl;
						return( NULL );
					}
				}
			}
			else
			{
				fmt = guess_format( "mpeg", NULL, NULL );
				if( fmt == NULL )
				{
					std::cerr << "Could not find specified video format." << std::endl;
					return( NULL );
				}
			}

			return( fmt );
		}

		AVOutputFormat *find_video_format( CodecID codec_id )
		{
			AVOutputFormat *fmt = av_oformat_next( NULL );
			while( fmt != NULL )
			{
				if( fmt->video_codec == codec_id )
				{
					return( fmt );
				}

				fmt = av_oformat_next( fmt );
			}

			return( NULL );
		}

		AVFrame *allocate_frame( int w, int h, PixelFormat pix_fmt )
		{
			// �����p�̃t���[���o�b�t�@��p�ӂ���
			AVFrame *frame = avcodec_alloc_frame( );
			if( frame == NULL )
			{
				return( NULL );
			}
			else
			{
				int size = avpicture_get_size( pix_fmt, w, h );
				uint8_t *buff = ( uint8_t * )av_malloc( size );
				avpicture_fill( ( AVPicture * )frame, buff, pix_fmt, w, h );

				return( frame );
			}
		}

		void free_frame( AVFrame **frame )
		{
			if( frame != NULL )
			{
				av_free( ( *frame )->data[ 0 ] );
				av_free( ( *frame ) );
				*frame = NULL;
			}
		}
	};

	/// @brief �r�f�I���̓N���X
	//! 
	//! �I�[�f�B�I�X�g���[���͖��T�|�[�g
	//!
	class decoder : public video_io_vase
	{
	private:
		AVFormatContext	*p_fctx_;				///< @brief �r�f�I�t�H�[�}�b�g���C�f�R�[�_��
		AVFrame			*p_frame_src_;			///< @brief �ǂݏo���ꂽ�t���[���摜�o�b�t�@
		AVFrame			*p_frame_rgb_;			///< @brief RGB�t�H�[�}�b�g�̃t���[���摜�o�b�t�@�iarray2�`���̃t���[���摜�𓾂邽�߂̒��ԃf�[�^�j
		bool			is_open_;				///< @brief �r�f�I���J���Ă��邩�ǂ����̃t���O
		bool			is_eof_;				///< @brief �r�f�I���J���Ă��邩�ǂ����̃t���O
		int				video_stream_index_;	///< @brief �r�f�I���w���X�g���[���ԍ�
		SwsContext		*p_swscale_;			///< @brief �f�R�[�h��̃t���[����RGB�̃t���[���ɕϊ�����t�B���^���w���|�C���^
		int64_t			frame_pts_;				///< @brief �r�f�I�X�g���[�����ł̌��݂̃t���[���ʒu��ێ�����ϐ�

	public:
		/// @brief �R���X�g���N�^
		//! 
		//! �f�t�H���g�R���X�g���N�^
		//! 
		decoder( ) : p_fctx_( NULL ), p_frame_src_( NULL ), p_frame_rgb_( NULL ), is_open_( false ), is_eof_( true ), video_stream_index_( -1 ), p_swscale_( NULL ), frame_pts_( -1 )
		{
			bool &bInitialized = singleton< bool, 60602 >::get_instance( );
			if( !bInitialized )
			{
				avcodec_register_all( );
				//avdevice_register_all( );
				av_register_all( );
				bInitialized = true;
			}
		}

		/// @brief �R���X�g���N�^
		//! 
		//! �f�t�H���g�R���X�g���N�^
		//! 
		decoder( const std::string &filename ) : p_fctx_( NULL ), p_frame_src_( NULL ), p_frame_rgb_( NULL ), is_open_( false ), is_eof_( true ), video_stream_index_( -1 ), p_swscale_( NULL ), frame_pts_( -1 )
		{
			bool &bInitialized = singleton< bool, 60602 >::get_instance( );
			if( !bInitialized )
			{
				avcodec_register_all( );
				//avdevice_register_all( );
				av_register_all( );
				bInitialized = true;
			}

			if( !open( filename ) )
			{
				throw;
			}
		}

		/// @brief �f�X�g���N�^
		//! 
		virtual ~decoder( )
		{ 
			close( );
		}


	public:
		/// @brief �r�f�I�X�g���[�����J���Ă��邩�ǂ�����Ԃ�
		virtual bool is_open( ) const { return( is_open_ ); }

		/// @brief �r�f�I�X�g���[���̏I���ɗ������ǂ�����Ԃ�
		virtual bool is_eof( ) const { return( is_eof_ ); }

		/// @brief �X�g���[���̃t�H�[�}�b�g��W���o�͂Ƀ_���v����
		virtual int codec_id( ) const
		{
			if( is_open( ) && video_stream_index_ >= 0 )
			{
				return( p_fctx_->streams[ video_stream_index_ ]->codec->codec_id );
			}
			else
			{
				return( CODEC_ID_NONE );
			}
		}

		/// @brief �X�g���[���̃t�H�[�}�b�g��W���o�͂Ƀ_���v����
		virtual bool dump( ) const
		{
			if( is_open( ) )
			{
				dump_format( p_fctx_, 0, p_fctx_->filename, false );
				return( true );
			}
			else
			{
				return( false );
			}
		}

		/// @brief ���݂̃r�f�I�X�g���[����ł̍Đ��ʒu��\���b��
		virtual long double time( ) const
		{
			if( is_open( ) )
			{
				return( static_cast< long double >( frame_pts_ ) / static_cast< long double >( AV_TIME_BASE ) );
			}
			else
			{
				return( -1.0 );
			}
		}

		/// @brief ���݂̃r�f�I�X�g���[����ł̍Đ��ʒu��\���b��
		virtual int64_t frame_id( ) const
		{
			if( is_open( ) )
			{
				AVStream *stream = p_fctx_->streams[ video_stream_index_ ];
				return( av_rescale( frame_pts_, stream->time_base.den, stream->time_base.num, AV_TIME_BASE ) );
			}
			else
			{
				return( 0 );
			}
		}

		/// @brief �r�f�I�X�g���[���̑��b����Ԃ�
		virtual long double duration( ) const
		{
			if( is_open( ) )
			{
				AVStream *stream = p_fctx_->streams[ video_stream_index_ ];
				int64_t pts = p_fctx_->streams[ video_stream_index_ ]->duration * AV_TIME_BASE;
				return( static_cast< long double >( av_rescale( pts, stream->time_base.num, stream->time_base.den, AV_TIME_BASE ) ) );
			}
			else
			{
				return( -1 );
			}
		}

		/// @brief ���t���[�����𓾂�
		virtual int64_t number_of_frames( ) const
		{
			if( is_open( ) )
			{
				return( p_fctx_->streams[ video_stream_index_ ]->duration );
			}
			else
			{
				return( 0 );
			}
		}

		/// @brief �t�@�C�����𓾂�
		virtual const std::string filename( ) const
		{
			if( is_open( ) )
			{
				return( p_fctx_->filename );
			}
			else
			{
				return( "" );
			}
		}
	    
		/// @brief �r�b�g���[�g�𓾂�
		virtual size_type bit_rate( )  const
		{
			if( is_open( ) )
			{
				return( p_fctx_->streams[ video_stream_index_ ]->codec->bit_rate );
			}
			else
			{
				return( 0 );
			}
		}
		
		/// @brief �t���[���̕��𓾂�
		virtual size_type width( ) const
		{
			if( is_open( ) )
			{
				return( p_fctx_->streams[ video_stream_index_ ]->codec->width );
			}
			else
			{
				return( 0 );
			}
		}
		
		/// @brief �t���[���̍����𓾂�
		virtual size_type height( ) const
		{
			if( is_open( ) )
			{
				return( p_fctx_->streams[ video_stream_index_ ]->codec->height );
			}
			else
			{
				return( 0 );
			}
		}

		/// @brief �t���[�����[�g�𓾂�
		virtual long double frame_rate_numerator( ) const
		{
			if( is_open( ) )
			{
				return( p_fctx_->streams[ video_stream_index_ ]->codec->time_base.num );
			}
			else
			{
				return( 0 );
			}
		}
		
		/// @brief �t���[�����[�g�x�[�X�𓾂�i���ۂ̃t���[�����[�g���t���[�����[�g/�t���[�����[�g�x�[�X�j
		virtual long double frame_rate_denominator( ) const
		{
			if( is_open( ) )
			{
				return( p_fctx_->streams[ video_stream_index_ ]->codec->time_base.den );
			}
			else
			{
				return( 0 );
			}
		}

		/// @brief �t���[���̃A�X�y�N�g��𓾂�i�E�B���h�E��/�E�B���h�E�����j
		virtual long double frame_aspect_ratio( ) const
		{
			if( is_open( ) )
			{
				return( av_q2d( p_fctx_->streams[ video_stream_index_ ]->codec->sample_aspect_ratio ) * ( double )this->width( ) / this->height( ) );
			}
			else
			{
				return( 4.0 / 3.0 );
			}
		}

	public:
		/// @brief �t�@�C��������͗p�r�f�I�X�g���[�����J��
		//! 
		//! @param[in] filename �c ���̓t�@�C����
		//! 
		virtual bool open( const std::string &filename )
		{
			if( !is_open( ) )
			{
				// �t�@�C���̃w�b�_����ǂݎ���ăr�f�I�t�H�[�}�b�g���擾����
				if( av_open_input_file( &p_fctx_, filename.c_str( ), NULL, 0, NULL ) != 0 )
				{
					printf( "Couldn't open file %s\n", filename.c_str( ) );
					return( false );
				}

				// �t�@�C������X�g���[�������擾����
				if( av_find_stream_info( p_fctx_ ) < 0 )
				{
					printf( "Couldn't find stream information\n" );
					return( false );
				}

				// �r�f�I�X�g���[����T��
				video_stream_index_ = -1;
				for( unsigned int i = 0 ; i < p_fctx_->nb_streams ; i++ )
				{
					if( p_fctx_->streams[ i ]->codec->codec_type == CODEC_TYPE_VIDEO )
					{
						video_stream_index_ = i;
						break;
					}
				}

				// �r�f�I�X�g���[����������Ȃ������̂ŏI������
				if( video_stream_index_ == -1 )
				{
					printf( "Didn't find a video stream\n" );
					return( false );
				}

				AVCodecContext *p_cctx = p_fctx_->streams[ video_stream_index_ ]->codec;

				// �Ή�����R�[�f�b�N���擾����
				AVCodec *p_codec = avcodec_find_decoder( p_cctx->codec_id );
				if( p_codec == NULL )
				{
					printf( "Codec %d not found\n", p_cctx->codec_id );
					return( false );
				}

				// �R�[�f�b�N���J��
				if( avcodec_open( p_cctx, p_codec) < 0 )
				{
					printf( "Could not open codec\n" );
					return( false );
				}

				// �t���[�����Ǘ�����I�u�W�F�N�g���쐬����
				p_frame_src_ = avcodec_alloc_frame( );
				p_frame_rgb_ = allocate_frame( p_cctx->width, p_cctx->height, PIX_FMT_BGR24 );

				// �摜�̕ϊ��p�̃t�B���^��ݒ肷��
				p_swscale_ = sws_getContext( p_cctx->width, p_cctx->height, p_cctx->pix_fmt, p_cctx->width, p_cctx->height, PIX_FMT_BGR24, SWS_FAST_BILINEAR, NULL, NULL, NULL);

				// �t�@�C���̃I�[�v���ɐ��������t���O��ݒ肷��
				is_open_ = true;
				is_eof_  = false;

				// �擪�t���[����ǂݍ���ł���
				this->skip( 1 );

				// �擪�����܂��ǂ߂Ȃ��ꍇ�͍ēx�擪�ɖ߂�
				if( frame_pts_ != 0 )
				{
					this->seek_frame( 0 );
				}

				return( true );
			}

			return( false );
		}

		/// @brief �r�f�I�X�g���[�������
		//! 
		virtual bool close( )
		{
			if( is_open( ) )
			{
				// �t�B���^���������
				sws_freeContext( p_swscale_ );

				// �R�[�f�b�N�����
				avcodec_close( p_fctx_->streams[ video_stream_index_ ]->codec );

				// �t�@�C�������
				av_close_input_file( p_fctx_ );

				// �ꎞ�t���[���̈���������
				av_free( p_frame_src_);

				// �摜�ϊ��p�t���[���̈���������
				free_frame( &p_frame_rgb_ );

				is_open_ = false;

				return( true );
			}

			return( false );
		}

		/// @brief �w�肵���񐔂����t���[���X�L�b�v
		//! 
		//! @param[in] nskips �c �X�L�b�v����
		//!
		bool skip( difference_type nskips = 1 )
		{
			if( is_open( ) && !this->is_eof( ) )
			{
				return( decode( nskips ) && !is_eof( ) );
			}
			else
			{
				return( false );
			}
		}

		/// @brief �w�肵���������r�f�I�X�g���[������T��
		//! 
		//! @param[in] tm �c �r�f�I�X�g���[����ł̈ʒu��\���b��
		//!
		bool seek( long double tm )
		{
			if( is_open( ) && tm >= 0 )
			{
				AVStream *stream = p_fctx_->streams[ video_stream_index_ ];
				int64_t tgt = static_cast< int64_t >( AV_TIME_BASE * tm );
				int64_t pts = av_rescale( tgt, stream->time_base.den, stream->time_base.num, AV_TIME_BASE );

				if( av_seek_frame( p_fctx_, video_stream_index_, pts, AVSEEK_FLAG_BACKWARD ) >= 0 )
				{
					return( decode( -1, tgt ) );
				}
			}

			return( false );
		}

		/// @brief �w�肵���t���[���ԍ����r�f�I�X�g���[������T��
		//! 
		//! @param[in] fno �c �r�f�I�X�g���[����ł̈ʒu��\���t���[���ԍ�
		//!
		bool seek_frame( int64_t fno )
		{
			if( is_open( ) && fno >= 0 )
			{
				if( av_seek_frame( p_fctx_, video_stream_index_, fno, AVSEEK_FLAG_BACKWARD ) >= 0 )
				{
					AVStream *stream = p_fctx_->streams[ video_stream_index_ ];
					return( decode( -1, av_rescale_( AV_TIME_BASE, fno, stream->time_base.num, stream->time_base.den ) ) );
				}
			}

			return( false );
		}

		/// @brief ���݂̃t���[���o�b�t�@��array2�`���ɕϊ����Ċi�[����
		//! 
		//! @param[out] image �c �t���[���摜���i�[���邽�߂̂Q�����z��
		//!
		template < class T, class Allocator >
		bool read_buffer( array2< T, Allocator > &image )
		{
			typedef _pixel_converter_< T > pixel_converter;
			typedef typename pixel_converter::color_type color_type;

			if( is_open( ) && !this->is_eof( ) )
			{
				AVCodecContext *p_cctx = p_fctx_->streams[ video_stream_index_ ]->codec;

				// �摜�̃T�C�Y�𒲐�����
				image.resize( p_cctx->width, p_cctx->height );

				unsigned char *p = p_frame_rgb_->data[ 0 ];

				if( ( image.width( ) % 2 ) == 0 )
				{
					for( size_type i = 0 ; i < image.size( ) ; i++ )
					{
						image[ i ] = pixel_converter::convert_to( p[ 2 ], p[ 1 ], p[ 0 ] );
						p += 3;
					}
				}
				else
				{
					typename array2< T, Allocator >::pointer pi = &image[ 0 ];
					size_type stride = p_frame_rgb_->linesize[ 0 ];
					for( size_type j = 0 ; j < image.height( ) ; j++ )
					{
						for( size_type i = 0 ; i < stride ; i += 3 )
						{
							*pi++ = pixel_converter::convert_to( p[ i + 2 ], p[ i + 1 ], p[ i ] );
						}
						p += stride;
					}
				}

				return( true );
			}
			else
			{
				return( false );
			}
		}

		/// @brief ���݂̃t���[���o�b�t�@��array2�`���ɕϊ����Ċi�[���C�t���[���ԍ���1�i�߂�
		//! 
		//! @param[out] image �c �t���[���摜���i�[���邽�߂̂Q�����z��
		//! 
		//! @return �X�g���[���̏I���𒴂��ēǂݍ��݂��s�����ꍇ�� false ��Ԃ��D
		//!
		template < class T >
		bool read( array2< T > &image )
		{
			if( read_buffer( image ) )
			{
				this->skip( 1 );
				return( true );
			}
			else
			{
				return( false );
			}
		}

		/// @brief ���݂̃t���[���o�b�t�@��array2�`���ɕϊ����Ċi�[���C�t���[���ԍ���1�i�߂�
		//! 
		//! @param[out] image      �c �t���[���摜���i�[���邽�߂̂Q�����z��
		//! @param[out] frame_time �c �ǂݍ��񂾃t���[���̃r�f�I�X�g���[����ł̎������i�[����ϐ�
		//! 
		//! @return �X�g���[���̏I���𒴂��ēǂݍ��݂��s�����ꍇ�� false ��Ԃ��D
		//!
		template < class T >
		bool read( array2< T > &image, long double &frame_time )
		{
			frame_time = this->time( );
			if( read_buffer( image ) )
			{
				this->skip( 1 );
				return( true );
			}
			else
			{
				return( false );
			}
		}

	protected:
		/// @brief �w�肵���񐔂������͎w�肵���ʒu�ɗ���܂Ńt���[�����f�R�[�h����
		//! 
		//! @param[in] ntimes       �c �X�L�b�v����
		//! @param[in] pts          �c �I���ʒu
		//!
		bool decode( difference_type ntimes = 1, int64_t pts = type_limits< int64_t >::maximum( ) )
		{
			if( is_open( ) && !this->is_eof( ) && pts >= 0 )
			{
				if( ntimes >= 0 )
				{
					if( ntimes == 0 )
					{
						return( true );
					}

					AVPacket packet;
					AVStream *stream = p_fctx_->streams[ video_stream_index_ ];
					AVCodecContext *p_cctx = stream->codec;
					int bFinished = 0;

					frame_pts_ = 0;
					for( difference_type i = 1 ; !is_eof( ) && i <= ntimes && frame_pts_ < pts ; i++ )
					{
						bFinished = 0;
						while( bFinished == 0 && !is_eof( ) )
						{
							is_eof_ = av_read_frame( p_fctx_, &packet ) < 0;

							// ����X�g���[����T��
							if( packet.stream_index == video_stream_index_ )
							{
								frame_pts_ = av_rescale_( AV_TIME_BASE, packet.pts < 0 ? packet.dts : packet.pts, ( int64_t ) stream->time_base.num, stream->time_base.den );

								if( !is_eof( ) )
								{
									// �p�P�b�g���ŃR�[�h����
									avcodec_get_frame_defaults( p_frame_src_ );
									avcodec_decode_video( p_cctx, p_frame_src_, &bFinished, packet.data, packet.size );
								}
							}

							if( bFinished != 0 && ( i == ntimes || frame_pts_ >= pts ) )
							{
								// �ꎞ�o�b�t�@�Ƀf�[�^���R�s�[����
								sws_scale( p_swscale_, p_frame_src_->data, p_frame_src_->linesize, 0, p_cctx->height, p_frame_rgb_->data, p_frame_rgb_->linesize );
							}

							av_free_packet( &packet );
						}
					}

					return( bFinished != 0 );
				}
				else
				{
					AVPacket packet;
					AVStream *stream = p_fctx_->streams[ video_stream_index_ ];
					AVCodecContext *p_cctx = stream->codec;
					int bFinished = 0;

					frame_pts_ = pts - 1;
					for( difference_type i = 1 ; !is_eof( ) && frame_pts_ < pts ; i++ )
					{
						bFinished = 0;
						while( bFinished == 0 && !is_eof( ) )
						{
							is_eof_ = av_read_frame( p_fctx_, &packet ) < 0;

							// ����X�g���[����T��
							if( packet.stream_index == video_stream_index_ )
							{
								frame_pts_ = av_rescale_( AV_TIME_BASE, packet.pts < 0 ? packet.dts : packet.pts, ( int64_t ) stream->time_base.num, stream->time_base.den );

								if( !is_eof( ) )
								{
									// �p�P�b�g���ŃR�[�h����
									avcodec_get_frame_defaults( p_frame_src_ );
									avcodec_decode_video( p_cctx, p_frame_src_, &bFinished, packet.data, packet.size );
								}
							}

							if( bFinished != 0 && frame_pts_ >= pts )
							{
								// �ꎞ�o�b�t�@�Ƀf�[�^���R�s�[����
								sws_scale( p_swscale_, p_frame_src_->data, p_frame_src_->linesize, 0, p_cctx->height, p_frame_rgb_->data, p_frame_rgb_->linesize );
							}

							av_free_packet( &packet );
						}
					}

					return( bFinished != 0 );
				}
			}
			else
			{
				return( false );
			}
		}

		int64_t av_rescale( int64_t a, int64_t b, int64_t c ) const
		{
			long double A = static_cast< long double >( a );
			long double B = static_cast< long double >( b );
			long double C = static_cast< long double >( c );
			long double D = A * ( B / C );
			return( static_cast< int64_t >( D + 0.5 ) );
		}

		int64_t av_rescale_( int64_t s, int64_t a, int64_t b, int64_t c ) const
		{
			long double S = static_cast< long double >( s );
			long double A = static_cast< long double >( a );
			long double B = static_cast< long double >( b );
			long double C = static_cast< long double >( c );
			long double D = S * ( A * ( B / C ) );
			return( static_cast< int64_t >( D + 0.5 ) );
		}

		int64_t av_rescale( int64_t a, int64_t b, int64_t c, int64_t d ) const
		{
			long double A = static_cast< long double >( a );
			long double B = static_cast< long double >( b );
			long double C = static_cast< long double >( c );
			long double D = static_cast< long double >( d );
			long double BC = B / C;
			long double AD = A / D;
			return( static_cast< int64_t >( AD * BC + 0.5 ) );
		}

	public:	// �I�y���[�^�̎���
		/// @brief ���̃t���[���֐i��
		decoder & operator ++( )
		{
			this->skip( 1 );
			return( *this );
		}

		/// @brief ��O�̃t���[���֖߂�
		decoder & operator --( )
		{
			this->seek_frame( this->frame_id( ) - 1 );
			//this->seek( this->time( ) - this->seconds_per_frame( ) );
			return( *this );
		}

		/// @brief �r�f�I�X�g���[���̈ʒu���w�肵���t���[���������i�߂�
		//! 
		//! @param[in] fno �c �X�g���[���̈ʒu��i�߂�t���[����
		//!
		decoder & operator +=( difference_type fno )
		{
			this->skip( fno );
			return( *this );
		}

		/// @brief �r�f�I�X�g���[���̈ʒu���w�肵���t���[���������߂�
		//! 
		//! @param[in] fno �c �X�g���[���̈ʒu��i�߂�t���[����
		//!
		decoder & operator -=( difference_type fno )
		{
			this->seek_frame( this->frame_id( ) - fno );
			return( *this );
		}
	};



	/// @brief �r�f�I�o�̓N���X
	//! 
	//! �I�[�f�B�I�X�g���[���͖��T�|�[�g
	//!
	class encoder : public video_io_vase
	{
	protected:
		AVFormatContext	*p_fctx_;				///< @brief �t�H�[�}�b�g���C�f�R�[�_��
		AVFrame			*p_frame_dst_;			///< @brief �����o�����t���[���摜�o�b�t�@
		AVFrame			*p_frame_rgb_;			///< @brief RGB�t�H�[�}�b�g�̃t���[���摜�o�b�t�@(array2�`���̉摜�𓾂邽�߂̒��ԃf�[�^)
		SwsContext		*p_swscale_;			///< @brief �f�R�[�h��̃t���[����RGB�̃t���[���ɕϊ�����t�B���^���w���|�C���^
		int64_t			 frame_pts_;				///< @brief �r�f�I�X�g���[�����ł̌��݂̃t���[���ʒu��ێ�����ϐ�

		bool			is_open_;				///< @brief �r�f�I���J���Ă��邩�ǂ����̃t���O
		uint8_t*		encode_buf_;			///< @brief �G���R�[�h�o�b�t�@
		size_type		encode_buf_size_;		///< @brief �G���R�[�h�o�b�t�@�̃T�C�Y
		size_type		width_;					///< @brief �t���[���摜�̕�
		size_type		height_;				///< @brief �t���[���摜�̍���
		size_type		source_width_;			///< @brief �����Ŏg�p����ϐ�
		size_type		source_height_;			///< @brief �����Ŏg�p����ϐ�
		size_type		frame_rate_num_;		///< @brief �t���[�����[�g
		size_type		frame_rate_den_;		///< @brief �t���[�����[�g�x�[�X�i���ۂ̃t���[�����[�g���t���[�����[�g/�t���[�����[�g�x�[�X�j
		double			frame_aspect_ratio_;	///< @brief �t���[���̃A�X�y�N�g��i���������j
		size_type		bit_rate_;				///< @brief �r�b�g���[�g
		size_type		qmin_;					///< @brief ���k�̃N�I���e�B�[[0�`32]�i�r�b�g���[�g�Ƃǂ��炩���w��j
		size_type		qmax_;					///< @brief ���k�̃N�I���e�B�[[0�`100]�i�r�b�g���[�g�Ƃǂ��炩���w��j
		size_type		gop_size_;				///< @brief GOP�T�C�Y�i���̖����̘A���t���[�����ɕK���P���ȏ�I�t���[�������݂���j
		size_type		max_b_frames_;			///< @brief �ő�A��B�t���[����
		size_type		audio_bit_rate_;		///< @brief �����̃r�b�g���[�g
		size_type		audio_sampling_rate_;	///< @brief �����̃T���v�����O���[�g
		size_type		audio_channels_;		///< @brief �����̃`�����l����

	public:
		/// @brief �R���X�g���N�^
		//! 
		//! @param[in] w                   �c 320�i�f�t�H���g�l�j
		//! @param[in] h                   �c 240�i�f�t�H���g�l�j
		//! @param[in] frame_rate_num      �c 1�i�f�t�H���g�l�j
		//! @param[in] frame_rate_den      �c 30�i�f�t�H���g�l�j
		//! @param[in] frame_aspect_ratio  �c 4/3�i�f�t�H���g�l�j
		//! @param[in] bit_rate            �c 1150000�i�f�t�H���g�l�j
		//! @param[in] qmin                �c �Œ�i���̎w��[0�`31]�i�l���������قǍ��i���j�B0�ȊO���w�肵���ꍇ��bit_rate�͖��������
		//! @param[in] qmax                �c �Œ�i���̎w��[0�`31]�i�l���������قǍ��i���j�B0�ȊO���w�肵���ꍇ��bit_rate�͖��������
		//! @param[in] gop_size            �c 12�i�f�t�H���g�l�j
		//! @param[in] max_b_frames        �c 2�i�f�t�H���g�l�j
		//! @param[in] audio_bit_rate      �c 64000�i�f�t�H���g�l�j
		//! @param[in] audio_sampling_rate �c 44100�i�f�t�H���g�l�j
		//! @param[in] audio_channels      �c 2�i�f�t�H���g�l�j
		//!
		encoder( size_type w = 320, size_type h = 240, size_type frame_rate_num = 1, size_type frame_rate_den = 30, double frame_aspect_ratio = 4.0 / 3.0,
				 size_type bit_rate = 1150000, size_type qmin = 0, size_type qmax = 0, size_type gop_size = 12, size_type max_b_frames = 2, size_type audio_bit_rate = 64000, size_type audio_sampling_rate = 44100, size_type audio_channels = 2 )
					: p_fctx_( NULL ), p_frame_dst_( NULL ), p_frame_rgb_( NULL ), p_swscale_( NULL ), frame_pts_( 0 ), is_open_( false ), encode_buf_( NULL ), encode_buf_size_( 0 ),
						width_( w ), height_( h ), source_width_( w ), source_height_( h ), frame_rate_num_( frame_rate_num ), frame_rate_den_( frame_rate_den ), frame_aspect_ratio_( frame_aspect_ratio ), bit_rate_( bit_rate ), qmin_( qmin ), qmax_( qmax ),
						gop_size_( gop_size ), max_b_frames_( max_b_frames ), audio_bit_rate_( audio_bit_rate ), audio_sampling_rate_( audio_bit_rate ), audio_channels_( audio_channels )
		{
			bool &bInitialized = singleton< bool, 60602 >::get_instance( );
			if( !bInitialized )
			{
				avcodec_register_all( );
				//avdevice_register_all( );
				av_register_all( );
				bInitialized = true;
			}
		}

		/// @brief �f�X�g���N�^
		//! 
		virtual ~encoder( )
		{
			close( );
		}

	public:
		/// @brief �r�f�I�X�g���[�����J���Ă��邩�ǂ�����Ԃ�
		virtual bool is_open( ) const { return( is_open_ ); }

		/// @brief �r�f�I�X�g���[���̏I���ɗ������ǂ�����Ԃ�
		virtual bool is_eof( ) const { return( false ); }

		/// @brief �X�g���[���̃t�H�[�}�b�g��W���o�͂Ƀ_���v����
		virtual bool dump( ) const
		{
			if( is_open( ) )
			{
				dump_format( p_fctx_, 0, p_fctx_->filename, true );
				return( true );
			}
			else
			{
				return( false );
			}
		}

		/// @brief ���݂̃t���[���ԍ��𓾂�
		virtual long double time( ) const
		{
			if( is_open( ) )
			{
				AVStream *stream = p_fctx_->streams[ 0 ];
				int64_t pts = av_rescale( frame_pts_ * AV_TIME_BASE, stream->time_base.num, stream->time_base.den );
				return( static_cast< long double >( pts ) / static_cast< long double >( AV_TIME_BASE ) );
			}
			else
			{
				return( -1 );
			}
		}

		/// @brief ���t���[�����𓾂�
		virtual long double duration( ) const
		{
			if( is_open( ) )
			{
				return( time( ) );
			}
			else
			{
				return( -1 );
			}
		}

		/// @brief �t�@�C�����𓾂�
		virtual const std::string filename( ) const
		{
			if( is_open( ) )
			{
				return( p_fctx_->filename );
			}
			else
			{
				return( "" );
			}
		}
		    
		/// @brief �r�b�g���[�g�𓾂�
		virtual size_type bit_rate( )  const
		{
			return( bit_rate_ );
		}
		
		/// @brief �t���[���̕��𓾂�
		virtual size_type width( ) const
		{
			return( width_ );
		}
		
		/// @brief �t���[���̍����𓾂�
		virtual size_type height( ) const
		{
			return( height_ );
		}

		/// @brief �t���[�����[�g�𓾂�
		virtual long double frame_rate_numerator( ) const
		{
			return( frame_rate_num_ );
		}

		/// @brief �t���[�����[�g�x�[�X�𓾂�i���ۂ̃t���[�����[�g���t���[�����[�g/�t���[�����[�g�x�[�X�j
		virtual long double frame_rate_denominator( ) const
		{
			return( frame_rate_den_ );
		}

		/// @brief �t���[���̃A�X�y�N�g��𓾂�i�E�B���h�E��/�E�B���h�E�����j
		virtual long double frame_aspect_ratio( ) const
		{
			return( frame_aspect_ratio_ );
		}

	public:
		virtual bool open( const std::string &filename )
		{
			return( open( filename, filename, "", "" ) );
		}

		/// @brief �r�f�I�t�@�C���������MIME��񓙂�p���ďo�͗p�r�f�I�X�g���[�����J��
		//! 
		//! @param[in] filename    �c �o�̓t�@�C����
		//! @param[in] format_type �c �r�f�I�̏o�̓t�H�[�}�b�g�ɑΉ������g���q�i"."���܂ށj
		//! @param[in] video_type  �c �o�͂���r�f�I�̖���
		//! @param[in] mime_type   �c �o�͂���r�f�I��MIME�^�C�v
		//! @param[in] codec_id    �c �t�H�[�}�b�g���Ŏg�p����R�[�f�b�NID
		//! 
		bool open( const std::string &filename, const std::string &format_type, const std::string &video_type, const std::string &mime_type, CodecID codec_id = CODEC_ID_NONE )
		{
			if( !is_open_ )
			{
				// �o�̓t�H�[�}�b�g���擾����
				AVOutputFormat	*format = guess_video_format( format_type, video_type, mime_type );
				if( format == NULL )
				{
					std::cerr << "Could not find specified video format" << std::endl;
					return( false );
				}
				else
				{
					return( open( filename, format, codec_id ) );
				}
			}
			else
			{
				return( false );
			}
		}

		/// @brief �r�f�I�X�g���[�������
		//! 
		bool close( )
		{
			if( is_open_ )
			{
				// �t�B���^���������
				sws_freeContext( p_swscale_ );

				// �r�f�I�t�@�C������
				av_write_trailer( p_fctx_ );

				if( ( p_fctx_->oformat->flags & AVFMT_NOFILE ) == 0 )
				{
					if( url_fclose( p_fctx_->pb ) < 0 )
					{
						std::cerr << "Failed to close output file." << std::endl;
						return( false );
					}
				}

				// ���ׂẴX�g���[�����������
				for( size_type i = 0 ; i < p_fctx_->nb_streams ; i++ )
				{
					// CODEC�����
					if( p_fctx_->streams[ i ]->codec->codec_id != CODEC_ID_NONE )
					{
						avcodec_close( p_fctx_->streams[ i ]->codec );
					}

					av_freep( &( p_fctx_->streams[ i ] ->codec ) );
					av_freep( &( p_fctx_->streams[ i ] ) );
				}

				// �G���R�[�h�Ɏg�p�����o�b�t�@�����
				if( encode_buf_ != NULL )
				{
					av_freep( &encode_buf_ );
				}

				// �c��̃f�[�^���������
				free_frame( &p_frame_dst_ );
				free_frame( &p_frame_rgb_ );
				av_freep( &p_fctx_ );

				is_open_ = false;

				return( true );
			}
			else
			{
				return( false );
			}
		}

	protected:
		/// @brief AVOutputFormat ����p���ďo�͗p�r�f�I�X�g���[�����J��
		//! 
		//! @param[in] filename             �c �o�̓t�@�C����
		//! @param[in] format               �c �r�f�I�̏o�̓t�H�[�}�b�g
		//! @param[in] video_codec_id       �c �t�H�[�}�b�g���Ŏg�p����r�f�I�R�[�f�b�NID
		//! @param[in] audio_codec_id       �c �t�H�[�}�b�g���Ŏg�p����I�[�f�B�I�R�[�f�b�NID
		//! @param[in] default_pixel_format �c �t�H�[�}�b�g���Ńf�t�H���g�̉�f�\��
		//! 
		bool open( const std::string &filename, AVOutputFormat *format, CodecID video_codec_id = CODEC_ID_NONE, CodecID audio_codec_id = CODEC_ID_NONE, PixelFormat default_pixel_format = PIX_FMT_YUV420P )
		{
			if( !is_open_ )
			{
				if( format == NULL )
				{
					std::cerr << "Could not find specified video format" << std::endl;
					return( false );
				}

				p_fctx_ = av_alloc_format_context( );
				if( p_fctx_ == NULL )
				{
					std::cerr << "Could not allocate format context" << std::endl;
					return( false );
				}

				p_fctx_->oformat = format;
#ifdef WIN32
				_snprintf( p_fctx_->filename, sizeof( p_fctx_->filename ), "%s", filename.c_str( ) );
#else
				snprintf( p_fctx_->filename, sizeof( p_fctx_->filename ), "%s", filename.c_str( ) );
#endif

				AVStream *vstream = NULL;
				if( p_fctx_->oformat->video_codec != CODEC_ID_NONE )
				{
					vstream = av_new_stream( p_fctx_, 0 );
					if( vstream == NULL )
					{
						std::cerr << "Could not allocate video encode stream" << std::endl;
						return( false );
					}

					// �R�[�f�b�N�̃f�t�H���g�ݒ���s��
					avcodec_get_context_defaults2( vstream->codec, CODEC_TYPE_VIDEO );

					// �R�[�f�b�N�𐄑�����
					if( video_codec_id == CODEC_ID_NONE )
					{
						video_codec_id = av_guess_codec( p_fctx_->oformat, NULL, p_fctx_->filename, NULL, CODEC_TYPE_VIDEO );
					}

					// �R�[�f�b�N��T��
					AVCodec *codec = avcodec_find_encoder( video_codec_id );
					if( codec == NULL && p_fctx_->oformat->video_codec != CODEC_ID_NONE )
					{
						std::cerr << "Could not find appropriate Video Codec from the database." << std::endl;
						return( false );
					}

					// �r�f�I�X�g���[���ɃR�[�f�b�N�����w�肷��
					AVCodecContext *cctx = vstream->codec;
					cctx->codec_id   = codec == NULL ? video_codec_id : codec->id;
					cctx->codec_type = CODEC_TYPE_VIDEO;

					// �摜�̃T�C�Y��ݒ�
					cctx->width      = static_cast< int >( width( ) );
					cctx->height     = static_cast< int >( height( ) );

					// ��f�t�H�[�}�b�g��ݒ�
					cctx->pix_fmt    = default_pixel_format;

					// �s�N�Z���t�H�[�}�b�g�̑Ή��󋵂𒲂ׂ�
					if( codec->pix_fmts )
					{
						const PixelFormat *p = codec->pix_fmts;
						for( ; *p != -1 ; p++ )
						{
							if( *p == cctx->pix_fmt )
							{
								break;
							}
						}

						/* ��Ή��̏ꍇ�͑Ή����镨�ɍ����ւ� */
						if( *p == -1 )
						{
							cctx->pix_fmt = codec->pix_fmts[ 0 ];
						}
					}

					// �r�b�g���[�g��ݒ�
					cctx->bit_rate = static_cast< int >( bit_rate( ) );

					// �Œ�N�I���e�B�[���ݒ肳��Ă���ꍇ�̏���
					if( qmin_ > 0 && qmax_ > 0 )
					{
						cctx->qmin = qmin_;
						cctx->qmax = qmax_;
					}

					// �t���[�����[�g��ݒ�
					cctx->time_base.den = static_cast< int >( frame_rate_denominator( ) );
					cctx->time_base.num = static_cast< int >( frame_rate_numerator( ) );

					cctx->gop_size = gop_size_;

					// �t���[���̃A�X�y�N�g���ݒ肷��
					cctx->sample_aspect_ratio = vstream->sample_aspect_ratio = av_d2q( frame_aspect_ratio_ * cctx->height / cctx->width, 255 );

					if( cctx->codec_id == CODEC_ID_MPEG2VIDEO )
					{
						cctx->max_b_frames = static_cast< int >( max_b_frames_ );
					}
					if( cctx->codec_id == CODEC_ID_MPEG1VIDEO )
					{
						cctx->mb_decision=2;
					}

					// �O���[�o���w�b�_�̗v���𒲂ׂ�
					if( ( format->flags & AVFMT_GLOBALHEADER ) != 0 )
					{
						cctx->flags |= CODEC_FLAG_GLOBAL_HEADER;
					}

					// �p�����[�^��ݒ肷��
					if( av_set_parameters( p_fctx_, NULL ) < 0 ) 
					{
						std::cerr << "Invalid output format parameters" << std::endl;
						return( false );
					}

					// �R�[�f�b�N���J��
					if( avcodec_open( cctx, codec ) < 0 )
					{
						std::cerr << "Could not open codec." << std::endl;
						return( false );
					}
				}

				if( audio_codec_id != CODEC_ID_NONE )
				{
					AVStream *astream = av_new_stream( p_fctx_, 1 );
					if( astream == NULL )
					{
						std::cerr << "Could not allocate audio encode stream" << std::endl;
						return( false );
					}

					astream->codec->codec_id    = audio_codec_id;
					astream->codec->codec_type  = CODEC_TYPE_AUDIO;

					// �I�[�f�B�I�̃X�g���[����ݒ肷��
					astream->codec->bit_rate    = static_cast< int >( audio_bit_rate_ );
					astream->codec->sample_rate = static_cast< int >( audio_sampling_rate_ );
					astream->codec->channels    = static_cast< int >( audio_channels_ );

					// �R�[�f�b�N��T��
					AVCodec *codec = avcodec_find_encoder( astream->codec->codec_id );
					if( codec == NULL )
					{
						std::cerr << "Could not find appropriate Audio Codec from the database." << std::endl;
						return( false );
					}

					// �R�[�f�b�N���J��
					if( avcodec_open( astream->codec, codec ) < 0 )
					{
						std::cerr << "Could not open codec." << std::endl;
						return( false );
					}
				}

				// �t�@�C�����J��
				if( ( p_fctx_->oformat->flags & AVFMT_NOFILE ) == 0 )
				{
					if( url_fopen( &p_fctx_->pb, p_fctx_->filename, URL_WRONLY ) < 0 )
					{
						std::cerr << "Could not open file " << filename << std::endl;
						return( false );
					}
				}

				p_fctx_->preload = static_cast< int >( 0.5 * AV_TIME_BASE );
				p_fctx_->max_delay = static_cast< int >( 0.7 * AV_TIME_BASE );
				p_fctx_->loop_output = AVFMT_NOOUTPUTLOOP;

				// �w�b�_������������
				if( av_write_header( p_fctx_ ) != 0 )
				{
					std::cerr << "Failed to write video header" << std::endl;
					return( false );
				}

				encode_buf_ = NULL;
				encode_buf_size_ = width( ) * height( ) * 4;
				if( ( p_fctx_->oformat->flags & AVFMT_RAWPICTURE ) == 0 )
				{
					encode_buf_ = ( uint8_t * )av_malloc( encode_buf_size_ );
				}

				PixelFormat pix_fmt = vstream == NULL ? PIX_FMT_BGR24 : vstream->codec->pix_fmt;

				// �G���R�[�h�p�̃t���[���o�b�t�@��p�ӂ���
				p_frame_dst_ = allocate_frame( width( ), height( ), pix_fmt );
				if( p_frame_dst_ == NULL )
				{
					std::cerr << "Could not allocate frame buffer." << std::endl;
					return( false );
				}

				// �ꎞ�̈�p�̃t���[���o�b�t�@��p�ӂ���
				p_frame_rgb_ = allocate_frame( width( ), height( ), PIX_FMT_BGR24 );
				if( p_frame_dst_ == NULL )
				{
					std::cerr << "Could not allocate temporal frame buffer." << std::endl;
					return( false );
				}

				// �摜�̕ϊ��p�̃t�B���^��ݒ肷��
				source_width_  = width( );
				source_height_ = height( );
				p_swscale_ = sws_getContext( source_width_, source_height_, PIX_FMT_BGR24, width( ), height( ), pix_fmt, SWS_LANCZOS, NULL, NULL, NULL);

				is_open_   = true;
				frame_pts_ = 0;

				return( true );
			}
			else
			{
				return( false );
			}
		}

	public:
		/// @brief array2�`���̉摜���t���[���o�b�t�@�ɏ������݁C�G���R�[�h���ăX�g���[���ɏo�͂���
		//! 
		//! @param[in] image �c �r�f�I�X�g���[���ɃG���R�[�h����摜
		//! @param[in] tm    �c �r�f�I�X�g���[���ɃG���R�[�h����ۂ̃^�C���X�^���v�i-1�̏ꍇ�̓t���[�����[�g�����Ƃɏ������݈ʒu���Z�o����j
		//! 
		template < class T, class Allocator >
		bool write( const array2< T, Allocator > &image, long double tm = -1.0 )
		{
			if( is_open( ) )
			{
				int ret = 0;

				if( source_width_ != image.width( ) || source_height_ != image.height( ) )
				{
					if( p_swscale_ != NULL )
					{
						// �t�B���^���������
						sws_freeContext( p_swscale_ );

						// �t���[�����������
						free_frame( &p_frame_rgb_ );
					}

					PixelFormat pix_fmt = p_fctx_->streams[ 0 ] == NULL ? PIX_FMT_BGR24 : p_fctx_->streams[ 0 ]->codec->pix_fmt;

					if( source_width_ < image.width( ) )
					{
						p_swscale_ = sws_getContext( image.width( ), image.height( ), PIX_FMT_BGR24, width( ), height( ), pix_fmt, SWS_AREA, NULL, NULL, NULL);
					}
					else
					{
						p_swscale_ = sws_getContext( image.width( ), image.height( ), PIX_FMT_BGR24, width( ), height( ), pix_fmt, SWS_LANCZOS, NULL, NULL, NULL);
					}

					source_width_  = image.width( );
					source_height_ = image.height( );
					p_frame_rgb_ = allocate_frame( source_width_, source_height_, PIX_FMT_BGR24 );
				}

				typedef _pixel_converter_< T > pixel_converter;
				typedef typename pixel_converter::color_type color_type;

				unsigned char *p = p_frame_rgb_->data[ 0 ];
				if( ( source_width_ % 2 ) == 0 )
				{
					for( size_type i = 0 ; i < image.size( ) ; i++ )
					{
						color_type c = pixel_converter::convert_from( image[ i ] );
						p[ 0 ] = c.b;
						p[ 1 ] = c.g;
						p[ 2 ] = c.r;
						p += 3;
					}
				}
				else
				{
					typename array2< T, Allocator >::const_pointer pi = &image[ 0 ];
					size_type stride = p_frame_rgb_->linesize[ 0 ];
					for( size_type j = 0 ; j < image.height( ) ; j++ )
					{
						for( size_type i = 0 ; i < stride ; i += 3 )
						{
							color_type c = pixel_converter::convert_from( *pi++ );
							p[ i + 0 ] = c.b;
							p[ i + 1 ] = c.g;
							p[ i + 2 ] = c.r;
						}

						p += stride;
					}
				}

				// �摜�̌`����ϊ�����
				sws_scale( p_swscale_, p_frame_rgb_->data, p_frame_rgb_->linesize, 0, source_height_, p_frame_dst_->data, p_frame_dst_->linesize );

				if( ( p_fctx_->oformat->flags & AVFMT_RAWPICTURE ) != 0 )
				{
					AVPacket packet;
					av_init_packet( &packet );

					packet.flags |= PKT_FLAG_KEY;
					packet.stream_index = p_fctx_->streams[ 0 ]->index;
					packet.data = ( uint8_t * )p_frame_dst_;
					packet.size = sizeof( AVPicture );
					frame_pts_++;

					ret = av_write_frame( p_fctx_, &packet );
				}
				else
				{
					// �摜���G���R�[�h����
					AVStream  *stream = p_fctx_->streams[ 0 ];
					AVCodecContext *c = stream->codec;
					p_frame_dst_->quality = static_cast< int >( stream->quality );
					int out_size = avcodec_encode_video( c, encode_buf_, encode_buf_size_, p_frame_dst_ );

					if( out_size > 0 )
					{
						AVPacket packet;
						av_init_packet( &packet );

						packet.stream_index = stream->index;
						packet.data         = encode_buf_;
						packet.size         = out_size;
						if( tm < 0 )
						{
							packet.pts = av_rescale_q( c->coded_frame->pts, c->time_base, stream->time_base );
							frame_pts_ = static_cast< difference_type >( packet.pts );
						}
						else
						{
							packet.pts = av_rescale( static_cast< int64_t >( tm * AV_TIME_BASE ), stream->time_base.den, stream->time_base.num * AV_TIME_BASE );
							frame_pts_ = static_cast< difference_type >( packet.pts );
						}

						if( c->coded_frame->key_frame )
						{
							packet.flags |= PKT_FLAG_KEY;
						}

						ret = av_interleaved_write_frame( p_fctx_, &packet );
						//ret = av_write_frame( p_fctx_, &packet );
					}
					else
					{
						ret = 0;
					}
				}

				return( ret != 0 );
			}
			else
			{
				return( false );
			}
		}
	};

	namespace mpeg1
	{
		/// @brief MPEG1�r�f�I�o�̓N���X
		//! 
		//! �I�[�f�B�I�X�g���[���͖��T�|�[�g
		//!
		class encoder : public video::encoder
		{
		private:
			typedef video::encoder base;

		public:
			/// @brief �R���X�g���N�^
			//! 
			//! @param[in] w                   �c 320�i�f�t�H���g�l�j
			//! @param[in] h                   �c 240�i�f�t�H���g�l�j
			//! @param[in] frame_rate_num      �c 1�i�f�t�H���g�l�j
			//! @param[in] frame_rate_den      �c 30�i�f�t�H���g�l�j
			//! @param[in] frame_aspect_ratio  �c 4/3�i�f�t�H���g�l�j
			//! @param[in] bit_rate            �c 1150000�i�f�t�H���g�l�j
			//! @param[in] qmin                �c �Œ�i���̎w��[0�`31]�i�l���������قǍ��i���j�B0�ȊO���w�肵���ꍇ��bit_rate�͖��������
			//! @param[in] qmax                �c �Œ�i���̎w��[0�`31]�i�l���������قǍ��i���j�B0�ȊO���w�肵���ꍇ��bit_rate�͖��������
			//! @param[in] gop_size            �c 12�i�f�t�H���g�l�j
			//! @param[in] max_b_frames        �c 2�i�f�t�H���g�l�j
			//!
			encoder( size_type w = 320, size_type h = 240, size_type frame_rate_num = 1, size_type frame_rate_den = 30, double frame_aspect_ratio = 4.0 / 3.0, size_type bit_rate = 1150000, size_type qmin = 0, size_type qmax = 0, size_type gop_size = 12, size_type max_b_frames = 2 )
				: base( w, h, frame_rate_num, frame_rate_den, frame_aspect_ratio, bit_rate, qmin, qmax, gop_size, max_b_frames )
			{
			}

			/// @brief �R���X�g���N�^
			//! 
			//! �R���X�g���N�^�̎��s���ɏo�̓r�f�I�t�@�C���̏��������s��
			//! 
			//! @param[in] filename            �c �o�̓t�@�C����
			//! @param[in] w                   �c 320�i�f�t�H���g�l�j
			//! @param[in] h                   �c 240�i�f�t�H���g�l�j
			//! @param[in] frame_rate_num      �c 1�i�f�t�H���g�l�j
			//! @param[in] frame_rate_den      �c 30�i�f�t�H���g�l�j
			//! @param[in] frame_aspect_ratio  �c 4/3�i�f�t�H���g�l�j
			//! @param[in] bit_rate            �c 1150000�i�f�t�H���g�l�j
			//! @param[in] qmin                �c �Œ�i���̎w��[0�`31]�i�l���������قǍ��i���j�B0�ȊO���w�肵���ꍇ��bit_rate�͖��������
			//! @param[in] qmax                �c �Œ�i���̎w��[0�`31]�i�l���������قǍ��i���j�B0�ȊO���w�肵���ꍇ��bit_rate�͖��������
			//! @param[in] gop_size            �c 12�i�f�t�H���g�l�j
			//! @param[in] max_b_frames        �c 2�i�f�t�H���g�l�j
			//!
			encoder( const std::string &filename, size_type w = 320, size_type h = 240, size_type frame_rate_num = 1, size_type frame_rate_den = 30, double frame_aspect_ratio = 4.0 / 3.0, size_type bit_rate = 1150000, size_type qmin = 0, size_type qmax = 0, size_type gop_size = 12, size_type max_b_frames = 2 )
				: base( w, h, frame_rate_num, frame_rate_den, frame_aspect_ratio, bit_rate, qmin, qmax, gop_size, max_b_frames )
			{
				if( !open( filename ) )
				{
					throw;
				}
			}

			/// @brief �f�X�g���N�^
			//! 
			virtual ~encoder( )
			{
				base::close( );
			}

		public:
			virtual bool open( const std::string &filename )
			{
				return( base::open( filename, ".mpg", "mpeg", "" ) );
			}
		};
	}

	namespace mpeg4
	{
		/// @brief MPEG4�r�f�I�o�̓N���X
		//! 
		//! �I�[�f�B�I�X�g���[���͖��T�|�[�g
		//!
		class encoder : public video::encoder
		{
		private:
			typedef video::encoder base;

		public:
			/// @brief �R���X�g���N�^
			//! 
			//! @param[in] w                   �c 320�i�f�t�H���g�l�j
			//! @param[in] h                   �c 240�i�f�t�H���g�l�j
			//! @param[in] frame_rate_num      �c 1�i�f�t�H���g�l�j
			//! @param[in] frame_rate_den      �c 30�i�f�t�H���g�l�j
			//! @param[in] frame_aspect_ratio  �c 4/3�i�f�t�H���g�l�j
			//! @param[in] bit_rate            �c 1150000�i�f�t�H���g�l�j
			//!
			encoder( size_type w = 320, size_type h = 240, size_type frame_rate_num = 1, size_type frame_rate_den = 30, double frame_aspect_ratio = 4.0 / 3.0, size_type bit_rate = 1150000 )
				: base( w, h, frame_rate_num, frame_rate_den, frame_aspect_ratio, bit_rate )
			{
			}

			/// @brief �R���X�g���N�^
			//! 
			//! �R���X�g���N�^�̎��s���ɏo�̓r�f�I�t�@�C���̏��������s��
			//! 
			//! @param[in] filename            �c �o�̓t�@�C����
			//! @param[in] w                   �c 320�i�f�t�H���g�l�j
			//! @param[in] h                   �c 240�i�f�t�H���g�l�j
			//! @param[in] frame_rate_num      �c 1�i�f�t�H���g�l�j
			//! @param[in] frame_rate_den      �c 30�i�f�t�H���g�l�j
			//! @param[in] frame_aspect_ratio  �c 4/3�i�f�t�H���g�l�j
			//! @param[in] bit_rate            �c 1150000�i�f�t�H���g�l�j
			//!
			encoder( const std::string &filename, size_type w = 320, size_type h = 240, size_type frame_rate_num = 1, size_type frame_rate_den = 30, double frame_aspect_ratio = 4.0 / 3.0, size_type bit_rate = 1150000 )
				: base( w, h, frame_rate_num, frame_rate_den, frame_aspect_ratio, bit_rate )
			{
				if( !open( filename ) )
				{
					throw;
				}
			}
			
			/// @brief �f�X�g���N�^
			//! 
			virtual ~encoder( )
			{
				base::close( );
			}

		public:
			virtual bool open( const std::string &filename )
			{
				//return( base::open( filename, "m4v", "" ) );
				return( base::open( filename, ".avi", "", "" ) );
			}
		};
	}

	namespace mjpeg
	{
		/// @brief MotionJPEG�r�f�I�o�̓N���X
		//! 
		//! �I�[�f�B�I�X�g���[���͖��T�|�[�g
		//!
		class encoder : public video::encoder
		{
		private:
			typedef video::encoder base;

		public:
			/// @brief �R���X�g���N�^
			//! 
			//! @param[in] w                   �c 320�i�f�t�H���g�l�j
			//! @param[in] h                   �c 240�i�f�t�H���g�l�j
			//! @param[in] frame_rate_num      �c 1�i�f�t�H���g�l�j
			//! @param[in] frame_rate_den      �c 30�i�f�t�H���g�l�j
			//! @param[in] frame_aspect_ratio  �c 4/3�i�f�t�H���g�l�j
			//! @param[in] bit_rate            �c 11500000�i�f�t�H���g�l�j
			//! @param[in] qmin                �c �Œ�i���̎w��[0�`31]�i�l���������قǍ��i���j�B0�ȊO���w�肵���ꍇ��bit_rate�͖��������
			//! @param[in] qmax                �c �Œ�i���̎w��[0�`31]�i�l���������قǍ��i���j�B0�ȊO���w�肵���ꍇ��bit_rate�͖��������
			//!
			encoder( size_type w = 320, size_type h = 240, size_type frame_rate_num = 1, size_type frame_rate_den = 30, double frame_aspect_ratio = 4.0 / 3.0, size_type bit_rate = 11500000, size_type qmin = 0, size_type qmax = 0 )
				: base( w, h, frame_rate_num, frame_rate_den, frame_aspect_ratio, bit_rate, qmin, qmax, 0 )
			{
			}

			/// @brief �R���X�g���N�^
			//! 
			//! �R���X�g���N�^�̎��s���ɏo�̓r�f�I�t�@�C���̏��������s��
			//! 
			//! @param[in] filename            �c �o�̓t�@�C����
			//! @param[in] w                   �c 320�i�f�t�H���g�l�j
			//! @param[in] h                   �c 240�i�f�t�H���g�l�j
			//! @param[in] frame_rate_num      �c 1�i�f�t�H���g�l�j
			//! @param[in] frame_rate_den      �c 30�i�f�t�H���g�l�j
			//! @param[in] frame_aspect_ratio  �c 4/3�i�f�t�H���g�l�j
			//! @param[in] bit_rate            �c 11500000�i�f�t�H���g�l�j
			//! @param[in] qmin                �c �Œ�i���̎w��[0�`31]�i�l���������قǍ��i���j�B0�ȊO���w�肵���ꍇ��bit_rate�͖��������
			//! @param[in] qmax                �c �Œ�i���̎w��[0�`31]�i�l���������قǍ��i���j�B0�ȊO���w�肵���ꍇ��bit_rate�͖��������
			//!
			encoder( const std::string &filename, size_type w = 320, size_type h = 240, size_type frame_rate_num = 1, size_type frame_rate_den = 30, double frame_aspect_ratio = 4.0 / 3.0, size_type bit_rate = 11500000, size_type qmin = 0, size_type qmax = 0 )
				: base( w, h, frame_rate_num, frame_rate_den, frame_aspect_ratio, bit_rate, qmin, qmax, 0 )
			{
				if( !open( filename ) )
				{
					throw;
				}
			}
			
			/// @brief �f�X�g���N�^
			//! 
			virtual ~encoder( )
			{
				base::close( );
			}

		public:
			virtual bool open( const std::string &filename )
			{
				//return( base::open( filename, "m4v", "" ) );
				return( base::open( filename, ".avi", "", "", CODEC_ID_MJPEG ) );
			}
		};
	}

	namespace h264
	{
		/// @brief H264�r�f�I�o�̓N���X
		//! 
		//! �I�[�f�B�I�X�g���[���͖��T�|�[�g
		//!
		class encoder : public video::encoder
		{
		private:
			typedef video::encoder base;

		public:
			/// @brief �R���X�g���N�^
			//! 
			//! @param[in] w                   �c 320�i�f�t�H���g�l�j
			//! @param[in] h                   �c 240�i�f�t�H���g�l�j
			//! @param[in] frame_rate_num      �c 1�i�f�t�H���g�l�j
			//! @param[in] frame_rate_den      �c 30�i�f�t�H���g�l�j
			//! @param[in] frame_aspect_ratio  �c 4/3�i�f�t�H���g�l�j
			//! @param[in] bit_rate            �c 1150000�i�f�t�H���g�l�j
			//!
			encoder( size_type w = 320, size_type h = 240, size_type frame_rate_num = 1, size_type frame_rate_den = 30, double frame_aspect_ratio = 4.0 / 3.0, size_type bit_rate = 1150000 )
				: base( w, h, frame_rate_num, frame_rate_den, frame_aspect_ratio, bit_rate )
			{
			}

			/// @brief �R���X�g���N�^
			//! 
			//! �R���X�g���N�^�̎��s���ɏo�̓r�f�I�t�@�C���̏��������s��
			//! 
			//! @param[in] filename            �c �o�̓t�@�C����
			//! @param[in] w                   �c 320�i�f�t�H���g�l�j
			//! @param[in] h                   �c 240�i�f�t�H���g�l�j
			//! @param[in] frame_rate_num      �c 1�i�f�t�H���g�l�j
			//! @param[in] frame_rate_den      �c 30�i�f�t�H���g�l�j
			//! @param[in] frame_aspect_ratio  �c 4/3�i�f�t�H���g�l�j
			//! @param[in] bit_rate            �c 1150000�i�f�t�H���g�l�j
			//!
			encoder( const std::string &filename, size_type w = 320, size_type h = 240, size_type frame_rate_num = 1, size_type frame_rate_den = 30, double frame_aspect_ratio = 4.0 / 3.0, size_type bit_rate = 1150000 )
				: base( w, h, frame_rate_num, frame_rate_den, frame_aspect_ratio, bit_rate )
			{
				if( !open( filename ) )
				{
					throw;
				}
			}
			
			/// @brief �f�X�g���N�^
			//! 
			virtual ~encoder( )
			{
				base::close( );
			}

		public:
			virtual bool open( const std::string &filename )
			{
				//return( base::open( filename, "m4v", "" ) );
				return( base::open( filename, ".avi", "", "", CODEC_ID_H264 ) );
			}
		};
	}

	namespace wmv
	{
		/// @brief WMV�r�f�I�o�̓N���X
		//! 
		//! �I�[�f�B�I�X�g���[���͖��T�|�[�g
		//!
		class encoder : public video::encoder
		{
		private:
			typedef video::encoder base;

		public:
			/// @brief �R���X�g���N�^
			//! 
			//! @param[in] w                   �c 320�i�f�t�H���g�l�j
			//! @param[in] h                   �c 240�i�f�t�H���g�l�j
			//! @param[in] frame_rate_num      �c 1�i�f�t�H���g�l�j
			//! @param[in] frame_rate_den      �c 30�i�f�t�H���g�l�j
			//! @param[in] frame_aspect_ratio  �c 4/3�i�f�t�H���g�l�j
			//! @param[in] bit_rate            �c 1150000�i�f�t�H���g�l�j
			//!
			encoder( size_type w = 320, size_type h = 240, size_type frame_rate_num = 1, size_type frame_rate_den = 30, double frame_aspect_ratio = 4.0 / 3.0, size_type bit_rate = 1150000 )
				: base( w, h, frame_rate_num, frame_rate_den, frame_aspect_ratio, bit_rate )
			{
			}

			/// @brief �R���X�g���N�^
			//! 
			//! �R���X�g���N�^�̎��s���ɏo�̓r�f�I�t�@�C���̏��������s��
			//! 
			//! @param[in] filename            �c �o�̓t�@�C����
			//! @param[in] w                   �c 320�i�f�t�H���g�l�j
			//! @param[in] h                   �c 240�i�f�t�H���g�l�j
			//! @param[in] frame_rate_num      �c 1�i�f�t�H���g�l�j
			//! @param[in] frame_rate_den      �c 30�i�f�t�H���g�l�j
			//! @param[in] frame_aspect_ratio  �c 4/3�i�f�t�H���g�l�j
			//! @param[in] bit_rate            �c 1150000�i�f�t�H���g�l�j
			//!
			encoder( const std::string &filename, size_type w = 320, size_type h = 240, size_type frame_rate_num = 1, size_type frame_rate_den = 30, double frame_aspect_ratio = 4.0 / 3.0, size_type bit_rate = 1150000 )
				: base( w, h, frame_rate_num, frame_rate_den, frame_aspect_ratio, bit_rate )
			{
				if( !open( filename ) )
				{
					throw;
				}
			}
			
			/// @brief �f�X�g���N�^
			//! 
			virtual ~encoder( )
			{
				base::close( );
			}

		public:
			virtual bool open( const std::string &filename )
			{
				//return( base::open( filename, "m4v", "" ) );
				return( base::open( filename, ".wmv", "", "", CODEC_ID_WMV2 ) );
			}
		};
	}

	namespace dv
	{
		/// @brief DV�r�f�I�o�̓N���X
		//! 
		//! �I�[�f�B�I�X�g���[���͖��T�|�[�g
		//!
		class encoder : public video::encoder
		{
		private:
			typedef video::encoder base;

		public:
			/// @brief �R���X�g���N�^
			//! 
			//! @param[in] w                   �c 720�i�f�t�H���g�l�j
			//! @param[in] h                   �c 480�i�f�t�H���g�l�j
			//! @param[in] frame_aspect_ratio  �c 4/3�i�f�t�H���g�l�j
			//!
			encoder( size_type w = 720, size_type h = 480, double frame_aspect_ratio = 4.0 / 3.0 )
				: base( w, h, 1001, 30000, frame_aspect_ratio )
			{
				audio_sampling_rate_ = 48000;
			}

			/// @brief �R���X�g���N�^
			//! 
			//! �R���X�g���N�^�̎��s���ɏo�̓r�f�I�t�@�C���̏��������s��
			//! 
			//! @param[in] filename            �c �o�̓t�@�C����
			//! @param[in] w                   �c 720�i�f�t�H���g�l�j
			//! @param[in] h                   �c 480�i�f�t�H���g�l�j
			//! @param[in] frame_aspect_ratio  �c 4/3�i�f�t�H���g�l�j
			//!
			encoder( const std::string &filename, size_type w = 720, size_type h = 480, double frame_aspect_ratio = 4.0 / 3.0 )
				: base( w, h, 1001, 30000, frame_aspect_ratio )
			{
				audio_sampling_rate_ = 48000;

				if( !open( filename ) )
				{
					throw;
				}
			}

			/// @brief �f�X�g���N�^
			//! 
			virtual ~encoder( )
			{
				base::close( );
			}

		public:
			virtual bool open( const std::string &filename )
			{
				AVOutputFormat *fmt = guess_video_format( ".avi", "", "" );
				return( base::open( filename, fmt, CODEC_ID_DVVIDEO, CODEC_ID_NONE, PIX_FMT_YUV411P ) );
			}
		};
	}

	namespace raw
	{
		/// @brief RAW�r�f�I�o�̓N���X
		//! 
		//! �I�[�f�B�I�X�g���[���͖��T�|�[�g
		//!
		class encoder : public video::encoder
		{
		private:
			typedef video::encoder base;

		public:
			/// @brief �R���X�g���N�^
			//! 
			//! @param[in] w                   �c 320�i�f�t�H���g�l�j
			//! @param[in] h                   �c 240�i�f�t�H���g�l�j
			//! @param[in] frame_rate_num      �c 1�i�f�t�H���g�l�j
			//! @param[in] frame_rate_den      �c 30�i�f�t�H���g�l�j
			//! @param[in] frame_aspect_ratio  �c 4/3�i�f�t�H���g�l�j
			//!
			encoder( size_type w = 320, size_type h = 240, size_type frame_rate_num = 1, size_type frame_rate_den = 30, double frame_aspect_ratio = 4.0 / 3.0 )
				: base( w, h, frame_rate_num, frame_rate_den, frame_aspect_ratio )
			{
			}

			/// @brief �R���X�g���N�^
			//! 
			//! �R���X�g���N�^�̎��s���ɏo�̓r�f�I�t�@�C���̏��������s��
			//! 
			//! @param[in] filename            �c �o�̓t�@�C����
			//! @param[in] w                   �c 320�i�f�t�H���g�l�j
			//! @param[in] h                   �c 240�i�f�t�H���g�l�j
			//! @param[in] frame_rate_num      �c 1�i�f�t�H���g�l�j
			//! @param[in] frame_rate_den      �c 30�i�f�t�H���g�l�j
			//! @param[in] frame_aspect_ratio  �c 4/3�i�f�t�H���g�l�j
			//!
			encoder( const std::string &filename, size_type w = 320, size_type h = 240, size_type frame_rate_num = 1, size_type frame_rate_den = 30, double frame_aspect_ratio = 4.0 / 3.0 )
				: base( w, h, frame_rate_num, frame_rate_den, frame_aspect_ratio )
			{
				if( !open( filename ) )
				{
					throw;
				}
			}

			/// @brief �f�X�g���N�^
			//! 
			virtual ~encoder( )
			{
				base::close( );
			}

		public:
			virtual bool open( const std::string &filename )
			{
				return( base::open( filename, ".avi", "", "", CODEC_ID_RAWVIDEO ) );
			}
		};
	}


	template < class T, class Allocator >
	inline decoder &operator >>( decoder &in, array2< T, Allocator > &img )
	{
		in.read( img );
		return( in );
	}

	template < class T, class Allocator >
	inline encoder &operator <<( encoder &in, array2< T, Allocator > &img )
	{
		in.write( img );
		return( in );
	}

	/// @brief �w�肵���񐔂����r�f�I�X�g���[���ԂŃt���[���摜�̃R�s�[
	//! 
	//! @param[in,out] src         �c �R�s�[���r�f�I�X�g���[��
	//! @param[out]    dst         �c �R�s�[��r�f�I�X�g���[��
	//! @param[in]     num         �c �R�s�[��
	//!
	template< typename Stream1, typename Stream2 >
	inline bool copy_frames( Stream1 &src, Stream2 &dst, const size_t num = 1 )
	{
		array2< rgb< unsigned char > > tmp;

		for( size_t i = 0 ; i < num ; i ++ )
		{
			if( src.read( tmp ) )
			{
				if( !dst.write( tmp ) )
				{
					return( false );
				}
			}
			else
			{
				return( false );
			}
		}

		return( true );
	}
}


/// @}
// video_group �r�f�I�t�@�C���̓��o�͂��s�����C�u�����̏I���


// mist���O��Ԃ̏I���
_MIST_END



#endif // __INCLUDE_IO_VIDEO_H__
