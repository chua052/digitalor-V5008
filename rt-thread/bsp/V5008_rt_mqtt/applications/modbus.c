#include "modbus.h"
#include "rtthread.h"
#include "global.h"
#include "math.h"
#include "mqtt.h"
#include "iwdg.h"

extern u8 edit_ustaus_log(u8 *ustatus_new, u8 *ustatus_old, u8 m_addr, u8 u_count);
extern u8 edit_uwarn_log(u16 *uwarn_new, u16 *uwarn_old, u8 m_addr, u8 u_count);

unsigned char sem_flag = 1;

static const UCHAR aucCRCHi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40
};

static const UCHAR aucCRCLo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
    0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
    0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,
    0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
    0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
    0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
    0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 
    0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,
    0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
    0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
    0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 
    0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
    0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
    0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,
    0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,
    0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,
    0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
    0x41, 0x81, 0x80, 0x40
};

struct rt_semaphore sync_sem;
struct rt_semaphore sync_sem2;
struct rt_semaphore sync_sem3;
struct rt_semaphore sync_sem4;
extern unsigned char err_ena_flag;
static stHoldingReg HoldingReg[];

//===新信号量
rt_sem_t dynamic_sem = RT_NULL;
rt_sem_t dynamic_sem2 = RT_NULL;
rt_sem_t dynamic_sem3 = RT_NULL;
rt_sem_t dynamic_sem4 = RT_NULL;

stModbusSync  ModbusSync[3]; //用于modbus同步

USHORT MBCRC16( UCHAR * pucFrame, USHORT usLen )
{
    UCHAR           ucCRCHi = 0xFF;
    UCHAR           ucCRCLo = 0xFF;
    int             iIndex;

    while( usLen-- )
    {
        iIndex = ucCRCLo ^ *( pucFrame++ );
        ucCRCLo = ( UCHAR )( ucCRCHi ^ aucCRCHi[iIndex] );
        ucCRCHi = aucCRCLo[iIndex];
    }
    return ( USHORT )( ucCRCHi << 8 | ucCRCLo );
}

void MBFuncReadInputRegister(COM com, uint8 dev_addr, uint16 start_addr, uint16 len)
{
	uint8 cmd[8];
	uint16 crc;
	ModbusSync[com].dev_addr = dev_addr;
	ModbusSync[com].addr     = start_addr;
	ModbusSync[com].len 		= len;
	ModbusSync[com].fnc      = 0x04;
	cmd[0] = dev_addr;
	cmd[1] = 0x04;
	cmd[2] = (uint8)(start_addr >> 8);
	cmd[3] = (uint8)(start_addr  & 0x00FF);
	cmd[4] = (uint8)(len >> 8);
	cmd[5] = (uint8)(len  & 0x00FF);
	crc = MBCRC16(cmd, 6);
	cmd[6] = (uint8)(crc & 0x00ff);
	cmd[7] = (uint8)(crc >> 8);
	UART_Send(com, cmd, 8);
}

void MBFuncWriteMultipleHoldingRegister(COM com, uint8 dev_addr, uint16 start_addr, uint16 len, uint8* data)
{
	uint16 crc;
	uint8* cmd = rt_malloc(9 + (len * 2));
	if(cmd == RT_NULL)
		return;
	ModbusSync[com].dev_addr = dev_addr;
	ModbusSync[com].addr     = start_addr;
	ModbusSync[com].len 		= len;
	ModbusSync[com].fnc      = 0x10;
	
	cmd[0] = dev_addr;
	cmd[1] = 0x10;
	cmd[2] = (uint8)(start_addr >> 8);
	cmd[3] = (uint8)(start_addr  & 0x00FF);
	cmd[4] = (uint8)(len >> 8);
	cmd[5] = (uint8)(len  & 0x00FF);
	cmd[6] = (uint8)(len * 2);
	rt_memcpy(&cmd[7], data,  cmd[6]);
	crc = MBCRC16(cmd, 7 + (len * 2));
	cmd[7 + (len * 2)] = (uint8)(crc & 0x00ff);
	cmd[8 + (len * 2)] = (uint8)(crc >> 8);
	UART_Send(com, cmd, 9 + (len * 2));
	rt_free(cmd);
}

void MBFuncWriteSingleHoldingRegister(COM com, uint8 dev_addr, uint16 start_addr, uint8* data)
{
	uint16 crc;
	uint8 cmd[8];
	
	ModbusSync[com].dev_addr = dev_addr;
	ModbusSync[com].addr     = start_addr;
	ModbusSync[com].len 		= 1;
	ModbusSync[com].fnc      = 0x06;
	
	cmd[0] = dev_addr;
	cmd[1] = 0x06;
	cmd[2] = (uint8)(start_addr >> 8);
	cmd[3] = (uint8)(start_addr  & 0x00FF);
	cmd[4] = data[0];
	cmd[5] = data[1];
	crc = MBCRC16(cmd, 6);
	cmd[6] = (uint8)(crc & 0x00ff);
	cmd[7] = (uint8)(crc >> 8);
	UART_Send(com, cmd, 8);
}


void MBFuncReadHoldingRegister(COM com, uint8 dev_addr, uint16 start_addr, uint16 len)
{
	uint8 cmd[8];
	uint16 crc;
	ModbusSync[com].dev_addr = dev_addr;
	ModbusSync[com].addr     = start_addr;
	ModbusSync[com].len 		= len;
	ModbusSync[com].fnc      = 0x03;
	cmd[0] = dev_addr;
	cmd[1] = 0x03;
	cmd[2] = (uint8)(start_addr >> 8);
	cmd[3] = (uint8)(start_addr  & 0x00FF);
	cmd[4] = (uint8)(len >> 8);
	cmd[5] = (uint8)(len  & 0x00FF);
	crc = MBCRC16(cmd, 6);
	cmd[6] = (uint8)(crc & 0x00ff);
	cmd[7] = (uint8)(crc >> 8);
	UART_Send(com, cmd, 8);
}


void CheckMod(void)
{
	uint8 i;

	for(i=0; i<MODMAX; i++)
	{
		if(rt_sem_take(dynamic_sem, 30) == RT_EOK)
		{
			MBFuncReadHoldingRegister(COM3,i + 1, 0x00, 1);
		}
		else
		{
			rt_sem_release(dynamic_sem); //超时释放
		}
	}

	
}

float ToFloat(uint32 value)
{
	uint8 i = 0;
	uint32 temp = value;
	
	while(temp)
	{
		temp /= 10;
		i++;
	}
	return value / pow(10, i);
}

uint8 TH_compare(uint32* new, uint32* older, uint16 size)
{
	#define T_Diff 0.5f   //温度差值
	#define H_Diff 5.0f   //湿度差值 百分比

	float temperature, humidity, temperature_older, humidity_older;
	uint16 i;
	uint8  res = 0;
	for(i = 0; i < size; i++)
	{
		temperature 	  = ((new[2*i] & (uint32)(0xFF<<24)) == (uint32)(0x01<<24) ? 1.0f : -1.0f) * ((float)((new[2*i] & (uint32)(0xFF<<16)) >> 16) + ToFloat((new[2*i] & (uint32)(0xFF<<8)) >> 8));
		temperature_older = ((older[2*i] & (uint32)(0xFF<<24)) == (uint32)(0x01<<24) ? 1.0f : -1.0f) * ((float)((older[2*i] & (uint32)(0xFF<<16)) >> 16) + ToFloat((older[2*i] & (uint32)(0xFF<<8)) >> 8));
        humidity 	  = ((new[2*i+1] & (uint32)(0xFF<<24)) == (uint32)(0x01<<24) ? 1.0f : -1.0f) * ((float)((new[2*i+1] & (uint32)(0xFF<<16)) >> 16) + ToFloat((new[2*i+1] & (uint32)(0xFF<<8)) >> 8));
		humidity_older = ((older[2*i+1] & (uint32)(0xFF<<24)) == (uint32)(0x01<<24) ? 1.0f : -1.0f) * ((float)((older[2*i+1] & (uint32)(0xFF<<16)) >> 16) + ToFloat((older[2*i+1] & (uint32)(0xFF<<8)) >> 8)); 
        if((fabs(temperature - temperature_older) >= T_Diff) || (fabs(humidity - humidity_older) >= H_Diff))
        {
            res = 1;
            break;
        }
	}
	return res;
}


extern unsigned char pub_test_flag;//测试用发布标志
extern unsigned char lab_ena_flag;//使能发布标志 
extern unsigned char th_ena_flag;//使能发布标志 
extern unsigned char ack_ena_flag;//使能发布标志 
extern unsigned char module;         //模块标号


//======主题1：上报标签变化，一有某个U位状态发生变化，则上报设备目前有U位上架状态或者警报状态的所有信息。
extern unsigned char *pub_lab_data;
Lab_Sta_Head sta_head;
u8 lab_sta_buff[333] = {0};
/*数组大小（328）：模块地址(1byte)+门磁状态（1byte）+U位总数(1byte)+上报的u数(1byte)+
[U位号(1byte)+有无警报(1byte)+标签码(4byte)]*n   最大若上报54U的时候就273给字节
*/

void edit_lab_data(u8* Usta_data, unsigned long * Uid_data, u16* Uwarn_data, u8 U_addr, u8 U_count, u32 M_ID)
{	
	sta_head.module_addr = U_addr;      //0x03;//模块地址
	sta_head.mag_state = 0xff;          //门磁状态，0xFF无， 0x01开， 0x00关
	sta_head.U_mount = U_count;         //0x36;//总U数
	sta_head.module_ID = M_ID;
	u8 us_mount = 0,i = 0,m=0;
	
	for(i=0; i<U_count; i++)
	{
		if(Usta_data[i] == 0x01 || (Uwarn_data[i] == 0x0001))
		{
			us_mount++; //统计需要上报多少个U位
		}
	}
	sta_head.U_State_mount = us_mount;  //0x02;//有上架状态或者警报状态的U位数
	
	u8 sta_pack_len = 0,temp_flag = 0;
	sta_pack_len = sta_head.U_State_mount;
	Lab_Sta_Pack sta_pack[sta_pack_len];
	
	for(i=0,m=0; i<U_count; i++)
	{
		if(Usta_data[i] == 0x01)//有标签
		{
			sta_pack[m].U_number = i+1;
			sta_pack[m].Call_State   = (u8)(Uwarn_data[i] & 0x00FF);
			
			sta_pack[m].Lable_Tag[0] = (u8)(Uid_data[i]  & 0x000000FF) ;
			sta_pack[m].Lable_Tag[1] = (u8)((Uid_data[i] & 0x0000FF00) >> 8);
			sta_pack[m].Lable_Tag[2] = (u8)((Uid_data[i] & 0x00FF0000) >> 16);
			sta_pack[m].Lable_Tag[3] = (u8)((Uid_data[i] & 0xFF000000) >> 24);
			
			m++;
		}
		
		if(Uwarn_data[i] == 0x0001 && Usta_data[i] == 0x00)//无标签有警报
//		if(Uwarn_data[i] == 0x0001)//无标签有警报
		{
			sta_pack[m].U_number = i+1;
			sta_pack[m].Call_State   = (u8)(Uwarn_data[i] & 0x00FF);
			
			sta_pack[m].Lable_Tag[0] = (u8)(Uid_data[i]  & 0x000000FF) ;
			sta_pack[m].Lable_Tag[1] = (u8)((Uid_data[i] & 0x0000FF00) >> 8);
			sta_pack[m].Lable_Tag[2] = (u8)((Uid_data[i] & 0x00FF0000) >> 16);
			sta_pack[m].Lable_Tag[3] = (u8)((Uid_data[i] & 0xFF000000) >> 24);
			
			m++;
		}
	}
	
	lab_sta_buff[0] = 0xBB;
	lab_sta_buff[1] = sta_head.module_addr;
	lab_sta_buff[6] = sta_head.mag_state;
	lab_sta_buff[7] = sta_head.U_mount;
	lab_sta_buff[8] = sta_head.U_State_mount;
	my_memcpy(&lab_sta_buff[2], (char*)&sta_head.module_ID, 4, 4);

	for(i=0,m=0; i<sta_head.U_State_mount; i++,m++)
	{
		lab_sta_buff[9+6*i] = sta_pack[m].U_number;
		lab_sta_buff[10+6*i] = sta_pack[m].Call_State;
		lab_sta_buff[11+6*i] = sta_pack[m].Lable_Tag[0];
		lab_sta_buff[12+6*i] = sta_pack[m].Lable_Tag[1];
		lab_sta_buff[13+6*i] = sta_pack[m].Lable_Tag[2];
		lab_sta_buff[14+6*i] = sta_pack[m].Lable_Tag[3];
	}
	
}

extern unsigned char *pub_th_data;
unsigned char Tem_Hum_buff[35];
unsigned char Tem_Hum_num[6] = {0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
Tem_Hum_Pack tem_hum_pack;
//编辑温湿度信息======
void edit_TH_data(void)
{	
	
	u8 i = 0;
	char PN = 1;
	Tem_Hum_buff[0] = module+1;
	my_memcpy(&Tem_Hum_buff[1], (unsigned char*)&UW[module].M_ID, 4, 4);

	for(i=0; i<6; i++)
	{
		if( (UW[module].TemperatureHumidityBack[2*i])>>24 & 0x00000001 )
		{
			PN = 1;//温度值为正数
		}
		else
		{
			PN = -1;//温度值为负数
		}
		
		Tem_Hum_buff[5*i+5] = Tem_Hum_num[i];
	//===温度数据
		Tem_Hum_buff[5*i+6] = (char)((UW[module].TemperatureHumidity[2*i] &   0x00FF0000) >> 16) * PN;
		Tem_Hum_buff[5*i+7] = (char)((UW[module].TemperatureHumidity[2*i] &   0x0000FF00) >> 8);
		
	//===湿度数据==
		Tem_Hum_buff[5*i+8] = (char)((UW[module].TemperatureHumidity[2*i+1] & 0x00FF0000) >> 16);
		Tem_Hum_buff[5*i+9] = (char)((UW[module].TemperatureHumidity[2*i+1] & 0x0000FF00) >> 8);
	}
}

uint8 iszero(uint8* data, uint16 len)
{
	uint16 i;
	for(i = 0; i < len; i++)
	{
		if(data[i] != 0x00)
			return 0;
	}
	return 1;
}

void StatusFromBit(uint8* byte, uint8* bit, uint8 len)
{
      uint8 i,j;
	  if(len > 54)
	  	len = 54;
       for(i=0;i<len;i++)
       {
            j = i / 8;
            if(bit[j] & (1<<(7-i%8)))
            {
                byte[i] = 0x01;
            }
			else
			{
				 byte[i] = 0x00;
			}
       }
}

void CardIDFormStatus(uint32* CardID, uint8* data,uint8* Status, uint8 len)
{
	uint8 i,k=0;
	if(len > 54)
		len = 54;
	
	for(i=0;i<len;i++)
	{
		if(Status[i] == 1)
		{
			memcpy((char*)&CardID[i], (char*)&data[k*4], 4);
            k++;
		}
		else
		{
			CardID[i] = 0;
		}
	}
}

void WarnFromBit(uint16* warn, uint8* bit, uint8 len)
{
	 uint8 i,j;
	 if(len > 54)
	 	len = 54;
     for(i=0;i<len;i++)
     {
          j = i / 8;
          if(bit[j] & (1<<(7-i%8)))
          {
              warn[i] = 0x01;
          }
		  else
		  {
		  		warn[i] = 0x00;
		  }
     }
}

uint8 LableCount(uint8* bit, uint8 len)
{
	uint8 i, j, count = 0;
	
	if(len > 54)
	 	len = 54;
     for(i=0;i<len;i++)
     {
          j = i / 8;
          if(bit[j] & (1<<(7-i%8)))
          {
              count++;
          }
     }
	 return count;
}

u8 TH_moudle_flag = 0;
u8 heart_buff01[10] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A};//心跳包组成1，存放模块编号
u32 heart_buff02[10] = {0};//心跳包组成2，存放主模块ID
u8 heart_buff03[10] = {0};//心跳包组成3，存放模块U位数


//函数功能：处理modbus的响应结果
void ModbusHandler(COM com, uint8* fram)
{
	static uint8 u_flag = 0;
	static uint8 id_flag = 0;
	uint8 i = 0;
	if(com == COM3)
		i = fram[0] - 1;
	
	if(i == 0)
	{
		memset(heart_buff02, 0, sizeof(heart_buff02));
		memset(heart_buff03, 0, sizeof(heart_buff03));
	}
	
	IWDG_Feed();
	
	if((fram[1] == 0x03) && (ModbusSync[com].fnc == 0x03))
	{
		
		if(ModbusSync[com].addr == 0)
		{
			UW[i].Online 	= 1;      //模块在线 
			UW[i].M_Len  	= fram[3];
			UW[i].U_Count   = fram[4];
			UW[i].M_Address = i + 1;
			
		}
		else if((ModbusSync[com].addr >= 3) && (ModbusSync[com].addr <= 4)) //模块ID
		{
			my_memcpy(&UW[i].M_ID , &fram[3], 4, fram[2]);
			
			heart_buff02[i] = UW[i].M_ID;
			heart_buff03[i] = UW[i].U_Count;
		}
		else if((ModbusSync[com].addr >= 10) && (ModbusSync[com].addr <= 36)) //U位状态
		{
			u_flag = 0;
			#if (PROTOCOL == 1)
			StatusFromBit(&UW[i].U_Status[(ModbusSync[com].addr - 10) * 2], &fram[3], fram[2] * 8);
			HoldingReg[3].regLen = LableCount(&fram[3], fram[2] * 8) * 2;  //只获取有标签的
            if(HoldingReg[3].regLen == 0)
               HoldingReg[3].regLen = 2; 
			#else
			memcpy(&UW[i].U_Status[(ModbusSync[com].addr - 10) * 2] , &fram[3], fram[2]);
			#endif
//			if(memcmp(UW[i].U_Status, UW[i].U_StatusBack, 54) != 0)
			if(edit_ustaus_log(UW[i].U_Status, UW[i].U_StatusBack, UW[i].M_Address, UW[i].U_Count) == 1)
			{
				u_flag = 1;
//				lab_ena_flag = 1;
				module = i;
                memcpy(UW[i].U_StatusBack, UW[i].U_Status,54);
			}
		}
		else if((ModbusSync[com].addr >= 499) && (ModbusSync[com].addr <= 522))//温湿度
		{
			if(iszero(&fram[3], fram[2]) == 0)
			{
				TH_moudle_flag = 1;
				my_memcpy(UW[i].TemperatureHumidity , &fram[3], 4, fram[2]);
				//if(TH_compare(UW[i].TemperatureHumidity, UW[i].TemperatureHumidityBack, 6) == 1)
				{
					memcpy(UW[i].TemperatureHumidityBack, UW[i].TemperatureHumidity, 48);
					
					edit_TH_data();
					pub_th_data = (unsigned char *)&Tem_Hum_buff[0];
					th_ena_flag  = 1;
					module       = i;
				}
			}
			else
			{
				TH_moudle_flag = 0;
			}
			
		}
		else if((ModbusSync[com].addr >= 64) && (ModbusSync[com].addr <= 171))
		{
			#if (PROTOCOL == 1)
			CardIDFormStatus(UW[i].U_CardID, &fram[3], UW[i].U_Status, 54);
			#else
			memcpy(&UW[i].U_CardID[(ModbusSync[com].addr - 64) / 2] , &fram[3], fram[2]);
			#endif
			
			if((memcmp(UW[i].U_CardID, UW[i].U_CardIDBack, 216) != 0) || (u_flag == 1))
			{
				u_flag = 0;
				id_flag = 1;
                memcpy(UW[i].U_CardIDBack, UW[i].U_CardID, 216);
//				lab_ena_flag = 1;
				module       = i;
			}
		}
		else if((ModbusSync[com].addr >= 199) && (ModbusSync[com].addr <= 252))
		{
			#if (PROTOCOL == 1)
			WarnFromBit(&UW[i].U_Warn[ModbusSync[com].addr - 199], &fram[3], fram[2] * 8);
			#else
			my_memcpy(&UW[i].U_Warn[ModbusSync[com].addr - 199] , &fram[3], 2, fram[2]);
			#endif
//			if((memcmp(UW[i].U_Warn, UW[i].U_WarnBack, 54 * 2) != 0)  || (id_flag == 1))
			if(edit_uwarn_log(UW[i].U_Warn, UW[i].U_WarnBack, UW[i].M_Address, UW[i].U_Count) == 1 || id_flag == 1)
			{
				id_flag = 0;
                memcpy(UW[i].U_WarnBack, UW[i].U_Warn, 54 * 2);
				
				//整合数据
				edit_lab_data(UW[i].U_Status, UW[i].U_CardID, UW[i].U_Warn, UW[i].M_Address, UW[i].U_Count, UW[i].M_ID);
				lab_ena_flag = 1;
				module       = i;
			}
		}
		
		if(com == COM3)
			rt_sem_release(dynamic_sem);
		if(com == COM1)
			rt_sem_release(dynamic_sem3);
			
	}
	else if((fram[1] == 0x06) && (ModbusSync[com].fnc == 0x06))
	{
		rt_sem_release(dynamic_sem2);
	}
}

 void ModbusInit(void)
{
	uint8 i;
	UW[0].U_CardID = (uint32*)UW_CARDID1;
	UW[1].U_CardID = (uint32*)UW_CARDID2;
	UW[2].U_CardID = (uint32*)UW_CARDID3;
	UW[3].U_CardID = (uint32*)UW_CARDID4;
	UW[4].U_CardID = (uint32*)UW_CARDID5;	
    
	UW[0].U_CardIDBack = (uint32*)UW_CARDIDBACK1;
	UW[1].U_CardIDBack = (uint32*)UW_CARDIDBACK2;
	UW[2].U_CardIDBack = (uint32*)UW_CARDIDBACK3;
	UW[3].U_CardIDBack = (uint32*)UW_CARDIDBACK4;
	UW[4].U_CardIDBack = (uint32*)UW_CARDIDBACK5;
	
	UW[0].U_Status = (uint8*)UW_Index1;
	UW[1].U_Status = (uint8*)UW_Index2;
	UW[2].U_Status = (uint8*)UW_Index3;
	UW[3].U_Status = (uint8*)UW_Index4;
	UW[4].U_Status = (uint8*)UW_Index5;
	
	UW[0].U_StatusBack = (uint8*)UW_Index1Back;
	UW[1].U_StatusBack = (uint8*)UW_Index2Back;
	UW[2].U_StatusBack = (uint8*)UW_Index3Back;
	UW[3].U_StatusBack = (uint8*)UW_Index4Back;
	UW[4].U_StatusBack = (uint8*)UW_Index5Back;
    
	#if (MODMAX == 10)
	UW[5].U_CardID = (uint32*)UW_CARDID6;
	UW[6].U_CardID = (uint32*)UW_CARDID7;
	UW[7].U_CardID = (uint32*)UW_CARDID8;
	UW[8].U_CardID = (uint32*)UW_CARDID9;
	UW[9].U_CardID = (uint32*)UW_CARDID10;
	
	UW[5].U_CardIDBack = (uint32*)UW_CARDIDBACK6;
	UW[6].U_CardIDBack = (uint32*)UW_CARDIDBACK7;
	UW[7].U_CardIDBack = (uint32*)UW_CARDIDBACK8;
	UW[8].U_CardIDBack = (uint32*)UW_CARDIDBACK9;
	UW[9].U_CardIDBack = (uint32*)UW_CARDIDBACK10;
    
	UW[5].U_Status = (uint8*)UW_Index6;
	UW[6].U_Status = (uint8*)UW_Index7;
	UW[7].U_Status = (uint8*)UW_Index8;
	UW[8].U_Status = (uint8*)UW_Index9;
	UW[9].U_Status = (uint8*)UW_Index10;
    
	UW[5].U_StatusBack = (uint8*)UW_Index6Back;
	UW[6].U_StatusBack = (uint8*)UW_Index7Back;
	UW[7].U_StatusBack = (uint8*)UW_Index8Back;
	UW[8].U_StatusBack = (uint8*)UW_Index9Back;
	UW[9].U_StatusBack = (uint8*)UW_Index10Back;
	#endif
    
	for(i=0;i<MODMAX;i++)
	{
		UW[i].TemperatureHumidity     = (uint32*)TemperatureHumidity[i];
		UW[i].TemperatureHumidityBack = (uint32*)TemperatureHumidityBack[i];
		UW[i].U_Warn			      = (uint16*)Warn[i];
		UW[i].U_WarnBack			  = (uint16*)WarnBack[i];
	}
	
//	rt_sem_init(&sync_sem, "sync_sem", 1, RT_IPC_FLAG_FIFO);
//	rt_sem_init(&sync_sem2, "sync_sem2", 1, RT_IPC_FLAG_FIFO);
//	rt_sem_init(&sync_sem3, "sync_sem3", 1, RT_IPC_FLAG_FIFO);
//	rt_sem_init(&sync_sem4, "sync_sem4", 1, RT_IPC_FLAG_FIFO);
	
	dynamic_sem = rt_sem_create("dynamic_sem", 1, RT_IPC_FLAG_FIFO);
    if (dynamic_sem == RT_NULL)
    {
        rt_kprintf("create dynamic_sem semaphore failed.\n");
    }
	dynamic_sem2 = rt_sem_create("dynamic_sem2", 1, RT_IPC_FLAG_FIFO);
    if (dynamic_sem2 == RT_NULL)
    {
        rt_kprintf("create dynamic_sem2 semaphore failed.\n");
    }
	dynamic_sem3 = rt_sem_create("dynamic_sem3", 1, RT_IPC_FLAG_FIFO);
    if (dynamic_sem3 == RT_NULL)
    {
        rt_kprintf("create dynamic_sem3 semaphore failed.\n");
    }
	dynamic_sem4 = rt_sem_create("dynamic_sem4", 1, RT_IPC_FLAG_FIFO);
    if (dynamic_sem4 == RT_NULL)
    {
        rt_kprintf("create dynamic_sem4 semaphore failed.\n");
    }
}

#if (PROTOCOL == 1)
static stHoldingReg HoldingReg[] = 
{
	{//模块节数 U位数 
		0,
		1,
		60
	},
	{//模块ID
		3,
		2,
		80
	},
	{//U位状态
		10,
		4,
		80
	},
	{//标签ID
		64,
		108,
		200
	},
	{//温湿度
		499,
		24,
		100
	},
	{//警告
		199,
		4,
		100
	}
};
#else
static stHoldingReg HoldingReg[] = 
{
	{//模块节数 U位数 
		0,
		1,
		60
	},
	{//模块ID
		3,
		2,
		80
	},
	{//U位状态
		10,
		8,
		80
	},
	{//标签ID
		64,
		108,
		100
	},
	{//温湿度
		499,
		24,
		100
	},
	{//警告
		199,
		8,
		100
	}
};
#endif

void ModbusPoll(void)
{
	uint8 i,j,sem_wait_time;  
	static uint8 k = 0;
	for(i = 0; i < MODMAX; i++)
	{
		
		for(j = 0; j < sizeof(HoldingReg) / sizeof(stHoldingReg); )
		{
			if(j == 0)
			{
				sem_wait_time = HoldingReg[sizeof(HoldingReg) / sizeof(stHoldingReg) - 1].sem_wait_time;
			}
			else
			{
				sem_wait_time = HoldingReg[j - 1].sem_wait_time;
			}
			if(rt_sem_take(dynamic_sem2, 10) == RT_EOK)
			{
				if(rt_sem_take(dynamic_sem, sem_wait_time) == RT_EOK)
				{
					if((j > 0) && (UW[i].Online == 0))
					{
						rt_sem_release(dynamic_sem); //超时释放
						rt_sem_release(dynamic_sem2);
						break;
					}
					
						MBFuncReadHoldingRegister(COM3,i+1, HoldingReg[j].startReg, HoldingReg[j].regLen);
						
					j++;
				}
				else
				{
					if(j == 1) //已经发了获取U位数指令
					{
						UW[i].Online = 0;
						lab_sta_buff[0] = i+1;
						lab_sta_buff[1] = 0xFF;
						err_ena_flag = 1;  //数据获取失败上报
					}
					
					rt_sem_release(dynamic_sem); //超时释放
				}
				rt_sem_release(dynamic_sem2);
			}	
			
		}
	}
	if(rt_sem_take(dynamic_sem4, 10) == RT_EOK)
	{
		if(rt_sem_take(dynamic_sem3, 50) == RT_EOK)
		{
			MBFuncReadHoldingRegister(COM1, 10 + k++, 499, 4);
			if(k>=6)
				k=0;
//			rt_sem_release(dynamic_sem3);
		}
		else
		{
			rt_sem_release(dynamic_sem3);
		}
		rt_sem_release(dynamic_sem4);
	}
}


void modbus_thread_entry(void* parameter)
{

	while(1)
	{
		ModbusPoll();
		IWDG_Feed();
		
		rt_thread_mdelay(20);
		rt_thread_yield(); 			/* 放弃剩余时间片，进行一次线程切换 */
	}
}


