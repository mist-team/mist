#include "config.h"

#include "main_window.h"

#include "filter/input.h"

#include "filter/threshold.h"

#include "filter/median.h"
#include "filter/interlace.h"

#include "filter/morphology.h"

#include "filter/copy.h"
#include "filter/convert.h"
#include "filter/blend.h"

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

		treelist_->collapseTree( input );
	}

	{
		FXTreeItem *utility = treelist_->addItemLast( root, "Utility" );
		treelist_->expandTree( utility );

		insert_filter( treelist_, utility, flist, new color2gray_filter( ) );

		insert_filter( treelist_, utility, flist, new copy_filter1( ) );
		insert_filter( treelist_, utility, flist, new copy_filter2( ) );
		insert_filter( treelist_, utility, flist, new copy_filter3( ) );

		insert_filter( treelist_, utility, flist, new blend_filter( ) );

		treelist_->collapseTree( utility );
	}

	{
		FXTreeItem *threshold = treelist_->addItemLast( root, "Threshold" );
		treelist_->expandTree( threshold );

		insert_filter( treelist_, threshold, flist, new ptile_threshold_filter( ) );
		insert_filter( treelist_, threshold, flist, new discriminant_analysis_threshold_filter( ) );

		treelist_->collapseTree( threshold );
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

			treelist_->collapseTree( median );
		}

		{
			FXTreeItem *interlace = treelist_->addItemLast( smoothing, "Interlace" );
			treelist_->expandTree( interlace );

			insert_filter( treelist_, interlace, flist, new interlace_filter1( ) );
			insert_filter( treelist_, interlace, flist, new interlace_filter2( ) );
			insert_filter( treelist_, interlace, flist, new interlace_filter3( ) );

			treelist_->collapseTree( interlace );
		}
	}

	{
		FXTreeItem *morphology = treelist_->addItemLast( root, "Morphology" );
		treelist_->expandTree( morphology );

		insert_filter( treelist_, morphology, flist, new erosion_filter( ) );
		insert_filter( treelist_, morphology, flist, new dilation_filter( ) );
		insert_filter( treelist_, morphology, flist, new opening_filter( ) );
		insert_filter( treelist_, morphology, flist, new closing_filter( ) );

		treelist_->collapseTree( morphology );
	}
}


