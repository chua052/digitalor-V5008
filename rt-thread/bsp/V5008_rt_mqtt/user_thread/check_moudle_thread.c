
#include <rtthread.h>

#include "mqtt.h"
#include "modbus.h"
#include "mini_beep.h"
 
#define THREAD_STACK_SIZE	1024
#define THREAD_PRIORITY	    20
#define THREAD_TIMESLICE    10

unsigned char pub_test_flag = 1;//测试用发布标志 
unsigned char lab_ena_flag = 0;//使能发布标志 
unsigned char th_ena_flag = 0;//使能发布标志 
unsigned char ack_ena_flag = 0;//使能发布标志
unsigned char module;          //模块标号
unsigned char result_falg = 0xA1;
unsigned char err_ena_flag = 0;//数据获取失败发布标志 

extern unsigned char Ctrl_lab_flag;//控灯命令标志位 
extern unsigned char Realse_call_flag;//解除警报标志位 
extern unsigned char Set_THled_flag;//设置温湿度模块LED灯颜色标志位

extern unsigned char pMessage_cache[4];

extern stModbus ctrl_led_data;
extern stModbus rea_call_data;
extern stModbus set_thled_data;
extern struct rt_semaphore sync_sem;
extern struct rt_semaphore sync_sem2;
extern struct rt_semaphore sync_sem3;
extern struct rt_semaphore sync_sem4;

extern rt_sem_t dynamic_sem;
extern rt_sem_t dynamic_sem2;
extern rt_sem_t dynamic_sem3;
extern rt_sem_t dynamic_sem4;

extern unsigned char sem_flag;
extern u8 TH_moudle_flag;

/* 线程入口 */
void check_moudle_entry(void* parameter)
{
//    rt_uint32_t value = 1;
	uint8 res = 0;
    while (1)
    {
		//触发发布部分
        
		//接收服务器下发的命令，触发对模块的执行操作
		if(Ctrl_lab_flag == 1)
		{
			Ctrl_lab_flag = 0;
			rt_kprintf("触发控灯相关操作------>\r\n");
			/*****操作执行****/
			if(ctrl_led_data.mbfnc == FUN_06)
			{
				if(rt_sem_take(dynamic_sem, 100) == RT_EOK)
				{
					if(rt_sem_take(dynamic_sem2, 100) == RT_EOK)
					{
						
//						SetMiniBeepValue(1);
//						rt_thread_mdelay(5);
//						SetMiniBeepValue(0);
						
						MBFuncWriteSingleHoldingRegister(COM3,ctrl_led_data.dev_addr,
												 ctrl_led_data.start_addr,
												 ctrl_led_data.data);
						if(rt_sem_take(dynamic_sem2, 50) == RT_EOK)
						{
							res = 1;
						}
					}
					rt_sem_release(dynamic_sem);
					rt_sem_release(dynamic_sem2);
				}
			}
			
			
			/*****将操作完成结果打包成应答包发布给mqtt*****/
			if(res)//===读取相应的寄存器的值，判断操作是否确实执行到位
			{
				unsigned char result = 0xA1;//成功
				result_falg = 0xA1;
				edit_ack_data(result);
				ack_ena_flag = 1;
			}
			else
			{
				unsigned char result = 0xA0;//失败
				result_falg = 0xA0;
				edit_ack_data(result);
				ack_ena_flag = 1;
			}
		}
		
		if(Realse_call_flag == 1)
		{
			Realse_call_flag = 0;
			rt_kprintf("触发解除警报相关操作------>\r\n");
			/*****操作执行****/
			if(rea_call_data.mbfnc == FUN_06)
			{
					if(rt_sem_take(dynamic_sem2, 100) == RT_EOK)
					{
						
//						SetMiniBeepValue(1);
//						rt_thread_mdelay(5);
//						SetMiniBeepValue(0);
						
						MBFuncWriteSingleHoldingRegister(COM3, rea_call_data.dev_addr,
												 rea_call_data.start_addr,
												 rea_call_data.data);
						if(rt_sem_take(dynamic_sem2, 50) == RT_EOK)
						{
							res = 1;
						}
					}
					rt_sem_release(dynamic_sem2);
			}
			
			/*****将操作完成结果打包成应答包发布给mqtt*****/
			if(res)//===读取相应的寄存器的值，判断操作是否确实执行到位
			{
				unsigned char result = 0xA1;//成功
				result_falg = 0xA1;
				edit_ack_data(result);
				ack_ena_flag = 1;
			}
			else
			{
				unsigned char result = 0xA0;//失败
				result_falg = 0xA0;
				edit_ack_data(result);
				ack_ena_flag = 1;
			}
			
		}
		
		if(Set_THled_flag == 1)
		{
			Set_THled_flag = 0;
			rt_kprintf("触发设置温湿度模块led灯颜色相关操作------>\r\n");
			/*****操作执行****/
			if(set_thled_data.mbfnc == FUN_06)
			{
				if(rt_sem_take(dynamic_sem, 100) == RT_EOK)
				{
					if(rt_sem_take(dynamic_sem2, 100) == RT_EOK)
					{
						MBFuncWriteSingleHoldingRegister(COM3, set_thled_data.dev_addr,
												 set_thled_data.start_addr,
												 set_thled_data.data);
						if(rt_sem_take(dynamic_sem2, 50) == RT_EOK)
						{
							if(TH_moudle_flag == 1)//该串口有接模块时才等于1
							{
								res = 1;
							}
						}
					}
					rt_sem_release(dynamic_sem);
					rt_sem_release(dynamic_sem2);
				}
				
				if(rt_sem_take(dynamic_sem3, 100) == RT_EOK)
				{
					if(rt_sem_take(dynamic_sem4, 100) == RT_EOK)
					{
						set_thled_data.dev_addr = set_thled_data.data[0];
						MBFuncWriteSingleHoldingRegister(COM1, set_thled_data.dev_addr,
												 set_thled_data.start_addr,
												 set_thled_data.data);
						if(rt_sem_take(dynamic_sem4, 50) == RT_EOK)
						{
							if(TH_moudle_flag == 1)//该串口有接模块时才等于1
							{
								res = 1;
							}
						}
					}
					rt_sem_release(dynamic_sem3);
					rt_sem_release(dynamic_sem4);
				}
				
			}
			
			/*****将操作完成结果打包成应答包发布给mqtt*****/
			if(res)//===读取相应的寄存器的值，判断操作是否确实执行到位
			{
				unsigned char result = 0xA1;//成功
				result_falg = 0xA1;
				edit_ack_data(result);
				ack_ena_flag = 1;
			}
			else
			{
				unsigned char result = 0xA0;//失败
				result_falg = 0xA0;
				edit_ack_data(result);
				ack_ena_flag = 1;
			}
			
		}
		
		rt_thread_mdelay(20);
		rt_thread_yield(); 			/* 放弃剩余时间片，进行一次线程切换 */
     } 
}
 
int timeslice_sample()
{
    rt_thread_t tid;
    /* 创建线程1 */
    tid = rt_thread_create("thread1", 
                            check_moudle_entry, (void*)1, 
                            THREAD_STACK_SIZE, 
                            THREAD_PRIORITY, THREAD_TIMESLICE); 
    if (tid != RT_NULL) 
        rt_thread_startup(tid);
 
 
    /* 创建线程2 */
    tid = rt_thread_create("thread2", 
                            check_moudle_entry, (void*)2,
                            THREAD_STACK_SIZE, 
                            THREAD_PRIORITY, THREAD_TIMESLICE-5);
    if (tid != RT_NULL) 
        rt_thread_startup(tid);
    return 0;

}

