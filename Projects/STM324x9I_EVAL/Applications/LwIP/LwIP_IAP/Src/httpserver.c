/* Copyright (c) 2001, Swedish Institute of Computer Science.
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted provided that the following conditions
  * are met:
  * 1. Redistributions of source code must retain the above copyright
  *    notice, this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright
  *    notice, this list of conditions and the following disclaimer in the
  *    documentation and/or other materials provided with the distribution.
  * 3. Neither the name of the Institute nor the names of its contributors
  *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * httpd.c
 *
 * Author : Adam Dunkels <adam@sics.se>
 *
 */


#include "httpserver.h"
#include "lwip/tcp.h"
#include "fsdata.c"
#include "main.h"
#include "flash_if.h"
#include "lcd_log.h"
#include <string.h>
#include <stdio.h>

#ifdef USE_IAP_HTTP

static __IO uint32_t DataFlag=0;
static __IO uint32_t size =0;
static __IO uint32_t FlashWriteAddress;
static uint32_t TotalReceived=0;
static char LeftBytesTab[4];
static uint8_t LeftBytes=0;
static __IO uint8_t resetpage=0;
static uint32_t ContentLengthOffset =0,BrowserFlag=0;
static __IO uint32_t TotalData=0, checklogin=0; 
struct http_state
{
  char *file;
  u32_t left;
};

typedef enum 
{
  LoginPage = 0,
  FileUploadPage,
  UploadDonePage,
  ResetDonePage
}htmlpageState;
  
htmlpageState htmlpage;
 
static const char http_crnl_2[4] = 
/* "\r\n--" */
{0xd, 0xa,0x2d,0x2d};
static const char octet_stream[14] = 
/* "octet-stream" */
{0x6f, 0x63, 0x74, 0x65, 0x74, 0x2d, 0x73, 0x74, 0x72, 0x65, 0x61, 0x6d,0x0d, };
static const char Content_Length[17] = 
/* Content Length */
{0x43, 0x6f, 0x6e, 0x74, 0x65, 0x6e, 0x74, 0x2d, 0x4c, 0x65, 0x6e, 0x67,0x74, 0x68, 0x3a, 0x20, };


static uint32_t Parse_Content_Length(char *data, uint32_t len);
static void IAP_HTTP_writedata(char* data, uint32_t len);

/* file must be allocated by caller and will be filled in
   by the function. */
static int fs_open(char *name, struct fs_file *file);

/**
  * @brief  callback function for handling connection errors
  * @param  arg: pointer to an argument to be passed to callback function
  * @param  err: LwIP error code   
  * @retval None
  */
static void conn_err(void *arg, err_t err)
{
  struct http_state *hs;

  hs = arg;
  mem_free(hs);
}

/**
  * @brief  closes tcp connection
  * @param  pcb: pointer to a tcp_pcb struct
  * @param  hs: pointer to a http_state struct
  * @retval
  */
static void close_conn(struct tcp_pcb *pcb, struct http_state *hs)
{
  tcp_arg(pcb, NULL);
  tcp_sent(pcb, NULL);
  tcp_recv(pcb, NULL);
  mem_free(hs);
  tcp_close(pcb);
}

/**
  * @brief sends data found in  member "file" of a http_state struct
  * @param pcb: pointer to a tcp_pcb struct
  * @param hs: pointer to a http_state struct
  * @retval None
  */
static void send_data(struct tcp_pcb *pcb, struct http_state *hs)
{
  err_t err;
  u16_t len;

  /* We cannot send more data than space available in the send
     buffer */
  if (tcp_sndbuf(pcb) < hs->left)
  {
    len = tcp_sndbuf(pcb);
  }
  else
  {
    len = hs->left;
  }
  err = tcp_write(pcb, hs->file, len, 0);
  if (err == ERR_OK)
  {
    hs->file += len;
    hs->left -= len;
  }
}

/**
  * @brief tcp poll callback function
  * @param arg: pointer to an argument to be passed to callback function
  * @param pcb: pointer on tcp_pcb structure
  * @retval err_t
  */
static err_t http_poll(void *arg, struct tcp_pcb *pcb)
{
  if (arg == NULL)
  {
    tcp_close(pcb);
  }
  else
  {
    send_data(pcb, (struct http_state *)arg);
  }
  return ERR_OK;
}

/**
  * @brief callback function called after a successful TCP data packet transmission  
  * @param arg: pointer to an argument to be passed to callback function
  * @param pcb: pointer on tcp_pcb structure
  * @param len
  * @retval err : LwIP error code
  */
static err_t http_sent(void *arg, struct tcp_pcb *pcb, u16_t len)
{
  struct http_state *hs;

  hs = arg;

  if (hs->left > 0)
  {
    send_data(pcb, hs);
  }
  else
  {
    close_conn(pcb, hs);
    if(resetpage ==1)
    { 
      /* Generate a software reset */
      NVIC_SystemReset();
    }
      
  }
  return ERR_OK;
}


/**
  * @brief callback function for handling TCP HTTP traffic
  * @param arg: pointer to an argument structure to be passed to callback function
  * @param pcb: pointer to a tcp_pcb structure
  * @param p: pointer to a packet buffer
  * @param err: LwIP error code
  * @retval err
  */
static err_t http_recv(void *arg, struct tcp_pcb *pcb,  struct pbuf *p, err_t err)
{
  int32_t i,len=0;
  uint32_t DataOffset, FilenameOffset;
  char *data, *ptr, filename[40], login[LOGIN_SIZE+1];
  struct fs_file file = {0, 0};
  struct http_state *hs;
  struct pbuf *ptmp = p;

#ifdef USE_LCD
  char message[46];
#endif
  
  hs = arg;

  if (err == ERR_OK && p != NULL)
  {
    /* Inform TCP that we have taken the data */
    tcp_recved(pcb, p->tot_len);
    
    if (hs->file == NULL)
    {
      data = p->payload;
      len = p->tot_len;
      
      /* process HTTP GET Login page requests */
      if (strncmp(data, "GET / HTTP", 10) == 0)
      {
        /*send the login page (which is the index page) */
        htmlpage = LoginPage;
        fs_open("/index.html", &file);
        hs->file = file.data;
        hs->left = file.len;
        pbuf_free(p);
        
        /* send index.html page */ 
        send_data(pcb, hs);
        
        /* Tell TCP that we wish be to informed of data that has been
        successfully sent by a call to the http_sent() function. */
        tcp_sent(pcb, http_sent);
      }
      /* process HTTP GET reset mcu requests */
      else if ((strncmp(data, "GET /resetmcu.cgi", 17) ==0)&&(htmlpage == UploadDonePage))
      {
        htmlpage = ResetDonePage;
        fs_open("/reset.html", &file);
        hs->file = file.data;
        hs->left = file.len;
        pbuf_free(p);
        
        /* send reset.html page */ 
        send_data(pcb, hs);   
        resetpage = 1;
        
        /* Tell TCP that we wish be to informed of data that has been
        successfully sent by a call to the http_sent() function. */
        tcp_sent(pcb, http_sent);
      }
      
      /* process POST request for checking login */
      else if ((strncmp(data, "POST /checklogin.cgi",20)==0)&&(htmlpage== LoginPage))
      {
          /* parse packet for the username & password */
          for (i=0;i<len;i++)
          {
             if (strncmp ((char*)(data+i), "username=", 9)==0)
             {            
               sprintf((char *)login,"username=%s&password=%s",USERID,PASSWORD);
               if (strncmp((char*)(data+i), (char *)login ,LOGIN_SIZE)==0)
               {
                 htmlpage = FileUploadPage;
                 fs_open("/upload.html", &file);

               }
               else
               {
                 htmlpage = LoginPage;
                 /* reload index.html */
                 fs_open("/index.html", &file);
               }
                 hs->file = file.data;
                 hs->left = file.len; 
                 pbuf_free(p);  
                 
                 /* send index.html page */ 
                 send_data(pcb, hs);
          
                 /* Tell TCP that we wish be to informed of data that has been
                 successfully sent by a call to the http_sent() function. */
                 tcp_sent(pcb, http_sent); 
               break;
             }
          }     
      }
    
      /* process POST request for file upload and incoming data packets after POST request*/
      else if (((strncmp(data, "POST /upload.cgi",16)==0)||(DataFlag >=1))&&(htmlpage == FileUploadPage))
      { 
        DataOffset =0;
        
        /* POST Packet received */
        if (DataFlag ==0)
        { 
          BrowserFlag=0;
          TotalReceived =0;
          
          /* parse packet for Content-length field */
          size = Parse_Content_Length(data, (uint32_t)(p->tot_len));
           
          /* parse packet for the octet-stream field */
          for (i=0;i<len;i++)
          {
             if (strncmp ((char*)(data+i), octet_stream, 13)==0)
             {
               DataOffset = i+16;
               break;
             }
          }  
          /* case of MSIE8 : we do not receive data in the POST packet */ 
          if (DataOffset==0)
          {
             DataFlag++;
             BrowserFlag = 1;
             pbuf_free(p);
             return ERR_OK;
             
          }
          /* case of Mozilla Firefox : we receive data in the POST packet*/
          else
          {
            TotalReceived = len - (ContentLengthOffset + 4);
          }
        }
        
        if (((DataFlag ==1)&&(BrowserFlag==1)) || ((DataFlag ==0)&&(BrowserFlag==0)))
        { 
           if ((DataFlag ==0)&&(BrowserFlag==0)) 
           {
             DataFlag++;
           }
           else if ((DataFlag ==1)&&(BrowserFlag==1))
           {
             /* parse packet for the octet-stream field */
             for (i=0;i<len;i++)
             {
               if (strncmp ((char*)(data+i), octet_stream, 13)==0)
               {
                 DataOffset = i+16;
                 break;
               }
             }
             TotalReceived+=len;
             DataFlag++;
           }  
                
           /* parse packet for the filename field */
           FilenameOffset = 0;
           for (i=0;i<len;i++)
           {
             if (strncmp ((char*)(data+i), "filename=", 9)==0)
             {
                FilenameOffset = i+10;
                break;
             }
           }           
           i =0;
           if (FilenameOffset)
           {
             while((*(data+FilenameOffset + i)!=0x22 )&&(i < 40))
             {
               filename[i] = *(data+FilenameOffset + i);
               i++;
             }
             filename[i] = 0x0;
           }
           
           if (i==0)
           {
             htmlpage = FileUploadPage;
             /* no filename, in this case reload upload page */
             fs_open("/upload.html", &file);
             hs->file = file.data;
             hs->left = file.len;
             pbuf_free(p);
             
             /* send index.html page */ 
             send_data(pcb, hs);
          
             /* Tell TCP that we wish be to informed of data that has been
             successfully sent by a call to the http_sent() function. */
             tcp_sent(pcb, http_sent); 
             DataFlag=0;
             return ERR_OK;
             
           }
             
#ifdef USE_LCD                
           LCD_UsrLog("IAP using HTTP\n");
           sprintf(message, "File: %s",filename);
           LCD_UsrLog("%s\n",message);
           LCD_UsrLog("  State: Erasing...\n");
#endif /* USE_LCD */
          
           TotalData =0;
           /* init flash */
           FLASH_If_Init();
           /* erase user flash area */
           FLASH_If_Erase(USER_FLASH_FIRST_PAGE_ADDRESS);
          
           FlashWriteAddress = USER_FLASH_FIRST_PAGE_ADDRESS;
          
#ifdef USE_LCD
            /*indicate start of flash programming */
           LCD_UsrLog("  State: Programming...\n");
#endif
         }
         /* DataFlag >1 => the packet is data only  */
         else 
         {
           TotalReceived +=len;
         }
        
         ptr = (char*)(data + DataOffset);
         len-= DataOffset;
        
         /* update Total data received counter */
         TotalData +=len;
        
         /* check if last data packet */
         if (TotalReceived == size)
         {
           /* if last packet need to remove the http boundary tag */
           /* parse packet for "\r\n--" starting from end of data */
           i=4; 
           while (strncmp ((char*)(data+ p->tot_len -i),http_crnl_2 , 4) && (p->tot_len -i > 0))
           {
             i++;
           }
           len-=i;
           TotalData-=i;
          
           /* write data in Flash */
           if (len)
           IAP_HTTP_writedata(ptr,len);
          
           DataFlag=0;
          
#ifdef USE_LCD
          sprintf(message, "%d bytes ",(int)TotalData);
          LCD_UsrLog("Tot bytes Received: %s\n", message);
          LCD_UsrLog("  State: Prog Finished ");
#endif
          htmlpage = UploadDonePage;
          /* send uploaddone.html page */
          fs_open("/uploaddone.html", &file);
          hs->file = file.data;
          hs->left = file.len;
          send_data(pcb, hs);
          
          /* Tell TCP that we wish be to informed of data that has been
           successfully sent by a call to the http_sent() function. */
          tcp_sent(pcb, http_sent);  
        }
        /* not last data packet */
        else
        {
          /* write data in flash */
          len = ptmp->len;
          len-= DataOffset;
          /* write data in flash */
          while(ptmp != NULL)
          {
            if(len)
            IAP_HTTP_writedata(ptr,len);
            ptmp = ptmp->next;
            len = ptmp->len;
            ptr = ptmp->payload;
          }
        }
        pbuf_free(p);
      }
      else
      {

       /* Bad HTTP requests */
#ifdef USE_LCD
        LCD_ErrLog("Bad HTTP request\n");
#endif
        close_conn(pcb, hs);
      }
    }
    else
    {
      pbuf_free(p);
      close_conn(pcb,hs);
    }
    
  }
  if (err == ERR_OK && p == NULL)
  {
    /* received empty frame */
    close_conn(pcb, hs);
  }

  return ERR_OK;
}

/**
  * @brief  callback function on TCP connection setup ( on port 80)
  * @param  arg: pointer to an argument structure to be passed to callback function
  * @param  pcb: pointer to a tcp_pcb structure
  * &param  err: Lwip stack error code
  * @retval err
  */
static err_t http_accept(void *arg, struct tcp_pcb *pcb, err_t err)
{
  struct http_state *hs;

  /* Allocate memory for the structure that holds the state of the connection */
  hs = mem_malloc(sizeof(struct http_state));

  if (hs == NULL)
  {
    return ERR_MEM;
  }

  /* Initialize the structure. */
  hs->file = NULL;
  hs->left = 0;

  /* Tell TCP that this is the structure we wish to be passed for our
     callbacks. */
  tcp_arg(pcb, hs);

  /* Tell TCP that we wish to be informed of incoming data by a call
     to the http_recv() function. */
  tcp_recv(pcb, http_recv);

  tcp_err(pcb, conn_err);

  tcp_poll(pcb, http_poll, 10);
  return ERR_OK;
}

/**
  * @brief  initialize HTTP webserver  
  * @param  none
  * @retval None
  */
void IAP_httpd_init(void)
{
  struct tcp_pcb *pcb;
  /*create new pcb*/
  pcb = tcp_new();
  /* bind HTTP traffic to pcb */
  tcp_bind(pcb, IP_ADDR_ANY, 80);
  /* start listening on port 80 */
  pcb = tcp_listen(pcb);
  /* define callback function for TCP connection setup */
  tcp_accept(pcb, http_accept);
}

/**
  * @brief  Opens a file defined in fsdata.c ROM filesystem 
  * @param  name : pointer to a file name
  * @param  file : pointer to a fs_file structure  
  * @retval  1 if success, 0 if fail
  */
static int fs_open(char *name, struct fs_file *file)
{
  struct fsdata_file_noconst *f;

  for (f = (struct fsdata_file_noconst *)FS_ROOT; f != NULL; f = (struct fsdata_file_noconst *)f->next)
  {
    if (!strcmp(name, f->name))
    {
      file->data = f->data;
      file->len = f->len;
      return 1;
    }
  }
  return 0;
}

/**
  * @brief  Extract the Content_Length data from HTML data  
  * @param  data : pointer on receive packet buffer 
  * @param  len  : buffer length  
  * @retval size : Content_length in numeric format
  */
static uint32_t Parse_Content_Length(char *data, uint32_t len)
{
  uint32_t i=0,size=0, S=1;
  int32_t j=0;
  char sizestring[6], *ptr;
   
  ContentLengthOffset =0;
  
  /* find Content-Length data in packet buffer */
  for (i=0;i<len;i++)
  {
    if (strncmp ((char*)(data+i), Content_Length, 16)==0)
    {
      ContentLengthOffset = i+16;
      break;
    }
  }
  /* read Content-Length value */
  if (ContentLengthOffset)
  {
    i=0;
    ptr = (char*)(data + ContentLengthOffset);
    while(*(ptr+i)!=0x0d)
    {
      sizestring[i] = *(ptr+i);
      i++;
      ContentLengthOffset++; 
    }
    if (i>0)
    {
      /* transform string data into numeric format */
      for(j=i-1;j>=0;j--)
      {
        size += (sizestring[j]-'0')*S;
        S=S*10;
      }
    }
  }
  return size;
}

/**
  * @brief  writes received data in flash    
  * @param  ptr: data pointer
  * @param  len: data length
  * @retval None 
  */
static void IAP_HTTP_writedata(char * ptr, uint32_t len)            
{
  uint32_t count, i=0, j=0;
  
  /* check if any left bytes from previous packet transfer*/
  /* if it is the case do a concat with new data to create a 32-bit word */
  if (LeftBytes)
  {
    while(LeftBytes<=3)
    {
      if(len>(j+1))
      {
        LeftBytesTab[LeftBytes++] = *(ptr+j);
      }
      else
      {
        LeftBytesTab[LeftBytes++] = 0xFF;
      }
      j++;
    }
    FLASH_If_Write(&FlashWriteAddress, (uint32_t*)(LeftBytesTab),1);
    LeftBytes =0;
    
    /* update data pointer */
    ptr = (char*)(ptr+j);
    len = len -j;
  }
  
  /* write received bytes into flash */
  count = len/4;
  
  /* check if remaining bytes < 4 */
  i= len%4;
  if (i>0)
  {
    if (TotalReceived != size)
    {
      /* store bytes in LeftBytesTab */
      LeftBytes=0;
      for(;i>0;i--)
      LeftBytesTab[LeftBytes++] = *(char*)(ptr+ len-i);  
    }
    else count++;
  }
  FLASH_If_Write(&FlashWriteAddress, (uint32_t*)ptr ,count);
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
