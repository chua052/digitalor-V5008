/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2006-08-31     Bernard      first implementation
 */

#include <rthw.h>
#include <rtthread.h>

#include "board.h"
#include "usart.h"
#include "device.h"

/**
 * @addtogroup STM32
 */

/*@{*/

extern int  rt_application_init(void);

#if defined(__CC_ARM) || defined(__CLANG_ARM)
extern int Image$$RW_IRAM1$$ZI$$Limit;
#elif __ICCARM__
#pragma section="HEAP"
#else
extern int __bss_end;
#endif

/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{
    rt_kprintf("\n\r Wrong parameter value detected on\r\n");
    rt_kprintf("       file  %s\r\n", file);
    rt_kprintf("       line  %d\r\n", line);

    while (1) ;
}
extern void ModbusInit(void);
extern void w5500_net_init(void);
/**
 * This function will startup RT-Thread RTOS.
 */
void rtthread_startup(void)
{
    /* init board */
    rt_hw_board_init();
	
    /* show version */
    rt_show_version();

#ifdef RT_USING_HEAP
#if STM32_EXT_SRAM
    rt_system_heap_init((void*)STM32_EXT_SRAM_BEGIN, (void*)STM32_EXT_SRAM_END);
#else
#if defined(__CC_ARM) || defined(__CLANG_ARM)
    rt_system_heap_init((void*)&Image$$RW_IRAM1$$ZI$$Limit, (void*)STM32_SRAM_END);
#elif __ICCARM__
    rt_system_heap_init(__segment_end("HEAP"), (void*)STM32_SRAM_END);
#else
    /* init memory system */
    rt_system_heap_init((void*)&__bss_end, (void*)STM32_SRAM_END);
#endif
#endif  /* STM32_EXT_SRAM */
#endif /* RT_USING_HEAP */

    /* init scheduler system */
    rt_system_scheduler_init();

    /* initialize timer */
    rt_system_timer_init();

    /* init timer thread */
    rt_system_timer_thread_init();
	
    /* init application */
    rt_application_init();

    /* init idle thread */
    rt_thread_idle_init();
	
	
	ModbusInit();
	
    /* start scheduler */
    rt_system_scheduler_start();

    /* never reach here */
    return ;
}

uint8_t domain_ip[4]={192,168,0,240};//115,29,225,215  //MQTT云服务器IP地址   //192,168,0,240    101,37,79,55
uint16_t service_port = 1883;
uint16_t pub_port = 5001;
uint16_t sub_port = 50000;

char TH_Topic[] = "TemHum";//温湿度主题名字，“TemHum”,数组大小需多留一个字节
char LS_Topic[] = "LabelState";//标签变化主题名字，“LabelState”,数组大小需多留一个字节
char ACK_Topic[] = "OpeAck";//操作应答主题名字，“OpeAck”,数组大小需多留一个字节
char IP_Topic[] = "IpUpdata";//提示更改IP地址

char Pub_Test_Topic[17] = {"V5008/2900644827"};//操作应答主题名字，“OpeAck”,数组大小需多留一个字节
char Pub_LS_Topic[40] = {0};
char Pub_TH_Topic[30] = {0};
char Pub_ACK_Topic[30] = {0};
char Pub_IP_Topic[30] = {0};

int main(void)
{
    /* disable interrupt first */
    rt_hw_interrupt_disable();

    /* startup RT-Thread RTOS */
    rtthread_startup();

    return 0;
}


/*@}*/
