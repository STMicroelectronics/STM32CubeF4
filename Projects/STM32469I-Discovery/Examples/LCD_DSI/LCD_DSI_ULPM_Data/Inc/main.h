/**
  ******************************************************************************
  * @file    LCD_DSI/LCD_DSI_ULPM_Data/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

#include "stm32469i_discovery.h"
#include "stm32469i_discovery_lcd.h"
#include "stm32469i_discovery_sdram.h"

/* Exported types ------------------------------------------------------------*/

/**
 *  @brief LCD_DsiDphyClkTypeDef
 *  Possible values of supported frequencies of DPHY Clock in  LCD
 */
typedef enum
{
    LCD_DSI_ULPM_MODE_NONE                = 0x00, /*!< Never activate ULPM */
    LCD_DSI_ULPM_MODE_DATA_LANE_ONLY      = 0x01, /*!< Activate ULPM when possible but on Data lane only   */
    LCD_DSI_ULPM_MODE_DATA_AND_CLOCK_LANE = 0x02, /*!< Activate ULPM when possible on Data and clock lanes */
    LCD_DSI_ULPM_MODE_INVALID             = 0x03  /*!< Invalid ULPM configuration */

} LCD_DsiUlpmModeTypeDef;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Toggle_Leds(void);

#endif /* __MAIN_H */
