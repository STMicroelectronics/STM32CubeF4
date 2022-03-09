/**
  ******************************************************************************
  * @file    USB_Host/DualCore_Standalone/Src/msc_menu.c
  * @author  MCD Application Team
  * @brief   Mass Stoarge Process
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

uint8_t *MSC_main_menu[] =
{
  (uint8_t *)"      1 - File Operations                                                   ",
  (uint8_t *)"      2 - Explorer Disk                                                     ",
  (uint8_t *)"      3 - Return                                                            ",
};

/* Private function prototypes -----------------------------------------------*/
extern DEMO_StateMachine demo;

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Manages MSC Menu Process.
  * @param  None
  * @retval None
  */
void MSC_MenuProcess(void)
{
  switch(demo.msc_state)
  {
  case APPLI_MSC_IDLE:
    Demo_SelectItem(MSC_main_menu, 0);
    demo.msc_state = APPLI_MSC_WAIT;
    demo.select = 0;
    break;

  case APPLI_MSC_WAIT:

    if(demo.select != prev_select)
    {
      prev_select = demo.select;
      Demo_SelectItem(MSC_main_menu, demo.select & 0x7F);

      /* Handle select item */
      if(demo.select & 0x80)
      {
        demo.select &= 0x7F;

        switch(demo.select)
        {
        case 0:
          demo.msc_state = APPLI_MSC_FILE_OPERATIONS;
          break;

        case 1:
          demo.msc_state = APPLI_MSC_EXPLORER;
          break;

        case 2: /* Return */
          demo.state = DEMO_IDLE;
          demo.select = 0;
          LCD_UsrLogY("> MSC application closed.\n");
          f_mount(0,0,0);
          break;

        default:
          break;
        }
      }
    }
    break;

  case APPLI_MSC_FILE_OPERATIONS:

    /* Read and Write File Here */
    if(Appli_HS_state == APPLICATION_HS_READY)
    {
      MSC_File_Operations();
    }
    demo.msc_state = APPLI_MSC_WAIT;
    break;

  case APPLI_MSC_EXPLORER:
    /* Display disk content */
    if(Appli_HS_state == APPLICATION_HS_READY)
    {
      Explore_Disk("0:/", 1);
    }
    demo.msc_state = APPLI_MSC_WAIT;
    break;

  default:
    break;
  }
}
