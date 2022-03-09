/**
  ******************************************************************************
  * @file    BSP/Inc/anime.h
  * @author  MCD Application Team
  * @brief   This file contains graphics definitions for animation control.
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
#ifndef __ANIME_H
#define __ANIME_H

#define BPP_ARGB8888      0
#define BPP_RGB888        1
#define BPP_RGB565        2
#define BPP_ARGB1555      3
#define BPP_ARGB4444      4
#define BPP_L8            5
#define BPP_AL44          6
#define BPP_AL88          7
#define BPP_L4            8
#define BPP_A8            9
#define BPP_A4            10

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  uint32_t* address;     // Object address
  uint8_t  bpp;         // Object bpp (coded as per DMA2D)
  uint16_t size_x;      // X position
  uint16_t size_y;      // Y position
  uint32_t clut_address;// CLUT address
  uint32_t clut_size;   // CLUT size
  uint8_t  animation;   // Animation pointer incremented at each call

} anime_object_t;

#endif /* __ANIME_H */
