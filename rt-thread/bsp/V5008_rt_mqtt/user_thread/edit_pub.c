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
#include "sys.h"

//#define USING_DHCP   //�Ƿ�ʹ��DHCP��̬����IP��ַ

extern uint8 txsize[];										// �����ⲿ����������Socket���ͻ����С
extern uint8 rxsize[];										// �����ⲿ����������Socket���ջ����С

extern u8 new_ip_flag;
extern uint8 sub_succ_flag;

extern unsigned char pMessage_cache[4];
//int pub_flag = 0;

////======����1���ϱ���ǩ�仯��һ��ĳ��Uλ״̬�����仯�����ϱ��豸Ŀǰ��Uλ�ϼ�״̬���߾���״̬��������Ϣ��
//Lab_Sta_Head sta_head;
//u8 lab_sta_buff[273] = {0};

//void edit_lab_data(void)
//{	
//	sta_head.module_addr = 0x03;//ģ���ַ
//	sta_head.mag_state = 0xff;//�Ŵ�״̬��0xFF�ޣ� 0x01���� 0x00��
//	sta_head.U_mount = 0x36;//��U��
//	sta_head.U_State_mount = 0x02;//���ϼ�״̬���߾���״̬��Uλ��
//	
//	u8 sta_pack_len = 0;
//	sta_pack_len = sta_head.U_State_mount;
//	
//	Lab_Sta_Pack sta_pack[sta_pack_len];
//	
////	sta_pack[0].U_number = 0x32;//Uλ��
////	sta_pack[0].Call_State = 0x00;//����״̬
////	sta_pack[0].Lable_Tag[0] = 0xAA;//��ǩID,4�ֽ�
////	sta_pack[0].Lable_Tag[1] = 0xBB;
////	sta_pack[0].Lable_Tag[2] = 0xCC;
////	sta_pack[0].Lable_Tag[3] = 0xDD;
////	
////	sta_pack[1].U_number = 0x32;
////	sta_pack[1].Call_State = 0x01;
////	sta_pack[1].Lable_Tag[0] = 0x00;
////	sta_pack[1].Lable_Tag[1] = 0x00;
////	sta_pack[1].Lable_Tag[2] = 0x00;
////	sta_pack[1].Lable_Tag[3] = 0x00;
//	
//	
//	lab_sta_buff[0] = sta_head.module_addr;
//	lab_sta_buff[1] = sta_head.mag_state;
//	lab_sta_buff[2] = sta_head.U_mount;
//	lab_sta_buff[3] = sta_head.U_State_mount;

////	u8 i = 0,m = 0;
////	for(i=0; i<sta_head.U_State_mount; i++,m++)
////	{
////		lab_sta_buff[4+5*i] = sta_pack[m].U_number;
////		lab_sta_buff[5+5*i] = sta_pack[m].Call_State;
////		lab_sta_buff[6+5*i] = sta_pack[m].Lable_Tag[0];
////		lab_sta_buff[7+5*i] = sta_pack[m].Lable_Tag[1];
////		lab_sta_buff[8+5*i] = sta_pack[m].Lable_Tag[2];
////	}
//}

//======����2���ϱ���ʪ�ȱ仯����ʱ����ʪ����һ�����ȵı仯ʱ���ϱ�һ����Ϣ
//Tem_Hum_Pack tem_hum_pack;
//u8 Tem_Hum_buff[25] = {0};
//extern unsigned char module;          //ģ����


//void edit_TH_data(void)
//{	
////===ģ���ַ
//	tem_hum_pack.module_addr = module;
////===�¶�����
//	tem_hum_pack.Tem[0] = 0x20;
//	tem_hum_pack.Tem[1] = 0x20;
//	
//	tem_hum_pack.Tem[2] = 0x20;
//	tem_hum_pack.Tem[3] = 0x20;
//	
//	tem_hum_pack.Tem[4] = 0x20;
//	tem_hum_pack.Tem[5] = 0x20;
//	
//	tem_hum_pack.Tem[6] = 0x20;
//	tem_hum_pack.Tem[7] = 0x20;
//	
//	tem_hum_pack.Tem[8] = 0x20;
//	tem_hum_pack.Tem[9] = 0x20;
//	
//	tem_hum_pack.Tem[10] = 0x20;
//	tem_hum_pack.Tem[11] = 0x20;
////===ʪ������==
//	tem_hum_pack.Hum[0] = 0x78;
//	tem_hum_pack.Hum[1] = 0x78;
//	
//	tem_hum_pack.Hum[2] = 0x78;
//	tem_hum_pack.Hum[3] = 0x78;
//	
//	tem_hum_pack.Hum[4] = 0x78;
//	tem_hum_pack.Hum[5] = 0x78;
//	
//	tem_hum_pack.Hum[6] = 0x78;
//	tem_hum_pack.Hum[7] = 0x78;
//	
//	tem_hum_pack.Hum[8] = 0x78;
//	tem_hum_pack.Hum[9] = 0x78;
//	
//	tem_hum_pack.Hum[10] = 0x78;
//	tem_hum_pack.Hum[11] = 0x78;
//	
//	u8 i = 0,m = 0;
//	Tem_Hum_buff[0] = tem_hum_pack.module_addr;
//	for(i=0; i<6; i++,m+=2)
//	{
//		Tem_Hum_buff[4*i+1] = tem_hum_pack.Tem[m];
//		Tem_Hum_buff[4*i+2] = tem_hum_pack.Tem[m+1];
//		Tem_Hum_buff[4*i+3] = tem_hum_pack.Hum[m];
//		Tem_Hum_buff[4*i+4] = tem_hum_pack.Hum[m+1];
//	}
//}

//======����3���ϱ�Ӧ����Ϣ�������û��·�������Լ��������
Ack_Pack ack_pack;
u8 Ack_buff[10] = {0};
u32 device_id = 0;


void edit_ack_data(unsigned char result)
{	
	device_id = Get_Device_Id();
	ack_pack.Device_Id = device_id;//ACE453DB
//	ack_pack.Cmd[0] = 0xE1;
//	ack_pack.Cmd[1] = 0x01;
//	ack_pack.Cmd[2] = 0x32;
//	ack_pack.Cmd[3] = 0x06;
	ack_pack.Result = result;//A1�ɹ���A0ʧ��
	
	Ack_buff[0] = 0xAA;
	
	Ack_buff[4] |= ack_pack.Device_Id;
	Ack_buff[3] |= ack_pack.Device_Id >> 8;
	Ack_buff[2] |= ack_pack.Device_Id >> 16;
	Ack_buff[1] |= ack_pack.Device_Id >> 24;
	
	Ack_buff[5] = ack_pack.Result;
	
	Ack_buff[6] = pMessage_cache[0];
	Ack_buff[7] = pMessage_cache[1];
	Ack_buff[8] = pMessage_cache[2];
	Ack_buff[9] = pMessage_cache[3];
	
	
}



