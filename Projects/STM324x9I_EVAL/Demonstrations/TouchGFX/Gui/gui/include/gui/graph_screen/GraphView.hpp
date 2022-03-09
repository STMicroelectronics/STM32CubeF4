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
  


#ifndef GRAPHVIEW_HPP
#define GRAPHVIEW_HPP

#include <gui/common/DemoView.hpp>
#include <gui/graph_screen/GraphPresenter.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/widgets/ToggleButton.hpp>
#include <touchgfx/widgets/RadioButton.hpp>
#include <touchgfx/widgets/RadioButtonGroup.hpp>
#include <touchgfx/mixins/MoveAnimator.hpp>
#include <gui/graph_screen/Graph.hpp>
#include <gui/graph_screen/PieChart.hpp>
#include <gui/graph_screen/LegendEntry.hpp>
#include <touchgfx/mixins/MoveAnimator.hpp>
#include <touchgfx/mixins/FadeAnimator.hpp>
#include <touchgfx/containers/Container.hpp>


class GraphView : public DemoView<GraphPresenter>
{
public:
    enum MenuState
    {
        HIDDEN,
        COLLAPSED,
        EXPANDED
    };

    GraphView();
    virtual ~GraphView();

    virtual void setupScreen();
    virtual void tearDownScreen();

    void animateToMoveState(MenuState newState, int duration);
    MenuState getCurrentMoveState()
    {
        return currentMenuState;
    }

    virtual void handleTickEvent();

    virtual void screenSaverMajorTick();

private:
    static const int NUMBER_OF_GRID_LINES = 4;
    static const int NUMBER_OF_PIE_CHART_SECTIONS = 6;

    enum State
    {
        GRAPH,
        PIE_CHART
    };

    State currentState;

    MenuState currentMenuState;

    Image backgroundBottom;

    Container graphArea;
    Box graphBackground;
    Box graphGridLines[NUMBER_OF_GRID_LINES];
    TextAreaWithOneWildcard graphYValues[NUMBER_OF_GRID_LINES];
    Unicode::UnicodeChar graphYValuesbuf[NUMBER_OF_GRID_LINES][5];

    Graph primaryGraph;
    Graph secondaryGraph;

    Button newButton;

    Button graphOrPieButton;

    ToggleButton primaryGraphLineSetting;
    ToggleButton primaryGraphDotSetting;
    ToggleButton primaryGraphAreaSetting;

    ToggleButton secondaryGraphLineSetting;
    ToggleButton secondaryGraphDotSetting;
    ToggleButton secondaryGraphAreaSetting;

    Button donutOrPieButton;

    int pieChartX;
    int pieChartY;
    int donutX;
    int donutY;

    int pieChartAnimateOutDuration;
    int pieChartRedrawAnimateDuration;
    int pieChartRedraw2AnimateDuration;
    int pieChartDonutChangeAnimateDuration;
    int pieChartAnimateEntryDuration;
    int animateToDonutDelay;

    MoveAnimator<PieChart> pieChart;
#if !defined(USE_BPP) || USE_BPP==16
    PainterRGB565 pieChartPainter[6];
#elif USE_BPP==24
    PainterRGB888 pieChartPainter[6];
#elif USE_BPP==4
    PainterGRAY4 pieChartPainter[6];
#elif USE_BPP==2
    PainterGRAY2 pieChartPainter[6];
#else
#error Unknown USE_BPP
#endif
    MoveAnimator<LegendEntry> legendEntries[NUMBER_OF_PIE_CHART_SECTIONS];

    MoveAnimator<Container> topMenuContainer;
    Box topMenuBackground;
    Image topMenuBottomBar;
    Button topMenuButton;

    Callback<GraphView, const AbstractButton&> onButtonPressed;
    void buttonPressedHandler(const AbstractButton& button);

    Callback<GraphView, const MoveAnimator<LegendEntry>&> legendMoveEnded;
    void legendMoveEndedHandler(const MoveAnimator<LegendEntry>& legend);

    Callback<GraphView, const PieChart&, int> legendSelected;
    void legendSelectedHandler(const PieChart& thePieChart, int item);

    Callback<GraphView, const PieChart&> pieChartAnimationEnded;
    void pieChartAnimationEndedHandler(const PieChart& thePieChart);

    Callback<GraphView, const LegendEntry&> legendPressed;
    void legendPressedHandler(const LegendEntry& legend);

    Callback<GraphView, const MoveAnimator<Container>&> topMenuAnimationEnded;
    void topMenuAnimationEndedHandler(const MoveAnimator<Container>& menu);

    int tickCount;
    int graphX;
    int graphType;
    int leftX;
    int pointCounter;
    bool pieChartIsFull; ///< false means semi-donut
    int automaticTopMenuCollapseTimeout;
    static const int TOP_MENU_COLLAPSE_TIMEOUT = 300;

    enum PieChartAnimationType
    {
        REDRAW_PIECHART, // Fold+new slices
        REDRAW_PIECHART2, // Unfold
        SWITCH_TO_PIECHART,
        SWITCH_TO_GRAPH,
        SWITCH_TO_FULL_CIRCLE,
        SWITCH_TO_SEMI_DONUT
    };
    int pieChartAnimationType;

    void setCurrentState(State newState);
    void clearGraphs();
    void resetPieChart();
    void updateButtonSettings();
    void updateGraphSettings();
    void setAllButtonsTouchable(bool touchable);
};

#endif // GRAPHVIEW_HPP
