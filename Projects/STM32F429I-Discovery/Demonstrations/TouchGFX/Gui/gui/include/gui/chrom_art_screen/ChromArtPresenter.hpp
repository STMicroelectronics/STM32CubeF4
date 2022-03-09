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
  


#ifndef CHROM_ART_PRESENTER_HPP
#define CHROM_ART_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class ChromArtView;

class ChromArtPresenter : public Presenter, public ModelListener
{
public:
    ChromArtPresenter(ChromArtView& v);
    virtual ~ChromArtPresenter();

    virtual void activate();
    virtual void deactivate();

    virtual void mcuLoadUpdated(uint8_t mcuLoad);

private:
    ChromArtPresenter();

    ChromArtView& view;
};

#endif // CHROM_ART_PRESENTER_HPP
