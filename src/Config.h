#pragma once
#ifndef _d_h_config
#define _d_h_config

#include "Base.h"

//
#define _d_release 0

//
#define _d_enable_log_info 1
#define _d_enable_log_warn 1
#define _d_enable_log_err  1

//
#define _d_log_buffer_length 1024

//
#define _d_app_default_screen_width 800
#define _d_app_default_screen_height 600
#define _d_app_default_sound_volume 64
#define _d_app_window_caption "Night City Blues"
#if _d_os_win
  // Refer to resource.h
  #define _d_app_res_np IDB_PNG7

  #define _d_app_res_nc          IDB_PNG1
  #define _d_app_res_nc_lights_1 IDB_PNG3

  #define _d_app_res_airplane              IDB_PNG2
  #define _d_app_res_airplane_lights_red   IDB_PNG4
  #define _d_app_res_airplane_lights_green IDB_PNG5
  #define _d_app_res_airplane_lights_white IDB_PNG6

  #define _d_app_res_blues IDR_FOO1
#endif

//
#define _d_app_np_x 225
#define _d_app_np_y 491
#define _d_app_np_appear 19000
#define _d_app_np_fade 5000
#define _d_app_np_sleep 5000

//
#define _d_app_nc_lights_1_fade 5000

//
#define _d_app_airplane_landing_speed 0.003
#define _d_app_airplane_landing_from_x 771
#define _d_app_airplane_landing_from_y 225
#define _d_app_airplane_landing_to_x 236
#define _d_app_airplane_landing_to_y 431
#define _d_app_airplane_lights_fade 100
#define _d_app_airplane_lights_sleep 2000

#endif // #ifndef _d_h_config
