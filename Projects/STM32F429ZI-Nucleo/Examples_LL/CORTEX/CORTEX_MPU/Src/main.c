/**
  ******************************************************************************
  * @file    Examples_LL/CORTEX/CORTEX_MPU/Src/main.c
  * @author  MCD Application Team
  * @brief   This example presents the MPU features through
  *          the STM32F4xx CORTEX LL API.
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

/** @addtogroup STM32F4xx_LL_Examples
  * @{
  */

/** @addtogroup CORTEX_MPU
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ARRAY_ADDRESS_START                      (0x20002000UL)
#define ARRAY_SIZE                               LL_MPU_REGION_SIZE_32B
#define ARRAY_REGION_NUMBER                      LL_MPU_REGION_NUMBER3

#define EXAMPLE_RAM_ADDRESS_START                (0x20000000UL)
#define EXAMPLE_RAM_SIZE                         LL_MPU_REGION_SIZE_32KB
#define EXAMPLE_PERIPH_ADDRESS_START             (0x40000000)
#define EXAMPLE_PERIPH_SIZE                      LL_MPU_REGION_SIZE_512MB
#define EXAMPLE_FLASH_ADDRESS_START              (0x08000000)
#define EXAMPLE_FLASH_SIZE                       LL_MPU_REGION_SIZE_256KB
#define EXAMPLE_RAM_REGION_NUMBER                LL_MPU_REGION_NUMBER0
#define EXAMPLE_FLASH_REGION_NUMBER              LL_MPU_REGION_NUMBER1
#define EXAMPLE_PERIPH_REGION_NUMBER             LL_MPU_REGION_NUMBER2
#define portMPU_REGION_READ_WRITE                LL_MPU_REGION_FULL_ACCESS
#define portMPU_REGION_PRIVILEGED_READ_ONLY      LL_MPU_REGION_PRIV_RO
#define portMPU_REGION_READ_ONLY                 LL_MPU_REGION_PRIV_RO_URO
#define portMPU_REGION_PRIVILEGED_READ_WRITE     LL_MPU_REGION_PRIV_RW

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const uint16_t GPIO_PIN[LEDn] = {LED1_PIN, LED2_PIN, LED3_PIN};
#if defined ( __CC_ARM   )
uint8_t PrivilegedReadOnlyArray[32] __attribute__((at(0x20002000)));

#elif defined ( __ICCARM__ )
#pragma location=0x20002000
__no_init uint8_t PrivilegedReadOnlyArray[32];

#elif defined   (  __GNUC__  )
uint8_t PrivilegedReadOnlyArray[32] __attribute__((section(".ROarraySection")));

#endif

/* Private function prototypes -----------------------------------------------*/
void     SystemClock_Config(void);
void     Configure_MPU(void);
void     MPU_AccessPermConfig(void);
void     LED_Init(void);
void     LED_Blinking(uint32_t Period, Led_TypeDef Led);
void     UserButton_Init(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Configure the system clock to 180 MHz */
  SystemClock_Config();

  /* Configure LED1 and LED3 */
  LED_Init();

  /* Initialize button in EXTI mode */
  UserButton_Init();
  
  /* Set MPU regions */
  Configure_MPU();
  MPU_AccessPermConfig();
  
  /* Toggle quickly the LED in waiting for User-button press */
  LED_Blinking(LED_BLINK_FAST, LED1);

  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  Configures the main MPU regions.
  * @param  None
  * @retval None
  */
void Configure_MPU(void)
{
  /* Disable MPU */
  LL_MPU_Disable();

  /* Configure RAM region as Region N°0, 32KB of size and R/W region */
  LL_MPU_ConfigRegion(EXAMPLE_RAM_REGION_NUMBER, 0x00, EXAMPLE_RAM_ADDRESS_START, 
         EXAMPLE_RAM_SIZE | portMPU_REGION_READ_WRITE | LL_MPU_ACCESS_NOT_BUFFERABLE |
         LL_MPU_ACCESS_NOT_CACHEABLE | LL_MPU_ACCESS_NOT_SHAREABLE | LL_MPU_TEX_LEVEL0 |
         LL_MPU_INSTRUCTION_ACCESS_ENABLE);

  /* Configure FLASH region as REGION N°1, 256KB of size and R/W region */
  LL_MPU_ConfigRegion(EXAMPLE_FLASH_REGION_NUMBER, 0x00, EXAMPLE_FLASH_ADDRESS_START, 
         EXAMPLE_FLASH_SIZE | portMPU_REGION_READ_WRITE | LL_MPU_ACCESS_NOT_BUFFERABLE |
         LL_MPU_ACCESS_NOT_CACHEABLE | LL_MPU_ACCESS_NOT_SHAREABLE | LL_MPU_TEX_LEVEL0 |
         LL_MPU_INSTRUCTION_ACCESS_ENABLE);

  /* Configure Peripheral region as REGION N°2, 512MB of size, R/W and Execute
  Never region */
  LL_MPU_ConfigRegion(EXAMPLE_PERIPH_REGION_NUMBER, 0x00, EXAMPLE_PERIPH_ADDRESS_START, 
         EXAMPLE_PERIPH_SIZE | portMPU_REGION_READ_WRITE | LL_MPU_ACCESS_NOT_BUFFERABLE |
         LL_MPU_ACCESS_NOT_CACHEABLE | LL_MPU_ACCESS_NOT_SHAREABLE | LL_MPU_TEX_LEVEL0 |
         LL_MPU_INSTRUCTION_ACCESS_DISABLE);

  /* Enable MPU (any access not covered by any enabled region will cause a fault) */
  LL_MPU_Enable(LL_MPU_CTRL_PRIVILEGED_DEFAULT);
}

/**
  * @brief  This function configures the access right using Cortex-M4 MPU regions.
  * @param  None
  * @retval None
  */
void MPU_AccessPermConfig(void)
{
  /* Configure region for PrivilegedReadOnlyArray as REGION N°3, 32byte and R
     only in privileged mode */
  /* Disable MPU */
  LL_MPU_Disable();

  LL_MPU_ConfigRegion(ARRAY_REGION_NUMBER, 0x00, ARRAY_ADDRESS_START, 
         ARRAY_SIZE | portMPU_REGION_PRIVILEGED_READ_ONLY | LL_MPU_ACCESS_NOT_BUFFERABLE |
         LL_MPU_ACCESS_NOT_CACHEABLE | LL_MPU_ACCESS_NOT_SHAREABLE | LL_MPU_TEX_LEVEL0 |
         LL_MPU_INSTRUCTION_ACCESS_ENABLE);
  
  /* Enable MPU (any access not covered by any enabled region will cause a fault) */
  LL_MPU_Enable(LL_MPU_CTRL_PRIVILEGED_DEFAULT);
  LL_HANDLER_EnableFault(LL_HANDLER_FAULT_MEM);

  /* Read from PrivilegedReadOnlyArray. This will not generate error */
  (void)PrivilegedReadOnlyArray[0];
}

/**
  * @brief  Initialize LED1.
  * @param  None
  * @note   Peripheral configuration is minimal configuration from reset values.
  *         Thus, some useless LL unitary functions calls below are provided as
  *         commented examples - setting is default configuration from reset.
  * @retval None
  */
void LED_Init(void)
{
  /* Enable the GPIO_LED Clock */
  LEDx_GPIO_CLK_ENABLE();

  /* Configure LED1 and LED3 */
  LL_GPIO_SetPinMode(LEDx_GPIO_PORT, GPIO_PIN[LED1], LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinMode(LEDx_GPIO_PORT, GPIO_PIN[LED3], LL_GPIO_MODE_OUTPUT);
  /* Reset value is LL_GPIO_OUTPUT_PUSHPULL */
  //LL_GPIO_SetPinOutputType(LEDx_GPIO_PORT, GPIO_PIN[LED1] | GPIO_PIN[LED3], LL_GPIO_OUTPUT_PUSHPULL);
  /* Reset value is LL_GPIO_SPEED_FREQ_LOW */
  //LL_GPIO_SetPinSpeed(LEDx_GPIO_PORT, GPIO_PIN[LED1], LL_GPIO_SPEED_FREQ_LOW);
  //LL_GPIO_SetPinSpeed(LEDx_GPIO_PORT, GPIO_PIN[LED3], LL_GPIO_SPEED_FREQ_LOW);
  /* Reset value is LL_GPIO_PULL_NO */
  //LL_GPIO_SetPinPull(LEDx_GPIO_PORT, GPIO_PIN[LED1], LL_GPIO_PULL_NO);
  //LL_GPIO_SetPinPull(LEDx_GPIO_PORT, GPIO_PIN[LED3], LL_GPIO_PULL_NO);
}

/**
  * @brief  Set LED1 to Blinking mode for an infinite loop (toggle period based on value provided as input parameter).
  * @param  Period : Period of time (in ms) between each toggling of LED
  *   This parameter can be user defined values. Pre-defined values used in that example are :
  *     @arg LED_BLINK_FAST : Fast Blinking
  *     @arg LED_BLINK_SLOW : Slow Blinking
  *     @arg LED_BLINK_ERROR : Error specific Blinking
  * @param  Led: Specifies the Led to be configured. 
  *   This parameter can be one of following parameters:
  *     @arg  LED1
  *     @arg  LED2
  *     @arg  LED3
  * @retval None
  */
void LED_Blinking(uint32_t Period, Led_TypeDef Led)
{
  /* Toggle IO in an infinite loop */
  while (1)
  {
    LL_GPIO_TogglePin(LEDx_GPIO_PORT, GPIO_PIN[Led]);  
    LL_mDelay(Period);
  }
}

/**
  * @brief  Configures User push-button in EXTI Line Mode.
  * @retval None
  */
void UserButton_Init(void)
{
  /* Enable the BUTTON Clock */
  USER_BUTTON_GPIO_CLK_ENABLE();
  
  /* Configure GPIO for BUTTON */
  LL_GPIO_SetPinMode(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_PULL_NO);
  
  /* Configure NVIC for USER_BUTTON_EXTI_IRQn */
  NVIC_EnableIRQ(USER_BUTTON_EXTI_IRQn); 
  NVIC_SetPriority(USER_BUTTON_EXTI_IRQn,0x03);  
  
  /* Connect External Line to the GPIO*/
  USER_BUTTON_SYSCFG_SET_EXTI();
  
  /* Enable a rising trigger EXTI line 13 Interrupt */
  USER_BUTTON_EXTI_LINE_ENABLE();
  USER_BUTTON_EXTI_FALLING_TRIG_ENABLE();
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 180000000
  *            HCLK(Hz)                       = 180000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 360
  *            PLL_P                          = 2
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  /* Enable HSE oscillator */
  LL_RCC_HSE_Enable();
  while(LL_RCC_HSE_IsReady() != 1)
  {
  };

  /* Set FLASH latency */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_5);

  /* Enable PWR clock */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* Activation OverDrive Mode */
  LL_PWR_EnableOverDriveMode();
  while(LL_PWR_IsActiveFlag_OD() != 1)
  {
  };

  /* Activation OverDrive Switching */
  LL_PWR_EnableOverDriveSwitching();
  while(LL_PWR_IsActiveFlag_ODSW() != 1)
  {
  };

  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_8, 360, LL_RCC_PLLP_DIV_2);
  LL_RCC_PLL_Enable();
  while(LL_RCC_PLL_IsReady() != 1)
  {
  };

  /* Sysclk activation on the main PLL */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  };

  /* Set APB1 & APB2 prescaler */
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  /* Set systick to 1ms */
  SysTick_Config(180000000 / 1000);

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  SystemCoreClock = 180000000;
}

/******************************************************************************/
/*   USER IRQ HANDLER TREATMENT                                               */
/******************************************************************************/
/**
  * @brief  Function to manage User button IRQ Handler
  * @param  None
  * @retval None
  */
void UserButton_Callback(void)
{
  /* This will generate error */
  PrivilegedReadOnlyArray[0] = 'e';
}

/**
  * @brief  Function to manage memory exception
  * @param  None
  * @retval None
  */
void MemManage_Callback(void) 
{
  /* Turn on LED3 */
  LL_GPIO_SetOutputPin(LEDx_GPIO_PORT, GPIO_PIN[LED3]);

  /* Turn off LED1 */
  LL_GPIO_ResetOutputPin(LEDx_GPIO_PORT, GPIO_PIN[LED1]);

  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
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
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d", file, line) */

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
