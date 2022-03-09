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
  


#ifndef STM32F4HAL_DSI_HPP
#define STM32F4HAL_DSI_HPP

#include <touchgfx/hal/HAL.hpp>
#include <platform/driver/touch/TouchController.hpp>

/**
 * @class STM32F4HAL_DSI STM32F4HAL_DSI.hpp platform/hal/ST/mcu/stm32f4x9/STM32F4HAL_DSI.hpp
 *
 * @brief HAL implementation for STM32F4, for DSI displays
 *
 *        HAL implementation for STM32F4. Based on the ST CubeF4 API. This version uses the DSI display peripheral.
 *        This HAL implementation uses DSI in adapted command mode, and supports both single- and double buffering.
 *        Please refer to the implementation cpp file for details on this implementation.
 *
 * @sa HAL
 */
class STM32F4HAL_DSI : public touchgfx::HAL
{
public:
    /**
     * @fn STM32F4HAL_DSI::STM32F4HAL_DSI(touchgfx::DMA_Interface& dma, touchgfx::LCD& display, touchgfx::TouchController& tc, uint16_t width, uint16_t height) : touchgfx::HAL(dma, display, tc, width, height)
     *
     * @brief Constructor.
     *
     *        Constructor. Initializes members.
     *
     * @param [in,out] dma     Reference to DMA interface.
     * @param [in,out] display Reference to LCD interface.
     * @param [in,out] tc      Reference to Touch Controller driver.
     * @param width            Width of the display.
     * @param height           Height of the display.
     */
    STM32F4HAL_DSI(touchgfx::DMA_Interface& dma, touchgfx::LCD& display, touchgfx::TouchController& tc, uint16_t width, uint16_t height) : touchgfx::HAL(dma, display, tc, width, height)
    {
    }

    /**
     * @fn virtual void STM32F4HAL_DSI::disableInterrupts();
     *
     * @brief Disables the DMA and LCD interrupts.
     *
     *        Disables the DMA and LCD interrupts.
     */
    virtual void disableInterrupts();

    /**
     * @fn virtual void STM32F4HAL_DSI::enableInterrupts();
     *
     * @brief Enables the DMA and LCD interrupts.
     *
     *        Enables the DMA and LCD interrupts.
     */
    virtual void enableInterrupts();

    /**
     * @fn virtual void STM32F4HAL_DSI::configureInterrupts();
     *
     * @brief Sets the DMA and LCD interrupt priorities.
     *
     *        Sets the DMA and LCD interrupt priorities.
     */
    virtual void configureInterrupts();

    /**
     * @fn virtual void STM32F4HAL_DSI::enableLCDControllerInterrupt();
     *
     * @brief Configure the LCD controller to fire interrupts at VSYNC.
     *
     *        Configure the LCD controller to fire interrupts at VSYNC. Called automatically
     *        once TouchGFX initialization has completed.
     */
    virtual void enableLCDControllerInterrupt();

protected:
    /**
     * @fn virtual uint16_t* STM32F4HAL_DSI::getTFTFrameBuffer() const;
     *
     * @brief Gets the frame buffer address used by the TFT controller.
     *
     *        Gets the frame buffer address used by the TFT controller.
     *
     * @return The address of the frame buffer currently being displayed on the TFT.
     */
    virtual uint16_t* getTFTFrameBuffer() const;

    /**
     * @fn virtual void STM32F4HAL_DSI::setTFTFrameBuffer(uint16_t* adr);
     *
     * @brief Sets the frame buffer address used by the TFT controller.
     *
     *        Sets the frame buffer address used by the TFT controller.
     *
     * @param [in,out] adr New frame buffer address.
     */
    virtual void setTFTFrameBuffer(uint16_t* adr);

    /**
     * @fn virtual void setFrameBufferStartAddress(void* adr, uint16_t depth = 16, bool useDoubleBuffering = true, bool useAnimationStorage = true);
     *
     * This function is overridden in order to detect whether it is used in single- or double buffering mode.
     * @param [in] adr            Starting address to use for frame buffers.
     * @param depth               Depth of each pixel in bits, default is 16.
     * @param useDoubleBuffering  If true, reserve memory for an extra frame buffer.
     * @param useAnimationStorage If true, reserve memory for animation storage.
     * @note Will also call the base implementation.
     */
    virtual void setFrameBufferStartAddress(void* adr, uint16_t depth = 16, bool useDoubleBuffering = true, bool useAnimationStorage = true);

    /**
     * This function is overridden to detect whether there are any frame buffer changes in this frame.
     * @return The value of the base implementation.
     */
    virtual bool beginFrame();

    /**
     * This function is overridden to detect whether there are any frame buffer changes in this frame.
     * @note Will also call the base implementation.
     */
    virtual void endFrame();
};

#endif // STM32F4HAL_DSI_HPP
