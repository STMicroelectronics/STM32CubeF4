
/**
  *  Portions COPYRIGHT 2018 STMicroelectronics
  *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
  *
  ******************************************************************************
  * @file    rng_alt_template.c
  * @author  MCD Application Team
  * @brief   mbedtls alternate entropy data function.
  *          the mbedtls_hardware_poll() is customized to use the STM32 RNG
  *          to generate random data, required for TLS encryption algorithms.
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2018 STMicroelectronics</center></h2>
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

#include "mbedtls/entropy_poll.h"
#include "mbedtls/platform.h"

#ifdef MBEDTLS_ENTROPY_HARDWARE_ALT
/*
 * include the correct headerfile depending on the STM32 family */

#include "stm32XXXXX_hal.h"
#include <string.h>

__IO uint32_t isInitialized = 0;

static RNG_HandleTypeDef RNG_Handle;


static void RNG_Init(void);
/* RNG init function */
static void RNG_Init(void)
{
  if (isInitialized == 0)
  {
      RNG_Handle.Instance = RNG;
      /* DeInitialize the RNG peripheral */
      if (HAL_RNG_DeInit(&RNG_Handle) != HAL_OK)
      {
        return;
      }

      /* Initialize the RNG peripheral */
      if (HAL_RNG_Init(&RNG_Handle) != HAL_OK)
      {
        return;
      }
      isInitialized = 1;
  }
}



int mbedtls_hardware_poll( void *Data, unsigned char *Output, size_t Len, size_t *oLen )
{
  uint32_t index;
  uint32_t random_value;
  int ret;

  RNG_Init();

  if (isInitialized == 0)
  {
    ret = -1;
  }
  else
  {
      for (index = 0; index < Len/4; index++)
      {
        if (HAL_RNG_GenerateRandomNumber(&RNG_Handle, &random_value) == HAL_OK)
        {
          *oLen += 4;
          memset(&(Output[index * 4]), (int)random_value, 4);
        }
      }
      ret = 0;
  }

  return ret;
}

#if 0

/*
 * HAL_RNG_MspInit() and HAL_RNG_MspDeInit() are put here as reference
 */


/**
  * @brief RNG MSP Initialization
  *   This function configures the hardware resources used in this application:
  *   - Peripheral's clock enable
  * @param hrng: RNG handle pointer
  * @retval None
  */

void HAL_RNG_MspInit(RNG_HandleTypeDef *hrng)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

  /*Select PLL output as RNG clock source */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RNG;
  PeriphClkInitStruct.RngClockSelection = RCC_RNGCLKSOURCE_PLL;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

  /* RNG Peripheral clock enable */
  __RNG_CLK_ENABLE();

}
/**
  * @brief RNG MSP De-Initialization
  *        This function freeze the hardware resources used in this application:
  *          - Disable the Peripheral's clock
  * @param hrng: RNG handle pointer
  * @retval None
  */
void HAL_RNG_MspDeInit(RNG_HandleTypeDef *hrng)
{
  /* Enable RNG reset state */
  __HAL_RCC_RNG_FORCE_RESET();

  /* Release RNG from reset state */
  __HAL_RCC_RNG_RELEASE_RESET();
}
#endif

#endif /*MBEDTLS_ENTROPY_HARDWARE_ALT*/
