/**
  ******************************************************************************
  * @file    USB_Host/DynamicSwitch_Standalone/Src/msc_explorer.c
  * @author  MCD Application Team
  * @brief   Explore the USB flash disk content
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics International N.V.
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

/* Includes ----------------------------------------------------------------- */
#include "main.h"

/* Private typedef ---------------------------------------------------------- */
/* Private define ----------------------------------------------------------- */
/* Private macro ------------------------------------------------------------ */
/* Private variables -------------------------------------------------------- */
static int32_t recurseLevel = -1;
/* Private function prototypes ---------------------------------------------- */
/* Private functions -------------------------------------------------------- */

/**
  * @brief  Displays disk content.
  * @param  path: Pointer to root path
  * @param  recu_level: Disk content level
  * @retval Operation result
  */
FRESULT Explore_Disk(char *path, uint8_t recu_level)
{
  FRESULT res = FR_OK;
  FILINFO fno;
  DIR dir;
  char tmp[14];
  uint8_t line_idx = 0;

  recurseLevel++;
  res = f_opendir(&dir, path);
  if (res == FR_OK)
  {
    while (USBH_MSC_IsReady(&hUSBHost))
    {
      res = f_readdir(&dir, &fno);
      if (res != FR_OK || fno.fname[0] == 0)
      {
        break;
      }
      if (fno.fname[0] == '.')
      {
        continue;
      }

      strcpy(tmp, fno.fname);

      line_idx++;
      if (line_idx > YWINDOW_SIZE)
      {
        line_idx = 0;
        LCD_UsrLog("> Press [KEY] To Continue.\n");

        /* KEY Button in polling */
        while ((BSP_PB_GetState(BUTTON_KEY) != SET) &&
               (Appli_state != APPLICATION_DISCONNECT))
        {
          /* Wait for User Input */
        }
      }

      if (recu_level == 1)
      {
        LCD_DbgLog("   |__");
      }
      else if (recu_level == 2)
      {
        LCD_DbgLog("   |   |__");
      }
      if (fno.fattrib & AM_DIR)
      {
        strcat(tmp, "\n");
        LCD_UsrLog((void *)tmp);
        Explore_Disk(fno.fname, 2);
      }
      else
      {
        strcat(tmp, "\n");
        LCD_DbgLog((void *)tmp);
      }

      if ((fno.fattrib & AM_DIR) && (recu_level == 2))
      {
        Explore_Disk(fno.fname, 2);
      }
    }
    f_closedir(&dir);
  }

  if (--recurseLevel == -1)
  {
    LCD_UsrLog("> Select an operation to Continue.\n");
  }

  return res;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
