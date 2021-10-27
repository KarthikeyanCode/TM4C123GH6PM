#include "tm4c123gh6pm.h"
#include <stdint.h>

void PortF_Init()
{
	volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000020;     // 1) activate clock for Port F
  delay = SYSCTL_RCGC2_R;           // allow time for clock to start
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
  // only PF0 needs to be unlocked, other bits can't be locked
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog on PF
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) PCTL GPIO on PF4-0
  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 in, PF3-1 out
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) disable alt funct on PF7-0
  GPIO_PORTF_PUR_R = 0x11;          // enable pull-up on PF0 and PF4
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital I/O on PF4-0
}

void Delay(void)
{
    unsigned long volatile time;
    time=145448;
    while(time)
    {
        time--;
    }
}

int main(void)
{
	unsigned long Switch_1;
	unsigned long Switch_2;
	PortF_Init();//initialising Port F0 and F4 as input, port F1 to F3 as output
	while(1)
	{
		Switch_1 = GPIO_PORTF_DATA_R&0x01;
		Switch_2 = GPIO_PORTF_DATA_R&0x10;
		
		if(Switch_1 != 0x00 && Switch_2 == 0x00)
		{
			GPIO_PORTF_DATA_R = 0x00;
			Delay();
			GPIO_PORTF_DATA_R |= 0x02;
			Delay();
		}
		else if(Switch_1 == 0x00 && Switch_2 != 0x00) 
		{
			GPIO_PORTF_DATA_R = 0x00;
			Delay();
			GPIO_PORTF_DATA_R |= 0x08;
			Delay();
		}
		else if(Switch_1 == 0x00 && Switch_2 == 0x00)
		{
			GPIO_PORTF_DATA_R = 0x00;
			Delay();
			GPIO_PORTF_DATA_R |= 0x0A;
			Delay();
		}
		else
		{
			GPIO_PORTF_DATA_R = 0x00;
			Delay();
			GPIO_PORTF_DATA_R |= 0x04;
			Delay();
		}
	}
}
