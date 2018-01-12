/*
    filename	:serial_codes.h
	description	:transmit and receive data by serial port
	author		:apsis_team
	date		:11.08.07
*/

#define BAUD_1200  0
#define BAUD_9600  1
#define BAUD_28800 2
#define BAUD_57600 3
#define SERIAL_INTERRUPT_ENABLED  1
#define SERIAL_INTERRUPT_DISABLED 0





void fn_Initialize_Serial(char baud_rate);
char fn_Receive_Serial(void);
void fn_Transmit_Serial(unsigned char TRANS_DATA );
void fn_Display_String_Serial_Port(unsigned char *generic_ptr);
