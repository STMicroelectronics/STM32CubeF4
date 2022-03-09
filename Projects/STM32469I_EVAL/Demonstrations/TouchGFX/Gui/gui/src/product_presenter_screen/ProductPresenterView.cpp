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
    DemoView(),
    screenSaverCounter(0),
    onButtonPressed(this, &ProductPresenterView::buttonPressedHandler),
    languageSelectorStateChanged(this, &ProductPresenterView::languageSelectorStateChangedHandler)
{
}

ProductPresenterView::~ProductPresenterView()
{
    Texts::setLanguage(GBR);
}

void ProductPresenterView::setupScreen()
{
    background.setBitmap(Bitmap(BITMAP_PRODUCT_PRESENTER_BACKGROUND_ID));
    background.setXY(0, 0);
    add(background);

    poster[0].setup(BITMAP_POSTER_IMAGE_00_ID, T_POSTER_00_HEADLINE, T_POSTER_00_TEXT, false);
    poster[1].setup(BITMAP_POSTER_IMAGE_01_ID, T_POSTER_01_HEADLINE, T_POSTER_01_TEXT, true);
    poster[2].setup(BITMAP_POSTER_IMAGE_02_ID, T_POSTER_02_HEADLINE, T_POSTER_02_TEXT, false);

    poster[2].setHeadlineColor(Color::getColorFrom24BitRGB(0x0, 0x0, 0x0));

    for (int i = 0; i < NUMBER_OF_POSTERS; i++)
    {
        // Enlarge the poster swipe area by placing it in the posters container
        poster[i].setXY(302, 0);
        posters[i].add(poster[i]);
        posters[i].setPosition(0, 0, 302 + poster[i].getWidth() + 210, poster[i].getHeight());
        posterSwipeContainer.add(posters[i]);
    }

    posterSwipeContainer.setXY(0, 50);
    posterSwipeContainer.setSwipeCutoff(100);
    posterSwipeContainer.setEndSwipeElasticWidth(70);

    add(posterSwipeContainer);

    paperOverlay.setBitmap(Bitmap(BITMAP_PAPER_OVERLAY_ID));
    paperOverlay.setXY(0, getActiveDisplayHeight() - paperOverlay.getHeight());
    add(paperOverlay);

    activateLanguageSelectorButton.setXY(726, 433);
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
    languageSelector.setElementSpace(24);
    languageSelector.setExpandDirection(CollapsibleMenu::LEFT);
    languageSelector.setStateChangedCallback(languageSelectorStateChanged);
    languageSelector.setTimeout(300);
    languageSelector.finilizeInitialization();
    languageSelector.setXY(800 - 20 - languageSelector.getWidth(), 429);
    languageSelector.fadeOut(10);
    add(languageSelector);

    gotoMenuButton.setBitmaps(Bitmap(BITMAP_MENU_BUTTON_ID), Bitmap(BITMAP_MENU_BUTTON_ACTIVE_ID));
    gotoMenuButton.setXY(20, 18);
    add(gotoMenuButton);

    mcuLoadTxt.setXY(618, 40 - mcuLoadTxt.getTypedText().getFont()->getFontHeight());
    mcuLoadValueTxt.setPosition(mcuLoadTxt.getRect().right(), mcuLoadTxt.getY(), 45, 30);
    add(mcuLoadTxt);
    add(mcuLoadValueTxt);

    setLanguage();

    posterSwipeContainer.moveTo(-450, posterSwipeContainer.getY());
    posterSwipeContainer.startMoveAnimation(0, posterSwipeContainer.getY(), 24, EasingEquations::backEaseOut);
}

void ProductPresenterView::tearDownScreen()
{
}

void ProductPresenterView::buttonPressedHandler(const AbstractButton& button)
{
    if (languageSelector.isFadedOut())
    {
        languageSelector.fadeInAndExpand();
    } 
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
    activateLanguageSelectorButton.invalidate();

    Texts::setLanguage(languages[languageIndex]);
    mcuLoadTxt.invalidate();
    for (int i = 0; i < NUMBER_OF_POSTERS; i++)
    {
        poster[i].invalidateText();
    }
}

void ProductPresenterView::screenSaverMinorTick()
{
    screenSaverCounter++;

    if (screenSaverCounter > 10)
    {
        gotoMenuClickedHandler(gotoMenuButton);
    }
    else
    {
        poster[posterSwipeContainer.getSelectedScreen()].toggleViewMode();
    }
}
