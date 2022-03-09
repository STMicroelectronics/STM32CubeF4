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
  


#ifndef PRODUCT_PRESENTER_VIEW_HPP
#define PRODUCT_PRESENTER_VIEW_HPP

#include <gui/common/DemoView.hpp>
#include <gui/product_presenter_screen/ProductPresenterPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <gui/common/CollapsibleMenu.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/mixins/MoveAnimator.hpp>
#include <touchgfx/mixins/FadeAnimator.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/containers/ScrollableContainer.hpp>


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
    static const uint8_t BODY_TEXT_MARGIN = 18;

    LANGUAGES languages[NUMBER_OF_LANGUAGES];
    BitmapId inactiveLanguageButtons[NUMBER_OF_LANGUAGES];

    Container posterContainer;
    MoveAnimator<Image> poster;
    MoveAnimator<Image> posterSwap;
    int selectedPoster;
    Box shadowBox;

    Button nextButton;

    Box textBackground;
    MoveAnimator<TextArea> headlineText;
    MoveAnimator<TextArea> headlineTextSwap;
    ScrollableContainer textAreaContainer;
    FadeAnimator<TextArea> bodyText;

    int fadeDuration;
    int nextTextIndex;

    Image navigationBar;

    CollapsibleMenu languageSelector;
    Button activateLanguageSelectorButton;

    Callback<ProductPresenterView, const AbstractButton&> onButtonPressed;
    Callback<ProductPresenterView, const CollapsibleMenu&, const bool> languageSelectorStateChanged;

    Callback<ProductPresenterView, const FadeAnimator<TextArea>&> onFadeAnimationEnded;
    void fadeAnimationEndedHandler(const FadeAnimator<TextArea>& textArea);

    void animateToPoster(int posterIndex, int duration);

    void buttonPressedHandler(const AbstractButton& button);
    void languageSelectorStateChangedHandler(const CollapsibleMenu& menu, const bool isExpanded);

    BitmapId getPosterBitmap(int index);
    TEXTS getHeadline(int index);
    void setLanguage();
    TEXTS getBodyText(int index);
};

#endif // PRODUCT_PRESENTER_VIEW_HPP
