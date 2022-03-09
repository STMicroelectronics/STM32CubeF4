/**
  ******************************************************************************
  * @file    ts_calibration.h
  * @author  MCD Application Team
  * @brief   Header for ts_calibration file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
  

#ifndef TS_CALIBRATION_H
#define TS_CALIBRATION_H

uint8_t Touchscreen_Calibration(void);
uint16_t TouchScreen_Get_Calibrated_X(uint16_t x);
uint16_t TouchScreen_Get_Calibrated_Y(uint16_t y);
uint8_t TouchScreen_IsCalibrationDone(void);

#endif /* TS_CALIBRATION_H */
