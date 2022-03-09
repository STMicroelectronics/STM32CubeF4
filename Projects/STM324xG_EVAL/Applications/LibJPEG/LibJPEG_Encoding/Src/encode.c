/**
  ******************************************************************************
  * @file    LibJPEG/LibJPEG_Encoding/Src/encode.c 
  * @author  MCD Application Team
  * @brief   This file contain the compress method.
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
#include "encode.h"

/* Private typedef -----------------------------------------------------------*/
   /* This struct contains the JPEG compression parameters */
   static struct jpeg_compress_struct cinfo_; 
   /* This struct represents a JPEG error handler */
   struct jpeg_error_mgr jerr_; 
  
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Jpeg encode 
  * @param  file:          pointer to the bmp file
  * @param  file1:         pointer to the jpg file  
  * @param  width:         image width
  * @param  height:        image height
  * @param  image_quality: image quality
  * @param  buff:          pointer to the image line   
  * @retval None
  */
void jpeg_encode(JFILE *file, JFILE *file1, uint32_t width, uint32_t height, uint32_t image_quality, uint8_t * buff)
{ 
    
  /* Encode BMP Image to JPEG */ 
  /* Pointer to a single row */
  JSAMPROW row_pointer;    
  uint32_t bytesread;
  uint32_t index;
  
  /* Step 1: allocate and initialize JPEG compression object */
  /* Set up the error handler */
  cinfo_.err = jpeg_std_error(&jerr_);
  
  /* Initialize the JPEG compression object */  
  jpeg_create_compress(&cinfo_);
  
  /* Step 2: specify data destination */
  jpeg_stdio_dest(&cinfo_, file1);
  
  /* Step 3: set parameters for compression */
  cinfo_.image_width = width;
  cinfo_.image_height = height;
  cinfo_.input_components = 3;
  cinfo_.in_color_space = JCS_RGB;
  
  /* Set default compression parameters */
  jpeg_set_defaults(&cinfo_);
  
  cinfo_.dct_method  = JDCT_FLOAT;    
  
  jpeg_set_quality(&cinfo_, image_quality, TRUE);
  
  /* Step 4: start compressor */
  jpeg_start_compress(&cinfo_, TRUE);
  
  /* Get bitmap data address offset */
  f_read(file, buff, 14, (UINT*)&bytesread);
  index = *(__IO uint16_t *) (buff + 10);
  index |= (*(__IO uint16_t *) (buff + 12)) << 16;

  while (cinfo_.next_scanline < cinfo_.image_height)
  {
    /* In this application, the input file is a BMP, which first encodes the bottom of the picture */
    /* JPEG encodes the highest part of the picture first. We need to read the lines upside down   */
    /* Move the read pointer to 'last line of the picture - next_scanline'    */
    f_lseek(file, ((cinfo_.image_height-1-cinfo_.next_scanline)*width*3)+index);
    if(f_read(file, buff, width*3, (UINT*)&bytesread) == FR_OK)
    {
      row_pointer = (JSAMPROW)buff;
      jpeg_write_scanlines(&cinfo_, &row_pointer, 1);        
    }
  }
  
  /* Step 5: finish compression */
  jpeg_finish_compress(&cinfo_);
  
  /* Step 6: release JPEG compression object */
  jpeg_destroy_compress(&cinfo_);
    
}
