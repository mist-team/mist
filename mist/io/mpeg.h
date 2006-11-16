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
//! @brief mpeg1, mpeg2形式ビデオ外リーム入出力
//!

#ifndef __INCLUDE_IO_MPEG_H__
#define __INCLUDE_IO_MPEG_H__



#include <avcodec.h>
#include <avformat.h>

#include <mist/mist.h>
#include <mist/config/color.h>
#include <iostream>



// mist名前空間の始まり
_MIST_BEGIN



namespace __libavcodec__
{
	static bool is_libavcodec_initialized = false;		///< @brief libavcodecの初期化フラグ
}



//! @defgroup mpeg_group mpeg1, mpeg2形式ビデオストリーム入出力のためのライブラリ
//!
//! 本ライブラリは，http://ffmpeg.sourceforge.net/index.php で開発が行われている ffmpeg-0.4.8 の libavcodec, libavformat を利用している
//! エンコードとデコードはこのライブラリのAPIを用いて実装されているため，別途ライブラリを用意する必要がある
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/io/mpeg.h>
//! @endcode
//!
//! @code 使用例(mpegのビデオストリームから連番bmpファイルを作成)
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
//! @code 使用例(連番bmpからmpeg1videoストリームを作成)
//! mist::array2< mist::rgb< unsigned char > > img;
//! mist::mpeg::output_video ov;
//! // パラメータの変更（【注意！】ストリームを開く前に変更しなければならない）
//! // ov.encode_buf_size( 524288 );	// 十分に確保する
//! // ov.bit_rate( 1000000 );			// 少ないと品質が落ちる
//! ov.width( 320 );					// 画像のサイズに合わせて変更
//! ov.height( 240 );					// 画像のサイズに合わせて変更
//! // ov.frame_rate( 30 );				// フレームレート
//! // ov.frame_rate_base( 1 );			// 
//! // ov.gop_size( 12 );				// 多いと品質が落ちる
//! // ov.max_b_frames( 2 );			// 多いと品質が落ちる
//! mist::mpeg::open( ov, "output.mpeg" );
//! mist::mpeg::dump_format( ov );
//! char f_name[ 256 ];
//! const size_t frame_num = 100;       // 連番bmpファイルの数
//! for( size_t i = 0 ; i < frame_num ; i ++ )
//! {
//!		sprintf( f_name, "tmp/frame%04d.bmp", i );
//!		mist::read_bmp( img, f_name );
//!		mist::mpeg::write_frame( ov, img );
//! }
//! mist::mpeg::close( ov );
//! @endcode
//!
//! @code 使用例(ストリーム中の各フレームの画像処理フィルタ処理)
//! mist::array2< mist::rgb< unsigned char > > in_img, out_img;
//! mist::mpeg::input_video iv;
//! mist::mpeg::output_video ov;
//! mist::mpeg::open( iv, "input.mpeg" );
//! mist::mpeg::copy_format( iv, ov );
//! // 必要ならovのパラメータの変更（解像度変換するような場合にはフレームの幅と高さを変更）
//! mist::mpeg::open( ov, "output.mpeg" );
//! mist::mpeg::dump_format( iv );
//! mist::mpeg::dump_format( ov );
//! while( mist::mpeg::read_frame( iv, in_img ) )
//! {
//!		// in_imgを入力してout_imgを出力するようなフィルタ
//!		mist::gaussian( in_img, out_img ); // 例
//!		mist::mpeg::write_frame( ov, out_img );
//! }
//! mist::mpeg::close( iv );
//! mist::mpeg::close( ov );
//! @endcode
//!
//! //! @code 使用例(mpeg中の任意のフレームからmpeg1videoストリームを作成)
//! mist::mpeg::input_video iv;
//! mist::mpeg::output_video ov; 
//! mist::mpeg::open( iv, "input.mpeg" );
//! mist::mpeg::copy_format( iv, ov );
//! // 必要ならovのパラメータの変更
//! mist::mpeg::open( ov, "output.mpeg" );
//! mist::mpeg::dump_format( iv );
//! mist::mpeg::dump_format( ov );
//! mist::mpeg::skip_frame( iv, 60 );      // いらないフレームはスキップ
//! mist::mpeg::copy_frame( iv, ov, 240 ); // 必要なフレームのみコピー
//! mist::mpeg::close( iv );
//! mist::mpeg::close( ov );
//! @endcode
//!
//!
//!  @{



/// @brief mpeg1,mpeg2ファイルからビデオストリームの読み込み，mpeg1videoビデオストリームのmpegファイル書き出し
namespace mpeg
{

	/// @brief 異なるストリーム間でフレームバッファのやり取りを行うためのクラス
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
	

	/// @brief mpegビデオ入力クラス
	//! 
	//! オーディオストリームは未サポート
	//!
	class input_video
	{

	private:

		AVFormatContext	*p_fctx_;				///< @brief mpegのフォーマット情報，デコーダ等
		AVFrame			*p_frame_src_;			///< @brief 読み出されたフレーム画像バッファ
		AVFrame			*p_frame_rgb_;			///< @brief RGBフォーマットのフレーム画像バッファ（array2形式のフレーム画像を得るための中間データ）
		bool			is_open_;				///< @brief ビデオが開いているかどうかのフラグ
		int				video_stream_index_;	///< @brief ビデオを指すストリーム番号
		AVPacket		decoding_packet_;		///< @brief デコード中のパケット
		int				bytes_not_decoded_;		///< @brief まだデコードされていないデータのバイト数
		uint8_t			*p_decoding_packet_;	///< @brief デコード中のパケット内の位置を指すポインタ

	public:

		/// @brief コンストラクタ
		//! 
		//! デフォルトコンストラクタ
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

		/// @brief デストラクタ
		//! 
		~input_video( )
		{ 
			close( );
		}

		/// @brief mpegファイルから入力用ビデオストリームを開く
		//! 
		//! @param[in] filename … 入力mpegファイル
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

		/// @brief ビデオストリームを閉じる
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

		/// @brief ビデオストリームが開いているかどうかを返す
		//! 
		bool is_open( ) const
		{
			return is_open_;
		}

		/// @brief ストリームのフォーマットを標準出力にダンプする
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

		/// @brief 現在のフレームバッファを得る
		//! 
		frame_t frame( ) const
		{
			if( is_open_ )
			{
				return frame_t( p_frame_src_, p_fctx_->streams[ video_stream_index_ ]->codec.pix_fmt );
			}
			return frame_t( );
		}

		/// @brief 現在のフレーム番号を得る
		//! 
		int frame_number( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.frame_number;
			}
			return -1;
		}

		/// @brief 現在のフレームがキーフレームなら真を返す
		//! 
		bool is_key_frame( ) const
		{
			if( is_open_ )
			{
				return p_frame_src_->key_frame == 1;
			}
			return false;
		}

		/// @brief 指定した回数だけフレームスキップ
		//! 
		//! @param[in] num … スキップ枚数
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

		/// @brief デコードされたフレームバッファをarray2形式に変換して格納(RGB)
		//! 
		//! @param[out] image … フレーム画像を格納するための２次元配列
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

		/// @brief デコードされたフレーム画像をarray2形式に変換して格納(グレイスケール)
		//! 
		//! @param[out] image … フレーム画像を格納するための２次元配列
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

		/// @brief フレーム読み込んで，デコードしてフレームバッファに格納
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

		/// @brief フレームバッファに格納された画像のインターレス除去
		//! 
		bool deinterlace( )
		{
			AVCodecContext *p_cctx = &p_fctx_->streams[ video_stream_index_ ]->codec;
			return avpicture_deinterlace( ( AVPicture * )p_frame_src_, ( AVPicture * )p_frame_src_, p_cctx->pix_fmt, p_cctx->width, p_cctx->height) >= 0;
		}

		/// @brief mpegファイル名を得る
		//! 
		char *filename( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->filename;
			}
			return NULL;
		}
	    
		/// @brief ビットレートを得る
		//! 
		int bit_rate( )  const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.bit_rate;
			}
			return 0;
		}
		
		/// @brief フレームの幅を得る
		//! 
		int width( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.width;
			}
			return 0;
		}
		
		/// @brief フレームの高さを得る
		//! 
		int height( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.height;
			}
			return 0;
		}
		
		/// @brief フレームレートを得る
		//! 
		int frame_rate( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.frame_rate;
			}
			return 0;
		}
		
		/// @brief フレームレートベースを得る（実際のフレームレート＝フレームレート/フレームレートベース）
		//! 
		int frame_rate_base( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.frame_rate_base;
			}
			return 0;
		}
		
		/// @brief GOPサイズを得る（この枚数の連続フレーム中に必ず１枚以上Iフレームが存在する）
		//! 
		int gop_size( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.gop_size;
			}
			return 0;
		}
		
		/// @brief 最大連続Bフレーム数を得る
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



	/// @brief mpegビデオ出力クラス
	//! 
	//! オーディオストリームは未サポート
	//!
	class output_video
	{

	private:

		AVFormatContext	*p_fctx_;				///< @brief mpegのフォーマット情報，デコーダ等
		AVFrame			*p_frame_dst_;			///< @brief 書き出されるフレーム画像バッファ
		AVFrame			*p_frame_rgb_;			///< @brief RGBフォーマットのフレーム画像バッファ(array2形式の画像を得るための中間データ)
		bool			is_open_;				///< @brief ビデオが開いているかどうかのフラグ
		const int		video_stream_index_;	///< @brief ビデオを指すストリーム番号
		int				encode_buf_size_;		///< @brief エンコードバッファのサイズ
		uint8_t			*encode_buf_;			///< @brief エンコードバッファ
		int				bit_rate_;				///< @brief ビットレート
		int				width_;					///< @brief フレーム画像の幅
		int				height_;				///< @brief フレーム画像の高さ
		int				frame_rate_;			///< @brief フレームレート
		int				frame_rate_base_;		///< @brief フレームレートベース（実際のフレームレート＝フレームレート/フレームレートベース）
		int				gop_size_;				///< @brief GOPサイズ（この枚数の連続フレーム中に必ず１枚以上Iフレームが存在する）
		int				max_b_frames_;			///< @brief 最大連続Bフレーム数

	public:

		/// @brief コンストラクタ
		//! 
		//! @param[in] encode_buf_size … 4194304（デフォルト値）
		//! @param[in] bit_rate        … 1048576（デフォルト値）
		//! @param[in] width           … 160（デフォルト値）
		//! @param[in] height          … 120（デフォルト値）
		//! @param[in] frame_rate      … 30（デフォルト値）
		//! @param[in] frame_rate_base … 1（デフォルト値）
		//! @param[in] gop_size        … 12（デフォルト値）
		//! @param[in] max_b_frames    … 2（デフォルト値）
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
		
		/// @brief デストラクタ
		//! 
		~output_video( )
		{
			close( );
		}

		/// @brief mpegファイルから出力用ビデオストリームを開く
		//! 
		//! @param[in] filename … 出力mpegファイル
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

		/// @brief ビデオストリームを閉じる
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

		/// @brief ビデオが開いているかどうかを返す
		//! 
		bool is_open( ) const
		{
			return is_open_;
		}

		/// @brief ストリームのフォーマットを標準出力にダンプする
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

		/// @brief 現在のフレームバッファを得る
		//! 
		frame_t frame( ) const
		{
			if( is_open_ )
			{
				return frame_t( p_fctx_->streams[ video_stream_index_ ]->codec.coded_frame, p_fctx_->streams[ video_stream_index_ ]->codec.pix_fmt );
			}
			return frame_t( );
		}

		/// @brief 現在のフレーム番号を得る
		//! 
		int frame_number( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.coded_frame->coded_picture_number;//.frame_number;
			}
			return -1;
		}

		/// @brief 現在のフレームがキーフレームであれば、真を返す
		//! 
		bool is_key_frame( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.coded_frame->key_frame == 1;
			}
			return false;
		}

		/// @brief array2形式の画像をフレームバッファに書き込む(RGB)
		//! 
		//! @param[in] image … フレーム画像
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

		/// @brief array2形式の画像をフレームバッファに書き込む(グレイスケール)
		//! 
		//! @param[in] image … フレーム画像
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

		/// @brief frame_t型のフレーム画像をフレームバッファに書き込む
		//! 
		//! @param[in] frame … frame_t型のフレーム画像
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

		/// @brief フレームバッファの内容をエンコードしてストリームに書き込む
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

		/// @brief フレームバッファに格納された画像のインターレス除去
		bool deinterlace( )
		{
			AVCodecContext *p_cctx = &p_fctx_->streams[ video_stream_index_ ]->codec;
			return avpicture_deinterlace( ( AVPicture * )p_frame_dst_, ( AVPicture * )p_frame_dst_, p_cctx->pix_fmt, p_cctx->width, p_cctx->height) >= 0;
		}

		/// @brief エンコードバッファサイズを設定
		//! 
		//! @param[in] encode_buf_size … エンコードバッファサイズ
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

		/// @brief ビットレートを設定
		//! 
		//! @param[in] bit_rate … ビットレート
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

		/// @brief フレーム画像の幅を設定(２の倍数)
		//! 
		//! @param[in] width … フレーム画像の幅
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

		/// @brief フレーム画像の高さを設定(２の倍数)
		//! 
		//! @param[in] height … フレーム画像の高さ
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

		/// @brief フレームレートを設定
		//! 
		//! @param[in] frame_rate … フレームレーt
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

		/// @brief フレームレートベースを設定（実際のフレームレート＝フレームレート/フレームレートベース）
		//! 
		//! @param[in] frame_rate_base … フレームレートベース
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

		/// @brief GOPサイズを設定（この枚数の連続フレーム中に必ず１枚以上Iフレームが存在する）
		//! 
		//! @param[in] gop_size … GOPサイズ
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

		/// @brief 最大連続Bフレーム数を設定
		//! 
		//! @param[in] max_b_frames … 最大連続Bフレーム数
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

		/// @brief mpegファイル名を得る
		//! 
		char *filename( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->filename;
			}
			return NULL;
		}

		/// @brief ビットレートを得る
		//! 
		int bit_rate( )  const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.bit_rate;
			}
			return 0;
		}

		/// @brief フレーム画像の幅を得る
		//! 
		int width( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.width;
			}
			return 0;
		}

		/// @brief フレーム画像の高さを得る
		//! 
		int height( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.height;
			}
			return 0;
		}

		/// @brief フレームレートを得る
		//! 
		int frame_rate( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.frame_rate;
			}
			return 0;
		}

		/// @brief フレームレートベースを得る（実際のフレームレート＝フレームレート/フレームレートベース）
		//! 
		int frame_rate_base( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.frame_rate_base;
			}
			return 0;
		}

		/// @brief GOPサイズを得る（この枚数の連続フレーム中に必ず１枚以上Iフレームが存在する）
		//! 
		int gop_size( ) const
		{
			if( is_open_ )
			{
				return p_fctx_->streams[ video_stream_index_ ]->codec.gop_size;
			}
			return 0;
		}

		/// @brief 最大連続Bフレーム数を得る
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



	/// @brief ファイルからストリームを開く
	//! 
	//! @param[in,out] av              … ストリーム
	//! @param[in] filename             … ファイル名
	//!
	template< typename Stream >
	inline bool open( Stream &av, const char *filename )
	{
		return av.open( filename );
	}

	/// @brief ストリームを閉じる
	//! 
	//! @param[in,out]  av              … ストリーム
	//!
	template< typename Stream >
	inline bool close( Stream &av )
	{
		return av.close( );
	}

	/// @brief ストリームが開いているかどうかを返す
	//! 
	//! @param[in]  av              … ストリーム
	//!
	template< typename Stream >
	inline bool is_open( const Stream &av )
	{
		return av.is_open( );
	}

	/// @brief ストリームのフォーマットを標準出力にダンプする
	//! 
	//! @param[in]  av              … ストリーム
	//!
	template< typename Stream >
	inline bool dump_format( const Stream &av )
	{
		return av.dump_format( );
	}

	/// @brief ビデオストリームからフレームを取り出してarray2形式の画像に格納
	//! 
	//! @param[in,out] video           … ビデオストリーム
	//! @param[out]    image           … フレーム画像
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

	/// @brief array2形式の画像をビデオストリームに書き込む
	//! 
	//! @param[in,out] video           … ビデオストリーム
	//! @param[in]     image           … フレーム画像
	//!
	template< typename Stream, typename Image >
	inline bool write_frame( Stream &video, const Image &image )
	{
		video.put( image );
		return video.write( );
	}

	/// @brief 指定した回数だけフレームスキップ
	//! 
	//! @param[in,out] video         … ビデオストリーム
	//! @param[in]     num           … スキップ回数
	//!
	template< typename Stream >
	inline bool skip_frame( Stream &video, const size_t num = 1 )
	{
		return video.skip( num );
	}

	/// @brief 指定した回数だけビデオストリーム間でフレーム画像のコピー
	//! 
	//! @param[in,out] src         … コピー元ビデオストリーム
	//! @param[out]    dst         … コピー先ビデオストリーム
	//! @param[in]     num         … コピー回数
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

	/// @brief ビデオストリーム間でフォーマット情報のコピー
	//! 
	//! @param[in,out] src         … コピー元ビデオストリーム
	//! @param[out]    dst         … コピー先ビデオストリーム
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



// mist名前空間の終わり
_MIST_END



#endif // __INCLUDE_IO_MPEG_H__
