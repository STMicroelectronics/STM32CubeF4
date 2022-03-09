/**
  ******************************************************************************
  * @file    LwIP/LwIP_TFTP_Server/Inc/tftputils.h
  * @author  MCD Application Team
  * @brief   tftp utils header file.
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

#ifndef __TFTP_UTILS_H_
#define __TFTP_UTILS_H_

#include "tftpserver.h"

tftp_opcode tftp_decode_op(char *buf);
void tftp_extract_filename(char *fname, char *buf);
u16_t tftp_extract_block(char *buf);
void tftp_set_opcode(char *buffer, tftp_opcode opcode);
void tftp_set_errorcode(char *buffer, tftp_errorcode errCode);
void tftp_set_errormsg(char * buffer, char* errormsg);
u32_t tftp_is_correct_ack(char *buf, int block);
void tftp_set_data_message(char* packet, char* buf, int buflen);
void tftp_set_block(char* packet, u16_t block);

#endif
