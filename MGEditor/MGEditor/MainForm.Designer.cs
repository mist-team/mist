namespace MGEditor
{
	partial class MainForm
	{
		/// <summary>
		/// 必要なデザイナ変数です。
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// 使用中のリソースをすべてクリーンアップします。
		/// </summary>
		/// <param name="disposing">マネージ リソースが破棄される場合 true、破棄されない場合は false です。</param>
		protected override void Dispose( bool disposing )
		{
			if( disposing && ( components != null ) )
			{
				components.Dispose( );
			}
			base.Dispose( disposing );
		}

		#region Windows フォーム デザイナで生成されたコード

		/// <summary>
		/// デザイナ サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディタで変更しないでください。
		/// </summary>
		private void InitializeComponent( )
		{
			this.mgCanvas1 = new MGEditor.MGCanvas( );
			this.SuspendLayout( );
			// 
			// mgCanvas1
			// 
			this.mgCanvas1.BackColor = System.Drawing.Color.WhiteSmoke;
			this.mgCanvas1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.mgCanvas1.Location = new System.Drawing.Point( 0, 0 );
			this.mgCanvas1.Name = "mgCanvas1";
			this.mgCanvas1.Size = new System.Drawing.Size( 613, 535 );
			this.mgCanvas1.TabIndex = 0;
			this.mgCanvas1.Text = "mgCanvas1";
			// 
			// MainForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF( 6F, 12F );
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size( 613, 535 );
			this.Controls.Add( this.mgCanvas1 );
			this.Name = "MainForm";
			this.Text = "MGEdit";
			this.ResumeLayout( false );

		}

		#endregion

		private MGCanvas mgCanvas1;
	}
}

