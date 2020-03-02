#ifndef GLOBAL_H
#define GLOBAL_H
#include "stdint.h"
#include "mini_driver485.h"
#include "mini_beep.h"
#include "run_led.h"
#include "main.h"
#include "modbus.h"
#include "serial_com.h"

#define  						MINN_CLOSE    		0
#define  						MINI_OPEN     		1


#define            				MINI_USE_DOG       1

#define  UWHumNum                        1
#define TCPbuflen (1024)

#define  DEBUG_NOTE         1

#define Local_UDP_Port  65534

// #define ConInterval 20000     //连接间隔 当服务器没开时 6秒
// #define ConInterval 50000     //连接间隔 当服务器没开时 约24秒
// 带温度传感器版本
#define   ConInterval  3000     //连接间隔 当服务器没开时 约20秒
//#define ConInterval 60000     //连接间隔 当服务器没开时 6秒

#define  DOORCLOSE   1
#define  DOOROPEN    0

#define  MODMAX      10

typedef struct
{
	uint8 	M_Address; 		//模块地址
	uint8 	M_Len;	 		//模块长度 节数
	uint8 	U_Count;	 	//U位数
	uint32	M_ID;			//模块ID
	uint32	*U_CardID;		//卡ID
	uint32	*U_CardIDBack;	//卡号ID备份
	uint8	*U_Status;		//U位状态
	uint8	*U_StatusBack;		//U位状态 备份
	uint32	*TemperatureHumidity;	//温度
	uint32	*TemperatureHumidityBack;	//温度 备份
	uint16	*U_Warn;			//警告
	uint16	*U_WarnBack;		//警告 备份
	uint8	Online;			//在线标志
	
}stUW;

extern stUW UW[MODMAX];




extern   struct  		MINIBEEP       	miniBeep;	
extern	 struct  		RUNLED         	runLed;
extern   struct  		MINIDRIVER485  	mini_driv485;

extern unsigned char  			USE_OPEN_RUNLED;


extern unsigned char ModbusRbuf[300];
extern unsigned char ModbusTbuf[300];
extern unsigned char ModbusRecTcpOK; 
extern unsigned char ModbusSendTcpOK; 
extern unsigned int  ModbusRecTcpLen; 
extern unsigned int  ModbusSendTcpLen; 


extern unsigned char MiniVerM;
extern unsigned char MiniVerP;
extern unsigned char MiniId[4];


extern unsigned char PC_CTRL_BFLAG;     
extern unsigned char PC_CTRL_BMode; 
extern unsigned int  PC_BEEP_KHZ_NUM;  
extern unsigned int  PC_BEEP_KHZ_FLAG; 


extern unsigned char pc_beep_sat;


extern signed char   M_BeepTime;
extern unsigned int  M_BEEP_START ; 

extern unsigned int  M_BEEP_KHZ_NUM  ;  
extern unsigned char M_beep_sat;


extern unsigned char led_off;
extern unsigned char led_r;
extern unsigned char led_v;
extern unsigned char led_ye;
extern unsigned char led_g;
extern unsigned char led_yo;
extern unsigned char led_b;
extern unsigned char led_w;



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

extern unsigned char UW_InfoChange[10];

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


extern uint32	TemperatureHumidity[10][12];
extern uint32	TemperatureHumidityBack[10][12];
extern uint16	Warn[10][54];
extern uint16	WarnBack[10][54];


extern unsigned int ser_ct;


#ifdef Err_Report
extern uint8_t Err_Time[10];//对应模块地址1到9，下标为0的元素不使用
#endif

extern uint8_t Broken[6];
extern uint16_t Send_UnAuth_Delay;
extern uint8_t moni_alm;
extern uint8_t Monitor_ALM[55];
extern uint8_t To_Write_Auth; //写入监控权限
extern uint8_t AuthenBuf[55];//1代表监控，2代表不监控，0xff不做处理
// extern uint8_t UnAuthenBuf[55];
extern uint16_t WK_Info_int;
extern uint16_t Set_Int;
extern uint16_t DS18B20_TMP;
extern uint8_t  Weight_Percent;
extern uint8_t  Load_Percent;
extern uint8_t Alarm_Over_Weight;
extern uint8_t Alarm_Over_Load;
extern uint16_t UW_Weight[55];
extern uint16_t UW_Power[55];
extern uint16_t Total_Weight;
extern uint16_t Total_Power_Comsuption;
// extern uint16_t Run_Success_Flag=0;
extern uint8_t UW_Hight[55];
extern uint8_t Last_Up_Pos;
extern uint8_t Last_Down_Pos;
extern uint8_t Store[55];
extern uint8_t Curr_Slave_Data; //记录主机当前存储的是哪个从机的标签数据
extern uint8_t Wait_Slave_Data; 
extern uint8_t Up_Send_Flag[55]; //已成功发送标签上架位置
extern uint8_t To_Send_Next[2];
extern uint8_t Send_Times; //某节模块上的标签信息需要传送的次数（一次传一个标签信息）
extern uint8_t Cur_Module;

extern uint8_t U_Position; //当前发送位置的U位标签信息

extern TagDaT TagData[6];
// extern uint8_t IC_Data[14][48];
extern uint8_t ReREAD_Slave_TAG;
extern uint8_t Reboot_Slave_Flag;

extern uint16_t Need_Update;
extern uint16_t Ver_No;
extern uint32_t Colour_Light_Ticks;
extern uint8_t SyncLed;
extern uint8_t LINK_ESTABLISHED; //与服务器的连接是建立状态
extern uint8_t link;  //网线物理连接
extern uint8_t Link_First;  //初次与服务器连接
extern uint16_t PowerUpTime;
extern uint8_t Stop_Colour_Light;
extern uint8_t HeartbeatTime;
extern uint8_t Wait_ACK[4];
extern uint16_t Ack_Tick[4];
extern uint8_t NO_SLAVE;
extern uint16_t Heartbeat_packet;

//====20160528 注释掉未使用的变量 节省内存
// extern uint8_t TcpSendBuf[TCPbuflen];

extern uint8_t TCPRECBUF[TCPbuflen];
// extern uint8_t First_Send;
// extern uint16_t power_up;
// extern uint16_t fifo_pop_delay;
// extern uint16_t Up_BAK[8];  //20151203  备份上架信息，有变化则主动上报
// extern uint16_t Up_BAK_OLD[8];  //20151203  备份上架信息，有变化则主动上报
// extern uint32_t No_Connection_Tick;
extern uint32_t Dat_Lth;
// extern uint8_t LedCtl_InProcess;

// extern uint8_t Sys_Para[10];
extern uint8_t Host_Addr;				//本机地址
extern uint8_t Slave_Addr;				//从机地址
// extern uint8_t SlaveCnt=0;				//从机数量
extern uint8_t UW_Cnt;				//U位数量
extern uint8_t UW_Sta;				//U位状态
extern uint8_t Tim_IWDG_Cnt;			//看门狗喂狗轮询计数器
extern uint8_t Tim_Read_Cnt;			//读卡循环计数器
extern uint8_t Tim_Check_Cnt;		//检查磁感应计数器
extern uint8_t Tim_Flash_Cnt;		//上线闪光计数器
extern uint8_t Tim_GetCd_Cnt;		//获取下位机数据计数器
extern uint8_t Tim_SetAd_Cnt;		//设置下位机地址计数器

// extern uint8_t Response_Delay=0;


// extern uint16_t Tim_GetSlCnt_Dly=0;	//获取从机数量延时

// extern uint8_t card_buf[4];

// extern uint32_t CardNum;
extern uint8_t RdCardPos;
extern uint8_t GetDat_Pos;				//切换获取卡号
extern uint8_t GetLed_Sta;       //切换获取LED状态
extern uint8_t TrsLed_Sta;       //切换下发LED状态
extern uint8_t LED_STATUS[54];   //20151123  53U支持
extern uint8_t LED_STATUS_DOWNLOAD[54] /* = {5,5,5,5,5,5,} */ ;  //20151123  53U支持


// volatile ReaderDaT ReaderDa[Sys_MK_Count];

//2016-05-28 去掉未使用的变量，减少内存占用
// ReaderDaT ReaderDaTmp1[6];
// ReaderDaT ReaderDaTmp2[6];

extern _WK_Par WK_Cur_Val;  //设置值及当前值
extern TCPDat *pTCPDa;//,*pTCPDa1;

extern RTU_DaT *pRTU_Da;

extern UW_DaT *pUW_Da;

extern uint8_t Sys_UW_Cnt[Sys_MK_Count];		//系统从机U位数量
extern uint8_t Sys_UW_Num[54];				//系统U位高度
extern uint8_t Sys_MK_Cnt_New;					//系统模块最新数量
extern uint8_t Sys_MK_Cnt_Old;					//系统模块上次数量
extern uint8_t Sys_UW_Sta_New[Sys_MK_Count];	//系统U位最新状态
extern uint8_t Sys_UW_Sta_Old[Sys_MK_Count];	//系统U位上次状态
extern uint32_t Sys_SerialNum;					//系统序列号
extern uint16_t Sys_RS485Addr;					//系统RS485地址

extern uint8_t Ud_UW_Num;						//最新一次U位号
extern uint8_t Ud_UW_High;						//最新一次U位高度

// extern uint64_t Ud_RFID_ID[2];					//最新一次RFID标签ID
// extern uint64_t Ud_RFID_ID_EXT[2];			//最新一次RFID标签ID扩展8字节

extern uint16_t Ud_UW_Updata;					//U位数据变化编码

// extern uint8_t ReadDaCnt[6];			//读取数据次数


extern uint8_t SenFlag[6];				//磁感应标志1~6
extern uint8_t RdFlag[6];				//已读卡标志1~6
extern uint8_t WtFlag[6];        //已写权限标志1~6
extern uint8_t UpFlashFg[6];			//上线闪灯标志1~6
extern uint8_t DnFlashFg[6];			//下线闪灯标志1~6
extern uint8_t DyFlashFg[6];			//动态闪灯标志1~6

extern uint8_t FlashVal[6];			//闪灯值1~6

extern uint8_t UpFlashDly[6];			//上线闪光控制延时

extern uint8_t DnFlashDly[6];			//下线闪光控制延时			

extern uint8_t DyFlashDly[6];			//动态闪光控制延时

extern uint8_t DyFlashPos[6];			//动态闪光灯号
extern uint8_t DyFlashClr[6];			//动态闪光灯颜色

extern uint8_t UnitResponse;       //从机回应标识


//  extern uint8_t MifareKey[6] = {'D','G','U','W','E','I'};
// //extern uint8_t MifareKey[6] = {255,255,255,255,255,255};

#ifdef Original_key
	extern uint8_t MifareKey[6];
#else
	extern uint8_t MifareKey[6];
#endif


extern UINT  times_3s;						//看门狗定时复位

extern UCHAR my_ips[4],sb_net[4],gw_ips[4],rm_ips[4]; 
 
 
// UINT Tcp_Client_port=5000;
 
extern UCHAR bc_mac[6];					//={0xff,0xff,0xff,0xff,0xff,0xff};
extern UCHAR my_mac[6];					//={0x00,0xAB,0xCC,0x65,0xDE,0xAA};
extern ULONG my_ipaddr;					//=0xC0A8016FL;
extern ULONG my_subnet;					//=0xFFFFFF00L;
extern ULONG gw_ipaddr;					//=0xC0A80101L;
extern ULONG remote_ip;					//=0xC0A80164L;
extern UINT  remote_port;					//=50000;
extern UINT  self_port;					//=50000;
	
// UCHAR volatile event_word;			//周期事件状态字
 
extern Setup_Parameter NetParameter;		//通信网络参数

extern const UCHAR StartFlag_buf[FLASH_FLAG_NUM];//开机初始化


extern const UCHAR StartParameter_buf[NET_PARA_LEN];

//3.1或3.2版主机，通过刷特殊标签实现参数复位功能
#ifndef _V3_3
extern const uint8_t Para_Reset_Info[];
extern uint8_t Need_To_Reset_Par;
#endif

// // 输入寄存器内容
// uint16_t usRegInputBuf[REG_INPUT_NREGS] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
// // 寄存器起始地址
// uint16_t usRegInputStart = REG_INPUT_START;

// 保持寄存器内容
extern uint16_t usRegHoldingBuf1[REG_HOLDING_NREGS_1];
// 保持寄存器起始地址
extern uint16_t usRegHoldingStart1;

// // 保持寄存器内容
// uint16_t usRegHoldingBuf2[REG_HOLDING_NREGS_2]  /*= {5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5} */;
// // 保持寄存器起始地址
// uint16_t usRegHoldingStart2 = REG_HOLDING_START_2;

// 保持寄存器内容
extern uint16_t usRegHoldingBuf3[REG_HOLDING_NREGS_3]  /*= {5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5} */;
// 保持寄存器起始地址
extern uint16_t usRegHoldingStart3;

extern uint16_t usRegHoldingBufBkp3[REG_HOLDING_NREGS_3] ;

// 保持寄存器内容
extern uint16_t usRegHoldingBuf4[REG_HOLDING_NREGS_4]  /*= {5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5} */;
// 保持寄存器起始地址
extern uint16_t usRegHoldingStart4;

extern uint16_t usRegHoldingBufBkp4[REG_HOLDING_NREGS_4] ;

// // 线圈状态
// uint8_t ucRegCoilsBuf[REG_COILS_SIZE / 8] = {0xFF, 0x00};
// // 开关状态
// uint8_t ucRegDiscBuf[REG_DISC_SIZE / 8] = {0x00,0xFF};

extern uint16_t testbuf[5];

extern stUart Uart[3];
extern uint8 RxBuff3[300];
extern uint8 RxBuff1[30];


#endif


