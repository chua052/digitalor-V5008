#include <rtthread.h>

#include "mqtt.h"
#include "modbus.h"
#include "stmflash.h"
#include "main.h"
#include "rtc.h"
#include "iwdg.h"



extern unsigned char connet_flag;

/******
一条日志信息包括
年（2）+月（1）+日（1）+时（1）+分（1）+秒（1）+模块地址(1)+U位号（1）+上下架或警报（1）
10个字节，保存到flash时由于加上了日志类型标志，所以为11字节
******/

typedef struct			 					
{
	unsigned short year;
	unsigned char month;
	unsigned char day;
	unsigned char hour;
	unsigned char min;
	unsigned char sec;
	unsigned char mod_addr;
	unsigned char uw_addr;
	unsigned char status;//上架 0xB1 下架 0xB2 警报 0xB3 
}LogMessage;

//将日志信息转换成单字节模式，并在第一个字节加上标志位，0xA1为普通缓存，0xA2为断网缓存
void change_buff(LogMessage logmessage, u8 *buff, u8 cache_flag)
{
	buff[0] = cache_flag;
	buff[1] = logmessage.year>>8;
	buff[2] = logmessage.year;
	buff[3] = logmessage.month;
	buff[4] = logmessage.day;
	buff[5] = logmessage.hour;
	buff[6] = logmessage.min;
	buff[7] = logmessage.sec;
	buff[8] = logmessage.mod_addr;
	buff[9] = logmessage.uw_addr;
	buff[10] = logmessage.status;
}

LogMessage logmessage[170];
LogMessage logmessage_backup[170];
u8 log_cache_pos;//记录普通日志缓存下标位置
u8 log_back_pos = 0;//记录断网缓存日志下标位置
u16 pub_pos = 0;//记录当模块产生变化时，logmessage[]的下标当前位置，用于后续若服务器没收到数据，统计需要保存到本地缓存的日志信息数
extern unsigned char lab_ena_flag;//使能发布标志

//编辑标签标签变化日志信息，参数，u位前状态，u位现状态，模块地址,U位数
u8 edit_ustaus_log(u8 *ustatus_new, u8 *ustatus_old, u8 m_addr, u8 u_count)
{
	u8 i=0, cmp_falg=0;
	
	RTC_Get();
	IWDG_Feed();
	
	for(i=0; i<u_count; i++)
	{
		if(ustatus_new[i] != ustatus_old[i])
		{
			cmp_falg = 1;
			
			logmessage[log_cache_pos].year = calendar.w_year;
			logmessage[log_cache_pos].month = calendar.w_month;
			logmessage[log_cache_pos].day = calendar.w_day;
			logmessage[log_cache_pos].hour = calendar.hour;
			logmessage[log_cache_pos].min = calendar.min;
			logmessage[log_cache_pos].sec = calendar.sec;
			logmessage[log_cache_pos].mod_addr = m_addr;
			logmessage[log_cache_pos].uw_addr = i+1;
			if(ustatus_new[i] == 0x01 && ustatus_old[i] == 0x00)//u位上架
			{
				logmessage[log_cache_pos].status = 0xB1;
			}
			else//U位下架
			{
				logmessage[log_cache_pos].status = 0xB2;
			}
			
			u8 buff[12];
			u32 log_addr = (log_cache_pos*12) + LOG_cache_ADDR;
			change_buff(logmessage[log_cache_pos], buff, 0xA1);
			
			u16 new_buff[6] = {0};
			char_to_short(buff, new_buff, 12, 0);
			STMFLASH_Write(log_addr,new_buff,6);
			IWDG_Feed();
			
			if(connet_flag == 0)//若当前网络连接断开，则将此日志信息保存到本地备份空间
			{
				u8 buff[12];
				u32 log_addr = (log_back_pos*12) + LOG_backup_ADDR;
				change_buff(logmessage[log_cache_pos], buff, 0xA2);
				
				u16 new_buff[6] = {0};
				char_to_short(buff, new_buff, 12, 0);
				STMFLASH_Write(log_addr,new_buff,6);
				
				log_back_pos++;
				if(log_back_pos>168)
				{
					log_back_pos = 0;
				}
				
				IWDG_Feed();
			}
			
			log_cache_pos++;
			if(log_cache_pos>168)
			{
				log_cache_pos = 0;
			}
		}
	}
	return cmp_falg;//返回1则代表有变化
}

//编辑u位警报日志信息，参数，u位前状态，u位现状态，模块地址,U位数
u8 edit_uwarn_log(u16 *uwarn_new, u16 *uwarn_old, u8 m_addr, u8 u_count)
{
	u8 i=0, cmp_falg=0;
	RTC_Get();
	for(i=0; i<u_count; i++)
	{
		if(uwarn_new[i] != uwarn_old[i])
		{
			cmp_falg=1;
			
			logmessage[log_cache_pos].year = calendar.w_year;
			logmessage[log_cache_pos].month = calendar.w_month;
			logmessage[log_cache_pos].day = calendar.w_day;
			logmessage[log_cache_pos].hour = calendar.hour;
			logmessage[log_cache_pos].min = calendar.min;
			logmessage[log_cache_pos].sec = calendar.sec;
			logmessage[log_cache_pos].mod_addr = m_addr;
			logmessage[log_cache_pos].uw_addr = i+1;
			if(uwarn_new[i] == 0x0001 && uwarn_old[i] == 0x0000)//u位警报
			{
				logmessage[log_cache_pos].status = 0xB3;
			}
			
			u8 buff[12];
			u32 log_addr = log_cache_pos*12+LOG_cache_ADDR;
			change_buff(logmessage[log_cache_pos], buff, 0xA1);
//			FlashWriteData(log_addr ,buff, 12);
//			FlashPageWrite(log_addr,buff,12,LOG_cache_ADDR);
			
			u16 new_buff[6] = {0};
			char_to_short(buff, new_buff, 12, 0);
			STMFLASH_Write(log_addr,new_buff,6);
			
			if(connet_flag == 0)//若当前网络连接断开，则将此日志信息保存到本地备份空间
			{
				u8 buff[12];
				u32 log_addr = log_back_pos*12+LOG_backup_ADDR;

				change_buff(logmessage[log_cache_pos], buff, 0xA2);
//				FlashWriteData( log_addr ,buff, 12);
//				FlashPageWrite(log_addr, buff, 12, LOG_backup_ADDR);
				
				u16 new_buff[6] = {0};
				char_to_short(buff, new_buff, 12, 0);
				STMFLASH_Write(log_addr,new_buff,6);
				
				log_back_pos++;
				if(log_back_pos>168)
				{
					log_back_pos = 0;
				}
				
				IWDG_Feed();
			}
			
			log_cache_pos++;
			if(log_cache_pos>168)
			{
				log_cache_pos = 0;
			}
		}
	}
	return cmp_falg;//返回1则代表有变化
}

/* 线程入口 */
void log_entry(void* parameter)
{
    while (1)
    {
		if(lab_ena_flag == 1)//触发标签变化发布消息
		{
			
		}
	}
}

