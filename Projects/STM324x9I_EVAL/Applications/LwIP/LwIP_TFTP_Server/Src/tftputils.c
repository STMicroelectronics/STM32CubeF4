/**
  ******************************************************************************
  * @file    LwIP/LwIP_TFTP_Server/Src/tftputils.c
  * @author  MCD Application Team
  * @brief   tftp utils source file.
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

#include <string.h>
#include "tftputils.h"

/**
  * @brief  Extracts the opcode from a TFTP message
**/ 
tftp_opcode tftp_decode_op(char *buf)
{
  return (tftp_opcode)(buf[1]);
}

/**
  * @brief Extracts the block number from TFTP message
**/
u16_t tftp_extract_block(char *buf)
{
  u16_t *b = (u16_t*)buf;
  return ntohs(b[1]);
}

/**
  * @brief Extracts the filename from TFTP message
**/ 
void tftp_extract_filename(char *fname, char *buf)
{
  strcpy(fname, buf + 2);
}

/**
  * @brief set the opcode in TFTP message: RRQ / WRQ / DATA / ACK / ERROR 
**/ 
void tftp_set_opcode(char *buffer, tftp_opcode opcode)
{

  buffer[0] = 0;
  buffer[1] = (u8_t)opcode;
}

/**
  * @brief Set the errorcode in TFTP message
**/
void tftp_set_errorcode(char *buffer, tftp_errorcode errCode)
{

  buffer[2] = 0;
  buffer[3] = (u8_t)errCode;
}

/**
  * @brief Sets the error message
**/
void tftp_set_errormsg(char * buffer, char* errormsg)
{
  strcpy(buffer + 4, errormsg);
}

/**
  * @brief Sets the block number
**/
void tftp_set_block(char* packet, u16_t block)
{

  u16_t *p = (u16_t *)packet;
  p[1] = htons(block);
}

/**
  * @brief Set the data message
**/
void tftp_set_data_message(char* packet, char* buf, int buflen)
{
  memcpy(packet + 4, buf, buflen);
}

/**
  * @brief Check if the received acknowledgement is correct
**/
u32_t tftp_is_correct_ack(char *buf, int block)
{
  /* first make sure this is a data ACK packet */
  if (tftp_decode_op(buf) != TFTP_ACK)
    return 0;

  /* then compare block numbers */
  if (block != tftp_extract_block(buf))
    return 0;

  return 1;
}

