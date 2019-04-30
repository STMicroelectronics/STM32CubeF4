#ifndef __HTTPSERVER_NETCONN_H__
#define __HTTPSERVER_NETCONN_H__

#include "lwip/api.h"

void http_server_netconn_init(void);
void DynWebPage(struct netconn *conn);

#endif /* __HTTPSERVER_NETCONN_H__ */
