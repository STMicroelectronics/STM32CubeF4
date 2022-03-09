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
  


#include <gui/easing_equation_screen/EasingEquationView.hpp>
#include "BitmapDatabase.hpp"
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Color.hpp>

EasingEquationView::EasingEquationView() :
    DemoView<EasingEquationPresenter>(),
    tickCounter(0),
    moveAnimationCounter(0),
    onEasingEquationSelected(this, &EasingEquationView::easingEquationSelected),
    onInOrOutSelected(this, &EasingEquationView::inOrOutSelected)
{
    background.setBitmap(Bitmap(BITMAP_EASING_BACKGROUND_ID));
    background.setXY(0, 0);
    add(background);

    selector.setXY(0, 0);
    selector.setEasingEquationSelectedCallback(onEasingEquationSelected);
    add(selector);

    inOrOut.setXY(selector.getRect().right(), HAL::DISPLAY_HEIGHT - inOrOut.getHeight());
    inOrOut.setInOrOutSelectedCallback(onInOrOutSelected);
    add(inOrOut);

    easingEquation = EasingEquations::cubicEaseInOut;
    dot.setBitmap(Bitmap(BITMAP_EASING_DOT_ID));
    dot.setXY(150, 197);
    add(dot);

    mcuLoadTxt.setTypedText(TypedText(T_DEMO_VIEW_MCU_LOAD_TEXT_SMALL));
    mcuLoadTxt.resizeToCurrentText();
    mcuLoadValueTxt.setTypedText(TypedText(T_DEMO_VIEW_MCU_LOAD_VALUE_SMALL));
    mcuLoadTxt.setXY(246, 20 - mcuLoadTxt.getTypedText().getFont()->getFontHeight());
    mcuLoadTxt.setColor(Color::getColorFrom24BitRGB(0x41, 0x5D, 0x62));
    mcuLoadValueTxt.setPosition(mcuLoadTxt.getRect().right(), mcuLoadTxt.getY(), 30, 14);
    mcuLoadValueTxt.setColor(mcuLoadTxt.getColor());
    add(mcuLoadTxt);
    add(mcuLoadValueTxt);

    gotoMenuButton.setBitmaps(Bitmap(BITMAP_EASING_MENU_BUTTON_ID), Bitmap(BITMAP_EASING_MENU_BUTTON_PRESSED_ID));
    gotoMenuButton.setXY(HAL::DISPLAY_WIDTH - gotoMenuButton.getWidth(), 0);
    add(gotoMenuButton);

    easingEquationName.setPosition(selector.getRect().right() + 80, 28, HAL::DISPLAY_WIDTH - (selector.getRect().right() + (80 * 2)), 24);
    easingEquationName.setColor(Color::getColorFrom24BitRGB(0x41, 0x5D, 0x62));
    easingEquationName.setTypedText(TypedText(T_EASING_EQUATION_SINUSOIDAL));
    add(easingEquationName);

    graph.setXY(185, 164 - 100 + GRAPH_Y_VALUE_OFFSET);
    graph.setup((NUMBER_OF_STEPS + 1) * 4, 100 - (2 * GRAPH_Y_VALUE_OFFSET), Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF), 2);
    graph.setRange(0, NUMBER_OF_STEPS + 1, (100 - GRAPH_Y_VALUE_OFFSET) * GRAPH_MULT, GRAPH_Y_VALUE_OFFSET * GRAPH_MULT);
    add(graph);

    graphDot.setBitmap(Bitmap(BITMAP_GRAPHDOT_ID));
    graphDot.setVisible(false);
    add(graphDot);

    selector.select(1);
    inOrOut.selectFirst();
}

EasingEquationView::~EasingEquationView()
{
}

void EasingEquationView::setupScreen()
{
}

void EasingEquationView::tearDownScreen()
{
}

void EasingEquationView::easingEquationSelected(const EasingEquationSelection& selection)
{
    this->easingEquationSelection = selection;
    easingEquationName.setTypedText(TypedText(selection.EasingEquationName));
    easingEquationName.invalidate();

    inOrOutSelected(this->inOrOutType);
}

void EasingEquationView::inOrOutSelected(EasingEquationsInOrOut newInOrOutType)
{
    inOrOutType = newInOrOutType;
    if (newInOrOutType == EASING_EQUATION_IN)
    {
        easingEquation = easingEquationSelection.in;
    }
    else if (newInOrOutType == EASING_EQUATION_OUT)
    {
        easingEquation = easingEquationSelection.out;
    }
    else
    {
        easingEquation = easingEquationSelection.inOut;
    }
    updateEasingEquation();
}

void EasingEquationView::updateEasingEquation()
{
    dot.moveTo(150, 197);
    moveAnimationCounter = 0;
    graph.clear();
    graph.invalidate();
    drawGraph = true;
}

void EasingEquationView::handleTickEvent()
{
    tickCounter++;

    if (tickCounter % 2 == 0)
    {
        int moveAnimationDuration = NUMBER_OF_STEPS;

        if (moveAnimationCounter <= moveAnimationDuration)
        {
            graphDot.setVisible(true);

            int deltaDot = easingEquation(moveAnimationCounter, 0, 258, moveAnimationDuration);
            dot.moveTo(150 + deltaDot, dot.getY());

            int deltaGraphDot = easingEquation(moveAnimationCounter, 0, 100 * GRAPH_MULT, moveAnimationDuration);
            graphDot.moveTo(graph.getX() + 4 * moveAnimationCounter - (graphDot.getWidth() / 2) - 1, graph.getRect().bottom() - (deltaGraphDot / GRAPH_MULT) + GRAPH_Y_VALUE_OFFSET - (graphDot.getHeight() / 2) - 1);

            if (drawGraph)
            {
                graph.addValue(moveAnimationCounter, deltaGraphDot);
            }

            moveAnimationCounter++;
        }
        else if (moveAnimationCounter <= moveAnimationDuration + PAUSE_DURATION)
        {
            moveAnimationCounter++;
        }
        else
        {
            // End of animation
            moveAnimationCounter = 0;
            drawGraph = false;
        }
    }
}

