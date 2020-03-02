#ifndef   _RUN_LED_H_
#define   _RUN_LED_H_


struct RUNLED
{
		 unsigned char c_val;
		 void (*pSetRunLedValue)(unsigned char val);
		 unsigned char  (*pGetRunLedValue)(void);
		 void (*pRunLedBsp)(void);
};

void  			   RunLedBsp(void);
unsigned char  GetRunLedValue(void);
void 					 SetRunLedValue(unsigned char val);



#endif

















