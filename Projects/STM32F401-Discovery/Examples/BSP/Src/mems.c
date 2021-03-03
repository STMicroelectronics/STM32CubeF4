/**
  ******************************************************************************
  * @file    BSP/Src/mems.c 
  * @author  MCD Application Team
  * @brief   This example code shows how to use MEMS features.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "mems.h"

/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern __IO uint8_t UserPressButton;

/* Init af threshold to detect acceleration on MEMS */
int16_t ThresholdHigh = 1000;
int16_t ThresholdLow = -1000;

/* Private function prototypes -----------------------------------------------*/
static void ACCELERO_ReadAcc(void);
static void GYRO_ReadAng(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Test ACCELERATOR MEMS Hardware.
  *         The main objective of this test is to check acceleration on 2 axes X and Y
  * @param  None
  * @retval None
  */
void ACCELERO_MEMS_Test(void)
{
  /* Init Accelerometer MEMS */
  if(BSP_ACCELERO_Init() != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }
  
  UserPressButton = 0;
  while(!UserPressButton)
  {
    ACCELERO_ReadAcc();
  }
}  

/**
  * @brief  Read Acceleration data.
  * @param  None
  * @retval None
  */
static void ACCELERO_ReadAcc(void)
{
  int16_t buffer[3] = {0};
  int16_t xval, yval = 0x00;
  
  /* Read Acceleration */
  BSP_ACCELERO_GetXYZ(buffer);

  xval = buffer[0];
  yval = buffer[1];

  if((ABS(xval))>(ABS(yval)))
  {
    if(xval > ThresholdHigh)
    { 
      /* LED5 On */
      BSP_LED_On(LED5);
      HAL_Delay(10);
    }
    else if(xval < ThresholdLow)
    { 
      /* LED4 On */
      BSP_LED_On(LED4);      
      HAL_Delay(10);
    }
    else
    { 
      HAL_Delay(10);
    }
  }
  else
  {
    if(yval < ThresholdLow)
    {
      /* LED6 On */
      BSP_LED_On(LED6);
      HAL_Delay(10);
    }
    else if(yval > ThresholdHigh)
    {
      /* LED3 On */
      BSP_LED_On(LED3);
      HAL_Delay(10);
    } 
    else
    { 
      HAL_Delay(10);
    }
  } 
  
  BSP_LED_Off(LED3);
  BSP_LED_Off(LED4);
  BSP_LED_Off(LED5);
  BSP_LED_Off(LED6);
}

/**
  * @brief  Test Gyroscope MEMS Hardware.
  *         The main objectif of this test is to check the hardware connection of the 
  *         MEMS peripheral.
  * @param  None
  * @retval None
  */
void GYRO_MEMS_Test(void)
{
  /* Init Gyroscope MEMS */
  if(BSP_ACCELERO_Init() != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }
  
  UserPressButton = 0;
  while(!UserPressButton)
  {
    GYRO_ReadAng();
  }
}  

/**
  * @brief  Read Gyroscope Angular data.
  * @param  None
  * @retval None
  */
static void GYRO_ReadAng(void)
{
  /* Gyroscope variables */
  float Buffer[3];
  float Xval, Yval = 0x00;
  
  /* Init Gyroscope Mems */
  if(BSP_GYRO_Init() != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }
  
  /* Read Gyroscope Angular data */
  BSP_GYRO_GetXYZ(Buffer);
  
  Xval = ABS((Buffer[0]));
  Yval = ABS((Buffer[1])); 
  
  if(Xval>Yval)
  {
    if(Buffer[0] > 5000.0f)
    { 
      /* LED5 On */
      BSP_LED_On(LED5);
      HAL_Delay(10);
    }
    else if(Buffer[0] < -5000.0f)
    { 
      /* LED4 On */
      BSP_LED_On(LED4);
      HAL_Delay(10);
    }
    else
    { 
      HAL_Delay(10);
    }
  }
  else
  {
    if(Buffer[1] < -5000.0f)
    {
      /* LED6 On */
      BSP_LED_On(LED6);
      
      HAL_Delay(10);
    }
    else if(Buffer[1] > 5000.0f)
    {
      /* LED3 On */
      BSP_LED_On(LED3);
      HAL_Delay(10);
    } 
    else
    { 
      HAL_Delay(10);
    }
  } 
  
  BSP_LED_Off(LED3);
  BSP_LED_Off(LED4);
  BSP_LED_Off(LED5);
  BSP_LED_Off(LED6);
}

/**
  * @}
  */ 

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
