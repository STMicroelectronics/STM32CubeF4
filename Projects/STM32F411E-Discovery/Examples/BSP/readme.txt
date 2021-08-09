/**
  @page BSP  Example on how to use the BSP drivers

  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    BSP/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the BSP example.
  ******************************************************************************
  *
  * Copyright (c) 2017 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                       opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
   @endverbatim

@par Example Description

This example provides a description of how to use the different BSP drivers.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals.
Then the SystemClock_Config() function is used to configure the systemclock (SYSCLK)
to run at 100 MHz.

This example shows how to use the different functionalities listed below by
switching between all tests using USER button.
- For Rev.A boards:
  - Accelerometer LSM303DLHC
  - Gyroscope L3GD20
  - Audio device CS43L22
  - ST MEMS microphone MP45DT02TR-M
- For Rev.C boards:
  - Accelerometer LSM303AGR
  - Gyroscope I3G4250D
  - Audio device CS43L22
  - ST MEMS microphone IMP34DT05TR

First, push the User button to start first Test.
4 LEDs will blink between each test.Press user key to start another test:

     1) ACCELEROMETER_MEMS_Test.    Device: "LSM303DLHC" / "LSM303AGR"
        LEDs 3,4,5 and 6 show board orientation (X and Y axes).

     2) GYROSCOPE_MEMS_Test.        Device: "L3GD20" / "I3G4250D"
        LEDs 3,4,5 and 6 show board movement (X and Y axes).

     3) AudioPlay_Test(Need headphone). Device: "CS43L22"
        Plug a headphone to hear a 48K sound  /!\ Take care of yours ears.
        Default volume is 50%.
        A click on the board will pause the audio file play (LED6 & LED4 ON).
		    Another click resumes audio file play (only LED6 on)
        @Note: Copy file "/Utilities/Media/Audio/art_of_gard_128K.bin" directly
        in the STM32 flash at @0x08020000

     4) AudioRecord_Test. Device: "MP45DT02TR-M" / "IMP34DT05TR"
        Record your voice (2 or 3 secs)
        When LED6 turns on, Plug a headphone to hear a 16K sound  /!\ Take care of yours ears.
        Default volume is 70%.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

BSP, LCD, Touchscreen, PSRAM, DMA, QSPI, Erase, Read, Write, Audio play, Record, MEMS, Audio Codec, DFSDM,
I2S, Microphone, Headphones

@par Directory contents

  - BSP/Src/system_stm32f4xx.c      STM32F4xx system clock configuration file
  - BSP/Src/main.c                  Main program
  - BSP/Src/mems.c                  Mems example and configuration file
  - BSP/Src/audio_play.c            Audio play file
  - BSP/Src/audio_record.c          Audio record file
  - BSP/Src/stm32f4xx_it.c          Interrupt handlers
  - BSP/Inc/main.h                  Main program header file
  - BSP/Inc/mems.h                  Mems example and configuration header file
  - BSP/Inc/audio_play.h            Audio play header file
  - BSP/Inc/audio_record.h          Audio record header file
  - BSP/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - BSP/Inc/stm32f4xx_it.h          Interrupt handlers header file


@par Hardware and Software environment

  - This example runs on and STM32F411xE devices.

  - This example has been tested with STMicroelectronics STM32F411E-Discovery
    board and can be easily tailored to any other supported device and development board.

  - Use STLink utility, available on www.st.com or any other in system programming
    tool to load "/Utilities/Media/Audio/art_of_gard_448K.bin" file to the STM32
    internal flash at the address 0x08020000.


@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
