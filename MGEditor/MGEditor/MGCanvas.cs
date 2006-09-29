using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Text;
using System.Windows.Forms;

namespace MGEditor
{
	public partial class MGCanvas : Control
	{
		#region 内部クラス
		#endregion

		#region 変数
		private List< MGComponent > components_ = new List<MGComponent>( );
		#endregion

		#region プロパティ
		#endregion

		#region コンストラクタ
		public MGCanvas( )
		{
			InitializeComponent( );

			SetStyle( ControlStyles.SupportsTransparentBackColor, true );

			base.DoubleBuffered = true;

			{
				Panel panel = new Panel( );
				panel.BackColor = Color.Aqua;
				panel.Size = new Size( 30, 30 );
				components_.Add( new MGComponent( this, panel, "hoge1", this.Font, 10, 10 ) );
			}

			{
				Panel panel = new Panel( );
				panel.BackColor = Color.Red;
				panel.Size = new Size( 50, 30 );
				components_.Add( new MGComponent( this, panel, "hoge2", this.Font, 150, 150 ) );
			}
		}
		#endregion

		#region 描画関連
		protected override void OnPaint( PaintEventArgs pe )
		{
			Graphics g = pe.Graphics;

			for( int i = components_.Count - 1 ; i >= 0 ; i-- )
			{
				MGComponent c = components_[ i ];
				c.OnPaintBackground( pe );
				c.OnPaint( pe );
			}
		}

		protected override void OnPaintBackground( PaintEventArgs pe )
		{
			base.OnPaintBackground( pe );

			//foreach( MGComponent c in components_ )
			//{
			//    c.OnPaintBackground( pe );
			//}
		}
		#endregion

		#region マウス関連
		protected override void OnMouseDown( MouseEventArgs e )
		{
			foreach( MGComponent c in components_ )
			{
				c.OnMouseDown( e );
			}

			base.OnMouseDown( e );
		}

		protected override void OnMouseUp( MouseEventArgs e )
		{
			foreach( MGComponent c in components_ )
			{
				c.OnMouseUp( e );
			}

			base.OnMouseUp( e );
		}

		protected override void OnMouseMove( MouseEventArgs e )
		{
			foreach( MGComponent c in components_ )
			{
				c.OnMouseMove( e );
			}

			base.OnMouseMove( e );
		}

		protected override void OnMouseEnter( EventArgs e )
		{
			foreach( MGComponent c in components_ )
			{
				c.OnMouseEnter( e );
			}

			base.OnMouseEnter( e );
		}

		protected override void OnMouseLeave( EventArgs e )
		{
			foreach( MGComponent c in components_ )
			{
				c.OnMouseLeave( e );
			}

			base.OnMouseLeave( e );
		}

		protected override void OnMouseHover( EventArgs e )
		{
			foreach( MGComponent c in components_ )
			{
				c.OnMouseHover( e );
			}

			base.OnMouseHover( e );
		}

		protected override void OnMouseWheel( MouseEventArgs e )
		{
			foreach( MGComponent c in components_ )
			{
				c.OnMouseWheel( e );
			}

			base.OnMouseWheel( e );
		}
		#endregion
	}
}
