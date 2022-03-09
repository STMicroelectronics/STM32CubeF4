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
  


#include <gui/product_presenter_screen/ProductPresenterView.hpp>
#include "BitmapDatabase.hpp"
#include <touchgfx/Color.hpp>
#include <touchgfx/EasingEquations.hpp>
#include <texts/TextKeysAndLanguages.hpp>

ProductPresenterView::ProductPresenterView() :
    DemoView<ProductPresenterPresenter>(),
    selectedPoster(0),
    onButtonPressed(this, &ProductPresenterView::buttonPressedHandler),
    languageSelectorStateChanged(this, &ProductPresenterView::languageSelectorStateChangedHandler),
    onFadeAnimationEnded(this, &ProductPresenterView::fadeAnimationEndedHandler)
{
}

ProductPresenterView::~ProductPresenterView()
{
    Texts::setLanguage(GBR);
}

void ProductPresenterView::setupScreen()
{
    posterContainer.setPosition(0, 0, Bitmap(BITMAP_POSTER_IMAGE_00_ID).getWidth(), Bitmap(BITMAP_POSTER_IMAGE_00_ID).getHeight());
    poster.setXY(0, 0);
    posterContainer.add(poster);
    posterContainer.add(posterSwap);

    shadowBox.setColor(Color::getColorFrom24BitRGB(0x0, 0x0, 0x0));
    shadowBox.setPosition(posterContainer.getRect().right(), posterContainer.getY(), 1, posterContainer.getHeight());
    add(shadowBox);

    textBackground.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    textBackground.setPosition(shadowBox.getRect().right(), 0, HAL::DISPLAY_WIDTH - shadowBox.getRect().right(), posterContainer.getHeight());
    add(textBackground);

    nextButton.setBitmaps(Bitmap(BITMAP_PRODUCT_PRESENTER_NEXT_ID), Bitmap(BITMAP_PRODUCT_PRESENTER_NEXT_PRESSED_ID));
    nextButton.setXY(HAL::DISPLAY_WIDTH - nextButton.getWidth(), 0);
    nextButton.setAction(onButtonPressed);

    headlineText.setPosition(textBackground.getX() + BODY_TEXT_MARGIN, textBackground.getY() + 20, nextButton.getX() - textBackground.getX(), 30);
    headlineText.setColor(Color::getColorFrom24BitRGB(0x76, 0x76, 0x76));
    headlineTextSwap.setWidth(headlineText.getWidth());
    headlineTextSwap.setHeight(headlineText.getHeight());
    headlineTextSwap.setColor(headlineText.getColor());
    add(headlineText);
    add(headlineTextSwap);
    add(nextButton);

    textAreaContainer.setPosition(textBackground.getX(), nextButton.getRect().bottom(), textBackground.getWidth(), textBackground.getHeight() - nextButton.getRect().bottom());
    bodyText.setPosition(BODY_TEXT_MARGIN, 0, textAreaContainer.getWidth() - 2 * BODY_TEXT_MARGIN, textAreaContainer.getWidth() - BODY_TEXT_MARGIN);
    bodyText.setColor(headlineText.getColor());
    bodyText.setFadeAnimationEndedAction(onFadeAnimationEnded);
    textAreaContainer.add(bodyText);
    add(textAreaContainer);

    add(posterContainer);

    navigationBar.setBitmap(Bitmap(BITMAP_PRODUCT_PRESENTER_NAV_BAR_ID));
    navigationBar.setXY(posterContainer.getX(), posterContainer.getRect().bottom());
    add(navigationBar);

    gotoMenuButton.setBitmaps(Bitmap(BITMAP_PRODUCT_PRESENTER_MENU_BUTTON_ID), Bitmap(BITMAP_PRODUCT_PRESENTER_MENU_BUTTON_PRESSED_ID));
    gotoMenuButton.setXY(0, navigationBar.getY());
    add(gotoMenuButton);

    mcuLoadTxt.setXY(8, 208 - mcuLoadTxt.getTypedText().getFont()->getFontHeight());
    mcuLoadValueTxt.setPosition(mcuLoadTxt.getRect().right(), mcuLoadTxt.getY(), 34, 14);
    add(mcuLoadTxt);
    add(mcuLoadValueTxt);

    activateLanguageSelectorButton.setXY(419, 231);
    activateLanguageSelectorButton.setAction(onButtonPressed);
    add(activateLanguageSelectorButton);

    languages[0] = GBR;
    inactiveLanguageButtons[0] = BITMAP_FLAG_GBR_INACTIVE_ID;
    languages[1] = DNK;
    inactiveLanguageButtons[1] = BITMAP_FLAG_DNK_INACTIVE_ID;
    languages[2] = JPN;
    inactiveLanguageButtons[2] = BITMAP_FLAG_JPN_INACTIVE_ID;
    languages[3] = UKR;
    inactiveLanguageButtons[3] = BITMAP_FLAG_UKR_INACTIVE_ID;
    languages[4] = NLD;
    inactiveLanguageButtons[4] = BITMAP_FLAG_NLD_INACTIVE_ID;

    languageSelector.addMenuElement(Bitmap(BITMAP_FLAG_GBR_ACTIVE_ID), Bitmap(BITMAP_FLAG_GBR_ACTIVE_ID));
    languageSelector.addMenuElement(Bitmap(BITMAP_FLAG_DNK_ACTIVE_ID), Bitmap(BITMAP_FLAG_DNK_ACTIVE_ID));
    languageSelector.addMenuElement(Bitmap(BITMAP_FLAG_JPN_ACTIVE_ID), Bitmap(BITMAP_FLAG_JPN_ACTIVE_ID));
    languageSelector.addMenuElement(Bitmap(BITMAP_FLAG_UKR_ACTIVE_ID), Bitmap(BITMAP_FLAG_UKR_ACTIVE_ID));
    languageSelector.addMenuElement(Bitmap(BITMAP_FLAG_NLD_ACTIVE_ID), Bitmap(BITMAP_FLAG_NLD_ACTIVE_ID));
    languageSelector.setElementSpace(20);
    languageSelector.setExpandDirection(CollapsibleMenu::LEFT);
    languageSelector.setStateChangedCallback(languageSelectorStateChanged);
    languageSelector.setTimeout(300);
    languageSelector.finilizeInitialization();
    languageSelector.setXY(469 - languageSelector.getWidth(), 227);
    languageSelector.fadeOut(10);
    add(languageSelector);


    setLanguage();
    animateToPoster(0, 1);
}

void ProductPresenterView::tearDownScreen()
{
}

void ProductPresenterView::buttonPressedHandler(const AbstractButton& button)
{
    if (&button == &activateLanguageSelectorButton)
    {
        if (languageSelector.isFadedOut())
        {
            languageSelector.fadeInAndExpand();
        }
    }
    else if (&button == &nextButton)
    {
        animateToPoster((selectedPoster + 1) % NUMBER_OF_POSTERS, 10);
    }
}

void ProductPresenterView::animateToPoster(int posterIndex, int duration)
{
    if (duration < 2)
    {
        headlineText.setTypedText(TypedText(getHeadline(posterIndex)));
        headlineText.invalidate();

        bodyText.setTypedText(TypedText(getBodyText(posterIndex)));
        bodyText.invalidate();

        poster.setBitmap(Bitmap(getPosterBitmap(posterIndex)));
    }
    else
    {
        nextButton.setTouchable(false);

        // Replace headlineText with swap text
        headlineTextSwap.setTypedText(headlineText.getTypedText());
        headlineTextSwap.setXY(headlineText.getX(), headlineText.getY());
        headlineTextSwap.invalidate();

        // Make headlineText ready to be animated in again
        headlineText.setTypedText(TypedText(getHeadline(posterIndex)));
        headlineText.moveTo(headlineTextSwap.getRect().right(), headlineTextSwap.getY());
        headlineText.invalidate();

        headlineTextSwap.startMoveAnimation(headlineTextSwap.getX() - headlineTextSwap.getWidth(), headlineTextSwap.getY(), duration, EasingEquations::cubicEaseInOut);
        headlineText.startMoveAnimation(headlineTextSwap.getX(), headlineTextSwap.getY(), duration, EasingEquations::cubicEaseInOut);

        // Start text fade
        bodyText.startFadeAnimation(0, duration);
        fadeDuration = duration;
        nextTextIndex = posterIndex;

        // Replace poster with swap poster
        posterSwap.setBitmap(Bitmap(poster.getBitmap()));
        posterSwap.setXY(poster.getX(), poster.getY());
        posterSwap.invalidate();

        // Make poster ready to be animated in again
        poster.setBitmap(Bitmap(getPosterBitmap(posterIndex)));
        poster.moveTo(posterSwap.getRect().right(), posterSwap.getY());
        poster.invalidate();

        posterSwap.startMoveAnimation(posterSwap.getX() - posterSwap.getWidth(), posterSwap.getY(), duration, EasingEquations::cubicEaseInOut);
        poster.startMoveAnimation(posterSwap.getX(), posterSwap.getY(), duration, EasingEquations::cubicEaseInOut);
    }

    selectedPoster = posterIndex;
}

void ProductPresenterView::languageSelectorStateChangedHandler(const CollapsibleMenu& menu, const bool isExpanded)
{
    if (!isExpanded)
    {
        languageSelector.fadeOut();
        setLanguage();
    }
}

void ProductPresenterView::setLanguage()
{
    int languageIndex = languageSelector.getSelectedElementIndex();
    activateLanguageSelectorButton.setBitmaps(Bitmap(inactiveLanguageButtons[languageIndex]), Bitmap(inactiveLanguageButtons[languageIndex]));
    activateLanguageSelectorButton.setWidth(activateLanguageSelectorButton.getWidth() + 20); // Increase the button active area to better react to click
    activateLanguageSelectorButton.setHeight(activateLanguageSelectorButton.getHeight() + 20); // Increase the button active area to better react to click
    activateLanguageSelectorButton.invalidate();

    Texts::setLanguage(languages[languageIndex]);
    bodyText.resizeToCurrentText();
    textAreaContainer.reset();
    textAreaContainer.invalidate();
    mcuLoadTxt.invalidate();
    headlineText.invalidate();
    bodyText.invalidate();
}



void ProductPresenterView::screenSaverMinorTick()
{

}

touchgfx::BitmapId ProductPresenterView::getPosterBitmap(int index)
{
    switch (index)
    {
    default:
    case 0:
        return BITMAP_POSTER_IMAGE_00_ID;
    case 1:
        return BITMAP_POSTER_IMAGE_01_ID;
    case 2:
        return BITMAP_POSTER_IMAGE_02_ID;
    }
}

TEXTS ProductPresenterView::getHeadline(int index)
{
    switch (index)
    {
    default:
    case 0:
        return T_POSTER_HEADLINE_00;
    case 1:
        return T_POSTER_HEADLINE_01;
    case 2:
        return T_POSTER_HEADLINE_02;
    }
}

TEXTS ProductPresenterView::getBodyText(int index)
{
    switch (index)
    {
    default:
    case 0:
        return T_POSTER_TEXT_00;
    case 1:
        return T_POSTER_TEXT_01;
    case 2:
        return T_POSTER_TEXT_02;
    }
}

void ProductPresenterView::fadeAnimationEndedHandler(const FadeAnimator<TextArea>& textArea)
{
    if (&textArea == &bodyText)
    {
        if (bodyText.getAlpha() == 0)
        {
            bodyText.setTypedText(TypedText(getBodyText(nextTextIndex)));
            bodyText.resizeToCurrentText();
            bodyText.startFadeAnimation(255, fadeDuration);

            textAreaContainer.reset();
            textAreaContainer.invalidate();

            nextButton.setTouchable(true);
        }
    }

}
