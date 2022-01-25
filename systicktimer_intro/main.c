#include "tm4c123gh6pm.h"
#include "PLL.h"
#include <stdint.h>
void PortD_Init(void)
{
	unsigned long volatile delay;
	SYSCTL_RCGC2_R |= 0x08; // Port D clock
	delay = SYSCTL_RCGC2_R; // wait 3-5 bus cycles
	GPIO_PORTD_DIR_R |= 0x01; // PD0 output
	GPIO_PORTD_AFSEL_R &= ~0x01; // not alternative
	GPIO_PORTD_AMSEL_R &= ~0x01; // no analog
	GPIO_PORTD_PCTL_R &= ~0x000000F; // bits for PD0
	GPIO_PORTD_DEN_R |= 0x01; // enable PD0
}
void SysInit(void)
{
	NVIC_ST_CTRL_R = 0;
	NVIC_ST_CURRENT_R = 0;// any write to current clears it
	//NVIC_SYS_PRI3_R = NVIC_SYS_PRI3_R&0x00FFFFFF;//priority 0
	NVIC_ST_CTRL_R = 0x00000005;// enable with core clock and interrupts
}
void SysLoad(unsigned long period)
{
	NVIC_ST_RELOAD_R = period -1;
	NVIC_ST_CURRENT_R = 0; // any value written to CURRENT clears
	while((NVIC_ST_CTRL_R&0x00010000)==0){} // wait for count flag
}
int main(void)
{
	PLL_Init();
	PortD_Init();
	SysInit();
	while(1)
	{
		GPIO_PORTD_DATA_R = 0x01;
		SysLoad(80000);
		GPIO_PORTD_DATA_R = 0x00;
		SysLoad(80000);
	}
}
