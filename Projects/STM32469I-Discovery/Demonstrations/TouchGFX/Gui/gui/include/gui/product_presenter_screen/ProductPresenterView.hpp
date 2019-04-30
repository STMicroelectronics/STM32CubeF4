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
  

#ifndef PRODUCT_PRESENTER_VIEW_HPP
#define PRODUCT_PRESENTER_VIEW_HPP

#include <gui/common/DemoView.hpp>
#include <gui/product_presenter_screen/ProductPresenterPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/containers/ListLayout.hpp>
#include <gui/product_presenter_screen/FeaturePoster.hpp>
#include <gui/common/SwipeContainer.hpp>
#include <gui/common/CollapsibleMenu.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/mixins/MoveAnimator.hpp>

class ProductPresenterView : public DemoView<ProductPresenterPresenter>
{
public:
    ProductPresenterView();
    virtual ~ProductPresenterView();

    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void screenSaverMinorTick();

private:
    static const uint8_t NUMBER_OF_POSTERS = 3;
    
    LANGUAGES languages[NUMBER_OF_LANGUAGES];
    BitmapId inactiveLanguageButtons[NUMBER_OF_LANGUAGES];

    Image background;
    Image paperOverlay;

    FeaturePoster poster[NUMBER_OF_POSTERS];
    Container posters[NUMBER_OF_POSTERS];

    MoveAnimator<SwipeContainer> posterSwipeContainer;
    CollapsibleMenu languageSelector;
    Button activateLanguageSelectorButton;

    int screenSaverCounter;

    Callback<ProductPresenterView, const AbstractButton&> onButtonPressed;
    Callback<ProductPresenterView, const CollapsibleMenu&, const bool> languageSelectorStateChanged;

    void buttonPressedHandler(const AbstractButton& button);
    void languageSelectorStateChangedHandler(const CollapsibleMenu& menu, const bool isExpanded);

    void setLanguage();
};

#endif // PRODUCT_PRESENTER_VIEW_HPP
