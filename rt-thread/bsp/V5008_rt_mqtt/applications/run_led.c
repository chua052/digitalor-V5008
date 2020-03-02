#include "run_led.h"
#include "Main.h"
#include "device.h"



struct RUNLED runLed;

void  RunLedBsp()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	
	GPIO_InitStructure.GPIO_Pin 	=  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP; 	    //��������
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_10MHz;		//IO���ٶ�Ϊ10MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					    //�����趨������ʼ��

}
unsigned char  GetRunLedValue()
{

  return    runLed.c_val ;
}
void SetRunLedValue(unsigned char val)
{
   if(val == 1)
	 {
		  GPIO_ResetBits( GPIOC, GPIO_Pin_0); 

      runLed.c_val  =1;
	 }
	 else
	 {
	 
	   GPIO_SetBits( GPIOC, GPIO_Pin_0); 
     runLed.c_val  =0;
	 }


}

