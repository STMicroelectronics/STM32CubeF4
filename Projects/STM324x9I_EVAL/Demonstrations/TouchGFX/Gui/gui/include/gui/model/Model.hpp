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
  


#ifndef MODEL_HPP
#define MODEL_HPP

#include <gui/model/Time.hpp>

class ModelListener;

/**
 * The Model class defines the data model in the model-view-presenter paradigm.
 * The Model is a singular object used across all presenters. The currently active
 * presenter will have a pointer to the Model through deriving from ModelListener.
 *
 * The Model will typically contain UI state information that must be kept alive
 * through screen transitions. It also usually provides the interface to the rest
 * of the system (the backend). As such, the Model can receive events and data from
 * the backend and inform the current presenter of such events through the modelListener
 * pointer, which is automatically configured to point to the current presenter.
 * Conversely, the current presenter can trigger events in the backend through the Model.
 */
class Model
{
public:
    Model();

    /**
     * Sets the modelListener to point to the currently active presenter. Called automatically
     * when switching screen.
     */
    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    void tick();

    Time getCurrentTime() const
    {
        return currentTime;
    }

    bool getMCULoadActive() const
    {
        return mcuLoadActive;
    }
    void setMCULoadActive(bool active)
    {
        mcuLoadActive = active;
    }

    void setSelectedMenuIndex(uint8_t index)
    {
        selectedMenuIndex = index;
    }
    uint8_t getSelectedMenuIndex() const
    {
        return selectedMenuIndex;
    }

    void screenSaverMinorTick();
    void screenSaverMajorTick();

protected:
    /**
     * Pointer to the currently active presenter.
     */
    ModelListener* modelListener;

    bool mcuLoadActive;

    Time currentTime;

    uint8_t selectedMenuIndex;
};

#endif /* MODEL_HPP */
