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
	/*��û��RTOS������³�ʼ��lwip��*/
	lwip_init();
	
	
	/*��û��RTOS���������������ӿ�*/
	flag_link=netif_add(&gnetif,&ipaddr,&netmask,&gw,NULL,&ethernetif_init,&ethernet_input);
	
	/*ע��Ĭ������ӿ�*/
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
 ALIENTEKս��STM32������ʵ��50
 LWIP����ͨ���ۺ�ʵ��-�⺯���汾
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 �������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

/*ALIENTEKΪLWIPѧϰר�ű�д�ֲᡶALIENTEK STM32F1 LWIPʹ�ý̳�.pdf������ϸ˵����ο��ֲᡣ*/
 
//extern u8 udp_demo_flag;  //UDP ����ȫ��״̬��Ǳ���
//����UI
//mode:
//bit0:0,������;1,����ǰ�벿��UI
//bit1:0,������;1,���غ�벿��UI
//void lwip_test_ui(u8 mode)
//{
//	u8 speed;
//	u8 buf[30]; 
//	POINT_COLOR=RED;
//	if(mode&1<<0)
//	{
//		LCD_Fill(30,30,lcddev.width,110,WHITE);	//�����ʾ
//		LCD_ShowString(30,30,200,16,16,"WarShip STM32F1");
//		LCD_ShowString(30,50,200,16,16,"Ethernet lwIP Test");
//		LCD_ShowString(30,70,200,16,16,"ATOM@ALIENTEK");
//		LCD_ShowString(30,90,200,16,16,"2015/3/21"); 	
//	}
//	if(mode&1<<1)
//	{
//		LCD_Fill(30,110,lcddev.width,lcddev.height,WHITE);	//�����ʾ
//		LCD_ShowString(30,110,200,16,16,"lwIP Init Successed");
//		if(lwipdev.dhcpstatus==2)sprintf((char*)buf,"DHCP IP:%d.%d.%d.%d",lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]);//��ӡ��̬IP��ַ
//		else sprintf((char*)buf,"Static IP:%d.%d.%d.%d",lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]);//��ӡ��̬IP��ַ
//		LCD_ShowString(30,130,210,16,16,buf); 
//		speed=DM9000_Get_SpeedAndDuplex();//�õ�����
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
	delay_init();	    	//��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ9600
// 	LED_Init();			    //LED�˿ڳ�ʼ��
//	LCD_Init();				//��ʼ��LCD
//	KEY_Init();	 			//��ʼ������
//	BEEP_Init();			//��������ʼ��
//	RTC_Init();				//RTC��ʼ��
//	T_Adc_Init();			//ADC��ʼ��
  TIM3_Int_Init(999,719);//��ʱ��3Ƶ��Ϊ100hz
// 	usmart_dev.init(72);	//��ʼ��USMART		 
// 	FSMC_SRAM_Init();		//��ʼ���ⲿSRAM
	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ��
	//my_mem_init(SRAMEX);		//��ʼ���ⲿ�ڴ��
	 
	 LwIP_Init();
	 UDP_Echo_Init();
	 while(1)
	 {
		printf("LWIP is Working!!!\r\n");
		 //�����������պ���
		 ethernetif_input(&gnetif);
	//	 printf("LWIP be break at ethernetif_input!!!\r\n");
		 //����lwip�ж�ʱ�¼�
		 sys_check_timeouts();
		// printf("LWIP be break at sys_check_timeouts!!!\r\n");
	 }
//	POINT_COLOR = RED; 		
//	lwip_test_ui(1);		//����ǰ�벿��UI		    
//	while(lwip_comm_init()) //lwip��ʼ��
//	{
//		LCD_ShowString(30,110,200,20,16,"LWIP Init Falied!");
//		delay_ms(1200);
//		LCD_Fill(30,110,230,130,WHITE); //�����ʾ
//		LCD_ShowString(30,110,200,16,16,"Retrying...");  
//	}
//	LCD_ShowString(30,110,200,20,16,"LWIP Init Success!");
// 	LCD_ShowString(30,130,200,16,16,"DHCP IP configing...");
//#if LWIP_DHCP   //ʹ��DHCP
//	while((lwipdev.dhcpstatus!=2)&&(lwipdev.dhcpstatus!=0XFF))//�ȴ�DHCP��ȡ�ɹ�/��ʱ���
//	{
//		lwip_periodic_handle();	//LWIP�ں���Ҫ��ʱ�����ĺ���
//		lwip_pkt_handle();
//	}
//#endif
//	lwip_test_ui(2);		//���غ�벿��UI 
//	httpd_init();  			//Web Serverģʽ
//  	while(1)
//	{	
//		key=KEY_Scan(0);
//		switch(key)
//		{
//			case KEY0_PRES://TCP Serverģʽ
//				tcp_server_test();
//				lwip_test_ui(3);//���¼���UI
//				break;
//			case KEY1_PRES://TCP Clientģʽ
//				tcp_client_test();
//				lwip_test_ui(3);//���¼���UI
//				break; 
//			case KEY2_PRES://UDPģʽ
//				udp_demo_test();
//				lwip_test_ui(3);//���¼���UI
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
//			LCD_Fill(30,230,230,230+16,WHITE);//�����ʾ
//			LED0=!LED0;
//		} 
//	}


//++++++++++++++++++++++++++++++++++++My_test_LWIP+++++++++++++++++++++++++++++++++++++++++++//


}