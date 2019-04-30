/**
  *  Portions COPYRIGHT 2018 STMicroelectronics
  *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
  *
  ********************************************************************************
  * @file    threading_alt_template.h
  * @author  MCD Application Team
  * @brief   template header file to be included the threading_alt.c

  ********************************************************************************
  * @attention
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

/*
 * this files defines the mbedtls_threading_mutex_t data type used by the
 * mbedtls_mutex_xxx() API.
 * this file is included in the "threadind_alt_template.c", thus it should be
 * copied under the project Inc directory and renamed "thredading_alt.h"
 */

#ifndef MBEDTLS_THREADING_ALT_H
#define MBEDTLS_THREADING_ALT_H

#ifdef __cplusplus
extern "C" {
#endif
#include "mbedtls/platform.h"

#define MBEDTLS_ERR_THREADING_FEATURE_UNAVAILABLE         -0x001A  /**< The selected feature is not available. */
#define MBEDTLS_ERR_THREADING_BAD_INPUT_DATA              -0x001C  /**< Bad input parameters to function. */
#define MBEDTLS_ERR_THREADING_MUTEX_ERROR                 -0x001E  /**< Locking / unlocking / free failed with error code. */


#include "cmsis_os.h"
typedef struct {
#if (osCMSIS < 0x20000U)
     osMutexId mutex_id;
     osStatus status;
#else
     osMutexId_t mutex_id;
     osStatus_t status;
#endif

} mbedtls_threading_mutex_t;

void cmsis_os_mutex_init( mbedtls_threading_mutex_t *mutex );
void cmsis_os_mutex_free( mbedtls_threading_mutex_t *mutex );
int cmsis_os_mutex_lock( mbedtls_threading_mutex_t *mutex );
int cmsis_os_mutex_unlock( mbedtls_threading_mutex_t *mutex );


#ifdef __cplusplus
}
#endif

#endif /* MBEDTLS_THREADING_ALT_H */
