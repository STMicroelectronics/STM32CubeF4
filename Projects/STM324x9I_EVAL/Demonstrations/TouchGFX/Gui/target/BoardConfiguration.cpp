/**
  ******************************************************************************
  * This file is part of the TouchGFX 4.10.0 distribution.
  *
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
  


#include <common/TouchGFXInit.hpp>
#include <touchgfx/hal/BoardConfiguration.hpp>
#include <touchgfx/hal/GPIO.hpp>
#include <platform/driver/lcd/LCD16bpp.hpp>
#include <platform/driver/lcd/LCD24bpp.hpp>

#include <STM32F4HAL.hpp>
#include <STM32F4DMA.hpp>
#include <STM32F4Instrumentation.hpp>

#include <TS3510TouchController.hpp>

#include "stm32f4xx.h"

#include "stm324x9i_eval.h"
#include "stm324x9i_eval_sram.h"
#include "stm324x9i_eval_sdram.h"
#include "stm324x9i_eval_nor.h"

#include "../Components/ampire480272/ampire480272.h"

static uint32_t frameBuf0 = (uint32_t)(0xC0000000);

LTDC_HandleTypeDef hltdc;
DMA2D_HandleTypeDef hdma2d;

extern "C" {
uint32_t LCD_GetXSize(void)
{
  return AMPIRE480272_WIDTH;
}

uint32_t LCD_GetYSize(void)
{
  return AMPIRE480272_HEIGHT;
}
}

static void LCD_MspInit(void)
{
    GPIO_InitTypeDef GPIO_Init_Structure;

    /* Enable the LTDC and DMA2D clocks */
    __HAL_RCC_LTDC_CLK_ENABLE();
    __HAL_RCC_DMA2D_CLK_ENABLE();

    /* Enable GPIOs clock */
    __HAL_RCC_GPIOI_CLK_ENABLE();
    __HAL_RCC_GPIOJ_CLK_ENABLE();
    __HAL_RCC_GPIOK_CLK_ENABLE();

    /*** LTDC Pins configuration ***/
    /* GPIOI configuration */
    GPIO_Init_Structure.Pin       = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
    GPIO_Init_Structure.Pull      = GPIO_NOPULL;
    GPIO_Init_Structure.Speed     = GPIO_SPEED_FAST;
    GPIO_Init_Structure.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOI, &GPIO_Init_Structure);

    /* GPIOJ configuration */  
    GPIO_Init_Structure.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                                    GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 |
                                    GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 |
                                    GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
    GPIO_Init_Structure.Pull      = GPIO_NOPULL;
    GPIO_Init_Structure.Speed     = GPIO_SPEED_FAST;
    GPIO_Init_Structure.Alternate = GPIO_AF14_LTDC;  
    HAL_GPIO_Init(GPIOJ, &GPIO_Init_Structure);  

    /* GPIOK configuration */  
    GPIO_Init_Structure.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                                    GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
    GPIO_Init_Structure.Pull      = GPIO_NOPULL;
    GPIO_Init_Structure.Speed     = GPIO_SPEED_FAST;
    GPIO_Init_Structure.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOK, &GPIO_Init_Structure);
}

/**
  * @brief  Initializes the LCD layers.
  * @param  LayerIndex: the layer foreground or background.
  * @param  FB_Address: the layer frame buffer.
  */
static void LCD_LayerDefaultInit(uint16_t LayerIndex, uint32_t FB_Address)
{
    LTDC_LayerCfgTypeDef Layercfg;

    /* Layer Init */
    Layercfg.WindowX0 = 0;
    Layercfg.WindowX1 = LCD_GetXSize();
    Layercfg.WindowY0 = 0;
    Layercfg.WindowY1 = LCD_GetYSize();
#if USE_BPP == 16
    Layercfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
#elif USE_BPP == 24
    Layercfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB888;
#else
#error Unknown USE_BPP
#endif
    Layercfg.FBStartAdress = FB_Address;
    Layercfg.Alpha = 255;
    Layercfg.Alpha0 = 0;
    Layercfg.Backcolor.Blue = 0;
    Layercfg.Backcolor.Green = 0;
    Layercfg.Backcolor.Red = 0;
    Layercfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
    Layercfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
    Layercfg.ImageWidth = LCD_GetXSize();
    Layercfg.ImageHeight = LCD_GetYSize();

    HAL_LTDC_ConfigLayer(&hltdc, &Layercfg, LayerIndex);

    /* Dithering activation */
    HAL_LTDC_EnableDither(&hltdc);
}

/**
  * @brief  Initializes the LCD.
  * @retval LCD state
  */
static uint8_t LCD_Init(void)
{
    RCC_PeriphCLKInitTypeDef periph_clk_init_struct;

    hltdc.Instance = LTDC;

    hltdc.Init.HorizontalSync = (AMPIRE480272_HSYNC - 1);
    hltdc.Init.VerticalSync = (AMPIRE480272_VSYNC - 1);
    hltdc.Init.AccumulatedHBP = (AMPIRE480272_HSYNC + AMPIRE480272_HBP - 1);
    hltdc.Init.AccumulatedVBP = (AMPIRE480272_VSYNC + AMPIRE480272_VBP - 1);
    hltdc.Init.AccumulatedActiveH = (AMPIRE480272_HEIGHT + AMPIRE480272_VSYNC + AMPIRE480272_VBP - 1);
    hltdc.Init.AccumulatedActiveW = (AMPIRE480272_WIDTH + AMPIRE480272_HSYNC + AMPIRE480272_HBP - 1);
    hltdc.Init.TotalHeigh = (AMPIRE480272_HEIGHT + AMPIRE480272_VSYNC + AMPIRE480272_VBP + AMPIRE480272_VFP - 1);
    hltdc.Init.TotalWidth = (AMPIRE480272_WIDTH + AMPIRE480272_HSYNC + AMPIRE480272_HBP + AMPIRE480272_HFP - 1);
  
    /* Initialize the LCD pixel width and pixel height */
    hltdc.LayerCfg->ImageWidth  = AMPIRE480272_WIDTH;
    hltdc.LayerCfg->ImageHeight = AMPIRE480272_HEIGHT;
    
    /* Background value */
    hltdc.Init.Backcolor.Blue = 0;
    hltdc.Init.Backcolor.Green = 0;
    hltdc.Init.Backcolor.Red = 0;

    /* Polarity */
    hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
    hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
    hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
    hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;

    LCD_MspInit();

    /* AMPIRE480272 LCD clock configuration */
    /* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
    /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN = 192 Mhz */
    /* PLLLCDCLK = PLLSAI_VCO Output/PLLSAIR = 192/5 = 38.4 Mhz */
    /* LTDC clock frequency = PLLLCDCLK / LTDC_PLLSAI_DIVR_4 = 38.4/4 = 9.6Mhz */
    periph_clk_init_struct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
    periph_clk_init_struct.PLLSAI.PLLSAIN = 192;
    periph_clk_init_struct.PLLSAI.PLLSAIR = AMPIRE480272_FREQUENCY_DIVIDER;
    periph_clk_init_struct.PLLSAIDivR = RCC_PLLSAIDIVR_4;
    HAL_RCCEx_PeriphCLKConfig(&periph_clk_init_struct);

    HAL_LTDC_Init(&hltdc);

    LCD_LayerDefaultInit(0, frameBuf0);

    return 0;
}

namespace touchgfx
{
STM32F4DMA dma;
TS3510TouchController tc;
STM32F4Instrumentation mcuInstr;
#if !defined(USE_BPP) || USE_BPP==16
static LCD16bpp display;
static uint16_t bitdepth = 16;
#else
static LCD24bpp display;
static uint16_t bitdepth = 24;
#endif

void touchgfx_init()
{
#if !defined(USE_BPP) || USE_BPP==16
    HAL& hal = touchgfx_generic_init<STM32F4HAL>(dma, display, tc, AMPIRE480272_WIDTH, AMPIRE480272_HEIGHT,
                                                 (uint16_t*)(frameBuf0 + (AMPIRE480272_WIDTH * AMPIRE480272_HEIGHT * 2) * 3),
                                                 1024 * 1024, 16);
#elif USE_BPP==24
    HAL& hal = touchgfx_generic_init<STM32F4HAL>(dma, display, tc, AMPIRE480272_WIDTH, AMPIRE480272_HEIGHT,
                                                 (uint16_t*)(frameBuf0 + (AMPIRE480272_WIDTH * AMPIRE480272_HEIGHT * 3) * 3),
                                                 1024 * 1024, 16);
#else
#error Unknown USE_BPP
#endif
    hal.setFrameBufferStartAddress((uint16_t*)frameBuf0, bitdepth);

    hal.setTouchSampleRate(2);
    hal.setFingerSize(1);

    // By default frame rate compensation is off.
    // Enable frame rate compensation to smooth out animations in case there is periodic slow frame rates.
    hal.setFrameRateCompensation(false);

    // This platform can handle simultaneous DMA and TFT accesses to SDRAM, so disable lock to increase performance.
    hal.lockDMAToFrontPorch(false);

    mcuInstr.init();

    //Set MCU instrumentation and Load calculation
    hal.setMCUInstrumentation(&mcuInstr);
    hal.enableMCULoadCalculation(true);
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 168000000
  *            HCLK(Hz)                       = 168000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 336
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

    /* Enable HSE Oscillator and Activate PLL with HSE ad source */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 25;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}

void hw_init()
{
    HAL_Init();

    __HAL_RCC_CRC_CLK_ENABLE();

    /* Configure the system clock to 168 MHz */
    SystemClock_Config();

    /* Initialize the SDRAM */
    BSP_SDRAM_Init();

    /* Initialize the NOR flash */
    BSP_NOR_Init();

    /* Configure LCD */
    LCD_Init();

    GPIO::init();
}
}
