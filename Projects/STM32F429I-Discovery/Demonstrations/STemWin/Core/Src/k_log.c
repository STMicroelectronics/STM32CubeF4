/**
  ******************************************************************************
  * @file    k_log.c
  * @author  MCD Application Team
  * @brief   This file provides the kernel log functions    
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
