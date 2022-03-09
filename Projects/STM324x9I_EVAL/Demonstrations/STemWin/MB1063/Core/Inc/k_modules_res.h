/**
  ******************************************************************************
  * @file    k_modules_res.h
  * @author  MCD Application Team
  * @brief   Header for k_modules_res.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __K_MODULE_RES_H
#define __K_MODULE_RES_H

#ifdef __cplusplus
 extern "C" {
#endif
 
/* Includes ------------------------------------------------------------------*/   
#include <stdlib.h>
#include "GUI.h"

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

/* Exported types ------------------------------------------------------------*/   
extern GUI_CONST_STORAGE GUI_BITMAP bmplay_pressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmplay_not_pressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmnext_not_pressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmnext_pressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmprevious_not_pressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmprevious_pressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmstop_pressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmstop_not_pressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmadd_pressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmadd_not_pressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmplaylist_pressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmplaylist_not_pressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmopen_pressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmopen_not_pressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmclose_not_pressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmclose_pressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmhide_not_pressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmhide_pressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmpause_pressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmpause_not_pressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmspeaker_not_mute;
extern GUI_CONST_STORAGE GUI_BITMAP bmspeaker_mute;
extern GUI_CONST_STORAGE GUI_BITMAP bmrepeat1;
extern GUI_CONST_STORAGE GUI_BITMAP bmrepeat_all;
extern GUI_CONST_STORAGE GUI_BITMAP bmrepeat_off;
extern GUI_CONST_STORAGE GUI_BITMAP bmwindowfullscreen;
extern GUI_CONST_STORAGE GUI_BITMAP bmwindownofullscreen;
extern GUI_CONST_STORAGE GUI_BITMAP bmsettings_button_not_pressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmsettings_button_pressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmstop2_pressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmstop2_not_pressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmstart_pressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmstart_not_pressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmok32;
extern GUI_CONST_STORAGE GUI_BITMAP bmDelete32;

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void DrawRect3D(int x0, int y0, int width, int height);

#ifdef __cplusplus
}
#endif

#endif /*__K_MODULE_RES_H */
