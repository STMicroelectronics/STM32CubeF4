/**
  ******************************************************************************
  * @file    wm8904.c
  * @author  MCD Application Team
  * @brief   This file provides the WM8904 Audio Codec driver.
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
#include "wm8904.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @defgroup WM8904
  * @brief     This file provides a set of functions needed to drive the
  *            WM8904 audio codec.
  * @{
  */

/** @defgroup WM8994_Private_Types
  * @{
  */

/**
  * @}
  */ 
  
/** @defgroup WM8994_Private_Defines
  * @{
  */
/* Uncomment this line to enable verifying data sent to codec after each write 
   operation (for debug purpose) */
#if !defined (VERIFY_WRITTENDATA)  
  //#define VERIFY_WRITTENDATA
#endif /* VERIFY_WRITTENDATA */
/**
  * @}
  */ 

/** @defgroup WM8904_Private_Macros
  * @{
  */

/**
  * @}
  */ 
  
/** @defgroup WM8904_Private_Variables
  * @{
  */

/* Audio codec driver structure initialization */
AUDIO_DrvTypeDef wm8904_drv =
{
  WM8904_Init,
  WM8904_DeInit,
  WM8904_ReadID,
  WM8904_Play,
  WM8904_Pause,
  WM8904_Resume,
  WM8904_Stop,
  WM8904_SetFrequency,
  WM8904_SetVolume,
  WM8904_SetMute,
  WM8904_SetOutputMode,
  WM8904_Reset
};
/**
  * @}
  */

/** @defgroup WM8904_Private_Macros Private Macros
  * @{
  */
#define VOLUME_OUT_CONVERT(Volume)     (((Volume) >= 100U) ? 63U : ((uint8_t)((((Volume) / 2U) + 13U))))
#define VOLUME_OUT_INVERT(Volume)      ((uint8_t)(((Volume) - 13U) * 2U))
/**
  * @}
  */

/** @defgroup WM8904_Private_Variables Private Variables
  * @{
  */
/**
  * @}
  */

/** @defgroup WM8904_Function_Prototypes
  * @{
  */
static uint8_t CODEC_IO_Write(uint8_t Addr, uint16_t Reg, uint16_t Value);
/**
  * @}
  */ 

/** @defgroup WM8904_Exported_Functions Exported Functions
  * @{
  */
  
/**
  * @brief Initializes the audio codec and the control interface.
  * @param DeviceAddr: Device address on communication Bus.   
  * @param OutputInputDevice: can  WM8904_OUT_HEADPHONE,
  *  WM8904_OUT_BOTH, WM8904_OUT_LINE, WM8904_IN_MIC1,
  *  WM8904_IN_MIC2, WM8904_IN_MIC1_MIC2, WM8904_IN_LINE1 
  *  WM8904_IN_LINE2 , WM8904_IN_DIGITAL_MIC1 or WM8904_IN_DIGITAL_MIC2
  * @param Volume: Initial volume level (from 0 (Mute) to 100 (Max))
  * @param AudioFreq: Audio Frequency 
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t WM8904_Init(uint16_t DeviceAddr, uint16_t OutputInputDevice, uint8_t Volume, uint32_t AudioFreq)
{
  uint32_t counter = 0;
  uint16_t output_device = OutputInputDevice & 0xFF;
  uint16_t input_device = OutputInputDevice & 0xFF00;
  uint16_t tmp;
  
  /* Initialize the Control interface of the Audio Codec */
  AUDIO_IO_Init();
  
  tmp = 0x0U; /* Reset registers */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_SW_RESET, tmp);
  
  /* Clock rate */
  tmp = 0x0004U; /* CLK_SYS_ENA = 1 */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_CLOCK_RATES2, tmp);

  /* Reference voltages and master bias */
  tmp = 0x0018U; /* ISEL[1:0] = 10b, BIAS_EN = 0 */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_BIAS_CONTROL0, tmp);
    
  tmp = 0x0047U; /* VMID_BUF_ENA = 1, VMID_RES[1:0] = 11b, VMID_ENA = 1 */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_VMID_CONTROL0, tmp);
  AUDIO_IO_Delay(100); /* Give VMID time to charge on cold start */

  tmp = 0x0043U; /* VMID_BUF_ENA = 1, VMID_RES[1:0] = 01b, VMID_ENA = 1 */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_VMID_CONTROL0, tmp);

  tmp = 0x0019U; /* ISEL[1:0] = 10b, BIAS_EN = 1 */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_BIAS_CONTROL0, tmp);

  /* Mic BIAS control */
  tmp = 0x0001U; /* MICBIAS_ENA = 1 */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_MIC_BIAS_CONTROL0, tmp);

  /* Power management */
  if (input_device == WM8904_IN_LINE2)
  {
  tmp = 0x0003U; /* INL_ENA = INR_ENA = 1 */
  }
  else if (input_device == WM8904_IN_MIC1)
  {
  tmp = 0x0002U; /* INL_ENA = 1 */
  }
  else /* WM8904_IN_DIGITAL_MIC2 */
  {
  tmp = 0x0000U; /* INx_ENA = 0 */
  }
  counter += CODEC_IO_Write(DeviceAddr, WM8904_PWR_MANAGEMENT0, tmp);

  tmp = 0x0003U; /* HPL_PGA_ENA = HPR_PGA_ENA = 1 */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_PWR_MANAGEMENT2, tmp);

  /* Clock rate */
  tmp = 0x0006U; /* CLK_DSP_ENA = CLK_SYS_ENA = 1 */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_CLOCK_RATES2, tmp);

  /* DAC and ADC*/
  if (input_device == WM8904_IN_LINE2)
  {
  tmp = 0x000FU; /* ADCR_ENA = ADCL_ENA = DACL_ENA = DACR_ENA = 1 */
  }
  else if (input_device == WM8904_IN_MIC1)
  {
  tmp = 0x000EU; /* ADCL_ENA = DACL_ENA = DACR_ENA = 1, ADCR_ENA = 0 */
  }
  else if (input_device == WM8904_IN_DIGITAL_MIC2)
  {
  tmp = 0x000EU; /* ADCL_ENA = DACL_ENA = DACR_ENA = 1, ADCR_ENA = 0 */
  }
  else /* WM8904_IN_DIGITAL_MIC2 */
  {
  tmp = 0x000EU; /* DACL_ENA = DACR_ENA = 1, ADCx_ENA = 0 */
  }
  counter += CODEC_IO_Write(DeviceAddr, WM8904_PWR_MANAGEMENT6, tmp);
  
  if (input_device == WM8904_IN_DIGITAL_MIC2)
  {
  tmp = 0x1800U; /* DMIC_ENA = 1, DMIC_SRC = IN1R/DMICDAT2 */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_DIGITAL_MICROPHONE0, tmp);

  tmp = 0x0008U; /* GPIO1_PU = GPIO1_PD = 0, GPIO1_SEL = 8 (DMIC clock out) */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_GPIO_CONTROL1, tmp);

  tmp = 0x81AFU; /* DRC_ENA = 1, DRC_DAC_PATH = ADC */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_DRC0, tmp);

  tmp = 0x32C1U; /* DRC_MINGAIN = 0dB, DRC_MAXGAIN = 18dB */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_DRC1, tmp);

  tmp = 0x002CU; /* DRC_HIGH_COMP = DRC_LO_COMP = 0 */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_DRC2, tmp);

  if (AudioFreq >= WM8904_FREQUENCY_32K)
  {
  tmp = 0x0000U; /* ADC_OSR128 = 0 (64xfs) */
  }
  else
  {
  tmp = 0x0001U; /* ADC_OSR128 = 1 (128xfs) */
  }
  counter += CODEC_IO_Write(DeviceAddr, WM8904_ANALOG_ADC0, tmp);
  
  }
  else
  {
  tmp = 0x0000U; /* DMIC_ENA = 0 */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_DIGITAL_MICROPHONE0, tmp);

  tmp = 0x0014U; /* GPIO1_PU = 0, GPIO1_PD = 1, GPIO1_SEL = 4 (IRQ, default value) */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_GPIO_CONTROL1, tmp);

  tmp = 0x01AFU; /* DRC_ENA = 0, DRC_DAC_PATH = ADC */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_DRC0, tmp);

  tmp = 0x0001U; /* ADC_OSR128 = 1 (128xfs) */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_ANALOG_ADC0, tmp);
  }
 
  if (input_device == WM8904_IN_LINE2)
  {
  tmp = 0x0050U; /* AIFADCL_SRC = 0 (Left), AIFADCR_SRC = 1 (Right) */
  }
  else if (input_device == WM8904_IN_MIC1)
  {
  tmp = 0x0010U; /* AIFADCL_SRC = 0 (Left), AIFADCR_SRC = 0 (left) */
  }
  else /* WM8904_IN_DIGITAL_MIC2 */
  {
  tmp = 0x0010U; /* AIFADCL_SRC = 0 (Left), AIFADCR_SRC = 0 (Left) */
  }
  counter += CODEC_IO_Write(DeviceAddr, WM8904_AUDIO_INTERFACE0, tmp);

  /* Resolution, protocol, frequency and volume */
  tmp = 0x0002U; /* Set resolution to 16B and protocol to I2S*/
  counter += CODEC_IO_Write(DeviceAddr, WM8904_AUDIO_INTERFACE1, tmp);

  counter += WM8904_SetFrequency(DeviceAddr, AudioFreq);
  
  if (input_device == WM8904_IN_LINE2)
  {
  tmp = 0x01C0U; /* ADC_VU = 1, ADC_VOL = +0dB */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_ADC_DIGITAL_VOL_LEFT, tmp);

  tmp = 0x01C0U; /* ADC_VU = 1, ADC_VOL = +0dB */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_ADC_DIGITAL_VOL_RIGHT, tmp);

  tmp = 0x0080U; /* LINMUTE = 1, LIN_VOL = -1.5dB */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_ANALOG_LEFT_INPUT0, tmp);

  tmp = 0x0080U; /* LINMUTE = 1, LIN_VOL = -1.5dB */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_ANALOG_RIGHT_INPUT0, tmp);

  tmp = 0x0014U; /* INL_CM_ENA = 0, L_IP_SEL_N[1:0] = IN2L (01b) */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_ANALOG_LEFT_INPUT1, tmp);

  tmp = 0x0014U; /* INR_CM_ENA = 0, R_IP_SEL_N[1:0] = IN2R (01b) */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_ANALOG_RIGHT_INPUT1, tmp);
  }
  else if (input_device == WM8904_IN_MIC1)
  {
  tmp = 0x01FFU; /* ADC_VU = 1, ADC_VOL = +17dB */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_ADC_DIGITAL_VOL_LEFT, tmp);

  tmp = 0x009AU; /* LINMUTE = 1, LIN_VOL = +14dB */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_ANALOG_LEFT_INPUT0, tmp);

  tmp = 0x0044U; /* INL_CM_ENA = 1, L_IP_SEL_N[1:0] = IN1L (00b) */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_ANALOG_LEFT_INPUT1, tmp);
  }
  else /* WM8904_IN_DIGITAL_MIC2 */
  {
  tmp = 0x0100U; /* ADC_VU = 1, ADC_VOL = Mute */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_ADC_DIGITAL_VOL_LEFT, tmp);
  }

  /* Charge pump */
  tmp = 0x0001U; /* CP_ENA = 1  */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_CHARGE_PUMP0, tmp);

  /* Output signal path */
  tmp = 0x0011U; /* HPL_ENA = HPR_ENA = 1 */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_ANALOG_HP0, tmp);

  tmp = 0x0033U; /* HPL_ENA_DLY = HPR_ENA_DLY = HPL_ENA = HPR_ENA = 1 */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_ANALOG_HP0, tmp);

  /* DC Servo */
  tmp = 0x0003U; /* Enable channels 0 and 1 */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_DC_SERVO0, tmp);

  tmp = 0x0030U; /* Startup 0 and 1 */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_DC_SERVO1, tmp);
  AUDIO_IO_Delay(100);
  
  /* Output signal path */
  tmp = 0x0077U; /* HPL_ENA_OUTP= HPL_ENA_OUTR = HPL_ENA_DLY = HPR_ENA_DLY = HPL_ENA = HPR_ENA = 1 */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_ANALOG_HP0, tmp);

  tmp = 0x00FFU; /* HPL_RMV_SHORT = HPR_RMV_SHORT = HPL_ENA_OUTP= HPL_ENA_OUTR = HPL_ENA_DLY = HPR_ENA_DLY = HPL_ENA = HPR_ENA = 1 */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_ANALOG_HP0, tmp);
  
  /* Charge pump */
  tmp = 0x01U; /* CP_DYN_PWR = 1 */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_CLASS_W0, tmp);
  
  /* Volume Control */
  if (output_device == WM8904_OUT_HEADPHONE)
  {
     counter += WM8904_SetVolume(DeviceAddr, Volume);
  }
 
  return counter;
}

/**
  * @brief  Deinitializes the audio codec.
  * @param  None
  * @retval None
  */
void WM8904_DeInit(void)
{
  /* Deinitialize Audio Codec interface */
  AUDIO_IO_DeInit();
}

/**
  * @brief  Get the WM8904 ID.
  * @param  DeviceAddr: Device address on communication Bus
  * @retval the WM8904 ID
  */
uint32_t WM8904_ReadID(uint16_t DeviceAddr)
{
  uint16_t Read_Value;

  /* Initialize the Control interface of the Audio Codec */
  AUDIO_IO_Init();

  /* Get ID from component */
  Read_Value =((uint32_t)AUDIO_IO_Read_16(DeviceAddr, WM8904_CHIPID_ADDR));
  
  return Read_Value;  
}

/**
  * @brief Start the audio Codec play feature.
  * @note For this codec no Play options are required.
  * @param DeviceAddr: Device address on communication Bus.   
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t WM8904_Play(uint16_t DeviceAddr, uint16_t* pBuffer, uint16_t Size)
{
  uint32_t counter = 0;
 
  /* Resumes the audio file playing */  
  /* Unmute the output first */
  counter += WM8904_SetMute(DeviceAddr, WM8904_MUTE_OFF);
  
  return counter;
}

/**
  * @brief Pauses playing on the audio codec.
  * @param DeviceAddr: Device address on communication Bus. 
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t WM8904_Pause(uint16_t DeviceAddr)
{
  uint32_t counter = 0;
  
  /* Pause the audio file playing */
  counter += WM8904_SetMute(DeviceAddr, WM8904_MUTE_ON);
  
  return counter;
}

/**
  * @brief Resumes playing on the audio codec.
  * @param DeviceAddr: Device address on communication Bus. 
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t WM8904_Resume(uint16_t DeviceAddr)
{
  uint32_t counter = 0;
 
  /* Resumes the audio file playing */
  counter += WM8904_SetMute(DeviceAddr, WM8904_MUTE_OFF);

  return counter;
}

/**
  * @brief Stops audio Codec playing. It powers down the codec.
  * @param  pObj pointer to component object
  * @param CodecPdwnMode  selects the  power down mode.
  *          - WM8904_PDWN_SW: only mutes the audio codec. When resuming from this
  *                           mode the codec keeps the previous initialization
  *                           (no need to re-Initialize the codec registers).
  *          - WM8904_PDWN_HW: Physically power down the codec. When resuming from this
  *                           mode, the codec is set to default configuration
  *                           (user should re-Initialize the codec in order to
  *                            play again the audio stream).
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t WM8904_Stop(uint16_t DeviceAddr, uint32_t CodecPdwnMode)
{
  uint32_t counter;
  uint16_t tmp;

  /* Mute the output */
  counter = WM8904_SetMute(DeviceAddr, WM8904_MUTE_ON);

  if (CodecPdwnMode == WM8904_PDWN_SW)
  {
   /* Only output mute required*/
  }
  else /* CODEC_PDWN_HW */
  {
  /* Output signal path */
  tmp = 0x0077U; /* HPL_RMV_SHORT = HPR_RMV_SHORT = 0 */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_ANALOG_HP0, tmp);

  tmp = 0x0000U; /* HPL_ENA_OUTP= HPL_ENA_OUTR = HPL_ENA_DLY = HPR_ENA_DLY = HPL_ENA = HPR_ENA = 0 */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_ANALOG_HP0, tmp);

  /* DC Servo */
  tmp = 0x0000U; /* Disable channels 0 and 1 */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_DC_SERVO0, tmp);

  /* Charge pump */
  tmp = 0x0000U; /* CP_ENA = 0  */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_CHARGE_PUMP0, tmp);

  /* DAC */
  tmp = 0x0000U; /* ADCL_ENA = DACL_ENA = DACR_ENA = 0 */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_PWR_MANAGEMENT6, tmp);

  /* Clock rate */
  tmp = 0x0004U; /* CLK_DSP_ENA = 0, CLK_SYS_ENA = 1 */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_CLOCK_RATES2, tmp);

  /* Output signal path */
  tmp = 0x0000U; /* HPL_PGA_ENA = HPR_PGA_ENA = 0 */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_PWR_MANAGEMENT2, tmp);

  tmp = 0x0000U; /* INL_ENA = 0 */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_PWR_MANAGEMENT0, tmp);

  /* Mic BIAS control */
  tmp = 0x0000U; /* MICBIAS_ENA = 0 */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_MIC_BIAS_CONTROL0, tmp);

  /* Reference voltages and master bias */
  tmp = 0x0018U; /* ISEL[1:0] = 10b, BIAS_EN = 0 */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_BIAS_CONTROL0, tmp);

  tmp = 0x0042U; /* VMID_BUF_ENA = 1, VMID_RES[1:0] = 01b, VMID_ENA = 0 */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_VMID_CONTROL0, tmp);
  AUDIO_IO_Delay(300);

  tmp = 0x0042U; /* VMID_BUF_ENA = 1, VMID_RES[1:0] = 01b, VMID_ENA = 0 */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_VMID_CONTROL0, tmp);

  tmp = 0x0000U; /* VMID_BUF_ENA = 0, VMID_RES[1:0] = 00b, VMID_ENA = 0 */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_VMID_CONTROL0, tmp);

  tmp = 0x0018U; /* ISEL[1:0] = 10b,  BIAS_EN = 0 */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_BIAS_CONTROL0, tmp);

  /* Clock rate */
  tmp = 0x0000U; /* CLK_SYS_ENA = 0 */
  counter += CODEC_IO_Write(DeviceAddr, WM8904_CLOCK_RATES2, tmp);
  }

  return counter;
}

/**
  * @brief Sets higher or lower the codec volume level.
  * @param DeviceAddr: Device address on communication Bus.
  * @param Volume: a byte value from 0 to 255 (refer to codec registers 
  *         description for more details).
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t WM8904_SetVolume(uint16_t DeviceAddr, uint8_t Volume)
{
  uint32_t counter = 0;
  uint8_t convertedvol;
  uint16_t tmp;

  /* Output volume */
  convertedvol = VOLUME_OUT_CONVERT(Volume);
  tmp = (uint16_t) convertedvol;
  
  /* Left Headphone Volume */  
  counter += CODEC_IO_Write(DeviceAddr, WM8904_ANALOG_OUTPUT1_LEFT, tmp);
  
  /* Right Headphone Volume (with volume update bit) */  
  tmp |= 0x80U;
  counter += CODEC_IO_Write(DeviceAddr, WM8904_ANALOG_OUTPUT1_RIGHT, tmp);

  return counter;
}


/**
  * @brief Enables or disables the mute feature on the audio codec.
  * @param DeviceAddr: Device address on communication Bus.   
  * @param Cmd: WM8904_MUTE_ON to enable the mute or WM8904_MUTE_OFF to disable the
  *             mute mode.
  * @retval 0 if correct communication, else wrong communication
  */

uint32_t WM8904_SetMute(uint16_t DeviceAddr, uint32_t Cmd)
{
  uint32_t counter = 0;
  uint16_t tmp;
  
  /* Set the Mute mode */
  if(Cmd == WM8904_MUTE_ON)
  {
    /* Mute the DAC Digital 1 (DAC_MUTE = 1, keep OSR128/unmute-ramp bits) */
    tmp = 0x0644U;
    counter += CODEC_IO_Write(DeviceAddr, WM8904_DAC_DIGITAL1, tmp);
  }
  else /* WM8904_MUTE_OFF : disable the Mute */
  {
    /* Unmute the DAC Digital 1 (DAC_MUTE = 0) */
    tmp = 0x0640U;
    counter += CODEC_IO_Write(DeviceAddr, WM8904_DAC_DIGITAL1, tmp);
  }

  return counter;
}


/**
  * @brief Switch dynamically (while audio file is played) the output target 
  *         (speaker or headphone).
  * @param DeviceAddr: Device address on communication Bus.
  * @param Output: specifies the audio output target: WM8904_OUT_HEADPHONE,
  *         WM8904_OUT_BOTH or WM8904_OUT_LINE 
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t WM8904_SetOutputMode(uint16_t DeviceAddr, uint8_t Output)
{
  (void)(DeviceAddr);
  (void)(Output);

  /* This feature is currently not supported */
  return 0;
}

/**
  * @brief Sets new frequency.
  * @param DeviceAddr: Device address on communication Bus.
  * @param AudioFreq: Audio frequency used to play the audio stream.
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t WM8904_SetFrequency(uint16_t DeviceAddr, uint32_t AudioFreq)
{
  int32_t counter = 0;
  uint16_t tmp;
  
  /*  Clock Configurations */
  switch(AudioFreq)
  {
  case WM8904_FREQUENCY_8K:
       /* Sample Rate = 8 (KHz), ratio=256 */ 
    tmp = 0x0C00U;
    counter += CODEC_IO_Write(DeviceAddr, WM8904_CLOCK_RATES1, tmp);
    break;

  case WM8904_FREQUENCY_11K:
  case WM8904_FREQUENCY_12K:
       /* Sample Rate = 11.025 (KHz) or 12 (KHz), ratio=256 */ 
    tmp = 0x0C01U;
    counter += CODEC_IO_Write(DeviceAddr, WM8904_CLOCK_RATES1, tmp);
    break;

  case WM8904_FREQUENCY_16K:
      /* Sample Rate = 16 (KHz), ratio=256 */ 
    tmp = 0x0C02U;
    counter += CODEC_IO_Write(DeviceAddr, WM8904_CLOCK_RATES1, tmp);
    break;

  case WM8904_FREQUENCY_22K:
  case WM8904_FREQUENCY_24K:
       /* Sample Rate = 22.05 (KHz) or 24 (KHz), ratio=256 */ 
    tmp = 0x0C03U;
    counter += CODEC_IO_Write(DeviceAddr, WM8904_CLOCK_RATES1, tmp);
    break;

  case WM8904_FREQUENCY_32K:
       /* Sample Rate = 32 (KHz), ratio=256 */ 
    tmp = 0x0C04U;
    counter += CODEC_IO_Write(DeviceAddr, WM8904_CLOCK_RATES1, tmp);
    break;

  case  WM8904_FREQUENCY_44K:
  case  WM8904_FREQUENCY_48K:
    /* 44.1k or 48k */
    tmp = 0x0C05U;
    counter += CODEC_IO_Write(DeviceAddr, WM8904_CLOCK_RATES1, tmp);
    break;

  default:
      /* 44.1k or 48k */
    tmp = 0x0C05U;
    counter += CODEC_IO_Write(DeviceAddr, WM8904_CLOCK_RATES1, tmp);
    break;
  }

  return counter;
}

/**
  * @brief Resets wm8904 registers.
  * @param DeviceAddr: Device address on communication Bus.
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t WM8904_Reset(uint16_t DeviceAddr)
{
  uint16_t tmp;
  int32_t  counter = 0;

  /* Reset Codec by writing in 0x0000 address register */
  tmp = 0x0U; /* Reset registers */
  counter = CODEC_IO_Write(DeviceAddr, WM8904_SW_RESET, tmp);

  return counter;
}

/**
  * @}
  */


/******************** Static functions ****************************************/
/**
  * @brief  Writes/Read a single data.
  * @param  Addr: I2C address
  * @param  Reg: Reg address 
  * @param  Value: Data to be written
  * @retval None
  */
static uint8_t CODEC_IO_Write(uint8_t Addr, uint16_t Reg, uint16_t Value)
{
  uint32_t result=0;
  
  AUDIO_IO_Write_16(Addr, Reg, Value);
  
#ifdef VERIFY_WRITTENDATA
  /* Verify that the data has been correctly written */
  result = (AUDIO_IO_Read_16(Addr, Reg) == Value) ? 0U : 1U;
#endif /* VERIFY_WRITTENDATA */
  
  return result;
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
