/**
  ******************************************************************************
  * @file    k_mem.h
  * @author  MCD Application Team
  * @brief   Header for k_mem.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
