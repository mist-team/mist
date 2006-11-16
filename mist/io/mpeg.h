// 
// Copyright (c) 2003-2006, MIST Project, Intelligent Media Integration COE, Nagoya University
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

/// @file mist/io/mpeg.h
//!
//! @brief mpeg1, mpeg2�`���r�f�I�O���[�����o��
//!

#ifndef __INCLUDE_IO_MPEG_H__
#define __INCLUDE_IO_MPEG_H__



#include <avcodec.h>
#include <avformat.h>

#include <mist/mist.h>
#include <mist/config/color.h>
#include <iostream>



// mist���O��Ԃ̎n�܂�
_MIST_BEGIN



namespace __libavcodec__
{
	static bool is_libavcodec_initialized = false;		///< @brief libavcodec�̏������t���O
}



//! @defgroup mpeg_group mpeg1, mpeg2�`���r�f�I�X�g���[�����o�͂̂��߂̃��C�u����
//!
//! �{���C�u�����́Chttp://ffmpeg.sourceforge.net/index.php �ŊJ�����s���Ă��� ffmpeg-0.4.8 �� libavcodec, libavformat �𗘗p���Ă���
//! �G���R�[�h�ƃf�R�[�h�͂��̃��C�u������API��p���Ď�������Ă��邽�߁C�ʓr���C�u������p�ӂ���K�v������
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/io/mpeg.h>
//! @endcode
//!
//! @code �g�p��(mpeg�̃r�f�I�X�g���[������A��bmp�t�@�C�����쐬)
//! mist::array2< mist::rgb< unsigned char > > img;
//! mist::mpeg::input_video iv;
//! mist::mpeg::open( iv, "input.mpeg" );
//! mist::mpeg::dump_format( iv );
//! char f_name[ 256 ];
//! size_t i = 0;
//! while( mist::mpeg::read_frame( iv, img ) )
//! {
//!		sprintf( f_name, "tmp/frame%04d.bmp", i ++ );
//!		mist::write_bmp( img, f_name );
//! }
//! mist::mpeg::close( iv );
//! @endcode
//!
//! @code �g�p��(�A��bmp����mpeg1video�X�g���[�����쐬)
//! mist::array2< mist::rgb< unsigned char > > img;
//! mist::mpeg::output_video ov;
//! // �p�����[�^�̕ύX�i�y���ӁI�z�X�g���[�����J���O�ɕύX���Ȃ���΂Ȃ�Ȃ��j
//! // ov.encode_buf_size( 524288 );	// �\���Ɋm�ۂ���
//! // ov.bit_rate( 1000000 );			// ���Ȃ��ƕi����������
//! ov.width( 320 );					// �摜�̃T�C�Y�ɍ��킹�ĕύX
//! ov.height( 240 );					// �摜�̃T�C�Y�ɍ��킹�ĕύX
//! // ov.frame_rate( 30 );				// �t���[�����[�g
//! // ov.frame_rate_base( 1 );			// 
//! // ov.gop_size( 12 );				// �����ƕi����������
//! // ov.max_b_frames( 2 );			// �����ƕi����������
//! mist::mpeg::open( ov, "output.mpeg" );
//! mist::mpeg::dump_format( ov );
//! char f_name[ 256 ];
//! const size_t frame_num = 100;       // �A��bmp�t�@�C���̐�
//! for( size_t i = 0 ; i < frame_num ; i ++ )
//! {
//!		sprintf( f_name, "tmp/frame%04d.bmp", i );
//!		mist::read_bmp( img, f_name );
//!		mist::mpeg::write_frame( ov, img );
//! }
//! mist::mpeg::close( ov );
//! @endcode
//!
//! @code �g�p��(�X�g���[�����̊e�t���[���̉摜�����t�B���^����)
//! mist::array2< mist::rgb< unsigned char > > in_img, out_img;
//! mist::mpeg::input_video iv;
//! mist::mpeg::output_video ov;
//! mist::mpeg::open( iv, "input.mpeg" );
//! mist::mpeg::copy_format( iv, ov );
//! // �K�v�Ȃ�ov�̃p�����[�^�̕ύX�i�𑜓x�ϊ�����悤�ȏꍇ�ɂ̓t���[���̕��ƍ�����ύX�j
//! mist::mpeg::open( ov, "output.mpeg" );
//! mist::mpeg::dump_format( iv );
//! mist::mpeg::dump_format( ov );
//! while( mist::mpeg::read_frame( iv, in_img ) )
//! {
//!		// in_img����͂���out_img���o�͂���悤�ȃt�B���^
//!		mist::gaussian( in_img, out_img ); // ��
//!		mist::mpeg::write_frame( ov, out_img );
//! }
//! mist::mpeg::close( iv );
//! mist::mpeg::close( ov );
//! @endcode
//!
//! //! @code �g�p��(mpeg���̔C�ӂ̃t���[������mpeg1video�X�g���[�����쐬)
//! mist::mpeg::input_video iv;
//! mist::mpeg::output_video ov; 
//! mist::mpeg::open( iv, "input.mpeg" );
//! mist::mpeg::copy_format( iv, ov );
//! // �K�v�Ȃ�ov�̃p�����[�^�̕ύX
//! mist::mpeg::open( ov, "output.mpeg" );
//! mist::mpeg::dump_format( iv );
//! mist::mpeg::dump_format( ov );
//! mist::mpeg::skip_frame( iv, 60 );      // ����Ȃ��t���[���̓X�L�b�v
//! mist::mpeg::copy_frame( iv, ov, 240 ); // �K�v�ȃt���[���̂݃R�s�[
//! mist::mpeg::close( iv );
//! mist::mpeg::close( ov );
//! @endcode
//!
//!
//!  @{



/// @brief mpeg1,mpeg2�t�@�C������r�f�I�X�g���[���̓ǂݍ��݁Cmpeg1video�r�f�I�X�g���[����mpeg�t�@�C�������o��
namespace mpeg
{

	/// @brief �قȂ�X�g���[���ԂŃt���[���o�b�t�@�̂������s�����߂̃N���X
	//! 
	class frame_t
	{
		AVFrame *p_frame_;
		PixelFormat pix_fmt_;
	public:
		frame_t( AVFrame *p_frame = NULL, const PixelFormat pix_fmt = PIX_FMT_NB ) : p_frame_( p_frame ), pix_fmt_( pix_fmt )
		{
		}
		AVFrame *p_frame( ) const 
		{
			return p_frame_;
		}
		PixelFormat pix_fmt( ) const 
		{
			return pix_fmt_;
		}
		~frame_t( )
		{
		}
	};
	

	/// @brief mpeg�r�f�I���̓N���X
	//! 
	//! �I�[�f�B�I�X�g���[���͖��T�|�[�g
	//!
	class input_video
	{

	private:

		AVFormatContext	*p_fctx_;				///< @brief mpeg�̃t�H�[�}�b�g���C�f�R�[�_��
		AVFrame			*p_frame_src_;			///< @brief �ǂݏo���ꂽ�t���[���摜�o�b�t�@
		AVFrame			*p_frame_rgb_;			///< @brief RGB�t�H�[�}�b�g�̃t���[���摜�o�b�t�@�iarray2�`���̃t���[���摜�𓾂邽�߂̒��ԃf�[�^�j
		bool			is_open_;				///< @brief �r�f�I���J���Ă��邩�ǂ����̃t���O
		int				video_stream_index_;	///< @brief �r�f�I���w���X�g���[���ԍ�
		AVPacket		decoding_packet_;		///< @brief �f�R�[�h���̃p�P�b�g
		int				bytes_not_decoded_;		///< @brief �܂��f�R�[�h����Ă��Ȃ��f�[�^�̃o�C�g��
		uint8_t			*p_decoding_packet_;	///< @brief �f�R�[�h���̃p�P�b�g���̈ʒu���w���|�C���^

	public:

		/// @brief �R���X�g���N�^
		//! 
		//! �f�t�H���g�R���X�g���N�^
		//! 
		input_video( ) : p_fctx_( NULL ), p_frame_src_( NULL ), p_frame_rgb_( NULL ), is_open_( false ), video_stream_index_( -1 ), bytes_not_decoded_( 0 ), p_decoding_packet_( NULL ) 
		{
			if( !__libavcodec__::is_libavcodec_initialized )
			{
				av_register_all( );
				__libavcodec__::is_libavcodec_initialized = true;
			}
			decoding_packet_.data = NULL;
		}

		/// @brief �f�X�g���N�^
		//! 
		~input_video( )
		{ 
			close( );
		}

		/// @brief mpeg�t�@�C��������͗p�r�f�I�X�g���[�����J��
		//! 
		//! @param[in] filename �c ����mpeg�t�@�C��
		//! 
		bool open( const char *filename )
		{
			if( !is_open_ )
			{
				if( av_open_input_file( &p_fctx_, filename, NULL, 0, NULL ) != 0 )
				{
					printf( "Couldn't open file %s\n", filename );
					return false;
				}
				if( av_find_stream_info( p_fctx_ ) < 0 )
				{
					printf( "Couldn't find stream information\n" );
					return false;
				}
				video_stream_index_ = -1;
				for( int i = 0 ; i < p_fctx_->nb_streams ; i ++ )
				{
					if( p_fctx_->streams[ i ]->codec.codec_type == CODEC_TYPE_VIDEO )
					{
						video_stream_index_ = i;
						break;
					}
				}
				if( video_stream_index_ == -1 )
				{
					printf( "Didn't find a video stream\n" );
					return false;
				}
				AVCodecContext *p_cctx = &p_fctx_->streams[ video_stream_index_ ]->codec;
				AVCodec *p_codec = avcodec_find_decoder( p_cctx->codec_id );
				if( p_codec == NULL )
				{
					printf( "Codec not found\n" );
					return false;
				}
				if( p_codec->capabilities & CODEC_CAP_TRUNCATED )
				{
					p_cctx->flags |= CODEC_FLAG_TRUNCATED;
				}
				if( avcodec_open( p_cctx, p_codec) < 0 )
				{
					printf( "Could not open codec\n" );
					return false;
				}
				if( p_cctx->frame_rate > 1000 && p_cctx->frame_rate_base == 1 )
				{
					p_cctx->frame_rate_base = 1000;
				}
				p_frame_src_ = avcodec_alloc_frame( );
				p_frame_rgb_ = avcodec_alloc_frame( );
				size_t size = avpicture_get_size( PIX_FMT_RGB24, p_cctx->width, p_cctx->height );
				uint8_t *rgb_buf = new uint8_t [ size ];
				avpicture_fill( ( AVPicture * )p_frame_rgb_, rgb_buf, PIX_FMT_RGB24, p_cctx->width, p_cctx->height );

				is_open_ = true;
				return true;
			}
			return false;
		}

		/// @brief �r�f�I�X�g���[�������
		//! 
		bool close( )
		{
			if( is_open_ )
			{
				av_free( p_frame_src_);
				delete [ ] p_frame_rgb_->data[ 0 ];
				av_free( p_frame_rgb_);
				avcodec_close( &p_fctx_->streams[ video_stream_index_ ]->codec );
				av_close_input_file( p_fctx_ );

				is_open_ = false;
				return true;
			}
			return false;
		}

		/// @brief �r�f�I�X�g���[�����J���Ă��邩�ǂ�����Ԃ�
		//! 
		bool is_open( ) const
		{
			return is_open_;
		}

		/// @brief �X�g���[���̃t�H�[�}�b�g��W���o�͂Ƀ_���v����
		//! 
		bool dump_format( ) const
		{
			if( is_open_ )
			{
				::dump_format( p_fctx_, 0, p_fctx_->filename, false );
				return true;
			}
			return false;
		}

		/// @brief ���݂̃t���[���o�b�t�@�𓾂�
		//! 
		frame_t frame( ) const
		{
			if( is_open_ )
			{
				return frame_t( p_frame_src_, p_fctx_->streams[ video_stream_index_ ]->codec.pix_fmt );
			}
			return frame_t( );
		}

		/// @brief ���݂̃t���[���ԍ��𓾂�
		//! 
		int frame_number( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.frame_number;
			}
			return -1;
		}

		/// @brief ���݂̃t���[�����L�[�t���[���Ȃ�^��Ԃ�
		//! 
		bool is_key_frame( ) const
		{
			if( is_open_ )
			{
				return p_frame_src_->key_frame == 1;
			}
			return false;
		}

		/// @brief �w�肵���񐔂����t���[���X�L�b�v
		//! 
		//! @param[in] num �c �X�L�b�v����
		//!
		bool skip( const size_t num = 1 )
		{
			if( is_open_ )
			{
				size_t i = 0;
				while( i < num )
				{
					if( !read( ) )
					{
						return false;
					}
					i ++;
				}
				return true;
			}
			return false;
		}

		/// @brief �f�R�[�h���ꂽ�t���[���o�b�t�@��array2�`���ɕϊ����Ċi�[(RGB)
		//! 
		//! @param[out] image �c �t���[���摜���i�[���邽�߂̂Q�����z��
		//!
		void get( mist::array2< mist::rgb< unsigned char > > &image )
		{
			AVCodecContext *p_cctx = &p_fctx_->streams[ video_stream_index_ ]->codec;
			img_convert( ( AVPicture * )p_frame_rgb_, PIX_FMT_RGB24, ( AVPicture * )p_frame_src_, p_cctx->pix_fmt, p_cctx->width, p_cctx->height );
			image.resize( p_cctx->width, p_cctx->height );
			unsigned char *p = p_frame_rgb_->data[ 0 ];
			for( size_t i = 0 ; i < image.size( ) ; i ++ )
			{
				image[ i ].r = *( p ++ );
				image[ i ].g = *( p ++ );
				image[ i ].b = *( p ++ );
			}
		}

		/// @brief �f�R�[�h���ꂽ�t���[���摜��array2�`���ɕϊ����Ċi�[(�O���C�X�P�[��)
		//! 
		//! @param[out] image �c �t���[���摜���i�[���邽�߂̂Q�����z��
		//!
		void get( mist::array2< unsigned char > &image )
		{
			AVCodecContext *p_cctx = &p_fctx_->streams[ video_stream_index_ ]->codec;
			img_convert( ( AVPicture * )p_frame_rgb_, PIX_FMT_RGB24, ( AVPicture * )p_frame_src_, p_cctx->pix_fmt, p_cctx->width, p_cctx->height );
			image.resize( p_cctx->width, p_cctx->height );
			unsigned char *p = p_frame_rgb_->data[ 0 ];
			for( size_t i = 0 ; i < image.size( ) ; i ++ )
			{
				image[ i ] = ( unsigned char )( *p * 0.298912 + *( p + 1 ) * 0.586610 + *( p + 2 ) * 0.114478 );
				p += 3;
			}
		}

		/// @brief �t���[���ǂݍ���ŁC�f�R�[�h���ăt���[���o�b�t�@�Ɋi�[
		//! 
		bool read( )
		{
			int bytes_decoded, frame_finished;
			AVCodecContext *p_cctx = &p_fctx_->streams[ video_stream_index_ ]->codec;
			while( true )
			{
				while( bytes_not_decoded_ > 0 )
				{
					bytes_decoded = avcodec_decode_video( p_cctx, p_frame_src_, &frame_finished, p_decoding_packet_, bytes_not_decoded_);						
					if( bytes_decoded < 0 )
					{
						printf( "Error while decoding frame\n" );
						return false;
					}
					bytes_not_decoded_ -= bytes_decoded;
					p_decoding_packet_ += bytes_decoded;
					if( frame_finished != 0 )
					{
						return true;
					}
				}
				do
				{
					if( decoding_packet_.data != NULL )
					{
						av_free_packet( &decoding_packet_ );
					}
					if( av_read_packet( p_fctx_, &decoding_packet_ ) < 0 )
					{
						goto loop_exit;
					}				
				} while( decoding_packet_.stream_index != video_stream_index_ );
				bytes_not_decoded_ = decoding_packet_.size;
				p_decoding_packet_ = decoding_packet_.data;
			}
	loop_exit:
			bytes_decoded = avcodec_decode_video( p_cctx, p_frame_src_, &frame_finished, p_decoding_packet_, bytes_not_decoded_);	
			if( decoding_packet_.data != NULL )
			{
				av_free_packet( &decoding_packet_ );
			}
			return frame_finished != 0;
		}

		/// @brief �t���[���o�b�t�@�Ɋi�[���ꂽ�摜�̃C���^�[���X����
		//! 
		bool deinterlace( )
		{
			AVCodecContext *p_cctx = &p_fctx_->streams[ video_stream_index_ ]->codec;
			return avpicture_deinterlace( ( AVPicture * )p_frame_src_, ( AVPicture * )p_frame_src_, p_cctx->pix_fmt, p_cctx->width, p_cctx->height) >= 0;
		}

		/// @brief mpeg�t�@�C�����𓾂�
		//! 
		char *filename( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->filename;
			}
			return NULL;
		}
	    
		/// @brief �r�b�g���[�g�𓾂�
		//! 
		int bit_rate( )  const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.bit_rate;
			}
			return 0;
		}
		
		/// @brief �t���[���̕��𓾂�
		//! 
		int width( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.width;
			}
			return 0;
		}
		
		/// @brief �t���[���̍����𓾂�
		//! 
		int height( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.height;
			}
			return 0;
		}
		
		/// @brief �t���[�����[�g�𓾂�
		//! 
		int frame_rate( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.frame_rate;
			}
			return 0;
		}
		
		/// @brief �t���[�����[�g�x�[�X�𓾂�i���ۂ̃t���[�����[�g���t���[�����[�g/�t���[�����[�g�x�[�X�j
		//! 
		int frame_rate_base( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.frame_rate_base;
			}
			return 0;
		}
		
		/// @brief GOP�T�C�Y�𓾂�i���̖����̘A���t���[�����ɕK���P���ȏ�I�t���[�������݂���j
		//! 
		int gop_size( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.gop_size;
			}
			return 0;
		}
		
		/// @brief �ő�A��B�t���[�����𓾂�
		//! 
		int max_b_frames( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.max_b_frames;
			}
			return 0;
		}

	};



	/// @brief mpeg�r�f�I�o�̓N���X
	//! 
	//! �I�[�f�B�I�X�g���[���͖��T�|�[�g
	//!
	class output_video
	{

	private:

		AVFormatContext	*p_fctx_;				///< @brief mpeg�̃t�H�[�}�b�g���C�f�R�[�_��
		AVFrame			*p_frame_dst_;			///< @brief �����o�����t���[���摜�o�b�t�@
		AVFrame			*p_frame_rgb_;			///< @brief RGB�t�H�[�}�b�g�̃t���[���摜�o�b�t�@(array2�`���̉摜�𓾂邽�߂̒��ԃf�[�^)
		bool			is_open_;				///< @brief �r�f�I���J���Ă��邩�ǂ����̃t���O
		const int		video_stream_index_;	///< @brief �r�f�I���w���X�g���[���ԍ�
		int				encode_buf_size_;		///< @brief �G���R�[�h�o�b�t�@�̃T�C�Y
		uint8_t			*encode_buf_;			///< @brief �G���R�[�h�o�b�t�@
		int				bit_rate_;				///< @brief �r�b�g���[�g
		int				width_;					///< @brief �t���[���摜�̕�
		int				height_;				///< @brief �t���[���摜�̍���
		int				frame_rate_;			///< @brief �t���[�����[�g
		int				frame_rate_base_;		///< @brief �t���[�����[�g�x�[�X�i���ۂ̃t���[�����[�g���t���[�����[�g/�t���[�����[�g�x�[�X�j
		int				gop_size_;				///< @brief GOP�T�C�Y�i���̖����̘A���t���[�����ɕK���P���ȏ�I�t���[�������݂���j
		int				max_b_frames_;			///< @brief �ő�A��B�t���[����

	public:

		/// @brief �R���X�g���N�^
		//! 
		//! @param[in] encode_buf_size �c 4194304�i�f�t�H���g�l�j
		//! @param[in] bit_rate        �c 1048576�i�f�t�H���g�l�j
		//! @param[in] width           �c 160�i�f�t�H���g�l�j
		//! @param[in] height          �c 120�i�f�t�H���g�l�j
		//! @param[in] frame_rate      �c 30�i�f�t�H���g�l�j
		//! @param[in] frame_rate_base �c 1�i�f�t�H���g�l�j
		//! @param[in] gop_size        �c 12�i�f�t�H���g�l�j
		//! @param[in] max_b_frames    �c 2�i�f�t�H���g�l�j
		//!
		output_video( const size_t encode_buf_size = 4194304, const size_t bit_rate = 1048576, const int width = 160, const int height = 120, const int frame_rate = 30, const int frame_rate_base = 1, const int gop_size = 12, const int max_b_frames = 2 )
			: p_fctx_( NULL ), p_frame_dst_( NULL ), p_frame_rgb_( NULL ), is_open_( false ), video_stream_index_( 0 ), encode_buf_size_( encode_buf_size ), bit_rate_( bit_rate ), width_( width ), height_( height ), frame_rate_( frame_rate ), frame_rate_base_( frame_rate_base ), gop_size_( gop_size ), max_b_frames_( max_b_frames )
		{
			if( !__libavcodec__::is_libavcodec_initialized )
			{
				av_register_all( );
				__libavcodec__::is_libavcodec_initialized = true;
			}
		}
		
		/// @brief �f�X�g���N�^
		//! 
		~output_video( )
		{
			close( );
		}

		/// @brief mpeg�t�@�C������o�͗p�r�f�I�X�g���[�����J��
		//! 
		//! @param[in] filename �c �o��mpeg�t�@�C��
		//! 
		bool open( const char *filename )
		{
			if( !is_open_ )
			{	
				p_fctx_ = ( AVFormatContext * )av_mallocz( sizeof( AVFormatContext ) );
				if( !p_fctx_ ) {
					printf( "Memory error\n" );
					return false;
				}
				p_fctx_->oformat = guess_format( "mpeg", NULL, NULL );
				AVStream *p_st= av_new_stream( p_fctx_, 0 );
				if( !p_st )
				{
					printf( "Could not alloc stream\n" );
					return false;
				}
				AVCodecContext *p_cctx = &p_st->codec;
				p_cctx->codec_id = ( CodecID )p_fctx_->oformat->video_codec;
				p_cctx->codec_type = CODEC_TYPE_VIDEO;
				p_cctx->bit_rate = bit_rate_;//ic->bit_rate;  
				p_cctx->width = width_;  
				p_cctx->height = height_;
				p_cctx->frame_rate = frame_rate_;  
				p_cctx->frame_rate_base = frame_rate_base_;
				p_cctx->gop_size = gop_size_;
				p_cctx->max_b_frames = max_b_frames_;
				p_fctx_->oformat->audio_codec = CODEC_ID_NONE;
				if( av_set_parameters( p_fctx_, NULL ) < 0 ) 
				{
					printf( "Invalid output format parameters\n" );
					return false;
				}
				AVCodec *p_codec = avcodec_find_encoder( p_cctx->codec_id );
				if(	!p_codec )
				{
					printf( "Codec not found\n" );
					return false;
				}
				if( avcodec_open( p_cctx, p_codec ) < 0) 
				{
					printf( "could not open codec\n" );
					return false;
				}
				encode_buf_ = ( uint8_t * )malloc( encode_buf_size_ );
				p_frame_dst_ = avcodec_alloc_frame( );
				size_t size = avpicture_get_size( PIX_FMT_YUV420P, p_cctx->width, p_cctx->height );
				uint8_t *buf_dst = new uint8_t [ size ];
				avpicture_fill( ( AVPicture * )p_frame_dst_, buf_dst, PIX_FMT_YUV420P, p_cctx->width, p_cctx->height );
				p_frame_rgb_ = avcodec_alloc_frame( );
				size = avpicture_get_size( PIX_FMT_RGB24, p_cctx->width, p_cctx->height );
				uint8_t *buf_rgb = new uint8_t [ size ];
				avpicture_fill( ( AVPicture * )p_frame_rgb_, buf_rgb, PIX_FMT_RGB24, p_cctx->width, p_cctx->height );
				sprintf( p_fctx_->filename, "%s", filename );
				if( !( p_fctx_->oformat->flags & AVFMT_NOFILE ) ) 
				{
					if( url_fopen( &p_fctx_->pb, filename, URL_WRONLY ) < 0 ) 
					{
						printf( "Could not open '%s'\n", filename );
						return false;
					}
				}
				av_write_header( p_fctx_ );

				is_open_ = true;
				return true;
			}
			return false;
		}

		/// @brief �r�f�I�X�g���[�������
		//! 
		bool close( )
		{
			if( is_open_ )
			{
				delete [ ] p_frame_dst_->data[ 0 ];
				av_free( p_frame_dst_ );
				p_frame_dst_ = NULL;
				while( write( ) )
				{
				}
				av_write_trailer( p_fctx_ ); 
				delete [ ] p_frame_rgb_->data[ 0 ];
				av_free( p_frame_rgb_ );
				avcodec_close( &p_fctx_->streams[ video_stream_index_ ]->codec );
				av_free( encode_buf_ );			
				av_freep( &p_fctx_->streams[ video_stream_index_ ] );
				av_free( p_fctx_ );

				is_open_ = false;
				return true;
			}
			return false;
		}

		/// @brief �r�f�I���J���Ă��邩�ǂ�����Ԃ�
		//! 
		bool is_open( ) const
		{
			return is_open_;
		}

		/// @brief �X�g���[���̃t�H�[�}�b�g��W���o�͂Ƀ_���v����
		//! 
		bool dump_format( ) const
		{
			if( is_open_ )
			{
				::dump_format( p_fctx_, 0, p_fctx_->filename, true );
				return true;
			}
			return false;
		}

		/// @brief ���݂̃t���[���o�b�t�@�𓾂�
		//! 
		frame_t frame( ) const
		{
			if( is_open_ )
			{
				return frame_t( p_fctx_->streams[ video_stream_index_ ]->codec.coded_frame, p_fctx_->streams[ video_stream_index_ ]->codec.pix_fmt );
			}
			return frame_t( );
		}

		/// @brief ���݂̃t���[���ԍ��𓾂�
		//! 
		int frame_number( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.coded_frame->coded_picture_number;//.frame_number;
			}
			return -1;
		}

		/// @brief ���݂̃t���[�����L�[�t���[���ł���΁A�^��Ԃ�
		//! 
		bool is_key_frame( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.coded_frame->key_frame == 1;
			}
			return false;
		}

		/// @brief array2�`���̉摜���t���[���o�b�t�@�ɏ�������(RGB)
		//! 
		//! @param[in] image �c �t���[���摜
		//!
		bool put( const mist::array2< mist::rgb< unsigned char > > &image )
		{
			if( is_open_ )
			{
				unsigned char *p = p_frame_rgb_->data[ 0 ];
				for( size_t i = 0 ; i < image.size( ) ; i ++ )
				{
					*( p ++ ) = image[ i ].r;
					*( p ++ ) = image[ i ].g;
					*( p ++ ) = image[ i ].b;
				}
				AVCodecContext *p_cctx = &p_fctx_->streams[ video_stream_index_ ]->codec;
				img_convert((AVPicture *)p_frame_dst_, p_cctx->pix_fmt/*PIX_FMT_YUV420P*/, (AVPicture*)p_frame_rgb_, PIX_FMT_RGB24, p_cctx->width, p_cctx->height);
				return true;		
			}
			return false;
		}

		/// @brief array2�`���̉摜���t���[���o�b�t�@�ɏ�������(�O���C�X�P�[��)
		//! 
		//! @param[in] image �c �t���[���摜
		//!
		bool put( const mist::array2< unsigned char > &image )
		{
			if( is_open_ )
			{
				unsigned char *p = p_frame_rgb_->data[ 0 ];
				for( size_t i = 0 ; i < image.size( ) ; i ++ )
				{
					*( p ++ ) = image[ i ];
					*( p ++ ) = image[ i ];
					*( p ++ ) = image[ i ];
				}
				AVCodecContext *p_cctx = &p_fctx_->streams[ video_stream_index_ ]->codec;
				img_convert((AVPicture *)p_frame_dst_, p_cctx->pix_fmt/*PIX_FMT_YUV420P*/, (AVPicture*)p_frame_rgb_, PIX_FMT_RGB24, p_cctx->width, p_cctx->height);
				return true;
			}
			return false;
		}

		/// @brief frame_t�^�̃t���[���摜���t���[���o�b�t�@�ɏ�������
		//! 
		//! @param[in] frame �c frame_t�^�̃t���[���摜
		//!
		bool put( const frame_t &frame )
		{
			if( is_open_ )
			{
				AVCodecContext *p_cctx = &p_fctx_->streams[ video_stream_index_ ]->codec;
				img_convert((AVPicture *)p_frame_dst_, p_cctx->pix_fmt/*PIX_FMT_YUV420P*/, (AVPicture*)( frame.p_frame( ) ), frame.pix_fmt( ), p_cctx->width, p_cctx->height);
				return true;
			}
			return false;
		}

		/// @brief �t���[���o�b�t�@�̓��e���G���R�[�h���ăX�g���[���ɏ�������
		//! 
		bool write( )
		{
			if( is_open_ )
			{
				AVStream *p_st = p_fctx_->streams[ video_stream_index_ ];
				int bytes_encoded = avcodec_encode_video( &p_st->codec, encode_buf_, encode_buf_size_, p_frame_dst_ );
				if( bytes_encoded != 0 ) 
				{
					if( av_write_frame( p_fctx_, p_st->index, encode_buf_, bytes_encoded ) != 0 )
					{
						printf( "Error while writing video frame\n" );
					}
				} 
				return bytes_encoded != 0;
			}
			return false;
		}

		/// @brief �t���[���o�b�t�@�Ɋi�[���ꂽ�摜�̃C���^�[���X����
		bool deinterlace( )
		{
			AVCodecContext *p_cctx = &p_fctx_->streams[ video_stream_index_ ]->codec;
			return avpicture_deinterlace( ( AVPicture * )p_frame_dst_, ( AVPicture * )p_frame_dst_, p_cctx->pix_fmt, p_cctx->width, p_cctx->height) >= 0;
		}

		/// @brief �G���R�[�h�o�b�t�@�T�C�Y��ݒ�
		//! 
		//! @param[in] encode_buf_size �c �G���R�[�h�o�b�t�@�T�C�Y
		//!
		bool encode_buf_size( const int encode_buf_size )
		{
			if( !is_open_ )
			{
				encode_buf_size_ = encode_buf_size;
				return true;
			}
			return false;
		}

		/// @brief �r�b�g���[�g��ݒ�
		//! 
		//! @param[in] bit_rate �c �r�b�g���[�g
		//!
		bool bit_rate( const int bit_rate )
		{
			if( !is_open_ )
			{
				bit_rate_ = bit_rate;
				return true;
			}
			return false;
		}

		/// @brief �t���[���摜�̕���ݒ�(�Q�̔{��)
		//! 
		//! @param[in] width �c �t���[���摜�̕�
		//!
		bool width( int width )
		{
			if( !is_open_ )
			{
				if( width % 2 == 1 )
				{
					width ++;
				}
				width_ = width;
				return true;
			}
			return false;
		}

		/// @brief �t���[���摜�̍�����ݒ�(�Q�̔{��)
		//! 
		//! @param[in] height �c �t���[���摜�̍���
		//!
		bool height( int height )
		{
			if( !is_open_ )
			{
				if( height % 2 == 1 )
				{
					height ++;
				}
				height_ = height;
				return true;
			}
			return false;
		}

		/// @brief �t���[�����[�g��ݒ�
		//! 
		//! @param[in] frame_rate �c �t���[�����[t
		//!
		bool frame_rate( const int frame_rate )
		{
			if( !is_open_ )
			{
				frame_rate_ = frame_rate;
				return true;
			}
			return false;
		}

		/// @brief �t���[�����[�g�x�[�X��ݒ�i���ۂ̃t���[�����[�g���t���[�����[�g/�t���[�����[�g�x�[�X�j
		//! 
		//! @param[in] frame_rate_base �c �t���[�����[�g�x�[�X
		//!
		bool frame_rate_base( const int frame_rate_base )
		{
			if( !is_open_ )
			{
				frame_rate_base_ = frame_rate_base;
				return true;
			}
			return false;
		}

		/// @brief GOP�T�C�Y��ݒ�i���̖����̘A���t���[�����ɕK���P���ȏ�I�t���[�������݂���j
		//! 
		//! @param[in] gop_size �c GOP�T�C�Y
		//!
		bool gop_size( const int gop_size )
		{
			if( !is_open_ )
			{
				gop_size_ = gop_size;
				return true;
			}
			return false;
		}

		/// @brief �ő�A��B�t���[������ݒ�
		//! 
		//! @param[in] max_b_frames �c �ő�A��B�t���[����
		//!
		bool max_b_frames( const int max_b_frames )
		{
			if( !is_open_ )
			{
				max_b_frames_ = max_b_frames;
				return true;
			}
			return false;
		}

		/// @brief mpeg�t�@�C�����𓾂�
		//! 
		char *filename( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->filename;
			}
			return NULL;
		}

		/// @brief �r�b�g���[�g�𓾂�
		//! 
		int bit_rate( )  const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.bit_rate;
			}
			return 0;
		}

		/// @brief �t���[���摜�̕��𓾂�
		//! 
		int width( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.width;
			}
			return 0;
		}

		/// @brief �t���[���摜�̍����𓾂�
		//! 
		int height( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.height;
			}
			return 0;
		}

		/// @brief �t���[�����[�g�𓾂�
		//! 
		int frame_rate( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.frame_rate;
			}
			return 0;
		}

		/// @brief �t���[�����[�g�x�[�X�𓾂�i���ۂ̃t���[�����[�g���t���[�����[�g/�t���[�����[�g�x�[�X�j
		//! 
		int frame_rate_base( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.frame_rate_base;
			}
			return 0;
		}

		/// @brief GOP�T�C�Y�𓾂�i���̖����̘A���t���[�����ɕK���P���ȏ�I�t���[�������݂���j
		//! 
		int gop_size( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.gop_size;
			}
			return 0;
		}

		/// @brief �ő�A��B�t���[�����𓾂�
		//! 
		int max_b_frames( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.max_b_frames;
			}
			return 0;
		}

	};



	/// @brief �t�@�C������X�g���[�����J��
	//! 
	//! @param[in,out] av              �c �X�g���[��
	//! @param[in] filename             �c �t�@�C����
	//!
	template< typename Stream >
	inline bool open( Stream &av, const char *filename )
	{
		return av.open( filename );
	}

	/// @brief �X�g���[�������
	//! 
	//! @param[in,out]  av              �c �X�g���[��
	//!
	template< typename Stream >
	inline bool close( Stream &av )
	{
		return av.close( );
	}

	/// @brief �X�g���[�����J���Ă��邩�ǂ�����Ԃ�
	//! 
	//! @param[in]  av              �c �X�g���[��
	//!
	template< typename Stream >
	inline bool is_open( const Stream &av )
	{
		return av.is_open( );
	}

	/// @brief �X�g���[���̃t�H�[�}�b�g��W���o�͂Ƀ_���v����
	//! 
	//! @param[in]  av              �c �X�g���[��
	//!
	template< typename Stream >
	inline bool dump_format( const Stream &av )
	{
		return av.dump_format( );
	}

	/// @brief �r�f�I�X�g���[������t���[�������o����array2�`���̉摜�Ɋi�[
	//! 
	//! @param[in,out] video           �c �r�f�I�X�g���[��
	//! @param[out]    image           �c �t���[���摜
	//!
	template< typename Stream, typename Image >
	inline bool read_frame( Stream &video, Image &image )
	{
		if ( video.read( ) )
		{
			video.get( image );
			return true;
		}
		return false;
	}

	/// @brief array2�`���̉摜���r�f�I�X�g���[���ɏ�������
	//! 
	//! @param[in,out] video           �c �r�f�I�X�g���[��
	//! @param[in]     image           �c �t���[���摜
	//!
	template< typename Stream, typename Image >
	inline bool write_frame( Stream &video, const Image &image )
	{
		video.put( image );
		return video.write( );
	}

	/// @brief �w�肵���񐔂����t���[���X�L�b�v
	//! 
	//! @param[in,out] video         �c �r�f�I�X�g���[��
	//! @param[in]     num           �c �X�L�b�v��
	//!
	template< typename Stream >
	inline bool skip_frame( Stream &video, const size_t num = 1 )
	{
		return video.skip( num );
	}

	/// @brief �w�肵���񐔂����r�f�I�X�g���[���ԂŃt���[���摜�̃R�s�[
	//! 
	//! @param[in,out] src         �c �R�s�[���r�f�I�X�g���[��
	//! @param[out]    dst         �c �R�s�[��r�f�I�X�g���[��
	//! @param[in]     num         �c �R�s�[��
	//!
	template< typename Stream1, typename Stream2 >
	inline bool copy_frame( Stream1 &src, Stream2 &dst, const size_t num = 1 )
	{
		for( size_t i = 0 ; i < num ; i ++ )
		{
			if( src.read( ) )
			{
				dst.put( src.frame( ) );
				if( !dst.write( ) )
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		return true;
	}

	/// @brief �r�f�I�X�g���[���ԂŃt�H�[�}�b�g���̃R�s�[
	//! 
	//! @param[in,out] src         �c �R�s�[���r�f�I�X�g���[��
	//! @param[out]    dst         �c �R�s�[��r�f�I�X�g���[��
	//!
	template< typename Stream1, typename Stream2 >
	inline bool copy_format( const Stream1 &src, Stream2 &dst )
	{
		if( src.is_open( ) && !dst.is_open( ) )
		{
			dst.bit_rate(        src.bit_rate( ) );
			dst.width(           src.width( ) );
			dst.height(          src.height( ) );
			dst.frame_rate(      src.frame_rate( ) );
			dst.frame_rate_base( src.frame_rate_base( ) );
			dst.gop_size(        src.gop_size( ) );
			dst.max_b_frames(    src.max_b_frames( ) );
			return true;
		}
		return false;
	}

} // namespace mpeg



// mist���O��Ԃ̏I���
_MIST_END



#endif // __INCLUDE_IO_MPEG_H__
