#ifndef __RTC_H
#define __RTC_H	    

#include "sys.h"
#include "Main.h"

//RTC实时时钟 驱动代码			 
//2010/6/6
//禁止时钟输出
#define RTCClockOutput_Enable		0
// extern uint8_t TimeAdjust;	//时间校准标志
#ifdef History_Event	
	
#endif

//时间结构体
typedef struct 
{
	uint8_t hour;
	uint8_t min;
	uint8_t sec;			
	//公历日月年周
	uint16_t w_year;
	uint8_t  w_month;
	uint8_t  w_day;
	uint8_t  week;		 
}_calendar_obj;					 
extern _calendar_obj calendar;	//日历结构体
extern _calendar_obj new_calendar;//时钟结构体 

extern u8 const mon_table[12];	//月份日期数据表
void Disp_Time(u8 x,u8 y,u8 size);//在制定位置开始显示时间
void Disp_Week(u8 x,u8 y,u8 size,u8 lang);//在指定位置显示星期
u8 RTC_Init(u8 flag);        //初始化RTC,返回0,失败;1,成功;
u8 Is_Leap_Year(u16 year);//平年,闰年判断
u8 RTC_Get(void);         //更新时间   
u8 RTC_Get_Week(u16 year,u8 month,u8 day);
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);//设置时间			 
#endif


