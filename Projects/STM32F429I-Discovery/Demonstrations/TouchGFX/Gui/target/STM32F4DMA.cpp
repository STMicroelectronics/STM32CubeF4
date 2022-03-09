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
  


#include <touchgfx/hal/OSWrappers.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <cassert>
#include <STM32F4DMA.hpp>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_dma2d.h"

static HAL_StatusTypeDef HAL_DMA2D_SetMode(DMA2D_HandleTypeDef *hdma2d, uint32_t mode, uint32_t color, uint32_t offset)
{
    assert_param(IS_DMA2D_ALL_INSTANCE(hdma2d->Instance));

    MODIFY_REG(hdma2d->Instance->CR, DMA2D_CR_MODE, mode);
    MODIFY_REG(hdma2d->Instance->OPFCCR, DMA2D_OPFCCR_CM, color);
    MODIFY_REG(hdma2d->Instance->OOR, DMA2D_OOR_LO, offset);

    return HAL_OK;
}

extern DMA2D_HandleTypeDef hdma2d;

extern "C" {

static void DMA2D_XferCpltCallback(DMA2D_HandleTypeDef* handle)
{
    touchgfx::HAL::getInstance()->signalDMAInterrupt();
}

static void DMA2D_XferErrorCallback(DMA2D_HandleTypeDef* handle)
{
    //assert(0);
}

}

STM32F4DMA::STM32F4DMA()
    : DMA_Interface(dma_queue), dma_queue(queue_storage, sizeof(queue_storage) / sizeof(queue_storage[0]))
{}

STM32F4DMA::~STM32F4DMA()
{
    HAL_DMA2D_DeInit(&hdma2d);
    NVIC_DisableIRQ(DMA2D_IRQn);
}

void STM32F4DMA::initialize()
{
    hdma2d.Instance = DMA2D;
    HAL_DMA2D_Init(&hdma2d);

    hdma2d.XferCpltCallback = DMA2D_XferCpltCallback;
    hdma2d.XferErrorCallback = DMA2D_XferErrorCallback;

    NVIC_EnableIRQ(DMA2D_IRQn);
}

volatile bool disableChromArt = false;

BlitOperations STM32F4DMA::getBlitCaps()
{
    if (disableChromArt)
    {
        return static_cast<BlitOperations>(0);
    }
    else
    {
        return static_cast<BlitOperations>(BLIT_OP_FILL
                                           | BLIT_OP_FILL_WITH_ALPHA
                                           | BLIT_OP_COPY
                                           | BLIT_OP_COPY_WITH_ALPHA
                                           | BLIT_OP_COPY_ARGB8888
                                           | BLIT_OP_COPY_ARGB8888_WITH_ALPHA
                                           | BLIT_OP_COPY_A4
                                           | BLIT_OP_COPY_A8);
    }
}

void STM32F4DMA::setupDataCopy(const BlitOp& blitOp)
{
    uint32_t dma2dTransferMode = DMA2D_M2M_BLEND;
    uint32_t dma2dColorMode = 0;

    bool blendingImage = (blitOp.operation == BLIT_OP_COPY_ARGB8888
                          || blitOp.operation == BLIT_OP_COPY_ARGB8888_WITH_ALPHA
                          || blitOp.operation == BLIT_OP_COPY_WITH_ALPHA);

    bool blendingText = (blitOp.operation == BLIT_OP_COPY_A4
                         || blitOp.operation == BLIT_OP_COPY_A8);

    uint8_t bitDepth = HAL::lcd().bitDepth();

    switch (blitOp.operation)
    {
    case BLIT_OP_COPY_A4:
        dma2dColorMode = CM_A4;
        break;
    case BLIT_OP_COPY_A8:
        dma2dColorMode = CM_A8;
        break;
    case BLIT_OP_COPY_WITH_ALPHA:
        dma2dTransferMode = DMA2D_M2M_BLEND;
        dma2dColorMode = (bitDepth == 16) ? CM_RGB565 : CM_RGB888;
        break;
    case BLIT_OP_COPY_ARGB8888:
    case BLIT_OP_COPY_ARGB8888_WITH_ALPHA:
        dma2dColorMode = CM_ARGB8888;
        break;
    default:
        dma2dTransferMode = DMA2D_M2M;
        dma2dColorMode = (bitDepth == 16) ? CM_RGB565 : CM_RGB888;
        break;
    }

    /* HAL_DMA2D_ConfigLayer() depends on hdma2d.Init! */
    hdma2d.Init.Mode = dma2dTransferMode;
    hdma2d.Init.ColorMode = (bitDepth == 16) ? DMA2D_RGB565 : DMA2D_RGB888;
    hdma2d.Init.OutputOffset = blitOp.dstLoopStride - blitOp.nSteps;

    HAL_DMA2D_SetMode(&hdma2d, dma2dTransferMode,
                      (bitDepth == 16) ? DMA2D_RGB565 : DMA2D_RGB888,
                      blitOp.dstLoopStride - blitOp.nSteps);

    hdma2d.LayerCfg[1].InputColorMode = dma2dColorMode;
    hdma2d.LayerCfg[1].InputOffset = blitOp.srcLoopStride - blitOp.nSteps;

    if (blendingImage || blendingText)
    {
        if (blitOp.alpha < 255)
        {
            hdma2d.LayerCfg[1].AlphaMode = DMA2D_COMBINE_ALPHA;
            hdma2d.LayerCfg[1].InputAlpha = blitOp.alpha;
        }
        else
        {
            hdma2d.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
        }

        if (blendingText)
        {
            if (bitDepth == 16)
            {
                uint32_t red = (((blitOp.color & 0xF800) >> 11) * 255) / 31;
                uint32_t green = (((blitOp.color & 0x7E0) >> 5) * 255) / 63;
                uint32_t blue = (((blitOp.color & 0x1F)) * 255) / 31;
                uint32_t alpha = blitOp.alpha;
                hdma2d.LayerCfg[1].InputAlpha = (alpha << 24) | (red << 16) | (green << 8) | blue;
            }
            else
            {
                hdma2d.LayerCfg[1].InputAlpha = blitOp.color.getColor32() | (blitOp.alpha << 24);
            }
        }

        hdma2d.LayerCfg[0].InputOffset = blitOp.dstLoopStride - blitOp.nSteps;
        hdma2d.LayerCfg[0].InputColorMode = (bitDepth == 16) ? CM_RGB565 : CM_RGB888;

        HAL_DMA2D_ConfigLayer(&hdma2d, 0);
    }

    HAL_DMA2D_ConfigLayer(&hdma2d, 1);

    if (blendingImage || blendingText)
    {
        HAL_DMA2D_BlendingStart_IT(&hdma2d,
                                   (unsigned int)blitOp.pSrc,
                                   (unsigned int)blitOp.pDst,
                                   (unsigned int)blitOp.pDst,
                                   blitOp.nSteps, blitOp.nLoops);
    }
    else
    {
        HAL_DMA2D_Start_IT(&hdma2d,
                           (unsigned int)blitOp.pSrc,
                           (unsigned int)blitOp.pDst,
                           blitOp.nSteps, blitOp.nLoops);
    }
}

void STM32F4DMA::setupDataFill(const BlitOp& blitOp)
{
    uint8_t bitDepth = HAL::lcd().bitDepth();
    uint32_t dma2dTransferMode;
    uint32_t dma2dColorMode = (bitDepth == 16) ? CM_RGB565 : CM_RGB888;

    uint32_t color = 0;
    if (bitDepth == 16)
    {
        uint32_t red = (((blitOp.color & 0xF800) >> 11) * 255) / 31;
        uint32_t green = (((blitOp.color & 0x7E0) >> 5) * 255) / 63;
        uint32_t blue = (((blitOp.color & 0x1F)) * 255) / 31;
        uint32_t alpha = blitOp.alpha;
        color = (alpha << 24) | (red << 16) | (green << 8) | blue;
    }
    else
    {
        color = (blitOp.alpha << 24) | blitOp.color.getColor32();
    }

    switch (blitOp.operation)
    {
    case BLIT_OP_FILL_WITH_ALPHA:
        dma2dTransferMode = DMA2D_M2M_BLEND;
        break;
    default:
        dma2dTransferMode = DMA2D_R2M;
        break;
    };

    /* HAL_DMA2D_ConfigLayer() depends on hdma2d.Init! */
    hdma2d.Init.Mode = dma2dTransferMode;
    hdma2d.Init.ColorMode = (bitDepth == 16) ? DMA2D_RGB565 : DMA2D_RGB888;
    hdma2d.Init.OutputOffset = blitOp.dstLoopStride - blitOp.nSteps;

    HAL_DMA2D_SetMode(&hdma2d, dma2dTransferMode,
                      (bitDepth == 16) ? DMA2D_RGB565 : DMA2D_RGB888,
                      blitOp.dstLoopStride - blitOp.nSteps);

    if (dma2dTransferMode == DMA2D_M2M_BLEND) {
        hdma2d.LayerCfg[1].AlphaMode = DMA2D_REPLACE_ALPHA;
        hdma2d.LayerCfg[1].InputAlpha = color;
        hdma2d.LayerCfg[1].InputColorMode = CM_A8;

        hdma2d.LayerCfg[0].InputOffset = blitOp.dstLoopStride - blitOp.nSteps;
        hdma2d.LayerCfg[0].InputColorMode = (bitDepth == 16) ? CM_RGB565 : CM_RGB888;
        HAL_DMA2D_ConfigLayer(&hdma2d, 0);
    } else {
        hdma2d.LayerCfg[1].InputColorMode = dma2dColorMode;
        hdma2d.LayerCfg[1].InputOffset = 0;
    }

    HAL_DMA2D_ConfigLayer(&hdma2d, 1);

    if (dma2dTransferMode == DMA2D_M2M_BLEND)
        HAL_DMA2D_BlendingStart_IT(&hdma2d,
                                   (unsigned int)blitOp.pDst,
                                   (unsigned int)blitOp.pDst,
                                   (unsigned int)blitOp.pDst,
                                   blitOp.nSteps, blitOp.nLoops);
    else
        HAL_DMA2D_Start_IT(&hdma2d, color, (unsigned int)blitOp.pDst,
                           blitOp.nSteps, blitOp.nLoops);
}
