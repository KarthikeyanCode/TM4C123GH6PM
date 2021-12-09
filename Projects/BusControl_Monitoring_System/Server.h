//Server header file for transmission of information 
#ifndef Server
#define Server

#include <stdio.h>
#include <time.h>

#define capacity 10 //total capacity of the bus

int passenger_count;
uint8_t passenger_ticket[10];
uint8_t buzzer;

struct ticketing_system
{
	int ticketing_passenger_count;
	int host_information[20];
};

// void server(int);

#endif

