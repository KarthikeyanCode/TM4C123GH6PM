#include "tm4c123gh6pm.h"
#include "PLL.h"
#include <stdint.h>

//portb_init
void portb_init()
{
	unsigned long volatile delay;
	SYSCTL_RCGC2_R |= 0x02;//activate clock for portb
	delay = SYSCTL_RCGC2_R; // wait 3-5 bus cycles
	GPIO_PORTB_DEN_R |= 0x04;//pinb2 is digital pin
	GPIO_PORTB_DIR_R |= 0x04;//pinb2 is output
	GPIO_PORTB_AMSEL_R = 0x00;//clearing AMSEL
	GPIO_PORTB_AFSEL_R = 0x00;//clearing AFSEL
	GPIO_PORTB_PCTL_R = 0x00;//selecting GPIO functionality by clearing PCTL register
}

//sysinit
void SysInit(void)
{
	NVIC_ST_CTRL_R = 0;
	NVIC_ST_CURRENT_R = 0;// any write to current clears it
	//NVIC_SYS_PRI3_R = NVIC_SYS_PRI3_R&0x00FFFFFF;//priority 0
	NVIC_ST_CTRL_R = 0x00000005;// enable with core clock and interrupts
}

//sysload
void SysLoad(unsigned long period)
{
	NVIC_ST_RELOAD_R = period -1;
	NVIC_ST_CURRENT_R = 0; // any value written to CURRENT clears
	while((NVIC_ST_CTRL_R&0x00010000)==0){} // wait for count flag
}

//main function 
int main(void)
{
	int i=0, j=0;
	
	PLL_Init();
	portb_init();
	SysInit();
	
	while(1)
	{
		//50% on/off 100Hz
		for(i=0; i<5; i++)
		{
			GPIO_PORTB_DATA_R = 0x04;
			SysLoad(400000);
			GPIO_PORTB_DATA_R = 0x00;
			SysLoad(400000);
		}
		
		//0.5 sec delay
		SysLoad(16000000);
		SysLoad(16000000);
		SysLoad(8000000);
		
		//80% on and 20% off 100Hz
		for(j=0; j<5; j++)
		{
			GPIO_PORTB_DATA_R = 0x04;
			SysLoad(640000);
			GPIO_PORTB_DATA_R = 0x00;
			SysLoad(160000);
		}
		
		//0.2 sec delay
		SysLoad(16000000);
	}
}
