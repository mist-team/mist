#ifndef __INCLUDE_MIST_LABELING__
#define __INCLUDE_MIST_LABELING__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#ifndef __INCLUDE_MIST_LIMITS__
#include "../limits.h"
#endif

// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


// ���f�B�A���t�B���^

namespace __labeling_controller__
{
	template < int nc >
	struct neighbor3
	{
		_MIST_CONST( size_t, array_num, 14 );

		template < class T, class Allocator >
		static inline void neighbor( array3< T, Allocator > &in, typename array3< T, Allocator >::value_type *l,
								typename array3< T, Allocator >::size_type i, typename array3< T, Allocator >::size_type j, typename array3< T, Allocator >::size_type k,
								typename array3< T, Allocator >::size_type w, typename array3< T, Allocator >::size_type h )
		{
			l[ 0 ]  = ( i     > 0 && j > 0 && k > 0       ) ? in( i - 1, j - 1, k - 1 ) : 0;
			l[ 1 ]  = ( j     > 0 && k > 0                ) ? in( i    , j - 1, k - 1 ) : 0;
			l[ 2 ]  = ( i + 1 < w && j > 0 && k > 0       ) ? in( i + 1, j - 1, k - 1 ) : 0;
			l[ 3 ]  = ( i     > 0 && k > 0                ) ? in( i - 1, j    , k - 1 ) : 0;
			l[ 4 ]  = ( k     > 0                         ) ? in( i    , j    , k - 1 ) : 0;
			l[ 5 ]  = ( i + 1 < w && k > 0                ) ? in( i + 1, j    , k - 1 ) : 0;
			l[ 6 ]  = ( i     > 0 && j + 1 < h  &&  k > 0 ) ? in( i - 1, j + 1, k - 1 ) : 0;
			l[ 7 ]  = ( j + 1 < h && k > 0                ) ? in( i    , j + 1, k - 1 ) : 0;
			l[ 8 ]  = ( i + 1 < w && j + 1 < h  &&  k > 0 ) ? in( i + 1, j + 1, k - 1 ) : 0;
			l[ 9 ]  = ( i     > 0 && j > 0                ) ? in( i - 1, j - 1, k     ) : 0;
			l[ 10 ] = ( j     > 0                         ) ? in( i    , j - 1, k     ) : 0;
			l[ 11 ] = ( i + 1 < w  &&  j > 0              ) ? in( i + 1, j - 1, k     ) : 0;
			l[ 12 ] = ( i     > 0                         ) ? in( i - 1, j    , k     ) : 0;
			l[ 13 ] = in( i, j, k );
		}
	};

	template < >
	struct neighbor3< 6 >
	{
		_MIST_CONST( size_t, array_num, 4 );

		template < class T, class Allocator >
		static inline void neighbor( array3< T, Allocator > &in, typename array3< T, Allocator >::value_type *l,
								typename array3< T, Allocator >::size_type i, typename array3< T, Allocator >::size_type j, typename array3< T, Allocator >::size_type k,
								typename array3< T, Allocator >::size_type w, typename array3< T, Allocator >::size_type h )
		{
			l[ 0 ] = ( k > 0 ) ? in( i    , j    , k - 1 ) : 0;
			l[ 1 ] = ( j > 0 ) ? in( i    , j - 1, k     ) : 0;
			l[ 2 ] = ( i > 0 ) ? in( i - 1, j    , k     ) : 0;
			l[ 3 ] = in( i, j, k );
		}
	};


	template < int nc >
	struct neighbor2
	{
		_MIST_CONST( size_t, array_num, 5 );

		template < class T, class Allocator >
		static inline void neighbor( array2< T, Allocator > &in, typename array2< T, Allocator >::value_type *l,
								typename array2< T, Allocator >::size_type i, typename array2< T, Allocator >::size_type j, typename array2< T, Allocator >::size_type w )
		{
			l[ 0 ] = ( i > 0 && j > 0     ) ? in( i - 1, j - 1 ) : 0;
			l[ 1 ] = ( j > 0              ) ? in( i    , j - 1 ) : 0;
			l[ 2 ] = ( i + 1 < w && j > 0 ) ? in( i + 1, j - 1 ) : 0;
			l[ 3 ] = ( i > 0              ) ? in( i - 1, j     ) : 0;
			l[ 4 ] = in( i, j );
		}
	};

	template < >
	struct neighbor2< 4 >
	{
		_MIST_CONST( size_t, array_num, 3 );

		template < class T, class Allocator >
		static inline void neighbor( array2< T, Allocator > &in, typename array2< T, Allocator >::value_type *l,
								typename array2< T, Allocator >::size_type i, typename array2< T, Allocator >::size_type j, typename array2< T, Allocator >::size_type w )
		{
			l[ 0 ] = ( j > 0 ) ? in( i    , j - 1 ) : 0;
			l[ 1 ] = ( i > 0 ) ? in( i - 1, j     ) : 0;
			l[ 2 ] = in( i, j );
		}
	};

	template < class T, class Allocator, class neighbor >
	typename array3< T, Allocator >::size_type labeling( array3< T, Allocator > &in, typename array3< T, Allocator >::value_type max_label, const neighbor dmy )
	{
		typedef typename array3< T, Allocator >::size_type  size_type;
		typedef typename array3< T, Allocator >::value_type value_type;

		size_type i, j, k, ii, jj, kk, pos;
		size_type array_num = neighbor::array_num;
		size_type w = in.width( );
		size_type h = in.height( );
		size_type d = in.depth( );
		value_type label = 0;
		value_type l[ neighbor::array_num ];
		value_type min, max, nmin;
		value_type m, n;
		value_type buf[ neighbor::array_num - 1 ];
		bool flag;
		value_type *t = new value_type[ max_label + 1 ];

		for( i = 0 ; i < max_label ; i++ )
		{
			t[ i ] = 0;
		}

		for( k = 0 ; k < d ; k++ )
		{
			for( j = 0 ; j < h ; j++ )
			{
				for( i = 0 ; i < w ; i++ )
				{
					if( in( i, j, k ) != 0 )
					{
						// �ߖT�̔����̎��o��
						neighbor::neighbor3( in, l, i, j, k, w, h );

						// �ߖT�l�̍ő�E�ŏ������߂�
						min = max = l[ 0 ];

						for( ii = 1 ; ii < array_num - 1 ; ii++ )
						{
							if( min > l[ ii ] )
							{
								min = l[ ii ];
							}
							else if( max < l[ ii ] )
							{
								max = l[ ii ];
							}
						}

						if( min == 0 && max == 0 )
						{
							// �ߖT�l�����ׂ�0�Ȃ�
							if( label < max_label )
							{
								// ���̃��x�����e�[�u���̑傫���ȉ��Ȃ�
								label++;

								// �e�[�u���ɐV�������x����o�^
								t[ label ] = label;

								// �摜�Ƀ��x��������
								in( i, j, k ) = label;
							}
							else
							{
								// �������x���ԍ��������ς��Ȃ�K�x�[�W�R���N�V����
								// �e�[�u���̒��̐���
								n = 1;

								for( ii = 1 ; ii <= label ; ii++ )
								{
									t[ ii ] = t[ ii ] < ii ? t[ t[ ii ] ] : t[ ii ];
								}

								for( ii = 1 ; ii <= label ; ii++ )
								{
									if( t[ ii ] > n )
									{
										m = t[ ii ];

										for( jj = 1 ; jj <= label ; jj++ )
										{
											t[ jj ] = t[ jj ] == m ? n : t[ jj ];
										}
									}

									n = t[ ii ] >= n ? n + 1 : n;
								}

								// ���x���ԍ��̕t������
								for( kk = 0 ; kk <= k ; kk++ )
								{
									for( jj = 0 ; jj <= j ; jj++ )
									{
										for( ii = 0 ; ii <= i ; ii++ )
										{
											in( ii, jj, kk ) = t[ in( ii, jj, kk ) ];
										}
									}
								}

								// �e�[�u���̒������ɖ߂�
								for( ii = 1 ; ii <= n ; ii++ )
								{
									t[ ii ] = static_cast< value_type >( ii );
								}

								label = n;

								// ����ł�����Ȃ�
								if( label >= max_label )
								{
									std::cerr << " TABLE is NOT ENOUGH" << std::endl;
									delete [] t;
									return ( max_label + 1 );
								}

								t[ label ] = label;
								in( i, j, k ) = label;
							}
						}
						else
						{
							pos = 0;

							// �ߖT�l�̒��̐��l���Ƃ��Ă���
							for( ii = 0 ; ii < array_num - 1 ; ii++ )
							{
								flag = false;

								for( jj = 0 ; jj < pos ; jj++ )
								{
									if( buf[ jj ] == l[ ii ] && l[ ii ] > 0 )
									{
										flag = true;
										break;
									}
								}

								if( !flag && l[ ii ] > 0 )
								{
									buf[ pos ] = l[ ii ];
									pos++;
								}
							}

							// �ߖT�l�̒��ōł��������l������
							if( pos > 1 )
							{
								for( ii = 1 ; ii <= label ; ii++ )
								{
									t[ ii ] = t[ ii ] < ii ? t[ t[ ii ] ] : t[ ii ];
								}

								nmin = t[ buf[ 0 ] ];

								for( ii = 0 ; ii < pos ; ii++ )
								{
									nmin = t[ buf[ ii ] ] < nmin ? t[ buf[ ii ] ] : nmin;
								}

								for( ii = 0 ; ii < pos ; ii++ )
								{
									if( t[ buf[ ii ] ] > nmin )
									{
										for( jj = 1 ; jj <= label ; jj++ )
										{
											t[ jj ] = t[ jj ] == t[ buf[ ii ] ] ? nmin : t[ jj ];
										}
									}

									t[ buf[ ii ] ] = nmin;
								}

								in( i, j, k ) = nmin;
							}
							else
							{
								in( i, j, k ) = t[ buf[ 0 ] ];
							}
						}
					}
				}
			}
		}

		// �e�[�u���̐���
		n = 1;

		for( ii = 1 ; ii <= label ; ii++ )
		{
			t[ ii ] = t[ ii ] < ii ? t[ t[ ii ] ] : t[ ii ];
		}

		for( ii = 1 ; ii <= label ; ii++ )
		{
			if( t[ ii ] > n )
			{
				m = t[ ii ];

				for( jj = 1 ; jj <= label ; jj++ )
				{
					t[ jj ] = t[ jj ] == m ? n : t[ jj ];
				}
			}

			n = t[ ii ] >= n ? n + 1 : n;
		}

		label = n - 1;

		for( i = 0 ; i < in.size( ) ; i++ )
		{
			in[ i ] = t[ in[ i ] ];
		}

		delete [] t;
		return ( label );
	}

	template < class T, class Allocator, class neighbor >
	typename array2< T, Allocator >::size_type labeling( array2< T, Allocator > &in, typename array2< T, Allocator >::value_type max_label, const neighbor labeling_type )
	{
		typedef typename array2< T, Allocator >::size_type  size_type;
		typedef typename array2< T, Allocator >::value_type value_type;

		size_type i, j, ii, jj, pos;
		size_type array_num = neighbor::array_num;
		size_type w = in.width( );
		size_type h = in.height( );
		value_type label = 0;
		value_type l[ neighbor::array_num ];
		value_type min, max, nmin;
		value_type m, n;
		value_type buf[ neighbor::array_num - 1 ];
		bool flag;
		value_type *t = new value_type[ max_label + 1 ];

		for( i = 0 ; i < max_label ; i++ )
		{
			t[ i ] = 0;
		}

		for( j = 0 ; j < h ; j++ )
		{
			for( i = 0 ; i < w ; i++ )
			{
				if( in( i, j ) != 0 )
				{
					// �ߖT�̔����̎��o��
					neighbor::neighbor( in, l, i, j, w );

					// �ߖT�l�̍ő�E�ŏ������߂�
					min = max = l[ 0 ];

					for( ii = 1 ; ii < array_num - 1 ; ii++ )
					{
						if( min > l[ ii ] )
						{
							min = l[ ii ];
						}
						else if( max < l[ ii ] )
						{
							max = l[ ii ];
						}
					}

					if( min == 0 && max == 0 )
					{
						// �ߖT�l�����ׂ�0�Ȃ�
						if( label < max_label )
						{
							// ���̃��x�����e�[�u���̑傫���ȉ��Ȃ�
							label++;

							// �e�[�u���ɐV�������x����o�^
							t[ label ] = label;

							// �摜�Ƀ��x��������
							in( i, j ) = label;
						}
						else
						{
							// �������x���ԍ��������ς��Ȃ�K�x�[�W�R���N�V����
							// �e�[�u���̒��̐���
							n = 1;

							for( ii = 1 ; ii <= label ; ii++ )
							{
								t[ ii ] = t[ ii ] < ii ? t[ t[ ii ] ] : t[ ii ];
							}

							for( ii = 1 ; ii <= label ; ii++ )
							{
								if( t[ ii ] > n )
								{
									m = t[ ii ];

									for( jj = 1 ; jj <= label ; jj++ )
									{
										t[ jj ] = t[ jj ] == m ? n : t[ jj ];
									}
								}

								n = t[ ii ] >= n ? n + 1 : n;
							}

							// ���x���ԍ��̕t������
							for( jj = 0 ; jj <= j ; jj++ )
							{
								for( ii = 0 ; ii <= i ; ii++ )
								{
									in( ii, jj ) = t[ in( ii, jj ) ];
								}
							}

							// �e�[�u���̒������ɖ߂�
							for( ii = 1 ; ii <= n ; ii++ )
							{
								t[ ii ] = static_cast< value_type >( ii );
							}

							label = n;

							// ����ł�����Ȃ�
							if( label >= max_label )
							{
								std::cerr << " TABLE is NOT ENOUGH" << std::endl;
								delete [] t;
								return ( max_label + 1 );
							}

							t[ label ] = label;
							in( i, j ) = label;
						}
					}
					else
					{
						pos = 0;

						// �ߖT�l�̒��̐��l���Ƃ��Ă���
						for( ii = 0 ; ii < array_num - 1 ; ii++ )
						{
							flag = false;

							for( jj = 0 ; jj < pos ; jj++ )
							{
								if( buf[ jj ] == l[ ii ] && l[ ii ] > 0 )
								{
									flag = true;
									break;
								}
							}

							if( !flag && l[ ii ] > 0 )
							{
								buf[ pos ] = l[ ii ];
								pos++;
							}
						}

						// �ߖT�l�̒��ōł��������l������
						if( pos > 1 )
						{
							for( ii = 1 ; ii <= label ; ii++ )
							{
								t[ ii ] = t[ ii ] < ii ? t[ t[ ii ] ] : t[ ii ];
							}

							nmin = t[ buf[ 0 ] ];

							for( ii = 0 ; ii < pos ; ii++ )
							{
								nmin = t[ buf[ ii ] ] < nmin ? t[ buf[ ii ] ] : nmin;
							}

							for( ii = 0 ; ii < pos ; ii++ )
							{
								if( t[ buf[ ii ] ] > nmin )
								{
									for( jj = 1 ; jj <= label ; jj++ )
									{
										t[ jj ] = t[ jj ] == t[ buf[ ii ] ] ? nmin : t[ jj ];
									}
								}

								t[ buf[ ii ] ] = nmin;
							}

							in( i, j ) = nmin;
						}
						else
						{
							in( i, j ) = t[ buf[ 0 ] ];
						}
					}
				}
			}
		}

		// �e�[�u���̐���
		n = 1;

		for( ii = 1 ; ii <= label ; ii++ )
		{
			t[ ii ] = t[ ii ] < ii ? t[ t[ ii ] ] : t[ ii ];
		}

		for( ii = 1 ; ii <= label ; ii++ )
		{
			if( t[ ii ] > n )
			{
				m = t[ ii ];

				for( jj = 1 ; jj <= label ; jj++ )
				{
					t[ jj ] = t[ jj ] == m ? n : t[ jj ];
				}
			}

			n = t[ ii ] >= n ? n + 1 : n;
		}

		label = n - 1;

		for( i = 0 ; i < in.size( ) ; i++ )
		{
			in[ i ] = t[ in[ i ] ];
		}

		delete [] t;
		return ( label );
	}
}


template < class T1, class T2, class Allocator1, class Allocator2 >
typename array2< T2, Allocator2 >::size_type labeling4( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out, typename array2< T2, Allocator2 >::value_type max_label = 0 )
{
	typedef typename array2< T2, Allocator2 >::size_type  size_type;
	typedef typename array2< T2, Allocator2 >::value_type value_type;

	if( max_label == 0 )
	{
		max_label = type_limits< value_type >::maximum( );
	}

	out.resize( in.size1( ), in.size2( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );

	for( size_type i = 0 ; i < in.size( ) ; i++ )
	{
		out[ i ] = in[ i ];
	}
	return( __labeling_controller__::labeling( out, max_label, __labeling_controller__::neighbor2< 4 >( ) ) );
}

template < class T1, class T2, class Allocator1, class Allocator2 >
typename array2< T2, Allocator2 >::size_type labeling8( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out, typename array2< T2, Allocator2 >::value_type max_label = 0 )
{
	typedef typename array2< T2, Allocator2 >::size_type  size_type;
	typedef typename array2< T2, Allocator2 >::value_type value_type;

	if( max_label == 0 )
	{
		max_label = type_limits< value_type >::maximum( );
	}

	out.resize( in.size1( ), in.size2( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );

	for( size_type i = 0 ; i < in.size( ) ; i++ )
	{
		out[ i ] = in[ i ];
	}
	return( __labeling_controller__::labeling( out, max_label, __labeling_controller__::neighbor2< 8 >( ) ) );
}

template < class T1, class T2, class Allocator1, class Allocator2 >
typename array3< T2, Allocator2 >::size_type labeling6( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out, typename array3< T2, Allocator2 >::value_type max_label = 0 )
{
	typedef typename array3< T2, Allocator2 >::size_type  size_type;
	typedef typename array3< T2, Allocator2 >::value_type value_type;

	if( max_label == 0 )
	{
		max_label = type_limits< value_type >::maximum( );
	}

	out.resize( in.size1( ), in.size2( ), in.size3( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );
	out.reso3( in.reso3( ) );

	for( size_type i = 0 ; i < in.size( ) ; i++ )
	{
		out[ i ] = in[ i ];
	}
	return( __labeling_controller__::labeling( out, max_label, __labeling_controller__::neighbor3< 6 >( ) ) );
}


template < class T1, class T2, class Allocator1, class Allocator2 >
typename array3< T2, Allocator2 >::size_type labeling26( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out, typename array3< T2, Allocator2 >::value_type max_label = 0 )
{
	typedef typename array3< T2, Allocator2 >::size_type  size_type;
	typedef typename array3< T2, Allocator2 >::value_type value_type;

	if( max_label == 0 )
	{
		max_label = type_limits< value_type >::maximum( );
	}

	out.resize( in.size1( ), in.size2( ), in.size3( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );
	out.reso3( in.reso3( ) );

	for( size_type i = 0 ; i < in.size( ) ; i++ )
	{
		out[ i ] = in[ i ];
	}
	return( __labeling_controller__::labeling( out, max_label, __labeling_controller__::neighbor3< 26 >( ) ) );
}

// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_LABELING__
