
@verbatim
******************************************************************************
* @file    st_readme.txt
* @author  MCD Application Team
* @brief   This file lists the main modification done by STMicroelectronics on
*          CMSIS_5 for integration with STM32Cube solution.
******************************************************************************
* Copyright 2024-2025 STMicroelectronics
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*   http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
* ******************************************************************************
@endverbatim

### 20-May-2025 ###
=======================
  Tag: v5.9.0_fix_620_dsp_v1.10.0_nn_v3.1.0_no_doc
  
  Remove Documentation/ subfolders and update index.html to link to
  online documentation https://arm-software.github.io/CMSIS_5/latest/General/html/index.html
  
### 14-January-2025 ###
=======================
  Tag: v5.9.0_fix_620_dsp_v1.10.0_nn_v3.1.0
  
  Update Core/Include/cachel1_armv7.h
  
    fix(cmsis): Integrate SCB_DisableDCache() issue 620 fixes from CMSIS_5 github

    Merge two following commits from https://github.com/ARM-software/CMSIS_5

    SHA-1: 36bd54f7963825954b3dd37c036dbbcd1494988f

    * Fix the endless loop issue with GCC O0.

    More details, see https://github.com/ARM-software/CMSIS_5/issues/620
    The issue only happens when local variables are in stack (GCC O0). If local variables are saved
    in general purpose register, then the function is OK.
    When local variables are in stack, after disabling the cache, flush the local variables cache
    line for data consistency.

    SHA-1: ae2a29fc077f2d62ad9c6793e19eea604bf76843

    * Core(M): Fix endless loop issue with non-optimized IAR builds

    This is an IAR fix for the problem described in
    https://github.com/ARM-software/CMSIS_5/issues/620

    IAR builds can not align the stack to the cache line size and
    thus the invalidation is done in separate steps for the three
    variables.

    Fix validated on STM32H7 HW.

### 14-October-2024 ###
=======================
  Tag: v5.9.0_dsp_v1.10.0_nn_v3.1.0
  
  CMSIS v5.9.0, please refer to "index.html" available under \Documentation folder.
  
  CMSIS v5.9.0, CMSIS-DSP v1.1.0, CMSIS-NN v3.1.0

  Note: content of \CMSIS\Core\Include has been copied under \Include to keep the same structure
       used in existing projects, and thus avoid projects mass update
  Note: the following components has been removed from ARM original delivery (not used in ST packages)
      - .gitattributes
      - .gitignore
      - \Device
      - CMSIS\DAP
         - \Firmware\Examples
         - \Firmware\Template
         - \Firmware\Validation
      - CMSIS\Driver
      - CMSIS\NN\Tests
      - CMSIS\RTOS\RTX
      - CMSIS\RTOS\Template
      - CMSIS\RTOS2\RTX
      - CMSIS\Utilities

