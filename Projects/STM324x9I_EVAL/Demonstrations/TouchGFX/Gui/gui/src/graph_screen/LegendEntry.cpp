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
  


#include <gui/graph_screen/LegendEntry.hpp>
#include <touchgfx/Color.hpp>
#include <BitmapDatabase.hpp>

LegendEntry::LegendEntry() :
    onButtonPressed(this, &LegendEntry::buttonPressedHandler)
{
    square.setBitmap(Bitmap(BITMAP_PIE_CHART_LEGEND_BLUE_ID));
    square.setXY(0, 0);
    add(square);

    text.setColor(Color::getColorFrom24BitRGB(0xBE, 0xC0, 0xC4));
    text.setPosition(square.getRect().right() + 6, 6, 46, 16);
    add(text);

    valueText.setWildcard(valueTextBuffer);
    valueText.setTypedText(TypedText(T_GRAPH_LEGEND_VALUE_TEXT));
    valueText.setColor(text.getColor());
    valueText.setPosition(text.getRect().right() + 6, text.getY(), 40, 16);
    add(valueText);

    setWidth(valueText.getRect().right());
    setHeight(square.getRect().bottom() + 20); // Add a little extra height to make it easier to press

    // SelectButton is an invisible button
    selectButton.setPosition(0, 0, getWidth(), getHeight());
    selectButton.setAction(onButtonPressed);
    add(selectButton);
}

LegendEntry::~LegendEntry()
{

}


void LegendEntry::setBitmaps(BitmapId squareNotSelectedBmp, BitmapId squareSelectedBmp)
{
    squareNotSelected = squareNotSelectedBmp;
    squareSelected = squareSelectedBmp;

    setSelected(false);
}

void LegendEntry::setText(TEXTS textId)
{
    text.setTypedText(TypedText(textId));
    text.invalidate();
}

void LegendEntry::setValueText(int value)
{
    Unicode::snprintf(valueTextBuffer, 6, "%i", value);
    valueText.invalidate();
}

void LegendEntry::setSelected(bool selected)
{
    if (selected)
    {
        square.setBitmap(Bitmap(squareSelected));
        text.setColor(Color::getColorFrom24BitRGB(0x0, 0x0, 0x0));
    }
    else
    {
        square.setBitmap(Bitmap(squareNotSelected));
        text.setColor(Color::getColorFrom24BitRGB(0xBE, 0xC0, 0xC4));
    }
    valueText.setColor(text.getColor());
    valueText.setVisible(selected);

    square.invalidate();
    text.invalidate();
    valueText.invalidate();
}

void LegendEntry::buttonPressedHandler(const AbstractButton& button)
{
    if (selectionAction && selectionAction->isValid())
    {
        selectionAction->execute(*this);
    }
}
