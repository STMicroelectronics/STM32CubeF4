/** 
  ******************************************************************************
  * @file    Examples_LL/I2C/I2C_OneBoard_AdvCommunication_DMAAndIT/Src/stm32f4xx_it.c
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
#include "stm32f4xx_it.h"

/** @addtogroup STM32F4xx_LL_Examples
  * @{
  */

/** @addtogroup I2C_OneBoard_AdvCommunication_DMAAndIT
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern __IO uint8_t  ubMasterRequestDirection;
extern __IO uint8_t  ubMasterXferDirection;
extern __IO uint8_t  ubMasterNbDataToReceive;
extern __IO uint8_t  ubSlaveNbDataToTransmit;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
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
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles external line 13 interrupt request.
  * @param  None
  * @retval None
  */
void USER_BUTTON_IRQHANDLER(void)
{
  /* Manage Flags */
  if(LL_EXTI_IsActiveFlag_0_31(USER_BUTTON_EXTI_LINE) != RESET)
  {
    LL_EXTI_ClearFlag_0_31(USER_BUTTON_EXTI_LINE);

    /* Manage code in main.c.*/
    UserButton_Callback(); 
  }
}

/**
  * Brief   This function handles I2C1 (Slave) event interrupt request.
  * Param   None
  * Retval  None
  */
void I2C1_EV_IRQHandler(void)
{
  /* Check ADDR flag value in ISR register */
  if(LL_I2C_IsActiveFlag_ADDR(I2C1))
  {
    /* Verify the slave transfer direction, a read direction, Slave enters receiver mode */
    if(LL_I2C_GetTransferDirection(I2C1) == LL_I2C_DIRECTION_READ)
    {
      /* Enable Buffer Interrupts */
      LL_I2C_EnableIT_BUF(I2C1);

      /* Clear ADDR flag value in ISR register */
      LL_I2C_ClearFlag_ADDR(I2C1);
    }
    else if(LL_I2C_GetTransferDirection(I2C1) == LL_I2C_DIRECTION_WRITE)
    {
      /* Enable Buffer Interrupts */
      LL_I2C_EnableIT_BUF(I2C1);

      /* Clear ADDR flag value in ISR register */
      LL_I2C_ClearFlag_ADDR(I2C1);
    }
  }
  /* Check RXNE flag value in ISR register */
  else if(LL_I2C_IsActiveFlag_RXNE(I2C1))
  {
    /* Call function Slave Reception Callback */
    Slave_Reception_Callback();
  }
  /* Check TXE flag value in ISR register */
  else if(LL_I2C_IsActiveFlag_TXE(I2C1))
  {
    /* Call function Slave Ready to Transmit Callback */
    Slave_Ready_To_Transmit_Callback();
  }
  /* Check BTF flag value in ISR register */
  else if(LL_I2C_IsActiveFlag_BTF(I2C1))
  {
    if(LL_I2C_GetTransferDirection(I2C1) == LL_I2C_DIRECTION_WRITE)
    {
      /* Send the next byte */
      /* Call function Slave Ready to Transmit Callback */
      Slave_Ready_To_Transmit_Callback();
    }
    else
    {
      /* Call function Slave Reception Callback */
      Slave_Reception_Callback();
    }
  }
  /* Check STOP flag value in ISR register */
  else if(LL_I2C_IsActiveFlag_STOP(I2C1))
  {
    /* Clear STOP flag value in ISR register */
    LL_I2C_ClearFlag_STOP(I2C1);
    
    /* Call function Slave Complete Callback */
    Slave_Complete_Callback();
  }
}

/**
  * Brief   This function handles I2C1 (Slave) error interrupt request.
  * Param   None
  * Retval  None
  */
void I2C1_ER_IRQHandler(void)
{
  /* Normal use case, if all bytes are sent and Acknowledge failure appears */
  /* This correspond to the end of communication */
  if((ubSlaveNbDataToTransmit == 0) && \
     (LL_I2C_IsActiveFlag_AF(I2C1)) && \
     (LL_I2C_GetTransferDirection(I2C1) == LL_I2C_DIRECTION_WRITE))
  {
    /* Clear AF flag value in ISR register */
    LL_I2C_ClearFlag_AF(I2C1);

    /* Call function Slave Complete Callback */
    Slave_Complete_Callback();
  }
  else
  {
    /* Call Error function */
    Error_Callback();
  }
}

/**
  * Brief   This function handles I2C3 (Master) interrupt request.
  * Param   None
  * Retval  None
  */
void I2C3_EV_IRQHandler(void)
{
  /* Check SB flag value in ISR register */
  if(LL_I2C_IsActiveFlag_SB(I2C3))
  {
    /* Send Slave address with a 7-Bit SLAVE_OWN_ADDRESS for a ubMasterRequestDirection request */
    LL_I2C_TransmitData8(I2C3, SLAVE_OWN_ADDRESS | ubMasterRequestDirection);
  }
  /* Check ADDR flag value in ISR register */
  else if(LL_I2C_IsActiveFlag_ADDR(I2C3))
  {
    /* Verify the transfer direction */
    if(LL_I2C_GetTransferDirection(I2C3) == LL_I2C_DIRECTION_READ)
    {
      ubMasterXferDirection = LL_I2C_DIRECTION_READ;

      if(ubMasterNbDataToReceive == 1)
      {
        /* Prepare the generation of a Non ACKnowledge condition after next received byte */
        LL_I2C_AcknowledgeNextData(I2C3, LL_I2C_NACK);

        /* Enable DMA transmission requests */
        LL_I2C_EnableDMAReq_RX(I2C3);
      }
      else if(ubMasterNbDataToReceive == 2)
      {
        /* Prepare the generation of a Non ACKnowledge condition after next received byte */
        LL_I2C_AcknowledgeNextData(I2C3, LL_I2C_NACK);

        /* Enable Pos */
        LL_I2C_EnableBitPOS(I2C3);
      }
      else
      {
        /* Enable Last DMA bit */
        LL_I2C_EnableLastDMA(I2C3);

        /* Enable DMA transmission requests */
        LL_I2C_EnableDMAReq_RX(I2C3);
      }
    }
    else
    {
      ubMasterXferDirection = LL_I2C_DIRECTION_WRITE;

      /* Enable DMA transmission requests */
      LL_I2C_EnableDMAReq_TX(I2C3);
    }

    /* Clear ADDR flag value in ISR register */
    LL_I2C_ClearFlag_ADDR(I2C3);
  }
}

/**
  * Brief   This function handles I2C3 (Master) error interrupt request.
  * Param   None
  * Retval  None
  */
void I2C3_ER_IRQHandler(void)
{
  /* Call Error function */
  Error_Callback();
}

/**
  * @brief  This function handles DMA1_Stream5 interrupt request.
  * @param  None
  * @retval None
  */
void DMA1_Stream5_IRQHandler(void)
{
  if(LL_DMA_IsActiveFlag_TC5(DMA1))
  {
    LL_DMA_ClearFlag_TC5(DMA1);

    Transfer_Complete_Callback();
  }
  else if(LL_DMA_IsActiveFlag_TE5(DMA1))
  {
    Transfer_Error_Callback();
  }
}

/**
  * @brief  This function handles DMA1_Stream2 interrupt request.
  * @param  None
  * @retval None
  */
void DMA1_Stream2_IRQHandler(void)
{
  if(LL_DMA_IsActiveFlag_TC2(DMA1))
  {
    LL_DMA_ClearFlag_TC2(DMA1);
    Transfer_Complete_Callback();
  }
  else if(LL_DMA_IsActiveFlag_TE2(DMA1))
  {
    Transfer_Error_Callback();
  }
}
/**
  * @}
  */

/**
  * @}
  */
