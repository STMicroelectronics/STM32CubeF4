/**
  ******************************************************************************
  * @file    BSP/Inc/anime.h
  * @author  MCD Application Team
  * @brief   This file contains graphics definitions for animation control.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
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

/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
