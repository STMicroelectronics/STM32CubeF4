/**
  ******************************************************************************
  * @file    Audio/Audio_playback_and_record/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module.
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
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4_discovery.h"
#include "stm32f4_discovery_audio.h"
#include "stm32f4_discovery_accelerometer.h"
#include <stdio.h>
#include "stm32f4xx_it.h"
#include "waveplayer.h"

#include "waverecorder.h"
#include "ff.h"
#include "ff_gen_drv.h"
#include "usbh_diskio_dma.h"
 
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Uncomment this define to disable repeat feature */
/* #define PLAY_REPEAT_DISABLED */

typedef enum
{
  APPLICATION_IDLE = 0,  
  APPLICATION_START,    
  APPLICATION_RUNNING,
}
MSC_ApplicationTypeDef;
/* You can change the Wave file name as you need, but do not exceed 11 characters */
#define WAVE_NAME "0:audio_sample.wav"
#define REC_WAVE_NAME "0:rec.wav"
  
/* State Machine for the USBH_USR_ApplicationState */
#define USBH_USR_FS_INIT    ((uint8_t)0x00)
#define USBH_USR_AUDIO      ((uint8_t)0x01)

/* Defines for the Audio used commands */
#define CMD_PLAY           ((uint32_t)0x00)
#define CMD_RECORD         ((uint32_t)0x01)
#define CMD_STOP           ((uint32_t)0x02)

/* Defines for LEDs lighting */
#define LED3_TOGGLE      0x03  /* Toggle LED3 */
#define LED4_TOGGLE      0x04  /* Toggle LED4 */
#define LED6_TOGGLE      0x06  /* Toggle LED6 */
#define LEDS_OFF         0x07  /* Turn OFF all LEDs */
#define STOP_TOGGLE      0x00  /* Stop LED Toggling */

/* Defines for the Audio playing process */
#define PAUSE_STATUS     ((uint32_t)0x00) /* Audio Player in Pause Status */
#define RESUME_STATUS    ((uint32_t)0x01) /* Audio Player in Resume Status */
#define IDLE_STATUS      ((uint32_t)0x02) /* Audio Player in Idle Status */

#define REPEAT_ON        ((uint32_t)0x00) /* Replay Status in ON */
#define REPEAT_OFF       ((uint32_t)0x01) /* Replay Status in OFF */

/* Defines for MEMS Acclerometer ID Types */
#define MEMS_LIS3DSH     0x3F /* LIS3DSH MEMS Acclerometer ID */
#define MEMS_LIS302DL    0x3B /* LIS302DL MEMS Acclerometer ID */
                                                                                    
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Error_Handler(void);

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
