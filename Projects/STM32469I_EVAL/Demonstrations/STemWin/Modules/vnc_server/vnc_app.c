/**
  ******************************************************************************
  * @file    LwIP/LwIP_HTTP_Server_Netconn_RTOS/Src/app_ethernet.c 
  * @author  MCD Application Team
  * @brief   Ethernet specefic module
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
#include "main.h"
#include "lwip/opt.h"
#include "lwip/dhcp.h"
#include "lwip/netif.h"
#include "lwip/tcpip.h"
#include "lwip/sockets.h"
#include "vnc_app.h"
#include "ethernetif.h"
#include "GUI_VNC.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#define MAX_DHCP_TRIES  4
__IO uint8_t VNC_State = VNC_IDLE;
__IO uint8_t VNC_LockState = 0;
struct netif gnetif; /* network interface structure */
/* Semaphore to signal Ethernet Link state update */
osSemaphoreId Netif_LinkSemaphore = NULL;
/* Ethernet link thread Argument */
struct link_str link_arg;
int _Sock;
static uint8_t iptxt[128];
static struct sockaddr_in _Addr;
static GUI_VNC_CONTEXT    _Context;
static int is_initialized = 0;
static int Connection_accepted = 0;
osThreadId  VNC_ThreadId = 0;
/* Private function prototypes -----------------------------------------------*/
void VNC_Thread(void const * argument);
extern void VNC_SERVER_LogMessage (const char *message);
extern void VNC_SERVER_StatusChanged (uint8_t status);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Called by the server to send data
  * @param  buf: buffer to be sent.
  * @param  len: length of buf.
  * @param  pConnectionInfo: Connection info  
  * @retval transmit status.
  */
static int _Send(const U8 * buf, int len, void * pConnectionInfo) {
  return ( send((long)pConnectionInfo, (const char *)buf, len, 0));
}

/**
  * @brief  Called by the server when data is received
  * @param  buf: buffer to get the received data.
  * @param  len: length of received data.
  * @param  pConnectionInfo: Connection info  
  * @retval receive status.
  */
static int _Recv(U8 * buf, int len, void * pConnectionInfo) {
  return recv((long)pConnectionInfo, (char *)buf, len, 0);
}

/**
  * @brief  Starts listening at a TCP port.
  * @param  Port: TCP port to listen at 
  * @retval listen status.
  */
static int _ListenAtTcpAddr(U16 Port) {
  struct sockaddr_in addr;
  int sock;
  
  sock = socket(AF_INET, SOCK_STREAM, 0);
  memset(&addr, 0, sizeof(addr));
  addr.sin_family      = AF_INET;
  addr.sin_port        = htons(Port);
  addr.sin_addr.s_addr = INADDR_ANY;
  bind(sock, (struct sockaddr *)&addr, sizeof(addr));
  listen(sock, 1);
  return sock;
}
/**
  * @brief  VNC server thread process
  * @param  argument: not used
  * @retval none
  */
static void VNC_Process( void)
{
  static int s;
  static uint8_t Is_SocketAssigned = 0;
  u32_t AddrLen;
  U16 Port;
  
  if(Is_SocketAssigned == 0)
  {
    Is_SocketAssigned = 1;
    /* Prepare socket (one time setup) 
    Default port for VNC is is 590x, where x is the 0-based layer index */
    Port = 5900 + _Context.ServerIndex; 
    
    /* Loop until we get a socket into listening state */
    do {
      s = _ListenAtTcpAddr(Port);
      if (s != -1) {
        break;
      }
      vTaskDelay(100); /* Try again */
    } while (1);
  }
  VNC_SERVER_StatusChanged(VNC_CONN_ESTABLISHED);
  /* Loop once per client and create a thread for the actual server */
  while (VNC_State == VNC_PROCESS) 
  {
    
    /* Wait for an incoming connection */
    AddrLen = sizeof(_Addr);
    Connection_accepted = 1;
    if ((_Sock = accept(s, (struct sockaddr*)&_Addr, &AddrLen)) < 1) {
      closesocket(_Sock);
      vTaskDelay(100);      
      continue; /* Error */
    }
    Connection_accepted = 0;
    VNC_SERVER_LogMessage ("Connected to VNC Client");
    GUI_VNC_Process(&_Context, _Send, _Recv, (void *)_Sock);
    VNC_SERVER_LogMessage ((char *)iptxt); 

    /* Close the connection */
    closesocket(_Sock);
  }
}

/**
  * @brief  Initializes the lwIP stack
  * @param  None
  * @retval None
  */
static void Netif_Config(void)
{
  struct ip4_addr ipaddr;
  struct ip4_addr netmask;
  struct ip4_addr gw;	
  
  /* IP address setting */
  IP4_ADDR(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
  IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1 , NETMASK_ADDR2, NETMASK_ADDR3);
  IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
  
  /* - netif_add(struct netif *netif, struct ip_addr *ipaddr,
  struct ip_addr *netmask, struct ip_addr *gw,
  void *state, err_t (* init)(struct netif *netif),
  err_t (* input)(struct pbuf *p, struct netif *netif))
  
  Adds your network interface to the netif_list. Allocate a struct
  netif and pass a pointer to this structure as the first argument.
  Give pointers to cleared ip_addr structures when using DHCP,
  or fill them with sane numbers otherwise. The state pointer may be NULL.
  
  The init function pointer must point to a initialization function for
  your ethernet netif interface. The following code illustrates it's use.*/
  
  netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &tcpip_input);
  
  /*  Registers the default network interface. */
  netif_set_default(&gnetif);
  
  if (netif_is_link_up(&gnetif))
  {
    /* When the netif is fully configured this function must be called.*/
    netif_set_up(&gnetif);
  }
  else
  {
    /* When the netif link is down this function must be called */
    netif_set_down(&gnetif);
  }
}


/**
  * @brief  EXTI line detection callbacks
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == MFX_IRQOUT_PIN)
  {
    /* Get the IT status register value */
    if(BSP_IO_ITGetStatus(MII_INT_PIN))
    {
      /* ethernetif_notify_conn_changed(&gnetif);; */
    }
    BSP_IO_ITClear();
  }
}

/**
  * @brief  Initializes audio
  * @param  None.
  * @retval Audio state.
  */
void VNC_SERVER_Start (void)
{
 
  if(is_initialized == 0)
  {
    is_initialized = 1;
   
    /* Create tcp_ip stack thread */
    tcpip_init(NULL, NULL);
    
    /* Initialize the LwIP stack */
    Netif_Config();
    
    /* Check connection */
    if (netif_is_up(&gnetif))
    {
      /* Update VNC state machine */
      VNC_State = VNC_LINK_UP;
      VNC_SERVER_StatusChanged(VNC_LINK_UP);
    }
    else
    {  
      /* Update VNC state machine */
      VNC_State = VNC_LINK_DOWN;
      VNC_SERVER_LogMessage ("Cable is not connected \n");
      VNC_SERVER_StatusChanged(VNC_LINK_DOWN);
    } 
  }
  else if(VNC_State == VNC_INIT)
  {
    /* Check connection */
    if (netif_is_up(&gnetif))
    {
      /* Update VNC state machine */
      VNC_State = VNC_LINK_UP;
      VNC_SERVER_StatusChanged(VNC_LINK_UP);
    }
    else
    {  
      /* Update VNC state machine */
      VNC_State = VNC_LINK_DOWN;
      VNC_SERVER_LogMessage ("Cable is not connected \n");
      VNC_SERVER_StatusChanged(VNC_LINK_DOWN);
    } 
  }
  
  /* Start VNC Task */
  if(VNC_ThreadId == 0)
  {
    osThreadDef(VNC, VNC_Thread, osPriorityAboveNormal, 0, 1024);
    VNC_ThreadId = osThreadCreate (osThread(VNC), NULL);  
  }
}

/**
  * @brief  Initializes audio
  * @param  None.
  * @retval Audio state.
  */
void VNC_SERVER_Stop (void)
{
  VNC_State = VNC_IDLE;
  if(!Connection_accepted)
  closesocket(_Sock);

  VNC_SERVER_StatusChanged(VNC_IDLE);
  VNC_SERVER_LogMessage ("VNC Connection OFF");
  
  if(VNC_ThreadId != 0)
  {
    osThreadTerminate(VNC_ThreadId);
    VNC_ThreadId = 0;
  }   
}
/**
  * @brief  This function notify user about link status changement.
  * @param  netif: the network interface
  * @retval None
  */
void ethernetif_notify_conn_changed(struct netif *netif)
{
  struct ip4_addr ipaddr;
  struct ip4_addr netmask;
  struct ip4_addr gw;

  if(netif_is_link_up(netif))
  {

    VNC_SERVER_LogMessage ("Cable is now connected.");

    /* Update DHCP state machine */
    VNC_State = VNC_LINK_UP;
    VNC_SERVER_StatusChanged(VNC_LINK_UP);
    netif_set_addr(netif, &ipaddr , &netmask, &gw);
    
    /* When the netif is fully configured this function must be called.*/
    netif_set_up(netif);     
  }
  else
  {
    /* Update DHCP state machine */
    VNC_State = VNC_LINK_DOWN;
   
    /*  When the netif link is down this function must be called.*/
    netif_set_down(netif);
    

    VNC_SERVER_LogMessage ("Cable is not connected.");
    closesocket(_Sock);
  }
}

/**
  * @brief  Get VNC state
  * @param  None.
  * @retval VNC state.
  */
uint8_t  VNC_GetState(void)
{
  return VNC_State;
}


/**
  * @brief  Get VNC state
  * @param  None.
  * @retval VNC state.
  */
void VNC_SetState(uint8_t state)
{
  VNC_State = state;
}
/**
  * @brief  Get VNC state
  * @param  None.
  * @retval VNC state.
  */
uint8_t  VNC_GetLockState(void)
{
  return VNC_LockState;
}

/**
  * @brief  Get VNC state
  * @param  None.
  * @retval VNC state.
  */
void  VNC_SetLockState(uint8_t LockState)
{
  VNC_LockState = LockState;
}
/**
  * @brief  VNC Thread
* @param  argument: network interface
  * @retval None
  */
void VNC_Thread(void const * argument)
{
  struct dhcp *dhcp;
  
  for (;;)
  { 
    switch (VNC_State)
    {
    case VNC_LINK_UP:
      {
        gnetif.ip_addr.addr = 0;
        gnetif.netmask.addr = 0;
        gnetif.gw.addr = 0;
        dhcp_start(&gnetif);
        VNC_State = VNC_WAIT_FOR_ADDRESS;
        VNC_SERVER_LogMessage ("Waiting for DHCP server...\n");
        VNC_SERVER_StatusChanged(VNC_WAIT_FOR_ADDRESS);
      }
      break;
      
    case VNC_WAIT_FOR_ADDRESS:
      {        
        if (dhcp_supplied_address(&gnetif)) 
        {
          VNC_State = VNC_START;
          VNC_SERVER_StatusChanged(VNC_START);          
        }
        else
        {
          dhcp = (struct dhcp *)netif_get_client_data(&gnetif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP);
          
          /* DHCP timeout */
          if (dhcp->tries > MAX_DHCP_TRIES)
          {
            VNC_State = VNC_ERROR;
            dhcp_stop(&gnetif);
            VNC_SERVER_LogMessage ("No reply from DHCP Server!\n");
          }
        }
      }
      break;
      
    case VNC_START: 
      
      sprintf((char*)iptxt, 
              "IP address : %d.%d.%d.%d\n", 
              (uint8_t)(gnetif.ip_addr.addr), 
              (uint8_t)((gnetif.ip_addr.addr) >> 8), 
              (uint8_t)((gnetif.ip_addr.addr) >> 16), 
              (uint8_t)((gnetif.ip_addr.addr) >> 24));       
      
      VNC_SERVER_LogMessage ((char *)iptxt);
      
      /* Init VNC context and attach to layer (so context is updated if the display-layer-contents change */
      GUI_VNC_AttachToLayer(&_Context, 0);
      _Context.ServerIndex = 0;
      GUI_VNC_SetProgName ("STM32 VNC Server");
      if(VNC_LockState)
      {
        GUI_VNC_SetPassword((U8 *)"STM32");
      }
      else
      {
        GUI_VNC_SetAuthentication(NULL);
      }
      VNC_State = VNC_PROCESS;
      break;
      
      
    case VNC_PROCESS: 
      VNC_Process();
      break;
      
    case VNC_IDLE:
      break;
      
    default: 
      break;
    }
    osDelay(250);
  }
}
