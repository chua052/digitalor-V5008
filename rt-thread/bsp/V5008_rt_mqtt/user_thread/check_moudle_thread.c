
#include <rtthread.h>

#include "mqtt.h"
#include "modbus.h"
#include "mini_beep.h"
 
#define THREAD_STACK_SIZE	1024
#define THREAD_PRIORITY	    20
#define THREAD_TIMESLICE    10

unsigned char pub_test_flag = 1;//�����÷�����־ 
unsigned char lab_ena_flag = 0;//ʹ�ܷ�����־ 
unsigned char th_ena_flag = 0;//ʹ�ܷ�����־ 
unsigned char ack_ena_flag = 0;//ʹ�ܷ�����־
unsigned char module;          //ģ����
unsigned char result_falg = 0xA1;
unsigned char err_ena_flag = 0;//���ݻ�ȡʧ�ܷ�����־ 

extern unsigned char Ctrl_lab_flag;//�ص������־λ 
extern unsigned char Realse_call_flag;//���������־λ 
extern unsigned char Set_THled_flag;//������ʪ��ģ��LED����ɫ��־λ

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

/* �߳���� */
void check_moudle_entry(void* parameter)
{
//    rt_uint32_t value = 1;
	uint8 res = 0;
    while (1)
    {
		//������������
        
		//���շ������·������������ģ���ִ�в���
		if(Ctrl_lab_flag == 1)
		{
			Ctrl_lab_flag = 0;
			rt_kprintf("�����ص���ز���------>\r\n");
			/*****����ִ��****/
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
			
			
			/*****��������ɽ�������Ӧ���������mqtt*****/
			if(res)//===��ȡ��Ӧ�ļĴ�����ֵ���жϲ����Ƿ�ȷʵִ�е�λ
			{
				unsigned char result = 0xA1;//�ɹ�
				result_falg = 0xA1;
				edit_ack_data(result);
				ack_ena_flag = 1;
			}
			else
			{
				unsigned char result = 0xA0;//ʧ��
				result_falg = 0xA0;
				edit_ack_data(result);
				ack_ena_flag = 1;
			}
		}
		
		if(Realse_call_flag == 1)
		{
			Realse_call_flag = 0;
			rt_kprintf("�������������ز���------>\r\n");
			/*****����ִ��****/
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
			
			/*****��������ɽ�������Ӧ���������mqtt*****/
			if(res)//===��ȡ��Ӧ�ļĴ�����ֵ���жϲ����Ƿ�ȷʵִ�е�λ
			{
				unsigned char result = 0xA1;//�ɹ�
				result_falg = 0xA1;
				edit_ack_data(result);
				ack_ena_flag = 1;
			}
			else
			{
				unsigned char result = 0xA0;//ʧ��
				result_falg = 0xA0;
				edit_ack_data(result);
				ack_ena_flag = 1;
			}
			
		}
		
		if(Set_THled_flag == 1)
		{
			Set_THled_flag = 0;
			rt_kprintf("����������ʪ��ģ��led����ɫ��ز���------>\r\n");
			/*****����ִ��****/
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
							if(TH_moudle_flag == 1)//�ô����н�ģ��ʱ�ŵ���1
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
							if(TH_moudle_flag == 1)//�ô����н�ģ��ʱ�ŵ���1
							{
								res = 1;
							}
						}
					}
					rt_sem_release(dynamic_sem3);
					rt_sem_release(dynamic_sem4);
				}
				
			}
			
			/*****��������ɽ�������Ӧ���������mqtt*****/
			if(res)//===��ȡ��Ӧ�ļĴ�����ֵ���жϲ����Ƿ�ȷʵִ�е�λ
			{
				unsigned char result = 0xA1;//�ɹ�
				result_falg = 0xA1;
				edit_ack_data(result);
				ack_ena_flag = 1;
			}
			else
			{
				unsigned char result = 0xA0;//ʧ��
				result_falg = 0xA0;
				edit_ack_data(result);
				ack_ena_flag = 1;
			}
			
		}
		
		rt_thread_mdelay(20);
		rt_thread_yield(); 			/* ����ʣ��ʱ��Ƭ������һ���߳��л� */
     } 
}
 
int timeslice_sample()
{
    rt_thread_t tid;
    /* �����߳�1 */
    tid = rt_thread_create("thread1", 
                            check_moudle_entry, (void*)1, 
                            THREAD_STACK_SIZE, 
                            THREAD_PRIORITY, THREAD_TIMESLICE); 
    if (tid != RT_NULL) 
        rt_thread_startup(tid);
 
 
    /* �����߳�2 */
    tid = rt_thread_create("thread2", 
                            check_moudle_entry, (void*)2,
                            THREAD_STACK_SIZE, 
                            THREAD_PRIORITY, THREAD_TIMESLICE-5);
    if (tid != RT_NULL) 
        rt_thread_startup(tid);
    return 0;

}

