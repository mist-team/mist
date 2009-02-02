// 
// Copyright (c) 2003-2008, MIST Project, Intelligent Media Integration COE, Nagoya University
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


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#ifndef __INCLUDE_MIST_COLOR_H__
#include "../config/color.h"
#endif

#include <string>

extern "C"
{
	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>
	#include <libswscale/swscale.h>
}


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
//! mist::mpeg::decoder iv;
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
//! mist::mpeg::decoder iv;
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
//! mist::mpeg::decoder iv;
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
namespace video
{

	/// @brief �قȂ�X�g���[���ԂŃt���[���o�b�t�@�̂������s�����߂̃N���X
	//! 
	class frame_t
	{
	private:
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

	class video_io_vase
	{
	public:
		typedef size_t		size_type;
		typedef ptrdiff_t	difference_type;

	public:
		virtual ~video_io_vase( ){ }

		virtual bool open( const std::string &filename ) = 0;	///< @brief 
		virtual bool close( ) = 0;								///< @brief 
		virtual bool is_open( ) const = 0;						///< @brief �r�f�I�X�g���[�����J���Ă��邩�ǂ�����Ԃ�
		virtual bool is_eof( ) const = 0;						///< @brief �r�f�I�X�g���[���̏I���ɗ������ǂ�����Ԃ�
		virtual bool dump( ) const = 0;							///< @brief �X�g���[���̃t�H�[�}�b�g��W���o�͂Ƀ_���v����
		virtual difference_type frame_id( ) const = 0;						///< @brief ���݂̃t���[���ԍ���Ԃ�
		virtual const std::string filename( ) const = 0;		///< @brief �t�@�C������Ԃ�
		virtual size_type bit_rate( ) const = 0;				///< @brief �r�b�g���[�g��Ԃ�
		virtual size_type width( ) const = 0;					///< @brief �t���[���̕���Ԃ�
		virtual size_type height( ) const = 0;					///< @brief �t���[���̍�����Ԃ�
		virtual size_type frame_rate_numerator( ) const = 0;	///< @brief �t���[�����[�g�iA��B�j�̕��q������A��Ԃ�
		virtual size_type frame_rate_denominator( ) const = 0;	///< @brief �t���[�����[�g�iA��B�j�̕��ꕔ����B��Ԃ�

		/// @brief �t���[�����[�g��Ԃ�
		virtual double frame_rate( ) const
		{
			return( frame_rate_numerator( ) / frame_rate_denominator( ) );
		}

	protected:
		#pragma region �������[�e�B���e�B�[�֐�
		AVOutputFormat *guess_video_format( const std::string &filename, const std::string &video_type )
		{
			AVOutputFormat *fmt = NULL;

			// �t�@�C���̊g���q����t�H�[�}�b�g�𐄑�����
			if( video_type == "" )
			{
				fmt = guess_format( NULL, filename.c_str( ), NULL );
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
				fmt = guess_format( video_type.c_str( ), NULL, NULL );
				if( fmt == NULL )
				{
					std::cerr << "Could not find specified video format." << std::endl;
					return( NULL );
				}
			}

			return( fmt );
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
		#pragma endregion
	};

	/// @brief mpeg�r�f�I���̓N���X
	//! 
	//! �I�[�f�B�I�X�g���[���͖��T�|�[�g
	//!
	class decoder : public video_io_vase
	{
	private:
		AVFormatContext	*p_fctx_;				///< @brief mpeg�̃t�H�[�}�b�g���C�f�R�[�_��
		AVFrame			*p_frame_src_;			///< @brief �ǂݏo���ꂽ�t���[���摜�o�b�t�@
		AVFrame			*p_frame_rgb_;			///< @brief RGB�t�H�[�}�b�g�̃t���[���摜�o�b�t�@�iarray2�`���̃t���[���摜�𓾂邽�߂̒��ԃf�[�^�j
		bool			is_open_;				///< @brief �r�f�I���J���Ă��邩�ǂ����̃t���O
		bool			is_eof_;				///< @brief �r�f�I���J���Ă��邩�ǂ����̃t���O
		int				video_stream_index_;	///< @brief �r�f�I���w���X�g���[���ԍ�
		SwsContext		*p_swscale_;			///< @brief �f�R�[�h��̃t���[����RGB�̃t���[���ɕϊ�����t�B���^���w���|�C���^

	public:

		/// @brief �R���X�g���N�^
		//! 
		//! �f�t�H���g�R���X�g���N�^
		//! 
		decoder( ) : p_fctx_( NULL ), p_frame_src_( NULL ), p_frame_rgb_( NULL ), is_open_( false ), is_eof_( true ), video_stream_index_( -1 ), p_swscale_( NULL )
		{
			if( !__libavcodec__::is_libavcodec_initialized )
			{
				av_register_all( );
				__libavcodec__::is_libavcodec_initialized = true;
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

		/// @brief ���݂̃t���[���ԍ��𓾂�
		virtual difference_type frame_id( ) const
		{
			if( is_open( ) )
			{
				return( p_fctx_->streams[ video_stream_index_ ]->codec->frame_number );
			}
			else
			{
				return( -1 );
			}
		}

		/// @brief mpeg�t�@�C�����𓾂�
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
		virtual size_type frame_rate_numerator( ) const
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
		
		/// @brief �t���[�����[�g�x�[�X�𓾂�i���ۂ̃t���[�����[�g���t���[�����[�g/�t���[�����[�g�x�[�X�j
		virtual size_type frame_rate_denominator( ) const
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

	public:
		/// @brief mpeg�t�@�C��������͗p�r�f�I�X�g���[�����J��
		//! 
		//! @param[in] filename �c ����mpeg�t�@�C��
		//! 
		virtual bool open( const std::string &filename )
		{
			if( !is_open( ) )
			{
				// �t�@�C���̃w�b�_����ǂݎ���ăr�f�I�t�H�[�}�b�g���擾����
				if( av_open_input_file( &p_fctx_, filename.c_str( ), NULL, 0, NULL ) != 0 )
				{
					printf( "Couldn't open file %s\n", filename );
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
					printf( "Codec not found\n" );
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
				p_frame_rgb_ = allocate_frame( p_cctx->width, p_cctx->height, PIX_FMT_RGB32 );

				// �摜�̕ϊ��p�̃t�B���^��ݒ肷��
				p_swscale_ = sws_getContext( p_cctx->width, p_cctx->height, p_cctx->pix_fmt, p_cctx->width, p_cctx->height, PIX_FMT_RGB32, SWS_FAST_BILINEAR, NULL, NULL, NULL);

				// �t�@�C���̃I�[�v���ɐ��������t���O��ݒ肷��
				is_open_ = true;
				is_eof_  = false;

				// �擪�t���[����ǂݍ���ł���
				this->skip( 1 );

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
		bool skip( const size_type nskips = 1 )
		{
			if( is_open( ) && !this->is_eof( ) )
			{
				AVPacket packet;
				AVCodecContext *p_cctx = p_fctx_->streams[ video_stream_index_ ]->codec;
				int bFinished = 0;

				for( size_type i = 1 ; !is_eof( ) && i <= nskips ; i++ )
				{
					bFinished = 0;
					while( bFinished == 0 && !is_eof( ) )
					{
						is_eof_ = av_read_frame( p_fctx_, &packet ) < 0;

						// ����X�g���[����T��
						if( !is_eof( ) && packet.stream_index == video_stream_index_ )
						{
							// �p�P�b�g���ŃR�[�h����
							avcodec_decode_video( p_cctx, p_frame_src_, &bFinished, packet.data, packet.size );
						}

						av_free_packet( &packet );
					}
				}

				if( !is_eof( ) )
				{
					// �摜�̌`����ϊ�����
					sws_scale( p_swscale_, p_frame_src_->data, p_frame_src_->linesize, 0, p_cctx->height, p_frame_rgb_->data, p_frame_rgb_->linesize );
				}

				return( bFinished != 0 );
			}
			else
			{
				return( false );
			}
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
				for( size_type i = 0 ; i < image.size( ) ; i ++ )
				{
					image[ i ] = pixel_converter::convert_to( p[ 2 ], p[ 1 ], p[ 0 ] );
					p += 4;
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
		//! @param[out] image   �c �t���[���摜���i�[���邽�߂̂Q�����z��
		//! @param[out] frameNo �c �ǂݍ��񂾃t���[���̃t���[���ԍ����i�[���镄���t������
		//! 
		//! @return �X�g���[���̏I���𒴂��ēǂݍ��݂��s�����ꍇ�� false ��Ԃ��D
		//!
		template < class T >
		bool read( array2< T > &image, difference_type &frameNo )
		{
			frameNo = this->frame_id( );
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

		/// @brief �t���[���o�b�t�@�Ɋi�[���ꂽ�摜�̃C���^�[���X����
		//! 
		bool deinterlace( )
		{
			AVCodecContext *p_cctx = p_fctx_->streams[ video_stream_index_ ]->codec;
			return( avpicture_deinterlace( ( AVPicture * )p_frame_src_, ( AVPicture * )p_frame_src_, p_cctx->pix_fmt, p_cctx->width, p_cctx->height) >= 0 );
		}

	public:	// �I�y���[�^�̎���
		decoder & operator ++( )
		{
			this->skip( 1 );
			return( *this );
		}

		const decoder & operator ++( int )
		{
			this->skip( 1 );
			return( *this );
		}

		decoder & operator +=( difference_type skip )
		{
			this->skip( skip );
			return( *this );
		}
	};



	/// @brief mpeg�r�f�I�o�̓N���X
	//! 
	//! �I�[�f�B�I�X�g���[���͖��T�|�[�g
	//!
	class encoder : public video_io_vase
	{
	private:
		AVFormatContext	*p_fctx_;				///< @brief mpeg�̃t�H�[�}�b�g���C�f�R�[�_��
		AVFrame			*p_frame_dst_;			///< @brief �����o�����t���[���摜�o�b�t�@
		AVFrame			*p_frame_rgb_;			///< @brief RGB�t�H�[�}�b�g�̃t���[���摜�o�b�t�@(array2�`���̉摜�𓾂邽�߂̒��ԃf�[�^)
		SwsContext		*p_swscale_;			///< @brief �f�R�[�h��̃t���[����RGB�̃t���[���ɕϊ�����t�B���^���w���|�C���^

		bool			is_open_;				///< @brief �r�f�I���J���Ă��邩�ǂ����̃t���O
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
		#pragma region �R���X�g���N�^�֘A
		/// @brief �R���X�g���N�^
		//! 
		//! @param[in] encode_buf_size �c 4194304�i�f�t�H���g�l�j
		//! @param[in] bit_rate        �c 1150000�i�f�t�H���g�l�j
		//! @param[in] width           �c 320�i�f�t�H���g�l�j
		//! @param[in] height          �c 240�i�f�t�H���g�l�j
		//! @param[in] frame_rate      �c 30000�i�f�t�H���g�l�j
		//! @param[in] frame_rate_base �c 1001�i�f�t�H���g�l�j
		//! @param[in] gop_size        �c 12�i�f�t�H���g�l�j
		//! @param[in] max_b_frames    �c 1�i�f�t�H���g�l�j
		//!
		encoder( const size_t encode_buf_size = 4194304, const size_t bit_rate = 1150000, const int width = 352, const int height = 240, const int frame_rate = 30000, const int frame_rate_base = 1001, const int gop_size = 12, const int max_b_frames = 1 )
			: p_fctx_( NULL ), p_frame_dst_( NULL ), p_frame_rgb_( NULL ), is_open_( false ), encode_buf_size_( encode_buf_size ), bit_rate_( bit_rate ), width_( width ), height_( height ), frame_rate_( frame_rate ), frame_rate_base_( frame_rate_base ), gop_size_( gop_size ), max_b_frames_( max_b_frames )
		{
			if( !__libavcodec__::is_libavcodec_initialized )
			{
				av_register_all( );
				__libavcodec__::is_libavcodec_initialized = true;
			}
		}
		
		/// @brief �f�X�g���N�^
		//! 
		virtual ~encoder( )
		{
			close( );
		}
		#pragma endregion

	public:
		#pragma region �r�f�I�̏�Ԃ��擾����֐��Q
		/// @brief �r�f�I�X�g���[�����J���Ă��邩�ǂ�����Ԃ�
		virtual bool is_open( ) const { return( is_open_ ); }

		/// @brief �r�f�I�X�g���[���̏I���ɗ������ǂ�����Ԃ�
		virtual bool is_eof( ) const { return( false ); }

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

		/// @brief ���݂̃t���[���ԍ��𓾂�
		virtual difference_type frame_id( ) const
		{
			if( is_open( ) )
			{
				return( p_fctx_->streams[ 0 ]->codec->frame_number );
			}
			else
			{
				return( -1 );
			}
		}

		/// @brief mpeg�t�@�C�����𓾂�
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
				return( p_fctx_->streams[ 0 ]->codec->bit_rate );
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
				return( p_fctx_->streams[ 0 ]->codec->width );
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
				return( p_fctx_->streams[ 0 ]->codec->height );
			}
			else
			{
				return( 0 );
			}
		}

		/// @brief �t���[�����[�g�𓾂�
		virtual size_type frame_rate_numerator( ) const
		{
			if( is_open( ) )
			{
				return( p_fctx_->streams[ 0 ]->codec->time_base.den );
			}
			else
			{
				return( 0 );
			}
		}
		
		/// @brief �t���[�����[�g�x�[�X�𓾂�i���ۂ̃t���[�����[�g���t���[�����[�g/�t���[�����[�g�x�[�X�j
		virtual size_type frame_rate_denominator( ) const
		{
			if( is_open( ) )
			{
				return( p_fctx_->streams[ 0 ]->codec->time_base.den );
			}
			else
			{
				return( 0 );
			}
		}
		#pragma endregion

	public:
		#pragma region �r�f�I�X�g���[������֘A
		virtual bool open( const std::string &filename )
		{
			return( open( filename, "" ) );
		}

		/// @brief mpeg�t�@�C������o�͗p�r�f�I�X�g���[�����J��
		//! 
		//! @param[in] filename �c �o��mpeg�t�@�C��
		//! 
		bool open( const std::string &filename, const std::string &video_type )
		{
			if( !is_open_ )
			{
				// �o�̓t�H�[�}�b�g���擾����
				AVOutputFormat	*format = guess_video_format( filename, video_type );
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
				_snprintf( p_fctx_->filename, sizeof( p_fctx_->filename ), "%s", filename.c_str( ) );
				//av_strlcpy( p_fctx_->filename, filename.c_str( ), sizeof( p_fctx_->filename ) );

				AVStream *stream = NULL;
				if( p_fctx_->oformat->video_codec != CODEC_ID_NONE )
				{
					stream = av_new_stream( p_fctx_, 0 );
					if( stream == NULL )
					{
						std::cerr << "Could not allocate encode stream" << std::endl;
						return( false );
					}

					// �R�[�f�b�N�𐄑�����
					CodecID codec_id = av_guess_codec( p_fctx_->oformat, NULL, p_fctx_->filename, NULL, CODEC_TYPE_VIDEO );

					// �R�[�f�b�N��T��
					AVCodec *codec = avcodec_find_encoder( codec_id );
					if( codec == NULL )
					{
						std::cerr << "Could not find appropriate Codec from the database." << std::endl;
						return( false );
					}

					// �r�f�I�X�g���[���ɃR�[�f�b�N�����w�肷��
					AVCodecContext *cctx = stream->codec;
					cctx->codec_id   = codec->id;
					cctx->codec_type = CODEC_TYPE_VIDEO;
					cctx->width      = 320;
					cctx->height     = 240;
					cctx->pix_fmt    = PIX_FMT_YUV420P;

					// �s�N�Z���t�H�[�}�b�g�̑Ή��󋵂𒲂ׂ�
					if( codec != NULL && codec->pix_fmts )
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
					cctx->bit_rate = 400000;

					// �摜�̃T�C�Y��ݒ�
					cctx->width = 320;
					cctx->height = 240;

					// �t���[�����[�g��ݒ�
					cctx->time_base.den = 2;
					cctx->time_base.num = 1;
					//cctx->time_base.den = 3000;
					//cctx->time_base.num = 1001;
					cctx->gop_size = 12;

					if( cctx->codec_id == CODEC_ID_MPEG2VIDEO )
					{
						cctx->max_b_frames = 2;
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

					// �R�[�f�b�N���J��
					if( avcodec_open( cctx, codec ) < 0 )
					{
						std::cerr << "Could not open codec." << std::endl;
						return( false );
					}
				}

				// �p�����[�^��ݒ肷��
				if( av_set_parameters( p_fctx_, NULL ) < 0 ) 
				{
					std::cerr << "Invalid output format parameters" << std::endl;
					return( false );
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

				// �w�b�_������������
				av_write_header( p_fctx_ );

				AVCodecContext *cctx = stream->codec;
				encode_buf_ = NULL;
				if( ( p_fctx_->oformat->flags & AVFMT_RAWPICTURE ) == 0 )
				{
					encode_buf_ = ( uint8_t * )av_malloc( encode_buf_size_ );
				}

				// �G���R�[�h�p�̃t���[���o�b�t�@��p�ӂ���
				p_frame_dst_ = allocate_frame( cctx->width, cctx->height, cctx->pix_fmt );
				if( p_frame_dst_ == NULL )
				{
					std::cerr << "Could not allocate frame buffer." << std::endl;
					return( false );
				}

				// �ꎞ�̈�p�̃t���[���o�b�t�@��p�ӂ���
				p_frame_rgb_ = allocate_frame( cctx->width, cctx->height, PIX_FMT_RGB32 );
				if( p_frame_dst_ == NULL )
				{
					std::cerr << "Could not allocate temporal frame buffer." << std::endl;
					return( false );
				}

				// �摜�̕ϊ��p�̃t�B���^��ݒ肷��
				p_swscale_ = sws_getContext( cctx->width, cctx->height, PIX_FMT_RGB32, cctx->width, cctx->height, cctx->pix_fmt, SWS_BILINEAR, NULL, NULL, NULL);

				is_open_ = true;

				return( true );
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

				// codec�����
				avcodec_close( p_fctx_->streams[ 0 ]->codec );

				// �G���R�[�h�Ɏg�p�����o�b�t�@�����
				if( encode_buf_ != NULL )
				{
					av_freep( &encode_buf_ );
				}

				// ���ׂẴX�g���[�����������
				for( size_type i = 0 ; i < p_fctx_->nb_streams ; i++ )
				{
					av_freep( &( p_fctx_->streams[ i ] ->codec ) );
					av_freep( &( p_fctx_->streams[ i ] ) );
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
		#pragma endregion

	public:
		#pragma region �t���[���̏����o��
		/// @brief array2�`���̉摜���t���[���o�b�t�@�ɏ������݁C�G���R�[�h���ăX�g���[���ɏo�͂���
		template < class T, class Allocator >
		bool write( const array2< T, Allocator > &image )
		{
			if( is_open( ) )
			{
				int ret = 0;

				AVCodecContext *p_cctx = p_fctx_->streams[ 0 ]->codec;

				unsigned char *p = p_frame_rgb_->data[ 0 ];
				size_type size = p_cctx->width * p_cctx->height;
				size = size < image.size( ) ? size : image.size( );
				for( size_t i = 0 ; i < size ; i++ )
				{
					p[ 0 ] = image[ i ].b;
					p[ 1 ] = image[ i ].g;
					p[ 2 ] = image[ i ].r;
					p[ 3 ] = 255;
					p += 4;
				}

				// �摜�̌`����ϊ�����
				sws_scale( p_swscale_, p_frame_rgb_->data, p_frame_rgb_->linesize, 0, p_cctx->height, p_frame_dst_->data, p_frame_dst_->linesize );

				if( ( p_fctx_->oformat->flags & AVFMT_RAWPICTURE ) != 0 )
				{
					// RAW Video
					AVPacket packet;
					av_init_packet( &packet );

					packet.flags |= PKT_FLAG_KEY;
					packet.stream_index = p_fctx_->streams[ 0 ]->index;
					packet.data = ( uint8_t * )p_frame_dst_;
					packet.size = sizeof( AVPicture );

					ret = av_write_frame( p_fctx_, &packet );
				}
				else
				{
					// �摜���G���R�[�h����
					AVStream  *stream = p_fctx_->streams[ 0 ];
					AVCodecContext *c = stream->codec;
					int out_size = avcodec_encode_video( c, encode_buf_, encode_buf_size_, p_frame_dst_ );

					if( out_size > 0 )
					{
						AVPacket packet;
						av_init_packet( &packet );

						packet.stream_index = stream->index;
						packet.data         = encode_buf_;
						packet.size         = out_size;

						if( c->coded_frame->pts != 0x8000000000000000 )
						{
							packet.pts = av_rescale_q( c->coded_frame->pts, c->time_base, stream->time_base );
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

		/// @brief �t���[���o�b�t�@�Ɋi�[���ꂽ�摜�̃C���^�[���X����
		bool deinterlace( )
		{
			AVCodecContext *p_cctx = p_fctx_->streams[ 0 ]->codec;
			return avpicture_deinterlace( ( AVPicture * )p_frame_dst_, ( AVPicture * )p_frame_dst_, p_cctx->pix_fmt, p_cctx->width, p_cctx->height) >= 0;
		}
		#pragma endregion
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
			//dst.gop_size(        src.gop_size( ) );
			//dst.max_b_frames(    src.max_b_frames( ) );
			return true;
		}
		return false;
	}

} // namespace mpeg


/// @}
//  mpeg1, mpeg2�`���r�f�I�X�g���[�����o�͂̂��߂̃��C�u�����̏I���


// mist���O��Ԃ̏I���
_MIST_END



#endif // __INCLUDE_IO_MPEG_H__
