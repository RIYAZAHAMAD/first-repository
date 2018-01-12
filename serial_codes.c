/*																				    
    filename	:serial_codes.c
	description	:transmit and receive data by serial port
	author		:apsis_team
	date		:11.08.07


*/

#include <reg51.h>

#include "serial_codes.h"





/*
To be used incase we wish to avoid printf
void fn_Display_String_Serial_Port(unsigned char *generic_ptr)
{   
    unsigned char uchSerialData;
    //generic_ptr =  string_IAmActive;
	while (*generic_ptr) 
	{
	    uchSerialData =*generic_ptr;
		fn_Transmit_Serial(uchSerialData);
		generic_ptr++;
	}
}
*/					                                       

//	Receiving the characters through the serial port
char fn_Receive_Serial(void)
{
	unsigned char RECV_DATA;

	while(RI==0);
	RECV_DATA=SBUF;
	RI=0;
	return RECV_DATA;
}

//	Transmitting the characters	through the serial port	
void fn_Transmit_Serial(unsigned char TRANS_DATA )
{
	SBUF=TRANS_DATA;
	while(TI==0);
	TI=0;
}


// Intiialising the Default Serial Port
void fn_Initialize_Serial(char baud_rate) 
{
	SCON=0X50;	 /* SCON: mode 1, 8-bit UART, enable rcvr      */
	TMOD=0X20;	//timer 1,8-bit auto reload 
	switch(baud_rate)
	{
		//	Initialization settings for 1200 baud rate
			case 0:
				{
						TH1=0XE8;
						break;
				}
		//	Initialization settings for 9600 baud rate	
			case 1:
				{
						TH1=0XFD;
						break;
				 }
		//	Initialization settings for 28800 baud rate					
			case 2:
				{
						TH1=0XFF;
						break;
				 }
	}	
	TR1=1;			/* TR1:  timer 1 run                          */
	TI    = 1;                  /* TI:   set TI to send first char of UART    */


}



