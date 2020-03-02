#include "mini_driver485.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "stmflash.h"
//#include "timer.h"
//#include "Main.h"
// #include "eth.h"
//#include "iwdg.h"
//#include "74hc595.h"
//#include "time.h"
//#include "mifare_card.h"
//#include "rc522.h"
//#include "device.h"
//#include "W5500.h"
//#include "loopfifo2.h"

struct MINIDRIVER485  mini_driv485;

void  Mini485Bsp()
{
		GPIO_InitTypeDef  GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);	
	
		GPIO_InitStructure.GPIO_Pin 	=  GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Mode	=  GPIO_Mode_Out_PP; 	//推挽输出
		GPIO_InitStructure.GPIO_Speed	=  GPIO_Speed_10MHz;		//IO口速度为10MHz
		GPIO_Init(GPIOA, &GPIO_InitStructure);					//根据设定参数初始化	
	
		GPIO_InitStructure.GPIO_Pin 	=  GPIO_Pin_12;	
		GPIO_InitStructure.GPIO_Mode	=  GPIO_Mode_Out_PP; 	//推挽输出
		GPIO_InitStructure.GPIO_Speed	=  GPIO_Speed_10MHz;		//IO口速度为10MHz
		GPIO_Init(GPIOB, &GPIO_InitStructure);					//根据设定参数初始化	
}




unsigned char  GetMini485Value(enum MiniRs485 rs485x,unsigned char val)
{
  if(rs485x == bus485)
	{
	  	return mini_driv485.bus_val;
	}
	else
	{
	    return mini_driv485.other_val;
	}
}


void SetMini485Value(enum MiniRs485 rs485x,unsigned char val)
{
  if(rs485x == bus485)
	{
	  	if(val == 1)
			{
			    GPIO_SetBits( GPIOB, GPIO_Pin_12); 
			mini_driv485.bus_val = 1; 
			}
			else
			{
			   	GPIO_ResetBits( GPIOB, GPIO_Pin_12); 
							mini_driv485.bus_val = 0; 
			}

	}
	else
	{
	   	if(val == 1)
			{
				 GPIO_SetBits(GPIOA, GPIO_Pin_8); 
				mini_driv485.other_val =1;
			}
			else
			{
				GPIO_ResetBits(GPIOA, GPIO_Pin_8);  
								mini_driv485.other_val =0;
			}
	}


}












