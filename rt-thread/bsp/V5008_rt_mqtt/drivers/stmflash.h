#ifndef __FLASH_H
#define __FLASH_H
#include "stdio.h"	
#include "sys.h" 

//////////////////////////////////////////////////////////////////////////////////////////////////////
//用户根据自己的需要设置
#define STM32_FLASH_SIZE 	256 	 		//所选STM32的FLASH容量大小(单位为K)
#define STM_SECTOR_SIZE		2048 			//每页含有字节
//FLASH起始地址
// #define STM32_FLASH_BASE		0x08000000 		// STM32 FLASH的起始地址
// #define SysParameter_Addr		0x0800F000		// 系统参数保存位置
// #define DotParameter_Addr		0x0800F400		// 点阵数据保存位置 


u8 FlashWriteData(u32 WriteAddr,const u8 *pBuffer,u16 NumToWrite);		//从指定地址开始写入指定长度的数据
u8 FlashReadData(u32 ReadAddr,u8 *pBuffer,u16 NumToRead);   		//从指定地址开始读出指定长度的数据
u8 FlashWriteOnly_16(u32 WriteAddr,const u16 *pBuffer,u16 NumToWrite);
u16 STMFLASH_ReadHalfWord(u32 faddr);
u8 FlashPageWrite(u32 WriteAddr,const u8 *pBuffer,u16 NumToWrite,u32 PageAddr);
void char_to_short(u8* buff_old, u16* buff_new, u16 buff_lenth, u16 pos);
void short_to_char(u8* buff_new, u16* buff_old, u16 buff_lenth);

u8 STMFLASH_WriteHalfWord(u32 faddr, u16 dat);
u8 STMFLASH_ErasePage(u32 paddr);
u8 STMFLASH_WaitDone(u16 time);
u8 STMFLASH_GetStatus(void);
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);
u16 STMFLASH_ReadHalfWord(u32 faddr);
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);
void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);

/*************************************************************************************/

#endif




















