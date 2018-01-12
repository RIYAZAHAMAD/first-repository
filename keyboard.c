/*
		File name				:	keyboard.c
		Discription				:	12x1 keyboard
		Date of creation		:	4-11-2011
		Author					:	Arjun Singh
		Code last update on		:
		Code last updated by	:	Arjun Singh

*/



#include <reg51.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "keyboard.h"
#include "delay.h"
#include "pinallocation.h"
#include "ProjectMain.h"
#include "LCD.h"


unsigned char	Keypad[12]= 	{'1','2','3','4','5','6','7','8','9','*','0','#'};
unsigned char  Keyboard_key_no;
unsigned char password_length ;
//char string_passwd[]={123456};
unsigned char key_pressed[7];

unsigned char length ;
//int passwordvalue;
//unsigned char key_pressed[6];
//unsigned char pointer_key_pressed[];





void fn_keyboard_12x1for_password(unsigned char password_length)
{
		fn_lcd_select_line_and_location(LCD_LINE_1,LOCATION_0);
  		fn_Display_String_LCD("                 ");
		fn_lcd_select_line_and_location(LCD_LINE_2,LOCATION_0);
  		fn_Display_String_LCD("                 ");
		fnModuleSelect(SELECT_PC_ON_UART);
		//delay_in_seconds(1);
    
	for(length=0;length<password_length;length++)
	{
		fn_lcd_select_line_and_location(LCD_LINE_1,LOCATION_0);
  		fn_Display_String_LCD(" Enter password");
		chfn_keyboard_12x1();
	    key_pressed[length]= Keyboard_key_no;
		delay_in_ms(500);
		fn_lcd_select_line_and_location(LCD_LINE_2,length);
  		fn_Display_String_LCD("*");
		printf("\nkey pressed is= %C\n",key_pressed)	 ;

	}
//	length=length+1		;
	key_pressed[length]= '\0';
	
//	passwordvalue=strcmp(string_passwd,key_pressed);
//	if(passwordvalue==0)
//	{
//		printf("password matched");
//		EM_lock_pin=LOCKENABLED;
//		fn_lcd_select_line_and_location(LCD_LINE_2,LOCATION_0);
//  		fn_Display_String_LCD("Password matched");
//
//	}
//	else
//	{
//		Buzzer_pin=BUZZERENABLED;
//		fn_lcd_select_line_and_location(LCD_LINE_2,LOCATION_0);
//  		fn_Display_String_LCD(" Wrong password ");
//		printf("Wrong password");
//	}
		
		
}





void chfn_keyboard_12x1(void)
{
	unsigned char i;
	Keyboard_key_9=1;
	Keyboard_key_10=1;
	Keyboard_key_11=1;
	Keyboard_key_12=1;
	P2=0xff	;
	i=((P2==0xff)&&Keyboard_key_9==1&& Keyboard_key_10==1&&Keyboard_key_11==1&&Keyboard_key_12==1 );
	while(i!=0)
	{
	i=((P2==0xff)&&Keyboard_key_9==1&& Keyboard_key_10==1&&Keyboard_key_11==1&&Keyboard_key_12==1 );
	}
	
	if(Keyboard_key_1==KEYBOARD_KEY_PRESSED)
		{
		delay_in_ms(100);
		if(Keyboard_key_1==KEYBOARD_KEY_PRESSED)
		Keyboard_key_no=Keypad[0];
		printf("%C",Keyboard_key_no);
//		break;
		}
   	if(Keyboard_key_2==KEYBOARD_KEY_PRESSED)
   		{
		delay_in_ms(100);
		if(Keyboard_key_2==KEYBOARD_KEY_PRESSED)
		Keyboard_key_no=Keypad[1];
		printf("%C",Keyboard_key_no);
//		break;
		}
   	if(Keyboard_key_3==KEYBOARD_KEY_PRESSED)
   		{
		delay_in_ms(100);
		if(Keyboard_key_3==KEYBOARD_KEY_PRESSED)
		Keyboard_key_no=Keypad[2];
		printf("%C",Keyboard_key_no);
//		break;
		}
   	if(Keyboard_key_4==KEYBOARD_KEY_PRESSED)
   		{
		delay_in_ms(100);
		if(Keyboard_key_4==KEYBOARD_KEY_PRESSED)
		Keyboard_key_no=Keypad[3];
		printf("%C",Keyboard_key_no);
//		break;
		}
   	if(Keyboard_key_5==KEYBOARD_KEY_PRESSED)
   		{
		delay_in_ms(100);
		if(Keyboard_key_5==KEYBOARD_KEY_PRESSED)
		Keyboard_key_no=Keypad[4];
		printf("%C",Keyboard_key_no);
//		break;
		}
	if(Keyboard_key_6==KEYBOARD_KEY_PRESSED)
		{
		delay_in_ms(100);
		if(Keyboard_key_6==KEYBOARD_KEY_PRESSED)
		Keyboard_key_no=Keypad[5];
		printf("%C",Keyboard_key_no);
//		break;
		}
	if(Keyboard_key_7==KEYBOARD_KEY_PRESSED)
		{
		delay_in_ms(100);
		if(Keyboard_key_7==KEYBOARD_KEY_PRESSED)
		Keyboard_key_no=Keypad[6];
		printf("%C",Keyboard_key_no);
//		break;
		}
	if(Keyboard_key_8==KEYBOARD_KEY_PRESSED)
		{
		delay_in_ms(100);
		if(Keyboard_key_8==KEYBOARD_KEY_PRESSED)
		Keyboard_key_no=Keypad[7];
		printf("%C",Keyboard_key_no);
//		break;
		}
	if(Keyboard_key_9==KEYBOARD_KEY_PRESSED)
		{
		delay_in_ms(100);
		if(Keyboard_key_9==KEYBOARD_KEY_PRESSED)
		Keyboard_key_no=Keypad[8];
		printf("%C",Keyboard_key_no);
//		break;
		}
	if(Keyboard_key_10==KEYBOARD_KEY_PRESSED)
		{
		delay_in_ms(100);
		if(Keyboard_key_10==KEYBOARD_KEY_PRESSED)
		Keyboard_key_no=Keypad[9];
		printf("%C",Keyboard_key_no);
		//break;
		}
	if(Keyboard_key_11==KEYBOARD_KEY_PRESSED)
		{
		delay_in_ms(100);
		if(Keyboard_key_11==KEYBOARD_KEY_PRESSED)
		Keyboard_key_no=Keypad[10];
		printf("%C",Keyboard_key_no);
	  //  break;
		}
	if(Keyboard_key_12==KEYBOARD_KEY_PRESSED)
		{
		delay_in_ms(100);
		if(Keyboard_key_12==KEYBOARD_KEY_PRESSED)
		Keyboard_key_no=Keypad[11];
		printf("%C",Keyboard_key_no);
	//	break;
		}
	
	//while((P2==0xff)&&KEYBOARD_KEY_9==1&& KEYBOARD_KEY_10==1&&KEYBOARD_KEY_11==1&&Keyboard_key_12 );
	//	return 	Keyboard_key_no;
	
	

}
