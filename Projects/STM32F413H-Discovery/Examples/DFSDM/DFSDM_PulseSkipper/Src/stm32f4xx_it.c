/**
  ******************************************************************************
  * @file    DFSDM/DFSDM_PulseSkipper/Src/stm32f4xx_it.c
  * @author  MCD Application Team
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
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
#include "stm32f4xx_it.h"

/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup DFSDM_AudioRecord
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern I2S_HandleTypeDef                haudio_i2s;
extern DFSDM_Filter_HandleTypeDef       Dfsdm2Filter1Handle;
extern DFSDM_Filter_HandleTypeDef       Dfsdm2Filter2Handle;
extern DFSDM_Filter_HandleTypeDef       Dfsdm2Filter3Handle;
extern DFSDM_Filter_HandleTypeDef       Dfsdm2Filter4Handle;
extern DFSDM_Filter_HandleTypeDef       Dfsdm1Filter1Handle;
extern TIM_HandleTypeDef                TimDfsdm2Handle;
extern TIM_HandleTypeDef                TimDfsdm1Handle;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  HAL_IncTick();
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles DMA2_Channel8 interrupt request.
  * @param  None
  * @retval None
  */
void DMA2_Stream0_IRQHandler(void)
{
  HAL_DMA_IRQHandler(Dfsdm2Filter1Handle.hdmaReg);
}

/**
  * @brief  This function handles DMA2_Channel8 interrupt request.
  * @param  None
  * @retval None
  */
void DMA2_Stream1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(Dfsdm2Filter2Handle.hdmaReg);
}

/**
  * @brief  This function handles DMA2_Channel8 interrupt request.
  * @param  None
  * @retval None
  */
void DMA2_Stream2_IRQHandler(void)
{
  HAL_DMA_IRQHandler(Dfsdm2Filter3Handle.hdmaReg);
}

/**
  * @brief  This function handles DMA2_Channel8 interrupt request.
  * @param  None
  * @retval None
  */
void DMA2_Stream3_IRQHandler(void)
{
  HAL_DMA_IRQHandler(Dfsdm2Filter4Handle.hdmaReg);
}

/**
  * @brief  This function handles DMA2_Channel3 interrupt request.
  * @param  None
  * @retval None
  */
void DMA2_Stream6_IRQHandler(void)
{
  HAL_DMA_IRQHandler(Dfsdm1Filter1Handle.hdmaReg);
}

/**
  * @brief  This function handles I2S DMA interrupt request.
  * @param  None
  * @retval None
  */
void DMA1_Stream4_IRQHandler(void)
{
  HAL_DMA_IRQHandler(haudio_i2s.hdmatx);
}

/**
* @brief  This function handles TIMER3 interrupt request.
* @param  None
* @retval None
*/
void TIM3_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&TimDfsdm2Handle);
}

/**
* @brief  This function handles TIMER4 interrupt request.
* @param  None
* @retval None
*/
void TIM4_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&TimDfsdm1Handle);
}

/**
  * @}
  */

/**
  * @}
  */
