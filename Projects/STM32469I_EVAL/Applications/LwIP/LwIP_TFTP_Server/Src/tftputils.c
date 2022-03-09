/**
  ******************************************************************************
  * @file    LwIP/LwIP_TFTP_Server/Src/tftputils.c
  * @author  MCD Application Team
  * @brief   tftp utils source file.
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
