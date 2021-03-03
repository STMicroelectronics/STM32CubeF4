/**
  ******************************************************************************
  * @file    GUIConf_stm32f413h_discovery.c.c
  * @author  MCD Application Team
  * @brief   Display controller initialization
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

/*************************** End of file ****************************/
