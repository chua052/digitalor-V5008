#include "timer.h"
#include "iwdg.h"
#include "Main.h"
#include "mini_beep.h"
#include "run_led.h"
#include "mini_driver485.h"
#include "rtc.h"

extern unsigned char  USE_OPEN_RUNLED;
extern struct  MINIBEEP   miniBeep;	
extern	struct RUNLED     runLed;

UINT  RandCnt;			//随机数产生种子序列



extern unsigned char led_off ;
extern unsigned char led_r ;
extern unsigned char led_v ;
extern unsigned char led_ye ;
extern unsigned char led_g ;
extern unsigned char led_yo ;
extern unsigned char led_b ;
extern unsigned char led_w ;



extern unsigned char Computer_LED_FLAG1[54];
extern unsigned char Computer_LED_FLAG2[54];
extern unsigned char Computer_LED_FLAG3[54];
extern unsigned char Computer_LED_FLAG4[54];
extern unsigned char Computer_LED_FLAG5[54];
extern unsigned char Computer_LED_FLAG6[54];
extern unsigned char Computer_LED_FLAG7[54];
extern unsigned char Computer_LED_FLAG8[54];
extern unsigned char Computer_LED_FLAG9[54];
extern unsigned char Computer_LED_FLAG10[54];


//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

  TIM_Cmd(TIM3, ENABLE);  //使能TIMx					 
}


extern   struct  		MINIDRIVER485  	mini_driv485;
extern unsigned char RS485_ForTH_Ok ;
extern signed int    RS485_ForTH_Ct ;
extern int         RS485_ForTH_Sflag ;



extern unsigned char RS485_ForUW_Ok ;
extern signed int    RS485_ForUW_Ct ;
extern int         RS485_ForUW_Sflag ;

extern unsigned  int   TemHandlerCout ;


extern uint16_t CRC_Compute(uint8_t *ptr, uint16_t len);
extern volatile unsigned char  		RX1_BUFF[50];//接收缓冲区2048字节
extern unsigned char 		RX1_CNT;//接收计数器

extern  unsigned char  		RX3_BUFF[300];//接收缓冲区2048字节
extern unsigned char 		RX3_CNT;//接收计数器

unsigned char W_H[36];	// 温湿度
/*******************************************************************************************************************
函数名:
功能：
入口参数:
返回值:
作者:
日期:
版本:
说明：    
************************************************************************************************************************/
void CopyWHData()
{
	unsigned char   addr = RX1_BUFF[0] - 2;
	if(addr <6)
	{
			
		W_H[addr*6] = RX1_BUFF[3];
		W_H[addr*6+1] = RX1_BUFF[4];
		W_H[addr*6+2] = RX1_BUFF[5];
		W_H[addr*6+3] = RX1_BUFF[7];
		W_H[addr*6+4] = RX1_BUFF[8];
		W_H[addr*6+5] = RX1_BUFF[9];
    delay_us(1);
	}
	
}
void RecokRx1BusWork()
{
		unsigned char pos = 0;
	  u16 recCRC,calCRC;
	 
    calCRC=CRC_Compute((uint8_t*)RX1_BUFF,RX1_CNT-2);//计算所接收数据的CRC
	  pos = RX1_CNT-1;
		recCRC = RX1_BUFF[pos];
	  recCRC = recCRC << 8;
		pos = RX1_CNT-2;
	  recCRC |= RX1_BUFF[pos];//接收到的CRC(低字节在前，高字节在后)
		if(calCRC==recCRC )//CRC校验正确
		{
			
			CopyWHData();
			delay_us(1);
		}
		else
		{
//		   delay_us(1);
	  }
    RX1_CNT = 0;  
}


unsigned char RX3FirstROK;






extern unsigned char UW_NUM1;
extern unsigned char UW_NUM2;
extern unsigned char UW_NUM3;
extern unsigned char UW_NUM4;
extern unsigned char UW_NUM5;
extern unsigned char UW_NUM6;
extern unsigned char UW_NUM7;
extern unsigned char UW_NUM8;
extern unsigned char UW_NUM9;
extern unsigned char UW_NUM10;



extern unsigned char UW_Index1Back[54];
extern unsigned char UW_Index2Back[54];
extern unsigned char UW_Index3Back[54];
extern unsigned char UW_Index4Back[54];
extern unsigned char UW_Index5Back[54];
extern unsigned char UW_Index6Back[54];
extern unsigned char UW_Index7Back[54];
extern unsigned char UW_Index8Back[54];
extern unsigned char UW_Index9Back[54];
extern unsigned char UW_Index10Back[54];






extern unsigned char UW_Index1[54];
extern unsigned char UW_Index2[54];
extern unsigned char UW_Index3[54];
extern unsigned char UW_Index4[54];
extern unsigned char UW_Index5[54];
extern unsigned char UW_Index6[54];
extern unsigned char UW_Index7[54];
extern unsigned char UW_Index8[54];
extern unsigned char UW_Index9[54];
extern unsigned char UW_Index10[54];


extern unsigned char UW_NoteIndex1[54];
extern unsigned char UW_NoteIndex2[54];
extern unsigned char UW_NoteIndex3[54];
extern unsigned char UW_NoteIndex4[54];
extern unsigned char UW_NoteIndex5[54];
extern unsigned char UW_NoteIndex6[54];
extern unsigned char UW_NoteIndex7[54];
extern unsigned char UW_NoteIndex8[54];
extern unsigned char UW_NoteIndex9[54];
extern unsigned char UW_NoteIndex10[54];
















extern unsigned char UW_CARDID1[216];
extern unsigned char UW_CARDID2[216];
extern unsigned char UW_CARDID3[216];
extern unsigned char UW_CARDID4[216];
extern unsigned char UW_CARDID5[216];
extern unsigned char UW_CARDID6[216];
extern unsigned char UW_CARDID7[216];
extern unsigned char UW_CARDID8[216];
extern unsigned char UW_CARDID9[216];
extern unsigned char UW_CARDID10[216];

extern unsigned char UW_CARDIDBACK1[216];
extern unsigned char UW_CARDIDBACK2[216];
extern unsigned char UW_CARDIDBACK3[216];
extern unsigned char UW_CARDIDBACK4[216];
extern unsigned char UW_CARDIDBACK5[216];
extern unsigned char UW_CARDIDBACK6[216];
extern unsigned char UW_CARDIDBACK7[216];
extern unsigned char UW_CARDIDBACK8[216];
extern unsigned char UW_CARDIDBACK9[216];
extern unsigned char UW_CARDIDBACK10[216];






extern unsigned char RecdLED1[54];
extern unsigned char RecdLED2[54];
extern unsigned char RecdLED3[54];
extern unsigned char RecdLED4[54];
extern unsigned char RecdLED5[54];
extern unsigned char RecdLED6[54];
extern unsigned char RecdLED7[54];
extern unsigned char RecdLED8[54];
extern unsigned char RecdLED9[54];
extern unsigned char RecdLED10[54];



void Rx3CopyUW_NUM(unsigned char pos ,unsigned int val)
{
   if(pos == 1)
	 {
	   UW_NUM1 = val;
	 }
	 else if(pos == 2)
	 {
	   UW_NUM2 = val;
	 }
	 	 else if(pos == 3)
	 {
	   UW_NUM3 = val;
	 }
	 	 else if(pos == 4)
	 {
	   UW_NUM4 = val;
	 }
	 	 else if(pos == 5)
	 {
	   UW_NUM5 = val;
	 }
	 	 else if(pos == 6)
	 {
	   UW_NUM6 = val;
	 }
	 	 else if(pos == 7)
	 {
	   UW_NUM7 = val;
	 }
	 	 else if(pos == 8)
	 {
	   UW_NUM8 = val;
	 }
	 	 else if(pos == 9)
	 {
	   UW_NUM9 = val;
	 }
	 	 else if(pos == 10)
	 {
	   UW_NUM10= val;
	 }
}
void Rx3CopyUW_Index(unsigned char pos ,unsigned char *p)
{
   if(pos == 1)
	 {
	   memcpy(UW_Index1,p,54);
	 }
	 else if(pos == 2)
	 {
	   memcpy(UW_Index2,p,54);
	 }
	 	 else if(pos == 3)
	 {
	   memcpy(UW_Index3,p,54);
	 }
	 	 else if(pos == 4)
	 {
	   memcpy(UW_Index4,p,54);
	 }
	 	 else if(pos == 5)
	 {
	   memcpy(UW_Index5,p,54);
	 }
	 	 else if(pos == 6)
	 {
	   memcpy(UW_Index6,p,54);
	 }
	 	 else if(pos == 7)
	 {
	   memcpy(UW_Index7,p,54);
	 }
	 	 else if(pos == 8)
	 {
	   memcpy(UW_Index8,p,54);
	 }
	 	 else if(pos == 9)
	 {
	  memcpy(UW_Index9,p,54);
	 }
	 	 else if(pos == 10)
	 {
	   memcpy(UW_Index10,p,54);
	 }

}
void Rx3CopyUW_CARDID(unsigned char pos ,unsigned char *p)
{
	 unsigned char i = 0;
   if(pos == 1)
	 {
		 for( i = 0; i < 54; i++)
		 {
		    if(UW_Index1[i] == 1)
				{
				  memcpy(&UW_CARDID1[i*4],&p[i*4],4);
				}
		 }
	 }
	 else if(pos == 2)
	 {
	   memcpy(UW_CARDID2,p,216);
	 }
	 	 else if(pos == 3)
	 {
	   memcpy(UW_CARDID3,p,216);
	 }
	 	 else if(pos == 4)
	 {
	   memcpy(UW_CARDID4,p,216);
	 }
	 	 else if(pos == 5)
	 {
	   memcpy(UW_CARDID5,p,216);
	 }
	 	 else if(pos == 6)
	 {
	   memcpy(UW_CARDID6,p,216);
	 }
	 	 else if(pos == 7)
	 {
	   memcpy(UW_CARDID7,p,216);
	 }
	 	 else if(pos == 8)
	 {
	   memcpy(UW_CARDID8,p,216);
	 }
	 	 else if(pos == 9)
	 {
	   memcpy(UW_CARDID9,p,216);
	 }
	 	 else if(pos == 10)
	 {
	   memcpy(UW_CARDID10,p,216);
	 }
}




unsigned char RecALLBusOK ;
extern unsigned char UW_InfoChange[10];



extern signed char   M_BeepTime;
extern unsigned int  M_BEEP_START ; 
extern  unsigned char PC_CTRL_BFLAG; 

extern void   MainRecodeHis(unsigned char devno,unsigned char u_pos,unsigned char action,unsigned char *pCard,
	_calendar_obj calen);

void RecodHanler(unsigned char devno,unsigned char *p1,unsigned char *p2,unsigned char *pCard, unsigned char len,
	_calendar_obj calen)
{
	 unsigned char i = 0;
	 unsigned char u_pos;
	 unsigned char action;
	 
   for(; i < len; i++)
	 {
	    if(p1[i] != p2[i]) 
			{
				 u_pos = i + 1;
         action = p1[i];
			   MainRecodeHis(devno,u_pos,action,&pCard[i*4],calen);
				
				 if(PC_CTRL_BFLAG != 1)
				 {
				   M_BEEP_START = 1;M_BeepTime = 3;
				 }
				 if(devno == 1)
				 {
					  if(p1[i] == 0x01)
						{
							 if(Computer_LED_FLAG1[i] == 0)
							 {
									if(UW_Index1[i]==1)
									{
										RecdLED1[i] = led_b;
								  }
									else
									{
									  RecdLED1[i] = led_off;
									}
							 }
						}
						else
						{
							if(Computer_LED_FLAG1[i] == 0)
							{
						     RecdLED1[i] = led_off;
							}
						}
				 }
				 p2[i] = p1[i];
			}
 	 }
	 
}


extern unsigned char ModbusTbuf[300];
extern unsigned char ModbusSendTcpOK; 

extern unsigned char MiniVerM ;
extern unsigned char MiniVerP ;
extern unsigned char MiniId[4];
extern  unsigned int  ModbusSendTcpLen; 
void   ComboxDataFrameTcp(unsigned char *p,unsigned int len)
{

  unsigned int t_len = len + 7;
  p[0]= 0xfe;
	p[1]= 0xfe;
	
  p[2]= (t_len >> 8 )&0xff;
	p[3]= (t_len & 0xff); 
	
	p[4]=  ModbusRTU;
	
  p[5]=  MiniVerM;
  p[6]=  MiniVerP;
	
	p[7]=  MiniId[0];
	p[8]=  MiniId[1];
	p[9]=  MiniId[2];
	p[10]= MiniId[3];

}
void		RecokRx3BusWorkToTCP()
{
    unsigned char pos = 0;
    u16 recCRC,calCRC;
	  if(RX3_CNT>2)
		{
				calCRC=CRC_Compute(RX3_BUFF,RX3_CNT-2);
				pos = RX3_CNT-1;
				recCRC = RX3_BUFF[pos];
				recCRC = recCRC << 8;
				pos = RX3_CNT-2;
				recCRC |= RX3_BUFF[pos];
				if(calCRC==recCRC )
				{
					 ComboxDataFrameTcp(ModbusTbuf,RX3_CNT);
           memcpy(&ModbusTbuf[11],RX3_BUFF,RX3_CNT);
					 ModbusSendTcpLen = RX3_CNT + 11;
           ModbusSendTcpOK = 1; 
				}
				else
				{
//					 delay_us(1);
				}
		}
    RX3_CNT = 0;  
}



void RecokRx3BusWork()
{
		unsigned char pos = 0;

	  u16 recCRC,calCRC;
	  if(RX3_CNT>2)
		{
				calCRC=CRC_Compute(RX3_BUFF,RX3_CNT-2);
				pos = RX3_CNT-1;
				recCRC = RX3_BUFF[pos];
				recCRC = recCRC << 8;
				pos = RX3_CNT-2;
				recCRC |= RX3_BUFF[pos];
				if(calCRC==recCRC )
				{
          if(RX3_CNT == 0x7d)
					{
					    RX3FirstROK =1;
						  Rx3CopyUW_NUM(RX3_BUFF[0] ,RX3_BUFF[4] );
					  	Rx3CopyUW_Index(RX3_BUFF[0] ,&RX3_BUFF[23]);
					}
					else if(RX3_CNT == 0xdd)
					{
					   if(RX3FirstROK == 1)
						 {
						   Rx3CopyUW_CARDID(RX3_BUFF[0] ,&RX3_BUFF[3]);
							 UW_InfoChange[RX3_BUFF[0]-1] = 1;
							 if(RX3_BUFF[0] == 1)
							 {
										RecodHanler(RX3_BUFF[0] ,UW_Index1,UW_NoteIndex1,UW_CARDID1,54,calendar);
							 }
//						delay_us(1);
						 }
					}
//          delay_us(1);
				}
				else
				{
//					 delay_us(1);
				}
		}
    RX3_CNT = 0;  
}


extern unsigned char PC_CTRL_BFLAG ;     //蜂鸣器控制者 1 PC 控制 ！1模块控制蜂鸣器
extern unsigned char PC_CTRL_BMode ;     //蜂鸣器模式
extern unsigned int  PC_BEEP_KHZ_NUM  ;  //蜂鸣器计数值
extern unsigned int  PC_BEEP_KHZ_FLAG  ; //蜂鸣器时间到发生标志


extern  unsigned int DoorCount;

extern unsigned int SendPCTime;
extern unsigned  int   UWHandlerCout;





extern signed char   M_BeepTime;
extern unsigned int  M_BEEP_KHZ_NUM  ;  
extern unsigned int  M_BEEP_START ; 



extern unsigned int ContorSendPCTime ;


unsigned char ibeep;
extern void MainBeepHandler(void);

extern  unsigned int MainManyTimes;
extern unsigned char ModRtuCmdIndex;
extern unsigned char ModRtuCmdLen;
extern signed  int   MainManyT ;
extern unsigned int MainManyTimesFlag;






extern unsigned int ser_ct;
void Net_comm(void);



void TIM3_IRQHandler(void)   //TIM3中断   25ms
{
	uint8_t i;
	uint8_t fg;
	static uint8_t Sec = 0;
	static uint16_t Read_Time = 0;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除TIMx更新中断标志 
    PowerUpTime++;
    RandCnt++;

    if(TemHandlerCout <500)
	  {
		TemHandlerCout ++;
		}
		if(UWHandlerCout<500)
		{
		  UWHandlerCout++;
		}
		if(DoorCount<40)
		{
		  DoorCount++;
		}
		if(SendPCTime <ContorSendPCTime)
		{
		  SendPCTime++;
		}
		if(MainManyTimesFlag)
		{
			if(MainManyTimes < 16)
			{
				MainManyTimes ++;
			}
		}
		if( RS485_ForTH_Sflag == 1)
    {
			 RS485_ForTH_Ct --;
			if( RS485_ForTH_Ct<=0)
			{
//        RS485_ForTH_Ok=1;//置位帧结束标记
				  RS485_ForTH_Sflag = 0;
				  RS485_ForTH_Ct = 0;
         	RecokRx1BusWork();
			}
    }
		if( RS485_ForUW_Sflag == 1)
    {
			 RS485_ForUW_Ct --;
			if( RS485_ForUW_Ct<=0)
			{
//        RS485_ForTH_Ok=1;//置位帧结束标记
				  RS485_ForUW_Sflag = 0;
				  RS485_ForUW_Ct = 0;
				  if(MODBUS_RTU_NETMODE == 0)
					{
         	  RecokRx3BusWork();
					}
					else
					{
					  RecokRx3BusWorkToTCP();
					}
			}
    }
		
		
		if(M_BEEP_START)
		{
          if(M_BEEP_KHZ_NUM < 4)  
				{
						M_BEEP_KHZ_NUM++;
				}
		}
		
		if(PC_CTRL_BFLAG == PC_CTRL_BEEP_MODE )
		{
				PC_BEEP_KHZ_NUM++;
			  if(PC_CTRL_BMode == 0x01)
				{
          if(PC_BEEP_KHZ_NUM >= 20)  
					{
							PC_BEEP_KHZ_FLAG = 1;
					}
				}
				else if(PC_CTRL_BMode == 0x02)
				{
          if(PC_BEEP_KHZ_NUM >= 80)  
					{
							PC_BEEP_KHZ_FLAG = 1;
					}
				}
			  else if(PC_CTRL_BMode == 0x03)
				{
					if(PC_BEEP_KHZ_NUM >= 210)  
					{
							PC_BEEP_KHZ_FLAG = 1;
					}
				}		
			  if(PC_BEEP_KHZ_NUM>=10000) 	//防止数据过大，溢出
				{
           PC_BEEP_KHZ_NUM = 0;
				}
		}
	 ibeep++;
		if(ibeep>4)
		{
		  MainBeepHandler();
			ibeep= 0;
		}
		Sec++;
		if(Sec>=160)
		{
			Sec = 0;
 
			
      if(USE_OPEN_RUNLED == 1)
			{				
					if(runLed.pGetRunLedValue())		  
					{
						runLed.pSetRunLedValue(0);
					}
					else
					{
					 runLed.pSetRunLedValue(1);
					}
			}
		  Colour_Light_Ticks++;
		}
		
		
		Read_Time++;
		if(Read_Time>40*10)
		{
			Read_Time = 0;
			memset(RdFlag,0,sizeof(RdFlag));
		}
		if(Send_UnAuth_Delay>0)
		{
			Send_UnAuth_Delay--;  //服务器不回复时，发送未授权标签下线信息间隔
		}
// 		Response_Delay++;
// 		if( Wait_ACK == 1)
// 		{
// 			Ack_Tick++;
// 			if(Ack_Tick >= Ack_Time)
// 			{
// 				Ack_Tick = Ack_Time +1;
// 			}
// 		}
		if( Wait_ACK[TAG_MSG] == 1)
		{
			Ack_Tick[TAG_MSG]++;
			if(Ack_Tick[TAG_MSG] >= Ack_Time)
			{
				Ack_Tick[TAG_MSG] = Ack_Time +1;
			}
		}

		if( Wait_ACK[ERR_MSG] == 1)
		{
			Ack_Tick[ERR_MSG]++;
			if(Ack_Tick[ERR_MSG] >= Ack_Time)
			{
				Ack_Tick[ERR_MSG] = Ack_Time +1;
			}
		}		
		
		if( Wait_ACK[LOAD_MSG] == 1)
		{
			Ack_Tick[LOAD_MSG]++;
			if(Ack_Tick[LOAD_MSG] >= Ack_Time)
			{
				Ack_Tick[LOAD_MSG] = Ack_Time +1;
			}
		}

		if( Wait_ACK[WEIGHT_MSG] == 1)
		{
			Ack_Tick[WEIGHT_MSG]++;
			if(Ack_Tick[WEIGHT_MSG] >= Ack_Time)
			{
				Ack_Tick[WEIGHT_MSG] = Ack_Time +1;
			}
		}			

		
// 		power_up++;
		Heartbeat_packet++;
		if(Heartbeat_packet > Heartbt)
		{
			Heartbeat_packet = Heartbt+1;
		}
	
		if(ser_ct <80)
		{
				ser_ct++;

	
		}
// 		if(power_up==400)
// 		{
// 			First_Send = 1;
// 			power_up =401;
// 		}
// 		if(power_up>400)
// 		{
// 			power_up =401;
// 		}
		
// 		if(fifo_pop_delay>0)
// 		{
// 			fifo_pop_delay--;
// 		}

//===20160203 	用其它变量统计模块个数	
// 		if(Response_Delay>=50)
// 		{
// 			SlaveCnt = 0;
// 		}

//===2015-09-25 原软件UDP协议栈相关内容		
// 		switch(count1)
// 		{
// 			case 4:	event_word |= EVENT_READ_KEY;
// 						break;

// 			case 20:	event_word |= EVENT_ARP_RETRANSMIT;	
// 						count1=0;
// 						count2++;
// 						if(count2 > 120)	// 60s cycle
// 						{
// 							event_word |= EVENT_AGE_ARP_CACHE;  
// 							count2=0;
// 						}
// 						break;
// 			default:	break;
// 		}
		
		//--------------------------------------
// 		if(RxBegin)			//串口接收开始标志
// 		{
// 			RxDelay++;
// 			if(RxDelay > 1)	// 10ms 接收超时结束
// 			{
// 				RxBegin = 0;
// 				RxDelay = 0;
// 				Com_Recv_Len = RxDataCnt;	
// 				RxDataCnt = 0;
//  	         	event_word |= EVENT_232_ARRIVED;
// 			}
// 		}
		
		Tim_IWDG_Cnt++;
		Tim_Read_Cnt++;
		Tim_Check_Cnt++;
		Tim_Flash_Cnt++;	
		
		Tim_GetCd_Cnt++;
		Tim_SetAd_Cnt++;
// 		Tim_GetSlCnt_Dly++;

		for(fg=0;fg<6;fg++)
		{
			if(UpFlashFg[fg])
			{
#ifndef ONLINE_BLINK
				if(UpFlashDly[fg])UpFlashDly[fg]--;
				else UpFlashFg[fg] = 0;				
#endif				
			}		
		}
		
		for(fg=0;fg<6;fg++)
		{
			if(DnFlashFg[fg])
			{
				if(DnFlashDly[fg])DnFlashDly[fg]--;
			}		
		}
		
		
		//============= 485 COM1 ======================
		if(RxBegin1>0)		//开始接收数据
		{
			RxOverCnt1++;	//串口接收计时
			if(RxOverCnt1>1)//等待接收已超时>50ms
			{
				RxBegin1=0;
				DataBegin1=0;
				RxOverCnt1=0;
				RxDataCnt1=0;
				RxDataLen1=0;
				RxCheckSum1=0;
			}
		}			

// 		//============= 485 COM2 ======================
// 		if(RxBegin2>0)		//开始接收数据
// 		{
// 			RxOverCnt2++;	//串口接收计时
// 			if(RxOverCnt2>4)//等待接收已超时>100ms
// 			{
// 				RxBegin2=0;
// 				DataBegin2=0;
// 				RxOverCnt2=0;
// 				RxDataCnt2=0;
// 				RxDataLen2=0;
// 				RxCheckSum2=0;
// 				RxOver2=1;
// 			}
// 		}

		//============= 485 COM3 ======================
		if(RxBegin3>0)		//开始接收数据
		{
			RxOverCnt3++;	//串口接收计时
			if(RxOverCnt3>4)//等待接收已超时>50ms
			{
				RxBegin3=0;
				DataBegin3=0;
				RxOverCnt3=0;
				RxDataCnt3=0;
				RxDataLen3=0;
				RxCheckSum3=0;
			}
		}		
//20160608 维护时，要让多U的几个LED灯闪烁步调一致
		if(SyncLed==1)
		{
			times_3s++;
			if(times_3s>=20)
			{
				times_3s=0;
				SyncLed = 0;
				for(i=0;i<6;i++)
				{
					FlashVal[i] = 0;	
				}			
			}		
		}
	}
}



