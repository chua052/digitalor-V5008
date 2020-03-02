#ifndef __RESET_H__
#define __RESET_H__

#define RST_NET			PAin(0)	// PA0 ‰»Î 

#define RST_NET_IO		GPIO_Pin_0	

void Key_Reset(void);
void ResetSystem(void);
void RESET_GPIO_Init(void);

void config_mode(void);

#endif

