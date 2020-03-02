/***********************************************************************************************************
��������
***************************************************************************************************************/
#include "global.h"

//unsigned char  			USE_OPEN_RUNLED  = MINN_CLOSE;
unsigned char  			USE_OPEN_RUNLED  = MINI_OPEN;


unsigned char ModbusRbuf[300];
unsigned char ModbusTbuf[300];
unsigned char ModbusRecTcpOK; 
unsigned char ModbusSendTcpOK; 
unsigned int  ModbusRecTcpLen; 
unsigned int  ModbusSendTcpLen; 


unsigned char MiniVerM = 0x02;
unsigned char MiniVerP = 0x01;
unsigned char MiniId[4];


unsigned char PC_CTRL_BFLAG = 0;     
unsigned char PC_CTRL_BMode = 0; 
unsigned int  PC_BEEP_KHZ_NUM  = 0;  
unsigned int  PC_BEEP_KHZ_FLAG  = 0; 


unsigned char pc_beep_sat;


signed char   M_BeepTime;
unsigned int  M_BEEP_START ; 

unsigned int  M_BEEP_KHZ_NUM  ;  
unsigned char M_beep_sat;


unsigned char led_off = 0;
unsigned char led_r = 1;
unsigned char led_v = 2;
unsigned char led_ye = 3;
unsigned char led_g = 4;
unsigned char led_yo = 5;
unsigned char led_b = 6;
unsigned char led_w = 7;



unsigned char RecdLED1[54];
unsigned char RecdLED2[54];
unsigned char RecdLED3[54];
unsigned char RecdLED4[54];
unsigned char RecdLED5[54];
unsigned char RecdLED6[54];
unsigned char RecdLED7[54];
unsigned char RecdLED8[54];
unsigned char RecdLED9[54];
unsigned char RecdLED10[54];



unsigned char UW_NUM1;
unsigned char UW_NUM2;
unsigned char UW_NUM3;
unsigned char UW_NUM4;
unsigned char UW_NUM5;
unsigned char UW_NUM6;
unsigned char UW_NUM7;
unsigned char UW_NUM8;
unsigned char UW_NUM9;
unsigned char UW_NUM10;


unsigned char UW_Index1[54];
unsigned char UW_Index2[54];
unsigned char UW_Index3[54];
unsigned char UW_Index4[54];
unsigned char UW_Index5[54];
unsigned char UW_Index6[54];
unsigned char UW_Index7[54];
unsigned char UW_Index8[54];
unsigned char UW_Index9[54];
unsigned char UW_Index10[54];



unsigned char UW_Index1Back[54];
unsigned char UW_Index2Back[54];
unsigned char UW_Index3Back[54];
unsigned char UW_Index4Back[54];
unsigned char UW_Index5Back[54];
unsigned char UW_Index6Back[54];
unsigned char UW_Index7Back[54];
unsigned char UW_Index8Back[54];
unsigned char UW_Index9Back[54];
unsigned char UW_Index10Back[54];


unsigned char UW_NoteIndex1[54];
unsigned char UW_NoteIndex2[54];
unsigned char UW_NoteIndex3[54];
unsigned char UW_NoteIndex4[54];
unsigned char UW_NoteIndex5[54];
unsigned char UW_NoteIndex6[54];
unsigned char UW_NoteIndex7[54];
unsigned char UW_NoteIndex8[54];
unsigned char UW_NoteIndex9[54];
unsigned char UW_NoteIndex10[54];


unsigned char UW_CARDID1[216];
unsigned char UW_CARDID2[216];
unsigned char UW_CARDID3[216];
unsigned char UW_CARDID4[216];
unsigned char UW_CARDID5[216];
unsigned char UW_CARDID6[216];
unsigned char UW_CARDID7[216];
unsigned char UW_CARDID8[216];
unsigned char UW_CARDID9[216];
unsigned char UW_CARDID10[216];



unsigned char UW_CARDIDBACK1[216];
unsigned char UW_CARDIDBACK2[216];
unsigned char UW_CARDIDBACK3[216];
unsigned char UW_CARDIDBACK4[216];
unsigned char UW_CARDIDBACK5[216];
unsigned char UW_CARDIDBACK6[216];
unsigned char UW_CARDIDBACK7[216];
unsigned char UW_CARDIDBACK8[216];
unsigned char UW_CARDIDBACK9[216];
unsigned char UW_CARDIDBACK10[216];

unsigned char UW_InfoChange[10];

unsigned char Computer_LED_FLAG1[54];
unsigned char Computer_LED_FLAG2[54];
unsigned char Computer_LED_FLAG3[54];
unsigned char Computer_LED_FLAG4[54];
unsigned char Computer_LED_FLAG5[54];
unsigned char Computer_LED_FLAG6[54];
unsigned char Computer_LED_FLAG7[54];
unsigned char Computer_LED_FLAG8[54];
unsigned char Computer_LED_FLAG9[54];
unsigned char Computer_LED_FLAG10[54];

uint32	TemperatureHumidity[10][12];
uint32	TemperatureHumidityBack[10][12];
uint16	Warn[10][54];
uint16	WarnBack[10][54];


unsigned int ser_ct;

#ifdef Err_Report
uint8_t Err_Time[10] = {0,0,0,0,0,0,0,0,0,0};//��Ӧģ���ַ1��9���±�Ϊ0��Ԫ�ز�ʹ��
#endif

uint8_t Broken[6] = {0,0,0,0,0,0};
uint16_t Send_UnAuth_Delay = 0;
uint8_t moni_alm=0;
uint8_t Monitor_ALM[55];
uint8_t To_Write_Auth=0; //д����Ȩ��
uint8_t AuthenBuf[55];//1�����أ�2������أ�0xff��������

uint16_t WK_Info_int=0;
uint16_t Set_Int=0;
uint16_t DS18B20_TMP = 0;
uint8_t  Weight_Percent =0;
uint8_t  Load_Percent=0;
uint8_t Alarm_Over_Weight = 0 ;
uint8_t Alarm_Over_Load = 0 ;
uint16_t UW_Weight[55];
uint16_t UW_Power[55];
uint16_t Total_Weight = 0;
uint16_t Total_Power_Comsuption = 0;

uint8_t UW_Hight[55];
uint8_t Last_Up_Pos = 0;
uint8_t Last_Down_Pos = 0;
uint8_t Store[55];
uint8_t Curr_Slave_Data = 0; //��¼������ǰ�洢�����ĸ��ӻ��ı�ǩ����
uint8_t Wait_Slave_Data; 
uint8_t Up_Send_Flag[55]; //�ѳɹ����ͱ�ǩ�ϼ�λ��
uint8_t To_Send_Next[2];
uint8_t Send_Times = 0; //ĳ��ģ���ϵı�ǩ��Ϣ��Ҫ���͵Ĵ�����һ�δ�һ����ǩ��Ϣ��
uint8_t Cur_Module = 1;

uint8_t U_Position = 0; //��ǰ����λ�õ�Uλ��ǩ��Ϣ

TagDaT TagData[6];

uint8_t ReREAD_Slave_TAG = 0;
uint8_t Reboot_Slave_Flag = 0;

uint16_t Need_Update = 0;
uint16_t Ver_No = 0;
uint32_t Colour_Light_Ticks = 0;
uint8_t SyncLed=0;
uint8_t LINK_ESTABLISHED; //��������������ǽ���״̬
uint8_t link=1;  //������������
uint8_t Link_First;  //���������������
uint16_t PowerUpTime;
uint8_t Stop_Colour_Light = 0;
uint8_t HeartbeatTime =0;
uint8_t Wait_ACK[4];
uint16_t Ack_Tick[4];
uint8_t NO_SLAVE=0;
uint16_t Heartbeat_packet;


uint8_t TCPRECBUF[TCPbuflen];

uint32_t Dat_Lth;

uint8_t Host_Addr;				//������ַ
uint8_t Slave_Addr;				//�ӻ���ַ
// uint8_t SlaveCnt=0;				//�ӻ�����
uint8_t UW_Cnt=0;				//Uλ����
uint8_t UW_Sta=0;				//Uλ״̬
uint8_t Tim_IWDG_Cnt=0;			//���Ź�ι����ѯ������
uint8_t Tim_Read_Cnt=0;			//����ѭ��������
uint8_t Tim_Check_Cnt=0;		//���Ÿ�Ӧ������
uint8_t Tim_Flash_Cnt=0;		//�������������
uint8_t Tim_GetCd_Cnt=0;		//��ȡ��λ�����ݼ�����
uint8_t Tim_SetAd_Cnt=0;		//������λ����ַ������


uint8_t RdCardPos=0;
uint8_t GetDat_Pos;				//�л���ȡ����
uint8_t GetLed_Sta;       //�л���ȡLED״̬
uint8_t TrsLed_Sta=0;       //�л��·�LED״̬
uint8_t LED_STATUS[54];   //20151123  53U֧��
uint8_t LED_STATUS_DOWNLOAD[54] /* = {5,5,5,5,5,5,} */ ;  //20151123  53U֧��




_WK_Par WK_Cur_Val;  //����ֵ����ǰֵ
TCPDat *pTCPDa;//,*pTCPDa1;

RTU_DaT *pRTU_Da;

UW_DaT *pUW_Da;

uint8_t Sys_UW_Cnt[Sys_MK_Count];		//ϵͳ�ӻ�Uλ����
uint8_t Sys_UW_Num[54];				//ϵͳUλ�߶�
uint8_t Sys_MK_Cnt_New;					//ϵͳģ����������
uint8_t Sys_MK_Cnt_Old;					//ϵͳģ���ϴ�����
uint8_t Sys_UW_Sta_New[Sys_MK_Count];	//ϵͳUλ����״̬
uint8_t Sys_UW_Sta_Old[Sys_MK_Count];	//ϵͳUλ�ϴ�״̬
uint32_t Sys_SerialNum;					//ϵͳ���к�
uint16_t Sys_RS485Addr;					//ϵͳRS485��ַ

uint8_t Ud_UW_Num;						//����һ��Uλ��
uint8_t Ud_UW_High;						//����һ��Uλ�߶�

// uint64_t Ud_RFID_ID[2];					//����һ��RFID��ǩID
// uint64_t Ud_RFID_ID_EXT[2];			//����һ��RFID��ǩID��չ8�ֽ�

uint16_t Ud_UW_Updata;					//Uλ���ݱ仯����

// uint8_t ReadDaCnt[6];			//��ȡ���ݴ���


uint8_t SenFlag[6];				//�Ÿ�Ӧ��־1~6
uint8_t RdFlag[6];				//�Ѷ�����־1~6
uint8_t WtFlag[6];        //��дȨ�ޱ�־1~6
uint8_t UpFlashFg[6];			//�������Ʊ�־1~6
uint8_t DnFlashFg[6];			//�������Ʊ�־1~6
uint8_t DyFlashFg[6];			//��̬���Ʊ�־1~6

uint8_t FlashVal[6];			//����ֵ1~6

uint8_t UpFlashDly[6];			//�������������ʱ

uint8_t DnFlashDly[6];			//�������������ʱ			

uint8_t DyFlashDly[6];			//��̬���������ʱ

uint8_t DyFlashPos[6];			//��̬����ƺ�
uint8_t DyFlashClr[6];			//��̬�������ɫ

uint8_t UnitResponse;       //�ӻ���Ӧ��ʶ


//  uint8_t MifareKey[6] = {'D','G','U','W','E','I'};
// //uint8_t MifareKey[6] = {255,255,255,255,255,255};

#ifdef Original_key
	uint8_t MifareKey[6] = {255,255,255,255,255,255};
#else
	uint8_t MifareKey[6] = {'D','G','U','W','E','I'};
#endif


UINT  times_3s;						//���Ź���ʱ��λ

UCHAR my_ips[4],sb_net[4],gw_ips[4],rm_ips[4]; 
 
 
// UINT Tcp_Client_port=5000;
 
UCHAR bc_mac[6];					//={0xff,0xff,0xff,0xff,0xff,0xff};
UCHAR my_mac[6];					//={0x00,0xAB,0xCC,0x65,0xDE,0xAA};
ULONG my_ipaddr;					//=0xC0A8016FL;
ULONG my_subnet;					//=0xFFFFFF00L;
ULONG gw_ipaddr;					//=0xC0A80101L;
ULONG remote_ip;					//=0xC0A80164L;
UINT  remote_port;					//=50000;
UINT  self_port;					//=50000;
	
// UCHAR volatile event_word;			//�����¼�״̬��
 
Setup_Parameter NetParameter;		//ͨ���������

const UCHAR StartFlag_buf[FLASH_FLAG_NUM] = {0x55,0xaa,0x55,0xaa,0x55,0xaa,0x55,0xaa,0x55,0xaa};//������ʼ��


const UCHAR StartParameter_buf[NET_PARA_LEN] = 
{
	0x00,
	0x55,0xaa,0x55,0xaa,0x55,0xaa,0x55,0xaa,0x55,0xaa,0x55,0xaa,0x55,0xaa,0x55,0xaa,	//��λ�����ý���������	Ĭ�ϰ��飺0x55 0xAA
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	//��λ�����ý��ı�־��	Ĭ��ʮ������0xff
	0x08,                               // �������ֳ��� 5(MAX=15) 
//'U','-','W','E','I',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, //�������� U-WEI
	'M','i','n','i','B','o','x',' ','V','e','r',' ','2','.','1', //�������� U-WEI	
	 
	0x00,                               // ip  ģʽ 
	0x00,0xab,0xcc,0x45,0xde,0xaa,      // MAC ��ַ		00-AB-CC-45-DE-AA
	
//0xc0,0xa8,0x00,0xa0,                // ���� IP 		192.168.0.160
//0xc0,0xa8,0x00,0xb2,                // ���� IP 		192.168.0.178
	0xc0,0xa8,0x00,200,                 // ���� IP 		192.168.0.200  //132
	
	0xff,0xff,0xff,0x00,                // ��������		255.255.255.0
	0xc0,0xa8,0x00,0x01,                // Ĭ������		192.168.0.1  
	
//0xc0,0xa8,0x00,0x8b,                // Զ��IP��ַ	192.168.0.139
//  0xc0,0xa8,0x00,246,                // Զ��IP��ַ	192.168.0.199
	
	0xc0,0xa8,0x00,227,                // Զ��IP��ַ	192.168.0.199
	0x01,0xf6,                          // ���ض˿ں�	502	ע��С��ģʽ
	0x01,0xf6,                          // Զ�̶˿ں�	502	ע��С��ģʽ
	0x01,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x00,0x00,0x00,0x00,0x00,//�豸���1�ֽڣ��豸ID��10�ֽ� 
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x05,                               // ѡ��5 ������:38400  
	0x31,0x30,0x30,0x2e,0x31,0x30,0x30,	// Ӳ����汾��:100.100
};

//3.1��3.2��������ͨ��ˢ�����ǩʵ�ֲ�����λ����
#ifndef _V3_3
const uint8_t Para_Reset_Info[]="�����������˰����Ƽ�������λר��";
uint8_t Need_To_Reset_Par = 0;
#endif

// // ����Ĵ�������
// uint16_t usRegInputBuf[REG_INPUT_NREGS] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
// // �Ĵ�����ʼ��ַ
// uint16_t usRegInputStart = REG_INPUT_START;

// ���ּĴ�������
uint16_t usRegHoldingBuf1[REG_HOLDING_NREGS_1];
// ���ּĴ�����ʼ��ַ
uint16_t usRegHoldingStart1 = REG_HOLDING_START_1;

// // ���ּĴ�������
// uint16_t usRegHoldingBuf2[REG_HOLDING_NREGS_2]  /*= {5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5} */;
// // ���ּĴ�����ʼ��ַ
// uint16_t usRegHoldingStart2 = REG_HOLDING_START_2;

// ���ּĴ�������
uint16_t usRegHoldingBuf3[REG_HOLDING_NREGS_3]  /*= {5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5} */;
// ���ּĴ�����ʼ��ַ
uint16_t usRegHoldingStart3 = REG_HOLDING_START_3;

uint16_t usRegHoldingBufBkp3[REG_HOLDING_NREGS_3] ;

// ���ּĴ�������
uint16_t usRegHoldingBuf4[REG_HOLDING_NREGS_4]  /*= {5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5} */;
// ���ּĴ�����ʼ��ַ
uint16_t usRegHoldingStart4 = REG_HOLDING_START_4;

uint16_t usRegHoldingBufBkp4[REG_HOLDING_NREGS_4] ;

// // ��Ȧ״̬
// uint8_t ucRegCoilsBuf[REG_COILS_SIZE / 8] = {0xFF, 0x00};
// // ����״̬
// uint8_t ucRegDiscBuf[REG_DISC_SIZE / 8] = {0x00,0xFF};

uint16_t testbuf[5];

uint8 RxBuff3[300];
uint8 RxBuff1[30];

stUW UW[MODMAX];


