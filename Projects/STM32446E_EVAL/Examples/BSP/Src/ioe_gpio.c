/**
  ******************************************************************************
  * @file    BSP/Src/ioe_gpio.c 
  * @author  MCD Application Team
  * @brief   This test check the IO and exti of the MFX expander Driver
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

/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MFX_CONNECTET_PIN IO_PIN_0
#define MCU_GPIO_PORT GPIOB
#define MCU_GPIO_PIN  GPIO_PIN_4
#define __MCU_GPIO_PORTCLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()


#define IOE_GPIO_TEST_PASSED 0
#define IOE_GPIO_TEST_FAILED 1

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void IOE_GPIO_SetHint(void);
static void SetMcuGpioToBeConnectedToMfxGPI(void);
static void SetMcuGpioToBeConnectedToMfxGPO(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  SDRAM Demo
  * @param  None
  * @retval None
  */
void IOE_GPIO_demo (void)
{ 
  uint32_t ioe_irq_pending_status, ioe_gpio_status; 
  uint32_t lcd_line = 85;
  uint8_t test_result = IOE_GPIO_TEST_PASSED;
  uint8_t all_test_fail = 1;
  GPIO_PinState mcu_pin_state;

  IOE_GPIO_SetHint();

  /* Enable the Leds */
  BSP_IO_Init();
  BSP_LED_Init(LED1); 
  BSP_LED_Init(LED2); 
  
  BSP_LED_On(LED1); 
  BSP_LED_On(LED2); 
 
  BSP_IO_ConfigPin(MFX_CONNECTET_PIN, IO_MODE_OFF);

  /* TEST IO_MODE_OUPUT mode */
  /* ---------------------------------- */

  /* SetUp a GPIO to be connected to one of the MFX GPIOS via a wire */
  SetMcuGpioToBeConnectedToMfxGPO();
  test_result = IOE_GPIO_TEST_PASSED;
  BSP_IO_ConfigPin(MFX_CONNECTET_PIN, IO_MODE_OUTPUT);

  BSP_IO_WritePin(MFX_CONNECTET_PIN, BSP_IO_PIN_RESET);
  HAL_Delay(1);
  mcu_pin_state = HAL_GPIO_ReadPin (MCU_GPIO_PORT, MCU_GPIO_PIN);
  if (mcu_pin_state) 
  {
    test_result |= IOE_GPIO_TEST_FAILED;
  }
  else
  {
    test_result |= IOE_GPIO_TEST_PASSED;
  }

  BSP_IO_WritePin(MFX_CONNECTET_PIN, BSP_IO_PIN_SET);
  HAL_Delay(1);
  mcu_pin_state = HAL_GPIO_ReadPin (MCU_GPIO_PORT, MCU_GPIO_PIN);
  if (mcu_pin_state) 
  {
    test_result |= IOE_GPIO_TEST_PASSED;
  }
  else
  {
    test_result |= IOE_GPIO_TEST_FAILED;
  }

  BSP_IO_WritePin(MFX_CONNECTET_PIN, BSP_IO_PIN_RESET);
  HAL_Delay(1);
  mcu_pin_state = HAL_GPIO_ReadPin (MCU_GPIO_PORT, MCU_GPIO_PIN);
  if (mcu_pin_state) 
  {
    test_result |= IOE_GPIO_TEST_FAILED;
  }
  else
  {
    test_result |= IOE_GPIO_TEST_PASSED;
  }

  lcd_line += 15;
  if (test_result) 
  {
    BSP_LCD_DisplayStringAt(20, lcd_line, (uint8_t *)"IOE IO_MODE_OUTPUT: FAILED", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, lcd_line, (uint8_t *)"IOE IO_MODE_OUTPUT: PASSED", LEFT_MODE);
    all_test_fail = 0;
  }


  /* TEST IO_MODE_INPUT mode */
  /* ---------------------------------- */

  /* SetUp a GPIO to be connected to one of the MFX GPIOS via a wire */
  SetMcuGpioToBeConnectedToMfxGPI();
  test_result = IOE_GPIO_TEST_PASSED;
  BSP_IO_ConfigPin(MFX_CONNECTET_PIN, IO_MODE_INPUT);

  HAL_GPIO_WritePin(MCU_GPIO_PORT, MCU_GPIO_PIN, GPIO_PIN_RESET);
  HAL_Delay(1);
  ioe_gpio_status = BSP_IO_ReadPin (MFX_CONNECTET_PIN);
  if (ioe_gpio_status) 
  {
    test_result |= IOE_GPIO_TEST_FAILED;
  }
  else
  {
    test_result |= IOE_GPIO_TEST_PASSED;
  }

  HAL_GPIO_WritePin(MCU_GPIO_PORT, MCU_GPIO_PIN, GPIO_PIN_SET);
  HAL_Delay(1);
  ioe_gpio_status = BSP_IO_ReadPin (MFX_CONNECTET_PIN);
  if (ioe_gpio_status) 
  {
    test_result |= IOE_GPIO_TEST_PASSED;
  }
  else
  {
    test_result |= IOE_GPIO_TEST_FAILED;
  }

  HAL_GPIO_WritePin(MCU_GPIO_PORT, MCU_GPIO_PIN, GPIO_PIN_RESET);
  HAL_Delay(1);
  ioe_gpio_status = BSP_IO_ReadPin (MFX_CONNECTET_PIN);
  if (ioe_gpio_status) 
  {
    test_result |= IOE_GPIO_TEST_FAILED;
  }
  else
  {
    test_result |= IOE_GPIO_TEST_PASSED;
  }

  lcd_line += 15;
  if (test_result) 
  {
    BSP_LCD_DisplayStringAt(20, lcd_line, (uint8_t *)"IOE IO_MODE_INPUT: FAILED", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, lcd_line, (uint8_t *)"IOE IO_MODE_INPUT: PASSED", LEFT_MODE);
    all_test_fail = 0;
  }

  SetMcuGpioToBeConnectedToMfxGPI();
		
  /* TEST IO_MODE_IT_HIGH_LEVEL mode */
  /* ---------------------------------- */
  test_result = IOE_GPIO_TEST_PASSED;
  BSP_IO_ConfigPin(MFX_CONNECTET_PIN, IO_MODE_OFF);
  HAL_GPIO_WritePin(MCU_GPIO_PORT, MCU_GPIO_PIN, GPIO_PIN_RESET);
  BSP_IO_ITClear();   

  /* BSP_IO_ConfigPin(MFXIO_PIN_6, IO_MODE_INPUT); */
  BSP_IO_ConfigPin(MFX_CONNECTET_PIN, IO_MODE_IT_HIGH_LEVEL_PD);
  /* We just want to test the good functioning of pending bit and ack on the IOE */
  HAL_NVIC_DisableIRQ((IRQn_Type)(MFX_IRQOUT_EXTI_IRQn));

  ioe_irq_pending_status = BSP_IO_ITGetStatus(MFX_CONNECTET_PIN);
  if (ioe_irq_pending_status) 
  {
    BSP_IO_ITClear();   
    test_result |= IOE_GPIO_TEST_FAILED;
  }
  else
  {
    test_result |= IOE_GPIO_TEST_PASSED;
  }

  HAL_GPIO_WritePin(MCU_GPIO_PORT, MCU_GPIO_PIN, GPIO_PIN_SET);
  ioe_irq_pending_status = BSP_IO_ITGetStatus(MFX_CONNECTET_PIN);
  if (ioe_irq_pending_status) 
  {
    BSP_IO_ITClear();   
    test_result |= IOE_GPIO_TEST_PASSED;
  }
  else
  {
    test_result |= IOE_GPIO_TEST_FAILED;
  }
  HAL_Delay(1);

  ioe_irq_pending_status = BSP_IO_ITGetStatus(MFX_CONNECTET_PIN);
  HAL_GPIO_WritePin(MCU_GPIO_PORT, MCU_GPIO_PIN, GPIO_PIN_RESET);
  if (ioe_irq_pending_status) 
  {
    BSP_IO_ITClear();   
    test_result |= IOE_GPIO_TEST_PASSED;
  }
  else
  {
    test_result |= IOE_GPIO_TEST_FAILED;
  }

  ioe_irq_pending_status = BSP_IO_ITGetStatus(MFX_CONNECTET_PIN);
  if (ioe_irq_pending_status) 
  {
    BSP_IO_ITClear();   
    test_result |= IOE_GPIO_TEST_FAILED;
  }
  else
  {
    test_result |= IOE_GPIO_TEST_PASSED;
  }

  lcd_line += 15;
  if (test_result) 
  {
    BSP_LCD_DisplayStringAt(20, lcd_line, (uint8_t *)"IOE IO_MODE_IT_HIGH_LEVEL: FAILED", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, lcd_line, (uint8_t *)"IOE IO_MODE_IT_HIGH_LEVEL: PASSED", LEFT_MODE);
    all_test_fail = 0;
  }


  /* TEST IO_MODE_IT_LOW_LEVEL mode */
  /* ---------------------------------- */
  test_result = IOE_GPIO_TEST_PASSED;
  BSP_IO_ConfigPin(MFX_CONNECTET_PIN, IO_MODE_OFF);
  HAL_GPIO_WritePin(MCU_GPIO_PORT, MCU_GPIO_PIN, GPIO_PIN_SET);
  BSP_IO_ITClear();   

  /* BSP_IO_ConfigPin(MFXIO_PIN_6, IO_MODE_INPUT); */
  BSP_IO_ConfigPin(MFX_CONNECTET_PIN, IO_MODE_IT_LOW_LEVEL_PU);
  /* We just want to test the good functioning of pending bit and ack on the IOE */
  HAL_NVIC_DisableIRQ((IRQn_Type)(MFX_IRQOUT_EXTI_IRQn));

  ioe_irq_pending_status = BSP_IO_ITGetStatus(MFX_CONNECTET_PIN);
  if (ioe_irq_pending_status) 
  {
    BSP_IO_ITClear();   
    test_result |= IOE_GPIO_TEST_FAILED;
  }
  else
  {
    test_result |= IOE_GPIO_TEST_PASSED;
  }
  HAL_Delay(1);

  HAL_GPIO_WritePin(MCU_GPIO_PORT, MCU_GPIO_PIN, GPIO_PIN_RESET);
  ioe_irq_pending_status = BSP_IO_ITGetStatus(MFX_CONNECTET_PIN);
  if (ioe_irq_pending_status) 
  {
    BSP_IO_ITClear();   
    test_result |= IOE_GPIO_TEST_PASSED;
  }
  else
  {
    test_result |= IOE_GPIO_TEST_FAILED;
  }
  HAL_Delay(1);

  ioe_irq_pending_status = BSP_IO_ITGetStatus(MFX_CONNECTET_PIN);
  HAL_GPIO_WritePin(MCU_GPIO_PORT, MCU_GPIO_PIN, GPIO_PIN_SET);
  if (ioe_irq_pending_status) 
  {
    BSP_IO_ITClear();   
    test_result |= IOE_GPIO_TEST_PASSED;
  }
  else
  {
    test_result |= IOE_GPIO_TEST_FAILED;
  }

  ioe_irq_pending_status = BSP_IO_ITGetStatus(MFX_CONNECTET_PIN);
  if (ioe_irq_pending_status) 
  {
    BSP_IO_ITClear();   
    test_result |= IOE_GPIO_TEST_FAILED;
  }
  else
  {
    test_result |= IOE_GPIO_TEST_PASSED;
  }

  lcd_line += 15;
  if (test_result) 
  {
    BSP_LCD_DisplayStringAt(20, lcd_line, (uint8_t *)"IOE IO_MODE_IT_LOW_LEVEL: FAILED", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, lcd_line, (uint8_t *)"IOE IO_MODE_IT_LOW_LEVEL: PASSED", LEFT_MODE);
    all_test_fail = 0;
  }

  /* TEST IO_MODE_IT_RISING_EDGE mode */
  /* ---------------------------------- */
  test_result = IOE_GPIO_TEST_PASSED;
  BSP_IO_ConfigPin(MFX_CONNECTET_PIN, IO_MODE_OFF);
  HAL_GPIO_WritePin(MCU_GPIO_PORT, MCU_GPIO_PIN, GPIO_PIN_RESET);
  BSP_IO_ITClear();   

  /* BSP_IO_ConfigPin(MFXIO_PIN_6, IO_MODE_INPUT); */
  BSP_IO_ConfigPin(MFX_CONNECTET_PIN, IO_MODE_IT_RISING_EDGE_PD);
  /* We just want to test the good functioning of pending bit and ack on the IOE */
  HAL_NVIC_DisableIRQ((IRQn_Type)(MFX_IRQOUT_EXTI_IRQn));

  ioe_irq_pending_status = BSP_IO_ITGetStatus(MFX_CONNECTET_PIN);
  if (ioe_irq_pending_status) 
  {
    BSP_IO_ITClear();   
    test_result |= IOE_GPIO_TEST_FAILED;
  }
  else
  {
    test_result |= IOE_GPIO_TEST_PASSED;
  }

  HAL_GPIO_WritePin(MCU_GPIO_PORT, MCU_GPIO_PIN, GPIO_PIN_SET);
  HAL_Delay(1);
  ioe_irq_pending_status = BSP_IO_ITGetStatus(MFX_CONNECTET_PIN);
  if (ioe_irq_pending_status) 
  {
    BSP_IO_ITClear();   
    test_result |= IOE_GPIO_TEST_PASSED;
  }
  else
  {
    test_result |= IOE_GPIO_TEST_FAILED;
  }
  HAL_Delay(1);

  ioe_irq_pending_status = BSP_IO_ITGetStatus(MFX_CONNECTET_PIN);
  if (ioe_irq_pending_status) 
  {
    BSP_IO_ITClear();   
    test_result |= IOE_GPIO_TEST_FAILED;
  }
  else
  {
    test_result |= IOE_GPIO_TEST_PASSED;
  }

  HAL_GPIO_WritePin(MCU_GPIO_PORT, MCU_GPIO_PIN, GPIO_PIN_RESET);
  ioe_irq_pending_status = BSP_IO_ITGetStatus(MFX_CONNECTET_PIN);
  if (ioe_irq_pending_status) 
  {
    BSP_IO_ITClear();   
    test_result |= IOE_GPIO_TEST_FAILED;
  }
  else
  {
    test_result |= IOE_GPIO_TEST_PASSED;
  }

  lcd_line += 15;
  if (test_result) 
  {
    BSP_LCD_DisplayStringAt(20, lcd_line, (uint8_t *)"IOE IO_MODE_IT_RISING_EDGE: FAILED", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, lcd_line, (uint8_t *)"IOE IO_MODE_IT_RISING_EDGE: PASSED", LEFT_MODE);
    all_test_fail = 0;
  }

  /* TEST IO_MODE_IT_FALLING_EDGE mode */
  /* ---------------------------------- */
  test_result = IOE_GPIO_TEST_PASSED;
  BSP_IO_ConfigPin(MFX_CONNECTET_PIN, IO_MODE_OFF);
  HAL_GPIO_WritePin(MCU_GPIO_PORT, MCU_GPIO_PIN, GPIO_PIN_RESET);
  BSP_IO_ITClear();   

  /* BSP_IO_ConfigPin(MFXIO_PIN_6, IO_MODE_INPUT); */
  BSP_IO_ConfigPin(MFX_CONNECTET_PIN, IO_MODE_IT_FALLING_EDGE_PU);
  /* We just want to test the good functioning of pending bit and ack on the IOE */
  HAL_NVIC_DisableIRQ((IRQn_Type)(MFX_IRQOUT_EXTI_IRQn));

  ioe_irq_pending_status = BSP_IO_ITGetStatus(MFX_CONNECTET_PIN);
  if (ioe_irq_pending_status) 
  {
    BSP_IO_ITClear();   
    test_result |= IOE_GPIO_TEST_FAILED;
  }
  else
  {
    test_result |= IOE_GPIO_TEST_PASSED;
  }

  HAL_GPIO_WritePin(MCU_GPIO_PORT, MCU_GPIO_PIN, GPIO_PIN_SET);
  HAL_Delay(1);
  ioe_irq_pending_status = BSP_IO_ITGetStatus(MFX_CONNECTET_PIN);
  if (ioe_irq_pending_status) 
  {
    BSP_IO_ITClear();   
    test_result |= IOE_GPIO_TEST_FAILED;
  }
  else
  {
    test_result |= IOE_GPIO_TEST_PASSED;
  }

  HAL_GPIO_WritePin(MCU_GPIO_PORT, MCU_GPIO_PIN, GPIO_PIN_RESET);
  HAL_Delay(1);
  ioe_irq_pending_status = BSP_IO_ITGetStatus(MFX_CONNECTET_PIN);
  if (ioe_irq_pending_status) 
  {
    BSP_IO_ITClear();   
    test_result |= IOE_GPIO_TEST_PASSED;
  }
  else
  {
    test_result |= IOE_GPIO_TEST_FAILED;
  }

  lcd_line += 15;
  if (test_result) 
  {
    BSP_LCD_DisplayStringAt(20, lcd_line, (uint8_t *)"IOE IO_MODE_IT_FALLING_EDGE: FAILED", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, lcd_line, (uint8_t *)"IOE IO_MODE_IT_FALLING_EDGE: PASSED", LEFT_MODE);
    all_test_fail = 0;
  }

  if (all_test_fail) 
  {
    BSP_LCD_DisplayStringAt(20, lcd_line + 20, (uint8_t *)"          all IOE tests FAILED !!!", LEFT_MODE);
    BSP_LCD_DisplayStringAt(15, lcd_line + 40, (uint8_t *)"Are you sure that MCU_PB4 pin is connected ", LEFT_MODE);
    BSP_LCD_DisplayStringAt(15, lcd_line + 55, (uint8_t *)"to TP9 probe with a wire on the board ??", LEFT_MODE);
  }

  /* We just want to test the good functioning of pending bit and ack on the IOE */
  HAL_NVIC_EnableIRQ((IRQn_Type)(MFX_IRQOUT_EXTI_IRQn));
	
  while (1)
  {    
    if(CheckForUserInput() > 0)
    {
      return;
    }
  }
}

/**
  * @brief  Set GPIO for receiving MFX IRQ_OUT.
  * @param  None
  * @retval None
  */
static void SetMcuGpioToBeConnectedToMfxGPO(void)
{
  GPIO_InitTypeDef GPIO_Init_Structure;

  /*  GPIO A4 configuration to receive IrqOutPin from MFX */
  __MCU_GPIO_PORTCLK_ENABLE();
  GPIO_Init_Structure.Pin       = MCU_GPIO_PIN;
  GPIO_Init_Structure.Mode      = GPIO_MODE_INPUT;
  GPIO_Init_Structure.Pull      = GPIO_PULLUP;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(MCU_GPIO_PORT, &GPIO_Init_Structure);
}

/**
  * @brief  Set GPIO for receiving MFX IRQ_OUT.
  * @param  None
  * @retval None
  */
static void SetMcuGpioToBeConnectedToMfxGPI(void)
{
  GPIO_InitTypeDef GPIO_Init_Structure;

  /*  GPIO A4 configuration to receive IrqOutPin from MFX */
  __MCU_GPIO_PORTCLK_ENABLE();
  GPIO_Init_Structure.Pin       = MCU_GPIO_PIN;
  GPIO_Init_Structure.Mode      = GPIO_MODE_OUTPUT_PP;
  GPIO_Init_Structure.Pull      = GPIO_PULLUP;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(MCU_GPIO_PORT, &GPIO_Init_Structure);

  HAL_GPIO_WritePin(MCU_GPIO_PORT, MCU_GPIO_PIN, GPIO_PIN_RESET);
}

/**
  * @brief  Display IOE_GPIO Demo Hint
  * @param  None
  * @retval None
  */
static void IOE_GPIO_SetHint(void)
{
  /* Clear the LCD */ 
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set LCD Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"IOE_GPIO", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"This test check the IO and exti ", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t *)"of the MFX expander Driver", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 60, (uint8_t *)"Connect the requested pins with a wire", CENTER_MODE);

   /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
  BSP_LCD_DrawRect(10, 90, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize()- 100);
  BSP_LCD_DrawRect(11, 91, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize()- 102);
  
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE); 
 }



/**
  * @}
  */ 

/**
  * @}
  */ 
