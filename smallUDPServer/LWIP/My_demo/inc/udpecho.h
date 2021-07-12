#ifndef _UDPECHO_H_
#define _UDPECHO_H_

#include "lwip/netif.h"
#include "lwip/tcp.h"
#include "lwip/ip.h"
#include "lwip/ip4_addr.h"
#include "lwip/tcpip.h"
#include "lwip/init.h"
#include "lwipopts.h"
#include "ethernetif.h"
#include "ethernet.h"



#define UDP_ECHO_PORT   5000

void  UDP_Echo_Init(void);
char * Send_Data(struct pbuf* opt);


#endif


