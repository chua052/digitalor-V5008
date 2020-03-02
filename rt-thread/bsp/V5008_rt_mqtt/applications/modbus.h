#ifndef MODBUS_H
#define MODBUS_H
#include "main.h"
#include "serial_com.h"

#define PROTOCOL 1   // 

typedef enum
{
	FUN_01 = (1 << 0),
	FUN_02 = (1 << 1),
	FUN_03 = (1 << 2),
	FUN_04 = (1 << 3),
	FUN_06 = (1 << 4),
	FUN_10 = (1 << 5)
}enFNC;


typedef struct
{	
	enFNC		mbfnc;		//modbus������
	uint8 		dev_addr;	//ģ���ַ
	uint16 		start_addr;	//��ʵ�Ĵ�����ַ
	uint16		reg_nums;   //�Ĵ�������
	uint8*		data;		//����ָ��		
}stModbus;


typedef struct
{
	uint8 	fnc;		//����
	uint8   dev_addr;	//�豸��ַ
	uint16	addr;		//��ַ
	uint16  len;		//����
}stModbusSync;


typedef struct
{
	uint16 startReg;
	uint16 regLen;
	uint8  sem_wait_time;
}stHoldingReg;


USHORT MBCRC16( UCHAR * pucFrame, USHORT usLen);
USHORT MBCRC16( UCHAR * pucFrame, USHORT usLen);
void MBFuncReadInputRegister(COM com, uint8 dev_addr, uint16 start_addr, uint16 len);
void MBFuncWriteMultipleHoldingRegister(COM com, uint8 dev_addr, uint16 start_addr, uint16 len, uint8* data);
void MBFuncWriteSingleHoldingRegister(COM com, uint8 dev_addr, uint16 start_addr, uint8* data);
void MBFuncReadHoldingRegister(COM com, uint8 dev_addr, uint16 start_addr, uint16 len);
void ModbusHandler(COM com, uint8* fram);

#endif


