#include "sys.h"
#include "global.h"
#include "main.h"
#include "w5500.h"
#include "delay.h"
#include "stmflash.h"


void my_memcpy(void * dest, const void * src, int u, int n)
{
	int i,j;
	char *d = dest;
    const char*s = src;
	for(i = 0; i < n / u; i++)
	{
		for(j = 0; j < u; j++)
		{
			d[i*u+j] = s[i*u+(u-j-1)];
		}
	}
}

/*********************************************************************
����:	�ϵ���FLASH,������FLASHֵ
*********************************************************************/
int FlashCheck(void)
{
	UCHAR Temp_buf[4];
	
	//��ȡ���õ�IP��ַ������  ��IPΪĬ��IP����δ���ã�����Ҫ��������
	if(0 != FlashReadData(W5500_LOCAL_IP, &Temp_buf[0], 4))
	{								
		
	}
	
	
	return 0;
}


/*********************************************************************
����:   ��ʼ��Flash����,ԭ���� MAC �� ID �Ų��ı�
*********************************************************************/
//void FlashInit(void)
//{
//	UCHAR bFlag;
//	UCHAR n;
//	UCHAR NetPara_temp[NET_PARA_LEN],temp_mac[7],temp_id[20];
//	
//	for(n=0; n<NET_PARA_LEN; n++)
//	{
//		//���StartParameter_buf���ݾ��Ƕ�����������
//		NetPara_temp[n] = StartParameter_buf[n];	//����Ĭ��ֵ,���Ƶ���ʱ����  ��������150�ֽ�
//	}
//	
//	//====����stm32оƬ��ID����ΪMAC��ַ��һ���֣��Ա��������Ų����������ɲ�ͬMAC��ַ	
//	NetPara_temp[53] = (UCHAR)(Sys_SerialNum>>16);
//	if(NetPara_temp[53]==0)NetPara_temp[53] +=0x12;
//	NetPara_temp[54] = (UCHAR)(Sys_SerialNum>>8);
//	if(NetPara_temp[54]==0)NetPara_temp[54] +=0x34;
//	NetPara_temp[55] = (UCHAR)(Sys_SerialNum);
//	if(NetPara_temp[55]==0)NetPara_temp[55] +=0x56;
//	
//	
//													//���� STM32 FLASH �ڲ� MAC ��ַ
//	if(0 != FlashReadData(NetParameter_Addr+50*2, &temp_mac[0], 6))	
//	if(0 != FlashReadData(NetParameter_BakAddr+50*2,&temp_mac[0],6))	bFlag = TRUE;
//	if(((temp_mac[0]!=0xFF)&&(temp_mac[0]!=0x00))||((temp_mac[1]!=0xFF)&&(temp_mac[1]!=0x00))
//	 ||((temp_mac[2]!=0xFF)&&(temp_mac[2]!=0x00))||((temp_mac[3]!=0xFF)&&(temp_mac[3]!=0x00))
//	 ||((temp_mac[4]!=0xFF)&&(temp_mac[4]!=0x00))||((temp_mac[5]!=0xFF)&&(temp_mac[5]!=0x00)))
//	{
//		NetPara_temp[50] = temp_mac[0];				//STM32 FLASH ���� MAC ����
//		NetPara_temp[51] = temp_mac[1];				//����ԭ���� MAC ����
//		NetPara_temp[52] = temp_mac[2];
//		NetPara_temp[53] = temp_mac[3];
//		NetPara_temp[54] = temp_mac[4];
//		NetPara_temp[55] = temp_mac[5];
//	}
//	if(0 != FlashReadData(NetParameter_Addr+77*2, &temp_id[0], 10))	
//	if(0 != FlashReadData(NetParameter_BakAddr+77*2,&temp_id[0],10))	bFlag = TRUE;
//	for(n=0; n<10; n++)
//	{
//		NetPara_temp[77+n] = temp_id[n];			//����ԭ�е�ID�Ų���
//	}
//													//����Ĭ��ֵ��д��FLASH(ԭMAC����)

//	if(0 != FlashWriteData(FLASH_INIT_FLAG_ADDR,StartFlag_buf, FLASH_FLAG_NUM			))	bFlag = TRUE;//��ʼ����־λ    
//	if(0 != FlashWriteData(NetParameter_Addr,   NetPara_temp,	sizeof(Setup_Parameter)	))	bFlag = TRUE;//��ʼ�����ڲ��� 
//	if(0 != FlashWriteData(NetParameter_BakAddr,NetPara_temp,	sizeof(Setup_Parameter)	))	bFlag = TRUE;//��ʼ�����ڲ���������
//	
//	if(bFlag)
//	{
//		//LED1=0;
//		ResetSystem();
//	}
//}


//void ResetSystem(void)
//{
//	NVIC_SystemReset();
//}



/*********************************************************************
����:	��ʼ���������
����:   ϵͳ�������߲����ı�ʱ 
���:   ��IP,PORT�Ȳ�����Flash�ж�����������
*********************************************************************/
void InitPara(void)
{
    memset(&bc_mac[0], 0xFF, MAC_LEN);								// �㲥 MAC
	memcpy(&my_mac[0], &NetParameter.MAC_Local_buf[0], 	MAC_LEN);	// ���� MAC
	memcpy(&my_ips[0], &NetParameter.IP_Local_buf[0],  	IP_LEN);	// ���� IP
	memcpy(&sb_net[0], &NetParameter.Sub_Mask_buf[0],  	IP_LEN);	// �������� 
	memcpy(&gw_ips[0], &NetParameter.Gate_Way_buf[0],	IP_LEN);	// Ĭ������
	memcpy(&rm_ips[0], &NetParameter.IP_Remote_buf[0],	IP_LEN);	// Զ�� IP
	remote_port = Big2Little16(NetParameter.Port_Remote);			// Զ�̶˿�
	self_port   = Big2Little16(NetParameter.Port_Local);			// �����˿�
	my_ipaddr = 0;	
	my_subnet = 0;	
	gw_ipaddr = 0;	
	remote_ip = 0;	
	my_ipaddr = my_ipaddr<<8 | my_ips[0];	my_ipaddr = my_ipaddr<<8 | my_ips[1];
	my_ipaddr = my_ipaddr<<8 | my_ips[2];	my_ipaddr = my_ipaddr<<8 | my_ips[3];
	my_subnet = my_subnet<<8 | sb_net[0];	my_subnet = my_subnet<<8 | sb_net[1];
	my_subnet = my_subnet<<8 | sb_net[2];	my_subnet = my_subnet<<8 | sb_net[3];
	gw_ipaddr = gw_ipaddr<<8 | gw_ips[0];	gw_ipaddr = gw_ipaddr<<8 | gw_ips[1];
	gw_ipaddr = gw_ipaddr<<8 | gw_ips[2];	gw_ipaddr = gw_ipaddr<<8 | gw_ips[3];
	remote_ip = remote_ip<<8 | rm_ips[0];	remote_ip = remote_ip<<8 | rm_ips[1];
	remote_ip = remote_ip<<8 | rm_ips[2];	remote_ip = remote_ip<<8 | rm_ips[3];
}

void JATG_SWD_Close(void)
{
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO,  ENABLE);	
	//JTAG-DP Disabled and SW-DP Disabled
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);  //Full SWJ Disabled (JTAG-DP + SW-DP)
}

void JATG_Close(void)
{
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO,  ENABLE);	
	//JTAG-DP Disabled and SW-DP Enabled
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);  //
}

uint32_t CrcGen_STM32(uint32_t *data, uint32_t size)
{
    uint32_t i;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC,ENABLE);
	CRC->CR = CRC_CR_RESET;
	for(i=0;i<size;i++)
	{
		CRC->DR = data[i];
	}
	
    return CRC->DR;
}

/*
�������ܣ���ȡоƬID��
�������������оƬID������
��������ֵ����
��ע����
*/
uint32_t Get_Device_Id(void)
{
	uint32_t Device_Serial[3];
	uint32_t Crc_Serial=0;
	
	Device_Serial[0] = *(uint32_t *)(0X1FFFF7E8);  
	Device_Serial[1] = *(uint32_t *)(0X1FFFF7EC);
	Device_Serial[2] = *(uint32_t *)(0X1FFFF7F0);

	if(Device_Serial[0] != 0)
	{
		Crc_Serial = CrcGen_STM32(Device_Serial,3);
	}
	
	return Crc_Serial;
}

////��λ������ťʹ�ܶ�
//void RESET_GPIO_Init(void)
//{
//	GPIO_InitTypeDef  GPIO_InitStructure;
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
//	GPIO_InitStructure.GPIO_Pin 	=  RST_NET_IO;
//	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IPU; 	    //��������
//	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_10MHz;		//IO���ٶ�Ϊ10MHz
//	GPIO_Init(GPIOA, &GPIO_InitStructure);					    //�����趨������ʼ��
//}


void  Maindoorbsp(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
	GPIO_InitStructure.GPIO_Pin 	=  GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING; 	    //��������
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_10MHz;		//IO���ٶ�Ϊ10MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					    //�����趨������ʼ��
}


unsigned int DoorCount;
unsigned char door_sta;


void MainDoorHandler(void)
{
  
	if(DoorCount >= 40)
	{
	   DoorCount = 0;
		 door_sta = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1);
		 if(door_sta == DOOROPEN)
		 {
			 delay_us(20);
		   door_sta = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1);
		 }
	}
}

void system_initial(void)
{			
	EN485_1=0;
//	EN485_2=0;
	EN485_3=0;
	memset(SenFlag,0x00,6);
	memset(RdFlag,0x00,6);
	memset(UpFlashFg,0x00,6);
	memset(DnFlashFg,0x01,6);//20140811
	memset(FlashVal,0x00,6);
	memset(DyFlashPos,0x00,6);
	memset(DyFlashClr,0x00,6);

		
	UW_Cnt = Sys_UW_Count;			//Ĭ����6Uλ

	
//	Sys_MK_Cnt_New = SlaveCnt+1;	//ģ������  //20150305
	
// 	Sys_MK_Cnt_New = 1;	//ģ������(������λ��Ϊ1)  //20150305	

//�ϵ�ʱ������ģ��������Ϊ9���Ա���������ѯ������Ϣ�����������ʵ��ģ�������¸�ֵ	
//Ҳ���ڹ۲��ϵ�ʱ�������	
	Sys_MK_Cnt_New = 9;	//ģ������(������λ��Ϊ9)  //20160805	
	
	Sys_MK_Cnt_Old = Sys_MK_Cnt_New;
	Slave_Addr = 0x01;				//0x01:����,0xFF:�ӻ�
	Host_Addr = 0x01;				//����Ĭ�ϵ�ַ
	
	memset(Sys_UW_Num,0x01,sizeof(Sys_UW_Num));
	
	GetDat_Pos = 0;
	Sys_SerialNum = Get_Device_Id();
	Ud_UW_Updata = 0x0002;			//Uλ���ݱ仯���룬�ϵ�λ2
}


