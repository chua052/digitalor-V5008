#include "stm32f10x.h"
#include "device.h"
#include "config.h"
#include "socket.h"
#include "ult.h"
#include "w5500.h"
#include <stdio.h> 
#include <string.h>
#include <rtthread.h>
#include "stmflash.h"
#include "main.h"

CONFIG_MSG  ConfigMsg, RecvMsg;

uint8 txsize[MAX_SOCK_NUM] = {2,2,2,2,2,2,2,2};		// ѡ��8��Socketÿ��Socket���ͻ���Ĵ�С����w5500.c��void sysinit()�����ù���
uint8 rxsize[MAX_SOCK_NUM] = {2,2,2,2,2,2,2,2};		// ѡ��8��Socketÿ��Socket���ջ���Ĵ�С����w5500.c��void sysinit()�����ù���

extern uint8 MAC[6];
extern unsigned char ip_updata_flag;//��ʾ��������Ҫ����IP��ַ

uint8 pub_buf[1460];

void GPIO_Device_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
//  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO , ENABLE); 
//	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOC , ENABLE); 
  // Port A output
//  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1| GPIO_Pin_2 |GPIO_Pin_3; 
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);
//  
//  GPIO_SetBits(GPIOA, GPIO_Pin_0);
//  GPIO_SetBits(GPIOA, GPIO_Pin_1);
//  GPIO_SetBits(GPIOA, GPIO_Pin_2); // led off
//  GPIO_SetBits(GPIOA, GPIO_Pin_3); // led off  
  // Port B output;
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//  GPIO_Init(GPIOB, &GPIO_InitStructure);
//  GPIO_SetBits(GPIOB, GPIO_Pin_9);

  // Port C input
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//  GPIO_Init(GPIOC, &GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_OD;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);
//  GPIO_SetBits(GPIOA,GPIO_Pin_12);//DHT11 DATA
	
	//LED
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	
	GPIO_InitStructure.GPIO_Pin 	=  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP; 	    //��������
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_10MHz;		//IO���ٶ�Ϊ10MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					    //�����趨������ʼ��
	
	//BEEP
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin 	=  GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP; 	    //��������
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_10MHz;		//IO���ٶ�Ϊ10MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					    //�����趨������ʼ��
	
	//485	ȡ���˲������ֻ�Ӱ�����ʱ�Ĵ�ӡ������������������ʲ�������ʱȡ��ע��
//	GPIO_StructInit(&GPIO_InitStructure);
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;  			    // PA8 ����  RST���� 
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	  //�������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
  
}

void LED_ON(void)
{
	 GPIO_ResetBits( GPIOC, GPIO_Pin_0); 
}

void LED_OFF(void)
{
	GPIO_SetBits( GPIOC, GPIO_Pin_0); 
}

void BEEP_ON(void)
{
	 GPIO_SetBits( GPIOA, GPIO_Pin_1);
}

void BEEP_OFF(void)
{
	GPIO_ResetBits( GPIOA, GPIO_Pin_1); 
}


void Reset_W5500(void)
{
  GPIO_ResetBits(GPIOB, WIZ_RESET);
  rt_thread_mdelay(1);  
  GPIO_SetBits(GPIOB, WIZ_RESET);
  rt_thread_mdelay(600);
}

//reboot 
void reboot(void)
{
  pFunction Jump_To_Application;
  uint32 JumpAddress;
  JumpAddress = *(vu32*) (0x00000004);
  Jump_To_Application = (pFunction) JumpAddress;
  Jump_To_Application();
}




u8 check_ip(void)
{
	u8 falsh_ip[4] = {0};
	FlashReadData(W5500_LOCAL_IP, &falsh_ip[0], 4);
	
	if( (falsh_ip[0]==192) && (falsh_ip[1]==168) && (falsh_ip[2]==0) && (falsh_ip[3]==178) )
	{
		ip_updata_flag = 1;
		return 1;//ʹ�õ���Ĭ��IP������ʾ����������IP
	}
	
	if( (falsh_ip[0]==192) && (falsh_ip[1]==168) )
	{
		return 2;//�Ѿ����Ĺ����Ҳ���Ĭ��IP
	}
	else
	{
		ip_updata_flag = 1;
		return 3;//�豸�����ϵ�
	}
}

void set_network(void)		
{
	//==============
	u8 falsh_ip[4] = {0};
	u8 falsh_gw[4] = {0};
	u8 falsh_dns[4] = {0};
	u8 falsh_mac[6] = {0};
	
	FlashReadData(W5500_LOCAL_IP, &falsh_ip[0], 4);
	FlashReadData(W5500_GW_IP, &falsh_gw[0], 4);
	FlashReadData(W5500_SUB, &falsh_dns[0], 4);
	FlashReadData(W5500_MAC, &falsh_mac[0], 6);
	
	u8 ip_sta = 0;
	ip_sta = check_ip();
	if(ip_sta==1)//�豸ʹ�õ���Ĭ��ֵ�����ѷ������·��޸�IP��ַ��Ĭ�ϵ�ַֻ���ǳ����ϵ�ʱʹ�ã���������ʹ��Ĭ��IP 192.168.0.178  ��ֹ�������豸IP��ͻ
	{
		W5500_Configuration(ConfigMsg.gw, ConfigMsg.sub, ConfigMsg.mac, ConfigMsg.lip);
	}
	else if(ip_sta==2)//��flash���Ѿ��б�������ز���ֵ����ֱ��ʹ�øò���
	{
		W5500_Configuration(ConfigMsg.gw, ConfigMsg.sub, ConfigMsg.mac, falsh_ip);
	}
	else if(ip_sta==3)//�豸�����ϵ磬ʹ��Ĭ��IP�����ҽ�����д��flash
	{
		W5500_Configuration(ConfigMsg.gw, ConfigMsg.sub, ConfigMsg.mac, ConfigMsg.lip);
		FlashWriteData(W5500_LOCAL_IP,ConfigMsg.lip, 4);
		FlashWriteData(W5500_GW_IP,ConfigMsg.gw, 4);
		FlashWriteData(W5500_SUB,ConfigMsg.sub, 4);
		FlashWriteData(W5500_MAC,ConfigMsg.mac, 6);
	}

	sysinit(txsize, rxsize); 		// ��ʼ��8��socket
	setRTR(2000);					// ���ó�ʱʱ��
	setRCR(3);	

}

void set_default(void)				// ����Ĭ��MAC��IP��GW��SUB��DNS
{  
  uint8 mac[6]={0x80,0x80,0x80,0x80,0x80,0x80};
  
  u32 temp_id = 0;
  temp_id = Get_Device_Id();
  mac[2] = temp_id>>24;
  mac[3] = temp_id>>16;
  mac[4] = temp_id>>8;
  mac[5] = temp_id;//ʹ���豸ID��Ϊ�豸MAC��ַ��һ����
  
  uint8 lip[4]={192,168,0,180};
  uint8 sub[4]={255,255,0,0};
  uint8 gw[4]={192,168,0,1};
  uint8 dns[4]={8,8,8,8};
  memcpy(ConfigMsg.lip, lip, 4);
  memcpy(ConfigMsg.sub, sub, 4);
  memcpy(ConfigMsg.gw,  gw, 4);
  memcpy(ConfigMsg.mac, mac,6);
  memcpy(ConfigMsg.dns,dns,4);

  ConfigMsg.dhcp=0;
  ConfigMsg.debug=1;
  ConfigMsg.fw_len=0;
  
  ConfigMsg.state=NORMAL_STATE;
  ConfigMsg.sw_ver[0]=FW_VER_HIGH;
  ConfigMsg.sw_ver[1]=FW_VER_LOW;  
}


// �ж����ó���
//void NVIC_Configuration(void)
//{
//  NVIC_InitTypeDef NVIC_InitStructure;
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
//  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;			// ��ռ���ȼ���Ϊ1
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;						// �����ȼ���Ϊ0
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								// IRQͨ��ʹ��
//	NVIC_Init(&NVIC_InitStructure);																// ����ָ���Ĳ�����ʼ��NVIC�Ĵ���
//	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);  						// only app, no boot included
//}

void TIM2_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
		/* ����TIM2CLK Ϊ 72MHZ */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
    //TIM_DeInit(TIM2);
	
	/* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
    TIM_TimeBaseStructure.TIM_Period=1000;
	
    /* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
	  /* ʱ��Ԥ��Ƶ��Ϊ72 */
    TIM_TimeBaseStructure.TIM_Prescaler= 71;
	
		/* ���ⲿʱ�ӽ��в�����ʱ�ӷ�Ƶ,����û���õ� */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
		
    TIM_Cmd(TIM2, ENABLE);																		
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE);		/*�ȹرյȴ�ʹ��*/    
}

void TIM2_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);  													
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}



