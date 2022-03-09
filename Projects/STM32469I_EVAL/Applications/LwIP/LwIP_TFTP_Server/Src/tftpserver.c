/**
  ******************************************************************************
  * @file    LwIP/LwIP_TFTP_Server/Src/tftpserver.c
  * @author  MCD Application Team
  * @brief   basic tftp server implementation for IAP (only Write Req supported)
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

#include "tftpserver.h"
#include "tftputils.h" 
#include "ff.h"
#include "stm32f4xx_hal.h"
#include <string.h>


typedef struct
{
  int op;    /* RRQ/WRQ */

  /* last block read */
  char data[TFTP_DATA_PKT_LEN_MAX];
  uint32_t  data_len;

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

FATFS filesystem;
FIL file_SD, file_CR;
DIR dir_1, dir_2;

/* UDPpcb to be binded with port 69  */
struct udp_pcb *UDPpcb;

/* tftp_errorcode error strings */
char *tftp_errorcode_string[] = {
                                  "not defined",
                                  "file not found",
                                  "access violation",
                                  "disk full",
                                  "illegal operation",
                                  "unknown transfer id",
                                  "file already exists",
                                  "no such user",
                                };

void recv_callback_tftp(void *arg, struct udp_pcb *upcb, struct pbuf *pkt_buf, const ip_addr_t *addr, u16_t port);


/**
  * @brief  sends a TFTP message
  * @param  upcb: pointer on a udp pcb
  * @param  to_ip: pointer on remote IP address
  * @param  to_port: pointer on remote port  
  * @param buf: pointer on buffer where to create the message  
  * @param err: error code of type tftp_errorcode
  * @retval error code
  */
err_t tftp_send_message(struct udp_pcb *upcb, const ip_addr_t *to_ip, unsigned short to_port, char *buf, unsigned short buflen)
{
  err_t err;
  struct pbuf *pkt_buf; /* Chain of pbuf's to be sent */

  /* PBUF_TRANSPORT - specifies the transport layer */
  pkt_buf = pbuf_alloc(PBUF_TRANSPORT, buflen, PBUF_POOL);

  if (!pkt_buf)      /*if the packet pbuf == NULL exit and end transmission */
    return ERR_MEM;

  /* Copy the original data buffer over to the packet buffer's payload */
  memcpy(pkt_buf->payload, buf, buflen);

  /* Sending packet by UDP protocol */
  err = udp_sendto(upcb, pkt_buf, to_ip, to_port);

  /* free the buffer pbuf */
  pbuf_free(pkt_buf);

  return err;
}


/**
  * @brief construct an error message into buf
  * @param buf: pointer on buffer where to create the message  
  * @param err: error code of type tftp_errorcode
  * @retval 
  */
int tftp_construct_error_message(char *buf, tftp_errorcode err)
{
  int errorlen;
  /* Set the opcode in the 2 first bytes */
  tftp_set_opcode(buf, TFTP_ERROR);
  /* Set the errorcode in the 2 second bytes  */
  tftp_set_errorcode(buf, err);
  /* Set the error message in the last bytes */
  tftp_set_errormsg(buf, tftp_errorcode_string[err]);
  /* Set the length of the error message  */
  errorlen = strlen(tftp_errorcode_string[err]);

  /* return message size */
  return 4 + errorlen + 1;
}


/**
  * @brief Sends a TFTP error message
  * @param  upcb: pointer on a udp pcb
  * @param  to: pointer on remote IP address
  * @param  to_port: pointer on remote port  
  * @param  err: tftp error code
  * @retval error value
  */
int tftp_send_error_message(struct udp_pcb *upcb, const ip_addr_t *to, int to_port, tftp_errorcode err)
{
  char buf[512];
  int error_len;

  /* construct error */
  error_len = tftp_construct_error_message(buf, err);
  /* send error message */
  return tftp_send_message(upcb, to, to_port, buf, error_len);
}


/**
  * @brief  Sends TFTP data packet
  * @param  upcb: pointer on a udp pcb
  * @param  to: pointer on remote IP address
  * @param  to_port: pointer on remote udp port
  * @param  block: block number
  * @param  buf: pointer on data buffer
  * @param  buflen: buffer length
  * @retval error value
  */
int tftp_send_data_packet(struct udp_pcb *upcb, const ip_addr_t *to, int to_port, unsigned short block,
                          char *buf, int buflen)
{
  char packet[TFTP_DATA_PKT_LEN_MAX]; /* (512+4) bytes */

  /* Set the opcode 3 in the 2 first bytes */
  tftp_set_opcode(packet, TFTP_DATA);
  /* Set the block numero in the 2 second bytes */
  tftp_set_block(packet, block);
  /* Set the data message in the n last bytes */
  tftp_set_data_message(packet, buf, buflen);
  /* Send DATA packet */
  return tftp_send_message(upcb, to, to_port, packet, buflen + 4);
}

/**
  * @brief  Sends TFTP ACK packet
  * @param  upcb: pointer on a udp pcb
  * @param  to: pointer on remote IP address
  * @param  to_port: pointer on remote udp port
  * @param  block: block number
  * @retval error value
  */
int tftp_send_ack_packet(struct udp_pcb *upcb, const ip_addr_t *to, int to_port, unsigned short block)
{

  /* create the maximum possible size packet that a TFTP ACK packet can be */
  char packet[TFTP_ACK_PKT_LEN];

  /* define the first two bytes of the packet */
  tftp_set_opcode(packet, TFTP_ACK);

  /* Specify the block number being ACK'd.
   * If we are ACK'ing a DATA pkt then the block number echoes that of the DATA pkt being ACK'd (duh)
   * If we are ACK'ing a WRQ pkt then the block number is always 0
   * RRQ packets are never sent ACK pkts by the server, instead the server sends DATA pkts to the
   * host which are, obviously, used as the "acknowledgement" */
  tftp_set_block(packet, block);

  return tftp_send_message(upcb, to, to_port, packet, TFTP_ACK_PKT_LEN);
}


/**
  * @brief Cleanup after end of TFTP read operation
  * @param upcb: pointer on udp pcb
  * @param  args: pointer on a structure of type tftp_connection_args
  * @retval None
  */
void tftp_cleanup_rd(struct udp_pcb *upcb, tftp_connection_args *args)
{
  /* close the filesystem */
  f_close(&file_SD);
  f_mount(NULL, (TCHAR const*)"",0);
  /* Free the tftp_connection_args structure reserved for */
  mem_free(args);

  /* Disconnect the udp_pcb*/  
  udp_disconnect(upcb);

  /* close the connection */
  udp_remove(upcb);

  udp_recv(UDPpcb, recv_callback_tftp, NULL);
}

/**
  * @brief Cleanup after end of TFTP write operation
  * @param upcb: pointer on udp pcb
  * @param  args: pointer on a structure of type tftp_connection_args
  * @retval None
  */
void tftp_cleanup_wr(struct udp_pcb *upcb, tftp_connection_args *args)
{
  /* close the filesystem */
  f_close(&file_CR);
  f_mount(NULL, (TCHAR const*)"",0);
  /* Free the tftp_connection_args structure reserved for */
  mem_free(args);

  /* Disconnect the udp_pcb*/
  udp_disconnect(upcb);

  /* close the connection */
  udp_remove(upcb);

  /* reset the callback function */
  udp_recv(UDPpcb, recv_callback_tftp, NULL);
}

/**
  * @brief  sends next data block during TFTP READ operation
  * @param  upcb: pointer on a udp pcb
  * @param  args: pointer on structure of type tftp_connection_args
  * @param  to_ip: pointer on remote IP address
  * @param  to_port: pointer on remote udp port
  * @retval None
  */
void tftp_send_next_block(struct udp_pcb *upcb, tftp_connection_args *args,
                          const ip_addr_t *to_ip, u16_t to_port)
{
  /* Function to read 512 bytes from the file to send (file_SD), put them
   * in "args->data" and return the number of bytes read */
   f_read(&file_SD, (uint8_t*)args->data, TFTP_DATA_LEN_MAX, (UINT*)(&args->data_len));

  /*   NOTE: We need to send data packet even if args->data_len = 0*/
 
  /* sEndTransferthe data */
  tftp_send_data_packet(upcb, to_ip, to_port, args->block, args->data, args->data_len);

}

/**
  * @brief  receive callback during tftp read operation (not on standard port 69)
  * @param  arg: pointer on argument passed to callback
  * @param  udp_pcb: pointer on the udp pcb
  * @param  pkt_buf: pointer on the received pbuf
  * @param  addr: pointer on remote IP address
  * @param  port: pointer on remote udp port
  * @retval None
  */
void rrq_recv_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p,
                       const ip_addr_t *addr, u16_t port)
{
  /* Get our connection state  */
  tftp_connection_args *args = (tftp_connection_args *)arg;

  if (tftp_is_correct_ack(p->payload, args->block))
  {
    /* increment block # */
    args->block++;
  }
  else
  {
    /* we did not receive the expected ACK, so
       do not update block #. This causes the current block to be resent. */
  }

  /* if the last read returned less than the requested number of bytes
   * (i.e. TFTP_DATA_LEN_MAX), then we've sent the whole file and we can quit
   */
  if (args->data_len < TFTP_DATA_LEN_MAX)
  {
    /* Clean the connection*/
    tftp_cleanup_rd(upcb, args);

    pbuf_free(p);
  }

  /* if the whole file has not yet been sent then continue  */
  tftp_send_next_block(upcb, args, addr, port);

  pbuf_free(p);
}

/**
  * @brief  receive callback during tftp write operation (not on standard port 69)
  * @param  arg: pointer on argument passed to callback
  * @param  udp_pcb: pointer on the udp pcb
  * @param  pkt_buf: pointer on the received pbuf
  * @param  addr: pointer on remote IP address
  * @param  port: pointer on remote port
  * @retval None
  */
void wrq_recv_callback(void *arg, struct udp_pcb *upcb, struct pbuf *pkt_buf, const ip_addr_t *addr, u16_t port)
{
  tftp_connection_args *args = (tftp_connection_args *)arg;
  int n = 0;

  /* we expect to receive only one pbuf (pbuf size should be 
     configured > max TFTP frame size */
  if (pkt_buf->len != pkt_buf->tot_len)
  {
    return;
  }

  /* Does this packet have any valid data to write? */
  if ((pkt_buf->len > TFTP_DATA_PKT_HDR_LEN) &&
      (tftp_extract_block(pkt_buf->payload) == (args->block + 1)))
  {
    /* write the received data to the file */
    f_write(&file_CR, (char*)pkt_buf->payload + TFTP_DATA_PKT_HDR_LEN, pkt_buf->len - TFTP_DATA_PKT_HDR_LEN, (UINT*)&n);

    if (n <= 0)
    {
      tftp_send_error_message(upcb, addr, port, TFTP_ERR_FILE_NOT_FOUND);
      /* close the connection */
      tftp_cleanup_wr(upcb, args); /* close the connection */
    }
    
    /* update our block number to match the block number just received */
    args->block++;
    
    /* update total bytes  */
    (args->tot_bytes) += (pkt_buf->len - TFTP_DATA_PKT_HDR_LEN);
  }
  else if (tftp_extract_block(pkt_buf->payload) == (args->block + 1))
  {
    /* update our block number to match the block number just received  */
    args->block++;
  }

  /* Send the appropriate ACK pkt (the block number sent in the ACK pkt echoes
   * the block number of the DATA pkt we just received - see RFC1350)
   * NOTE!: If the DATA pkt we received did not have the appropriate block
   * number, then the args->block (our block number) is never updated and
   * we simply send "duplicate ACK" which has the same block number as the
   * last ACK pkt we sent.  This lets the host know that we are still waiting
   * on block number args->block+1. 
   */
  tftp_send_ack_packet(upcb, addr, port, args->block);

  /* If the last write returned less than the maximum TFTP data pkt length,
   * then we've received the whole file and so we can quit (this is how TFTP
   * signals the end of a transfer!)
   */
  if (pkt_buf->len < TFTP_DATA_PKT_LEN_MAX)
  {
    tftp_cleanup_wr(upcb, args);
    pbuf_free(pkt_buf);
  }
  else
  {
    pbuf_free(pkt_buf);
    return;
  }
}


/**
  * @brief  processes tftp read operation
  * @param  upcb: pointer on udp pcb 
  * @param  to: pointer on remote IP address
  * @param  to_port: pointer on remote udp port
  * @param  FileName: pointer on filename to be read
  * @retval error code
  */
int tftp_process_read(struct udp_pcb *upcb, const ip_addr_t *to, unsigned short to_port, char* FileName)
{
  tftp_connection_args *args = NULL;

  /* If Could not open the file which will be transmitted  */
  if (f_open(&file_SD, (const TCHAR*)FileName, FA_READ) != FR_OK)
  {
    tftp_send_error_message(upcb, to, to_port, TFTP_ERR_FILE_NOT_FOUND);

    tftp_cleanup_rd(upcb, args);

    return 0;
  }
  
  args = mem_malloc(sizeof *args);
  /* If we aren't able to allocate memory for a "tftp_connection_args" */
  if (!args)
  {
    /* unable to allocate memory for tftp args  */
    tftp_send_error_message(upcb, to, to_port, TFTP_ERR_NOTDEFINED);

    /* no need to use tftp_cleanup_rd because no "tftp_connection_args" struct has been malloc'd   */
    tftp_cleanup_rd(upcb, args);

    return 0;
  }

  /* initialize connection structure  */
  args->op = TFTP_RRQ;
  args->to_ip.addr = to->addr;
  args->to_port = to_port;
  args->block = 1; /* block number starts at 1 (not 0) according to RFC1350  */
  args->tot_bytes = 0;


  /* set callback for receives on this UDP PCB  */
  udp_recv(upcb, rrq_recv_callback, args);

  /* initiate the transaction by sending the first block of data,
    further blocks will be sent when ACKs are received */

  tftp_send_next_block(upcb, args, to, to_port);

  return 1;
}

/**
  * @brief  processes tftp write operation
  * @param  upcb: pointer on upd pcb 
  * @param  to: pointer on remote IP address
  * @param  to_port: pointer on remote udp port
  * @param  FileName: pointer on filename to be written 
  * @retval error code
  */
int tftp_process_write(struct udp_pcb *upcb, const ip_addr_t *to, unsigned short to_port, char *FileName)
{
  tftp_connection_args *args = NULL;

  /* Can not create file */
  if (f_open(&file_CR, (const TCHAR*)FileName, FA_CREATE_ALWAYS|FA_WRITE) != FR_OK)
  {
    tftp_send_error_message(upcb, to, to_port, TFTP_ERR_NOTDEFINED);

    tftp_cleanup_wr(upcb, args);

    return 0;
  }

  args = mem_malloc(sizeof *args);
  if (!args)
  {
    tftp_send_error_message(upcb, to, to_port, TFTP_ERR_NOTDEFINED);

    tftp_cleanup_wr(upcb, args);

    return 0;
  }

  args->op = TFTP_WRQ;
  args->to_ip.addr = to->addr;
  args->to_port = to_port;
  /* the block # used as a positive response to a WRQ is _always_ 0!!! (see RFC1350)  */
  args->block = 0;
  args->tot_bytes = 0;

  /* set callback for receives on this UDP PCB  */
  udp_recv(upcb, wrq_recv_callback, args);

  /* initiate the write transaction by sending the first ack */
  tftp_send_ack_packet(upcb, to, to_port, args->block);

  return 0;
}


/**
  * @brief  processes the tftp request on port 69
  * @param  pkt_buf: pointer on received pbuf
  * @param  ip_addr: pointer on source IP address
  * @param  port: pointer on source udp port
  * @retval None
  */
void process_tftp_request(struct pbuf *pkt_buf, const ip_addr_t *addr, u16_t port)
{
  tftp_opcode op = tftp_decode_op(pkt_buf->payload);
  char FileName[30];
  struct udp_pcb *upcb;
  err_t err;

  /* create new UDP PCB structure */
  upcb = udp_new();
  if (!upcb)
  {     
    /* Error creating PCB. Out of Memory  */
    return;
  }

  /* bind to port 0 to receive next available free port */
  /* NOTE:  This is how TFTP works.  There is a UDP PCB for the standard port
   * 69 which al transactions begin communication on, however all subsequent
   * transactions for a given "stream" occur on another port!  */
  err = udp_bind(upcb, IP_ADDR_ANY, 0);
  if (err != ERR_OK)
  {    
    /* Unable to bind to port */
    return;
  }
  switch (op)
  {
    case TFTP_RRQ: /* TFTP RRQ (read request) */
    {
      /* Read the name of the file asked by the client to be sent from the SD card */
      tftp_extract_filename(FileName, pkt_buf->payload);

      /* Could not open filesystem */
      if(f_mount(&filesystem, (TCHAR const*)"", 0) != FR_OK)
      {
        return;
      }
      /* Could not open the selected directory */
      if (f_opendir(&dir_1, "/") != FR_OK)
      {
        return;
      }
      /* Start the TFTP read mode*/
      tftp_process_read(upcb, addr, port, FileName);
      break;
    } 

    case TFTP_WRQ: /* TFTP WRQ (write request) */
    {
      /* Read the name of the file asked by the client to be received and written in the SD card */
      tftp_extract_filename(FileName, pkt_buf->payload);
  
      /* Could not open filesystem */
      if(f_mount(&filesystem, (TCHAR const*)"", 0) != FR_OK)
      {
        return;
      }
        
      /* If Could not open the selected directory */
      if (f_opendir(&dir_2, "/") != FR_OK)
      {
        return;
      }
        
      /* Start the TFTP write mode */
      tftp_process_write(upcb, addr, port, FileName);
      break;
    }
    default: /* TFTP unknown request op */
      /* send generic access violation message */
      tftp_send_error_message(upcb, addr, port, TFTP_ERR_ACCESS_VIOLATION);
      udp_remove(upcb);

      break;
  }
}


/**
  * @brief  tftp receive callback on port 69
  * @param  arg: pointer on argument passed to callback (not used here)
  * @param  upcb: pointer on udp pcb
  * @param  pkt_buf: pointer on received pbuf
  * @param  ip_addr: pointer on source IP address
  * @param  port: pointer on source udp port
  * @retval None
  */
void recv_callback_tftp(void *arg, struct udp_pcb *upcb, struct pbuf *pkt_buf,
                        const ip_addr_t *addr, u16_t port)
{
  /* process new connection request */
  process_tftp_request(pkt_buf, addr, port);

  /* free pbuf */
  pbuf_free(pkt_buf);
}



/**
  * @brief  Initializes the udp pcb for TFTP 
  * @param  None
  * @retval None
  */
void tftpd_init(void)
{
  err_t err;
  unsigned port = 69;

  /* create a new UDP PCB structure  */
  UDPpcb = udp_new();
  if (UDPpcb)
  {  
    /* Bind this PCB to port 69  */
    err = udp_bind(UDPpcb, IP_ADDR_ANY, port);
    if (err == ERR_OK)
    {    
      /* TFTP server start  */
      udp_recv(UDPpcb, recv_callback_tftp, NULL);
    }
  }
}
