/**
  ******************************************************************************
  * @file    GUIConf_stm32f413h_discovery.c.c
  * @author  MCD Application Team
  * @brief   Display controller initialization
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
#include "GUI.h"

/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/

//
// Define the available number of bytes available for the GUI
//
#define GUI_NUMBYTES  (1024) *  110   // x KByte

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

/* 32 bit aligned memory area */
static U32 extMem[GUI_NUMBYTES / 4];

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI_X_Config
*
* Purpose:
*   Called during the initialization process in order to set up the
*   available memory for the GUI.
*/
void GUI_X_Config(void)
{

  GUI_ALLOC_AssignMemory(extMem, GUI_NUMBYTES);

}
