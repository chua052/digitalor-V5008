#ifndef __RTC_H
#define __RTC_H	    

#include "sys.h"
#include "Main.h"

//RTCʵʱʱ�� ��������			 
//2010/6/6
//��ֹʱ�����
#define RTCClockOutput_Enable		0
// extern uint8_t TimeAdjust;	//ʱ��У׼��־
#ifdef History_Event	
	
#endif

//ʱ��ṹ��
typedef struct 
{
	uint8_t hour;
	uint8_t min;
	uint8_t sec;			
	//������������
	uint16_t w_year;
	uint8_t  w_month;
	uint8_t  w_day;
	uint8_t  week;		 
}_calendar_obj;					 
extern _calendar_obj calendar;	//�����ṹ��
extern _calendar_obj new_calendar;//ʱ�ӽṹ�� 

extern u8 const mon_table[12];	//�·��������ݱ�
void Disp_Time(u8 x,u8 y,u8 size);//���ƶ�λ�ÿ�ʼ��ʾʱ��
void Disp_Week(u8 x,u8 y,u8 size,u8 lang);//��ָ��λ����ʾ����
u8 RTC_Init(u8 flag);        //��ʼ��RTC,����0,ʧ��;1,�ɹ�;
u8 Is_Leap_Year(u16 year);//ƽ��,�����ж�
u8 RTC_Get(void);         //����ʱ��   
u8 RTC_Get_Week(u16 year,u8 month,u8 day);
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);//����ʱ��			 
#endif


