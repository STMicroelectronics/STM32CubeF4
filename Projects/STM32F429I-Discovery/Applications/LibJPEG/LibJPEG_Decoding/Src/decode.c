/**
  ******************************************************************************
  * @file    LibJPEG/LibJPEG_Decoding/Src/decode.c 
  * @author  MCD Application Team
  * @brief   This file contains the decompress method.
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
#include "decode.h"

/* Private typedef -----------------------------------------------------------*/
/* This struct contains the JPEG decompression parameters */
struct jpeg_decompress_struct cinfo;
/* This struct represents a JPEG error handler */
struct jpeg_error_mgr jerr;
  
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Decode
  * @param  callback:
  * @param  file:
  * @param  width:
  * @param  buff:    
  * @retval None
  */
void jpeg_decode(JFILE *file, uint32_t width, uint8_t * buff, uint8_t (*callback)(uint8_t*, uint32_t))
{ 
  /* Decode JPEG Image */
  JSAMPROW buffer[2] = {0}; /* Output row buffer */
  uint32_t row_stride = 0; /* Physical row width in image buffer */
  
  buffer[0] = buff;
  
  /* Step 1: Allocate and initialize JPEG decompression object */
  cinfo.err = jpeg_std_error(&jerr);
  
  /* Step 2: Initialize the JPEG decompression object */  
  jpeg_create_decompress(&cinfo);
  
  jpeg_stdio_src (&cinfo, file);
  
  /* Step 3: read image parameters with jpeg_read_header() */
  jpeg_read_header(&cinfo, TRUE);
  
  /* Step 4: set parameters for decompression */
  cinfo.dct_method = JDCT_FLOAT;
  
  /* Step 5: start decompressor */
  jpeg_start_decompress(&cinfo);
  
  row_stride = width * 3;
  while (cinfo.output_scanline < cinfo.output_height)
  {
    (void) jpeg_read_scanlines(&cinfo, buffer, 1);
    
    if (callback(buffer[0], row_stride) != 0)
    {
      break;
    }
  }
  
  /* Step 6: Finish decompression */
  jpeg_finish_decompress(&cinfo);
  
  /* Step 7: Release JPEG decompression object */
  jpeg_destroy_decompress(&cinfo);   
}
