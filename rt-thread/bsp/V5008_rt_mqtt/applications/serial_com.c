#include "serial_com.h"
#include <rtthread.h>
#include "global.h"
#include "modbus.h"
#include "iwdg.h"
#include "reset.h"
#include <string.h>

USART_TypeDef* const UARTx[5] = {USART1, USART2, USART3, UART4, UART5};
const uint32_t RCC_Periph_UARTx[5] = {RCC_APB2Periph_USART1, RCC_APB1Periph_USART2, RCC_APB1Periph_USART3, RCC_APB1Periph_UART4, RCC_APB1Periph_UART5};
const uint32_t TX_RCC_APB2Periph_GPIOx[5] = {RCC_APB2Periph_GPIOA, RCC_APB2Periph_GPIOA, RCC_APB2Periph_GPIOB, RCC_APB2Periph_GPIOC, RCC_APB2Periph_GPIOC};
const uint32_t RX_RCC_APB2Periph_GPIOx[5] = {RCC_APB2Periph_GPIOA, RCC_APB2Periph_GPIOA, RCC_APB2Periph_GPIOB, RCC_APB2Periph_GPIOC, RCC_APB2Periph_GPIOD};
GPIO_TypeDef* const TX_Port[5] = {GPIOA, GPIOA, GPIOB, GPIOC, GPIOC};
GPIO_TypeDef* const RX_Port[5] = {GPIOA, GPIOA, GPIOB, GPIOC, GPIOD};
const uint16_t TX_Pin[5] = {GPIO_Pin_9, GPIO_Pin_2, GPIO_Pin_10, GPIO_Pin_10, GPIO_Pin_12};
const uint16_t RX_Pin[5] = {GPIO_Pin_10, GPIO_Pin_3, GPIO_Pin_11, GPIO_Pin_11, GPIO_Pin_2};
const IRQn_Type UARTx_IRQn[5] = {USART1_IRQn, USART2_IRQn, USART3_IRQn, UART4_IRQn, UART5_IRQn};

stUart Uart[3];

static void RS485EN1Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = RS485PORTREPIN1; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(RS485PORTREIO1, &GPIO_InitStructure);
}

static void RS485EN3Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = RS485PORTREPIN3; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(RS485PORTREIO3, &GPIO_InitStructure);
}

void UARTConfig(COM COMx, u32 BaudRate, uint8* RxBuff, uint16 BuffSize)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	if(COMx == COM1)
	{
		RCC_APB2PeriphClockCmd(RCC_Periph_UARTx[COMx] | TX_RCC_APB2Periph_GPIOx[COMx] | RX_RCC_APB2Periph_GPIOx[COMx] | RCC_APB2Periph_AFIO, ENABLE);	//ʹ��ʱ��
	}
	else
	{
		RCC_APB1PeriphClockCmd(RCC_Periph_UARTx[COMx], ENABLE);
		RCC_APB2PeriphClockCmd(TX_RCC_APB2Periph_GPIOx[COMx] | RX_RCC_APB2Periph_GPIOx[COMx] | RCC_APB2Periph_AFIO, ENABLE);	
	}
	
	/*
	if(COMx == COM3)
	{
		GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE);
	}
	*/
	
	//UARTx_TX	 
	GPIO_InitStructure.GPIO_Pin = TX_Pin[COMx]; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_Init(TX_Port[COMx], &GPIO_InitStructure);
   
	//UARTx_RX	  
	GPIO_InitStructure.GPIO_Pin = RX_Pin[COMx];//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(RX_Port[COMx], &GPIO_InitStructure);	

	//NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = UARTx_IRQn[COMx];
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 1 ;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 		//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure); //����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = BaudRate;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ

	USART_Init(UARTx[COMx], &USART_InitStructure); //��ʼ������
	USART_ITConfig(UARTx[COMx], USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_Cmd(UARTx[COMx], ENABLE); 				   //ʹ�ܴ���

	if(COMx == RS485PORT1)
	{
		RS485EN1Init();
		RS485RE1 = 0;
	}
	if(COMx == RS485PORT3)
	{
		RS485EN3Init();
		RS485RE3 = 0;
	}

	Uart[COMx].rxCount = 0;
	Uart[COMx].buffSize = BuffSize;
	Uart[COMx].buff		= RxBuff;
}

void UART_Send(COM com,u8* data, u16 size)
{
	if(com == RS485PORT1)
	{
		RS485RE1 = 1; //485����ʹ��
	}
	else if(com == RS485PORT3)
	{
		RS485RE3 = 1; //485����ʹ��
	}
	
	while(USART_GetFlagStatus(UARTx[com], USART_FLAG_TC) == RESET);
	
	while(size--)
	{
		USART_SendData(UARTx[com], (u16)(*data++));
		while(USART_GetFlagStatus(UARTx[com], USART_FLAG_TC) == RESET);
	}
	
	if(com == RS485PORT1)
	{
		RS485RE1 = 0; //485����ʹ��
	}
	else if(com == RS485PORT3)
	{
		RS485RE3 = 0; //485����ʹ��
	}
}

static void UART1_CommunicateHandler(void)
{
	uint16 crc = MBCRC16(Uart[COM1].buff, Uart[COM1].rxCount - 2);
	if(crc == ((Uart[COM1].buff[Uart[COM1].rxCount - 1] << 8) | Uart[COM1].buff[Uart[COM1].rxCount - 2]))
	{
		ModbusHandler(COM1,Uart[COM1].buff);
	}
}

 static void UART2_CommunicateHandler(void)
{
	
}


static void UART3_CommunicateHandler(void)
{
	uint16 crc = MBCRC16(Uart[COM3].buff, Uart[COM3].rxCount - 2);
	if(crc == ((Uart[COM3].buff[Uart[COM3].rxCount - 1] << 8) | Uart[COM3].buff[Uart[COM3].rxCount - 2]))
	{
		ModbusHandler(COM3,Uart[COM3].buff);
	}
}


void UART_Check(COM com)
{
	u16 check;
	if(Uart[com].rxCount != 0)
	{
		check = Uart[com].rxCount;
		rt_thread_mdelay(10);
		if(check == Uart[com].rxCount)
		{
			switch(com)
			{
				case COM1:
					UART1_CommunicateHandler();
					break;
				case COM2:
					UART2_CommunicateHandler();
					break;
				case COM3:
					UART3_CommunicateHandler();
					break;
				default:
					break;
			}
			memset((char*)Uart[com].buff, 0x00, Uart[com].rxCount);
			Uart[com].rxCount = 0;
		}
	}
}




void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�
	{
		//������յ�������
		if(Uart[COM1].rxCount < Uart[COM1].buffSize - 1)
		{
			Uart[COM1].buff[Uart[COM1].rxCount++] = USART_ReceiveData(USART1);
		}
		else
		{
			USART_ReceiveData(USART1);
		}
     } 
} 



void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�
	{
		//������յ�������
		if(Uart[COM3].rxCount < Uart[COM3].buffSize - 1)
		{
			Uart[COM3].buff[Uart[COM3].rxCount++] = USART_ReceiveData(USART3);
		}
		else
		{
			USART_ReceiveData(USART3);
		}
     } 
}


//==�ñ����������ƿ��Ź�����ֵ�������߳��е�������mqtt���պ����Ǳ����㣨������ÿ5s����һ����������������ֵ����200ʱ������10s������Ϊ��ǰ����ͨ�Ų���
unsigned short connet_falg_count = 0;
extern unsigned char connet_flag;//���ӱ�־λ���ж���������״̬�Ƿ�����
unsigned char broken_connet_pos = 0;//��¼����֮ǰ������־��Ϣ��λ�ã��ڵڼ���
extern u8 log_back_pos;//��¼����������־�±�λ��
extern unsigned char pub_test_flag;//�����÷�����־

void serial_com_check_thread_entry(void* parameter)
{
	u8 count = 0;
//	u16 bro_connet_count	= 451;
	
	while(1)
	{
		UART_Check(COM1);
		UART_Check(COM3);
		IWDG_Feed();
		Key_Reset();
		
		if(connet_falg_count < 80)//20������
		{
			connet_flag = 1;
			connet_falg_count++;
			count = 0;
		}
		else
		{
			if(count == 0)
			{
				count = 1;
				broken_connet_pos = log_back_pos;
			}
		
			connet_flag = 0;
			
//			if(bro_connet_count%450 == 0)//������������8�뷢��һ�ζ������ѣ�ֱ���ٴ��յ�������
//			{
//				pub_test_flag = 1;
//			}
//			
//			if(bro_connet_count >= 2000)
//			{
//				bro_connet_count = 451;
//			}
//			else
//			{
//				bro_connet_count++;
//			}
			
		}
		
		rt_thread_mdelay(20);
		rt_thread_yield(); 			/* ����ʣ��ʱ��Ƭ������һ���߳��л� */
	}
}


