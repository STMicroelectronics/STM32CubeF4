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
