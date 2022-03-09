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
  


#ifndef PAINTERVERTICALALPHA_HPP
#define PAINTERVERTICALALPHA_HPP

#include <touchgfx/hal/Types.hpp>
#include <stdint.h>
#if !defined(USE_BPP) || USE_BPP==16
#include <touchgfx/widgets/canvas/AbstractPainterRGB565.hpp>
#elif USE_BPP==24
#include <touchgfx/widgets/canvas/AbstractPainterRGB888.hpp>
#elif USE_BPP==4
#include <touchgfx/widgets/canvas/AbstractPainterGRAY4.hpp>
#include <platform/driver/lcd/LCD4bpp.hpp>
#elif USE_BPP==2
#include <touchgfx/widgets/canvas/AbstractPainterGRAY2.hpp>
#include <platform/driver/lcd/LCD2bpp.hpp>
#else
#error Unknown USE_BPP
#endif

using namespace touchgfx;

/**
 * @class PainterVerticalAlpha PainterVerticalAlpha.hpp gui/common/PainterVerticalAlpha.hpp
 *
 * @brief A Painter that will paint using a fading color.
 *
 *        PainterVerticalAlpha will paint using the given color at y coordinate 0 fading
 *        the color to invisible at the given y coordinate.
 *
 * @see AbstractPainter
 */
class PainterVerticalAlpha :
#if !defined(USE_BPP) || USE_BPP==16
    public AbstractPainterRGB565
#elif USE_BPP==24
    public AbstractPainterRGB888
#elif USE_BPP==4
    public AbstractPainterGRAY4
#elif USE_BPP==2
    public AbstractPainterGRAY2
#else
#error Unknown USE_BPP
#endif
{
public:

    /**
     * @fn PainterVerticalAlpha::PainterVerticalAlpha(colortype color = 0, int alpha0atY = 100);
     *
     * @brief Constructor.
     *
     *        Constructor.
     *
     * @param color     The color.
     * @param alpha0atY The y coordinate where alpha has faded to 0.
     */
    PainterVerticalAlpha(colortype color = 0, int alpha0atY = 100);

    /**
     * @fn void PainterVerticalAlpha::setColor(colortype color, int alphaZeroAt);
     *
     * @brief Sets color and alpha to use when drawing the CanvasWidget.
     *
     *        Sets color and alpha to use when drawing the CanvasWidget.
     *
     * @param color       The color.
     * @param alphaZeroAt The y coordinate where alpha has faded to 0.
     */
    void setColor(colortype color, int alphaZeroAt);

    virtual void render(uint8_t* ptr, int x, int xAdjust, int y, unsigned count, const uint8_t* covers);

protected:
    /**
     * @fn virtual bool PainterVerticalAlpha::renderInit();
     *
     * @brief Get ready to render (part of) a scanline.
     *
     *        Get ready to render (part of) a scanline. Since the scanline will have the same y
     *        coordinate, the alpha for the scanline can be calculated here and used for all
     *        pixels in the scanline.
     *
     * @return true if it succeeds, false if it fails.
     */
    virtual bool renderInit();

#if !defined(USE_BPP) || USE_BPP==16 || USE_BPP==24
    virtual bool renderNext(uint8_t& pixelRed, uint8_t& pixelGreen, uint8_t& pixelBlue, uint8_t& pixelAlpha);
    uint8_t painterRed;   ///< The red part of the color
    uint8_t painterGreen; ///< The green part of the color
    uint8_t painterBlue;  ///< The blue part of the color
#elif USE_BPP==4 || USE_BPP==2
    virtual bool renderNext(uint8_t& gray, uint8_t& alpha);
    uint8_t painterGray;   ///< The gray color
#else
#error Unknown USE_BPP
#endif

    uint8_t painterAlpha; ///< Current alpha for the scan line
    int alpha0atY; ///< The Y coordinate where alpha has faded to 0

private:

}; // class PainterVerticalAlpha

#endif /* PAINTERVERTICALALPHA_HPP */
