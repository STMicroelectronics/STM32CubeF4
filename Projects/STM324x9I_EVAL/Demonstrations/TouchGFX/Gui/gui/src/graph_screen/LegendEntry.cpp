/**
  ******************************************************************************
  * This file is part of the TouchGFX 4.10.0 distribution.
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
