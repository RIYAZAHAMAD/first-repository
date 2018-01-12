/*
LCD.c
*/

#include <reg51.h>
#include <intrins.h>
#include "LCD.h"
#include "delay.h"
#include "PinAllocation.h"

bit sel_reg=0;		// for data write/command write used in void lcd_cmdw(unsigned char a) 

//unsigned char uch_Lcd_Data;

void fn_Display_String_LCD(unsigned char *generic_ptr)
{   
	while (*generic_ptr) 
	{
	    uch_Lcd_Data =*generic_ptr;
        lcd_write_data();
		generic_ptr++;
	}
}

void write_port_lcd(bit high_low_nibble) 
{
    if (high_low_nibble != 0)
    {
        lcd_d7_msb	=	uch_Lcd_Data & 0x80;   
        lcd_d6		=	uch_Lcd_Data & 0x40;   
        lcd_d5		=	uch_Lcd_Data & 0x20;  
        lcd_d4_lsb	=	uch_Lcd_Data & 0x10;  
    }
    else
    {
        lcd_d7_msb	=	uch_Lcd_Data & 0x08;   
        lcd_d6		=	uch_Lcd_Data & 0x04;   
        lcd_d5		=	uch_Lcd_Data & 0x02;   
        lcd_d4_lsb	=	uch_Lcd_Data & 0x01;   
    }
}    
void lcd_write_command(void) 
{
    RS_lcd = 0;
    Enable_lcd = 0;	
    write_port_lcd(HIGHER_NIBBLE);
    _nop_ ();
    _nop_ ();
    _nop_ ();
    Enable_lcd = 1;
    delay_100us(1);
    Enable_lcd = 0;
    
    _nop_ ();
    _nop_ ();
    _nop_ ();

    write_port_lcd(LOWER_NIBBLE);
    _nop_ ();
    _nop_ ();
    _nop_ ();
    Enable_lcd = 1;
    delay_100us(1);
    Enable_lcd = 0;
    
}
void lcd_write_data(void) 
{
    RS_lcd = 1;
    Enable_lcd = 0;	
    write_port_lcd(HIGHER_NIBBLE);
    _nop_ ();
    _nop_ ();
    _nop_ ();
    Enable_lcd = 1;
    delay_100us(1);
    Enable_lcd = 0;

    _nop_ ();
    _nop_ ();
    _nop_ ();

    write_port_lcd(LOWER_NIBBLE);
    _nop_ ();
    _nop_ ();
    _nop_ ();
    Enable_lcd = 1;
    delay_100us(1);
    Enable_lcd = 0;
}
void lcd_dummy_write(void) 
{
    uch_Lcd_Data = 0x20;

    RS_lcd = 0;
    Enable_lcd = 0;	
    write_port_lcd(HIGHER_NIBBLE);
    _nop_ ();
    _nop_ ();
    _nop_ ();
    Enable_lcd = 1;
    delay_100us(1);
    Enable_lcd = 0;

}


void lcd_16_2_Initialisation(void) 
{
    delay_in_half_seconds(1);
    lcd_dummy_write();

    uch_Lcd_Data = 0x2C;  ; //function set first time interface 8 bit long
    lcd_write_command();

    uch_Lcd_Data = 0x0C;  ; //display 0ff
    lcd_write_command();
     
    uch_Lcd_Data = 0x06;  ; //entry mode
    lcd_write_command();
  
    uch_Lcd_Data = 0x01;  ; //clear display
    lcd_write_command();
    delay_in_ms(1);
}  	

void fn_lcd_select_line_and_location(char ch_line_no, char ch_location_no) 
{
	if (ch_line_no== LCD_LINE_2)
		uch_Lcd_Data = 0xC0 + ch_location_no;
	else
    	uch_Lcd_Data = 0x80	+ ch_location_no;

    lcd_write_command();
    delay_in_ms(1);
}


