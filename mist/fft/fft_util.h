#ifndef __INCLUDE_FFT_UTIL_H__
#define __INCLUDE_FFT_UTIL_H__


#ifndef __INCLUDE_MIST_H__
#include "../config/mist_conf.h"
#endif

#include <cmath>
#include <complex>

// mist名前空間の始まり
_MIST_BEGIN


namespace __fft_util__
{
	//データサイズの2のべき乗判定ルーチン
	inline bool size_check( unsigned int t )
	{
		int bit;

		if( t & 0x00000001 )
		{
			return false;
		}

		for( bit = 0 ; 0 != t ; t = ( unsigned int ) ( t >> 1 ) )
		{
			if( t & 0x00000001 )
			{
				bit++;
			}
		}

		if( bit == 1 )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	template < class T >
	struct is_complex
	{
		_MIST_CONST( bool, value, false );
	};

#if defined(__MIST_MSVC__) && __MIST_MSVC__ < 7

	#define IS_COMPLEX( type ) \
		template < >\
	struct is_complex< std::complex< type > >\
		{\
		enum{ value = true };\
		};\

		// 各型に対する特殊化
		IS_COMPLEX(unsigned char)
		IS_COMPLEX(unsigned short)
		IS_COMPLEX(unsigned int)
		IS_COMPLEX(unsigned long)
		IS_COMPLEX(signed char)
		IS_COMPLEX(signed short)
		IS_COMPLEX(signed int)
		IS_COMPLEX(signed long)
		IS_COMPLEX(bool)
		IS_COMPLEX(char)
		IS_COMPLEX(float)
		IS_COMPLEX(double)
		IS_COMPLEX(long double)

	#undef IS_COLOR

#else

	template < class T >
	struct is_complex< std::complex< T > >
	{
		_MIST_CONST( bool, value, true );
	};

#endif

	template < bool b >
	struct convert_to_complex
	{
		template < class T >
		static const std::complex< double > convert_to( const T &v )
		{
			return( std::complex< double >( v, 0.0 ) );
		}
	};

	template < >
	struct convert_to_complex< true >
	{
		template < class T >
		static const std::complex< double > convert_to( const T &v )
		{
			return( std::complex< double >( static_cast< double >( v.real( ) ), static_cast< double >( v.imag( ) ) ) );
		}
	};

	template < bool b >
	struct convert_from_complex
	{
		template < class T >
		static void convert_from( const std::complex< double > &v, T &out )
		{
			out = static_cast< T >( v.real( ) );
		}

		template < class T >
		static void convert_from( const double r, const double i, T &out )
		{
			out = static_cast< T >( r );
		}
	};

	template < >
	struct convert_from_complex< true >
	{
		template < class T >
		static void convert_from( const std::complex< double > &v, T &out )
		{
			typedef typename T::value_type value_type;
			out = T( static_cast< value_type >( v.real( ) ), static_cast< value_type >( v.imag( ) ) );
		}

		template < class T >
		static void convert_from( const double r, const double i, T &out )
		{
			typedef typename T::value_type value_type;
			out = T( static_cast< value_type >( r ), static_cast< value_type >( i ) );
		}
	};

	// 一般的なデータ型から複素数型に変換する関数
	// 複素数型から一般的なデータ型に変換する関数
	template < class T >
	struct convert_complex
	{
		static const std::complex< double > convert_to( const T &v )
		{
			return( convert_to_complex< is_complex< T >::value >::convert_to( v ) );
		}

		static const T convert_from( const std::complex< double > &v )
		{
			T dmy;
			convert_from_complex< is_complex< T >::value >::convert_from( v, dmy );
			return( dmy );
		}

		static const T convert_from( double r, double i )
		{
			T dmy;
			convert_from_complex< is_complex< T >::value >::convert_from( r, i, dmy );
			return( dmy );
		}
	};


	struct FFT_MEMORY1
	{
		double	*data;
		double	*w;
		int		*ip;
		size_t	size;
	};

	struct FFT_MEMORY2
	{
		double	**data;
		double	*t;
		double	*w;
		int		*ip;
		size_t	size;
	};

	struct FFT_MEMORY3
	{
		double	***data;
		double	*t;
		double	*w;
		int		*ip;
		size_t	size1;
		size_t	size2;
	};

	inline bool allocate_memory( FFT_MEMORY1 &mem, size_t data_size, size_t ip_size, size_t w_size )
	{
		mem.data	= new double[ data_size ];
		mem.w		= new double[ w_size ];
		mem.ip		= new int[ ip_size ];
		mem.size	= data_size;
		return( true );
	}

	inline void deallocate_memory( FFT_MEMORY1 &mem )
	{
		delete [] mem.data;
		delete [] mem.w;
		delete [] mem.ip;
	}

	inline bool allocate_memory( FFT_MEMORY2 &mem, size_t size1, size_t size2, size_t t_size, size_t ip_size, size_t w_size )
	{
		mem.data		= new double *[ size1 ];
		for( size_t i = 0 ; i < size1 ; i++ )
		{
			mem.data[ i ] = new double[ size2 ];
		}
		mem.t			= new double[ t_size ];
		mem.w			= new double[ w_size ];
		mem.ip			= new int[ ip_size ];
		mem.size	= size1;
		return( true );
	}

	inline void deallocate_memory( FFT_MEMORY2 &mem )
	{
		for( size_t i = 0 ; i < mem.size ; i++ )
		{
			delete [] mem.data[ i ];
		}
		delete [] mem.data;
		delete [] mem.t;
		delete [] mem.w;
		delete [] mem.ip;
	}

	inline bool allocate_memory( FFT_MEMORY3 &mem, size_t size1, size_t size2, size_t size3, size_t t_size, size_t ip_size, size_t w_size )
	{
		mem.data		= new double **[ size1 ];
		for( size_t i = 0 ; i < size1 ; i++ )
		{
			mem.data[ i ] = new double *[ size2 ];

			for( size_t j = 0 ; j < size2 ; j++ )
			{
				mem.data[ i ][ j ] = new double[ size3 ];
			}
		}
		mem.t			= new double[ t_size ];
		mem.w			= new double[ w_size ];
		mem.ip			= new int[ ip_size ];
		mem.size1	= size1;
		mem.size2	= size2;
		return( true );
	}

	inline void deallocate_memory( FFT_MEMORY3 &mem )
	{
		for( size_t i = 0 ; i < mem.size1 ; i++ )
		{
			for( size_t j = 0 ; j < mem.size2 ; j++ )
			{
				delete [] mem.data[ i ][ j ];
			}
			delete [] mem.data[ i ];
		}
		delete [] mem.data;
		delete [] mem.t;
		delete [] mem.w;
		delete [] mem.ip;
	}
}


_MIST_END

#endif	// __INCLUDE_FFT_UTIL_H__

