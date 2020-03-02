#ifndef SERIAL_COM_H
#define SERIAL_COM_H
#include "sys.h"
#include "main.h"
#define RS485PORT1	   COM1
#define RS485RE1	   PAout(8)
#define RS485PORTREIO1  GPIOA
#define RS485PORTREPIN1 GPIO_Pin_8

#define RS485PORT3	   COM3
#define RS485RE3	   PBout(12)
#define RS485PORTREIO3  GPIOB
#define RS485PORTREPIN3 GPIO_Pin_12


typedef enum{
		COM1 = 0,
		COM2,
		COM3,
		COM4,
		COM5
	}COM;

typedef struct{
	u16 rxCount;
	u16 buffSize;
	u8* buff;
}stUart;

void UARTConfig(COM COMx, u32 BaudRate, uint8* RxBuff, uint16 BuffSize);
void UART_Send(COM com,u8* data, u16 size);

#endif

