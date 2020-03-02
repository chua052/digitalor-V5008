#ifndef __FLASH_H
#define __FLASH_H
#include "stdio.h"	
#include "sys.h" 

//////////////////////////////////////////////////////////////////////////////////////////////////////
//�û������Լ�����Ҫ����
#define STM32_FLASH_SIZE 	256 	 		//��ѡSTM32��FLASH������С(��λΪK)
#define STM_SECTOR_SIZE		2048 			//ÿҳ�����ֽ�
//FLASH��ʼ��ַ
// #define STM32_FLASH_BASE		0x08000000 		// STM32 FLASH����ʼ��ַ
// #define SysParameter_Addr		0x0800F000		// ϵͳ��������λ��
// #define DotParameter_Addr		0x0800F400		// �������ݱ���λ�� 


u8 FlashWriteData(u32 WriteAddr,const u8 *pBuffer,u16 NumToWrite);		//��ָ����ַ��ʼд��ָ�����ȵ�����
u8 FlashReadData(u32 ReadAddr,u8 *pBuffer,u16 NumToRead);   		//��ָ����ַ��ʼ����ָ�����ȵ�����
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




















