#include <rtthread.h>

#include "mqtt.h"
#include "modbus.h"
#include "stmflash.h"
#include "main.h"
#include "rtc.h"
#include "iwdg.h"



extern unsigned char connet_flag;

/******
һ����־��Ϣ����
�꣨2��+�£�1��+�գ�1��+ʱ��1��+�֣�1��+�루1��+ģ���ַ(1)+Uλ�ţ�1��+���¼ܻ򾯱���1��
10���ֽڣ����浽flashʱ���ڼ�������־���ͱ�־������Ϊ11�ֽ�
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
	unsigned char status;//�ϼ� 0xB1 �¼� 0xB2 ���� 0xB3 
}LogMessage;

//����־��Ϣת���ɵ��ֽ�ģʽ�����ڵ�һ���ֽڼ��ϱ�־λ��0xA1Ϊ��ͨ���棬0xA2Ϊ��������
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
u8 log_cache_pos;//��¼��ͨ��־�����±�λ��
u8 log_back_pos = 0;//��¼����������־�±�λ��
u16 pub_pos = 0;//��¼��ģ������仯ʱ��logmessage[]���±굱ǰλ�ã����ں�����������û�յ����ݣ�ͳ����Ҫ���浽���ػ������־��Ϣ��
extern unsigned char lab_ena_flag;//ʹ�ܷ�����־

//�༭��ǩ��ǩ�仯��־��Ϣ��������uλǰ״̬��uλ��״̬��ģ���ַ,Uλ��
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
			if(ustatus_new[i] == 0x01 && ustatus_old[i] == 0x00)//uλ�ϼ�
			{
				logmessage[log_cache_pos].status = 0xB1;
			}
			else//Uλ�¼�
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
			
			if(connet_flag == 0)//����ǰ�������ӶϿ����򽫴���־��Ϣ���浽���ر��ݿռ�
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
	return cmp_falg;//����1������б仯
}

//�༭uλ������־��Ϣ��������uλǰ״̬��uλ��״̬��ģ���ַ,Uλ��
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
			if(uwarn_new[i] == 0x0001 && uwarn_old[i] == 0x0000)//uλ����
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
			
			if(connet_flag == 0)//����ǰ�������ӶϿ����򽫴���־��Ϣ���浽���ر��ݿռ�
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
	return cmp_falg;//����1������б仯
}

/* �߳���� */
void log_entry(void* parameter)
{
    while (1)
    {
		if(lab_ena_flag == 1)//������ǩ�仯������Ϣ
		{
			
		}
	}
}

