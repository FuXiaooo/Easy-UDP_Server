#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "rtc.h"
#include "beep.h"
#include "tsensor.h"
#include "usart.h"	
#include "timer.h"
#include "timeouts.h"
#include "sram.h"
#include "malloc.h"
#include "string.h"
#include "usmart.h"	
#include "dm9000.h"
#include "lwip/netif.h"
#include "lwip/tcp.h"
#include "lwip/ip.h"
#include "lwip/ip4_addr.h"
#include "lwip/tcpip.h"
#include "lwip/init.h"
#include "lwipopts.h"
#include "ethernetif.h"
#include "ethernet.h"
#include "udpecho.h"
#include "main.h"
//#include "udp_demo.h"
//#include "tcp_client_demo.h"
//#include "tcp_server_demo.h"
//#include "httpd.h"
  /*Static IP ADDRESS:IP_ADDRESS:IP_ADDR0.IP_ADDR1.IP_ADDR2.IP_ADDR3*/
#define IP_ADDR0            192
#define IP_ADDR1            168
#define IP_ADDR2              1
#define IP_ADDR3            122
 
 /*NETMASK*/
#define NETMASK_ADDR0       255
#define NETMASK_ADDR1       255
#define NETMASK_ADDR2       255
#define NETMASK_ADDR3         0

/*NETGATWAY*/
#define GW_ADDR0            192
#define GW_ADDR1            168
#define GW_ADDR2              1
#define GW_ADDR3              1

struct netif  gnetif;
ip4_addr_t ipaddr;
ip4_addr_t netmask;
ip4_addr_t gw;
uint8_t IP_ADDRESS[4];
uint8_t NETMASK_ADDRESS[4];
uint8_t GATEWAY_ADDRESS[4];
 
 
 void LwIP_Init(void)
{
	struct  netif *flag_link;
	IP4_ADDR(&ipaddr,IP_ADDR0,IP_ADDR1,IP_ADDR2,IP_ADDR3);
	IP4_ADDR(&netmask,NETMASK_ADDR0,NETMASK_ADDR1,NETMASK_ADDR2,NETMASK_ADDR3);
	IP4_ADDR(&gw,GW_ADDR0,GW_ADDR1,GW_ADDR2,GW_ADDR3);
	/*在没有RTOS的情况下初始化lwip堆*/
	lwip_init();
	
	
	/*在没有RTOS的情况下添加网络接口*/
	flag_link=netif_add(&gnetif,&ipaddr,&netmask,&gw,NULL,&ethernetif_init,&ethernet_input);
	
	/*注册默认网络接口*/
	netif_set_default(&gnetif);
	
	if(flag_link!=NULL)
	{
		gnetif.flags=gnetif.flags|NETIF_FLAG_LINK_UP;
		printf("netif is set:%d!!!\r\n",flag_link->flags);
		netif_set_up(&gnetif);
	}
	else
	{
		printf("netif is not set!!!\r\n");
		netif_set_down(&gnetif);
		while(1);
	}
}


/************************************************
 ALIENTEK战舰STM32开发板实验50
 LWIP网络通信综合实验-库函数版本
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

/*ALIENTEK为LWIP学习专门编写手册《ALIENTEK STM32F1 LWIP使用教程.pdf》，详细说明请参考手册。*/
 
//extern u8 udp_demo_flag;  //UDP 测试全局状态标记变量
//加载UI
//mode:
//bit0:0,不加载;1,加载前半部分UI
//bit1:0,不加载;1,加载后半部分UI
//void lwip_test_ui(u8 mode)
//{
//	u8 speed;
//	u8 buf[30]; 
//	POINT_COLOR=RED;
//	if(mode&1<<0)
//	{
//		LCD_Fill(30,30,lcddev.width,110,WHITE);	//清除显示
//		LCD_ShowString(30,30,200,16,16,"WarShip STM32F1");
//		LCD_ShowString(30,50,200,16,16,"Ethernet lwIP Test");
//		LCD_ShowString(30,70,200,16,16,"ATOM@ALIENTEK");
//		LCD_ShowString(30,90,200,16,16,"2015/3/21"); 	
//	}
//	if(mode&1<<1)
//	{
//		LCD_Fill(30,110,lcddev.width,lcddev.height,WHITE);	//清除显示
//		LCD_ShowString(30,110,200,16,16,"lwIP Init Successed");
//		if(lwipdev.dhcpstatus==2)sprintf((char*)buf,"DHCP IP:%d.%d.%d.%d",lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]);//打印动态IP地址
//		else sprintf((char*)buf,"Static IP:%d.%d.%d.%d",lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]);//打印静态IP地址
//		LCD_ShowString(30,130,210,16,16,buf); 
//		speed=DM9000_Get_SpeedAndDuplex();//得到网速
//		if(speed&1<<1)LCD_ShowString(30,150,200,16,16,"Ethernet Speed:10M");
//		else LCD_ShowString(30,150,200,16,16,"Ethernet Speed:100M");
//		LCD_ShowString(30,170,200,16,16,"KEY0:TCP Server Test");
//		LCD_ShowString(30,190,200,16,16,"KEY1:TCP Client Test");
//		LCD_ShowString(30,210,200,16,16,"KEY2:UDP Test");
//	}
//}

 int main(void)
 {	 
//	u8 t;
//	u8 key;
	delay_init();	    	//延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为9600
// 	LED_Init();			    //LED端口初始化
//	LCD_Init();				//初始化LCD
//	KEY_Init();	 			//初始化按键
//	BEEP_Init();			//蜂鸣器初始化
//	RTC_Init();				//RTC初始化
//	T_Adc_Init();			//ADC初始化
  TIM3_Int_Init(999,719);//定时器3频率为100hz
// 	usmart_dev.init(72);	//初始化USMART		 
// 	FSMC_SRAM_Init();		//初始化外部SRAM
	my_mem_init(SRAMIN);		//初始化内部内存池
	//my_mem_init(SRAMEX);		//初始化外部内存池
	 
	 LwIP_Init();
	 UDP_Echo_Init();
	 while(1)
	 {
		// printf("LWIP is Working!!!\r\n");
		 //调用网卡接收函数
		 ethernetif_input(&gnetif);
	//	 printf("LWIP be break at ethernetif_input!!!\r\n");
		 //处理lwip中定时事件
		 sys_check_timeouts();
		// printf("LWIP be break at sys_check_timeouts!!!\r\n");
	 }
//	POINT_COLOR = RED; 		
//	lwip_test_ui(1);		//加载前半部分UI		    
//	while(lwip_comm_init()) //lwip初始化
//	{
//		LCD_ShowString(30,110,200,20,16,"LWIP Init Falied!");
//		delay_ms(1200);
//		LCD_Fill(30,110,230,130,WHITE); //清除显示
//		LCD_ShowString(30,110,200,16,16,"Retrying...");  
//	}
//	LCD_ShowString(30,110,200,20,16,"LWIP Init Success!");
// 	LCD_ShowString(30,130,200,16,16,"DHCP IP configing...");
//#if LWIP_DHCP   //使用DHCP
//	while((lwipdev.dhcpstatus!=2)&&(lwipdev.dhcpstatus!=0XFF))//等待DHCP获取成功/超时溢出
//	{
//		lwip_periodic_handle();	//LWIP内核需要定时处理的函数
//		lwip_pkt_handle();
//	}
//#endif
//	lwip_test_ui(2);		//加载后半部分UI 
//	httpd_init();  			//Web Server模式
//  	while(1)
//	{	
//		key=KEY_Scan(0);
//		switch(key)
//		{
//			case KEY0_PRES://TCP Server模式
//				tcp_server_test();
//				lwip_test_ui(3);//重新加载UI
//				break;
//			case KEY1_PRES://TCP Client模式
//				tcp_client_test();
//				lwip_test_ui(3);//重新加载UI
//				break; 
//			case KEY2_PRES://UDP模式
//				udp_demo_test();
//				lwip_test_ui(3);//重新加载UI
//				break; 
//		}
//		lwip_periodic_handle();
//		lwip_pkt_handle();
//		delay_ms(2);
//		t++;
//		if(t==100)LCD_ShowString(30,230,200,16,16,"Please choose a mode!");
//		if(t==200)
//		{ 
//			t=0;
//			LCD_Fill(30,230,230,230+16,WHITE);//清除显示
//			LED0=!LED0;
//		} 
//	}


//++++++++++++++++++++++++++++++++++++My_test_LWIP+++++++++++++++++++++++++++++++++++++++++++//


}
