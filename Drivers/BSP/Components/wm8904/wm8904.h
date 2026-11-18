/**
  ******************************************************************************
  * @file    wm8904.h
  * @author  MCD Application Team
  * @brief   This file contains all the functions prototypes for the
  *          wm8904.c driver.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef WM8904_H
#define WM8904_H

/* Includes ------------------------------------------------------------------*/
#include "../Common/audio.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Component
  * @{
  */

/** @addtogroup WM8904
  * @{
  */

/** @defgroup WM8904_Exported_Types Exported
  * @{
  */

/**
  * @}
  */

/** @defgroup WM8904_Exported_Constants
  * @{
  */
/******************************************************************************/
/****************************** REGISTER MAPPING ******************************/
/******************************************************************************/
/* SW Reset */
#define WM8904_SW_RESET               0x0000U

/* Bias Control */
#define WM8904_BIAS_CONTROL0          0x0004U
#define WM8904_VMID_CONTROL0          0x0005U
#define WM8904_MIC_BIAS_CONTROL0      0x0006U
#define WM8904_MIC_BIAS_CONTROL1      0x0007U

#define WM8904_ANALOG_ADC0            0x000AU

/* Power Management */
#define WM8904_PWR_MANAGEMENT0        0x000CU
#define WM8904_PWR_MANAGEMENT2        0x000EU
#define WM8904_PWR_MANAGEMENT3        0x000FU
#define WM8904_PWR_MANAGEMENT6        0x0012U

/* Clock Rates */
#define WM8904_CLOCK_RATES0           0x0014U
#define WM8904_CLOCK_RATES1           0x0015U
#define WM8904_CLOCK_RATES2           0x0016U

/* Audio Interface */
#define WM8904_AUDIO_INTERFACE0       0x0018U
#define WM8904_AUDIO_INTERFACE1       0x0019U
#define WM8904_AUDIO_INTERFACE2       0x001AU
#define WM8904_AUDIO_INTERFACE3       0x001BU

/* DAC */
#define WM8904_DAC_DIGITAL_VOL_LEFT   0x001EU
#define WM8904_DAC_DIGITAL_VOL_RIGHT  0x001FU
#define WM8904_DAC_DIGITAL0           0x0020U
#define WM8904_DAC_DIGITAL1           0x0021U

/* ADC */
#define WM8904_ADC_DIGITAL_VOL_LEFT   0x0024U
#define WM8904_ADC_DIGITAL_VOL_RIGHT  0x0025U
#define WM8904_ADC_DIGITAL0           0x0026U

#define WM8904_DIGITAL_MICROPHONE0    0x0027U

/* DRC */
#define WM8904_DRC0                   0x0028U
#define WM8904_DRC1                   0x0029U
#define WM8904_DRC2                   0x002AU
#define WM8904_DRC3                   0x002BU

/* Analog */
#define WM8904_ANALOG_LEFT_INPUT0     0x002CU
#define WM8904_ANALOG_RIGHT_INPUT0    0x002DU
#define WM8904_ANALOG_LEFT_INPUT1     0x002EU
#define WM8904_ANALOG_RIGHT_INPUT1    0x002FU
#define WM8904_ANALOG_OUTPUT1_LEFT    0x0039U
#define WM8904_ANALOG_OUTPUT1_RIGHT   0x003AU
#define WM8904_ANALOG_OUTPUT2_LEFT    0x003BU
#define WM8904_ANALOG_OUTPUT2_RIGHT   0x003CU
#define WM8904_ANALOG_OUTPUT12_ZC     0x003DU

/* DC Servo */
#define WM8904_DC_SERVO0              0x0043U
#define WM8904_DC_SERVO1              0x0044U
#define WM8904_DC_SERVO2              0x0045U
#define WM8904_DC_SERVO4              0x0047U
#define WM8904_DC_SERVO5              0x0048U
#define WM8904_DC_SERVO6              0x0049U
#define WM8904_DC_SERVO7              0x004AU
#define WM8904_DC_SERVO8              0x004BU
#define WM8904_DC_SERVO9              0x004CU
#define WM8904_DC_SERVO_READBACK0     0x004DU

/* Analog */
#define WM8904_ANALOG_HP0             0x005AU
#define WM8904_ANALOG_LINEOUT0        0x005EU

/* Charge pump */
#define WM8904_CHARGE_PUMP0           0x0062U

/* Class W/G */
#define WM8904_CLASS_W0               0x0068U

/* Write Sequencer */
#define WM8904_WRITE_SEQUENCER0       0x006CU
#define WM8904_WRITE_SEQUENCER1       0x006DU
#define WM8904_WRITE_SEQUENCER2       0x006EU
#define WM8904_WRITE_SEQUENCER3       0x006FU
#define WM8904_WRITE_SEQUENCER4       0x0070U

/* FLL Control */
#define WM8904_FLL_CONTROL1           0x0074U
#define WM8904_FLL_CONTROL2           0x0075U
#define WM8904_FLL_CONTROL3           0x0076U
#define WM8904_FLL_CONTROL4           0x0077U
#define WM8904_FLL_CONTROL5           0x0078U

/* GPIO Control */
#define WM8904_GPIO_CONTROL1          0x0079U
#define WM8904_GPIO_CONTROL2          0x007AU
#define WM8904_GPIO_CONTROL3          0x007BU
#define WM8904_GPIO_CONTROL4          0x007CU

#define WM8904_DIGITAL_PULLS          0x007EU

/* Interrupt */
#define WM8904_INTERRUPT_STATUS       0x007FU
#define WM8904_INTERRUPT_STATUS_MASK  0x0080U
#define WM8904_INTERRUPT_POLARITY     0x0081U
#define WM8904_INTERRUPT_DEBOUNCE     0x0082U

/* EQ */
#define WM8904_EQ1                    0x0086U
#define WM8904_EQ2                    0x0087U
#define WM8904_EQ3                    0x0088U
#define WM8904_EQ4                    0x0089U
#define WM8904_EQ5                    0x008AU
#define WM8904_EQ6                    0x008BU
#define WM8904_EQ7                    0x008CU
#define WM8904_EQ8                    0x008DU
#define WM8904_EQ9                    0x008EU
#define WM8904_EQ10                   0x008FU
#define WM8904_EQ11                   0x0090U
#define WM8904_EQ12                   0x0091U
#define WM8904_EQ13                   0x0092U
#define WM8904_EQ14                   0x0093U
#define WM8904_EQ15                   0x0094U
#define WM8904_EQ16                   0x0095U
#define WM8904_EQ17                   0x0096U
#define WM8904_EQ18                   0x0097U
#define WM8904_EQ19                   0x0098U
#define WM8904_EQ20                   0x0099U
#define WM8904_EQ21                   0x009AU
#define WM8904_EQ22                   0x009BU
#define WM8904_EQ23                   0x009CU
#define WM8904_EQ24                   0x009DU

#define WM8904_ADC_TEST0              0x00C6U

#define WM8904_FLL_TEST0              0x00F7U
#define WM8904_FLL_TEST1              0x00F8U

/**
  * @}
  */

/******************************************************************************/
/***************************  Codec User defines ******************************/
/******************************************************************************/
/* Audio Input Device */
#define WM8904_IN_NONE           0x0000U
#define WM8904_IN_MIC1           0x0100U
#define WM8904_IN_MIC2           0x0200U
#define WM8904_IN_MIC1_MIC2      0x0300U
#define WM8904_IN_LINE1          0x0400U
#define WM8904_IN_LINE2          0x0800U
#define WM8904_IN_DIGITAL_MIC1   0x1000U
#define WM8904_IN_DIGITAL_MIC2   0x2000U

/* Audio Output Device */
#define WM8904_OUT_NONE          0x0000U
#define WM8904_OUT_HEADPHONE     0x0002U
#define WM8904_OUT_BOTH          0x0003U
#define WM8904_OUT_LINE          0x0004U

#define OUTPUT_DEVICE_AUTO       WM8904_OUT_HEADPHONE

/* AUDIO FREQUENCY */
#define WM8904_FREQUENCY_48K     48000U
#define WM8904_FREQUENCY_44K     44100U
#define WM8904_FREQUENCY_32K     32000U
#define WM8904_FREQUENCY_24K     24000U
#define WM8904_FREQUENCY_22K     22050U
#define WM8904_FREQUENCY_16K     16000U
#define WM8904_FREQUENCY_12K     12000U
#define WM8904_FREQUENCY_11K     11025U
#define WM8904_FREQUENCY_8K      8000U

/* AUDIO RESOLUTION */
#define WM8904_RESOLUTION_16B    0x00U
#define WM8904_RESOLUTION_20B    0x01U
#define WM8904_RESOLUTION_24B    0x02U
#define WM8904_RESOLUTION_32B    0x03U

/* Codec stop options */
#define WM8904_PDWN_HW           0x00U
#define WM8904_PDWN_SW           0x01U

#define CODEC_PDWN_HW            WM8904_PDWN_HW
#define CODEC_PDWN_SW            WM8904_PDWN_SW

/* Volume Input Output selection */
#define VOLUME_INPUT                  0U
#define VOLUME_OUTPUT                 1U

/* MUTE commands */
#define WM8904_MUTE_ON                1U
#define WM8904_MUTE_OFF               0U


/* AUDIO PROTOCOL */
#define WM8904_PROTOCOL_R_JUSTIFIED    0x0000U
#define WM8904_PROTOCOL_L_JUSTIFIED    0x0001U
#define WM8904_PROTOCOL_I2S            0x0002U
#define WM8904_PROTOCOL_DSP            0x0003U

/******************************************************************************/
/****************************** REGISTER MAPPING ******************************/
/******************************************************************************/
/**
  * @brief  WM8904 ID
  */
#define  WM8904_ID      0x8904U

/**
  * @brief Device ID Register: Reading from this register will indicate device 
  *                            family ID WM8904
  */
#define WM8904_CHIPID_ADDR     0x00U


/**
  * @}
  */

/** @addtogroup WM8904_Exported_Variables
  * @{
  */

/**
  * @}
  */

/** @defgroup WM8994_Exported_Macros
  * @{
  */ 

/** @addtogroup WM8904_Exported_Functions
  * @{
  */
/*------------------------------------------------------------------------------
                           Audio Codec functions
------------------------------------------------------------------------------*/
/* High Layer codec functions */
uint32_t   WM8904_Init(uint16_t DeviceAddr, uint16_t OutputInputDevice, uint8_t Volume, uint32_t AudioFreq);
void       WM8904_DeInit(void);
uint32_t   WM8904_ReadID(uint16_t DeviceAddr);
uint32_t   WM8904_Play(uint16_t DeviceAddr, uint16_t* pBuffer, uint16_t Size);
uint32_t   WM8904_Pause(uint16_t DeviceAddr);
uint32_t   WM8904_Resume(uint16_t DeviceAddr);
uint32_t   WM8904_Stop(uint16_t DeviceAddr, uint32_t Cmd);
uint32_t   WM8904_SetVolume(uint16_t DeviceAddr, uint8_t Volume);
uint32_t   WM8904_SetMute(uint16_t DeviceAddr, uint32_t Cmd);
uint32_t   WM8904_SetOutputMode(uint16_t DeviceAddr, uint8_t Output);
uint32_t   WM8904_SetFrequency(uint16_t DeviceAddr, uint32_t AudioFreq);
uint32_t   WM8904_Reset(uint16_t DeviceAddr);

/* AUDIO IO functions */
void      AUDIO_IO_Init(void);
void      AUDIO_IO_DeInit(void);
void      AUDIO_IO_Write_16(uint8_t Addr, uint16_t Reg, uint16_t Value);
uint16_t  AUDIO_IO_Read_16(uint8_t Addr, uint16_t Reg);
void      AUDIO_IO_Delay(uint32_t Delay);

/* Audio driver structure */
extern AUDIO_DrvTypeDef   wm8904_drv;

/**
  * @}
  */

#endif /* WM8904_H */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
