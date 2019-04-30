/**
  ******************************************************************************
  * @file    k_module.c
  * @author  MCD Application Team
  * @brief   This file provides the kernel module functions 
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
#include "k_module.h"   
#include "DIALOG.h"

/** @addtogroup CORE
  * @{
  */

/** @defgroup KERNEL_MODULE
  * @brief Kernel module routines
  * @{
  */

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
#define MAX_MODULES_NUM                                                       15
/* Private variables ---------------------------------------------------------*/
K_ModulePropertyTypedef    module_prop [MAX_MODULES_NUM];
K_ModuleOpenTypedef        open_link   [MAX_MODULES_NUM];
uint16_t module_num = 0;
uint16_t openlink_num = 0;
/* Private function prototypes -----------------------------------------------*/
static uint8_t k_ModuleVerify(K_ModuleItem_Typedef *module);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Module Initialization.
  * @param  None.
  * @retval None
  */
void k_ModuleInit(void) 
{
  module_num = 0;
  openlink_num = 0;
  memset(module_prop, 0, sizeof(module_prop));
  memset(open_link, 0, sizeof(module_prop));  
}

/**
  * @brief  Get the module number.
  * @param  None.
  * @retval None
  */
uint8_t k_ModuleGetNumber(void) 
{
  return module_num;
}
/**
  * @brief  Add module.
  * @param  module: pointer to data structure of type K_ModuleItem_Typedef
  * @retval module add status
  */
uint8_t k_ModuleAdd(K_ModuleItem_Typedef *module) 
{
  if((module_num < (MAX_MODULES_NUM - 1)) && (k_ModuleVerify(module) == 0))
  {
    module_prop[module_num].module = module;
    module_num++;
    return 0;
   }
  else
  {
    return 1;
  }
}

/**
  * @brief  Link module to file browser open menu
  * @param  module: pointer to data structure of type K_ModuleItem_Typedef
  * @param  ext:    pointer to file extension.  
  * @retval None
  */

uint8_t k_ModuleOpenLink(K_ModuleItem_Typedef *module, char *ext) 
{
  if(openlink_num < (MAX_MODULES_NUM - 1))
  {
    open_link[openlink_num].module = module;
    strcpy(open_link[openlink_num].ext, ext);
    openlink_num++;
    return 0;
   }
  else
  {
    return 1;
  }
}

/**
  * @brief  Link module to file browser open menu
  * @param  ext: file extension
  * @retval pointer to module
  */

K_GET_DIRECT_OPEN_FUNC *k_ModuleCheckLink(char *ext) 
{
  uint8_t i   = 0;
  
  for (i = 0; i < MAX_MODULES_NUM; i++)
  {
    if(strcmp(open_link[i].ext, ext) == 0)
    {
      return (K_GET_DIRECT_OPEN_FUNC *)(open_link[i].module->DirectOpen);
    }
  }
  return NULL;
}

/**
  * @brief  Verify module.
  * @param  module: pointer to data structure of type K_ModuleItem_Typedef
  * @retval module ID status
  */

static uint8_t k_ModuleVerify(K_ModuleItem_Typedef *module) 
{
  uint8_t i   = 0;

  for (i = 0; i < MAX_MODULES_NUM; i++)
  {
    if(module_prop[i].module != NULL)
    {
      if(module_prop[i].module->id == module->id)
      {
        return 1;
      }
    }
  }
  return 0;
}

/**
  * @brief  Remove module
  * @param  module: pointer to data structure of type K_ModuleItem_Typedef
  * @retval None
  */

void k_ModuleRemove(K_ModuleItem_Typedef *module) 
{
  uint8_t idx = 0;
  
  for (idx  = 0; idx < MAX_MODULES_NUM; idx ++)
  {
    if(module_prop[module_num].module->id == module->id)
    {
      module_prop[module_num].module = NULL;
      module_num--;
    }
  }
}
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
