
#include <rtthread.h>
 
#define THREAD_STACK_SIZE	1024
#define THREAD_PRIORITY	    20
#define THREAD_TIMESLICE    10
 
/* �߳���� */
void thread_entry(void* parameter)
{
    rt_uint32_t value;
    rt_uint32_t time,old_time,count = 0;
 
    value = (rt_uint32_t)parameter;
    while (1)
    {
        old_time = rt_tick_get();
        if(old_time == time)
        {
            /* �����µ�ʱ��Ƭ����ʲôҲ���� */
        }
        else
        {
            /* һ���µ�ʱ��Ƭ�������ʹ�ӡһ�Σ�count�������������ǵڼ���ʱ��Ƭ */
            rt_kprintf("thread %d is running ,thread %d count = %d\n", value , value , count);
 
            count++;
            
            time = old_time;
            if(count > 29)
            {
               return;
            }
        }
     } 
}
 
int timeslice_sample()
{
    rt_thread_t tid;
    /* �����߳�1 */
    tid = rt_thread_create("thread1", 
                            thread_entry, (void*)1, 
                            THREAD_STACK_SIZE, 
                            THREAD_PRIORITY, THREAD_TIMESLICE); 
    if (tid != RT_NULL) 
        rt_thread_startup(tid);
 
 
    /* �����߳�2 */
    tid = rt_thread_create("thread2", 
                            thread_entry, (void*)2,
                            THREAD_STACK_SIZE, 
                            THREAD_PRIORITY, THREAD_TIMESLICE-5);
    if (tid != RT_NULL) 
        rt_thread_startup(tid);
    return 0;

}