#include "udpecho.h"
#include "lwip/netif.h"
#include "lwip/ip.h"
#include "lwip/tcp.h"
#include "lwip/tcpip.h"
#include "lwip/ip4_addr.h"
#include "lwip/init.h"
#include "lwip/etharp.h"
#include "lwip/udp.h"
#include "lwip/pbuf.h"
#include <stdio.h>
#include "string.h"
#include "main.h"
#include "ip4_addr.h"

extern ip4_addr_t ipaddr;

static void udp_demo_callback(void *arg,struct udp_pcb *upcb,struct pbuf *p,const ip_addr_t *addr,u16_t port)
{
	char *reply;
	struct pbuf *q=NULL;
	err_t  err;
	if(arg)
	{
		printf("%s",(char *)arg);
	}
	reply=Send_Data(p);
	pbuf_free(p);
	q=pbuf_alloc(PBUF_TRANSPORT,strlen(reply)+1,PBUF_RAM);
	if(!q)
	{
		printf("EMPTY!!!\r\n");
		return ;
	}
	printf("SourceIP:%ld\r\n",addr->addr);
	memset(q->payload,0,q->len);
	memcpy(q->payload,reply,strlen(reply));
	printf("The Value of Q:%s\r\n",q->payload);
	
	err=udp_sendto(upcb,q,addr,port);
	if(err!=ERR_OK)
	{
		printf("UDP send_error:%d\r\n",err);
	}
	else
	{
		printf("data over!\r\n");
	}
	//udp_disconnect(upcb);
	pbuf_free(q);
}

static char  *st_buffer="We get a data\r\n";
void UDP_Echo_Init(void)
{
	err_t err;
	struct udp_pcb *udpecho_pcb;
	ip4_addr_t sip;
	IP4_ADDR(&sip,192,168,1,1);
	/*新建一个控制块*/
	udpecho_pcb=udp_new();
	if(udpecho_pcb!=NULL)printf("Finish _udp_new()!!!\r\n");
	printf("LocaalIP:%ld\r\n",ipaddr.addr);
	/*绑定端口*/
	err=udp_bind(udpecho_pcb,&ipaddr,UDP_ECHO_PORT);
	if(err==ERR_OK)printf("Finish _udp_bind()!!!\r\n");
	//udp_connect(udpecho_pcb,&sip,UDP_ECHO_PORT);
	
	/*注册回调函数*/
	udp_recv(udpecho_pcb,udp_demo_callback,(void *)st_buffer);
	printf("Finish _udp_recv()!!!\r\n");

}
/*选择输出不同的数据*/
//input：数据类型、得到的数据
char* Send_Data(struct pbuf* opt)
{
	unsigned char *p=opt->payload;
	switch((*p))
	{
		case '1':
		{
			return "temp:23.1\r\n";
		}
		case '2':
		{
			return "win:34km\\s\r\n";
		}
		default:
		{
			return "ERROR DATA TYPE!\r\n";
		}
	}
}
