/**
  ******************************************************************************
  * @file    vncserver_app.c
  * @author  MCD Application Team
  * @brief   VNC application functions
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
#include <stdlib.h>
#include "string.h"
#include "cmsis_os.h"
#include "GUI.h"
#include "GUI_VNC.h"
#include "k_mem.h"
#include "main.h"
#include "lwip/opt.h"
#include "lwip/memp.h"
#include "lwip/dhcp.h"
#include "lwip/tcpip.h"
#include "lwip/sockets.h"
#include "vncserver_app.h"
#include "ethernetif.h"

/** @addtogroup VNC_SERVER_MODULE
  * @{
  */

/** @defgroup VNC_APPLICATION
  * @brief vnc application routines
  * @{
  */

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define _VNCServer_PRIO         osPriorityAboveNormal
#define _VNCServer_Stack         (10 * configMINIMAL_STACK_SIZE) 
#define MAX_DHCP_TRIES  4
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

__IO uint8_t DHCP_state;
struct netif gnetif; 
uint8_t ip_address[4], sn_mask[4], gw_address[4]; 
static GUI_VNC_CONTEXT    _Context;
static struct sockaddr_in _Addr;
int _Sock;
osThreadId  _VNCServer_TCB;
osThreadId   _DHCPClinet_TCB = NULL;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Called by the server to send data
  * @param  buf: buffer to be sent.
  * @param  len: length of buf.
  * @param  pConnectionInfo: Connection info  
  * @retval transmit status.
  */
static int _Send(const U8 * buf, int len, void * pConnectionInfo) {
  int r;

  r = send((long)pConnectionInfo, (const char *)buf, len, 0);
  return r;
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
  int sock;
  struct sockaddr_in addr;

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
  * @retval None
  */
static void _ServerThread(void const * argument)
{
  int s;
  u32_t AddrLen;
  U16 Port;
  
  /* Prepare socket (one time setup) 
  Default port for VNC is is 590x, where x is the 0-based layer index */
  Port = 5900 + _Context.ServerIndex; 
  
  /* Loop until we get a socket into listening state */
  do {
    s = _ListenAtTcpAddr(Port);
    if (s != -1) {
      break;
    }
    vTaskDelay(100); // Try again
  } while (1);
  
  /* Loop once per client and create a thread for the actual server */
  while (1) 
  {
    /* Wait for an incoming connection */
    AddrLen = sizeof(_Addr);
    _Sock = accept(s, (struct sockaddr*)&_Addr, &AddrLen);
    
    /* Run the actual server */
    GUI_VNC_Process(&_Context, _Send, _Recv, (void *)_Sock);
    
    /* Close the connection */
    closesocket(_Sock);
    memset(&_Addr, 0, sizeof(struct sockaddr_in));
  }
}

/**
  * @brief  starts VNC server thread  
  * @param  LayerIndex: LCD layer index
  * @param  ServerIndex: Server index
  * @retval server started or not
  */
int GUI_VNC_X_StartServer(int LayerIndex, int ServerIndex) {

  /* Init VNC context and attach to layer (so context is updated if the display-layer-contents change */
  GUI_VNC_AttachToLayer(&_Context, LayerIndex);
  _Context.ServerIndex = ServerIndex;
  _Context.LayerIndex = LayerIndex;
  
  /* Create task for VNC Server */
  osThreadDef(VNC_Server, _ServerThread, _VNCServer_PRIO, 0, _VNCServer_Stack);
  _VNCServer_TCB = osThreadCreate (osThread(VNC_Server), NULL);
 
  if (_VNCServer_TCB == 0)
  {
    /* K.O., server initialization failed */
    return 0;
  }
  else
  {
    /* O.k., server has been started */ 
    return 1;
  }
}

/**
  * @brief  Retrieves the IP addr. of the currently connected VNC client. 
  * @param  Addr: IP address
  * @retval None
  */
void GUI_VNC_X_getpeername(U32 * Addr) {
  *Addr = _Addr.sin_addr.s_addr;
}

/**
  * @brief  Get IP, MASK or GW address from VNC server GUI 
  * @param  type: type of address can be (IP_ADDRESS, SUBNET_MASK, GW_ADDRESS) 
  * @param  addrx: x: 3 -> 0 specify the byte of the address
  * @retval None
  */
void _VNCApp_GetIPAddress(uint8_t type, uint8_t addr3, uint8_t addr2, uint8_t addr1, uint8_t addr0)
{
  switch (type)
  {
  case IP_ADDRESS:
    ip_address[3] = addr3;
    ip_address[2] = addr2;
    ip_address[1] = addr1;
    ip_address[0] = addr0;
    break;
  case SUBNET_MASK:
    sn_mask[3] = addr3;
    sn_mask[2] = addr2;
    sn_mask[1] = addr1;
    sn_mask[0] = addr0;
    break;
  case GW_ADDRESS:
    gw_address[3] = addr3;
    gw_address[2] = addr2;
    gw_address[1] = addr1;
    gw_address[0] = addr0;
    break;
  }
}

/**
  * @brief  Network configuration 
  * @param  use_dhcp: indicates if DHCP client is activated or not
  * @retval None
  */
void NetworkInit(uint8_t use_dhcp)
{
  struct ip_addr ipaddr;
  struct ip_addr netmask;
  struct ip_addr gw;
  
  /* Create tcp_ip stack thread */
  tcpip_init( NULL, NULL );
  
  /* IP address setting */
  if (use_dhcp)
  {
    ipaddr.addr = 0;
    netmask.addr = 0;
    gw.addr = 0;
  } 
  else
  {
    IP4_ADDR(&ipaddr, ip_address[3], ip_address[2], ip_address[1], ip_address[0]);
    IP4_ADDR(&netmask, sn_mask[3], sn_mask[2] , sn_mask[1], sn_mask[0]);
    IP4_ADDR(&gw, gw_address[3], gw_address[2], gw_address[1], gw_address[0]); 
  }
  
  /* Add the network interface */
  netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &tcpip_input);
  
  /*  Registers the default network interface. */
  netif_set_default(&gnetif);
  
  if (netif_is_link_up(&gnetif))
  {
    /* When the netif is fully configured this function must be called.*/
    netif_set_up(&gnetif);
    
    _VNCServer_Notify(NOTIFY_SERVER_NETIF_UP);
    
    if (use_dhcp) 
    {
      DHCP_state = DHCP_START;
    }
  }
  else
  {
    /* When the netif link is down this function must be called */
    netif_set_down(&gnetif);

    _VNCServer_Notify(NOTIFY_SERVER_NETIF_DOWN);
  }
  
  if (use_dhcp) 
  {
    /* Start DHCPClient */
    osThreadDef(DHCP, DHCP_thread, osPriorityBelowNormal, 0, configMINIMAL_STACK_SIZE);
    _DHCPClinet_TCB = osThreadCreate (osThread(DHCP), &gnetif);
  }
  
  /* Cable management BSP Configuration ***************************************/
  /* Init IO Expander */
  BSP_IO_Init();
  /* Enable IO Expander interrupt for ETH MII pin */
  BSP_IO_ConfigPin(MII_INT_PIN, IO_MODE_IT_FALLING_EDGE);
}

/**
  * @brief  This function notify user about link status changement.
  * @param  netif: the network interface
  * @retval None
  */
void ethernetif_notify_conn_changed(struct netif *netif)
{
  struct ip_addr ipaddr;
  struct ip_addr netmask;
  struct ip_addr gw;

  if(netif_is_link_up(netif))
  {
    /* IP address setting */
    IP4_ADDR(&ipaddr, ip_address[3], ip_address[2], ip_address[1], ip_address[0]);
    IP4_ADDR(&netmask, sn_mask[3], sn_mask[2] , sn_mask[1], sn_mask[0]);
    IP4_ADDR(&gw, gw_address[3], gw_address[2], gw_address[1], gw_address[0]);
    
    netif_set_addr(netif, &ipaddr , &netmask, &gw);
    
    /* When the netif is fully configured this function must be called.*/
    netif_set_up(netif);
    
    _VNCServer_Notify(NOTIFY_SERVER_NETIF_UP);

    /* If DHCP: re-start process */
    if (_DHCPClinet_TCB != 0)
    {
      /* Update DHCP state machine */
      DHCP_state = DHCP_START;
    }
  }
  else
  {   
    /*  When the netif link is down this function must be called.*/
    netif_set_down(netif);
    
    _VNCServer_Notify(NOTIFY_SERVER_NETIF_DOWN);
    
    /* If DHCP: stop process */
    if (_DHCPClinet_TCB != 0)
    {
      /* Update DHCP state machine */
      DHCP_state = DHCP_LINK_DOWN;
    }
  }
  /* Clear IO Expander Interrupt flag */
  BSP_IO_ITClear();
}

/**
  * @brief  DHCP Process
  * @param  argument: network interface
  * @retval None
  */
void DHCP_thread(void const * argument)
{
  struct netif *netif = (struct netif *) argument;
  struct ip_addr ipaddr;
  struct ip_addr netmask;
  struct ip_addr gw;
  uint32_t IPaddress;
  
  for (;;)
  {
    switch (DHCP_state)
    {
    case DHCP_START:
      {
        _VNCServer_Notify(NOTIFY_SERVER_DHCP_START);
        netif->ip_addr.addr = 0;
        netif->netmask.addr = 0;
        netif->gw.addr = 0;
        IPaddress = 0;
        dhcp_start(netif);
        DHCP_state = DHCP_WAIT_ADDRESS;
        _VNCServer_Notify(NOTIFY_SERVER_DHCP_WAIT_ADDRESS);
      }
      break;
      
    case DHCP_WAIT_ADDRESS:
      {
        /* Read the new IP address */
        IPaddress = netif->ip_addr.addr;
        
        if (IPaddress!=0) 
        {
          /* Stop DHCP */
          dhcp_stop(netif);
          
          DHCP_state = DHCP_ADDRESS_ASSIGNED;
  
          _VNCServer_GetAssignedAddress(IP_ADDRESS, (uint8_t)(IPaddress), (uint8_t)(IPaddress >> 8), (uint8_t)(IPaddress >> 16), (uint8_t)(IPaddress >> 24));
          
          IPaddress = netif->netmask.addr;
          _VNCServer_GetAssignedAddress(SUBNET_MASK, (uint8_t)(IPaddress), (uint8_t)(IPaddress >> 8), (uint8_t)(IPaddress >> 16), (uint8_t)(IPaddress >> 24));
          
          IPaddress = netif->gw.addr;
          _VNCServer_GetAssignedAddress(GW_ADDRESS, (uint8_t)(IPaddress), (uint8_t)(IPaddress >> 8), (uint8_t)(IPaddress >> 16), (uint8_t)(IPaddress >> 24));
          
          _VNCServer_Notify(NOTIFY_SERVER_DHCP_ADDRESS_ASSIGNED);        
        }
        else
        {
          /* DHCP timeout */
          if (netif->dhcp->tries > MAX_DHCP_TRIES)
          {
            DHCP_state = DHCP_TIMEOUT;
            
            /* Stop DHCP */
            dhcp_stop(netif);
            
            /* Static address used */
            IP4_ADDR(&ipaddr, IP_ADDR3 ,IP_ADDR2 , IP_ADDR1 , IP_ADDR0 );
            IP4_ADDR(&netmask, NETMASK_ADDR3, NETMASK_ADDR2, NETMASK_ADDR1, NETMASK_ADDR0);
            IP4_ADDR(&gw, GW_ADDR3, GW_ADDR2, GW_ADDR1, GW_ADDR0);
            netif_set_addr(netif, &ipaddr , &netmask, &gw);
            
            _VNCServer_GetAssignedAddress(IP_ADDRESS, IP_ADDR3, IP_ADDR2, IP_ADDR1, IP_ADDR0);
            _VNCServer_GetAssignedAddress(SUBNET_MASK, NETMASK_ADDR3, NETMASK_ADDR2, NETMASK_ADDR1, NETMASK_ADDR0);
            _VNCServer_GetAssignedAddress(GW_ADDRESS, GW_ADDR3, GW_ADDR2, GW_ADDR1, GW_ADDR0);
            _VNCServer_Notify(NOTIFY_SERVER_DHCP_TIMEOUT);
          }
        }
      }
      break;
      
    default: break;
    }
    
    /* wait 250 ms */
    osDelay(250);
  }
}

/**
  * @brief  Process notification received from VNC Server GUI
  * @param  ID: notification ID
  * @retval None
  */
void _VNCApp_Notify(uint8_t ID)
{
  struct ip_addr ipaddr;
  struct ip_addr netmask;
  struct ip_addr gw;
  
  switch (ID)
  {
  case NOTIFY_APP_DHCP_ENABLED:
    DHCP_state = DHCP_START;
    
    if (_DHCPClinet_TCB == 0) 
    {
      /* Start DHCPClient */
      osThreadDef(DHCP, DHCP_thread, osPriorityBelowNormal, 0, configMINIMAL_STACK_SIZE * 2);
      _DHCPClinet_TCB = osThreadCreate (osThread(DHCP), &gnetif);
    }
    
    break;
  case NOTIFY_APP_DHCP_DISABLED:
    
    if (_DHCPClinet_TCB) 
    {
      /* Delete DHCP thread */
      osThreadTerminate(_DHCPClinet_TCB);
      _DHCPClinet_TCB = 0;
    }
    break;
  case NOTIFY_APP_IPADDRESS_UPDATE:
    /* IP address setting */
    IP4_ADDR(&ipaddr, ip_address[3], ip_address[2], ip_address[1], ip_address[0]);
    IP4_ADDR(&netmask, sn_mask[3], sn_mask[2] , sn_mask[1], sn_mask[0]);
    IP4_ADDR(&gw, gw_address[3], gw_address[2], gw_address[1], gw_address[0]);
    
    netif_set_addr(&gnetif, &ipaddr , &netmask, &gw); 
    
    break;
  }
}
/**
  * @}
  */

/**
  * @}
  */
