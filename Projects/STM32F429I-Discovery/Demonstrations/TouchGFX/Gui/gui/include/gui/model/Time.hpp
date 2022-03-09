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
  


#ifndef TIME_HPP
#define TIME_HPP

#include <touchgfx/hal/Types.hpp>

struct Time
{
    Time() : hours(0), minutes(0), seconds(0), milliseconds(0) {}

    explicit Time(int ms)
    {
        hours = ms / 3600000;
        ms -= hours * 3600000;
        minutes = ms / 60000;
        ms -= minutes * 60000;
        seconds = ms / 1000;
        ms -= seconds * 1000;
        milliseconds = ms;
    }

    Time(uint8_t h, uint8_t m, uint8_t s, uint16_t millis) : hours(h), minutes(m), seconds(s), milliseconds(millis) {}

    bool operator==(const Time& other) const
    {
        return ((hours == other.hours) &&
                (minutes == other.minutes) &&
                (seconds == other.seconds) &&
                (milliseconds == other.milliseconds));
    }

    bool operator!=(const Time& other) const
    {
        return !(*this == other);
    }

    // Returns the time difference as count of milliseconds
    long operator-(const Time& other) const
    {
        int diff = (hours - other.hours) * 60 * 60 * 1000 +
                   (minutes - other.minutes) * 60 * 1000 +
                   (seconds - other.seconds) * 1000 +
                   (milliseconds - other.milliseconds);
        if (diff < 0)
        {
            diff += 24 * 60 * 60 * 1000;
        }
        return diff;
    }

    static float toSeconds(long milliseconds)
    {
        return milliseconds / (float) 1000.0;
    }

    bool hasValue() const
    {
        return ((hours > 0) ||
                (minutes > 0) ||
                (seconds > 0) ||
                (milliseconds > 0));
    }

    int getValueInSeconds() const
    {
        return hours * 3600 + minutes * 60 + seconds;
    }

    int getValueInMillis() const
    {
        return getValueInSeconds() * 1000 + milliseconds;
    }

    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
    uint16_t milliseconds;
};

#endif /* TIME_HPP */
