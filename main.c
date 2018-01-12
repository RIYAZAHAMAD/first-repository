/*
	filename	:main.c
	description	:main function
	author		:
	date		:
	
	Good practice
	1. Downsizing Your Variables
	2. Use Unsigned Types
	3. Stay Away from Floating Point
	4. Make Use of bit Variables
	5. Use Locals instead of Globals
	6. Use Internal Memory for Variables - DATA, IDATA, PDATA, XDATA.
	7. Use Macros Instead of Functions - For speed
	   Use functions Instead of Macros - For Code space less
	8. Never add printf in the code it uses about 1K code
*/
#pragma OT(1, SIZE) // to decide on the level of optimisation


#include <reg52.h>
#include <stdio.h>                /* prototype declarations for I/O functions */


#include "serial_codes.h"
#include "main.h"
#include "LCD.h"
#include "delay.h"
#include "ProjectMain.h"
#include "PinAllocation.h"
//#include "keyboard.h"

unsigned char bdata uch_Lcd_Data;
unsigned char uchSerialData;
//extern unsigned char key_pressed[6];
//unsigned char keywordvalue[];
//unsigned char value=5;


void main(void)
{
	Buzzer_pin=BUZZERDISABLE;
	//EM_lock_pin=LOCKENABLED;	
	fn_Initialize_Serial(BAUD_9600);
	printf(string_project_name);
	EM_lock_enabe_button=1 ;


	lcd_16_2_Initialisation();
    fn_lcd_select_line_and_location(LCD_LINE_1, LOCATION_0);
    fn_Display_String_LCD(string_project_name);
    fn_lcd_select_line_and_location(LCD_LINE_2, LOCATION_0);
    fn_Display_String_LCD(string_welcome);
    delay_in_seconds(1);
//	while(1)
//	{
//
//	fn_keyboard_12x1for_password(value)	;
//	 //keywordvalue[6]=key_pressed;
//	 printf(keywordvalue);
//	delay_in_seconds(5);
//	 }

fnProjectMain();

}


