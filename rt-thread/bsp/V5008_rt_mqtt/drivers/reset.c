#include "reset.h"
#include "types.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <rtthread.h>

#include "sys.h"
#include "ult.h"
#include "mini_beep.h"
#include "iwdg.h"
#include "config.h"
#include "stmflash.h"
#include "main.h"


extern rt_thread_t pub_sub_th;
extern rt_thread_t init_thread;
extern rt_thread_t serial_com_thread;
extern rt_thread_t modbus_thread;
extern rt_thread_t lable_test_th;

void ResetSystem(void)
{
	NVIC_SystemReset();
}

//复位参数按钮使能端
void RESET_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	GPIO_InitStructure.GPIO_Pin 	=  RST_NET_IO;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IPU; 	    //上拉输入
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_10MHz;		//IO口速度为10MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					    //根据设定参数初始化
}

u8 enter_config_flag = 0;//进入配置模式标志位

/************************************
功能:  用按键复位系统
************************************/
void Key_Reset(void)
{

	if(RST_NET==0)				//接通按键
	{
		rt_thread_mdelay(1);			     //等待硬件上电稳定
		if(RST_NET==0)				//接通按键
		{
			rt_thread_mdelay(3000);
			if(RST_NET==0) //若按住复位键超过三秒，则进入配置模式
			{
				//蜂鸣器响短暂的两声
				SetMiniBeepValue(1);
				rt_thread_mdelay(50);
				SetMiniBeepValue(0);
				rt_thread_mdelay(50);
				SetMiniBeepValue(1);
				rt_thread_mdelay(50);
				SetMiniBeepValue(0);
				rt_thread_mdelay(50);
				
				//关闭其他正在运行的线程，只保留当前线程 serial_com_thread
				rt_thread_delete(pub_sub_th);//关闭MQTT发布订阅线程
				rt_thread_delete(modbus_thread);//关闭modbus数据转换线程
				rt_thread_delete(lable_test_th);//关闭设备应答操作线程
				
				
				//进入配置模式
				config_mode();
			}
			else//只是单次按了复位键，执行普通复位
			{
//				SetMiniBeepValue(1);
//				rt_thread_mdelay(50);
//				SetMiniBeepValue(0);
//				rt_thread_mdelay(50);
				ResetSystem();
			}
			
		}
	}

}

u8 ip_changed_flag = 0;//判断配置模式下有没有修改了设备IP，有则接下来重新连接网络时，采用静态分配
CONFIG_MSG  ConfigMsgNew;
extern uint8_t domain_ip[4];
extern uint16_t service_port;
extern uint16_t pub_port;
extern uint16_t sub_port;
extern void pub_sub_entry(void* parameter);

//配置模式，设备接收来自上位机的信息，进行网关参数，MQTT通信参数等配置
void config_mode(void)
{
	u8 config_buff[28] = {0};
	u8 recv_complite = 0;
	while(recv_complite == 1)//等待接收数据完成
	{
		//网关参数
		ConfigMsgNew.lip[0] = config_buff[0];
		ConfigMsgNew.lip[1] = config_buff[1];
		ConfigMsgNew.lip[2] = config_buff[2];
		ConfigMsgNew.lip[3] = config_buff[3];
		
		ConfigMsgNew.sub[0] = config_buff[4];
		ConfigMsgNew.sub[1] = config_buff[5];
		ConfigMsgNew.sub[2] = config_buff[6];
		ConfigMsgNew.sub[3] = config_buff[7];
		
		ConfigMsgNew.gw[0] = config_buff[8];
		ConfigMsgNew.gw[1] = config_buff[9];
		ConfigMsgNew.gw[2] = config_buff[10];
		ConfigMsgNew.gw[3] = config_buff[11];
		
		ConfigMsgNew.mac[0] = config_buff[12];
		ConfigMsgNew.mac[1] = config_buff[13];
		ConfigMsgNew.mac[2] = config_buff[14];
		ConfigMsgNew.mac[3] = config_buff[15];
		ConfigMsgNew.mac[4] = config_buff[16];
		ConfigMsgNew.mac[5] = config_buff[17];
		
		//MQTT通信部分参数
		domain_ip[0] = config_buff[18];
		domain_ip[1] = config_buff[19];
		domain_ip[2] = config_buff[20];
		domain_ip[3] = config_buff[21];
		
		service_port = ((u16)config_buff[22]<<8) | ((u16)config_buff[23]);
		pub_port = ((u16)config_buff[24]<<8) | ((u16)config_buff[25]);
		sub_port = ((u16)config_buff[26]<<8) | ((u16)config_buff[27]);
		
		//判断用户此次有无修改网关参数
		u8 falsh_ip[4] = {0};
		u8 falsh_gw[4] = {0};
		u8 falsh_dns[4] = {0};
		u8 falsh_mac[6] = {0};
		
		FlashReadData(W5500_LOCAL_IP, &falsh_ip[0], 4);
		FlashReadData(W5500_GW_IP, &falsh_gw[0], 4);
		FlashReadData(W5500_SUB, &falsh_dns[0], 4);
		FlashReadData(W5500_MAC, &falsh_mac[0], 6);
		
		u8 temp01 = memcmp(falsh_ip, ConfigMsgNew.lip, 4);
		u8 temp02 = memcmp(falsh_gw, ConfigMsgNew.gw, 4);
		u8 temp03 = memcmp(falsh_dns, ConfigMsgNew.sub, 4);
		u8 temp04 = memcmp(falsh_mac, ConfigMsgNew.mac, 6);
		
		if((temp01!=0) || (temp02!=0) || (temp03!=0) || (temp04!=0))
		{
			ip_changed_flag = 1;
		}
		break;
	}
	
	//重新开启线程
	pub_sub_th = rt_thread_create("pub_sub_th",
							   pub_sub_entry, RT_NULL,
							   2048, 7, 30);
	if (pub_sub_th != RT_NULL)
		rt_thread_startup(pub_sub_th);
}



