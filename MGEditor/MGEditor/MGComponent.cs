using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;
using System.ComponentModel;
using System.ComponentModel.Design;
using System.Windows.Forms.Design;

namespace MGEditor
{
	class MGComponent
	{
		#region 内部クラス
		#endregion

		#region 変数
		private bool isCollapsed = false;
		private bool isHover = false;
		private bool isPushed = false;
		private bool isDrag = false;
		private Color textColor = Color.White;
		private Color leftColor = Color.FromArgb( 120, 120, 147 );
		private Color rightColor = Color.FromArgb( 178, 180, 198 );
		private Color backColor = Color.White;
		private int panelHeight = 100;
		private int captionCurveRadius = 7;
		private int captionHeight = 25;
		private static Image expandImage1 = Properties.Resources.expand1;
		private static Image expandImage2 = Properties.Resources.expand2;
		private static Image collapseImage1 = Properties.Resources.collapse1;
		private static Image collapseImage2 = Properties.Resources.collapse2;

		// フィルターコンポーネントの基本情報
		private System.Windows.Forms.Control parent_ = null;
		private string text_ = "";
		private Padding padding_ = Padding.Empty;
		private Rectangle bounds_ = Rectangle.Empty;
		private System.Windows.Forms.Control property_ = null;
		private System.Drawing.Font font_ = null;
		private int propertyMargin = 3;
		#endregion

		#region プロパティ
		public System.Windows.Forms.Control Parent
		{
			get
			{
				return ( parent_ );
			}
		}

		public Point Location
		{
			get
			{
				return ( new Point( bounds_.X, bounds_.Y ) );
			}
			set
			{
				bounds_.X = value.X;
				bounds_.Y = value.Y;

				if( property_ != null )
				{
					property_.Location = new Point( bounds_.X + padding_.Left + propertyMargin, bounds_.Y + captionHeight + padding_.Top + propertyMargin );
				}
			}
		}

		[Category( "表示" ), DefaultValue( false ), Description( "閉じた状態にするかどうか" )]
		public bool IsCollapsed
		{
			get { return ( isCollapsed ); }
			set
			{
				if( isCollapsed != value || property_ == null )
				{
					isCollapsed = value;

					// デザインモードでなければ実行する
					if( isCollapsed || property_ == null )
					{
						panelHeight = this.Height;

						Rectangle rc = this.Bounds;

						property_.Visible = false;

						// 大きさを変更する
						this.Height = padding_.Top + padding_.Bottom + captionHeight;

						this.Invalidate( rc );
					}
					else
					{
						// 大きさを元に戻す
						this.Height = panelHeight;

						property_.Visible = true;

						this.Invalidate( this.Bounds );
					}
				}
			}
		}

		[Category( "表示" ), DefaultValue( "テキスト" ), Description( "タイトルに表示する文字列" ), Browsable( true )]
		public virtual string Text
		{
			get { return ( text_ ); }
			set
			{
				text_ = value;
				this.Invalidate( this.CaptionRectangle );
			}
		}

		[Browsable( false ), DesignerSerializationVisibility( DesignerSerializationVisibility.Hidden )]
		protected Rectangle CaptionRectangle
		{
			get
			{
				Rectangle cRect = this.ClientRectangle;
				return ( new Rectangle( cRect.Left, cRect.Top, cRect.Width, captionHeight ) );
			}
		}

		[Browsable( false ), DesignerSerializationVisibility( DesignerSerializationVisibility.Hidden )]
		protected Rectangle ButtonRectangle
		{
			get
			{
				Rectangle cRect = this.ClientRectangle;
				return ( new Rectangle( cRect.Right - 19 - 6, 3 + cRect.Top, 19, 19 ) );
			}
		}

		public virtual Rectangle Bounds
		{
			get
			{
				return ( bounds_ );
			}
			set
			{
				bounds_ = value;
			}
		}

		public virtual Rectangle ClientRectangle
		{
			get
			{
				return ( new Rectangle( bounds_.Left + padding_.Left, bounds_.Top + padding_.Top, bounds_.Width - ( padding_.Left + padding_.Right ), bounds_.Height - ( padding_.Top + padding_.Bottom ) ) );
			}
		}

		public virtual int Width
		{
			get
			{
				return ( bounds_.Width );
			}
			set
			{
				if( value > 0 )
				{
					bounds_.Width = value;
				}
			}
		}

		public virtual int Height
		{
			get
			{
				return ( bounds_.Height );
			}
			set
			{
				if( value > 0 )
				{
					bounds_.Height = value;
				}
			}
		}
		#endregion

		#region コンストラクタ
		public MGComponent( System.Windows.Forms.Control parent, System.Windows.Forms.Control control, string text, System.Drawing.Font font, int x, int y )
		{
			this.parent_ = parent;
			this.text_ = text;
			this.font_ = font;

			Size size = TextRenderer.MeasureText( text, font_ );
			int width = size.Width + 10 + 26 + padding_.Left + padding_.Right + propertyMargin * 2;
			int height = captionHeight + padding_.Top + padding_.Bottom + propertyMargin * 2;

			property_ = control;

			if( property_ != null )
			{
				if( parent != null )
				{
					parent.Controls.Add( property_ );
				}

				this.Location = new Point( x, y );

				if( width < property_.Width + propertyMargin * 2 )
				{
					width = property_.Width + propertyMargin * 2;
				}
				else
				{
					property_.Width = width - propertyMargin * 2;
				}

				height += property_.Height;
			}

			bounds_ = new Rectangle( x, y, width, height );
		}
		#endregion

		#region 描画関連
		public void OnPaint( PaintEventArgs pe )
		{
			Graphics g = pe.Graphics;

			Rectangle bRect = ButtonRectangle;

			// ボタンの描画を行う
			if( IsCollapsed )
			{
				if( isPushed || isHover )
				{
					g.DrawImage( collapseImage2, bRect, new Rectangle( 0, 0, bRect.Width, bRect.Height ), GraphicsUnit.Pixel );
				}
				else
				{
					g.DrawImage( collapseImage1, bRect, new Rectangle( 0, 0, bRect.Width, bRect.Height ), GraphicsUnit.Pixel );
				}
			}
			else
			{
				if( isPushed || isHover )
				{
					g.DrawImage( expandImage2, bRect, new Rectangle( 0, 0, bRect.Width, bRect.Height ), GraphicsUnit.Pixel );
				}
				else
				{
					g.DrawImage( expandImage1, bRect, new Rectangle( 0, 0, bRect.Width, bRect.Height ), GraphicsUnit.Pixel );
				}
			}

			// テキストの描画を行う
			using( StringFormat f = new StringFormat( ) )
			{
				f.Alignment = StringAlignment.Near;
				f.LineAlignment = StringAlignment.Center;
				using( Brush b = new SolidBrush( textColor ) )
				{
					Rectangle rc = CaptionRectangle;
					g.DrawString( this.Text, font_, b, new Rectangle( rc.Left + 10, 1 + rc.Top, rc.Width - 20 - 20, rc.Height - 1 ), f );
				}
			}
		}

		public void OnPaintBackground( PaintEventArgs pe )
		{
			Graphics g = pe.Graphics;

			Rectangle cRect = this.ClientRectangle;
			Rectangle rc = this.CaptionRectangle;

			// キャプションの背景を描画する
			using( GraphicsPath path = RoundedRectangle( cRect, captionCurveRadius ) )
			{
				// エッジをスムーズに描画するようにする
				SmoothingMode oldSmoothingMode = g.SmoothingMode;
				PixelOffsetMode oldPixelOffsetMode = g.PixelOffsetMode;
				g.PixelOffsetMode = System.Drawing.Drawing2D.PixelOffsetMode.HighQuality;
				g.SmoothingMode = SmoothingMode.AntiAlias;

				// 角の丸い四角形のパスを塗りつぶす
				using( LinearGradientBrush b = new LinearGradientBrush( rc, leftColor, rightColor, LinearGradientMode.Horizontal ) )
				{
					g.FillPath( b, path );
				}


				g.IntersectClip( new Region( path ) );

				if( !this.IsCollapsed )
				{
					// コントロールを配置する部分の背景を描画する
					Rectangle rect = new Rectangle( cRect.Left, cRect.Top + captionHeight, cRect.Width, cRect.Height - captionHeight );
					using( SolidBrush b = new SolidBrush( backColor ) )
					{
						g.FillRectangle( b, rect );
					}
				}

				g.ResetClip( );

				// 描画モードを元に戻す
				g.SmoothingMode = oldSmoothingMode;
				g.PixelOffsetMode = oldPixelOffsetMode;
			}
		}

		protected GraphicsPath RoundedRectangle( Rectangle rect, int cornerRadius )
		{
			GraphicsPath path = new GraphicsPath( );
			int size = cornerRadius * 2 + 1;
			int x = rect.X;
			int y = rect.Y;
			int w = x + rect.Width;
			int h = y + rect.Height;
			path.FillMode = System.Drawing.Drawing2D.FillMode.Winding;
			path.AddArc( w - size, y, size, size, 270, 90 );
			path.AddArc( w - size, h - size, size, size, 0, 90 );
			path.AddArc( x, h - size, size, size, 90, 90 );
			path.AddArc( x, y, size, size, 180, 90 );
			return ( path );
		}
		#endregion	}

		#region マウス関連
		public virtual void OnMouseDown( MouseEventArgs e )
		{
			if( e.Button == MouseButtons.Left )
			{
				isPushed = ButtonRectangle.Contains( e.X, e.Y );
				if( !isPushed && e.Clicks == 2 && CaptionRectangle.Contains( e.X, e.Y ) )
				{
					// ダブルクリックで閉じる
					IsCollapsed = !IsCollapsed;
				}
				else if( this.CaptionRectangle.Contains( e.Location ) )
				{
					dragPt = e.Location;
					isDrag = true;
				}
			}
			else
			{
				isPushed = false;
			}
		}

		Point dragPt = Point.Empty;
		public virtual void OnMouseUp( MouseEventArgs e )
		{
			if( e.Button == MouseButtons.Left )
			{
				if( isPushed && ButtonRectangle.Contains( e.X, e.Y ) )
				{
					IsCollapsed = !IsCollapsed;
				}

				isDrag = false;
				isPushed = false;
			}
		}

		public virtual void OnMouseMove( MouseEventArgs e )
		{
			bool hover = ButtonRectangle.Contains( e.X, e.Y );
			if( hover != isHover )
			{
				isHover = hover;
				this.Invalidate( ButtonRectangle );
			}
			else if( isDrag )
			{
				Point oldPt = dragPt;
				dragPt = e.Location;
				Rectangle rc = this.Bounds;
				Point pt = this.Location;

				pt.X += dragPt.X - oldPt.X;
				pt.Y += dragPt.Y - oldPt.Y;

				Rectangle invalidRect = Rectangle.Union( this.bounds_, rc );

				this.Location = pt;

				this.Invalidate( invalidRect );
			}
		}

		public virtual void OnMouseEnter( EventArgs e )
		{
		}

		public virtual void OnMouseLeave( EventArgs e )
		{
			isPushed = isHover = false;
			this.Invalidate( this.ButtonRectangle );
		}

		public virtual void OnMouseHover( EventArgs e )
		{
		}

		public virtual void OnMouseWheel( MouseEventArgs e )
		{
		}
		#endregion

		#region ユーティリティー関数
		public void Invalidate( )
		{
			if( parent_ != null )
			{
				parent_.Invalidate( );
			}
		}

		public void Invalidate( Rectangle rect )
		{
			if( parent_ != null )
			{
				parent_.Invalidate( rect );
			}
		}
		#endregion
	}
}
