//implementing the ticketing counter 

#include "tm4c123gh6pm.h"
#include "TM4C123GH6PM2.h"
#include "Nokia5110.h"
#include <stdint.h>
#include <stdlib.h>
#include "Server.h"

void PortF_Init()
{
	volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000020;     // 1 activate clock for Port F
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
//LED
unsigned long Led;
unsigned long sw1, sw1_prev;//Switch1 will decrease the passenger counter
unsigned long sw2, sw2_prev;//switch2 will incerease the passenger counter

//Ticket Counter 
void Ticket_Counter()
{
	int i = 0;
	for(i=passenger_count; i<capacity; i++)
	{
		passenger_ticket[i] = rand()%2;
	}
}

void Delay(void)
{
	
	unsigned long volatile time;
	time=581792;//1 second
	while(time)
	{
			time--;
	}
}
//switch states possible (2 inbuilt switches connected to PF0 and PF4)
//sw2(PF0) refers to leaving or entering of passengers and sw1(PF1) refers to with/without ticket
//0x01 and 0x10 - Traveling state - both are unchecked
//0x01 and 0x00 - Passenger leaving - sw2 unchecked and sw1 checked
//0x00 and 0x10 - Passenger entering without ticket - sw2 checked and sw1 unchecked
//0x00 and 0x00 - Passenger entering with ticket - both are checked
int main(void)
{
	struct ticketing_system t;
	int tens_digit, ones_digit;
	int seats;
	char count[3];
	//char *count_ptr;
	passenger_count = 0;
	t.ticketing_passenger_count = 0;
	PortF_Init();
	Nokia5110_Init();
	//count_ptr = &count;
	while(1)
	{
		//assigning tickets to the rest of the passengers
		Ticket_Counter();
		
		//readin switch state with data register PORTF
		sw2  =  GPIO_PORTF_DATA_R&0x01;//reading PF0 0b00000001
		sw1  =  GPIO_PORTF_DATA_R&0x10;//reading PF4 0b00010000

		Nokia5110_Clear();
		if(sw2==0x00)
		{
			if(sw1==0x00)//entering with checking for ticket 
			{
				if(passenger_count < capacity)
				{
					if(passenger_ticket[passenger_count] == 1)
					{
						passenger_count++;
						t.ticketing_passenger_count++;
						Led |= 0x04;//Turn on Blue LED
						Nokia5110_Printf("Ticket Detected  ");
						Nokia5110_Printf("Passenger        ");
						Nokia5110_Printf("Allowed          ");
						buzzer = 0;
					}
					else
					{
						Nokia5110_Printf("No Ticket        ");
						Nokia5110_Printf("Passenger        ");
						Nokia5110_Printf("Not Allowed      ");
						Led |= 0x02;//turn on red LED
						buzzer = 1;
					}
				}
				else 
				{
					Nokia5110_Printf("Bus stationed    ");
				}
			}
			else//Bus stationed
			{
				Nokia5110_Printf("Bus stationed    ");
				buzzer = 0;
				//passenger not allowed
			}
		}
		if(sw2==0x01)
		{
			if(sw1==0x10)//travelling state 
			{
				//bus is in travelling state, no passengers are allowed to enter/exit
				Nokia5110_Printf("Bus Travelling   ");
				buzzer = 0;
			}
			else//passenger leaving 
			{
				if(passenger_count > 0)
				{
					passenger_count--;
					t.ticketing_passenger_count--;
					Led |= 0x06;//turn on blue and red LED and show pink color while leaving
					Nokia5110_Printf("Passenger        ");
					Nokia5110_Printf("Leaving          ");
					buzzer = 0;
				}
				else
				{
					Nokia5110_Printf("Bus stationed    ");
					buzzer = 0;
				}
			}
		}
		
		seats = capacity - passenger_count;
		if(passenger_count == capacity)//bus full 
		{
			//bus is full and no seats available
			Nokia5110_Printf("Seats Full");
		}
		else //if seats available
		{
			if(passenger_count == 0)
			{
				//bus empty 
				Nokia5110_Printf("Bus Empty        ");
			}
			Led |= 0x08;//led glows green 
			Nokia5110_Printf("Seats available: ");
			if(seats>=10)
			{
				tens_digit = seats/10;
				ones_digit = seats - (tens_digit*10);
				count[0] = tens_digit+48;
				count[1] = ones_digit+48;
				OutChar(count[0]);
				OutChar(count[1]);
			}
			else
			{
				count[0] = seats+48;
				OutChar(count[0]);
			}
		}
		
		GPIO_PORTF_DATA_R = Led;//updating LED state
		Delay();
		Led &= 0x00;
		GPIO_PORTF_DATA_R = Led;//updating LED state
		GPIO_PORTF_IS_R;
		Delay();
	}
}
