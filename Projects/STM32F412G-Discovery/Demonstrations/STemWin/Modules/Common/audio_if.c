/**
  ******************************************************************************
  * @file    audio_if.c
  * @author  MCD Application Team
  * @brief   Audio common interface
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
  
/* Includes ------------------------------------------------------------------*/

#include "audio_if.h"

/** @addtogroup AUDIO_PLAYER_MODULE
  * @{
  */

/** @defgroup AUDIO_APPLICATION
  * @brief audio application routines
  * @{
  */


/* External variables --------------------------------------------------------*/
static AUDIO_IFTypeDef  AudioIf;
AUDIO_ProcessTypdef haudio;

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

 
 /**
  * @brief  Register Audio callbacks
  * @param  callbacks
  * @retval None
  */
void AUDIO_IF_RegisterCallbacks(pFunc  tc_cb, 
                                pFunc  ht_cb, 
                                pFunc  err_cb)
{
    AudioIf.TransferComplete_CallBack = tc_cb;
    AudioIf.HalfTransfer_CallBack = ht_cb; 
    AudioIf.Error_CallBack = err_cb;        
}
/**
  * @brief  Manages the DMA Transfer complete interrupt.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_OUT_TransferComplete_CallBack(void)
{
  if(AudioIf.TransferComplete_CallBack)
  {
    AudioIf.TransferComplete_CallBack();
  }
}

/**
  * @brief  Manages the DMA Half Transfer complete interrupt.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_OUT_HalfTransfer_CallBack(void)
{ 
  if (AudioIf.HalfTransfer_CallBack )
  {
    AudioIf.HalfTransfer_CallBack();
  }
}

/**
  * @brief  Manages the DMA FIFO error interrupt.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_OUT_Error_CallBack(void)
{
  if(AudioIf.Error_CallBack)
  {
    AudioIf.Error_CallBack();
  }
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
