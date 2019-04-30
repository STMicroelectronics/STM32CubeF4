/**
  ******************************************************************************
  * @file    USB_Host/DualCore_Standalone/Src/msc_menu.c
  * @author  MCD Application Team
  * @brief   Mass Stoarge Process
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V.
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license.
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
