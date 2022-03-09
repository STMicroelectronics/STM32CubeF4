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
#include <touchgfx/hal/OSWrappers.hpp>
#include <ResistiveTouchController.hpp>
#include <platform/driver/lcd/LCD16bpp.hpp>
#include <platform/driver/lcd/LCD24bpp.hpp>

#include <STM32F4HAL.hpp>
#include <STM32F4DMA.hpp>
#include <STM32F4Instrumentation.hpp>

#include "stm32f4xx.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_sdram.h"

#include "../Components/ili9341/ili9341.h"

/***********************************************************
 ******   Single buffer in internal RAM              *******
 ***********************************************************
 * On this platform, TouchGFX is able to run using a single
 * frame buffer in internal SRAM, thereby avoiding the need
 * for external SDRAM.
 * This feature was introduced in TouchGFX 4.7.0. To enable it,
 * uncomment the define below. The function touchgfx_init()
 * later in this file will check for this define and configure
 * TouchGFX accordingly.
 * For details on the single buffer strategy, please refer to
 * the knowledge base article "Single vs double buffering in TouchGFX"
 * on our support site.
 */
//#define SINGLE_FRAME_BUFFER_INTERNAL

/***********************************************************
 ******         24 Bits Per Pixel Support            *******
 ***********************************************************
 *
 * The default bit depth of the framebuffer is 16bpp. If you want 24bpp support, define the symbol "USE_BPP" with a value
 * of "24", e.g. "USE_BPP=24". This symbol affects the following:
 *
 * 1. Type of TouchGFX LCD (16bpp vs 24bpp)
 * 2. Bit depth of the framebuffer(s)
 * 3. TFT controller configuration.
 *
 * WARNING: Remember to modify your image formats accordingly in app/config/. Please see the following knowledgebase article
 * for further details on how to choose and configure the appropriate image formats for your application:
 *
 * https://touchgfx.zendesk.com/hc/en-us/articles/206725849
 */

#ifdef SINGLE_FRAME_BUFFER_INTERNAL
uint16_t frameBuffer[240 * 320];
static uint32_t frameBuf0 = (uint32_t)& frameBuffer[0];
#else
//Use start of SDRAM
static uint32_t frameBuf0 = (uint32_t)(0xd0000000);
#endif

#define LCD_FRAME_BUFFER frameBuf0

#define LCD_FRAME_BUFFER_LAYER0                  (LCD_FRAME_BUFFER+0x130000)
#define LCD_FRAME_BUFFER_LAYER1                  LCD_FRAME_BUFFER

LTDC_HandleTypeDef hltdc;
DMA2D_HandleTypeDef hdma2d;

static RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;
static LCD_DrvTypeDef *LcdDrv;

extern "C" {
uint32_t LCD_GetXSize(void)
{
  return LcdDrv->GetLcdPixelWidth();
}

/**
  * @brief  Gets the LCD Y size.  
  * @retval The used LCD Y size
  */
uint32_t LCD_GetYSize(void)
{
  return LcdDrv->GetLcdPixelHeight();
}
}

/**
  * @brief  Initializes the LTDC MSP.
  */
static void LCD_MspInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable the LTDC and DMA2D Clock */
    __HAL_RCC_LTDC_CLK_ENABLE();
    __HAL_RCC_DMA2D_CLK_ENABLE(); 

    /* Enable GPIOs clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();

    /* GPIOs Configuration */
    /*
    +------------------------+-----------------------+----------------------------+
    +                       LCD pins assignment                                   +
    +------------------------+-----------------------+----------------------------+
    |  LCD_TFT R2 <-> PC.10  |  LCD_TFT G2 <-> PA.06 |  LCD_TFT B2 <-> PD.06      |
    |  LCD_TFT R3 <-> PB.00  |  LCD_TFT G3 <-> PG.10 |  LCD_TFT B3 <-> PG.11      |
    |  LCD_TFT R4 <-> PA.11  |  LCD_TFT G4 <-> PB.10 |  LCD_TFT B4 <-> PG.12      |
    |  LCD_TFT R5 <-> PA.12  |  LCD_TFT G5 <-> PB.11 |  LCD_TFT B5 <-> PA.03      |
    |  LCD_TFT R6 <-> PB.01  |  LCD_TFT G6 <-> PC.07 |  LCD_TFT B6 <-> PB.08      |
    |  LCD_TFT R7 <-> PG.06  |  LCD_TFT G7 <-> PD.03 |  LCD_TFT B7 <-> PB.09      |
    -------------------------------------------------------------------------------
            |  LCD_TFT HSYNC <-> PC.06  | LCDTFT VSYNC <->  PA.04 |
            |  LCD_TFT CLK   <-> PG.07  | LCD_TFT DE   <->  PF.10 |
             -----------------------------------------------------
    */

    /* GPIOA configuration */
    GPIO_InitStructure.Pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_6 |
                             GPIO_PIN_11 | GPIO_PIN_12;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
    GPIO_InitStructure.Alternate= GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* GPIOB configuration */
    GPIO_InitStructure.Pin = GPIO_PIN_8 | \
                             GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* GPIOC configuration */
    GPIO_InitStructure.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_10;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* GPIOD configuration */
    GPIO_InitStructure.Pin = GPIO_PIN_3 | GPIO_PIN_6;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);

    /* GPIOF configuration */
    GPIO_InitStructure.Pin = GPIO_PIN_10;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStructure);     

    /* GPIOG configuration */  
    GPIO_InitStructure.Pin = GPIO_PIN_6 | GPIO_PIN_7 | \
                             GPIO_PIN_11;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);

    /* GPIOB configuration */  
    GPIO_InitStructure.Pin = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_InitStructure.Alternate= GPIO_AF9_LTDC;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* GPIOG configuration */  
    GPIO_InitStructure.Pin = GPIO_PIN_10 | GPIO_PIN_12;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);
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
    LCD_MspInit();

    /* On STM32F429I-DISCO, it is not possible to read ILI9341 ID because */
    /* PIN EXTC is not connected to VDD and then LCD_READ_ID4 is not accessible. */
    /* In this case, ReadID function is bypassed.*/  
    /*if(ili9341_drv.ReadID() == ILI9341_ID)*/

    /* LTDC Configuration ----------------------------------------------------*/
    hltdc.Instance = LTDC;

    /* Timing configuration  (Typical configuration from ILI9341 datasheet)
          HSYNC=10 (9+1)
          HBP=20 (29-10+1)
          ActiveW=240 (269-20-10+1)
          HFP=10 (279-240-20-10+1)
    
          VSYNC=2 (1+1)
          VBP=2 (3-2+1)
          ActiveH=320 (323-2-2+1)
          VFP=4 (327-320-2-2+1)
      */
    
    /* Configure horizontal synchronization width */
    hltdc.Init.HorizontalSync = ILI9341_HSYNC;
    /* Configure vertical synchronization height */
    hltdc.Init.VerticalSync = ILI9341_VSYNC;
    /* Configure accumulated horizontal back porch */
    hltdc.Init.AccumulatedHBP = ILI9341_HBP;
    /* Configure accumulated vertical back porch */
    hltdc.Init.AccumulatedVBP = ILI9341_VBP;
    /* Configure accumulated active width */
    hltdc.Init.AccumulatedActiveW = 269;
    /* Configure accumulated active height */
    hltdc.Init.AccumulatedActiveH = 323;
    /* Configure total width */
    hltdc.Init.TotalWidth = 279;
    /* Configure total height */
    hltdc.Init.TotalHeigh = 327;
    
    /* Configure R,G,B component values for LCD background color */
    hltdc.Init.Backcolor.Red= 0;
    hltdc.Init.Backcolor.Blue= 0;
    hltdc.Init.Backcolor.Green= 0;
    
    /* LCD clock configuration */
    /* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
    /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN = 192 Mhz */
    /* PLLLCDCLK = PLLSAI_VCO Output/PLLSAIR = 192/4 = 48 Mhz */
    /* LTDC clock frequency = PLLLCDCLK / LTDC_PLLSAI_DIVR_8 = 48/4 = 6Mhz */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
    PeriphClkInitStruct.PLLSAI.PLLSAIN = 192;
    PeriphClkInitStruct.PLLSAI.PLLSAIR = 4;
    PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_8;
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct); 
    
    /* Polarity */
    hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
    hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
    hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
    hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;

    HAL_LTDC_Init(&hltdc);

    /* Select the device */
    LcdDrv = &ili9341_drv;

    /* LCD Init */
    LcdDrv->Init();

    LCD_LayerDefaultInit(0, LCD_FRAME_BUFFER_LAYER1);

    return 0;
}

namespace touchgfx
{
STM32F4DMA dma;
ResistiveTouchController tc;
STM32F4Instrumentation mcuInstr;

#if !defined(USE_BPP) || USE_BPP==16
static LCD16bpp display;
static uint16_t bitdepth = 16;
#elif USE_BPP==24
static LCD24bpp display;
static uint16_t bitdepth = 24;
#else
#error Unknown USE_BPP
#endif

void touchgfx_init()
{
    uint16_t dispWidth = 240;
    uint16_t dispHeight = 320;
#if !defined(USE_BPP) || USE_BPP==16
    HAL& hal = touchgfx_generic_init<STM32F4HAL>(dma, display, tc, dispWidth, dispHeight,
                                                 (uint16_t*)(frameBuf0 + (dispWidth * dispHeight * 2) * 3),
                                                 2 * 1024 * 1024, 16);
#ifdef SINGLE_FRAME_BUFFER_INTERNAL
    //setup for single buffering
    hal.setFrameBufferStartAddress((uint16_t*)frameBuf0, bitdepth, false, false);

    // The optimized strategy for single buffering requires the presence of a
    // task delay function.
    hal.registerTaskDelayFunction(&OSWrappers::taskDelay);

    // Enable strategy.
    hal.setFrameRefreshStrategy(HAL::REFRESH_STRATEGY_OPTIM_SINGLE_BUFFER_TFT_CTRL);
#else
    //setup for double buffering.
    hal.setFrameBufferStartAddress((uint16_t*)frameBuf0, bitdepth);
#endif
#elif USE_BPP==24
    HAL& hal = touchgfx_generic_init<STM32F4HAL>(dma, display, tc, dispWidth, dispHeight,
                                                 (uint16_t*)(frameBuf0 + (dispWidth * dispHeight * 3) * 3),
                                                 2 * 1024 * 1024, 16);
#ifdef  SINGLE_FRAME_BUFFER_INTERNAL
#error Single frame buffer in internal is only possible in 16bpp due to memory constraints.
#endif
    hal.setFrameBufferStartAddress((uint16_t*)frameBuf0, bitdepth);
#else
#error Unknown USE_BPP
#endif
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
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
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

    /* Enable HSE Oscillator and activate PLL with HSE as source */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = 2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
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

    /* Initializes the SDRAM device */
    BSP_SDRAM_Init();

    /* Configure LCD */
    LCD_Init();

    GPIO::init();
}
}
