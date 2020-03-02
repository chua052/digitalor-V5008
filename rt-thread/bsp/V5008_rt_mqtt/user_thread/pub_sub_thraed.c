#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include <rtthread.h>

#include <rtdbg.h>

#include "usart.h"
#include "device.h"
#include "spi.h"
#include "ult.h"
#include "socket.h"
#include "w5500.h"

#include "mqtt.h"

#include "dhcp.h"
#include "sys.h"
#include "iwdg.h"
#include "config.h"
#include "mini_beep.h"
#include "rtc.h"
#include "reset.h"

#define USING_DHCP   //�Ƿ�ʹ��DHCP��̬����IP��ַ

extern uint8 txsize[];										// �����ⲿ����������Socket���ͻ����С
extern uint8 rxsize[];										// �����ⲿ����������Socket���ջ����С

extern CONFIG_MSG  ConfigMsg;

u8 ip_updata_flag = 0;

uint32_t device_id_hex;

extern u8 new_ip_flag;
int sub_flag = 0;
char device_id_char[10] = {0};//�ַ���ʽ���豸ID�������������������ʽ

//uint8 buffer[2048];// ����һ��2KB�����飬�������Socket��ͨ������

void iwdg_entry(void* parameter)
{
	IWDG_Init(6,625);	
	while(1)
	{
		IWDG_Feed();	
		rt_thread_mdelay(200);
	}
}

extern void serial_com_check_thread_entry(void* parameter);
extern void modbus_thread_entry(void* parameter);
extern void check_moudle_entry(void* parameter);

//rt_thread_t serial_com_thread;
extern rt_thread_t modbus_thread;
extern rt_thread_t lable_test_th;
//rt_thread_t iwdg_thread;

extern char Pub_LS_Topic[30];
extern char Pub_ACK_Topic[30];
extern char LS_Topic[];
extern char Pub_TH_Topic[];
extern char TH_Topic[];
extern char ACK_Topic[];

extern char IP_Topic[];
extern char Pub_IP_Topic[30];

unsigned char connet_flag = 0;//���ӱ�־λ���ж���������״̬�Ƿ�����

extern u8 ip_changed_flag;

//mqtt����+�����̣߳��������ȱ�֤�ɳɹ������ٸ���"����������־λ"�ж��Ƿ���Ҫ������Ϣ
void pub_sub_entry(void* parameter)
{
	static char meassage[150];//��Ӧ������Ϣ�����Ǳߵ�buf

	//========�������ز���
	if(ip_changed_flag == 1)//�û�������ģʽ���޸����µ�IP�����þ�̬���䷽ʽ
	{
		ip_changed_flag = 0;
		w5500_net_init02();
	}
	else//��������ʱ����̬����IP
	{
		w5500_net_init();
	}
	
	IWDG_Init(6,1024);//IWDG_Init(6,625);
	MinBeepBsp();
	RESET_GPIO_Init();
	RTC_Init(0);
	
	char sub_topic_buff[30] = {0};//�豸���ĵ������������������ǡ�V5008/ʮ���Ƶ��豸ID�� �硰V5008/3922275694��
	//====��ȡ�豸ID���ĸ��ֽڵ�ʮ�����ơ��޷���ʮ���ơ��ַ���ʽ====
	device_id_hex = Get_Device_Id();
	rt_kprintf("old_device_id == %u\r\n",device_id_hex);
	unsigned char i=0;
	for(i=0; i<10; i++)
	{
		device_id_char[9-i] = device_id_hex%10 + 48;
		device_id_hex /= 10;
	}
	rt_kprintf("new_device_id == %s\r\n",device_id_char);
	
	//====ƴ������Ҫ���ĵ���������   �硰V5008Download/3922275694��
	sprintf(sub_topic_buff, "V5008Download/%s", device_id_char);
	//====ƴ������Ҫ��������������   �硰V5008Upload/3922275694/OpeAck��
	sprintf(Pub_LS_Topic, "V5008Upload/%s/%s",device_id_char, LS_Topic);
	sprintf(Pub_TH_Topic, "V5008Upload/%s/%s",device_id_char, TH_Topic);
	sprintf(Pub_ACK_Topic, "V5008Upload/%s/%s",device_id_char, ACK_Topic);
	sprintf(Pub_IP_Topic, "V5008Upload/%s/%s",device_id_char, IP_Topic);
	
	//���������߳�
	lable_test_th = rt_thread_create("check_moudle_th", 
							check_moudle_entry, RT_NULL,
							512, 7, 10);
	if (lable_test_th != RT_NULL) 
		rt_thread_startup(lable_test_th);

		
//	serial_com_thread = rt_thread_create("serial",
//                                   serial_com_check_thread_entry, RT_NULL,
//                                   512, 7, 10);
//	if (serial_com_thread != RT_NULL)
//        rt_thread_startup(serial_com_thread);

	modbus_thread = rt_thread_create("modbus",
                                   modbus_thread_entry, RT_NULL,
                                   512, 7, 10);
	if (modbus_thread != RT_NULL)
		rt_thread_startup(modbus_thread);

		
//===========
	while(1)
	{
		connet_flag = 0;
		memset(meassage,0,sizeof(meassage));
		sub_flag = do_mqtt_subscrib(sub_topic_buff,meassage);//��������digitalorTest
		while(sub_flag < 0)
		{
			connet_flag = 0;
			rt_thread_mdelay(20);
			sub_flag = do_mqtt_subscrib(sub_topic_buff,meassage);//��������digitalorTest
		}
		
		rt_thread_mdelay(100);
	}
}



