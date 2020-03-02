#include <stdlib.h>
#include <string.h>
#include <stdint.h>

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

//#define USING_DHCP   //�Ƿ�ʹ��DHCP��̬����IP��ַ

extern uint8 txsize[];										// �����ⲿ����������Socket���ͻ����С
extern uint8 rxsize[];										// �����ⲿ����������Socket���ջ����С

extern u8 new_ip_flag;
extern uint8 sub_succ_flag;
//int pub_flag = 0;

MQTTDat mqtt_data;

void edit_data(void)
{	
	mqtt_data.Header[0] = 0xFE;
	mqtt_data.Header[1] = 0xFE;
	
	mqtt_data.Datlen[0] = 0x00;
	mqtt_data.Datlen[1] = 0x93;
	
	mqtt_data.COM[0] = 0xE7;
	
	mqtt_data.VerNo[0] = 0x01;
	mqtt_data.VerNo[1] = 0x01;
	
	mqtt_data.ID[0] = 0x30;
	mqtt_data.ID[1] = 0x08;
	mqtt_data.ID[2] = 0x12;
	mqtt_data.ID[3] = 0xC5;
	
	mqtt_data.DatTR[0] = 0x01;
}

//mqtt�����߳�
void mqtt_pub_entry(void* parameter)
{
	static char pub_meassage[50] = {"V2-ON00"};
	
	unsigned char *pub_data = NULL;
	edit_data();
	pub_data = &mqtt_data.Header[0];
	
	char pub_topic_buff[20] = {"123"};
	int rc=0;
	
	w5500_net_init();

//===========
	
	while(1)
	{
		//sub_succ_flag��־λ�������Ըĳ��ж��ʲ����Ƿ��б仯����Ҫ������Ϣ�����У����־λ��1������һ�η���
//		if(sub_succ_flag == 0)//ģ�鶩 �ĳɹ��ٽ��з������ҷ���һ�Σ����Խ׶��õ��Ƕ��ĸ�������ͬһ������
//		{
//			pub_flag = do_mqtt_publish(pub_topic_buff,pub_data,12);//��������digitalorTest
//			while(pub_flag < 0)
//			{
//					pub_flag = do_mqtt_publish("PUBtest",pub_data,12);//��������digitalorTest
//			}
//			rc++;
//			if(rc > 999)
//			{
//				rc = 0;
//			}
//			rt_kprintf("��%d�η����ɹ�������������\n\r",rc);
//		}
		rt_thread_mdelay(1000);
//		rc++;
//		if(rc > 999)
//		{
//			rc = 100;
//		}
  }
}

