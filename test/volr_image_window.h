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

// generated by Fast Light User Interface Designer (fluid) version 1.0106

#ifndef volr_image_window_h
#define volr_image_window_h
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Value_Input.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Button.H>

class parameter_window {
public:
  parameter_window();
  Fl_Double_Window *param_window;
  Fl_Value_Input *width;
  Fl_Value_Input *height;
  Fl_Value_Input *depth;
  Fl_Value_Input *sizeX;
  Fl_Value_Input *sizeY;
  Fl_Value_Input *sizeZ;
  Fl_Value_Input *offset;
  Fl_Return_Button *OK;
private:
  inline void cb_OK_i(Fl_Return_Button*, void*);
  static void cb_OK(Fl_Return_Button*, void*);
public:
  Fl_Button *Cancel;
private:
  inline void cb_Cancel_i(Fl_Button*, void*);
  static void cb_Cancel(Fl_Button*, void*);
  int val;
public:
  int value();
  void hide();
  bool show();
};
#include <FL/Fl_Group.H>
#include "volumerender_test.h"
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Progress.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Value_Slider.H>

class volr_image_window {
public:
  volr_image_window( );
  Fl_Double_Window *image_window;
  volr_draw_area *volr_image;
  static Fl_Menu_Item menu_[];
private:
  inline void cb_Open_i(Fl_Menu_*, void*);
  static void cb_Open(Fl_Menu_*, void*);
  inline void cb_Save_i(Fl_Menu_*, void*);
  static void cb_Save(Fl_Menu_*, void*);
  inline void cb_Render_i(Fl_Menu_*, void*);
  static void cb_Render(Fl_Menu_*, void*);
public:
  Fl_Progress *progress_bar;
  Fl_Choice *high_reso;
  static Fl_Menu_Item menu_high_reso[];
private:
  inline void cb_512x512_i(Fl_Menu_*, void*);
  static void cb_512x512(Fl_Menu_*, void*);
  inline void cb_256x256_i(Fl_Menu_*, void*);
  static void cb_256x256(Fl_Menu_*, void*);
  inline void cb_128x128_i(Fl_Menu_*, void*);
  static void cb_128x128(Fl_Menu_*, void*);
public:
  Fl_Choice *low_reso;
  static Fl_Menu_Item menu_low_reso[];
private:
  inline void cb_512x5121_i(Fl_Menu_*, void*);
  static void cb_512x5121(Fl_Menu_*, void*);
  inline void cb_256x2561_i(Fl_Menu_*, void*);
  static void cb_256x2561(Fl_Menu_*, void*);
  inline void cb_128x1281_i(Fl_Menu_*, void*);
  static void cb_128x1281(Fl_Menu_*, void*);
  inline void cb_Specular_i(Fl_Check_Button*, void*);
  static void cb_Specular(Fl_Check_Button*, void*);
  inline void cb_Termination_i(Fl_Value_Slider*, void*);
  static void cb_Termination(Fl_Value_Slider*, void*);
  inline void cb_Sampling_i(Fl_Value_Slider*, void*);
  static void cb_Sampling(Fl_Value_Slider*, void*);
  inline void cb_Light_i(Fl_Value_Slider*, void*);
  static void cb_Light(Fl_Value_Slider*, void*);
  inline void cb_Field_i(Fl_Value_Slider*, void*);
  static void cb_Field(Fl_Value_Slider*, void*);
  inline void cb_OUT_i(Fl_Button*, void*);
  static void cb_OUT(Fl_Button*, void*);
public:
  void show();
};
#endif
