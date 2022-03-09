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
  

#ifndef FEATUREPOSTER_HPP
#define FEATUREPOSTER_HPP

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/AnimatedImage.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/mixins/MoveAnimator.hpp>
#include <touchgfx/mixins/FadeAnimator.hpp>
#include <touchgfx/containers/ZoomAnimationImage.hpp>
#include <texts/TextKeysAndLanguages.hpp>


using namespace touchgfx;

class FeaturePoster : public Container
{
public:
    FeaturePoster();
    virtual ~FeaturePoster();

    void setup(BitmapId posterImage, TEXTS posterHeadline, TEXTS posterText, bool showAlphaExample);
    void invalidateText();

    void setHeadlineColor(uint16_t color);

    void toggleViewMode();

protected:
    enum State
    {
        SHOWING_IMAGE,
        SHOWING_TEXT
    };

    State currentState;

    Image shadow[4]; // Shadows around the poster

    Container posterArea; 

    MoveAnimator<Image> posterImage;
    MoveAnimator<Button> startAnimationButton;
    MoveAnimator<AnimatedImage> startAnimationButtonArrow;

    FadeAnimator<Box> alphaBox;

    TextArea headlineBottom;

    MoveAnimator<TextArea> headlineTop;
    int headlineTopStartY;

    MoveAnimator<Container> textAreaContainer;
    Box textBackground;
    TextArea text;
    int textStartY;

    FadeAnimator<Image> gradientOverlay;

    Callback<FeaturePoster, const AbstractButton& > startAnimationCallback;
    void startAnimationHandler(const AbstractButton& button);

    void animate();

    MoveAnimator<Image> redBall;
    MoveAnimator<Image> greenBall;
    MoveAnimator<Image> blueBall;
    Callback<FeaturePoster, const MoveAnimator<Image>& > moveAnimationEnded;
    void moveAnimationEndedHandler(const MoveAnimator<Image>& image);
};

#endif /* FEATUREPOSTER_HPP */
