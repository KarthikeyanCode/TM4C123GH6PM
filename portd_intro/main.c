//header files 
#include <stdint.h>
#include "tm4c123gh6pm.h"

/*
	EDM19B001
	S.Karthikeyan
	esd lab1 exercise1 
	control external led with onboard switches 
*/


//initialise portf registers
void PortF_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000020;     // 1) activate clock for Port F
  delay = SYSCTL_RCGC2_R;           // allow time for clock to start
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
  // only PF0 needs to be unlocked, other bits can't be locked
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog on PF
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) PCTL GPIO on PF4-0
  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 in, PF3-1 out "0b00001110" direction register - i/p or o/p
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) disable alt funct on PF7-0
  GPIO_PORTF_PUR_R = 0x11;          // enable pull-up on PF0 and PF4
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital I/O on PF4-0
}

//initialise portd registers
void portd_init(void)
{
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x00000008;//activate clock for portd
	delay = SYSCTL_RCGC2_R;//wait 3-5 clock cycles
	GPIO_PORTD_AMSEL_R &= ~(0x09);//disable analog on portd pin 0 and 3
	GPIO_PORTD_PCTL_R &= ~(0x09);//PCTL gpio portd pin 0 and 3
	GPIO_PORTD_DIR_R |= 0x09;//set pins 0 and 3 of portd as output
	GPIO_PORTD_AFSEL_R &= ~(0x09);//disable alt function on portd pin 0 and 3
	GPIO_PORTD_DEN_R |= 0x09;//enable digital on pins 0 and 3 of portd
}

//delay function
void Delay(void)
{
	unsigned long volatile time;
  time = 145448;  // 0.1sec delay
  while(time)
	{
		time--;
  }
}
int main(void)
{ 	
  PortF_Init();//initialise portf
	portd_init();//initialise portd
  while(1)
	{
    uint8_t Switch = GPIO_PORTF_DATA_R & 0x11;
		
		if(Switch == 0x11)//switch 1 and switch 2 are off
		{
			GPIO_PORTD_DATA_R = 0x00;//no LED
		}
		else if(Switch == 0x01)//switch 1 is on and switch 2 is off
		{
			GPIO_PORTD_DATA_R = 0x08;//2nd LED 
		}
		else if(Switch == 0x10)//switch 1 is off and switch 2 is on 
		{
			GPIO_PORTD_DATA_R = 0x01;//1st LED
		}
		else//both the switches are on 
		{
			GPIO_PORTD_DATA_R = 0x09;//both LED 
		}
  }
	
}

// color    led(s) portf
// dark     ---    0
// red      r--    0x02
// blue     --b    0x04
// green    -g-    0x08
// yellow   rg-    0x0a
// sky blue -gb    0x0c
// white    rgb    0x0e
// pink     r-b    0x06
