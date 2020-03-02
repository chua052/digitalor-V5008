#include "sys.h"
#include "Main.h"
#include "device.h"
#include "mini_beep.h"


struct MINIBEEP   miniBeep;


void  MinBeepBsp()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	GPIO_InitStructure.GPIO_Pin 	=  GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP; 	    //��������
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_10MHz;		//IO���ٶ�Ϊ10MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					    //�����趨������ʼ��
}

unsigned char  GetMiniBeepValue()
{
  unsigned char rev = miniBeep.c_val;
	return rev;
}
void SetMiniBeepValue(unsigned char val)
{
	 if(val == 1)
	 {
	  	GPIO_SetBits( GPIOA, GPIO_Pin_1); 
      miniBeep.c_val = 1;		 
	 }
	 else
	 {
	 
	    GPIO_ResetBits( GPIOA, GPIO_Pin_1); 
        miniBeep.c_val = 0;		 
	 }
}


