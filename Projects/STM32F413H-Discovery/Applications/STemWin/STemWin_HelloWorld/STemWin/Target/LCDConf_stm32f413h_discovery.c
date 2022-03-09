/**
  ******************************************************************************
  * @file    LCDConf_stm32f413h_discovery.c
  * @author  MCD Application Team
  * @brief   Driver for STM32F413H-Discovery board LCD
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
#include "GUI.h"
#include "GUIDRV_FlexColor.h"
#include "main.h"
#include "st7789h2/st7789h2.h"

/*********************************************************************
*
*       Layer configuration (to be modified)
*
**********************************************************************
*/

//
// Physical display size
//
#define XSIZE_PHYS  240
#define YSIZE_PHYS  240

#define NUM_BUFFERS  3 /* Number of multiple buffers to be used */

/*********************************************************************
*
*       Configuration checking
*
**********************************************************************
*/
#ifndef   VXSIZE_PHYS
  #define VXSIZE_PHYS XSIZE_PHYS
#endif
#ifndef   VYSIZE_PHYS
  #define VYSIZE_PHYS YSIZE_PHYS
#endif
#ifndef   XSIZE_PHYS
  #error Physical X size of display is not defined!
#endif
#ifndef   YSIZE_PHYS
  #error Physical Y size of display is not defined!
#endif
#ifndef   GUICC_M565
  #error Color conversion not defined!
#endif
#ifndef   GUIDRV_FLEXCOLOR
  #error No display driver defined!
#endif

/*********************************************************************
*
*       Defines, sfrs
*
**********************************************************************
*/
//
// COG interface register addr.
//

typedef struct
{
  __IO uint16_t REG;
  __IO uint16_t RAM;

}LCD_CONTROLLER_TypeDef;

#define FMC_BANK3_BASE  ((uint32_t)(0x60000000 | 0x08000000))
#define FMC_BANK3       ((LCD_CONTROLLER_TypeDef *) FMC_BANK3_BASE)

/*********************************************************************
*
*       Local functions
*
**********************************************************************
*/

static void     STM_FMC_BANK3_WriteData(uint16_t Data);
static void     STM_FMC_BANK3_WriteReg(uint8_t Reg);
static uint16_t STM_FMC_BANK3_ReadData(void);
static void     LCD_LL_IO_Init(void);
void            BSP_LCD_MspInit(void);

/********************************************************************
*
*       LcdWriteReg
*
* Function description:
*   Sets display register
*/
static void LcdWriteReg(U16 Data) 
{
  STM_FMC_BANK3_WriteReg(Data);
}

/********************************************************************
*
*       LcdWriteData
*
* Function description:
*   Writes a value to a display register
*/
static void LcdWriteData(U16 Data) 
{
  STM_FMC_BANK3_WriteData(Data);
}

/********************************************************************
*
*       LcdWriteDataMultiple
*
* Function description:
*   Writes multiple values to a display register.
*/
static void LcdWriteDataMultiple(U16 * pData, int NumItems) 
{
  while (NumItems--) 
  {
    STM_FMC_BANK3_WriteData(*pData++);
  }
}

/********************************************************************
*
*       LcdReadDataMultiple
*
* Function description:
*   Reads multiple values from a display register.
*/
static void LcdReadDataMultiple(U16 * pData, int NumItems) 
{
  while (NumItems--) 
  {
    *pData++ = STM_FMC_BANK3_ReadData();
  }
}

/*********************************************************************
*
*       Public functions
*
**********************************************************************
*/

/**
  * @brief  Initializes the LCD.
  * @param  None
  * @retval LCD state
  */
void LCD_LL_Init(void)
{  
  /* LCD Init */
  LCD_LL_IO_Init();
}

/*********************************************************************
*
*       LCD_X_Config
*
* Function description:
*   Called during the initialization process in order to set up the
*   display driver configuration.
*
*/
void LCD_X_Config(void) 
{
  GUI_DEVICE * pDevice;
  CONFIG_FLEXCOLOR Config = {0};
  GUI_PORT_API PortAPI = {0};
  //
  // Set display driver and color conversion
  //
  pDevice = GUI_DEVICE_CreateAndLink(GUIDRV_FLEXCOLOR, GUICC_M565, 0, 0);
  //
  // Display driver configuration, required for Lin-driver
  //
  LCD_SetSizeEx (0, XSIZE_PHYS , YSIZE_PHYS);
  LCD_SetVSizeEx(0, VXSIZE_PHYS, VYSIZE_PHYS);
  //
  // Orientation
  //
  
  #if (NUM_BUFFERS > 1)
      GUI_MULTIBUF_ConfigEx(0, NUM_BUFFERS);
  #endif
    
  GUIDRV_FlexColor_Config(pDevice, &Config);
  //
  // Set controller and operation mode
  //
  PortAPI.pfWrite16_A0  = LcdWriteReg;
  PortAPI.pfWrite16_A1  = LcdWriteData;
  PortAPI.pfWriteM16_A1 = LcdWriteDataMultiple;
  PortAPI.pfReadM16_A1  = LcdReadDataMultiple;
  GUIDRV_FlexColor_SetFunc(pDevice, &PortAPI, GUIDRV_FLEXCOLOR_F66709, GUIDRV_FLEXCOLOR_M16C0B16);
}

/*********************************************************************
*
*       LCD_X_DisplayDriver
*
* Function description:
*   This function is called by the display driver for several purposes.
*   To support the according task the routine needs to be adapted to
*   the display controller. Please note that the commands marked with
*   'optional' are not cogently required and should only be adapted if
*   the display controller supports these features.
*
* Parameter:
*   LayerIndex - Index of layer to be configured
*   Cmd        - Please refer to the details in the switch statement below
*   pData      - Pointer to a LCD_X_DATA structure
*
* Return Value:
*   < -1 - Error
*     -1 - Command not handled
*      0 - Ok
*/
int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData) {
  int r;
  (void) LayerIndex;
  (void) pData;
  
  switch (Cmd) {
  case LCD_X_INITCONTROLLER: {
    
    LCD_LL_Init();
    
    return 0;
  }
  default:
    r = -1;
  }
  return r;
}

/**
  * @brief  Initializes the LCD.
  * @param  None
  * @retval None
  */
static void LCD_LL_IO_Init(void)
{ 
  /* Initialize LCD special pins GPIOs */
  BSP_LCD_MspInit();
  
  /* Backlight control signal assertion */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET);
  
  /* Apply hardware reset according to procedure indicated in FRD154A24IPS documentation */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
  HAL_Delay(5);   /* Reset signal asserted during 5ms  */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
  HAL_Delay(10);  /* Reset signal released during 10ms */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
  HAL_Delay(20);  /* Reset signal asserted during 20ms */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
  HAL_Delay(10);  /* Reset signal released during 10ms */
  
  if(ST7789H2_drv.ReadID() == ST7789H2_ID)
  {    
    ST7789H2_Init();   
  }
}

/**
  * @brief  Initializes the LCD GPIO special pins MSP.
  * @param  None
  * @retval None
  */
void BSP_LCD_MspInit(void)
{
  GPIO_InitTypeDef gpio_init_structure;

  /* Enable GPIOs clock */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();

  /* LCD_RESET GPIO configuration */
  gpio_init_structure.Pin       = GPIO_PIN_13;     /* LCD_RESET pin has to be manually controlled */
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FAST;
  gpio_init_structure.Mode      = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(GPIOB, &gpio_init_structure);

  /* LCD_TE GPIO configuration */
  gpio_init_structure.Pin       = GPIO_PIN_14;        /* LCD_TE pin has to be manually managed */
  gpio_init_structure.Mode      = GPIO_MODE_INPUT;
  HAL_GPIO_Init(GPIOB, &gpio_init_structure);

  /* LCD_BL_CTRL GPIO configuration */
  gpio_init_structure.Pin       = GPIO_PIN_5;   /* LCD_BL_CTRL pin has to be manually controlled */
  gpio_init_structure.Mode      = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(GPIOE, &gpio_init_structure);
}

/**
  * @brief  Writes register value.
  * @param  Data: 
  * @retval None
  */
static void STM_FMC_BANK3_WriteData(uint16_t Data) 
{
  /* Write 16-bit Reg */
  FMC_BANK3->RAM = Data;
}

/**
  * @brief  Writes register address.
  * @param  Reg: 
  * @retval None
  */
static void STM_FMC_BANK3_WriteReg(uint8_t Reg) 
{
  /* Write 16-bit Index, then write register */
  FMC_BANK3->REG = Reg;
}

/**
  * @brief  Reads register value.
  * @param  None
  * @retval Read value
  */
static uint16_t STM_FMC_BANK3_ReadData(void) 
{
  return FMC_BANK3->RAM;
}
