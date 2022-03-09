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
  

#include <gui/graph_screen/PainterColoredBitmap.hpp>
#include <touchgfx/Color.hpp>

using namespace touchgfx;

PainterColoredBitmap::PainterColoredBitmap()
{
    setColor(0);
    setBitmap(Bitmap(BITMAP_INVALID));
    setAlpha(255);
}

PainterColoredBitmap::PainterColoredBitmap(colortype color, const Bitmap& bmp, uint8_t alpha)
{
    setColor(color);
    setBitmap(bmp);
    setAlpha(alpha);
}

void PainterColoredBitmap::setColor(colortype color)
{
#if !defined(USE_BPP) || USE_BPP==16 || USE_BPP==24
    painterRed = Color::getRedColor(color);
    painterGreen = Color::getGreenColor(color);
    painterBlue = Color::getBlueColor(color);
#elif USE_BPP==4 || USE_BPP==2
    painterGray = (uint8_t)color;
#else
#error Unknown USE_BPP
#endif
}

void PainterColoredBitmap::render(uint8_t* ptr, int x, int xAdjust, int y, unsigned count, const uint8_t* covers)
{
#if !defined(USE_BPP) || USE_BPP==16
    // Prevent the optimized version of PainterRGB565Bitmap::render to be used, we need the slower, per pixel version
    AbstractPainterRGB565::render(ptr, x, xAdjust, y, count, covers);
#elif USE_BPP==24
    // Prevent the optimized version of PainterRGB888Bitmap::render to be used, we need the slower, per pixel version
    AbstractPainterRGB888::render(ptr, x, xAdjust, y, count, covers);
#elif USE_BPP==4
    // Prevent the optimized version of PainterGRAY4Bitmap::render to be used, we need the slower, per pixel version
    AbstractPainterGRAY4::render(ptr, x, xAdjust, y, count, covers);
#elif USE_BPP==2
    // Prevent the optimized version of PainterGRAY2Bitmap::render to be used, we need the slower, per pixel version
    AbstractPainterGRAY2::render(ptr, x, xAdjust, y, count, covers);
#else
#error Unknown USE_BPP
#endif
}

#if !defined(USE_BPP) || USE_BPP==16 || USE_BPP==24
bool PainterColoredBitmap::renderNext(uint8_t& pixelRed, uint8_t& pixelGreen, uint8_t& pixelBlue, uint8_t& pixelAlpha)
{
    // Get RGBA from the bitmap
#if !defined(USE_BPP) || USE_BPP==16
    if (PainterRGB565Bitmap::renderNext(pixelRed, pixelGreen, pixelBlue, pixelAlpha))
#elif USE_BPP==24
    if (PainterRGB888Bitmap::renderNext(pixelRed, pixelGreen, pixelBlue, pixelAlpha))
#else
#error Unknown USE_BPP
#endif
    {
        // Multiply with our color
        pixelRed   = (pixelRed   * painterRed) / 255;
        pixelGreen = (pixelGreen * painterGreen) / 255;
        pixelBlue  = (pixelBlue  * painterBlue) / 255;
        return true;
    }
    return false;
}
#elif USE_BPP==4
bool PainterColoredBitmap::renderNext(uint8_t& pixelGray, uint8_t& pixelAlpha)
{
    if (PainterGRAY4Bitmap::renderNext(pixelGray, pixelAlpha))
    {
        // Multiply with our color
        pixelGray = (pixelGray * painterGray) / 16;
        return true;
    }
    return false;
}
#elif USE_BPP==2
bool PainterColoredBitmap::renderNext(uint8_t& pixelGray, uint8_t& pixelAlpha)
{
    if (PainterGRAY2Bitmap::renderNext(pixelGray, pixelAlpha))
    {
        // Multiply with our color
        pixelGray = (pixelGray * painterGray) / 4;
        return true;
    }
    return false;
}

#else
#error Unknown USE_BPP
#endif
