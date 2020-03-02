
#include <rtthread.h>
 
#define THREAD_STACK_SIZE	1024
#define THREAD_PRIORITY	    20
#define THREAD_TIMESLICE    10
 
/* 线程入口 */
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
            /* 不是新的时间片，就什么也不干 */
        }
        else
        {
            /* 一个新的时间片到来，就打印一次，count用来计算现在是第几个时间片 */
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
    /* 创建线程1 */
    tid = rt_thread_create("thread1", 
                            thread_entry, (void*)1, 
                            THREAD_STACK_SIZE, 
                            THREAD_PRIORITY, THREAD_TIMESLICE); 
    if (tid != RT_NULL) 
        rt_thread_startup(tid);
 
 
    /* 创建线程2 */
    tid = rt_thread_create("thread2", 
                            thread_entry, (void*)2,
                            THREAD_STACK_SIZE, 
                            THREAD_PRIORITY, THREAD_TIMESLICE-5);
    if (tid != RT_NULL) 
        rt_thread_startup(tid);
    return 0;

}