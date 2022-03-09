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
  


#include <gui/chrom_art_screen/ChromArtPresenter.hpp>
#include <gui/chrom_art_screen/ChromArtView.hpp>
#include <gui/common/FrontendApplication.hpp>

ChromArtPresenter::ChromArtPresenter(ChromArtView& v) :
    view(v)
{
}

ChromArtPresenter::~ChromArtPresenter()
{
}

void ChromArtPresenter::activate()
{

}

void ChromArtPresenter::deactivate()
{

}

void ChromArtPresenter::mcuLoadUpdated(uint8_t mcuLoad)
{
    view.updateMCULoad(mcuLoad);
}
