/**
  ******************************************************************************
  * @file    USB_Host/MTP_Standalone/Src/mtp.c 
  * @author  MCD Application Team
  * @brief   This file provides APIs to explorer MTP Storage Objects
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

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
MTP_ObjectHandlesTypedef WavHandles;
uint32_t NumObs = 0;

/* Private function prototypes -----------------------------------------------*/
static uint8_t MTP_GetWavObjectHandles(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Starts Audio streaming.
  * @param  None
  * @retval Returns 0 if OK, otherwise 1.
  */
uint8_t MTP_Init(void)
{
  static uint8_t is_initialized = 0;
  uint8_t ret = 1;
  
  if(is_initialized == 0)
  {
    if(USBH_MTP_IsReady(&hUSBHost) > 0)
    { 
      if(USBH_MTP_GetNumObjects(&hUSBHost, 0, PTP_OFC_WAV, PTP_AT_Undefined, &NumObs) == USBH_OK)
      {
        /* Get objects handlers */
        if(MTP_GetWavObjectHandles() == 0)
        {
          is_initialized = 1;
          ret = 0;  
        }
      }
    }
  }
  else
  {
    ret = 0; 
  }
  return ret;
}

/**
  * @brief  Explores Wav Files. 
  * @param  None
  * @retval Returns 0 if OK, otherwise 1.
  */
uint8_t MTP_ExploreWavFile(void)
{
  uint8_t ret = 1;
  uint32_t index;
  MTP_ObjectInfoTypedef objectinfo;
  
  MTP_Init();
  
  if(USBH_MTP_IsReady(&hUSBHost) > 0)
  {
    LCD_UsrLog("\nAvailable wav files:\n");
 
    /* Get Available WAV files number */
    if((NumObs = MTP_GetWavObjectNumber()) > 0)
    {
      /* Get objects handlers */
      if(MTP_GetWavObjectHandles() == 0)
      {
        ret = 0; 
        
        for (index = 0; index < NumObs; index ++)
        {
          if( USBH_MTP_GetObjectInfo(&hUSBHost, 
                                      WavHandles.Handler[index], 
                                      &objectinfo) == USBH_OK)
            
          {
            LCD_DbgLog(" %lu- %s\n", index, objectinfo.Filename); 
          }
          else
          {
            ret = 1; 
          }
        }
      }
    }
  }
  else
  {
    LCD_ErrLog("MTP Device Not yet ready...\n");
  }
  return ret;
}

/**
  * @brief  Gets Data from MTP Device.
  * @param  file_idx: File index
  * @param  offset: Offset
  * @param  maxbytes: Max bytes
  * @param  object: Pointer to the file object
  * @param  len: Pointer to the file length        
  * @retval Returns Status 0 if OK, otherwise 1.
  */
uint8_t MTP_GetData(uint32_t file_idx, uint32_t offset, uint32_t maxbytes, uint8_t *object, uint32_t *len)
{ 
  USBH_MTP_GetPartialObject(&hUSBHost, 
                            WavHandles.Handler[file_idx], 
                            offset,
                            maxbytes, 
                            object,
                            len);    
  return 0;
}

/**
  * @brief  Gets Wav Object Number.
  * @param  None
  * @retval Number of Wav object files
  */
uint16_t MTP_GetWavObjectNumber(void)
{
  return NumObs;
}

/**
  * @brief  Gets Wav Object Names.
  * @param  None
  * @retval Returns Status 0 if OK, otherwise 1.
  */
uint8_t MTP_GetWavObjectName(uint16_t object_index, uint8_t *filename)
{
  uint8_t ret = 1;
  MTP_ObjectInfoTypedef objectinfo;
  
  if(USBH_MTP_GetObjectInfo(&hUSBHost, WavHandles.Handler[object_index], &objectinfo) == USBH_OK)
  {
    strcpy((char *)filename, (char *)objectinfo.Filename);
    ret = 0;
  }
  return ret;
}

/**
  * @brief  Gets Wav Object Handles.
  * @param  None
  * @retval Returns Status 0 if OK, otherwise 1.
  */
static uint8_t MTP_GetWavObjectHandles(void)
{
  uint8_t ret = 1;
   
  /* Get objects handlers */
  if(USBH_MTP_GetObjectHandles(&hUSBHost, 0, PTP_OFC_WAV, PTP_AT_Undefined, &WavHandles) == USBH_OK)
  {
    ret = 0;
  }
  return ret;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
