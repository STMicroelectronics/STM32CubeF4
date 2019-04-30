/**
  *  Portions COPYRIGHT 2018 STMicroelectronics
  *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
  *
  ********************************************************************************
  * @file    threading_alt_template.c
  * @author  MCD Application Team
  * @brief   mutex management functions iplementation based on cmsis-os V1/V2
             API. This file to be copied under the application project source tree.
  ********************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2018 STMicroelectronics</center></h2>
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

/*
 * This files implments the mbedTLS mutex management API using the CMSIS-RTOS
 * V1 & V2 API. To correctly use this file make sure that:
 *
 *  - this file as well as the "threading_alt_template.h" files are renamed
 *     and copied under the project source tree.
 *
 *  - The project seetings are pointing to the correct CMSIS-RTOS files
 *
 *  - the mbedTLS config file is enabling the flags "MBEDTLS_THREADING_C" and
 *    "MBEDTLS_THREADING_ALT"
 *
 *  - the mbedtls_set_threading_alt(...) is called before any other mbedtls
 *   API, in order to register the cmisis_os_mutex_xxx() API.
 *
 */

#include "threading_alt.h"

#if defined(MBEDTLS_THREADING_ALT)

void cmsis_os_mutex_init( mbedtls_threading_mutex_t *mutex )
{
#if (osCMSIS < 0x20000U)
  osMutexDef(thread_mutex);
  mutex->mutex_id = osMutexCreate(osMutex(thread_mutex));
#else
  mutex->mutex_id = osMutexNew(NULL);
#endif
  if (mutex->mutex_id != NULL)
  {
    mutex->status = osOK;
  }
  else
  {
    mutex->status = osErrorOS;
  }

}

void cmsis_os_mutex_free( mbedtls_threading_mutex_t *mutex )
{
  if (mutex->mutex_id != NULL)
  {
    osMutexDelete(mutex->mutex_id);
  }
}

int cmsis_os_mutex_lock( mbedtls_threading_mutex_t *mutex )
{
  if ((mutex == NULL) || (mutex->mutex_id == NULL) || (mutex->status != osOK))
  {
    return MBEDTLS_ERR_THREADING_BAD_INPUT_DATA;
  }
#if (osCMSIS < 0x20000U)
  mutex->status = osMutexWait(mutex->mutex_id, osWaitForever);
#else
  mutex->status = osMutexAcquire(mutex->mutex_id, osWaitForever);
#endif
  if (mutex->status != osOK)
  {
    return MBEDTLS_ERR_THREADING_MUTEX_ERROR;
  }

  return 0;
}

int cmsis_os_mutex_unlock( mbedtls_threading_mutex_t *mutex )
{
   if((mutex == NULL) || (mutex->mutex_id == NULL) || (mutex->status != osOK))
   {
     return MBEDTLS_ERR_THREADING_BAD_INPUT_DATA;
   }

   mutex->status = osMutexRelease(mutex->mutex_id);
   if (mutex->status != osOK)
   {
     return MBEDTLS_ERR_THREADING_MUTEX_ERROR;
   }

   return 0;
}
#endif
