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
	enFNC		mbfnc;		//modbus功能码
	uint8 		dev_addr;	//模块地址
	uint16 		start_addr;	//其实寄存器地址
	uint16		reg_nums;   //寄存器数量
	uint8*		data;		//数据指针		
}stModbus;


typedef struct
{
	uint8 	fnc;		//功能
	uint8   dev_addr;	//设备地址
	uint16	addr;		//地址
	uint16  len;		//长度
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


