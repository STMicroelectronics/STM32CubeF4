/**
  ******************************************************************************
  * @file    k_log.c
  * @author  MCD Application Team
  * @brief   This file provides the kernel log functions    
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
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
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "k_log.h"
#include "k_module.h"  
#include "k_mem.h" 
#include  <stdio.h>

/** @addtogroup CORE
  * @{
  */

/** @defgroup KERNEL_LOG
  * @brief Kernel Log routines
  * @{
  */

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define LOG_DEPTH    (4 * 512)
/* Private macros ------------------------------------------------------------*/
#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/* Private variables ---------------------------------------------------------*/

uint8_t  *pLOG_CacheBuffer;  
uint32_t LOG_IN_ptr = 0;

/**
  * @brief  Init Kernel Log
  * @param  None
  * @retval None
  */
void k_LogInit(void)
{
  pLOG_CacheBuffer = (uint8_t *)k_malloc(LOG_DEPTH);
  memset (pLOG_CacheBuffer, 0, LOG_DEPTH);
  LOG_IN_ptr = 0; 
}

/**
  * @brief  Redirect printf
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
   uint32_t cnt = 0;
   
   if( LOG_IN_ptr++ >= LOG_DEPTH)
   {
     
    for (cnt = 1; cnt <  LOG_DEPTH; cnt ++)
    {
      pLOG_CacheBuffer[cnt -1 ] = pLOG_CacheBuffer[cnt];
    }
    LOG_IN_ptr = LOG_DEPTH;
   }
   pLOG_CacheBuffer [LOG_IN_ptr - 1] = ch;
   
   if(ch == '\n')
   {
     k_UpdateLog ((char *)pLOG_CacheBuffer);
   }
   
   return ch;
}
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
