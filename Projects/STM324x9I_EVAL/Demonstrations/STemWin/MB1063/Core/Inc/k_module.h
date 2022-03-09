/**
  ******************************************************************************
  * @file    k_module.h
  * @author  MCD Application Team
  * @brief   Header for k_module.c file
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
