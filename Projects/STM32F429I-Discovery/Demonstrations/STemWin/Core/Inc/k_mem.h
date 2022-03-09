/**
  ******************************************************************************
  * @file    k_mem.h
  * @author  MCD Application Team
  * @brief   Header for k_mem.c file
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _K_MEM_H
#define _K_MEM_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
   
/* Exported types ------------------------------------------------------------*/
#define SIZE_OF_PAGE    1024   /* 1 KBytes pages */
#define MAX_PAGE_NUMBER   64    /* Maximum of 64 pages */


typedef struct
{
  uint32_t mallocBase;                    /* Memory pool base address */
  uint32_t size[MAX_PAGE_NUMBER];         /* Sizes Table to allow safe deallocation */
  uint8_t PageTable[MAX_PAGE_NUMBER];     /* Memory page state table '1'-> Allocated '0' -> Free */
}
mem_TypeDef;
/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void k_MemInit(void);
void * k_malloc(size_t s);
void k_free(void * p);

#ifdef __cplusplus
}
#endif

#endif /* _K_MEM_H */
