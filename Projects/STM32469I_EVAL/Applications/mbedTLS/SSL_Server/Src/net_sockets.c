/** 
  *  Portions COPYRIGHT 2016 STMicroelectronics
  *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
  *
  ******************************************************************************
  * @file    net_sockets.c
  * @author  MCD Application Team
  * @brief   TCP/IP or UDP/IP networking functions implementation based on LwIP API
             see the file "mbedTLS/library/net_socket_template.c" for the standard
			 implementation
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

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#include <string.h>
#include <stdint.h>
#if defined(MBEDTLS_NET_C)

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdlib.h>
#endif

#include "mbedtls/net_sockets.h"

#include "lwip/dhcp.h"
#include "lwip/tcpip.h"
#include "lwip/ip_addr.h"
#include "lwip/netdb.h"
#include "lwip/sockets.h"
   
#include "netif/ethernet.h"

#include "ethernetif.h"
#include "stm32f4xx_hal.h"

#include "main.h"

static struct netif netif;
static int initialized = 0;
struct sockaddr_storage client_addr;

static int net_would_block( const mbedtls_net_context *ctx );
/*
 * Initialize LwIP stack and get a dynamic IP address.
 */
void mbedtls_net_init( mbedtls_net_context *ctx )
{
  
  ip4_addr_t addr;
  ip4_addr_t netmask;
  ip4_addr_t gw;
  uint32_t start;
 
  ctx->fd = -1;
 
  if (initialized != 0)
    return;
  
  tcpip_init(NULL, NULL);

  /* IP default settings, to be overridden by DHCP */  

  IP4_ADDR(&addr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
  IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
  IP4_ADDR(&netmask, MASK_ADDR0, MASK_ADDR1, MASK_ADDR2, MASK_ADDR3);
  
  /* add the network interface */    
  netif_add(&netif, &addr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);
  
  /* register the default network interface. */
  netif_set_default(&netif);
 
  if (netif_is_link_up(&netif))
  {
    netif_set_up(&netif);
  }
  else
  {
    netif_set_down(&netif);
  }
#ifdef USE_DHCP
  dhcp_start(&netif);
#endif
  osDelay(500);

  start = HAL_GetTick();
  
  while((netif.ip_addr.addr == 0) && (HAL_GetTick() - start < 10000))
  {
  }
  
  if (netif.ip_addr.addr == 0)
  {
    printf(" Failed to get ip address! Please check your network configuration.\n");
    Error_Handler();
  }
  else
  {
     printf("\nIpAdress = %lu.%lu.%lu.%lu\n", (netif.ip_addr.addr & 0xff), ((netif.ip_addr.addr >> 8) & 0xff)
                                        , ((netif.ip_addr.addr >> 16) & 0xff), ((netif.ip_addr.addr >> 24)& 0xff));

    initialized = 1;
  }
}

/*
 * Initiate a TCP connection with host:port and the given protocol
 */
int mbedtls_net_connect( mbedtls_net_context *ctx, const char *host, const char *port, int proto )
{
  int ret;
  struct addrinfo hints;
  struct addrinfo *list;
  struct addrinfo *current;

  /* Do name resolution with both IPv6 and IPv4 */
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = proto == MBEDTLS_NET_PROTO_UDP ? SOCK_DGRAM : SOCK_STREAM;
  hints.ai_protocol = proto == MBEDTLS_NET_PROTO_UDP ? IPPROTO_UDP : IPPROTO_TCP;

  if(getaddrinfo(host, port, &hints, &list) != 0)
    return MBEDTLS_ERR_NET_UNKNOWN_HOST;

  /* Try the sockaddrs until a connection succeeds */
  ret = MBEDTLS_ERR_NET_UNKNOWN_HOST;
  for( current = list; current != NULL; current = current->ai_next)
  {
    ctx->fd = (int) socket(current->ai_family, current->ai_socktype, current->ai_protocol);
    if(ctx->fd < 0)
    {
      ret = MBEDTLS_ERR_NET_SOCKET_FAILED;
      continue;
    }

    if(connect(ctx->fd, current->ai_addr, (uint32_t)current->ai_addrlen) == 0)
    {
      ret = 0;
      break;
    }

    close( ctx->fd );
    ret = MBEDTLS_ERR_NET_CONNECT_FAILED;
  }

  freeaddrinfo(list);

  return ret;

}

/*
 * Create a listening socket on bind_ip:port
 */
int mbedtls_net_bind( mbedtls_net_context *ctx, const char *bind_ip, const char *port, int proto )
{
    int n, ret;
    struct addrinfo hints, *addr_list, *cur;

    /* Bind to IPv6 and/or IPv4, but only in the desired protocol */
    memset( &hints, 0, sizeof( hints ) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = proto == MBEDTLS_NET_PROTO_UDP ? SOCK_DGRAM : SOCK_STREAM;
    hints.ai_protocol = proto == MBEDTLS_NET_PROTO_UDP ? IPPROTO_UDP : IPPROTO_TCP;
    if( bind_ip == NULL )
        hints.ai_flags = AI_PASSIVE;

    if( getaddrinfo( bind_ip, port, &hints, &addr_list ) != 0 )
        return( MBEDTLS_ERR_NET_UNKNOWN_HOST );

    /* Try the sockaddrs until a binding succeeds */
    ret = MBEDTLS_ERR_NET_UNKNOWN_HOST;
    for( cur = addr_list; cur != NULL; cur = cur->ai_next )
    {
        ctx->fd = (int) socket( cur->ai_family, cur->ai_socktype,
                            cur->ai_protocol );
        if( ctx->fd < 0 )
        {
            ret = MBEDTLS_ERR_NET_SOCKET_FAILED;
            continue;
        }

        n = 1;
        if( setsockopt( ctx->fd, SOL_SOCKET, SO_REUSEADDR,
                        (const char *) &n, sizeof( n ) ) != 0 )
        {
            close( ctx->fd );
            ret = MBEDTLS_ERR_NET_SOCKET_FAILED;
            continue;
        }

        if( bind( ctx->fd, cur->ai_addr, (socklen_t)cur->ai_addrlen ) != 0 )
        {
            close( ctx->fd );
            ret = MBEDTLS_ERR_NET_BIND_FAILED;
            continue;
        }

        /* Listen only makes sense for TCP */
        if( proto == MBEDTLS_NET_PROTO_TCP )
        {
            if( listen( ctx->fd, MBEDTLS_NET_LISTEN_BACKLOG ) != 0 )
            {
                close( ctx->fd );
                ret = MBEDTLS_ERR_NET_LISTEN_FAILED;
                continue;
            }
    }

    /* I we ever get there, it's a success */
    ret = 0;
    break;
  }

  freeaddrinfo( addr_list );

  return ret;
}

/*
 * Accept a connection from a remote client
 */
int mbedtls_net_accept( mbedtls_net_context *bind_ctx,
                        mbedtls_net_context *client_ctx,
                        void *client_ip, size_t buf_size, size_t *ip_len )
{

  int ret;
  int type;

#if defined(__socklen_t_defined) || defined(_SOCKLEN_T) ||  \
    defined(_SOCKLEN_T_DECLARED) || defined(__DEFINED_socklen_t)
#error _SOCKLEN_T
	
    socklen_t n = (socklen_t) sizeof( client_addr );
    socklen_t type_len = (socklen_t) sizeof( type );
#else
    int n = (int) sizeof( client_addr );
    int type_len = (int) sizeof( type );
#endif

  /* Is this a TCP or UDP socket? */
  if(getsockopt(bind_ctx->fd, SOL_SOCKET, SO_TYPE, (void *) &type, (u32_t *)(&type_len) ) != 0 ||
               (type != SOCK_STREAM && type != SOCK_DGRAM))
  {
    return( MBEDTLS_ERR_NET_ACCEPT_FAILED );
  }

  if( type == SOCK_STREAM )
  {
    /* TCP: actual accept() */
    ret = client_ctx->fd = (int) accept( bind_ctx->fd, (struct sockaddr *) &client_addr, (u32_t *)(&n) );
  }
  else
  {
    /* UDP: wait for a message, but keep it in the queue */
    char buf[1] = { 0 };
    ret = (int) recvfrom( bind_ctx->fd, buf, sizeof( buf ), MSG_PEEK, (struct sockaddr *) &client_addr, (u32_t *)(&n) );
  }

  if( ret < 0 )
  {
    if( net_would_block( bind_ctx ) != 0 )
      return( MBEDTLS_ERR_SSL_WANT_READ );

    return( MBEDTLS_ERR_NET_ACCEPT_FAILED );
  }

  /* UDP: hijack the listening socket to communicate with the client,
   * then bind a new socket to accept new connections */
  if( type != SOCK_STREAM )
  {
    struct sockaddr_storage local_addr;
    int one = 1;

    if( connect( bind_ctx->fd, (struct sockaddr *) &client_addr, n ) != 0 )
    {
      return( MBEDTLS_ERR_NET_ACCEPT_FAILED );
    }

    client_ctx->fd = bind_ctx->fd;
    bind_ctx->fd   = -1; /* In case we exit early */

    n = sizeof( struct sockaddr_storage );
    if( getsockname(client_ctx->fd, (struct sockaddr *) &local_addr, (u32_t*)(&n) ) != 0 ||
                    (bind_ctx->fd = (int) socket( local_addr.ss_family, SOCK_DGRAM, IPPROTO_UDP ) ) < 0 ||
                     setsockopt( bind_ctx->fd, SOL_SOCKET, SO_REUSEADDR, (const char *) &one, sizeof( one ) ) != 0 )
    {
      return( MBEDTLS_ERR_NET_SOCKET_FAILED );
    }

    if( bind( bind_ctx->fd, (struct sockaddr *) &local_addr, n ) != 0 )
    {
      return( MBEDTLS_ERR_NET_BIND_FAILED );
    }
  }

  if( client_ip != NULL )
  {

    if( client_addr.ss_family == AF_INET )
    {
#if LWIP_IPV4
      struct sockaddr_in *addr4 = (struct sockaddr_in *) &client_addr;
      *ip_len = sizeof( addr4->sin_addr.s_addr );

      if( buf_size < *ip_len )
      {
        return( MBEDTLS_ERR_NET_BUFFER_TOO_SMALL );
	  }
      memcpy( client_ip, &addr4->sin_addr.s_addr, *ip_len );
#endif
    }
    else
    {
#if LWIP_IPV6
      struct sockaddr_in6 *addr6 = (struct sockaddr_in6 *) &client_addr;
      *ip_len = sizeof( addr6->sin6_addr.s6_addr );

      if( buf_size < *ip_len )
	  {
        return( MBEDTLS_ERR_NET_BUFFER_TOO_SMALL );
	  }
      memcpy( client_ip, &addr6->sin6_addr.s6_addr, *ip_len);
#endif
    }
  }

  return( 0 );
}

/*
 * Set the socket blocking or non-blocking
 */
int mbedtls_net_set_block( mbedtls_net_context *ctx )
{
  mbedtls_printf ("%s() NOT IMPLEMENTED!!\n", __FUNCTION__);
  return 0;
}

int mbedtls_net_set_nonblock( mbedtls_net_context *ctx )
{
  mbedtls_printf ("%s() NOT IMPLEMENTED!!\n", __FUNCTION__);
  return 0;
}

/*
 * Portable usleep helper
 */
void mbedtls_net_usleep( unsigned long usec )
{
  mbedtls_printf ("%s() NOT IMPLEMENTED!!\n", __FUNCTION__);
}

/*
 * Read at most 'len' characters
 */
int mbedtls_net_recv( void *ctx, unsigned char *buf, size_t len )
{
  int32_t ret;
  int32_t fd = ((mbedtls_net_context *) ctx)->fd;

  if( fd < 0 )
  {
    return MBEDTLS_ERR_NET_INVALID_CONTEXT;
  }
 
  ret = (int32_t) read( fd, buf, len );

  if( ret < 0 )
  {
    if(net_would_block(ctx) != 0)
    {
      return MBEDTLS_ERR_SSL_WANT_READ;
    }
    
    if(errno == EPIPE || errno == ECONNRESET)
    {
      return MBEDTLS_ERR_NET_CONN_RESET;
    }

    if(errno == EINTR)
    {
      return MBEDTLS_ERR_SSL_WANT_READ;
    }

    return MBEDTLS_ERR_NET_RECV_FAILED;
  }

  return ret;
}

/*
 * Read at most 'len' characters, blocking for at most 'timeout' ms
 */
int mbedtls_net_recv_timeout( void *ctx, unsigned char *buf, size_t len,
                      uint32_t timeout )
{
  mbedtls_printf ("%s() NOT IMPLEMENTED!!\n", __FUNCTION__);
  return mbedtls_net_recv( ctx, buf, len );
}


static int net_would_block( const mbedtls_net_context *ctx )
{
  /*
   * Do not return 'WOULD BLOCK' on a non-blocking socket
   */

  int val = 0;
  UNUSED(val);

  if( ( fcntl( ctx->fd, F_GETFL, val) & O_NONBLOCK ) != O_NONBLOCK )
    return( 0 );

  switch( errno )
  {
#if defined EAGAIN
    case EAGAIN:
#endif
#if defined EWOULDBLOCK && EWOULDBLOCK != EAGAIN
    case EWOULDBLOCK:
#endif
    return( 1 );
  }

  return( 0 );
}

/*
 * Write at most 'len' characters
 */
int mbedtls_net_send( void *ctx, const unsigned char *buf, size_t len )
{
  int32_t ret;
  int fd = ((mbedtls_net_context *) ctx)->fd;

  if( fd < 0 )
  {
    return MBEDTLS_ERR_NET_INVALID_CONTEXT;
  }
  
  ret = (int32_t) write(fd, buf, len);

  if( ret < 0 )
  {
    if(net_would_block(ctx) != 0)
    {
      return MBEDTLS_ERR_SSL_WANT_WRITE;
    }
    
    if(errno == EPIPE || errno == ECONNRESET)
    {
      return MBEDTLS_ERR_NET_CONN_RESET;
    }
    
    if(errno == EINTR)
    {
      return MBEDTLS_ERR_SSL_WANT_WRITE;
    }

    return MBEDTLS_ERR_NET_SEND_FAILED;
  }

  return ret;
}

/*
 * Gracefully close the connection
 */
void mbedtls_net_free( mbedtls_net_context *ctx )
{
  if( ctx->fd == -1 )
    return;

  shutdown( ctx->fd, 2 );
  close( ctx->fd );

  ctx->fd = -1;

  initialized = 0;
 }

#endif /* MBEDTLS_NET_C */
