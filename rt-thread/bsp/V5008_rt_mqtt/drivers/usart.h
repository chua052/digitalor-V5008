/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 */

#ifndef __USART_H__
#define __USART_H__

#include <rthw.h>
#include <rtthread.h>
#include "stm32f10x.h"
#include <stdio.h>
#include "types.h"

#define UART_ENABLE_IRQ(n)            NVIC_EnableIRQ((n))
#define UART_DISABLE_IRQ(n)           NVIC_DisableIRQ((n))

void rt_hw_usart_init(void);

//===================
#define USART_REC_LEN  		200  							// ���崮�ڽ��ջ����С
#define EN_USART1_RX 			1									// ʹ�ܴ���1���գ�0Ϊ��ֹ
  	
extern u8  USART_RX_BUF[USART_REC_LEN]; 		// �������ڽ��ջ��� 
extern u16 USART_RX_STA;         						// �������ڽ���״̬��־λ

//void USART1_Init(void);
int fputc(int ch, FILE *f);
void USART1_Putc(unsigned char c);
void USART1_Puts(char * str);
void USART1_IRQHandler(void);

//============

#define RS485_HIGH  GPIO_SetBits(GPIOA,GPIO_Pin_8)
#define RS485_LOW  GPIO_ResetBits(GPIOA,GPIO_Pin_8)

#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����

#define SEND_MAX	 		1600
#define RECV_MAX	 		1600
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���

void load_data(u8 *base, u16 size);
void uart1_init(u32 bound);


#endif
