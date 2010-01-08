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
//! @brief さまざまなビデオストリームの入出力ライブラリ
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


// mist名前空間の始まり
_MIST_BEGIN



//! @defgroup video_group ビデオファイルの入出力を行うライブラリ
//!
//! 本ライブラリは，http://ffmpeg.sourceforge.net/index.php で開発が行われている ffmpeg の libavcodec, libavformat を利用している
//! エンコードとデコードはこのライブラリのAPIを用いて実装されているため，別途ライブラリを用意する必要がある
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/io/video.h>
//! @endcode
//!
//! @code 使用例(ビデオストリームから連番bmpファイルを作成)
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
//! @code 使用例(入力ビデオをmpeg1ビデオに変換)
//! mist::array2< mist::rgb< unsigned char > > img;
//! mist::video::decoder iv( "input.avi" );
//! 
//! // コンストラクタの引数を変更して圧縮パラメータの変更
//! // （【注意！】ストリームを開く前に変更しなければならない）
//! mist::video::mpeg1::encoder ov( "output.mpg", 320, 240 );
//! ov.dump( );
//! 
//! while( !iv.is_eof( ) )
//! {
//!     std::cout << iv.time( ) << std::endl;
//!     iv >> img;
//!     /* ここで img に何らかの画像処理を施すことも可能 */
//!     ov << img;
//! }
//! 
//! ov.close( );
//! iv.close( );
//! @endcode
//!
//!  @{


/// @brief ビデオファイルを扱うライブラリ
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
		virtual bool is_open( ) const = 0;							///< @brief ビデオストリームが開いているかどうかを返す
		virtual bool is_eof( ) const = 0;							///< @brief ビデオストリームの終わりに来たかどうかを返す
		virtual bool dump( ) const = 0;								///< @brief ストリームのフォーマットを標準出力にダンプする
		virtual long double time( ) const = 0;						///< @brief 現在のビデオストリーム上での再生位置を表す秒数
		virtual long double duration( ) const = 0;					///< @brief ビデオストリームの総秒数を返す
		virtual const std::string filename( ) const = 0;			///< @brief ファイル名を返す
		virtual size_type bit_rate( ) const = 0;					///< @brief ビットレートを返す
		virtual size_type width( ) const = 0;						///< @brief フレームの幅を返す
		virtual size_type height( ) const = 0;						///< @brief フレームの高さを返す
		virtual long double frame_rate_numerator( ) const = 0;		///< @brief フレームレート（A÷B）の分子部分のAを返す
		virtual long double frame_rate_denominator( ) const = 0;	///< @brief フレームレート（A÷B）の分母部分のBを返す
		virtual long double frame_aspect_ratio( ) const = 0;		///< @brief フレームのアスペクト比を得る（ウィンドウ幅/ウィンドウ高さ）

		/// @brief フレームレートを返す
		virtual long double frame_rate( ) const
		{
			return( frame_rate_denominator( ) / frame_rate_numerator( ) );
		}

		/// @brief 1フレームあたりの時間を返す
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
				// ファイルの拡張子からフォーマットを推測する
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
			// 処理用のフレームバッファを用意する
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

	/// @brief ビデオ入力クラス
	//! 
	//! オーディオストリームは未サポート
	//!
	class decoder : public video_io_vase
	{
	private:
		AVFormatContext	*p_fctx_;				///< @brief ビデオフォーマット情報，デコーダ等
		AVFrame			*p_frame_src_;			///< @brief 読み出されたフレーム画像バッファ
		AVFrame			*p_frame_rgb_;			///< @brief RGBフォーマットのフレーム画像バッファ（array2形式のフレーム画像を得るための中間データ）
		bool			is_open_;				///< @brief ビデオが開いているかどうかのフラグ
		bool			is_eof_;				///< @brief ビデオが開いているかどうかのフラグ
		int				video_stream_index_;	///< @brief ビデオを指すストリーム番号
		SwsContext		*p_swscale_;			///< @brief デコード後のフレームをRGBのフレームに変換するフィルタを指すポインタ
		int64_t			frame_pts_;				///< @brief ビデオストリーム中での現在のフレーム位置を保持する変数

	public:
		/// @brief コンストラクタ
		//! 
		//! デフォルトコンストラクタ
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

		/// @brief コンストラクタ
		//! 
		//! デフォルトコンストラクタ
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

		/// @brief デストラクタ
		//! 
		virtual ~decoder( )
		{ 
			close( );
		}


	public:
		/// @brief ビデオストリームが開いているかどうかを返す
		virtual bool is_open( ) const { return( is_open_ ); }

		/// @brief ビデオストリームの終わりに来たかどうかを返す
		virtual bool is_eof( ) const { return( is_eof_ ); }

		/// @brief ストリームのフォーマットを標準出力にダンプする
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

		/// @brief ストリームのフォーマットを標準出力にダンプする
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

		/// @brief 現在のビデオストリーム上での再生位置を表す秒数
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

		/// @brief 現在のビデオストリーム上での再生位置を表す秒数
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

		/// @brief ビデオストリームの総秒数を返す
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

		/// @brief 総フレーム数を得る
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

		/// @brief ファイル名を得る
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
	    
		/// @brief ビットレートを得る
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
		
		/// @brief フレームの幅を得る
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
		
		/// @brief フレームの高さを得る
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

		/// @brief フレームレートを得る
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
		
		/// @brief フレームレートベースを得る（実際のフレームレート＝フレームレート/フレームレートベース）
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

		/// @brief フレームのアスペクト比を得る（ウィンドウ幅/ウィンドウ高さ）
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
		/// @brief ファイルから入力用ビデオストリームを開く
		//! 
		//! @param[in] filename … 入力ファイル名
		//! 
		virtual bool open( const std::string &filename )
		{
			if( !is_open( ) )
			{
				// ファイルのヘッダ情報を読み取ってビデオフォーマットを取得する
				if( av_open_input_file( &p_fctx_, filename.c_str( ), NULL, 0, NULL ) != 0 )
				{
					printf( "Couldn't open file %s\n", filename.c_str( ) );
					return( false );
				}

				// ファイルからストリーム情報を取得する
				if( av_find_stream_info( p_fctx_ ) < 0 )
				{
					printf( "Couldn't find stream information\n" );
					return( false );
				}

				// ビデオストリームを探す
				video_stream_index_ = -1;
				for( unsigned int i = 0 ; i < p_fctx_->nb_streams ; i++ )
				{
					if( p_fctx_->streams[ i ]->codec->codec_type == CODEC_TYPE_VIDEO )
					{
						video_stream_index_ = i;
						break;
					}
				}

				// ビデオストリームが見つからなかったので終了する
				if( video_stream_index_ == -1 )
				{
					printf( "Didn't find a video stream\n" );
					return( false );
				}

				AVCodecContext *p_cctx = p_fctx_->streams[ video_stream_index_ ]->codec;

				// 対応するコーデックを取得する
				AVCodec *p_codec = avcodec_find_decoder( p_cctx->codec_id );
				if( p_codec == NULL )
				{
					printf( "Codec %d not found\n", p_cctx->codec_id );
					return( false );
				}

				// コーデックを開く
				if( avcodec_open( p_cctx, p_codec) < 0 )
				{
					printf( "Could not open codec\n" );
					return( false );
				}

				// フレームを管理するオブジェクトを作成する
				p_frame_src_ = avcodec_alloc_frame( );
				p_frame_rgb_ = allocate_frame( p_cctx->width, p_cctx->height, PIX_FMT_BGR24 );

				// 画像の変換用のフィルタを設定する
				p_swscale_ = sws_getContext( p_cctx->width, p_cctx->height, p_cctx->pix_fmt, p_cctx->width, p_cctx->height, PIX_FMT_BGR24, SWS_FAST_BILINEAR, NULL, NULL, NULL);

				// ファイルのオープンに成功したフラグを設定する
				is_open_ = true;
				is_eof_  = false;

				// 先頭フレームを読み込んでおく
				this->skip( 1 );

				// 先頭をうまく読めない場合は再度先頭に戻す
				if( frame_pts_ != 0 )
				{
					this->seek_frame( 0 );
				}

				return( true );
			}

			return( false );
		}

		/// @brief ビデオストリームを閉じる
		//! 
		virtual bool close( )
		{
			if( is_open( ) )
			{
				// フィルタを解放する
				sws_freeContext( p_swscale_ );

				// コーデックを閉じる
				avcodec_close( p_fctx_->streams[ video_stream_index_ ]->codec );

				// ファイルを閉じる
				av_close_input_file( p_fctx_ );

				// 一時フレーム領域を解放する
				av_free( p_frame_src_);

				// 画像変換用フレーム領域を解放する
				free_frame( &p_frame_rgb_ );

				is_open_ = false;

				return( true );
			}

			return( false );
		}

		/// @brief 指定した回数だけフレームスキップ
		//! 
		//! @param[in] nskips … スキップ枚数
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

		/// @brief 指定した時刻をビデオストリームから探す
		//! 
		//! @param[in] tm … ビデオストリーム上での位置を表す秒数
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

		/// @brief 指定したフレーム番号をビデオストリームから探す
		//! 
		//! @param[in] fno … ビデオストリーム上での位置を表すフレーム番号
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

		/// @brief 現在のフレームバッファをarray2形式に変換して格納する
		//! 
		//! @param[out] image … フレーム画像を格納するための２次元配列
		//!
		template < class T, class Allocator >
		bool read_buffer( array2< T, Allocator > &image )
		{
			typedef _pixel_converter_< T > pixel_converter;
			typedef typename pixel_converter::color_type color_type;

			if( is_open( ) && !this->is_eof( ) )
			{
				AVCodecContext *p_cctx = p_fctx_->streams[ video_stream_index_ ]->codec;

				// 画像のサイズを調整する
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

		/// @brief 現在のフレームバッファをarray2形式に変換して格納し，フレーム番号を1進める
		//! 
		//! @param[out] image … フレーム画像を格納するための２次元配列
		//! 
		//! @return ストリームの終わりを超えて読み込みを行った場合は false を返す．
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

		/// @brief 現在のフレームバッファをarray2形式に変換して格納し，フレーム番号を1進める
		//! 
		//! @param[out] image      … フレーム画像を格納するための２次元配列
		//! @param[out] frame_time … 読み込んだフレームのビデオストリーム上での時刻を格納する変数
		//! 
		//! @return ストリームの終わりを超えて読み込みを行った場合は false を返す．
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
		/// @brief 指定した回数もしくは指定した位置に来るまでフレームをデコードする
		//! 
		//! @param[in] ntimes       … スキップ枚数
		//! @param[in] pts          … 終了位置
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

							// 動画ストリームを探す
							if( packet.stream_index == video_stream_index_ )
							{
								frame_pts_ = av_rescale_( AV_TIME_BASE, packet.pts < 0 ? packet.dts : packet.pts, ( int64_t ) stream->time_base.num, stream->time_base.den );

								if( !is_eof( ) )
								{
									// パケットをでコードする
									avcodec_get_frame_defaults( p_frame_src_ );
									avcodec_decode_video( p_cctx, p_frame_src_, &bFinished, packet.data, packet.size );
								}
							}

							if( bFinished != 0 && ( i == ntimes || frame_pts_ >= pts ) )
							{
								// 一時バッファにデータをコピーする
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

							// 動画ストリームを探す
							if( packet.stream_index == video_stream_index_ )
							{
								frame_pts_ = av_rescale_( AV_TIME_BASE, packet.pts < 0 ? packet.dts : packet.pts, ( int64_t ) stream->time_base.num, stream->time_base.den );

								if( !is_eof( ) )
								{
									// パケットをでコードする
									avcodec_get_frame_defaults( p_frame_src_ );
									avcodec_decode_video( p_cctx, p_frame_src_, &bFinished, packet.data, packet.size );
								}
							}

							if( bFinished != 0 && frame_pts_ >= pts )
							{
								// 一時バッファにデータをコピーする
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

	public:	// オペレータの実装
		/// @brief 次のフレームへ進む
		decoder & operator ++( )
		{
			this->skip( 1 );
			return( *this );
		}

		/// @brief 一つ前のフレームへ戻る
		decoder & operator --( )
		{
			this->seek_frame( this->frame_id( ) - 1 );
			//this->seek( this->time( ) - this->seconds_per_frame( ) );
			return( *this );
		}

		/// @brief ビデオストリームの位置を指定したフレーム数だけ進める
		//! 
		//! @param[in] fno … ストリームの位置を進めるフレーム数
		//!
		decoder & operator +=( difference_type fno )
		{
			this->skip( fno );
			return( *this );
		}

		/// @brief ビデオストリームの位置を指定したフレーム数だけ戻す
		//! 
		//! @param[in] fno … ストリームの位置を進めるフレーム数
		//!
		decoder & operator -=( difference_type fno )
		{
			this->seek_frame( this->frame_id( ) - fno );
			return( *this );
		}
	};



	/// @brief ビデオ出力クラス
	//! 
	//! オーディオストリームは未サポート
	//!
	class encoder : public video_io_vase
	{
	protected:
		AVFormatContext	*p_fctx_;				///< @brief フォーマット情報，デコーダ等
		AVFrame			*p_frame_dst_;			///< @brief 書き出されるフレーム画像バッファ
		AVFrame			*p_frame_rgb_;			///< @brief RGBフォーマットのフレーム画像バッファ(array2形式の画像を得るための中間データ)
		SwsContext		*p_swscale_;			///< @brief デコード後のフレームをRGBのフレームに変換するフィルタを指すポインタ
		int64_t			 frame_pts_;				///< @brief ビデオストリーム中での現在のフレーム位置を保持する変数

		bool			is_open_;				///< @brief ビデオが開いているかどうかのフラグ
		uint8_t*		encode_buf_;			///< @brief エンコードバッファ
		size_type		encode_buf_size_;		///< @brief エンコードバッファのサイズ
		size_type		width_;					///< @brief フレーム画像の幅
		size_type		height_;				///< @brief フレーム画像の高さ
		size_type		source_width_;			///< @brief 内部で使用する変数
		size_type		source_height_;			///< @brief 内部で使用する変数
		size_type		frame_rate_num_;		///< @brief フレームレート
		size_type		frame_rate_den_;		///< @brief フレームレートベース（実際のフレームレート＝フレームレート/フレームレートベース）
		double			frame_aspect_ratio_;	///< @brief フレームのアスペクト比（幅÷高さ）
		size_type		bit_rate_;				///< @brief ビットレート
		size_type		qmin_;					///< @brief 圧縮のクオリティー[0～32]（ビットレートとどちらかを指定）
		size_type		qmax_;					///< @brief 圧縮のクオリティー[0～100]（ビットレートとどちらかを指定）
		size_type		gop_size_;				///< @brief GOPサイズ（この枚数の連続フレーム中に必ず１枚以上Iフレームが存在する）
		size_type		max_b_frames_;			///< @brief 最大連続Bフレーム数
		size_type		audio_bit_rate_;		///< @brief 音声のビットレート
		size_type		audio_sampling_rate_;	///< @brief 音声のサンプリングレート
		size_type		audio_channels_;		///< @brief 音声のチャンネル数

	public:
		/// @brief コンストラクタ
		//! 
		//! @param[in] w                   … 320（デフォルト値）
		//! @param[in] h                   … 240（デフォルト値）
		//! @param[in] frame_rate_num      … 1（デフォルト値）
		//! @param[in] frame_rate_den      … 30（デフォルト値）
		//! @param[in] frame_aspect_ratio  … 4/3（デフォルト値）
		//! @param[in] bit_rate            … 1150000（デフォルト値）
		//! @param[in] qmin                … 固定品質の指定[0～31]（値が小さいほど高品質）。0以外を指定した場合はbit_rateは無視される
		//! @param[in] qmax                … 固定品質の指定[0～31]（値が小さいほど高品質）。0以外を指定した場合はbit_rateは無視される
		//! @param[in] gop_size            … 12（デフォルト値）
		//! @param[in] max_b_frames        … 2（デフォルト値）
		//! @param[in] audio_bit_rate      … 64000（デフォルト値）
		//! @param[in] audio_sampling_rate … 44100（デフォルト値）
		//! @param[in] audio_channels      … 2（デフォルト値）
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

		/// @brief デストラクタ
		//! 
		virtual ~encoder( )
		{
			close( );
		}

	public:
		/// @brief ビデオストリームが開いているかどうかを返す
		virtual bool is_open( ) const { return( is_open_ ); }

		/// @brief ビデオストリームの終わりに来たかどうかを返す
		virtual bool is_eof( ) const { return( false ); }

		/// @brief ストリームのフォーマットを標準出力にダンプする
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

		/// @brief 現在のフレーム番号を得る
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

		/// @brief 総フレーム数を得る
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

		/// @brief ファイル名を得る
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
		    
		/// @brief ビットレートを得る
		virtual size_type bit_rate( )  const
		{
			return( bit_rate_ );
		}
		
		/// @brief フレームの幅を得る
		virtual size_type width( ) const
		{
			return( width_ );
		}
		
		/// @brief フレームの高さを得る
		virtual size_type height( ) const
		{
			return( height_ );
		}

		/// @brief フレームレートを得る
		virtual long double frame_rate_numerator( ) const
		{
			return( frame_rate_num_ );
		}

		/// @brief フレームレートベースを得る（実際のフレームレート＝フレームレート/フレームレートベース）
		virtual long double frame_rate_denominator( ) const
		{
			return( frame_rate_den_ );
		}

		/// @brief フレームのアスペクト比を得る（ウィンドウ幅/ウィンドウ高さ）
		virtual long double frame_aspect_ratio( ) const
		{
			return( frame_aspect_ratio_ );
		}

	public:
		virtual bool open( const std::string &filename )
		{
			return( open( filename, filename, "", "" ) );
		}

		/// @brief ビデオファイル名およびMIME情報等を用いて出力用ビデオストリームを開く
		//! 
		//! @param[in] filename    … 出力ファイル名
		//! @param[in] format_type … ビデオの出力フォーマットに対応した拡張子（"."を含む）
		//! @param[in] video_type  … 出力するビデオの名称
		//! @param[in] mime_type   … 出力するビデオのMIMEタイプ
		//! @param[in] codec_id    … フォーマット内で使用するコーデックID
		//! 
		bool open( const std::string &filename, const std::string &format_type, const std::string &video_type, const std::string &mime_type, CodecID codec_id = CODEC_ID_NONE )
		{
			if( !is_open_ )
			{
				// 出力フォーマットを取得する
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

		/// @brief ビデオストリームを閉じる
		//! 
		bool close( )
		{
			if( is_open_ )
			{
				// フィルタを解放する
				sws_freeContext( p_swscale_ );

				// ビデオファイル閉じる
				av_write_trailer( p_fctx_ );

				if( ( p_fctx_->oformat->flags & AVFMT_NOFILE ) == 0 )
				{
					if( url_fclose( p_fctx_->pb ) < 0 )
					{
						std::cerr << "Failed to close output file." << std::endl;
						return( false );
					}
				}

				// すべてのストリームを解放する
				for( size_type i = 0 ; i < p_fctx_->nb_streams ; i++ )
				{
					// CODECを閉じる
					if( p_fctx_->streams[ i ]->codec->codec_id != CODEC_ID_NONE )
					{
						avcodec_close( p_fctx_->streams[ i ]->codec );
					}

					av_freep( &( p_fctx_->streams[ i ] ->codec ) );
					av_freep( &( p_fctx_->streams[ i ] ) );
				}

				// エンコードに使用したバッファを解放
				if( encode_buf_ != NULL )
				{
					av_freep( &encode_buf_ );
				}

				// 残りのデータを解放する
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
		/// @brief AVOutputFormat 情報を用いて出力用ビデオストリームを開く
		//! 
		//! @param[in] filename             … 出力ファイル名
		//! @param[in] format               … ビデオの出力フォーマット
		//! @param[in] video_codec_id       … フォーマット内で使用するビデオコーデックID
		//! @param[in] audio_codec_id       … フォーマット内で使用するオーディオコーデックID
		//! @param[in] default_pixel_format … フォーマット内でデフォルトの画素表現
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

					// コーデックのデフォルト設定を行う
					avcodec_get_context_defaults2( vstream->codec, CODEC_TYPE_VIDEO );

					// コーデックを推測する
					if( video_codec_id == CODEC_ID_NONE )
					{
						video_codec_id = av_guess_codec( p_fctx_->oformat, NULL, p_fctx_->filename, NULL, CODEC_TYPE_VIDEO );
					}

					// コーデックを探す
					AVCodec *codec = avcodec_find_encoder( video_codec_id );
					if( codec == NULL && p_fctx_->oformat->video_codec != CODEC_ID_NONE )
					{
						std::cerr << "Could not find appropriate Video Codec from the database." << std::endl;
						return( false );
					}

					// ビデオストリームにコーデック情報を指定する
					AVCodecContext *cctx = vstream->codec;
					cctx->codec_id   = codec == NULL ? video_codec_id : codec->id;
					cctx->codec_type = CODEC_TYPE_VIDEO;

					// 画像のサイズを設定
					cctx->width      = static_cast< int >( width( ) );
					cctx->height     = static_cast< int >( height( ) );

					// 画素フォーマットを設定
					cctx->pix_fmt    = default_pixel_format;

					// ピクセルフォーマットの対応状況を調べる
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

						/* 非対応の場合は対応する物に差し替え */
						if( *p == -1 )
						{
							cctx->pix_fmt = codec->pix_fmts[ 0 ];
						}
					}

					// ビットレートを設定
					cctx->bit_rate = static_cast< int >( bit_rate( ) );

					// 固定クオリティーが設定されている場合の処理
					if( qmin_ > 0 && qmax_ > 0 )
					{
						cctx->qmin = qmin_;
						cctx->qmax = qmax_;
					}

					// フレームレートを設定
					cctx->time_base.den = static_cast< int >( frame_rate_denominator( ) );
					cctx->time_base.num = static_cast< int >( frame_rate_numerator( ) );

					cctx->gop_size = gop_size_;

					// フレームのアスペクト比を設定する
					cctx->sample_aspect_ratio = vstream->sample_aspect_ratio = av_d2q( frame_aspect_ratio_ * cctx->height / cctx->width, 255 );

					if( cctx->codec_id == CODEC_ID_MPEG2VIDEO )
					{
						cctx->max_b_frames = static_cast< int >( max_b_frames_ );
					}
					if( cctx->codec_id == CODEC_ID_MPEG1VIDEO )
					{
						cctx->mb_decision=2;
					}

					// グローバルヘッダの要求を調べる
					if( ( format->flags & AVFMT_GLOBALHEADER ) != 0 )
					{
						cctx->flags |= CODEC_FLAG_GLOBAL_HEADER;
					}

					// パラメータを設定する
					if( av_set_parameters( p_fctx_, NULL ) < 0 ) 
					{
						std::cerr << "Invalid output format parameters" << std::endl;
						return( false );
					}

					// コーデックを開く
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

					// オーディオのストリームを設定する
					astream->codec->bit_rate    = static_cast< int >( audio_bit_rate_ );
					astream->codec->sample_rate = static_cast< int >( audio_sampling_rate_ );
					astream->codec->channels    = static_cast< int >( audio_channels_ );

					// コーデックを探す
					AVCodec *codec = avcodec_find_encoder( astream->codec->codec_id );
					if( codec == NULL )
					{
						std::cerr << "Could not find appropriate Audio Codec from the database." << std::endl;
						return( false );
					}

					// コーデックを開く
					if( avcodec_open( astream->codec, codec ) < 0 )
					{
						std::cerr << "Could not open codec." << std::endl;
						return( false );
					}
				}

				// ファイルを開く
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

				// ヘッダ情報を書き込む
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

				// エンコード用のフレームバッファを用意する
				p_frame_dst_ = allocate_frame( width( ), height( ), pix_fmt );
				if( p_frame_dst_ == NULL )
				{
					std::cerr << "Could not allocate frame buffer." << std::endl;
					return( false );
				}

				// 一時領域用のフレームバッファを用意する
				p_frame_rgb_ = allocate_frame( width( ), height( ), PIX_FMT_BGR24 );
				if( p_frame_dst_ == NULL )
				{
					std::cerr << "Could not allocate temporal frame buffer." << std::endl;
					return( false );
				}

				// 画像の変換用のフィルタを設定する
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
		/// @brief array2形式の画像をフレームバッファに書き込み，エンコードしてストリームに出力する
		//! 
		//! @param[in] image … ビデオストリームにエンコードする画像
		//! @param[in] tm    … ビデオストリームにエンコードする際のタイムスタンプ（-1の場合はフレームレートをもとに書き込み位置を算出する）
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
						// フィルタを解放する
						sws_freeContext( p_swscale_ );

						// フレームを解放する
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

				// 画像の形式を変換する
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
					// 画像をエンコードする
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
		/// @brief MPEG1ビデオ出力クラス
		//! 
		//! オーディオストリームは未サポート
		//!
		class encoder : public video::encoder
		{
		private:
			typedef video::encoder base;

		public:
			/// @brief コンストラクタ
			//! 
			//! @param[in] w                   … 320（デフォルト値）
			//! @param[in] h                   … 240（デフォルト値）
			//! @param[in] frame_rate_num      … 1（デフォルト値）
			//! @param[in] frame_rate_den      … 30（デフォルト値）
			//! @param[in] frame_aspect_ratio  … 4/3（デフォルト値）
			//! @param[in] bit_rate            … 1150000（デフォルト値）
			//! @param[in] qmin                … 固定品質の指定[0～31]（値が小さいほど高品質）。0以外を指定した場合はbit_rateは無視される
			//! @param[in] qmax                … 固定品質の指定[0～31]（値が小さいほど高品質）。0以外を指定した場合はbit_rateは無視される
			//! @param[in] gop_size            … 12（デフォルト値）
			//! @param[in] max_b_frames        … 2（デフォルト値）
			//!
			encoder( size_type w = 320, size_type h = 240, size_type frame_rate_num = 1, size_type frame_rate_den = 30, double frame_aspect_ratio = 4.0 / 3.0, size_type bit_rate = 1150000, size_type qmin = 0, size_type qmax = 0, size_type gop_size = 12, size_type max_b_frames = 2 )
				: base( w, h, frame_rate_num, frame_rate_den, frame_aspect_ratio, bit_rate, qmin, qmax, gop_size, max_b_frames )
			{
			}

			/// @brief コンストラクタ
			//! 
			//! コンストラクタの実行時に出力ビデオファイルの初期化を行う
			//! 
			//! @param[in] filename            … 出力ファイル名
			//! @param[in] w                   … 320（デフォルト値）
			//! @param[in] h                   … 240（デフォルト値）
			//! @param[in] frame_rate_num      … 1（デフォルト値）
			//! @param[in] frame_rate_den      … 30（デフォルト値）
			//! @param[in] frame_aspect_ratio  … 4/3（デフォルト値）
			//! @param[in] bit_rate            … 1150000（デフォルト値）
			//! @param[in] qmin                … 固定品質の指定[0～31]（値が小さいほど高品質）。0以外を指定した場合はbit_rateは無視される
			//! @param[in] qmax                … 固定品質の指定[0～31]（値が小さいほど高品質）。0以外を指定した場合はbit_rateは無視される
			//! @param[in] gop_size            … 12（デフォルト値）
			//! @param[in] max_b_frames        … 2（デフォルト値）
			//!
			encoder( const std::string &filename, size_type w = 320, size_type h = 240, size_type frame_rate_num = 1, size_type frame_rate_den = 30, double frame_aspect_ratio = 4.0 / 3.0, size_type bit_rate = 1150000, size_type qmin = 0, size_type qmax = 0, size_type gop_size = 12, size_type max_b_frames = 2 )
				: base( w, h, frame_rate_num, frame_rate_den, frame_aspect_ratio, bit_rate, qmin, qmax, gop_size, max_b_frames )
			{
				if( !open( filename ) )
				{
					throw;
				}
			}

			/// @brief デストラクタ
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
		/// @brief MPEG4ビデオ出力クラス
		//! 
		//! オーディオストリームは未サポート
		//!
		class encoder : public video::encoder
		{
		private:
			typedef video::encoder base;

		public:
			/// @brief コンストラクタ
			//! 
			//! @param[in] w                   … 320（デフォルト値）
			//! @param[in] h                   … 240（デフォルト値）
			//! @param[in] frame_rate_num      … 1（デフォルト値）
			//! @param[in] frame_rate_den      … 30（デフォルト値）
			//! @param[in] frame_aspect_ratio  … 4/3（デフォルト値）
			//! @param[in] bit_rate            … 1150000（デフォルト値）
			//!
			encoder( size_type w = 320, size_type h = 240, size_type frame_rate_num = 1, size_type frame_rate_den = 30, double frame_aspect_ratio = 4.0 / 3.0, size_type bit_rate = 1150000 )
				: base( w, h, frame_rate_num, frame_rate_den, frame_aspect_ratio, bit_rate )
			{
			}

			/// @brief コンストラクタ
			//! 
			//! コンストラクタの実行時に出力ビデオファイルの初期化を行う
			//! 
			//! @param[in] filename            … 出力ファイル名
			//! @param[in] w                   … 320（デフォルト値）
			//! @param[in] h                   … 240（デフォルト値）
			//! @param[in] frame_rate_num      … 1（デフォルト値）
			//! @param[in] frame_rate_den      … 30（デフォルト値）
			//! @param[in] frame_aspect_ratio  … 4/3（デフォルト値）
			//! @param[in] bit_rate            … 1150000（デフォルト値）
			//!
			encoder( const std::string &filename, size_type w = 320, size_type h = 240, size_type frame_rate_num = 1, size_type frame_rate_den = 30, double frame_aspect_ratio = 4.0 / 3.0, size_type bit_rate = 1150000 )
				: base( w, h, frame_rate_num, frame_rate_den, frame_aspect_ratio, bit_rate )
			{
				if( !open( filename ) )
				{
					throw;
				}
			}
			
			/// @brief デストラクタ
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
		/// @brief MotionJPEGビデオ出力クラス
		//! 
		//! オーディオストリームは未サポート
		//!
		class encoder : public video::encoder
		{
		private:
			typedef video::encoder base;

		public:
			/// @brief コンストラクタ
			//! 
			//! @param[in] w                   … 320（デフォルト値）
			//! @param[in] h                   … 240（デフォルト値）
			//! @param[in] frame_rate_num      … 1（デフォルト値）
			//! @param[in] frame_rate_den      … 30（デフォルト値）
			//! @param[in] frame_aspect_ratio  … 4/3（デフォルト値）
			//! @param[in] bit_rate            … 11500000（デフォルト値）
			//! @param[in] qmin                … 固定品質の指定[0～31]（値が小さいほど高品質）。0以外を指定した場合はbit_rateは無視される
			//! @param[in] qmax                … 固定品質の指定[0～31]（値が小さいほど高品質）。0以外を指定した場合はbit_rateは無視される
			//!
			encoder( size_type w = 320, size_type h = 240, size_type frame_rate_num = 1, size_type frame_rate_den = 30, double frame_aspect_ratio = 4.0 / 3.0, size_type bit_rate = 11500000, size_type qmin = 0, size_type qmax = 0 )
				: base( w, h, frame_rate_num, frame_rate_den, frame_aspect_ratio, bit_rate, qmin, qmax, 0 )
			{
			}

			/// @brief コンストラクタ
			//! 
			//! コンストラクタの実行時に出力ビデオファイルの初期化を行う
			//! 
			//! @param[in] filename            … 出力ファイル名
			//! @param[in] w                   … 320（デフォルト値）
			//! @param[in] h                   … 240（デフォルト値）
			//! @param[in] frame_rate_num      … 1（デフォルト値）
			//! @param[in] frame_rate_den      … 30（デフォルト値）
			//! @param[in] frame_aspect_ratio  … 4/3（デフォルト値）
			//! @param[in] bit_rate            … 11500000（デフォルト値）
			//! @param[in] qmin                … 固定品質の指定[0～31]（値が小さいほど高品質）。0以外を指定した場合はbit_rateは無視される
			//! @param[in] qmax                … 固定品質の指定[0～31]（値が小さいほど高品質）。0以外を指定した場合はbit_rateは無視される
			//!
			encoder( const std::string &filename, size_type w = 320, size_type h = 240, size_type frame_rate_num = 1, size_type frame_rate_den = 30, double frame_aspect_ratio = 4.0 / 3.0, size_type bit_rate = 11500000, size_type qmin = 0, size_type qmax = 0 )
				: base( w, h, frame_rate_num, frame_rate_den, frame_aspect_ratio, bit_rate, qmin, qmax, 0 )
			{
				if( !open( filename ) )
				{
					throw;
				}
			}
			
			/// @brief デストラクタ
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
		/// @brief H264ビデオ出力クラス
		//! 
		//! オーディオストリームは未サポート
		//!
		class encoder : public video::encoder
		{
		private:
			typedef video::encoder base;

		public:
			/// @brief コンストラクタ
			//! 
			//! @param[in] w                   … 320（デフォルト値）
			//! @param[in] h                   … 240（デフォルト値）
			//! @param[in] frame_rate_num      … 1（デフォルト値）
			//! @param[in] frame_rate_den      … 30（デフォルト値）
			//! @param[in] frame_aspect_ratio  … 4/3（デフォルト値）
			//! @param[in] bit_rate            … 1150000（デフォルト値）
			//!
			encoder( size_type w = 320, size_type h = 240, size_type frame_rate_num = 1, size_type frame_rate_den = 30, double frame_aspect_ratio = 4.0 / 3.0, size_type bit_rate = 1150000 )
				: base( w, h, frame_rate_num, frame_rate_den, frame_aspect_ratio, bit_rate )
			{
			}

			/// @brief コンストラクタ
			//! 
			//! コンストラクタの実行時に出力ビデオファイルの初期化を行う
			//! 
			//! @param[in] filename            … 出力ファイル名
			//! @param[in] w                   … 320（デフォルト値）
			//! @param[in] h                   … 240（デフォルト値）
			//! @param[in] frame_rate_num      … 1（デフォルト値）
			//! @param[in] frame_rate_den      … 30（デフォルト値）
			//! @param[in] frame_aspect_ratio  … 4/3（デフォルト値）
			//! @param[in] bit_rate            … 1150000（デフォルト値）
			//!
			encoder( const std::string &filename, size_type w = 320, size_type h = 240, size_type frame_rate_num = 1, size_type frame_rate_den = 30, double frame_aspect_ratio = 4.0 / 3.0, size_type bit_rate = 1150000 )
				: base( w, h, frame_rate_num, frame_rate_den, frame_aspect_ratio, bit_rate )
			{
				if( !open( filename ) )
				{
					throw;
				}
			}
			
			/// @brief デストラクタ
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
		/// @brief WMVビデオ出力クラス
		//! 
		//! オーディオストリームは未サポート
		//!
		class encoder : public video::encoder
		{
		private:
			typedef video::encoder base;

		public:
			/// @brief コンストラクタ
			//! 
			//! @param[in] w                   … 320（デフォルト値）
			//! @param[in] h                   … 240（デフォルト値）
			//! @param[in] frame_rate_num      … 1（デフォルト値）
			//! @param[in] frame_rate_den      … 30（デフォルト値）
			//! @param[in] frame_aspect_ratio  … 4/3（デフォルト値）
			//! @param[in] bit_rate            … 1150000（デフォルト値）
			//!
			encoder( size_type w = 320, size_type h = 240, size_type frame_rate_num = 1, size_type frame_rate_den = 30, double frame_aspect_ratio = 4.0 / 3.0, size_type bit_rate = 1150000 )
				: base( w, h, frame_rate_num, frame_rate_den, frame_aspect_ratio, bit_rate )
			{
			}

			/// @brief コンストラクタ
			//! 
			//! コンストラクタの実行時に出力ビデオファイルの初期化を行う
			//! 
			//! @param[in] filename            … 出力ファイル名
			//! @param[in] w                   … 320（デフォルト値）
			//! @param[in] h                   … 240（デフォルト値）
			//! @param[in] frame_rate_num      … 1（デフォルト値）
			//! @param[in] frame_rate_den      … 30（デフォルト値）
			//! @param[in] frame_aspect_ratio  … 4/3（デフォルト値）
			//! @param[in] bit_rate            … 1150000（デフォルト値）
			//!
			encoder( const std::string &filename, size_type w = 320, size_type h = 240, size_type frame_rate_num = 1, size_type frame_rate_den = 30, double frame_aspect_ratio = 4.0 / 3.0, size_type bit_rate = 1150000 )
				: base( w, h, frame_rate_num, frame_rate_den, frame_aspect_ratio, bit_rate )
			{
				if( !open( filename ) )
				{
					throw;
				}
			}
			
			/// @brief デストラクタ
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
		/// @brief DVビデオ出力クラス
		//! 
		//! オーディオストリームは未サポート
		//!
		class encoder : public video::encoder
		{
		private:
			typedef video::encoder base;

		public:
			/// @brief コンストラクタ
			//! 
			//! @param[in] w                   … 720（デフォルト値）
			//! @param[in] h                   … 480（デフォルト値）
			//! @param[in] frame_aspect_ratio  … 4/3（デフォルト値）
			//!
			encoder( size_type w = 720, size_type h = 480, double frame_aspect_ratio = 4.0 / 3.0 )
				: base( w, h, 1001, 30000, frame_aspect_ratio )
			{
				audio_sampling_rate_ = 48000;
			}

			/// @brief コンストラクタ
			//! 
			//! コンストラクタの実行時に出力ビデオファイルの初期化を行う
			//! 
			//! @param[in] filename            … 出力ファイル名
			//! @param[in] w                   … 720（デフォルト値）
			//! @param[in] h                   … 480（デフォルト値）
			//! @param[in] frame_aspect_ratio  … 4/3（デフォルト値）
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

			/// @brief デストラクタ
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
		/// @brief RAWビデオ出力クラス
		//! 
		//! オーディオストリームは未サポート
		//!
		class encoder : public video::encoder
		{
		private:
			typedef video::encoder base;

		public:
			/// @brief コンストラクタ
			//! 
			//! @param[in] w                   … 320（デフォルト値）
			//! @param[in] h                   … 240（デフォルト値）
			//! @param[in] frame_rate_num      … 1（デフォルト値）
			//! @param[in] frame_rate_den      … 30（デフォルト値）
			//! @param[in] frame_aspect_ratio  … 4/3（デフォルト値）
			//!
			encoder( size_type w = 320, size_type h = 240, size_type frame_rate_num = 1, size_type frame_rate_den = 30, double frame_aspect_ratio = 4.0 / 3.0 )
				: base( w, h, frame_rate_num, frame_rate_den, frame_aspect_ratio )
			{
			}

			/// @brief コンストラクタ
			//! 
			//! コンストラクタの実行時に出力ビデオファイルの初期化を行う
			//! 
			//! @param[in] filename            … 出力ファイル名
			//! @param[in] w                   … 320（デフォルト値）
			//! @param[in] h                   … 240（デフォルト値）
			//! @param[in] frame_rate_num      … 1（デフォルト値）
			//! @param[in] frame_rate_den      … 30（デフォルト値）
			//! @param[in] frame_aspect_ratio  … 4/3（デフォルト値）
			//!
			encoder( const std::string &filename, size_type w = 320, size_type h = 240, size_type frame_rate_num = 1, size_type frame_rate_den = 30, double frame_aspect_ratio = 4.0 / 3.0 )
				: base( w, h, frame_rate_num, frame_rate_den, frame_aspect_ratio )
			{
				if( !open( filename ) )
				{
					throw;
				}
			}

			/// @brief デストラクタ
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

	/// @brief 指定した回数だけビデオストリーム間でフレーム画像のコピー
	//! 
	//! @param[in,out] src         … コピー元ビデオストリーム
	//! @param[out]    dst         … コピー先ビデオストリーム
	//! @param[in]     num         … コピー回数
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
// video_group ビデオファイルの入出力を行うライブラリの終わり


// mist名前空間の終わり
_MIST_END



#endif // __INCLUDE_IO_VIDEO_H__
