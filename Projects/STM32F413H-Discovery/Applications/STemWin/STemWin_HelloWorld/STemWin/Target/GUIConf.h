/**
  ******************************************************************************
  * @file    GUIConf.h
  * @author  MCD Application Team
  * @brief   Display driver configuration file
  ******************************************************************************
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */ 
#ifndef GUICONF_H
#define GUICONF_H

/*********************************************************************
*
*       Multi layer/display support
*/
#define GUI_NUM_LAYERS            2    // Maximum number of available layers

/*********************************************************************
*
*       Multi tasking support
*/
#ifdef OS_SUPPORT
 #define GUI_OS                    (1)  // Compile with multitasking support
#else
 #define GUI_OS                    (0)
#endif

/*********************************************************************
*
*       Configuration of touch support
*/
#ifndef   GUI_SUPPORT_TOUCH
  #define GUI_SUPPORT_TOUCH       (1)  // Support touchscreen
#endif

/*********************************************************************
*
*       Default font
*/
#define GUI_DEFAULT_FONT          &GUI_Font6x8

/*********************************************************************
*
*         Configuration of available packages
*/
#define GUI_SUPPORT_MOUSE             (1)    /* Support a mouse */
#define GUI_WINSUPPORT                (1)    /* Use window manager */
#define GUI_SUPPORT_MEMDEV            (1)    /* Memory device package available */
#define GUI_SUPPORT_DEVICES           (1)    /* Enable use of device pointers */
#define GUI_USE_ARGB                  (0)    /* The color format to use is ARBG */

#endif  /* Avoid multiple inclusion */
