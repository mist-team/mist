#include "config.h"

#include "main_window.h"

#include "filter/input.h"
#include "filter/median.h"

#include "filter/convert.h"


void main_window::ClearFilters( )
{
	mist_builder &app = getAppInstance( );
	app.filters.clear( );

	for( size_type i = 0 ; i < app.filters.size( ) ; i++ )
	{
		delete app.filters[ i ];
	}

	app.filters.clear( );
}


void insert_filter( FXTreeList *tree_list, FXTreeItem *parent, filter_list &flist, filter *f )
{
	flist.push_back( f );
	tree_list->addItemLast( parent, f->name( ).c_str( ), NULL, NULL, f );
}

void main_window::EnumFilters( )
{
	mist_builder &app = getAppInstance( );
	ClearFilters( );

	// グローバルスペースにフィルタの一覧を作成する

	filter_list &flist = app.filters;
	FXTreeItem *root = NULL;

	// フィルタデータをリストに登録する
	{
		FXTreeItem *input = treelist_->addItemLast( root, "Input" );
		treelist_->expandTree( input );

		insert_filter( treelist_, input, flist, new input_filter( ) );
	}

	{
		FXTreeItem *convert = treelist_->addItemLast( root, "Converter" );
		treelist_->expandTree( convert );

		insert_filter( treelist_, convert, flist, new color2gray_filter( ) );
	}

	{
		FXTreeItem *smoothing = treelist_->addItemLast( root, "Smoothing" );
		treelist_->expandTree( smoothing );

		{
			FXTreeItem *median = treelist_->addItemLast( smoothing, "Median" );
			treelist_->expandTree( median );

			insert_filter( treelist_, median, flist, new median_filter1( ) );
			insert_filter( treelist_, median, flist, new median_filter2( ) );
			insert_filter( treelist_, median, flist, new median_filter3( ) );
		}
	}
}


