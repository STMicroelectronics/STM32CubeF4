/**
  ******************************************************************************
  * @file    USB_Host/FWupgrade_Standalone/Src/iap_menu.c
  * @author  MCD Application Team
  * @brief   COMMAND IAP Execute Application
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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* State Machine for the DEMO State */
#define DEMO_INIT       ((uint8_t)0x00)
#define DEMO_IAP        ((uint8_t)0x01)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t UploadCondition = 0x00;
DIR dir;
FILINFO fno;
static uint8_t Demo_State = DEMO_INIT;
extern char USBDISKPath[4];

/* Private function prototypes -----------------------------------------------*/
static void IAP_UploadTimeout(void);
static void USBH_USR_BufferSizeControl(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Demo application for IAP through USB mass storage.
  * @param  None
  * @retval None
  */
void FW_UPGRADE_Process(void)
{
  switch(Demo_State)
  {
  case DEMO_INIT:
    /* Register the file system object to the FatFs module */
	if (FATFS_LinkDriver(&USBH_Driver, USBDISKPath) == 0)
    {
      if (f_mount(&USBH_fatfs, "", 0) != FR_OK)
      {
        /* FatFs initialization fails */
        /* Toggle LED3 and LED4 in infinite loop */
        FatFs_Fail_Handler();
      }
    }

    /* Go to IAP menu */
    Demo_State = DEMO_IAP;
    break;

  case DEMO_IAP:
    while(USBH_MSC_IsReady(&hUSBHost))
    {
      /* Control BUFFER_SIZE value */
      USBH_USR_BufferSizeControl();

      /* Keep LED1 and LED3 Off when Device connected */
      BSP_LED_Off(LED1);
      BSP_LED_Off(LED3);

      /* KEY Button pressed Delay */
      IAP_UploadTimeout();

      /* Writes Flash memory */
      COMMAND_Download();

      /* Check if KEY Button is already pressed */
      if((UploadCondition == 0x01))
      {
        /* Reads all flash memory */
        COMMAND_Upload();
      }
      else
      {
        /* Turn LED2 Off: Download Done */
        BSP_LED_Off(LED2);
        /* Turn LED1 On: Waiting KEY button pressed */
        BSP_LED_On(LED1);
      }

      /* Waiting KEY Button Released */
      while((BSP_PB_GetState(BUTTON_KEY) == GPIO_PIN_RESET) && (Appli_state == APPLICATION_READY))
      {}

      /* Waiting KEY Button Pressed */
      while((BSP_PB_GetState(BUTTON_KEY) != GPIO_PIN_RESET) && (Appli_state == APPLICATION_READY))
      {}

      /* Waiting KEY Button Released */
      while((BSP_PB_GetState(BUTTON_KEY) == GPIO_PIN_RESET) && (Appli_state == APPLICATION_READY))
      {}

      if(Appli_state == APPLICATION_READY)
      {
        /* Jump to user application code located in the internal Flash memory */
        COMMAND_Jump();
      }
    }
    break;

  default:
    break;
  }
  if(Appli_state == APPLICATION_DISCONNECT)
  {
    /* Toggle LED3: USB device disconnected */
    BSP_LED_Toggle(LED3);
    HAL_Delay(100);
  }
}

/**
  * @brief  Button state time control.
  * @param  None
  * @retval None
  */
static void IAP_UploadTimeout(void)
{
  /* Check if KEY button is pressed */
  if(BSP_PB_GetState(BUTTON_KEY) == GPIO_PIN_RESET)
  {
    /* To execute the UPLOAD command the KEY button should be kept pressed 3s
       just after a board reset, at firmware startup */
    HAL_Delay (3000);

    if(BSP_PB_GetState(BUTTON_KEY) == GPIO_PIN_RESET)
    {
      /* UPLOAD command will be executed immediately after
      completed execution of the DOWNLOAD command */

      UploadCondition = 0x01;

      /* Turn LED3 on : Upload condition Verified */
      BSP_LED_On(LED3);
    }
    else
    {
      /* Only the DOWNLOAD command is executed */
      UploadCondition = 0x00;
    }
  }
}

/**
  * @brief  Handles the program fail.
  * @param  None
  * @retval None
  */
void Fail_Handler(void)
{
  while(1)
  {
    /* Toggle LED3 */
    BSP_LED_Toggle(LED3);
    HAL_Delay(100);
  }
}

/**
  * @brief  Handles the Flash Erase fail.
  * @param  None
  * @retval None
  */
void Erase_Fail_Handler(void)
{
  while(1)
  {
    /* Toggle LED2 and LED3 */
    BSP_LED_Toggle(LED2);
    BSP_LED_Toggle(LED3);
    HAL_Delay(100);
  }
}

/**
  * @brief  Handles the FatFs fail.
  * @param  None
  * @retval None
  */
void FatFs_Fail_Handler(void)
{
  while(1)
  {
    /* Toggle LED3 and LED4 */
    BSP_LED_Toggle(LED3);
    BSP_LED_Toggle(LED4);
    HAL_Delay(100);
  }
}

/**
  * @brief  Controls Buffer size value.
  * @param  None
  * @retval None
  */
static void USBH_USR_BufferSizeControl(void)
{
  /* Control BUFFER_SIZE and limit this value to 32Kbyte maximum */
  if((BUFFER_SIZE % 4 != 0x00) || (BUFFER_SIZE / 4 > 8192))
  {
    while(1)
    {
      /* Toggle LED2, LED3 and LED4 */
      BSP_LED_Toggle(LED2);
      BSP_LED_Toggle(LED3);
      BSP_LED_Toggle(LED4);
      HAL_Delay(100);
    }
  }
}
