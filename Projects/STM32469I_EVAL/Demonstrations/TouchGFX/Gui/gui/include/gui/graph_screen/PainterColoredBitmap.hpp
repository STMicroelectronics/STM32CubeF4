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
  

#ifndef PAINTERCOLOREDBITMAP_HPP
#define PAINTERCOLOREDBITMAP_HPP

#include <stdint.h>
#if !defined(USE_BPP) || USE_BPP==16
#include <touchgfx/widgets/canvas/PainterRGB565Bitmap.hpp>
#elif USE_BPP==24
#include <touchgfx/widgets/canvas/PainterRGB888Bitmap.hpp>
#elif USE_BPP==4
#include <touchgfx/widgets/canvas/PainterGRAY4Bitmap.hpp>
#elif USE_BPP==2
#include <touchgfx/widgets/canvas/PainterGRAY2Bitmap.hpp>
#else
#error Unknown USE_BPP
#endif

using namespace touchgfx;

/**
 * @class PainterColoredBitmap PainterColoredBitmap.hpp gui/graph_screen/PainterColoredBitmap.hpp
 *
 * @brief A Painter that will paint using a bitmap with a given color.
 *
 *        PainterColoredBitmap will take the color for a given point from a bitmap. The pixel
 *        from the bitmap is 'multiplied' with the given color. This means that if the given
 *        color is red (255,0,0), the blue and green parts of the image will be masked out.
 *        Interesting effects can be obtained if the given image contains colors from the
 *        entire spectrum, but usually the bitmap will be a gray scale bitmap and in this case
 *        the painter can be used to get various shades of the selected color.
 *
 * @see AbstractPainter
 */
class PainterColoredBitmap :
#if !defined(USE_BPP) || USE_BPP==16
    public PainterRGB565Bitmap
#elif USE_BPP==24
    public PainterRGB888Bitmap
#elif USE_BPP==4
    public PainterGRAY4Bitmap
#elif USE_BPP==2
    public PainterGRAY2Bitmap
#else
#error Unknown USE_BPP
#endif
{
public:
    /**
     * @fn PainterColoredBitmap::PainterColoredBitmap();
     *
     * @brief Default constructor.
     *
     *        Default constructor.
     */
    PainterColoredBitmap();

    /**
     * @fn PainterColoredBitmap::PainterColoredBitmap(colortype color, const Bitmap& bmp = Bitmap(BITMAP_INVALID), uint8_t alpha = 255);
     *
     * @brief Constructor.
     *
     *        Constructor.
     *
     * @param color The color.
     * @param bmp   The bitmap.
     * @param alpha the alpha.
     */
    PainterColoredBitmap(colortype color, const Bitmap& bmp = Bitmap(BITMAP_INVALID), uint8_t alpha = 255);

    void setColor(colortype color);

    virtual void render(uint8_t* ptr, int x, int xAdjust, int y, unsigned count, const uint8_t* covers);

protected:
#if !defined(USE_BPP) || USE_BPP==16 || USE_BPP==24
    virtual bool renderNext(uint8_t& pixelRed, uint8_t& pixelGreen, uint8_t& pixelBlue, uint8_t& pixelAlpha);

    uint8_t painterRed;   ///< The red part of the color
    uint8_t painterGreen; ///< The green part of the color
    uint8_t painterBlue;  ///< The blue part of the color
#elif USE_BPP==4 || USE_BPP==2
    virtual bool renderNext(uint8_t& pixelGray, uint8_t& pixelAlpha);
    uint8_t painterGray;   ///< The gray color
#endif
};

#endif /* PAINTERCOLOREDBITMAP_HPP */
