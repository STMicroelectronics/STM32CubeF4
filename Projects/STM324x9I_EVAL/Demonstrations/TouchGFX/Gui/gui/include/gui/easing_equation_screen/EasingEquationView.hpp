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
  


#ifndef EASINGEQUATIONVIEW_HPP
#define EASINGEQUATIONVIEW_HPP

#include <gui/common/DemoView.hpp>
#include <gui/easing_equation_screen/EasingEquationPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/EasingEquations.hpp>
#include <touchgfx/containers/ScrollableContainer.hpp>
#include <touchgfx/containers/ListLayout.hpp>
#include <gui/easing_equation_screen/EasingEquationSelector.hpp>
#include <gui/easing_equation_screen/EasingEquationInOrOutOrInOut.hpp>
#include <gui/easing_equation_screen/EasingEquationGraph.hpp>

using namespace touchgfx;

class EasingEquationView : public DemoView<EasingEquationPresenter>
{
public:
    EasingEquationView();
    virtual ~EasingEquationView();

    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();

private:
    static const int GRAPH_MULT = 6;
    static const int NUMBER_OF_STEPS = 50;
    static const int GRAPH_Y_VALUE_OFFSET = -30;
    static const int PAUSE_DURATION = 20;

    int tickCounter;
    int moveAnimationCounter;

    Image background;
    Image dot;
    EasingEquationSelector selector;
    EasingEquationInOrOutOrInOut inOrOut;
    TextArea easingEquationName;

    EasingEquationSelection easingEquationSelection;
    EasingEquationsInOrOut inOrOutType;
    EasingEquation easingEquation;

    EasingEquationGraph graph;
    Image graphDot;
    bool drawGraph;

    Callback<EasingEquationView, const EasingEquationSelection&> onEasingEquationSelected;
    void easingEquationSelected(const EasingEquationSelection& selection);

    Callback<EasingEquationView, EasingEquationsInOrOut> onInOrOutSelected;
    void inOrOutSelected(EasingEquationsInOrOut newInOrOutType);

    void updateEasingEquation();
};

#endif /* EASINGEQUATIONVIEW_HPP */
