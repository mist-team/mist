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
//! @brief mpeg1, mpeg2形式ビデオ外リーム入出力
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
//! mist::mpeg::decoder iv;
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
//! mist::mpeg::decoder iv;
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
namespace video
{

	/// @brief 異なるストリーム間でフレームバッファのやり取りを行うためのクラス
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
		virtual bool is_open( ) const = 0;						///< @brief ビデオストリームが開いているかどうかを返す
		virtual bool is_eof( ) const = 0;						///< @brief ビデオストリームの終わりに来たかどうかを返す
		virtual bool dump( ) const = 0;							///< @brief ストリームのフォーマットを標準出力にダンプする
		virtual difference_type frame_id( ) const = 0;						///< @brief 現在のフレーム番号を返す
		virtual const std::string filename( ) const = 0;		///< @brief ファイル名を返す
		virtual size_type bit_rate( ) const = 0;				///< @brief ビットレートを返す
		virtual size_type width( ) const = 0;					///< @brief フレームの幅を返す
		virtual size_type height( ) const = 0;					///< @brief フレームの高さを返す
		virtual size_type frame_rate_numerator( ) const = 0;	///< @brief フレームレート（A÷B）の分子部分のAを返す
		virtual size_type frame_rate_denominator( ) const = 0;	///< @brief フレームレート（A÷B）の分母部分のBを返す

		/// @brief フレームレートを返す
		virtual double frame_rate( ) const
		{
			return( frame_rate_numerator( ) / frame_rate_denominator( ) );
		}

	protected:
		#pragma region 内部ユーティリティー関数
		AVOutputFormat *guess_video_format( const std::string &filename, const std::string &video_type )
		{
			AVOutputFormat *fmt = NULL;

			// ファイルの拡張子からフォーマットを推測する
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
		#pragma endregion
	};

	/// @brief mpegビデオ入力クラス
	//! 
	//! オーディオストリームは未サポート
	//!
	class decoder : public video_io_vase
	{
	private:
		AVFormatContext	*p_fctx_;				///< @brief mpegのフォーマット情報，デコーダ等
		AVFrame			*p_frame_src_;			///< @brief 読み出されたフレーム画像バッファ
		AVFrame			*p_frame_rgb_;			///< @brief RGBフォーマットのフレーム画像バッファ（array2形式のフレーム画像を得るための中間データ）
		bool			is_open_;				///< @brief ビデオが開いているかどうかのフラグ
		bool			is_eof_;				///< @brief ビデオが開いているかどうかのフラグ
		int				video_stream_index_;	///< @brief ビデオを指すストリーム番号
		SwsContext		*p_swscale_;			///< @brief デコード後のフレームをRGBのフレームに変換するフィルタを指すポインタ

	public:

		/// @brief コンストラクタ
		//! 
		//! デフォルトコンストラクタ
		//! 
		decoder( ) : p_fctx_( NULL ), p_frame_src_( NULL ), p_frame_rgb_( NULL ), is_open_( false ), is_eof_( true ), video_stream_index_( -1 ), p_swscale_( NULL )
		{
			if( !__libavcodec__::is_libavcodec_initialized )
			{
				av_register_all( );
				__libavcodec__::is_libavcodec_initialized = true;
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

		/// @brief 現在のフレーム番号を得る
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

		/// @brief mpegファイル名を得る
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
		
		/// @brief フレームレートベースを得る（実際のフレームレート＝フレームレート/フレームレートベース）
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
		/// @brief mpegファイルから入力用ビデオストリームを開く
		//! 
		//! @param[in] filename … 入力mpegファイル
		//! 
		virtual bool open( const std::string &filename )
		{
			if( !is_open( ) )
			{
				// ファイルのヘッダ情報を読み取ってビデオフォーマットを取得する
				if( av_open_input_file( &p_fctx_, filename.c_str( ), NULL, 0, NULL ) != 0 )
				{
					printf( "Couldn't open file %s\n", filename );
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
					printf( "Codec not found\n" );
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
				p_frame_rgb_ = allocate_frame( p_cctx->width, p_cctx->height, PIX_FMT_RGB32 );

				// 画像の変換用のフィルタを設定する
				p_swscale_ = sws_getContext( p_cctx->width, p_cctx->height, p_cctx->pix_fmt, p_cctx->width, p_cctx->height, PIX_FMT_RGB32, SWS_FAST_BILINEAR, NULL, NULL, NULL);

				// ファイルのオープンに成功したフラグを設定する
				is_open_ = true;
				is_eof_  = false;

				// 先頭フレームを読み込んでおく
				this->skip( 1 );

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

						// 動画ストリームを探す
						if( !is_eof( ) && packet.stream_index == video_stream_index_ )
						{
							// パケットをでコードする
							avcodec_decode_video( p_cctx, p_frame_src_, &bFinished, packet.data, packet.size );
						}

						av_free_packet( &packet );
					}
				}

				if( !is_eof( ) )
				{
					// 画像の形式を変換する
					sws_scale( p_swscale_, p_frame_src_->data, p_frame_src_->linesize, 0, p_cctx->height, p_frame_rgb_->data, p_frame_rgb_->linesize );
				}

				return( bFinished != 0 );
			}
			else
			{
				return( false );
			}
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
		//! @param[out] image   … フレーム画像を格納するための２次元配列
		//! @param[out] frameNo … 読み込んだフレームのフレーム番号を格納する符号付き整数
		//! 
		//! @return ストリームの終わりを超えて読み込みを行った場合は false を返す．
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

		/// @brief フレームバッファに格納された画像のインターレス除去
		//! 
		bool deinterlace( )
		{
			AVCodecContext *p_cctx = p_fctx_->streams[ video_stream_index_ ]->codec;
			return( avpicture_deinterlace( ( AVPicture * )p_frame_src_, ( AVPicture * )p_frame_src_, p_cctx->pix_fmt, p_cctx->width, p_cctx->height) >= 0 );
		}

	public:	// オペレータの実装
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



	/// @brief mpegビデオ出力クラス
	//! 
	//! オーディオストリームは未サポート
	//!
	class encoder : public video_io_vase
	{
	private:
		AVFormatContext	*p_fctx_;				///< @brief mpegのフォーマット情報，デコーダ等
		AVFrame			*p_frame_dst_;			///< @brief 書き出されるフレーム画像バッファ
		AVFrame			*p_frame_rgb_;			///< @brief RGBフォーマットのフレーム画像バッファ(array2形式の画像を得るための中間データ)
		SwsContext		*p_swscale_;			///< @brief デコード後のフレームをRGBのフレームに変換するフィルタを指すポインタ

		bool			is_open_;				///< @brief ビデオが開いているかどうかのフラグ
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
		#pragma region コンストラクタ関連
		/// @brief コンストラクタ
		//! 
		//! @param[in] encode_buf_size … 4194304（デフォルト値）
		//! @param[in] bit_rate        … 1150000（デフォルト値）
		//! @param[in] width           … 320（デフォルト値）
		//! @param[in] height          … 240（デフォルト値）
		//! @param[in] frame_rate      … 30000（デフォルト値）
		//! @param[in] frame_rate_base … 1001（デフォルト値）
		//! @param[in] gop_size        … 12（デフォルト値）
		//! @param[in] max_b_frames    … 1（デフォルト値）
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
		
		/// @brief デストラクタ
		//! 
		virtual ~encoder( )
		{
			close( );
		}
		#pragma endregion

	public:
		#pragma region ビデオの状態を取得する関数群
		/// @brief ビデオストリームが開いているかどうかを返す
		virtual bool is_open( ) const { return( is_open_ ); }

		/// @brief ビデオストリームの終わりに来たかどうかを返す
		virtual bool is_eof( ) const { return( false ); }

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

		/// @brief 現在のフレーム番号を得る
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

		/// @brief mpegファイル名を得る
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
				return( p_fctx_->streams[ 0 ]->codec->bit_rate );
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
				return( p_fctx_->streams[ 0 ]->codec->width );
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
				return( p_fctx_->streams[ 0 ]->codec->height );
			}
			else
			{
				return( 0 );
			}
		}

		/// @brief フレームレートを得る
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
		
		/// @brief フレームレートベースを得る（実際のフレームレート＝フレームレート/フレームレートベース）
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
		#pragma region ビデオストリーム操作関連
		virtual bool open( const std::string &filename )
		{
			return( open( filename, "" ) );
		}

		/// @brief mpegファイルから出力用ビデオストリームを開く
		//! 
		//! @param[in] filename … 出力mpegファイル
		//! 
		bool open( const std::string &filename, const std::string &video_type )
		{
			if( !is_open_ )
			{
				// 出力フォーマットを取得する
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

					// コーデックを推測する
					CodecID codec_id = av_guess_codec( p_fctx_->oformat, NULL, p_fctx_->filename, NULL, CODEC_TYPE_VIDEO );

					// コーデックを探す
					AVCodec *codec = avcodec_find_encoder( codec_id );
					if( codec == NULL )
					{
						std::cerr << "Could not find appropriate Codec from the database." << std::endl;
						return( false );
					}

					// ビデオストリームにコーデック情報を指定する
					AVCodecContext *cctx = stream->codec;
					cctx->codec_id   = codec->id;
					cctx->codec_type = CODEC_TYPE_VIDEO;
					cctx->width      = 320;
					cctx->height     = 240;
					cctx->pix_fmt    = PIX_FMT_YUV420P;

					// ピクセルフォーマットの対応状況を調べる
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

						/* 非対応の場合は対応する物に差し替え */
						if( *p == -1 )
						{
							cctx->pix_fmt = codec->pix_fmts[ 0 ];
						}
					}

					// ビットレートを設定
					cctx->bit_rate = 400000;

					// 画像のサイズを設定
					cctx->width = 320;
					cctx->height = 240;

					// フレームレートを設定
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

					// グローバルヘッダの要求を調べる
					if( ( format->flags & AVFMT_GLOBALHEADER ) != 0 )
					{
						cctx->flags |= CODEC_FLAG_GLOBAL_HEADER;
					}

					// コーデックを開く
					if( avcodec_open( cctx, codec ) < 0 )
					{
						std::cerr << "Could not open codec." << std::endl;
						return( false );
					}
				}

				// パラメータを設定する
				if( av_set_parameters( p_fctx_, NULL ) < 0 ) 
				{
					std::cerr << "Invalid output format parameters" << std::endl;
					return( false );
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

				// ヘッダ情報を書き込む
				av_write_header( p_fctx_ );

				AVCodecContext *cctx = stream->codec;
				encode_buf_ = NULL;
				if( ( p_fctx_->oformat->flags & AVFMT_RAWPICTURE ) == 0 )
				{
					encode_buf_ = ( uint8_t * )av_malloc( encode_buf_size_ );
				}

				// エンコード用のフレームバッファを用意する
				p_frame_dst_ = allocate_frame( cctx->width, cctx->height, cctx->pix_fmt );
				if( p_frame_dst_ == NULL )
				{
					std::cerr << "Could not allocate frame buffer." << std::endl;
					return( false );
				}

				// 一時領域用のフレームバッファを用意する
				p_frame_rgb_ = allocate_frame( cctx->width, cctx->height, PIX_FMT_RGB32 );
				if( p_frame_dst_ == NULL )
				{
					std::cerr << "Could not allocate temporal frame buffer." << std::endl;
					return( false );
				}

				// 画像の変換用のフィルタを設定する
				p_swscale_ = sws_getContext( cctx->width, cctx->height, PIX_FMT_RGB32, cctx->width, cctx->height, cctx->pix_fmt, SWS_BILINEAR, NULL, NULL, NULL);

				is_open_ = true;

				return( true );
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

				// codecを閉じる
				avcodec_close( p_fctx_->streams[ 0 ]->codec );

				// エンコードに使用したバッファを解放
				if( encode_buf_ != NULL )
				{
					av_freep( &encode_buf_ );
				}

				// すべてのストリームを解放する
				for( size_type i = 0 ; i < p_fctx_->nb_streams ; i++ )
				{
					av_freep( &( p_fctx_->streams[ i ] ->codec ) );
					av_freep( &( p_fctx_->streams[ i ] ) );
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
		#pragma endregion

	public:
		#pragma region フレームの書き出し
		/// @brief array2形式の画像をフレームバッファに書き込み，エンコードしてストリームに出力する
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

				// 画像の形式を変換する
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
					// 画像をエンコードする
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

		/// @brief フレームバッファに格納された画像のインターレス除去
		bool deinterlace( )
		{
			AVCodecContext *p_cctx = p_fctx_->streams[ 0 ]->codec;
			return avpicture_deinterlace( ( AVPicture * )p_frame_dst_, ( AVPicture * )p_frame_dst_, p_cctx->pix_fmt, p_cctx->width, p_cctx->height) >= 0;
		}
		#pragma endregion
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
			//dst.gop_size(        src.gop_size( ) );
			//dst.max_b_frames(    src.max_b_frames( ) );
			return true;
		}
		return false;
	}

} // namespace mpeg


/// @}
//  mpeg1, mpeg2形式ビデオストリーム入出力のためのライブラリの終わり


// mist名前空間の終わり
_MIST_END



#endif // __INCLUDE_IO_MPEG_H__
