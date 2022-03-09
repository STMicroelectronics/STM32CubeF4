/**
  ******************************************************************************
  * @file    k_mem.c
  * @author  MCD Application Team
  * @brief   Utilities for file handling
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
#include "k_mem.h"

/** @addtogroup CORE
  * @{
  */

/** @defgroup KERNEL_MEMORY
  * @brief Kernel memory routines
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MEM_BASE                     0x10000000
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#if   (defined ( __CC_ARM ))
mem_TypeDef memory_pool __attribute__((at(MEM_BASE)));
#elif (defined (__ICCARM__))
#pragma location = MEM_BASE
__no_init mem_TypeDef memory_pool;
#elif defined   (  __GNUC__  )
mem_TypeDef memory_pool __attribute__((section(".ExtRAMData"))); 
#endif

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  This function Initialize a memory pool for private allocator.
  * @param  None
  * @retval None
  */
void k_MemInit(void)
{
  memset(&memory_pool, 0, sizeof(mem_TypeDef));
  memory_pool.mallocBase = MEM_BASE + sizeof(mem_TypeDef);
}

/**
  * @brief  This function implement a simple memory management algorithm (First fit).
  * @param  size : Requested memory size.
  * @retval Pointer to the allocated region.
  */
void * k_malloc(size_t size)
{
  __IO uint8_t index = 0, counter = 0, start = 0;
  uint8_t PageCount = 0, NewStart = 0;
  if (size > 0)
  {
    /* look for the first contiguous memory that meet requested size. */
    while ( index < (MAX_PAGE_NUMBER))
    {
      if (memory_pool.PageTable[index++] == 0)
      {
        counter++;
        if (size <= (counter * SIZE_OF_PAGE))
        {
          PageCount = counter - 1;
          NewStart = start;
          /* Set memory region state to allocated */
          for (index = 0; index <= PageCount;index++)
          {
            memory_pool.PageTable[NewStart + index] = 1;
          }
          /* Save size */
          memory_pool.size[NewStart] = counter;
          /* return pointer to allocated region. */
          return (void *)((memory_pool.mallocBase + (start  << 10)) + 1 * sizeof(uint32_t));
        }
      }
      else
      {
        start = index;
        counter = 0;
      }
    }
  }
  return NULL;
}

/**
  * @brief  Free previously allocated memory
  * @param  Pointer to the allocated region.
  * @retval None
  */
void k_free(void * p)
{
  __IO uint8_t index = 0;
  uint8_t counter = 0, start = 0;
  /* Handle NULL pointers */
  if (p == NULL)
    return;

  /* Find start Index */
  start = ((((uint32_t)p - sizeof(uint32_t)) - memory_pool.mallocBase) >> 10);
  /* Retrieve segment size */
  counter = memory_pool.size[start];
  /* Set size to zero */
  memory_pool.size[start] = 0;
  /* Deallocate memory region */
  for (index = 0; index < counter; index++)
  {
    memory_pool.PageTable[start + index] = 0;
  }

  return;
}

/**
  * @}
  */

/**
  * @}
  */
