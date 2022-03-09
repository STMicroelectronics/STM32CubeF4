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
  


#include <gui/graph_screen/PainterVerticalAlpha.hpp>
#include <touchgfx/Color.hpp>

using namespace touchgfx;

PainterVerticalAlpha::PainterVerticalAlpha(colortype color /*= 0*/, int alpha0atY /*= 100*/)
{
    setColor(color, alpha0atY);
}

void PainterVerticalAlpha::setColor(colortype color, int alphaZeroAt)
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
    alpha0atY = alphaZeroAt;
}

bool PainterVerticalAlpha::renderInit()
{
    int gradientAlpha = 255 - (currentY * 255 / alpha0atY);
    if (gradientAlpha < 0)
    {
        gradientAlpha = 0;
    }
    if (gradientAlpha > 255)
    {
        gradientAlpha = 255;
    }
    painterAlpha = gradientAlpha;
    return true;
}

void PainterVerticalAlpha::render(uint8_t* ptr, int x, int xAdjust, int y, unsigned count, const uint8_t* covers)
{
    currentY = y + areaOffsetY;
    renderInit(); // re-use code to set painterAlpha
#if !defined(USE_BPP) || USE_BPP==16 || USE_BPP==24
#if !defined(USE_BPP) || USE_BPP==16
    uint16_t* p = reinterpret_cast<uint16_t*>(ptr) + (x + xAdjust);
    currentX = x + areaOffsetX; // CurrentX is not used in 24bpp
#else
    uint8_t* p = ptr + ((x + xAdjust) * 3);
#endif
    uint8_t totalAlpha = (widgetAlpha * painterAlpha) / 255u;
    do
    {
        uint32_t combinedAlpha = (*covers) * totalAlpha;
        covers++;

#if !defined(USE_BPP) || USE_BPP==16
        uint8_t p_red = (*p & RMASK) >> 8;
        uint8_t p_green = (*p & GMASK) >> 3;
        uint8_t p_blue = (*p & BMASK) << 3;

        uint8_t pixelRed = static_cast<uint8_t>((((painterRed - p_red) * combinedAlpha) >> 16) + p_red);
        uint8_t pixelGreen = static_cast<uint8_t>((((painterGreen - p_green) * combinedAlpha) >> 16) + p_green);
        uint8_t pixelBlue = static_cast<uint8_t>((((painterBlue - p_blue) * combinedAlpha) >> 16) + p_blue);

#define fastrand(g_seed) ((214013*(uint32_t)(g_seed)+2531011)>>16)
        uint8_t rand = fastrand((currentX + *p) * (currentY + *p));
        if (pixelRed < 0xF8)
        {
            pixelRed = pixelRed + ((rand >> 5));    // Add bit 7-5 of rand to red to get rounding
        }
        if (pixelGreen < 0xFC)
        {
            pixelGreen = pixelGreen + ((rand >> 3) & 0x3);    // Add bit 4-3 of rand to green to get rounding
        }
        if (pixelBlue < 0xF8)
        {
            pixelBlue = pixelBlue + ((rand) & 0x7);    // Add bit 2-0 of rand to blue to get rounding
        }
        *p++ = ((pixelRed << 8) & RMASK) | ((pixelGreen << 3) & GMASK) | ((pixelBlue >> 3) & BMASK);
        currentX++;
#else
        uint8_t pixelBlue = static_cast<uint8_t>((((painterBlue - *p) * combinedAlpha) >> 16) + *p);
        *p++ = pixelBlue;

        uint8_t pixelGreen = static_cast<uint8_t>((((painterGreen - *p) * combinedAlpha) >> 16) + *p);
        *p++ = pixelGreen;

        uint8_t pixelRed = static_cast<uint8_t>((((painterRed - *p) * combinedAlpha) >> 16) + *p);
        *p++ = pixelRed;
#endif
    }
    while (--count != 0);
#elif USE_BPP==4
    x += xAdjust;
    uint8_t totalAlpha = (widgetAlpha * painterAlpha) / 255u;
    do
    {
        uint16_t combinedAlpha = (*covers) * totalAlpha;
        covers++;

        if (combinedAlpha != 0)
        {
            uint8_t p_gray = LCD4getPixel(ptr, x);
            LCD4setPixel(ptr, x, static_cast<uint8_t>((((painterGray - p_gray) * combinedAlpha) >> 16) + p_gray));
        }
        x++;
    }
    while (--count != 0);
#elif USE_BPP==2
    x += xAdjust;
    uint8_t totalAlpha = (widgetAlpha * painterAlpha) / 255u;
    do
    {
        uint8_t combinedAlpha = (*covers) * totalAlpha;
        covers++;

        if (combinedAlpha != 0)
        {
            uint8_t p_gray = LCD2getPixel(ptr, x);
            LCD2setPixel(ptr, x, static_cast<uint8_t>((((painterGray - p_gray) * combinedAlpha) >> 16) + p_gray));
        }
        x++;
    }
    while (--count != 0);
#else
#error Unknown USE_BPP
#endif
}

#if !defined(USE_BPP) || USE_BPP==16 || USE_BPP==24
bool PainterVerticalAlpha::renderNext(uint8_t& pixelRed, uint8_t& pixelGreen, uint8_t& pixelBlue, uint8_t& pixelAlpha)
{
    return false;
}
#elif USE_BPP==4 || USE_BPP==2
bool PainterVerticalAlpha::renderNext(uint8_t& pixelGray, uint8_t& pixelAlpha)
{
    return false;
}
#else
#error Unknown USE_BPP
#endif
