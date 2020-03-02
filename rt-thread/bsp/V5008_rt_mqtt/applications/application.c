/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 * 2013-07-12     aozima       update for auto initial.
 */

/**
 * @addtogroup STM32
 */
/*@{*/

#include <board.h>
#include <rtthread.h>



#ifdef RT_USING_DFS
/* dfs filesystem:ELM filesystem init */
#include <dfs_elm.h>
/* dfs Filesystem APIs */
#include <dfs_fs.h>
#endif

#ifdef RT_USING_RTGUI
#include <rtgui/rtgui.h>
#include <rtgui/rtgui_server.h>
#include <rtgui/rtgui_system.h>
#include <rtgui/driver.h>
#include <rtgui/calibration.h>
#endif

#include <rtdbg.h>

#include "led.h"
#include "w5500.h"
#include "global.h"
#include "delay.h"
#include "stmflash.h"
#include "rtc.h"
#include "iwdg.h"
//ALIGN(RT_ALIGN_SIZE)



#ifdef RT_USING_RTGUI
rt_bool_t cali_setup(void)
{
    rt_kprintf("cali setup entered\n");
    return RT_FALSE;
}

void cali_store(struct calibration_data *data)
{
    rt_kprintf("cali finished (%d, %d), (%d, %d)\n",
               data->min_x,
               data->max_x,
               data->min_y,
               data->max_y);
}
#endif /* RT_USING_RTGUI */


void rt_init_thread_entry(void* parameter)
{
#ifdef RT_USING_COMPONENTS_INIT
    /* initialization RT-Thread Components */
    rt_components_init();
#endif

    /* Filesystem Initialization */
//#if defined(RT_USING_DFS) && defined(RT_USING_DFS_ELMFAT)
//    /* mount sd card fat partition 1 as root directory */
//    if (dfs_mount("sd0", "/", "elm", 0, 0) == 0)
//    {
//        rt_kprintf("File System initialized!\n");
//    }
//    else
//        rt_kprintf("File System initialzation failed!\n");
//#endif  /* RT_USING_DFS */

#ifdef RT_USING_RTGUI
    {
        extern void rt_hw_lcd_init();
        extern void rtgui_touch_hw_init(void);

        rt_device_t lcd;

        /* init lcd */
        rt_hw_lcd_init();

        /* init touch panel */
        rtgui_touch_hw_init();

        /* find lcd device */
        lcd = rt_device_find("lcd");

        /* set lcd device as rtgui graphic driver */
        rtgui_graphic_set_device(lcd);

#ifndef RT_USING_COMPONENTS_INIT
        /* init rtgui system server */
        rtgui_system_server_init();
#endif

        calibration_set_restore(cali_setup);
        calibration_set_after(cali_store);
        calibration_init();
    }
#endif /* #ifdef RT_USING_RTGUI */
}

/******** thread extern ***********/
extern void pub_sub_entry(void* parameter);
extern void serial_com_check_thread_entry(void* parameter);
extern void modbus_thread_entry(void* parameter);
extern void check_moudle_entry(void* parameter);

extern void thread_entry(void* parameter);

rt_thread_t pub_sub_th;
rt_thread_t init_thread;
rt_thread_t serial_com_thread;
rt_thread_t modbus_thread;
rt_thread_t lable_test_th;

int rt_application_init(void)
{
		
#if (RT_THREAD_PRIORITY_MAX == 32)
    init_thread = rt_thread_create("init",
                                   rt_init_thread_entry, RT_NULL,
                                   1024, 8, 20);
#else
    init_thread = rt_thread_create("init",
                                   rt_init_thread_entry, RT_NULL,
                                   1024, 80, 20);
#endif
	
    if (init_thread != RT_NULL)
        rt_thread_startup(init_thread);
		
		/*******自定义线程********/

	//线程3：MQTT订阅+发布消息
	pub_sub_th = rt_thread_create("pub_sub_th",
							   pub_sub_entry, RT_NULL,
							   2048, 7, 30);
	if (pub_sub_th != RT_NULL)
		rt_thread_startup(pub_sub_th);
			
		
	serial_com_thread = rt_thread_create("serial",
                                   serial_com_check_thread_entry, RT_NULL,
                                   512, 7, 10);
	if (serial_com_thread != RT_NULL)
        rt_thread_startup(serial_com_thread);


    return 0;
}

/*@}*/
