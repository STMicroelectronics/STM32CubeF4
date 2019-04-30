/**
  ******************************************************************************
  * @file    Examples_MIX/DMA2D/DMA2D_MemToMemWithLCD/Src/main.c
  * @author  MCD Application Team
  * @brief   This example provides a description of how to configure 
  *          DMA2D peripheral in Memory to Memory transfer mode 
  *          and display the result on LCD, in resorting to DMA2D LL APIs.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include "RGB565_128x160.h"

/** @addtogroup STM32F4xx_MIX_Examples
  * @{
  */

/** @addtogroup DMA2D_MemToMemWithLCD
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
typedef enum 
{
  SHIELD_NOT_DETECTED = 0, 
  SHIELD_DETECTED
}ShieldStatus;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* DMA2D output address in SRAM : this is the buffer displayed on LCD screen */
uint16_t aBufferResult[LAYER_SIZE_X * LAYER_SIZE_Y];
/* Blended image Ready flag */
__IO uint32_t   blended_image_ready = 0;

/* Private function prototypes -----------------------------------------------*/
static ShieldStatus TFT_ShieldDetect(void);
static void LCD_ImageDisplay(void);
static void DMA2D_Config(void);
static void SystemClock_Config(void);
void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  blended_image_ready = 0;

  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();
  
  /* Configure the system clock */
  SystemClock_Config(); 
    
  /* Configure LED1, LED2 and LED3 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);

  /* Check the availability of adafruit 1.8" TFT shield on top of STM32NUCLEO
     board. This is done by reading the state of IO PF.03 pin (mapped to 
     JoyStick available on adafruit 1.8" TFT shield). If the state of PF.03 
     is high then the adafruit 1.8" TFT shield is available. */
  
  if(TFT_ShieldDetect() != SHIELD_DETECTED)
  {
    Error_Handler();
  }

  /* Initialize the LCD */
  BSP_LCD_Init();
  
  /*##-2- DMA2D configuration ################################################*/
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA2D);
  NVIC_SetPriority(DMA2D_IRQn, 0);  
  NVIC_EnableIRQ(DMA2D_IRQn);
  
  DMA2D_Config();
  
  /*##-3- Start DMA2D transfer ###############################################*/  
  LL_DMA2D_FGND_SetMemAddr(DMA2D, (uint32_t)&RGB565_128x160);      /* Source buffer in format RGB565 and size 128x160                 */
  LL_DMA2D_SetOutputMemAddr(DMA2D, (uint32_t)aBufferResult);    /* Output Buffer                                                   */
  LL_DMA2D_ConfigSize(DMA2D, LAYER_SIZE_Y * LAYER_SIZE_X, 1);   /* Force 1 pixel per line and width in pixels x height in pixels   */
                                                                /* as number of lines to align DMA2D transfer to LCD configuration */ 
   
  /* Enable the transfer complete, transfer error and configuration error interrupts */
  LL_DMA2D_EnableIT_TC(DMA2D);
  LL_DMA2D_EnableIT_TE(DMA2D);
  LL_DMA2D_EnableIT_CE(DMA2D);  
  
  /* Enable the Peripheral */
  LL_DMA2D_Start(DMA2D);
  
  /*##-4- Wait until DMA2D transfer is over ################################################*/
  while(blended_image_ready == 0) {;}

  /*##-5- Display the image ################################################*/
  LCD_ImageDisplay();
  
  while (1)
  { 
    ; 
  }
}

/**
  * @brief DMA2D configuration.
  * @note  This function Configure tha DMA2D peripheral :
  *        1) Configure the transfer mode : memory to memory
  *        2) Configure the output color mode as RGB565
  *        3) Configure the transfer from FLASH to SRAM   
  *        4) Configure the data size : 128x160 (pixels)  
  * @retval
  *  None
  */
static void DMA2D_Config(void)
{
  /* Configure the DMA2D Color Mode */  
  LL_DMA2D_SetOutputColorMode(DMA2D, LL_DMA2D_OUTPUT_MODE_RGB565);
  
  /* Foreground Configuration:     */
  /* Set Alpha value to full opaque */
  LL_DMA2D_FGND_SetAlpha(DMA2D, 0xFF);
  /* Foreground layer format is RGB565 (16 bpp) */
  LL_DMA2D_FGND_SetColorMode(DMA2D, LL_DMA2D_INPUT_MODE_RGB565);
}

/**
  * @brief  On Error Handler on condition TRUE.
  * @param  condition : Can be TRUE or FALSE
  * @retval None
  */
void OnError_Handler(uint32_t condition)
{
  if(condition)
  {
    BSP_LED_On(LED2);
    while(1) { ; } /* Blocking on error */
  }
}

/**
  * @brief  Check the availability of adafruit 1.8" TFT shield on top of STM32NUCLEO
  *         board. This is done by reading the state of IO PF.03 pin (mapped to 
  *         JoyStick available on adafruit 1.8" TFT shield). If the state of PF.03 
  *         is high then the adafruit 1.8" TFT shield is available.
  * @param  None
  * @retval SHIELD_DETECTED: 1.8" TFT shield is available
  *         SHIELD_NOT_DETECTED: 1.8" TFT shield is not available
  */
static ShieldStatus TFT_ShieldDetect(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct; 

  /* Enable GPIO clock */
  NUCLEO_ADCx_GPIO_CLK_ENABLE();
  
  GPIO_InitStruct.Pin = NUCLEO_ADCx_GPIO_PIN ;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(NUCLEO_ADCx_GPIO_PORT , &GPIO_InitStruct);
  
  if(HAL_GPIO_ReadPin(NUCLEO_ADCx_GPIO_PORT, NUCLEO_ADCx_GPIO_PIN) != 0)
  {
    return SHIELD_DETECTED;
  }
  else
  {
    return SHIELD_NOT_DETECTED;
  }
}

/**
  * @brief  LCD display image on Adafruit LCD.
  * @param  None
  * @retval None
  */
static void LCD_ImageDisplay(void)
{
  int16_t line = 0;
  uint16_t pixel = 0;
  uint16_t *img_ptr = NULL;
  
  /* Set pointer to DMA2D output buffer */
  img_ptr = aBufferResult;
  line = LAYER_SIZE_Y - 1;
  while (line >= 0)
  {
    /* Write pixels */ 
    BSP_LCD_DrawPixel(pixel, line, (uint16_t)*img_ptr);
    img_ptr++;
    pixel++;
    if (pixel == LAYER_SIZE_X)
    {
      /* Move to next line */
      pixel = 0;
      line--;	
    }
  }
}

/**
  * @brief  This function handles the test program fail.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  BSP_LED_On(LED3);
  while(1)
  {
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 180000000
  *            HCLK(Hz)                       = 180000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 360
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    while(1) { ; }
  }
  
  if(HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    while(1) { ; }
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    while(1) { ; }
  }
}
#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
