#include "Main.h"
#include "delay.h"
#include "usart.h"
#include <rtthread.h>
#include "stmflash.h"
#include "iwdg.h"
#include "delay.h" 

//////////////////////////////////////////////////////////////////////////////////	 
//STM32 FLASH ��������	   
//�޸�����:2013/4/17
//�汾��V1.0							  
//////////////////////////////////////////////////////////////////////////////////



//��ȡָ����ַ��2���ֽ�
//faddr:����ַ(�˵�ַ����Ϊ2�ı���!!)
//����ֵ:��Ӧ����.
u16 STMFLASH_ReadByte(u32 faddr)
{
	return *(u16*)faddr; 

}

// //��ȡָ����ַ��4���ֽ�
// //faddr:����ַ(�˵�ַ����Ϊ2�ı���!!)
// //����ֵ:��Ӧ����.
// u16 STMFLASH_ReadWord(u32 faddr)
// {
// 	return *(u32*)faddr; 
// }
 

//��ָ����ַ��ʼд��ָ�����ȵ�����,ֻ��ͬһ��ҳ��д�룬������ԭ�����ݣ��˺�������ʱ����д���������ֽ�
//WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)
//pBuffer:����ָ��
//NumToWrite:����(16λ)��(����Ҫд���16λ���ݵĸ���.)
u8 FlashWriteData(u32 WriteAddr,const u8 *pBuffer,u16 NumToWrite)	
{
	rt_enter_critical();//���̵߳�����
	
	u32 i;
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+2048*STM32_FLASH_SIZE)))return ERROR;//�Ƿ���ַ
	if(NumToWrite>(STM_SECTOR_SIZE/2))return ERROR;//����ÿҳ����
	IWDG_Feed();
	FLASH_Unlock();						//����
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
//	FLASH_ErasePage(WriteAddr);//�����������
	STMFLASH_ErasePage(WriteAddr);
	
	my_delay(20);
	for(i=0; i<NumToWrite; i++)
	{
//		FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
		u32 addr = WriteAddr;
		STMFLASH_WriteHalfWord(addr, pBuffer[i]);
		
//		u16 temp = STMFLASH_ReadByte(WriteAddr);
//		if(STMFLASH_ReadByte(WriteAddr) != pBuffer[i+1])	   //У��
//		{
//			FLASH_Lock();//����
//			return 1;
//		}
		WriteAddr+=2;//ƫ��2���ֽ�.
	}
	FLASH_Lock();//����
	
	rt_exit_critical();//����������
	return 0;
}


//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��
u8 FlashReadData(u32 ReadAddr,u8 *pBuffer,u16 NumToRead)   	
{
//	u8 i;
	rt_enter_critical();//���̵߳�����
		
	u32 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadByte(ReadAddr);//��ȡ2���ֽ�.
		ReadAddr+=2;//ƫ��2���ֽ�.	
	}
	
	rt_exit_critical();//����������
	return 0;
}







u16 STMFLASH_ReadHalfWord(u32 faddr)
{
	return *(u16*)faddr; 
}


//��ָ����ַ��ʼд��ָ�����ȵ�����,ֻ��ͬһ��ҳ��д�룬������ԭ������
//WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)
//pBuffer:����ָ��
//NumToWrite:����(16λ)��(����Ҫд���16λ���ݵĸ���.)
u8 FlashWriteOnly_16(u32 WriteAddr,const u16 *pBuffer,u16 NumToWrite)	
{
	u8 i;
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+2048*STM32_FLASH_SIZE)))return ERROR;//�Ƿ���ַ
	if(NumToWrite>(STM_SECTOR_SIZE/2))return ERROR;//����ÿҳ����
	FLASH_Unlock();						//����
	for(i=0; i<NumToWrite; i++)
	{
		FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
		if(STMFLASH_ReadHalfWord(WriteAddr) != pBuffer[i])	   //У��
		{
			FLASH_Lock();//����
			return 1;
		}
		WriteAddr+=2;//ƫ��2���ֽ�.
	}
	FLASH_Lock();//����
	return 0;
}


/*
�������ܣ�������V5008�İ�ҳflashд����
����������д���ַ��д�����ݣ�д�볤�ȣ�2�ı�����,��ҳ�׵�ַ
��������ֵ��0�ɹ���-1ʧ��
��ע����
*/
//u8 FlashPageWrite(u32 WriteAddr,const u8 *pBuffer,u16 NumToWrite,u32 PageAddr)	
//{

//	rt_enter_critical();//���̵߳�����
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
//	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+2048*STM32_FLASH_SIZE)))return ERROR;//�Ƿ���ַ
//	if(NumToWrite>(STM_SECTOR_SIZE/2))return ERROR;//����ÿҳ����
//	IWDG_Feed();
//	FLASH_Unlock();						//����
//	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
////	FLASH_ErasePage(WriteAddr);//�����������
//	STMFLASH_ErasePage(PageAddr);
//	
//	my_delay(20);

//	u16 leng = (num+NumToWrite)/2;
//	FlashWriteData(PageAddr,short_buff,leng);
//	
//	FLASH_Lock();//����
//	
//	rt_exit_critical();//����������
//	return 0;

//}

/****
�������ܣ����ݸ�ʽת������u8������ת����u16������
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
�������ܣ����ݸ�ʽת������u16������ת����u8������
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


//��ȡFLASH״̬�Ĵ������鿴״̬������ֵΪ״̬
u8 STMFLASH_GetStatus(void)
{
	u32 res;

	res=FLASH->SR; 

	if(res&(1<<0))return 1;    //æµ�򷵻�1
	else if(res&(1<<2))return 2; //��̴��󷵻�2
	else if(res&(1<<4))return 3; //д�������󷵻�3

	return 0; //��æµ״̬�����κδ���ʱ������0������˵���Զ�д�����Ļ�����0
}

//�ȴ�������ɺ���
u8 STMFLASH_WaitDone(u16 time)
{
	u8 res;

	do
	{
		res=STMFLASH_GetStatus();//��ȡFLASH״̬�Ƿ���æµ״̬
		if(res!=1)break;//��æµ״̬��break
		delay_us(1);
		time--;
	}while(time);
	
	if(time==0)res=0xff;//TIMEOUT��ʱ�ˣ�res����0xff
	
	return res;//������ɷ���0
}

//����ҳ
u8 STMFLASH_ErasePage(u32 paddr)
{
	u8 res=0;
	res=STMFLASH_WaitDone(0X5FFF);// ���Ȳ鿴�Ƿ���æµ״̬��

	if(res==0)//��æµ״̬���Բ���
	{ 
		FLASH->CR|=1<<1;//ѡ��ҳ����
		FLASH->AR=paddr;//������ַ
		FLASH->CR|=1<<6;//  �������� 
		res=STMFLASH_WaitDone(0X5FFF);//�Ƿ�������

		if(res!=1)//
		{
			FLASH->CR&=~(1<<1);//��ɺ�ҳ����ѡ��λ����
		}
	}

	return res;
}

//��ָ��λ��д�����
u8 STMFLASH_WriteHalfWord(u32 faddr, u16 dat)
{
	u8 res;      
	res=STMFLASH_WaitDone(0XFF);  

	if(res==0)//OK����æµ״̬�½���
	{
		FLASH->CR|=1<<0;//����д����
		*(vu16*)faddr=dat;//���ַ��д������
		res=STMFLASH_WaitDone(0XFF);//�ȴ��������
		if(res!=1)//
		{
			FLASH->CR&=~(1<<0);//����д����λ
		}
	} 

	return res;
}

//��ָ���ĵ�ַ��ָ�����ȵ�����
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)
{
	u16 i;

	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//��ȡ�ĵ�ַ����
		ReadAddr+=2;// һ�ζ�ȡ���ֽڵ����ݣ���˵�ַҪ��Ӧ��+2
	}

}

//������д��
//WriteAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��   
void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
{ 			 		 
	u16 i;
	for(i=0;i<NumToWrite;i++)
	{
		STMFLASH_WriteHalfWord(WriteAddr,pBuffer[i]);
	    WriteAddr+=2;//��ַ����2.
	}  
} 


//��ָ����ַ��ʼд��ָ�����ȵ�����
//WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)
//pBuffer:����ָ��
//NumToWrite:����(16λ)��(����Ҫд���16λ���ݵĸ���.)
#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //�ֽ�
#else 
#define STM_SECTOR_SIZE	2048
#endif		 
u16 STMFLASH_BUF[STM_SECTOR_SIZE/2];//�����2K�ֽ�
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)	
{
	u32 secpos;	   //������ַ
	u16 secoff;	   //������ƫ�Ƶ�ַ(16λ�ּ���)
	u16 secremain; //������ʣ���ַ(16λ�ּ���)	   
 	u16 i;    
	u32 offaddr;   //ȥ��0X08000000��ĵ�ַ
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))return;//�Ƿ���ַ
	FLASH_Unlock();						//����
	offaddr=WriteAddr-STM32_FLASH_BASE;		//ʵ��ƫ�Ƶ�ַ.
	secpos=offaddr/STM_SECTOR_SIZE;			//������ַ  0~127 for STM32F103RBT6
	secoff=(offaddr%STM_SECTOR_SIZE)/2;		//�������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
	secremain=STM_SECTOR_SIZE/2-secoff;		//����ʣ��ռ��С   
	if(NumToWrite<=secremain)secremain=NumToWrite;//�����ڸ�������Χ
	while(1) 
	{	
		STMFLASH_Read(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//������������������
		for(i=0;i<secremain;i++)	//У������
		{
			if(STMFLASH_BUF[secoff+i]!=0XFFFF)break;//��Ҫ����  	  
		}
		if(i<secremain)				//��Ҫ����
		{
			STMFLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);//�����������
			for(i=0;i<secremain;i++)//����
			{
				STMFLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//д����������  
		}else STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;				//������ַ��1
			secoff=0;				//ƫ��λ��Ϊ0 	 
		   	pBuffer+=secremain;  	//ָ��ƫ��
			WriteAddr+=secremain*2;	//д��ַƫ��(16λ���ݵ�ַ,��Ҫ*2)	   
		   	NumToWrite-=secremain;	//�ֽ�(16λ)���ݼ�
			if(NumToWrite>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2;//��һ����������д����
			else secremain=NumToWrite;//��һ����������д����
		}	 
	};	
	FLASH_Lock();//����
}

