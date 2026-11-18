/**
  ******************************************************************************
  * @file    iis2dlpc.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    17-Juin-2026
  * @brief   This file contains all the functions prototypes for the iis2dlpc.c
  *          firmware driver.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __IIS2DLPC_H
#define __IIS2DLPC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "../Common/accelero.h"

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup Components
  * @{
  */ 

/** @addtogroup IIS2DLPC
  * @{
  */

/** @defgroup IIS2DLPC_Exported_Types
  * @{
  */
/* IIS2DLPC struct */
typedef struct
{
  uint8_t Output_DataRate;              /* Output Data Rate into CTRL_REG1 register */
  uint8_t Axes_Enable;                  /* Axes enable into non register */
  uint8_t SPI_Wire;                     /* SPI Wire into CTRL_REG2 register */
  uint8_t Self_Test;                    /* Self test into CTRL_REG3 register */  
  uint8_t Full_Scale;                   /* Full scale into CTRL_REG6 register */
  uint8_t Filter_BW;                    /* Filter BW into CTRL_REG6 register */
}IIS2DLPC_InitTypeDef;

/* IIS2DLPC Interrupt struct */
typedef struct
{
  uint8_t Interrupt_Request;            /* Latch/Pulse Interrupt request into CTRL_REG3 register */
  uint8_t Interrupt_Selection_Enable;   /* Interrupt1/2 selection enable into CTRL_REG3 register */
  uint8_t Interrupt_Signal;             /* Interrupt signal active Low/High into CTRL_REG3 register */  
}IIS2DLPC_InterruptConfigTypeDef;  
/**
  * @}
  */

/** @defgroup IIS2DLPC_Exported_Constants
  * @{
  */

/******************************************************************************/
/*************************** START REGISTER MAPPING  **************************/
/******************************************************************************/

/*******************************************************************************
*  Temperature Low Register (Temperature output register in 12-bit resolution)
*  Read only register
*  Default value: 0x00
*  7:4 TEMP3-TEMP0: Temperature Low 4-bit
*  3:0 (0)
*******************************************************************************/
#define IIS2DLPC_OUT_T_L_ADDR                 0x0D
/*******************************************************************************
*  Temperature High Register (Temperature output register in 12-bit resolution)
*  Read only register
*  Default value: 0x00
*  7:0 TEMP11-TEMP4: Temperature High 8-bit
*******************************************************************************/
#define IIS2DLPC_OUT_T_H_ADDR                 0x0E


/*******************************************************************************
*  WHO_AM_I Register: Device Identification Register
*  Read only register
*  Default value: 0x44
*******************************************************************************/
#define IIS2DLPC_WHO_AM_I_ADDR                0x0F


/*******************************************************************************
*  CTRL_REG1 Register: Control Register 1
*  Read Write register
*  Default value: 0x00
*  7:4 ODR3-ODR0: Data rate selection
*      ODR3 | ODR2 | ODR1 | ORD0 | ORD Selection
*      -------------------------------------------
*       0  |  0   |  0   |  0   | Power Down (Default)
*       0  |  0   |  0   |  1   | HP/LP 12.5/1.6 Hz
*       0  |  0   |  1   |  0   | 12.5 Hz
*       0  |  0   |  1   |  1   | 25 Hz
*       0  |  1   |  0   |  0   | 50 Hz
*       0  |  1   |  0   |  1   | 100 Hz
*       0  |  1   |  1   |  0   | 200 Hz
*       0  |  1   |  1   |  1   | HP/LP 400/200 Hz
*       1  |  0   |  0   |  0   | HP/LP 800/200 Hz
*       1  |  0   |  0   |  1   | HP/LP 1600/200 Hz
*
*  3:2 MODE1-MODE0: Mode selection
*      MODE1 | MODE0 | MODE Selection
*      --------------------------------
*        0   |  0   | Low-Power Mode (12/14-bit resolution) (Default)
*        0   |  1   | High-Performance Mode (14-bit resolution)
*        1   |  0   | Single data conversion on demand mode (12/14-bit resolution)
*        1   |  1   | -
*
*  1:0 LP_MODE1-LP_MODE0: Low-power Mode selection
*      LPM1 | LPM0 | Low-power Mode Selection
*      ------------------------------------------
*       0   |  0   | Low-Power Mode 1 (12-bit resolution)
*       0   |  1   | Low-Power Mode 2 (14-bit resolution)
*       1   |  0   | Low-Power Mode 3 (14-bit resolution)
*       1   |  1   | Low-Power Mode 4 (14-bit resolution)
*******************************************************************************/
#define IIS2DLPC_CTRL_REG1_ADDR               0x20

/*******************************************************************************
*  CTRL_REG1 Register: Control Register 2
*  Read Write register
*  Default value: 0x04
*  7 BOOT: Force reboot, cleared as soon as the reboot is finished. Active High. 
*  6 SOFT_RESET: Soft reset acts as reset for all control registers, then goes to 0.
*  5 (0): This bit must be set to   0   for the correct operation of the device.
*  4 CS_PU_DISC: Disconnect CS pull-up. Default value: 0
*  3 BDU: Block data update.0: continuous update; 1: output registers not updated until MSB and LSB read
*  2 IF_ADD_INC: Register address automatically incremented during multiple byte access (0: disabled; 1: enabled)
*  1 I2C_DISABLE: Disable I2C communication protocol. Default value: 0
*  0 SIM: SPI serial interface mode selection. Default value: 0
*         0: 4-wire interface; 1: 3-wire interface
*******************************************************************************/
#define IIS2DLPC_CTRL_REG2_ADDR               0x21

/*******************************************************************************
*  CTRL_REG3 Register: Control Register 3
*  Read Write register
*  Default value: 0x00
*  7:6 ST2_ST1: Self-test Enable
*               ST2 | ST1 | ST Selection
*               -------------------------
*                0  |  0  | Normal Mode (Default)
*                0  |  1  | Positive sign self-test
*                1  |  0  | Negative sign-test
*                1  |  1  | Not Allowed
*
*  5 PP_OD: Push-pull/open-drain selection on interrupt pad. Default value: 0
*           0: push-pull; 1: open-drain
*  4 LIR: Latched Interrupt. Switches between latched ('1'-logic) and pulsed ('0'-logic) mode
*         0: interrupt request not latched; 1: interrupt request latched
*  3 H_LACTIVE: Interrupt active high, low. Default value: 0
*               0: active high; 1: active low
*  2 Reserved
*  1 SLP_MODE_SEL: Single data conversion on demand mode selection
*                  0: enabled with external trigger on INT2;
*                  1: enabled by I2C/SPI writing SLP_MODE_1 to 1.
*  0 SLP_MODE_1: Single data conversion on demand mode enable.
*******************************************************************************/
#define IIS2DLPC_CTRL_REG3_ADDR               0x22

/*******************************************************************************
*  CTRL_REG4 Register: Control Register 4 (INT1_PAD register)
*  Read Write register
*  Default value: 0x00
*  7 INT1_6D: 6D recognition is routed to INT1 pad
*             0: disabled; 1: enabled
*  6 INT1_SINGLE_TAP: Single-tap recognition is routed to INT1 pad. Default value: 0
*                     0: disabled; 1: enabled
*  5 INT1_WU: Wakeup recognition is routed to INT1 pad. Default value: 0
*             0: disabled; 1: enabled
*  4 INT1_FF: Free-fall recognition is routed to INT1 pad. Default value: 0
*             0: disabled; 1: enabled
*  3 INT1_TAP: Double-tap recognition is routed to INT1 pad. Default value: 0
*              0: disabled; 1: enabled
*  2 INT1_DIFF5: FIFO full recognition is routed to INT1 pad. Default value: 0
*                0: disabled; 1: enabled
*  1 INT1_FTH: FIFO threshold interrupt is routed to INT1 pad. Default value: 0
*              0: disabled; 1: enabled
*  0 INT1_DRDY: Data-Ready is routed to INT1 pad. Default value: 0
*               0: disabled; 1: enabled
*******************************************************************************/
#define IIS2DLPC_CTRL_REG4_ADDR               0x23              

/*******************************************************************************
*  CTRL_REG5 Register: Control Register 5 (INT2_PAD register)
*  Read Write register
*  Default value: 0x00
*  7 INT2_SLEEP_STATE: Enable routing of SLEEP_STATE on INT2 pad. Default value: 0
*                      0: disabled; 1: enabled
*  6 INT2_SLEEP_CHG: Sleep change status routed to INT2 pad. Default value: 0
*                    0: disabled; 1: enabled
*  5 INT2 _BOOT: Boot state routed to INT2 pad. Default value: 0
*                0: disabled; 1: enabled
*  4 INT2_DRDY_T: Temperature data-ready is routed to INT2. Default value: 0
*                 0: disabled; 1: enabled
*  3 INT2_OVR: FIFO overrun interrupt is routed to INT2 pad. Default value: 0
*              0: disabled; 1: enabled
*  2 INT2_DIFF5: FIFO full recognition is routed to INT2 pad. Default value: 0
*                0: disabled; 1: enabled
*  1 INT2_FTH: FIFO threshold interrupt is routed to INT2 pad. Default value: 0
*              0: disabled; 1: enabled
*  0 INT2_DRDY: Data-Ready is routed to INT2 pad. Default value: 0
*               0: disabled; 1: enabled
*******************************************************************************/
#define IIS2DLPC_CTRL_REG5_ADDR               0x24

/*******************************************************************************
*  CTRL_REG6 Register: Control Register 6
*  Read Write register
*  Default value: 0x00
*  7:6 BW_FILT[1:0]: Bandwidth selection
*      BW1 | BW0 | BW Selection
*      -------------------------
*       0  |  0  | ODR/2 up to 800 Hz (Default)
*       0  |  1  | ODR/4 up to 400 Hz
*       1  |  0  | ODR/10 up to 80 Hz
*       1  |  1  | ODR/20 up to 40 Hz
*
*  5:4 FS1-FS0: Full scale selection
*      FS1 | FS0 | Full scale selection
*      -----------------------------------
*       0  |  0  | +/-2g (Default)
*       0  |  1  | +/-4g
*       1  |  0  | +/-8g
*       1  |  1  | +/-16g
*
*  3 FDS: Filtered data type selection. Default value: 0
*         0: low-pass filter path selected; 1: high-pass filter path selected
*  2 LOW_NOISE: Low-noise configuration.
*               0: disabled; 1: enabled
*  1 (0)
*  0 (0)
*******************************************************************************/
#define IIS2DLPC_CTRL_REG6_ADDR               0x25

/*******************************************************************************
*  OUT_T Register: Temperature Output Register
*  Read only register
*  Default value: 0x00 (means 25  C ambient ?)
*  7:0 Temp7-Temp0: Temperature output Data
*******************************************************************************/
#define IIS2DLPC_OUT_T_ADDR                   0x26

/*******************************************************************************
*  STATUS Register: Status Data Register
*  Read only register
*  Default value: 0x00
*  7 FIFO_THS: FIFO threshold status flag.
*              0: FIFO filling is lower than threshold level; (Default)
*              1: FIFO filling is equal to or higher than the threshold level.
*  6 WU_IA: Wakeup event detection status.
*           0: Wakeup event not detected; 1: Wakeup event detected
*  5 SLEEP_STATE: Sleep event status.
*                 0: Sleep event not detected; 1: Sleep event detected
*  4 DOUBLE_TAP: Double-tap event status
*                0: Double-tap event not detected; 1: Double-tap event detected
*  3 SINGLE_TAP: Single-tap event status
*                0: Single-tap event not detected; 1: Single-tap event detected
*  2 6D_IA: Source of change in position portrait/landscape/face-up/face-down.
*           0: no event detected; 1: a change in position detected
*  1 FF_IA: Free-fall event detection status.
*           0: free-fall event not detected; 1: free-fall event detected
*  0 DRDY: Data-ready status.
*          0: not ready; 1: X-, Y- and Z-axis new data available
*******************************************************************************/
#define IIS2DLPC_STATUS_ADDR                  0x27

/*******************************************************************************
*  OUT_X_L Register: X-axis Output Acceleration Low Data
*  Read only register
*  Default value: output
*  7:0 XD7-XD0: X-axis output Data
*******************************************************************************/
#define IIS2DLPC_OUT_X_L_ADDR                 0x28

/*******************************************************************************
*  OUT_X_H Register: X-axis Output Acceleration High Data
*  Read only register
*  Default value: output
*  15:8 XD15-XD8: X-axis output Data
*******************************************************************************/
#define IIS2DLPC_OUT_X_H_ADDR                 0x29

/*******************************************************************************
*  OUT_Y_L Register: Y-axis Output Acceleration Low Data
*  Read only register
*  Default value: output
*  7:0 YD7-YD0: Y-axis output Data
*******************************************************************************/
#define IIS2DLPC_OUT_Y_L_ADDR                 0x2A

/*******************************************************************************
*  OUT_Y_H Register: Y-axis Output Acceleration High Data
*  Read only register
*  Default value: output
*  15:8 YD15-YD8: Y-axis output Data
*******************************************************************************/
#define IIS2DLPC_OUT_Y_H_ADDR                 0x2B

/*******************************************************************************
*  OUT_Z_L Register: Z-axis Output Acceleration Low Data
*  Read only register
*  Default value: output
*  7:0 ZD7-ZD0: Z-axis output Data
*******************************************************************************/
#define IIS2DLPC_OUT_Z_L_ADDR                 0x2C

/*******************************************************************************
*  OUT_Z_H Register: Z-axis Output Acceleration High Data
*  Read only register
*  Default value: output
*  15:8 ZD15-ZD8: Z-axis output Data
*******************************************************************************/
#define IIS2DLPC_OUT_Z_H_ADDR                 0x2D

/*******************************************************************************
*  FIFO_CTRL Register: FIFO Control Register
*  Read/Write register
*  Default value: 0x00
*  7:5 FMODE2-FMODE0: FIFO mode
*        FMODE2 | FMODE1 | FMODE0 | Mode description
*        --------------------------------------------------
*          0    |    0   |    0   | Bypass mode. FIFO turned off. (Default)
*          0    |    0   |    1   | FIFO mode. Stop collecting data when FIFO is full.
*          0    |    1   |    0   | Not to use.
*          0    |    1   |    1   | Continuous-to-FIFO: Stream mode until trigger is deasserted, then FIFO mode
*          1    |    0   |    0   | Bypass-to-Continuous: Bypass mode until trigger is deasserted, then FIFO mode
*          1    |    0   |    1   | Not to use.
*          1    |    1   |    0   | Continuous mode: If the FIFO is full, the new sample overwrites the older sample.
*          1    |    1   |    1   | Not to use.
*
*  4:0 FTH4-FTH0: FIFO threshold level setting.
*******************************************************************************/
#define IIS2DLPC_FIFO_CTRL_ADDR               0x2E

/*******************************************************************************
*  FIFO_SAMPLES Register: FIFO Samples control Register
*  Read only register
*  Default value: 0x00
*  7 FIFO_FTH: FIFO threshold status flag.
*              0: FIFO filling is lower than threshold level (Default)
*              1: FIFO filling is equal or higher than threshold level 
*  6 FIFO_OVR: FIFO overrun status.
*              0: FIFO is not completely filled (Default)
*              1: FIFO is completely filled and at least one sample has been overwritten
*  5:0 Diff: Represents the number of unread samples stored in FIFO.
*            000000 = FIFO empty; 100000 = FIFO full, 32 unread samples
*******************************************************************************/
#define IIS2DLPC_FIFO_SRC_ADDR                0x2F


/*******************************************************************************
*  TAP_THS_X Register: 4D configuration enable and TAP threshold configuration Register
*  Read/Write register
*  Default value: 0x00
*******************************************************************************/
#define IIS2DLPC_TAP_THS_X_ADDR               0x30

/*******************************************************************************
*  TAP_THS_Y Register: 4D configuration enable and TAP threshold configuration Register
*  Read/Write register
*  Default value: 0x00
*******************************************************************************/
#define IIS2DLPC_TAP_THS_Y_ADDR               0x31

/*******************************************************************************
*  TAP_THS_Z Register: 4D configuration enable and TAP threshold configuration Register
*  Read/Write register
*  Default value: 0x00
*******************************************************************************/
#define IIS2DLPC_TAP_THS_Z_ADDR               0x32


/*******************************************************************************
*  INT_DUR Register: Interrupt duration Register
*  Read/Write register
*  Default value: 0x00
*******************************************************************************/
#define IIS2DLPC_INT_DUR_ADDR                 0x33

/*******************************************************************************
*  WAKE_UP_THS Register: Wakeup threshold Register
*  Read/Write register
*  Default value: 0x00
*******************************************************************************/
#define IIS2DLPC_WAKE_UP_THS_ADDR             0x34

/*******************************************************************************
*  WAKE_UP_DUR Register: Wakeup and sleep duration configuration Register
*  Read/Write register
*  Default value: 0x00
*******************************************************************************/
#define IIS2DLPC_WAKE_UP_DUR_ADDR             0x35

/*******************************************************************************
*  FREE_FALL Register: Free-fall duration and threshold configuration Register
*  Read/Write register
*  Default value: 0x00
*******************************************************************************/
#define IIS2DLPC_FREE_FALL_ADDR               0x36

/*******************************************************************************
*  STATUS_DUP Register: Event detection status Register
*  Read only register
*  Default value: 0x00
*******************************************************************************/
#define IIS2DLPC_STATUS_DUP_ADDR              0x37

/*******************************************************************************
*  WAKE_UP_SRC Register: Wakeup source Register
*  Read only register
*  Default value: 0x00
*******************************************************************************/
#define IIS2DLPC_WAKE_UP_SRC_ADDR             0x38

/*******************************************************************************
*  TAP_SRC Register: Tap source Register
*  Read only register
*  Default value: 0x00
*******************************************************************************/
#define IIS2DLPC_TAP_SRC_ADDR                 0x39

/*******************************************************************************
*  SIXD_SRC Register: 6D source Register
*  Read only register
*  Default value: 0x00
*******************************************************************************/
#define IIS2DLPC_SIXD_SRC_ADDR                0x3A

/*******************************************************************************
*  ALL_INT_SRC Register: Clear ALL_INT Register
* (Reading this register, all related interrupt function flags routed to the INT pads are reset simultaneously.)
*  Read only register
*  Default value: 0x00
*******************************************************************************/
#define IIS2DLPC_ALL_INT_SRC_ADDR             0x3B

/*******************************************************************************
*  X_OFS_USR Register: user offset value on X-axis data Register (used for wakeup function)
*  Read/Write register
*  Default value: 0x00
*******************************************************************************/
#define IIS2DLPC_X_OFS_USR_ADDR               0x3C

/*******************************************************************************
*  Y_OFS_USR Register: user offset value on Y-axis data Register (used for wakeup function)
*  Read/Write register
*  Default value: 0x00
*******************************************************************************/
#define IIS2DLPC_Y_OFS_USR_ADDR               0x3D

/*******************************************************************************
*  Z_OFS_USR Register: user offset value on Z-axis data Register (used for wakeup function)
*  Read/Write register
*  Default value: 0x00
*******************************************************************************/
#define IIS2DLPC_Z_OFS_USR_ADDR               0x3E

/*******************************************************************************
*  CTRL_REG7 Register: Control Register 7
*  Read Write register
*  Default value: 0x00
*  7 DRDY_PULSED: Switches between latched and pulsed mode for data ready interrupt.
*                 0: latched mode is used; 1: pulsed mode enabled for data-ready
*  6 INT2_ON_INT1: Signal routing.
*                  1: all signals available only on INT2 are routed on INT1)
*  5 INTERRUPTS_ENABLE: Enable interrupts.
*  4 USR_OFF_ON_OUT: Enable application of user offset value on XL output data registers.
*                    FDS bit in CTRL6 (25h) must be set to '0'-logic (low-pass path selected).
*  3 USR_OFF_ON_WU: Enable application of user offset value on XL data for wakeup function only.
*  2 USR_OFF_W: Selects the weight of the user offset words specified by X_OFS_USR_[7:0],
*               Y_OFS_USR_[7:0] and Z_OFS_USR_[7:0] bits.
*               (0: 977   g/LSB; 1: 15.6 mg/LSB)
*  1 HP_REF_MODE: High-pass filter reference mode enable.
*                 0: high-pass filter reference mode disabled (default);
*                 1: high-pass filter reference mode enabled
*  0 LPASS_ON6D: low pass filtered data sent to 6D interrupt function
*                 0: ODR/2 low pass filtered data sent to 6D interrupt function (default);
*                 1: LPF2 output data sent to 6D interrupt function
*******************************************************************************/
#define IIS2DLPC_CTRL_REG7_ADDR               0x3F


/******************************************************************************/
/**************************** END REGISTER MAPPING  ***************************/
/******************************************************************************/
#define I_AM_IIS2DLPC                         0x44

/** @defgroup Sensitivity_selection               
  * @{
  */
#define IIS2DLPC_SENSITIVITY_0_06G            0.06  /* 0.06 mg/digit*/
#define IIS2DLPC_SENSITIVITY_0_12G            0.12  /* 0.12 mg/digit*/
#define IIS2DLPC_SENSITIVITY_0_18G            0.18  /* 0.18 mg/digit*/
#define IIS2DLPC_SENSITIVITY_0_24G            0.24  /* 0.24 mg/digit*/
#define IIS2DLPC_SENSITIVITY_0_73G            0.73  /* 0.73 mg/digit*/
/**
  * @}
  */

/** @defgroup Data_Rate_Power_Mode_selection                
  * @{
  */
#define IIS2DLPC_DATARATE_POWERDOWN           ((uint8_t)0x00)  /* Power Down Mode*/
#define IIS2DLPC_DATARATE_1_6                 ((uint8_t)0x10)  /* 12.5/1.6 Hz Normal/LP Mode */
#define IIS2DLPC_DATARATE_12_5                ((uint8_t)0x20)  /* 12.5  Hz Mode */
#define IIS2DLPC_DATARATE_25                  ((uint8_t)0x30)  /* 25    Hz Mode */
#define IIS2DLPC_DATARATE_50                  ((uint8_t)0x40)  /* 50    Hz Mode */
#define IIS2DLPC_DATARATE_100                 ((uint8_t)0x50)  /* 100   Hz Mode */
#define IIS2DLPC_DATARATE_200                 ((uint8_t)0x60)  /* 200   Hz Mode */
#define IIS2DLPC_DATARATE_400                 ((uint8_t)0x70)  /* 400/200   Hz Normal/LP Mode */
#define IIS2DLPC_DATARATE_800                 ((uint8_t)0x80)  /* 800/200   Hz Normal/LP Mode */
#define IIS2DLPC_DATARATE_1600                ((uint8_t)0x90)  /* 1600/200  Hz Normal/LP Mode */
#define IIS2DLPC_DATARATE_BITS                ((uint8_t)0xF0)  /* all Data_Rate BITS */

/** @defgroup Low_Power_Mode_selection                
  * @{
  */
#define IIS2DLPC_LPM_1                      ((uint8_t)0x00)  /* Low_Power_Mode 1(12-bit resolution) */
#define IIS2DLPC_LPM_2                      ((uint8_t)0x01)  /* Low_Power_Mode 2(14-bit resolution) */
#define IIS2DLPC_LPM_3                      ((uint8_t)0x02)  /* Low_Power_Mode 3(14-bit resolution) */
#define IIS2DLPC_LPM_4                      ((uint8_t)0x03)  /* Low_Power_Mode 4(14-bit resolution) */

/**
  * @}
  */
  
/** @defgroup Full_Scale_selection 
  * @{
  */
#define IIS2DLPC_FULLSCALE_2                  ((uint8_t)0x00)  /* 2 g  */
#define IIS2DLPC_FULLSCALE_4                  ((uint8_t)0x10)  /* 4 g  */
#define IIS2DLPC_FULLSCALE_8                  ((uint8_t)0x20)  /* 8 g  */
#define IIS2DLPC_FULLSCALE_16                 ((uint8_t)0x30)  /* 16 g */
#define IIS2DLPC__FULLSCALE_SELECTION         ((uint8_t)0x30)
/**
  * @}
  */
    
/** @defgroup Anti-Aliasing_Filter_Bandwidth 
  * @{
  */
#define IIS2DLPC_FILTER_BW_800                ((uint8_t)0x00)  /* 800 Hz */
#define IIS2DLPC_FILTER_BW_400                ((uint8_t)0x08)  /* 400 Hz */
#define IIS2DLPC_FILTER_BW_160                ((uint8_t)0x10)  /* 160 Hz */
#define IIS2DLPC_FILTER_BW_80                 ((uint8_t)0x18)  /* 80  Hz */
/**
  * @}
  */
  
/** @defgroup Self_Test_selection
  * @{
  */
#define IIS2DLPC_SELFTEST_NORMAL              ((uint8_t)0x00)
#define IIS2DLPC_SELFTEST_P                   ((uint8_t)0x40)
#define IIS2DLPC_SELFTEST_N                   ((uint8_t)0x80)
/**
  * @}
  */  

/** @defgroup Direction_XYZ_selection  
  * @{
  */
#define IIS2DLPC_X_ENABLE                     ((uint8_t)0x01)
#define IIS2DLPC_Y_ENABLE                     ((uint8_t)0x02)
#define IIS2DLPC_Z_ENABLE                     ((uint8_t)0x04)
#define IIS2DLPC_XYZ_ENABLE                   ((uint8_t)0x07)
/**
  * @}
  */
 
/** @defgroup SPI_Serial_Interface_Mode_selection  
  * @{
  */
#define IIS2DLPC_SERIALINTERFACE_4WIRE        ((uint8_t)0x00)
#define IIS2DLPC_SERIALINTERFACE_3WIRE        ((uint8_t)0x01)
/**
  * @}
  */ 

/** @defgroup Interrupt_Request_selection    Interrupt_Selection_Enable, CTRL_REG3
  * @{
  */
#define IIS2DLPC_INTERRUPT_REQUEST_PULSED     ((uint8_t)0x00)
#define IIS2DLPC_INTERRUPT_REQUEST_LATCHED    ((uint8_t)0x10)
/**
  * @}
  */ 

/** @defgroup Interrupt_Selection_Enable, INT1_SINGLE_TAP + INT1_DRDY @ CTRL_REG4
  * @{
  */
#define IIS2DLPC_INTERRUPT_1_ENABLE           ((uint8_t)0x41)
#define IIS2DLPC_INTERRUPT_2_ENABLE           ((uint8_t)0x00)
#define IIS2DLPC_INTERRUPT_1_2_ENABLE         ((uint8_t)0x41)
/**
  * @}
  */ 

/** @defgroup Interrupt_Signal_Active_Selection, CTRL_REG3
  * @{
  */
#define IIS2DLPC_INTERRUPT_SIGNAL_LOW         ((uint8_t)0x08)
#define IIS2DLPC_INTERRUPT_SIGNAL_HIGH        ((uint8_t)0x00)
/**
  * @}
  */ 


/** @defgroup Boot_Mode_selection
  * @{
  */
#define IIS2DLPC_BOOT_NORMALMODE              ((uint8_t)0x00)
#define IIS2DLPC_BOOT_FORCED                  ((uint8_t)0x80)
/**
  * @}
  */   


/** @defgroup FIFO_Mode_selection 
  * @{
  */
#define IIS2DLPC_FIFO_BYPASS_MODE             ((uint8_t)0x00)
#define IIS2DLPC_FIFO_MODE                    ((uint8_t)0x20)
#define IIS2DLPC_FIFO_STREAM_MODE             ((uint8_t)0x40)
#define IIS2DLPC_FIFO_SF_TRIGGER_MODE         ((uint8_t)0x60)
#define IIS2DLPC_FIFO_BS_TRIGGER_MODE         ((uint8_t)0x80)
#define IIS2DLPC_FIFO_BF_TRIGGER_MODE         ((uint8_t)0xE0)
/**
  * @}
  */

/**
  * @}
  */ 
  
/** @defgroup IIS2DLPC_Exported_Macros
  * @{
  */

/**
  * @}
  */ 

/** @defgroup IIS2DLPC_Exported_Functions
  * @{
  */ 
void    IIS2DLPC_Init(uint16_t InitStruct);
void    IIS2DLPC_DeInit(void);
uint8_t IIS2DLPC_ReadID(void);
void    IIS2DLPC_InterruptConfig(IIS2DLPC_InterruptConfigTypeDef *IIS2DLPC_IntConfigStruct);
void    IIS2DLPC_Click_IntConfig(void);
void    IIS2DLPC_ODR_LowpowerCmd(uint8_t ODR_LowPowerMode);
void    IIS2DLPC_DataRateCmd(uint8_t DataRateValue);
void    IIS2DLPC_FullScaleCmd(uint8_t FS_value);
void    IIS2DLPC_RebootCmd(void);
void    IIS2DLPC_ReadACC(int16_t *pData);

/* Accelerometer IO functions */  
void    ACCELERO_IO_Init(void);
void    ACCELERO_IO_ITConfig(void);
void    ACCELERO_IO_Write(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite);
void    ACCELERO_IO_Read(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead);

/* Accelerometer driver structure */
extern ACCELERO_DrvTypeDef Iis2dlpcDrv;
#ifdef __cplusplus
}
#endif

#endif /* __IIS2DLPC_H */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
