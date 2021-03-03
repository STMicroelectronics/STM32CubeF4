/**
  ******************************************************************************
  * @file    LibJPEG/LibJPEG_Decoding/Inc/jdata_conf.h 
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
#include "ff.h"
#include "stm32f4xx_hal.h"

/* Private typedef -----------------------------------------------------------*/  
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#define JFILE            FIL

#define JMALLOC   malloc    
#define JFREE     free  

size_t read_file (JFILE  *file, uint8_t *buf, uint32_t sizeofbuf);
size_t write_file (JFILE  *file, uint8_t *buf, uint32_t sizeofbuf) ;

#define JFREAD(file,buf,sizeofbuf)  \
   read_file (file,buf,sizeofbuf)

#define JFWRITE(file,buf,sizeofbuf)  \
   write_file (file,buf,sizeofbuf)

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
