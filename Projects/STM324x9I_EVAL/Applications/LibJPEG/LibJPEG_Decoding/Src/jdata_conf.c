/**
  ******************************************************************************
  * @file    LibJPEG/LibJPEG_Decoding/Src/jdata_conf.c 
  * @author  MCD Application Team
  * @brief   
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

/* Includes ------------------------------------------------------------------*/
#include "jdata_conf.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*
 * The modules that use fread() and fwrite() always invoke them through
 * these macros.  On some systems you may need to twiddle the argument casts.
 * CAUTION: argument order is different from underlying functions!
 */
size_t read_file (JFILE  *file, uint8_t *buf, uint32_t sizeofbuf)
{
  static size_t BytesReadfile ;  
  f_read (file, buf , sizeofbuf, &BytesReadfile); 
  return BytesReadfile; 
}

size_t write_file (JFILE  *file, uint8_t *buf, uint32_t sizeofbuf)
{
  static size_t BytesWritefile ;  
  f_write (file, buf , sizeofbuf, &BytesWritefile); 
  return BytesWritefile; 
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
