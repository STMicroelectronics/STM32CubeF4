/**
  ******************************************************************************
  * @file    iis2dlpc.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    17-Juin-2026
  * @brief   This file provides a set of functions needed to manage the IIS2DLPC
  *          MEMS Accelerometer.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "iis2dlpc.h"

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup Components
  * @{
  */ 

/** @addtogroup IIS2DLPC
  * @brief  This file includes the motion sensor driver for IIS2DLPC motion sensor 
  *         devices.
  * @{
  */

/** @defgroup IIS2DLPC_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */

/** @defgroup IIS2DLPC_Private_Defines
  * @{
  */

/**
  * @}
  */

/** @defgroup IIS2DLPC_Private_Macros
  * @{
  */

/**
  * @}
  */ 

/** @defgroup IIS2DLPC_Private_Variables
  * @{
  */
ACCELERO_DrvTypeDef Iis2dlpcDrv =
{
  IIS2DLPC_Init,
  IIS2DLPC_DeInit,
  IIS2DLPC_ReadID,
  IIS2DLPC_RebootCmd,
  IIS2DLPC_Click_IntConfig,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  IIS2DLPC_ReadACC,
};

/**
  * @}
  */

/** @defgroup IIS2DLPC_Private_FunctionPrototypes
  * @{
  */
  
/**
  * @}
  */

/** @defgroup IIS2DLPC_Private_Functions
  * @{
  */

/**
  * @brief  Set IIS2DLPC Initialization.
  * @param  InitStruct: contains mask of different init parameters
  * @retval None
  */
void IIS2DLPC_Init(uint16_t InitStruct)
{
  uint8_t ctrl = 0x00;
  
  /* Configure the low level interface */
  ACCELERO_IO_Init();

  /* Configure MEMS: power mode(ODR) */
  ctrl = (uint8_t) (InitStruct);
  /* Write value to MEMS CTRL_REG4 register */
  ACCELERO_IO_Write(&ctrl, IIS2DLPC_CTRL_REG1_ADDR, 1);

  /* Configure MEMS: full scale and Filter BW */
  ctrl = (uint8_t) (InitStruct >> 8);
  /* Write value to MEMS CTRL_REG2 register */
  ACCELERO_IO_Write(&ctrl, IIS2DLPC_CTRL_REG6_ADDR, 1);
}

/**
  * @brief  IIS2DLPC De-Initialization.
  * @param  None
  * @retval None.
  */
void IIS2DLPC_DeInit(void)
{
  
}

/**
  * @brief  Read IIS2DLPC device ID.
  * @param  None
  * @retval The Device ID (two bytes).
  */
uint8_t IIS2DLPC_ReadID(void)
{
  uint8_t tmp = 0;

  /* Configure the low level interface */
  ACCELERO_IO_Init();

  /* Read WHO_AM_I register */
  ACCELERO_IO_Read(&tmp, IIS2DLPC_WHO_AM_I_ADDR, 1);
  
  /* Return the ID */
  return (uint16_t)tmp;
}

/**
  * @brief  Set IIS2DLPC Interrupt configuration
  * @param  IIS2DLPC_InterruptConfig_TypeDef: pointer to a IIS2DLPC_InterruptConfig_TypeDef 
  *         structure that contains the configuration setting for the IIS2DLPC Interrupt.
  * @retval None
  */
void IIS2DLPC_InterruptConfig(IIS2DLPC_InterruptConfigTypeDef *IIS2DLPC_IntConfigStruct)
{
  uint8_t ctrl = 0x00;

  /* Configure Interrupt Selection , Request and Signal */
  ctrl = (uint8_t)(IIS2DLPC_IntConfigStruct->Interrupt_Request | \
                   IIS2DLPC_IntConfigStruct->Interrupt_Signal);
  /* Write value to MEMS CTRL_REG3 register */
  ACCELERO_IO_Write(&ctrl, IIS2DLPC_CTRL_REG3_ADDR, 1);

  /* Configure Interrupt Selection , Request and Signal */
  ctrl = (uint8_t)(IIS2DLPC_IntConfigStruct->Interrupt_Selection_Enable);
  /* Write value to MEMS CTRL_REG4 register */
  ACCELERO_IO_Write(&ctrl, IIS2DLPC_CTRL_REG4_ADDR, 1);

  /* Enable interrupt */
  ctrl = 0x20;
  /* Write value to MEMS CTRL_REG7 register */
  ACCELERO_IO_Write(&ctrl, IIS2DLPC_CTRL_REG7_ADDR, 1);
}

/**
  * @brief  Set IIS2DLPC for click detection
  * @param  None
  * @retval None
  */
void IIS2DLPC_Click_IntConfig(void)
{
  IIS2DLPC_InterruptConfigTypeDef   IIS2DLPC_InterruptStruct = {0}; 

  ACCELERO_IO_ITConfig();

  /* NOTE: IIS2DLPC only supports Tap/Double-Tap on INT1 pin */
  /* Set IIS2DLPC Interrupt configuration */
  IIS2DLPC_InterruptStruct.Interrupt_Selection_Enable = IIS2DLPC_INTERRUPT_2_ENABLE;
  IIS2DLPC_InterruptStruct.Interrupt_Request = IIS2DLPC_INTERRUPT_REQUEST_LATCHED;
  IIS2DLPC_InterruptStruct.Interrupt_Signal = IIS2DLPC_INTERRUPT_SIGNAL_HIGH;
  IIS2DLPC_InterruptConfig(&IIS2DLPC_InterruptStruct);
}

/**
  * @brief  Change the lowpower mode for IIS2DLPC.
  * @param  LowPowerMode: new state for the lowpower mode.
  *   This parameter can be one of the following values:
  *     @arg IIS2DLPC_LPM_1: 12-bit resolution
  *     @arg IIS2DLPC_LPM_2: 14-bit resolution
  *     @arg IIS2DLPC_LPM_3: 14-bit resolution
  *     @arg IIS2DLPC_LPM_4: 14-bit resolution
  * @retval None
  */
void IIS2DLPC_ODR_LowpowerCmd(uint8_t ODR_LowPowerMode)
{
  uint8_t tmpreg;
  
  /* Read CTRL_REG1 register */
  ACCELERO_IO_Read(&tmpreg, IIS2DLPC_CTRL_REG1_ADDR, 1);
  
  /* Set new low power mode configuration */
  tmpreg &= (uint8_t)~IIS2DLPC_LPM_4;
  tmpreg |= ODR_LowPowerMode;

  /* Write value to MEMS CTRL_REG1 register */
  ACCELERO_IO_Write(&tmpreg, IIS2DLPC_CTRL_REG1_ADDR, 1);
}

/**
  * @brief  Data Rate command. 
  * @param  DataRateValue: Data rate value.
  *   This parameter can be one of the following values:
  *     @arg IIS2DLPC_DATARATE_1_6: 1.6 Hz output data rate 
  *     @arg IIS2DLPC_DATARATE_12_5: 12.5  Hz output data rate
  *     @arg IIS2DLPC_DATARATE_25: 25 Hz output data rate
  *     @arg IIS2DLPC_DATARATE_50: 50 Hz output data rate 
  *     @arg IIS2DLPC_DATARATE_100: 100 Hz output data rate
  *     @arg IIS2DLPC_DATARATE_200: 200 Hz output data rate
  *     @arg IIS2DLPC_DATARATE_400: 400 Hz output data rate 
  *     @arg IIS2DLPC_DATARATE_800: 800 Hz output data rate
  *     @arg IIS2DLPC_DATARATE_1600: 1600 Hz output data rate
  * @retval None
  */
void IIS2DLPC_DataRateCmd(uint8_t DataRateValue)
{
  uint8_t tmpreg;
  
  /* Read CTRL_REG1 register */
  ACCELERO_IO_Read(&tmpreg, IIS2DLPC_CTRL_REG1_ADDR, 1);

  /* Set new data rate configuration from 12.5 to 200Hz */
  tmpreg &= (uint8_t)~IIS2DLPC_DATARATE_BITS; 
  tmpreg |= DataRateValue;

  /* Write value to MEMS CTRL_REG1 register */
  ACCELERO_IO_Write(&tmpreg, IIS2DLPC_CTRL_REG1_ADDR, 1);
}

/**
  * @brief  Change the Full Scale of IIS2DLPC.
  * @param  FS_value: new full scale value. 
  *   This parameter can be one of the following values:
  *     @arg IIS2DLPC_FULLSCALE_2: +-2g
  *     @arg IIS2DLPC_FULLSCALE_4: +-4g
  *     @arg IIS2DLPC_FULLSCALE_8: +-8g
  *     @arg IIS2DLPC_FULLSCALE_16: +-16g
  * @retval None
  */
void IIS2DLPC_FullScaleCmd(uint8_t FS_value)
{
  uint8_t tmpreg;
  
  /* Read CTRL_REG5 register */
  ACCELERO_IO_Read(&tmpreg, IIS2DLPC_CTRL_REG6_ADDR, 1);
  
  /* Set new full scale configuration */
  tmpreg &= (uint8_t)~IIS2DLPC__FULLSCALE_SELECTION;
  tmpreg |= FS_value;

  /* Write value to MEMS CTRL_REG5 register */
  ACCELERO_IO_Write(&tmpreg, IIS2DLPC_CTRL_REG6_ADDR, 1);
}

/**
  * @brief  Reboot memory content of IIS2DLPC.
  * @param  None
  * @retval None
  */
void IIS2DLPC_RebootCmd(void)
{
  uint8_t tmpreg;
  /* Read CTRL_REG2 register */
  ACCELERO_IO_Read(&tmpreg, IIS2DLPC_CTRL_REG2_ADDR, 1);

  /* Enable or Disable the reboot memory */
  tmpreg |= IIS2DLPC_BOOT_FORCED;

  /* Write value to MEMS CTRL_REG2 register */
  ACCELERO_IO_Write(&tmpreg, IIS2DLPC_CTRL_REG2_ADDR, 1);
}

/**
  * @brief  Read IIS2DLPC output register, and calculate the acceleration 
  *         ACC[mg]=SENSITIVITY* (out_h*256+out_l)/16 (12 bit representation).
  * @param  pointer on floating buffer.
  * @retval None
  */
void IIS2DLPC_ReadACC(int16_t *pData)
{
  int8_t buffer[6];
  uint8_t crtl, i = 0x00;
  float sensitivity = IIS2DLPC_SENSITIVITY_0_06G;
  float valueinfloat = 0;
  
  ACCELERO_IO_Read(&crtl, IIS2DLPC_CTRL_REG6_ADDR, 1);
  ACCELERO_IO_Read((uint8_t*)&buffer[0], IIS2DLPC_OUT_X_L_ADDR, 1);
  ACCELERO_IO_Read((uint8_t*)&buffer[1], IIS2DLPC_OUT_X_H_ADDR, 1);
  ACCELERO_IO_Read((uint8_t*)&buffer[2], IIS2DLPC_OUT_Y_L_ADDR, 1);
  ACCELERO_IO_Read((uint8_t*)&buffer[3], IIS2DLPC_OUT_Y_H_ADDR, 1);
  ACCELERO_IO_Read((uint8_t*)&buffer[4], IIS2DLPC_OUT_Z_L_ADDR, 1);
  ACCELERO_IO_Read((uint8_t*)&buffer[5], IIS2DLPC_OUT_Z_H_ADDR, 1);

  switch(crtl & IIS2DLPC__FULLSCALE_SELECTION) 
  {
    /* FS bit = 00 ==> Sensitivity typical value = 0.06milligals/digit */ 
  case IIS2DLPC_FULLSCALE_2:
    sensitivity = IIS2DLPC_SENSITIVITY_0_06G;
    break;
    
    /* FS bit = 01 ==> Sensitivity typical value = 0.12milligals/digit */ 
  case IIS2DLPC_FULLSCALE_4:
    sensitivity = IIS2DLPC_SENSITIVITY_0_12G;
    break;
    
    /* FS bit = 10 ==> Sensitivity typical value = 0.24milligals/digit */ 
  case IIS2DLPC_FULLSCALE_8:
    sensitivity = IIS2DLPC_SENSITIVITY_0_24G;
    break;
    
    /* FS bit = 11 ==> Sensitivity typical value = 0.73milligals/digit */ 
  case IIS2DLPC_FULLSCALE_16:
    sensitivity = IIS2DLPC_SENSITIVITY_0_73G;
    break;
    
  default:
    break;
  }
  
  /* Obtain the mg value for the three axis */
  for (i = 0; i < 3; i++)
  {
    valueinfloat = ((buffer[2*i+1] << 8) + buffer[2*i]) * sensitivity;
    pData[i] = (int16_t)valueinfloat;
  }
}

/**
  * @}
  */ 

/**
  * @}
  */ 
  
/**
  * @}
  */ 

/**
  * @}
  */
