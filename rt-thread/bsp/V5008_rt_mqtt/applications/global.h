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

// #define ConInterval 20000     //���Ӽ�� ��������û��ʱ 6��
// #define ConInterval 50000     //���Ӽ�� ��������û��ʱ Լ24��
// ���¶ȴ������汾
#define   ConInterval  3000     //���Ӽ�� ��������û��ʱ Լ20��
//#define ConInterval 60000     //���Ӽ�� ��������û��ʱ 6��

#define  DOORCLOSE   1
#define  DOOROPEN    0

#define  MODMAX      10

typedef struct
{
	uint8 	M_Address; 		//ģ���ַ
	uint8 	M_Len;	 		//ģ�鳤�� ����
	uint8 	U_Count;	 	//Uλ��
	uint32	M_ID;			//ģ��ID
	uint32	*U_CardID;		//��ID
	uint32	*U_CardIDBack;	//����ID����
	uint8	*U_Status;		//Uλ״̬
	uint8	*U_StatusBack;		//Uλ״̬ ����
	uint32	*TemperatureHumidity;	//�¶�
	uint32	*TemperatureHumidityBack;	//�¶� ����
	uint16	*U_Warn;			//����
	uint16	*U_WarnBack;		//���� ����
	uint8	Online;			//���߱�־
	
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
extern uint8_t Err_Time[10];//��Ӧģ���ַ1��9���±�Ϊ0��Ԫ�ز�ʹ��
#endif

extern uint8_t Broken[6];
extern uint16_t Send_UnAuth_Delay;
extern uint8_t moni_alm;
extern uint8_t Monitor_ALM[55];
extern uint8_t To_Write_Auth; //д����Ȩ��
extern uint8_t AuthenBuf[55];//1�����أ�2������أ�0xff��������
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
extern uint8_t Curr_Slave_Data; //��¼������ǰ�洢�����ĸ��ӻ��ı�ǩ����
extern uint8_t Wait_Slave_Data; 
extern uint8_t Up_Send_Flag[55]; //�ѳɹ����ͱ�ǩ�ϼ�λ��
extern uint8_t To_Send_Next[2];
extern uint8_t Send_Times; //ĳ��ģ���ϵı�ǩ��Ϣ��Ҫ���͵Ĵ�����һ�δ�һ����ǩ��Ϣ��
extern uint8_t Cur_Module;

extern uint8_t U_Position; //��ǰ����λ�õ�Uλ��ǩ��Ϣ

extern TagDaT TagData[6];
// extern uint8_t IC_Data[14][48];
extern uint8_t ReREAD_Slave_TAG;
extern uint8_t Reboot_Slave_Flag;

extern uint16_t Need_Update;
extern uint16_t Ver_No;
extern uint32_t Colour_Light_Ticks;
extern uint8_t SyncLed;
extern uint8_t LINK_ESTABLISHED; //��������������ǽ���״̬
extern uint8_t link;  //������������
extern uint8_t Link_First;  //���������������
extern uint16_t PowerUpTime;
extern uint8_t Stop_Colour_Light;
extern uint8_t HeartbeatTime;
extern uint8_t Wait_ACK[4];
extern uint16_t Ack_Tick[4];
extern uint8_t NO_SLAVE;
extern uint16_t Heartbeat_packet;

//====20160528 ע�͵�δʹ�õı��� ��ʡ�ڴ�
// extern uint8_t TcpSendBuf[TCPbuflen];

extern uint8_t TCPRECBUF[TCPbuflen];
// extern uint8_t First_Send;
// extern uint16_t power_up;
// extern uint16_t fifo_pop_delay;
// extern uint16_t Up_BAK[8];  //20151203  �����ϼ���Ϣ���б仯�������ϱ�
// extern uint16_t Up_BAK_OLD[8];  //20151203  �����ϼ���Ϣ���б仯�������ϱ�
// extern uint32_t No_Connection_Tick;
extern uint32_t Dat_Lth;
// extern uint8_t LedCtl_InProcess;

// extern uint8_t Sys_Para[10];
extern uint8_t Host_Addr;				//������ַ
extern uint8_t Slave_Addr;				//�ӻ���ַ
// extern uint8_t SlaveCnt=0;				//�ӻ�����
extern uint8_t UW_Cnt;				//Uλ����
extern uint8_t UW_Sta;				//Uλ״̬
extern uint8_t Tim_IWDG_Cnt;			//���Ź�ι����ѯ������
extern uint8_t Tim_Read_Cnt;			//����ѭ��������
extern uint8_t Tim_Check_Cnt;		//���Ÿ�Ӧ������
extern uint8_t Tim_Flash_Cnt;		//�������������
extern uint8_t Tim_GetCd_Cnt;		//��ȡ��λ�����ݼ�����
extern uint8_t Tim_SetAd_Cnt;		//������λ����ַ������

// extern uint8_t Response_Delay=0;


// extern uint16_t Tim_GetSlCnt_Dly=0;	//��ȡ�ӻ�������ʱ

// extern uint8_t card_buf[4];

// extern uint32_t CardNum;
extern uint8_t RdCardPos;
extern uint8_t GetDat_Pos;				//�л���ȡ����
extern uint8_t GetLed_Sta;       //�л���ȡLED״̬
extern uint8_t TrsLed_Sta;       //�л��·�LED״̬
extern uint8_t LED_STATUS[54];   //20151123  53U֧��
extern uint8_t LED_STATUS_DOWNLOAD[54] /* = {5,5,5,5,5,5,} */ ;  //20151123  53U֧��


// volatile ReaderDaT ReaderDa[Sys_MK_Count];

//2016-05-28 ȥ��δʹ�õı����������ڴ�ռ��
// ReaderDaT ReaderDaTmp1[6];
// ReaderDaT ReaderDaTmp2[6];

extern _WK_Par WK_Cur_Val;  //����ֵ����ǰֵ
extern TCPDat *pTCPDa;//,*pTCPDa1;

extern RTU_DaT *pRTU_Da;

extern UW_DaT *pUW_Da;

extern uint8_t Sys_UW_Cnt[Sys_MK_Count];		//ϵͳ�ӻ�Uλ����
extern uint8_t Sys_UW_Num[54];				//ϵͳUλ�߶�
extern uint8_t Sys_MK_Cnt_New;					//ϵͳģ����������
extern uint8_t Sys_MK_Cnt_Old;					//ϵͳģ���ϴ�����
extern uint8_t Sys_UW_Sta_New[Sys_MK_Count];	//ϵͳUλ����״̬
extern uint8_t Sys_UW_Sta_Old[Sys_MK_Count];	//ϵͳUλ�ϴ�״̬
extern uint32_t Sys_SerialNum;					//ϵͳ���к�
extern uint16_t Sys_RS485Addr;					//ϵͳRS485��ַ

extern uint8_t Ud_UW_Num;						//����һ��Uλ��
extern uint8_t Ud_UW_High;						//����һ��Uλ�߶�

// extern uint64_t Ud_RFID_ID[2];					//����һ��RFID��ǩID
// extern uint64_t Ud_RFID_ID_EXT[2];			//����һ��RFID��ǩID��չ8�ֽ�

extern uint16_t Ud_UW_Updata;					//Uλ���ݱ仯����

// extern uint8_t ReadDaCnt[6];			//��ȡ���ݴ���


extern uint8_t SenFlag[6];				//�Ÿ�Ӧ��־1~6
extern uint8_t RdFlag[6];				//�Ѷ�����־1~6
extern uint8_t WtFlag[6];        //��дȨ�ޱ�־1~6
extern uint8_t UpFlashFg[6];			//�������Ʊ�־1~6
extern uint8_t DnFlashFg[6];			//�������Ʊ�־1~6
extern uint8_t DyFlashFg[6];			//��̬���Ʊ�־1~6

extern uint8_t FlashVal[6];			//����ֵ1~6

extern uint8_t UpFlashDly[6];			//�������������ʱ

extern uint8_t DnFlashDly[6];			//�������������ʱ			

extern uint8_t DyFlashDly[6];			//��̬���������ʱ

extern uint8_t DyFlashPos[6];			//��̬����ƺ�
extern uint8_t DyFlashClr[6];			//��̬�������ɫ

extern uint8_t UnitResponse;       //�ӻ���Ӧ��ʶ


//  extern uint8_t MifareKey[6] = {'D','G','U','W','E','I'};
// //extern uint8_t MifareKey[6] = {255,255,255,255,255,255};

#ifdef Original_key
	extern uint8_t MifareKey[6];
#else
	extern uint8_t MifareKey[6];
#endif


extern UINT  times_3s;						//���Ź���ʱ��λ

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
	
// UCHAR volatile event_word;			//�����¼�״̬��
 
extern Setup_Parameter NetParameter;		//ͨ���������

extern const UCHAR StartFlag_buf[FLASH_FLAG_NUM];//������ʼ��


extern const UCHAR StartParameter_buf[NET_PARA_LEN];

//3.1��3.2��������ͨ��ˢ�����ǩʵ�ֲ�����λ����
#ifndef _V3_3
extern const uint8_t Para_Reset_Info[];
extern uint8_t Need_To_Reset_Par;
#endif

// // ����Ĵ�������
// uint16_t usRegInputBuf[REG_INPUT_NREGS] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
// // �Ĵ�����ʼ��ַ
// uint16_t usRegInputStart = REG_INPUT_START;

// ���ּĴ�������
extern uint16_t usRegHoldingBuf1[REG_HOLDING_NREGS_1];
// ���ּĴ�����ʼ��ַ
extern uint16_t usRegHoldingStart1;

// // ���ּĴ�������
// uint16_t usRegHoldingBuf2[REG_HOLDING_NREGS_2]  /*= {5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5} */;
// // ���ּĴ�����ʼ��ַ
// uint16_t usRegHoldingStart2 = REG_HOLDING_START_2;

// ���ּĴ�������
extern uint16_t usRegHoldingBuf3[REG_HOLDING_NREGS_3]  /*= {5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5} */;
// ���ּĴ�����ʼ��ַ
extern uint16_t usRegHoldingStart3;

extern uint16_t usRegHoldingBufBkp3[REG_HOLDING_NREGS_3] ;

// ���ּĴ�������
extern uint16_t usRegHoldingBuf4[REG_HOLDING_NREGS_4]  /*= {5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5} */;
// ���ּĴ�����ʼ��ַ
extern uint16_t usRegHoldingStart4;

extern uint16_t usRegHoldingBufBkp4[REG_HOLDING_NREGS_4] ;

// // ��Ȧ״̬
// uint8_t ucRegCoilsBuf[REG_COILS_SIZE / 8] = {0xFF, 0x00};
// // ����״̬
// uint8_t ucRegDiscBuf[REG_DISC_SIZE / 8] = {0x00,0xFF};

extern uint16_t testbuf[5];

extern stUart Uart[3];
extern uint8 RxBuff3[300];
extern uint8 RxBuff1[30];


#endif


