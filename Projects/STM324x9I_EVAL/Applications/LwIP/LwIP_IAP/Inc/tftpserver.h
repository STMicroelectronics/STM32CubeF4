/**
  ******************************************************************************
  * @file    LwIP/LwIP_TFTP_Server/Inc/tftpserver.h 
  * @author  MCD Application Team
  * @brief   This file is the header of tftpserver file.
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

#ifndef __TFTPSERVER_H_
#define __TFTPSERVER_H_

#include "lwip/mem.h"
#include "lwip/udp.h"



#define TFTP_OPCODE_LEN         2
#define TFTP_BLKNUM_LEN         2
#define TFTP_DATA_LEN_MAX       512
#define TFTP_DATA_PKT_HDR_LEN   (TFTP_OPCODE_LEN + TFTP_BLKNUM_LEN)
#define TFTP_ERR_PKT_HDR_LEN    (TFTP_OPCODE_LEN + TFTP_ERRCODE_LEN)
#define TFTP_ACK_PKT_LEN        (TFTP_OPCODE_LEN + TFTP_BLKNUM_LEN)
#define TFTP_DATA_PKT_LEN_MAX   (TFTP_DATA_PKT_HDR_LEN + TFTP_DATA_LEN_MAX)
#define TFTP_MAX_RETRIES        3
#define TFTP_TIMEOUT_INTERVAL   5


typedef struct
{
  int op;    /*WRQ */
  /* last block read */
  char data[TFTP_DATA_PKT_LEN_MAX];
  int  data_len;
  /* destination ip:port */
  ip_addr_t to_ip;
  int to_port;
  /* next block number */
  int block;
  /* total number of bytes transferred */
  int tot_bytes;
  /* timer interrupt count when last packet was sent */
  /* this should be used to resend packets on timeout */
  unsigned long long last_time;
 
}tftp_connection_args;


/* TFTP opcodes as specified in RFC1350   */
typedef enum {
  TFTP_RRQ = 1,
  TFTP_WRQ = 2,
  TFTP_DATA = 3,
  TFTP_ACK = 4,
  TFTP_ERROR = 5
} tftp_opcode;


/* TFTP error codes as specified in RFC1350  */
typedef enum {
  TFTP_ERR_NOTDEFINED,
  TFTP_ERR_FILE_NOT_FOUND,
  TFTP_ERR_ACCESS_VIOLATION,
  TFTP_ERR_DISKFULL,
  TFTP_ERR_ILLEGALOP,
  TFTP_ERR_UKNOWN_TRANSFER_ID,
  TFTP_ERR_FILE_ALREADY_EXISTS,
  TFTP_ERR_NO_SUCH_USER,
} tftp_errorcode;


void IAP_tftpd_init(void);


#endif

