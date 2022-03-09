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
  


#include <gui/animated_graphics_screen/BumpMapImage.hpp>
#include <touchgfx/EasingEquations.hpp>


BumpMapImage::BumpMapImage(BitmapId image_, const unsigned int* bump_map_, const unsigned char* light_source_) :
    currentState(ANIMATE_AUTO),
    image(image_),
    //bump(BITMAP_LIGHT_EFFECT_IMAGE_ID),
    bump(0),
    bump_map(bump_map_),
    light_source(light_source_),
    currentTime(0),
    catchTime(25),
    lightPositionX(1000), // Set position outside the visible area
    lightPositionY(1000),
    tempPositionX(1000),
    tempPositionY(1000)
{
    Application::getInstance()->registerTimerWidget(this);

    // Use these methods to convert and print a bitmap or light table
    //printBumpMap();
    //printLightTable();
}

BumpMapImage::~BumpMapImage()
{
    Application::getInstance()->unregisterTimerWidget(this);
}

void BumpMapImage::handleTickEvent()
{
    currentTime += 2;
    tempPositionX = (int)(128.0f * cos((double)currentTime / 64)) - 20  + 160;
    tempPositionY = (int)(128.0f * sin((double) - currentTime / 45)) + 20 + 110;

    invalidateAccordingToCurrentLightSetting();

    if (currentState == ANIMATE_AUTO)
    {
        lightPositionX = tempPositionX;
        lightPositionY = tempPositionY;
    }
    else if (currentState == ANIMATE_CATCH)
    {
        if (catchTime > 0)
        {
            lightPositionX = lightPositionX + ((tempPositionX - lightPositionX) / catchTime);
            lightPositionY = lightPositionY + ((tempPositionY - lightPositionY) / catchTime);
            catchTime--;
        }
        else
        {
            catchTime = 25;
            currentState = ANIMATE_AUTO;
        }
    }

    invalidateAccordingToCurrentLightSetting();
}

void BumpMapImage::handleClickEvent(const ClickEvent& evt)
{
    if (evt.getType() == ClickEvent::PRESSED)
    {
        setDragFinish(false);
    }
    else
    {
        setDragFinish(true);
    }
}

void BumpMapImage::draw(const touchgfx::Rect& r) const
{
    drawOptimized(r);
    //drawNormal(r);
}

void BumpMapImage::drawOptimized(const touchgfx::Rect& r) const
{
    // invalidatedArea is relative to enclosing container
    Rect absoluteRect = r;
    translateRectToAbsolute(absoluteRect);

    // In case of an odd r.x draw() will also redraw r.x-1
    // This is due to the fact that an uint32_t pointer is
    // used for the imageData/FB pointers. This is chosen
    // because of optimized performance.
    // This issue does not affect this demo screen.
    /*
    assert(absoluteRect.x % 2 == 0);
    assert(absoluteRect.width % 2 == 0);
    assert(HAL::DISPLAY_WIDTH % 2 == 0);
    */

    // Bump map slope information
    int16_t nx;
    int16_t ny;

    int16_t lightCalculationPositionX;
    int16_t lightCalculationPositionY;

    uint32_t* fb = (uint32_t*)HAL::getInstance()->lockFrameBuffer();
    const uint32_t* imageData = (const uint32_t*)(image.getData());

    int offset;

    uint16_t invalidatedY = absoluteRect.y;
    uint16_t invalidatedX = absoluteRect.x;

    uint32_t newValue = 0;
    uint16_t imageDataValue = 0;
    uint8_t lightDiff = 0;
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;
    uint32_t colorResult = 0;
    int relativeX;
    int relativeY;
    int bumpMapInfo;
    uint32_t pixelColorValues;



    // Go through each invalidated pixel. For optimization reasons the pointers used are 32 bits,
    // so each read yield values for two pixels.
    for (int absoluteY = invalidatedY; absoluteY < absoluteRect.bottom(); absoluteY++)
    {
        // Set offset to point to the next line in the image bitmap and bump_map
        offset = (absoluteY * image.getWidth() / 2) + (invalidatedX - getX()) / 2;

        for (int absoluteX = invalidatedX; absoluteX < absoluteRect.right(); absoluteX += 2, offset += 1)
        {
            newValue = 0;
            relativeX = absoluteX - getX();
            relativeY = absoluteY - getY();
            bumpMapInfo = bump_map[offset];
            pixelColorValues = *(imageData + offset);

            // Read the bump map value for the current pixel
            // The bump map info is packed in a 32 bit format and need a bit of shifting
            nx = (int8_t)(bumpMapInfo >> 24);
            ny = (int8_t)((bumpMapInfo >> 16) & 0xFF);

            // Calculate the pixels position in the light
            lightCalculationPositionX = (relativeX - lightPositionX) + (lightSourceWidth / 2) + nx;
            lightCalculationPositionY = (relativeY - lightPositionY) + (lightSourceHeight / 2) + ny;

            //lightDiff = light(lightCalculationPositionX, lightCalculationPositionY);

            // Get the light value that should be applied to the pixel
            if (lightCalculationPositionX < 0 || lightCalculationPositionY < 0 || lightCalculationPositionX >= lightSourceWidth || lightCalculationPositionY >= lightSourceHeight)
            {
                lightDiff = 0;
            }
            else
            {
                // Value can be found in the precalculated light_source
                lightDiff = light_source[lightCalculationPositionY * lightSourceWidth + lightCalculationPositionX];
            }

            // Get the actual color value from the 32 bit value (which contains two pixels) from the image
            imageDataValue = pixelColorValues & 0xFFFF;

            red = (imageDataValue & 0xF800) >> 11;
            green = (imageDataValue & 0x07E0) >> 5;
            blue = imageDataValue & 0x001F;

            // Apply the light effect
            if (lightDiff < lightMax / 2)
            {
                // Darken pixels that have no or a little extra light
                red = red / 4 + (red - red / 4) * lightDiff / (lightMax / 2);
                green = green / 4 + (green - green / 4) * lightDiff / (lightMax / 2);
                blue = blue / 4 + (blue - blue / 4) * lightDiff / (lightMax / 2);
            }
            else
            {
                red = red + (31 - red) * (lightDiff - (lightMax / 2)) / (lightMax / 2);
                green = green + (63 - green) * (lightDiff - (lightMax / 2)) / (lightMax / 2);
                blue = blue + (31 - blue) * (lightDiff - (lightMax / 2)) / (lightMax / 2);
            }

            // Combine the colors and add them to the new color value for the pixel (packed together in a 32 bit value)
            colorResult = (red << 11) | (green << 5) | blue;
            newValue |= colorResult;


            // Do the calculation for the second pixel in the 32 bit read

            // Read the bump map value for the current pixel
            nx = (int8_t)((bumpMapInfo >> 8) & 0xFF);
            ny = (int8_t)(bumpMapInfo & 0xFF);

            lightCalculationPositionX = (relativeX + 1 - lightPositionX) + (lightSourceWidth / 2) + nx;
            lightCalculationPositionY = (relativeY + 1 - lightPositionY) + (lightSourceHeight / 2) + ny;

            //lightDiff = light(lightCalculationPositionX, lightCalculationPositionY);

            if (lightCalculationPositionX < 0 || lightCalculationPositionY < 0 || lightCalculationPositionX >= lightSourceWidth || lightCalculationPositionY >= lightSourceHeight)
            {
                lightDiff = 0;
            }
            else
            {
                lightDiff = light_source[lightCalculationPositionY * lightSourceWidth + lightCalculationPositionX];
            }
            imageDataValue = pixelColorValues >> 16;


            red = (imageDataValue & 0xF800) >> 11;
            green = (imageDataValue & 0x07E0) >> 5;
            blue = imageDataValue & 0x001F;


            if (lightDiff < lightMax / 2)
            {
                // Darken pixels that have no or a little extra light
                red = red / 4 + (red - red / 4) * lightDiff / (lightMax / 2);
                green = green / 4 + (green - green / 4) * lightDiff / (lightMax / 2);
                blue = blue / 4 + (blue - blue / 4) * lightDiff / (lightMax / 2);
            }
            else
            {
                red = red + (31 - red) * (lightDiff - (lightMax / 2)) / (lightMax / 2);
                green = green + (63 - green) * (lightDiff - (lightMax / 2)) / (lightMax / 2);
                blue = blue + (31 - blue) * (lightDiff - (lightMax / 2)) / (lightMax / 2);
            }

            colorResult = (red << 11) | (green << 5) | blue;
            newValue |= (colorResult << 16);

            // Update framebuffer
            *(fb + (absoluteX / 2) + (HAL::DISPLAY_WIDTH / 2 * absoluteY)) = newValue;
        }
    }

    HAL::getInstance()->unlockFrameBuffer();
}

void BumpMapImage::drawNormal(const touchgfx::Rect& r) const
{
    // invalidatedArea is relative to enclosing container
    Rect absoluteRect = r;
    translateRectToAbsolute(absoluteRect);

    int8_t nx;
    int8_t ny;

    int16_t lightCalculationPositionX;
    int16_t lightCalculationPositionY;

    uint16_t* fb = HAL::getInstance()->lockFrameBuffer();
    const uint16_t* imageData = (const uint16_t*) image.getData();
    const uint16_t* bumpData = (const uint16_t*) bump.getData();


    // we skip the first line since there are no pixels above to calculate the slope with

    int offset = absoluteRect.x;
    if (absoluteRect.y == 0)
    {
        while (offset < absoluteRect.width)
        {
            fb[offset++] = 0x0000;
        }
        offset += (HAL::DISPLAY_WIDTH - absoluteRect.width);
    }

    uint16_t invalidatedY = absoluteRect.y;
    uint16_t invalidatedX = absoluteRect.x;
    if (invalidatedX == 0)
    {
        invalidatedX = 1;
    }
    if (invalidatedY == 0)
    {
        invalidatedY = 1;
    }

    int relativeX;
    int relativeY;
    uint8_t lightDiff;

    for (int absoluteY = invalidatedY; absoluteY < absoluteRect.bottom(); absoluteY++)
    {
        offset = (absoluteY * image.getWidth()) + invalidatedX - getX();

        // likewise, skip first pixel since there are no pixels on the left
        if (absoluteRect.x == 0)
        {
            fb[offset++] = 0;
        }

        for (int absoluteX = invalidatedX; absoluteX < absoluteRect.right(); absoluteX++, offset++)
        {
            relativeX = absoluteX - getX();
            relativeY = absoluteY - getY();


            /*
            if (offset%2==0)
            {
                nx = (int8_t)(bump_map[offset/2] >> 24);
                ny = (int8_t)((bump_map[offset/2] >> 16) & 0xFF);
            }
            else
            {
                nx = (int8_t)((bump_map[offset/2] >> 8) & 0xFF);
                ny = (int8_t)(bump_map[offset/2] & 0xFF);
            }*/

            nx = height(bumpData[offset - 1]) - height(bumpData[offset]);
            ny = height(bumpData[offset - image.getWidth()]) - height(bumpData[offset]);

            lightCalculationPositionX = (relativeX - lightPositionX) + (lightSourceWidth / 2) + nx;
            lightCalculationPositionY = (relativeY - lightPositionY) + (lightSourceHeight / 2) + ny;

            //lightDiff = light(lightCalculationPositionX, lightCalculationPositionY);
            if (lightCalculationPositionX < 0 || lightCalculationPositionY < 0 || lightCalculationPositionX >= lightSourceWidth || lightCalculationPositionY >= lightSourceHeight)
            {
                lightDiff = 0;
            }
            else
            {
                lightDiff = light_source[lightCalculationPositionY * lightSourceWidth + lightCalculationPositionX];
            }

            uint8_t red = (*(imageData + offset) & 0xF800) >> 11;
            uint8_t green = (*(imageData + offset) & 0x07E0) >> 5;
            uint8_t blue = *(imageData + offset) & 0x001F;

            if (lightDiff < lightMax / 2)
            {
                red = red / 4 + (red - red / 4) * lightDiff / (lightMax / 2);
                green = green / 4 + (green - green / 4) * lightDiff / (lightMax / 2);
                blue = blue / 4 + (blue - blue / 4) * lightDiff / (lightMax / 2);
            }
            else
            {
                red = red + (31 - red) * (lightDiff - (lightMax / 2)) / (lightMax / 2);
                green = green + (63 - green) * (lightDiff - (lightMax / 2)) / (lightMax / 2);
                blue = blue + (31 - blue) * (lightDiff - (lightMax / 2)) / (lightMax / 2);
            }

            // Update framebuffer
            *(fb + absoluteX + (HAL::DISPLAY_WIDTH * absoluteY)) = (uint16_t)((red << 11) | (green << 5) | blue);
        }
    }

    HAL::getInstance()->unlockFrameBuffer();
}

void BumpMapImage::invalidateAccordingToCurrentLightSetting()
{
    // Include an area around the light cone since these can also be illuminated
    uint8_t margin = 66;

    int16_t x = lightPositionX - (lightSourceWidth / 2) - margin;
    if (x < 0)
    {
        x = 0;
    }

    // Place x on an even address
    if (x % 2 == 1)
    {
        x--;
    }

    int16_t y = lightPositionY - (lightSourceHeight / 2) - margin;
    if (y < 0)
    {
        y = 0;
    }

    uint16_t width = lightSourceWidth + 2 * margin;
    uint16_t height = lightSourceHeight + 2 * margin;

    // Ensure that the width is even
    if (width % 2 == 1)
    {
        width++;
    }

    Rect newRect = Rect(x, y, width, height);

    invalidateRect(newRect);
}

void BumpMapImage::setLightPosition(uint16_t x, uint16_t y)
{
    lightPositionX = x;
    lightPositionY = y;
    invalidateAccordingToCurrentLightSetting();
}

void BumpMapImage::setDragFinish(bool finish)
{
    if (finish)
    {
        currentState = ANIMATE_CATCH;
    }
    else
    {
        currentState = DRAGGING;
    }
}

int BumpMapImage::height(uint16_t value) const
{
    uint8_t red = (value & 0xF800) >> 11;
    uint8_t green = (value & 0x07E0) >> 5;
    uint8_t blue = value & 0x001F;
    return red + green + blue;
}

Rect BumpMapImage::getSolidRect() const
{
    return Rect(0, 0, getWidth(), getHeight());
}

#ifdef SIMULATOR

#include <iostream>
#include <fstream>
void BumpMapImage::printLightTable()
{
    std::ofstream outputFile;
    outputFile.open("lightTableTemp.txt");

    uint32_t result;
    outputFile << "const unsigned short _light_source[] = {\n    ";

    for (int y = 0; y < lightSourceHeight; y++)
    {

        for (int x = 0; x < lightSourceWidth; x++)
        {
            result = light(x, y);
            outputFile << std::hex << "0x" << result;
            if (!((x == lightSourceWidth - 1) && (y == lightSourceHeight - 1)))
            {
                outputFile << ", ";
            }
        }
    }

    outputFile << "\n};";

    outputFile.close();

}

void BumpMapImage::printBumpMap()
{

    std::ofstream outputFile;
    outputFile.open("bumpMapTemp.txt");

    int8_t nx;
    int8_t ny;

    //uint16_t *fb = HAL::getInstance()->lockFrameBuffer();
    //const uint16_t *imageData = image.getData();
    const uint16_t* bumpData = (const uint16_t*) bump.getData();


    // we skip the first line since there are no pixels above to calculate the slope with
    int offset = 0;
    while (offset < image.getWidth() / 2)
    {
        outputFile << std::hex << "0x" << 0 << ", ";
        offset++;
    }
    offset = image.getWidth();

    uint32_t printValue = 0;

    for (int y = 1; y < image.getHeight(); y++)
    {

        // likewise, skip first pixel since there are no pixels on the left
        offset++;

        for (int x = 1; x < image.getWidth(); x++, offset++)
        {
            nx = height(bumpData[offset - 1]) - height(bumpData[offset]);
            ny = height(bumpData[offset - image.getWidth()]) - height(bumpData[offset]);

            if (x % 2 == 0)
            {
                printValue |= (uint32_t)((((uint8_t)(nx) << 8) | (uint8_t)(ny)) << 16);
            }
            else
            {
                printValue |= ((uint8_t)(nx) << 8) | (uint8_t)(ny);
                outputFile << std::hex << "0x" << printValue << ", ";
                printValue = 0;
            }
        }
    }

    HAL::getInstance()->unlockFrameBuffer();
    outputFile.close();
}

uint8_t BumpMapImage::light(int16_t x, int16_t y) const
{
    if (x < 0 || y < 0 || x > lightSourceWidth || y > lightSourceHeight)
    {
        return 0;
    }

    uint16_t distToCenter = (uint16_t)(sqrt((float)((x - lightSourceWidth / 2) * (x - lightSourceWidth / 2) + (y - lightSourceHeight / 2) * (y - lightSourceHeight / 2))));

    //uint16_t distToCenter = x - lightSourceWidth/2;

    int16_t result = lightMax - distToCenter;
    result = (result > 0) ? result : 0;


    return (uint8_t) result;
}

#endif // SIMULATOR
