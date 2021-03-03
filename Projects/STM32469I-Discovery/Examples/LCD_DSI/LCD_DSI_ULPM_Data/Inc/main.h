/**
  ******************************************************************************
  * @file    LCD_DSI/LCD_DSI_ULPM_Data/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
