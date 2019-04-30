/**
  ******************************************************************************
  * @file    LibJPEG/LibJPEG_Decoding/Src/decode.c 
  * @author  MCD Application Team
  * @brief   This file contain the decompress method.
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
  * @param  callback: line decoding callback
  * @param  file1:    pointer to the jpg file
  * @param  width:    image width
  * @param  buff:     pointer to the image line    
  * @retval None
  */
void jpeg_decode(JFILE *file, uint32_t width, uint8_t * buff, uint8_t (*callback)(uint8_t*, uint32_t))
{ 
    
  /* Decode JPEG Image */
  JSAMPROW buffer[2] = {0}; /* Output row buffer */
  uint32_t row_stride = 0; /* physical row width in image buffer */
  
  buffer[0] = buff;
	
  /* Step 1: allocate and initialize JPEG decompression object */
  cinfo.err = jpeg_std_error(&jerr);

  /* Initialize the JPEG decompression object */  
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
