#ifndef   _MINI_BEEP_H_
#define   _MINI_BEEP_H_



struct MINIBEEP
{
			 unsigned char c_val;
			 void (*pSetMiniBeepValue)(unsigned char val);
			 unsigned char  (*pGetMiniBeepValue)(void);
			 void (*pMinBeepBsp)(void);

};

void  					MinBeepBsp(void);
unsigned char  	GetMiniBeepValue(void);
void 						SetMiniBeepValue(unsigned char val);

#endif

