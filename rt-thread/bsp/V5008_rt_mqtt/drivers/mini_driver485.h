#ifndef   _MINI_DRIVER485_H_
#define   _MINI_DRIVER485_H_

enum MiniRs485{ bus485 = 1, other485 };

struct MINIDRIVER485
{
		 unsigned char bus_val;
		 unsigned char other_val;
		 void (*pMini485Bsp)();
		 unsigned char   (*pGetMini485Value)(enum MiniRs485 rs485x,unsigned char val);
		 void (*pSetMini485Value)(enum MiniRs485 rs485x,unsigned char val);
};






void  				 Mini485Bsp(void);
unsigned char  GetMini485Value(enum MiniRs485 rs485x,unsigned char val);
void 					 SetMini485Value(enum MiniRs485 rs485x,unsigned char val);



#endif
