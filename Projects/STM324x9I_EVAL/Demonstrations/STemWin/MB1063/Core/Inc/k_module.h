/**
  ******************************************************************************
  * @file    k_module.h
  * @author  MCD Application Team
  * @brief   Header for k_module.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __K_MODULE_H
#define __K_MODULE_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "WM.h"
#include "DIALOG.h"
/* Exported types ------------------------------------------------------------*/
typedef struct
{
 uint8_t     id;
 const char  *name;
 GUI_CONST_STORAGE GUI_BITMAP  *icon;
 void        (*startup) (WM_HWIN , uint16_t, uint16_t );
 void        (*DirectOpen) (char * ); 
}
K_ModuleItem_Typedef;

typedef struct
{
  uint8_t  idx;
  uint8_t  in_use;  
  const K_ModuleItem_Typedef   *module;
}
K_ModulePropertyTypedef;

typedef struct
{
  char   ext[4];
  const K_ModuleItem_Typedef   *module;
}
K_ModuleOpenTypedef;

typedef void K_GET_DIRECT_OPEN_FUNC(char *);

/* Structure for menu items */
typedef struct {
  char * sText;
  U16 Id;
  U16 Flags;
} MENU_ITEM;

extern K_ModulePropertyTypedef    module_prop[];
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void    k_InitMenu(void);
void    k_StartUp(void);
void    k_SetGuiProfile(void);
void    k_ModuleInit(void);
void    k_UpdateLog(char *Msg);
uint8_t k_ModuleAdd(K_ModuleItem_Typedef *module);
void    k_ModuleRemove(K_ModuleItem_Typedef *module); 
uint8_t k_ModuleGetNumber(void);
uint8_t k_ModuleOpenLink(K_ModuleItem_Typedef *module, char *ext);
K_GET_DIRECT_OPEN_FUNC *k_ModuleCheckLink(char *ext);

#ifdef __cplusplus
}
#endif

#endif /*__K_MODULE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
