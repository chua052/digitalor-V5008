#include "Main.h"
#include "delay.h"
#include "usart.h"
#include <rtthread.h>
#include "stmflash.h"
#include "iwdg.h"
#include "delay.h" 

//////////////////////////////////////////////////////////////////////////////////	 
//STM32 FLASH 驱动代码	   
//修改日期:2013/4/17
//版本：V1.0							  
//////////////////////////////////////////////////////////////////////////////////



//读取指定地址的2个字节
//faddr:读地址(此地址必须为2的倍数!!)
//返回值:对应数据.
u16 STMFLASH_ReadByte(u32 faddr)
{
	return *(u16*)faddr; 

}

// //读取指定地址的4个字节
// //faddr:读地址(此地址必须为2的倍数!!)
// //返回值:对应数据.
// u16 STMFLASH_ReadWord(u32 faddr)
// {
// 	return *(u32*)faddr; 
// }
 

//从指定地址开始写入指定长度的数据,只在同一个页内写入，不保留原有数据，此函数调用时不可写入奇数个字节
//WriteAddr:起始地址(此地址必须为2的倍数!!)
//pBuffer:数据指针
//NumToWrite:半字(16位)数(就是要写入的16位数据的个数.)
u8 FlashWriteData(u32 WriteAddr,const u8 *pBuffer,u16 NumToWrite)	
{
	rt_enter_critical();//锁线程调度器
	
	u32 i;
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+2048*STM32_FLASH_SIZE)))return ERROR;//非法地址
	if(NumToWrite>(STM_SECTOR_SIZE/2))return ERROR;//超出每页个数
	IWDG_Feed();
	FLASH_Unlock();						//解锁
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
//	FLASH_ErasePage(WriteAddr);//擦除这个扇区
	STMFLASH_ErasePage(WriteAddr);
	
	my_delay(20);
	for(i=0; i<NumToWrite; i++)
	{
//		FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
		u32 addr = WriteAddr;
		STMFLASH_WriteHalfWord(addr, pBuffer[i]);
		
//		u16 temp = STMFLASH_ReadByte(WriteAddr);
//		if(STMFLASH_ReadByte(WriteAddr) != pBuffer[i+1])	   //校验
//		{
//			FLASH_Lock();//上锁
//			return 1;
//		}
		WriteAddr+=2;//偏移2个字节.
	}
	FLASH_Lock();//上锁
	
	rt_exit_critical();//解锁调度器
	return 0;
}


//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数
u8 FlashReadData(u32 ReadAddr,u8 *pBuffer,u16 NumToRead)   	
{
//	u8 i;
	rt_enter_critical();//锁线程调度器
		
	u32 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadByte(ReadAddr);//读取2个字节.
		ReadAddr+=2;//偏移2个字节.	
	}
	
	rt_exit_critical();//解锁调度器
	return 0;
}







u16 STMFLASH_ReadHalfWord(u32 faddr)
{
	return *(u16*)faddr; 
}


//从指定地址开始写入指定长度的数据,只在同一个页内写入，不擦除原有数据
//WriteAddr:起始地址(此地址必须为2的倍数!!)
//pBuffer:数据指针
//NumToWrite:半字(16位)数(就是要写入的16位数据的个数.)
u8 FlashWriteOnly_16(u32 WriteAddr,const u16 *pBuffer,u16 NumToWrite)	
{
	u8 i;
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+2048*STM32_FLASH_SIZE)))return ERROR;//非法地址
	if(NumToWrite>(STM_SECTOR_SIZE/2))return ERROR;//超出每页个数
	FLASH_Unlock();						//解锁
	for(i=0; i<NumToWrite; i++)
	{
		FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
		if(STMFLASH_ReadHalfWord(WriteAddr) != pBuffer[i])	   //校验
		{
			FLASH_Lock();//上锁
			return 1;
		}
		WriteAddr+=2;//偏移2个字节.
	}
	FLASH_Lock();//上锁
	return 0;
}


/*
函数功能：适用于V5008的按页flash写函数
函数参数：写入地址，写入数据，写入长度（2的倍数）,该页首地址
函数返回值：0成功，-1失败
备注：无
*/
//u8 FlashPageWrite(u32 WriteAddr,const u8 *pBuffer,u16 NumToWrite,u32 PageAddr)	
//{

//	rt_enter_critical();//锁线程调度器
//	
//	u8 temp_buff[2048] = {0};
//	u16 short_buff[1024] = {0};
//	u16 num = WriteAddr - PageAddr;
////	FlashReadData(PageAddr,temp_buff,num);
//	STMFLASH_Read(PageAddr,short_buff,num);
//	
//	memcpy(&temp_buff[0], &pBuffer[0], NumToWrite);
//	char_to_short(temp_buff, &short_buff[num], NumToWrite, num);
//	
//	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+2048*STM32_FLASH_SIZE)))return ERROR;//非法地址
//	if(NumToWrite>(STM_SECTOR_SIZE/2))return ERROR;//超出每页个数
//	IWDG_Feed();
//	FLASH_Unlock();						//解锁
//	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
////	FLASH_ErasePage(WriteAddr);//擦除这个扇区
//	STMFLASH_ErasePage(PageAddr);
//	
//	my_delay(20);

//	u16 leng = (num+NumToWrite)/2;
//	FlashWriteData(PageAddr,short_buff,leng);
//	
//	FLASH_Lock();//上锁
//	
//	rt_exit_critical();//解锁调度器
//	return 0;

//}

/****
函数功能：数据格式转换，将u8的数组转换成u16的数组
*******/
void char_to_short(u8* buff_old, u16* buff_new, u16 buff_lenth, u16 pos)
{
	u16 i = 0;
	for(i=0; i<buff_lenth; i+=2)
	{
		buff_new[pos] |= (u16)buff_old[i];
		buff_new[pos] |= ((u16)buff_old[i+1]) << 8;
		pos++;
	}
}

/****
函数功能：数据格式转换，将u16的数组转换成u8的数组
*******/
void short_to_char(u8* buff_new, u16* buff_old, u16 buff_lenth)
{
	u16 i = 0,j=0;
	for(i=0; i<buff_lenth; i+=2)
	{
		buff_new[i] |= (u8)buff_old[j];
		buff_new[i+1] |= (u8)(buff_old[j] >> 8);
		j++;
	}
}


//===


//读取FLASH状态寄存器，查看状态，返回值为状态
u8 STMFLASH_GetStatus(void)
{
	u32 res;

	res=FLASH->SR; 

	if(res&(1<<0))return 1;    //忙碌则返回1
	else if(res&(1<<2))return 2; //编程错误返回2
	else if(res&(1<<4))return 3; //写保护错误返回3

	return 0; //非忙碌状态且无任何错误时，返回0，或者说可以读写操作的话返回0
}

//等待操作完成函数
u8 STMFLASH_WaitDone(u16 time)
{
	u8 res;

	do
	{
		res=STMFLASH_GetStatus();//读取FLASH状态是否处于忙碌状态
		if(res!=1)break;//非忙碌状态则break
		delay_us(1);
		time--;
	}while(time);
	
	if(time==0)res=0xff;//TIMEOUT超时了，res等于0xff
	
	return res;//操作完成返回0
}

//擦除页
u8 STMFLASH_ErasePage(u32 paddr)
{
	u8 res=0;
	res=STMFLASH_WaitDone(0X5FFF);// 首先查看是否是忙碌状态，

	if(res==0)//非忙碌状态可以操作
	{ 
		FLASH->CR|=1<<1;//选择页擦除
		FLASH->AR=paddr;//擦除地址
		FLASH->CR|=1<<6;//  擦除操作 
		res=STMFLASH_WaitDone(0X5FFF);//是否操作完成

		if(res!=1)//
		{
			FLASH->CR&=~(1<<1);//完成后，页擦除选择位清零
		}
	}

	return res;
}

//在指定位置写入半字
u8 STMFLASH_WriteHalfWord(u32 faddr, u16 dat)
{
	u8 res;      
	res=STMFLASH_WaitDone(0XFF);  

	if(res==0)//OK，非忙碌状态下进入
	{
		FLASH->CR|=1<<0;//设置写操作
		*(vu16*)faddr=dat;//向地址中写入数据
		res=STMFLASH_WaitDone(0XFF);//等待操作完成
		if(res!=1)//
		{
			FLASH->CR&=~(1<<0);//清零写操作位
		}
	} 

	return res;
}

//从指定的地址读指定长度的数据
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)
{
	u16 i;

	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//读取改地址数据
		ReadAddr+=2;// 一次读取两字节的数据，因此地址要相应的+2
	}

}

//不检查的写入
//WriteAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数   
void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
{ 			 		 
	u16 i;
	for(i=0;i<NumToWrite;i++)
	{
		STMFLASH_WriteHalfWord(WriteAddr,pBuffer[i]);
	    WriteAddr+=2;//地址增加2.
	}  
} 


//从指定地址开始写入指定长度的数据
//WriteAddr:起始地址(此地址必须为2的倍数!!)
//pBuffer:数据指针
//NumToWrite:半字(16位)数(就是要写入的16位数据的个数.)
#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //字节
#else 
#define STM_SECTOR_SIZE	2048
#endif		 
u16 STMFLASH_BUF[STM_SECTOR_SIZE/2];//最多是2K字节
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)	
{
	u32 secpos;	   //扇区地址
	u16 secoff;	   //扇区内偏移地址(16位字计算)
	u16 secremain; //扇区内剩余地址(16位字计算)	   
 	u16 i;    
	u32 offaddr;   //去掉0X08000000后的地址
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))return;//非法地址
	FLASH_Unlock();						//解锁
	offaddr=WriteAddr-STM32_FLASH_BASE;		//实际偏移地址.
	secpos=offaddr/STM_SECTOR_SIZE;			//扇区地址  0~127 for STM32F103RBT6
	secoff=(offaddr%STM_SECTOR_SIZE)/2;		//在扇区内的偏移(2个字节为基本单位.)
	secremain=STM_SECTOR_SIZE/2-secoff;		//扇区剩余空间大小   
	if(NumToWrite<=secremain)secremain=NumToWrite;//不大于该扇区范围
	while(1) 
	{	
		STMFLASH_Read(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//读出整个扇区的内容
		for(i=0;i<secremain;i++)	//校验数据
		{
			if(STMFLASH_BUF[secoff+i]!=0XFFFF)break;//需要擦除  	  
		}
		if(i<secremain)				//需要擦除
		{
			STMFLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);//擦除这个扇区
			for(i=0;i<secremain;i++)//复制
			{
				STMFLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//写入整个扇区  
		}else STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   
		if(NumToWrite==secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++;				//扇区地址增1
			secoff=0;				//偏移位置为0 	 
		   	pBuffer+=secremain;  	//指针偏移
			WriteAddr+=secremain*2;	//写地址偏移(16位数据地址,需要*2)	   
		   	NumToWrite-=secremain;	//字节(16位)数递减
			if(NumToWrite>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2;//下一个扇区还是写不完
			else secremain=NumToWrite;//下一个扇区可以写完了
		}	 
	};	
	FLASH_Lock();//上锁
}

