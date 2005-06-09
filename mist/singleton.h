/// @file mist/singleton.h
//!
//! @brief Singleton を扱うためのライブラリ
//!
#ifndef __INCLUDE_MIST_SINGLETON__
#define __INCLUDE_MIST_SINGLETON__


#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif


// mist名前空間の始まり
_MIST_BEGIN


/// @brief Singleton を扱うクラス
//! 
//! singleton< int, 0 > と singleton< int, 1 > を別物として扱うことができる．
//! ２つ目の引数を省略し，singleton< int > とすると ID が 0 のシングルトンガ作成される．
//! 
//! @attention 実行時の初期化順序は正しく扱えないので注意が必要
//! @attention そのため，大域的な変数を本Singletonクラスのインスタンスで初期化することはできない
//! @attention 本クラスの実態を作成することはできない
//! @attention 実態の初期化は，最初に関数が呼び出された時点で行われる．
//! 
//! @param T  … Singletonを作成する型
//! @param ID … 同じ型で複数のSingletonを作成する場合に利用するID
//! 
template < class T, size_t ID = 0 >
class singleton
{
public:
	typedef T value_type;	///< @brief Singletonを作成する型

	/// @brief Singletonの実態を返す関数
	//! 
	//! @return Singletonのインスタンス
	//! 
	static value_type &get_instance( )
	{
		static value_type singleton_;
		return( singleton_ );
	}

private:
	singleton( );
	singleton( const singleton &s );
	const singleton & operator =( const singleton &s );
};


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_SINGLETON__
