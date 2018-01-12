/*
delay.c
*/

#include <reg51.h>

#include "delay.h"




void delay_16us(unsigned int j)
{ 
  unsigned int i;
  for(i=0;i<j;i++);
}

void delay_ms(unsigned int j)
{
unsigned int i;
for(i=0;i<j;i++)
delay_16us(60);
}

void delay_100us(unsigned char uch_Count)
{
    unsigned int i;
    for(i=0;i<uch_Count;i++)
    {  
    }
}

void delay_in_ms(unsigned char uch_Count)
{
    unsigned int i;
    for(i=0;i<uch_Count;i++)
    {
        #pragma asm 
                            push 06
                			push 05
                			mov r6,#04h
        delay_1ms_2:		mov r5,#68h
        delay_1ms_1:		djnz r5,delay_1ms_1
                			djnz r6,delay_1ms_2
                			pop 05
                			pop 06
       
        #pragma endasm 
    }
}
void delay_in_half_seconds(unsigned char uch_Count)
{
    unsigned int i;
    for(i=0;i<uch_Count;i++)
    {
        delay_in_ms(200); 
        delay_in_ms(200); 
        delay_in_ms(133);
    }
}
void delay_in_seconds(unsigned char uch_Count)
{
    unsigned int i;
    for(i=0;i<uch_Count;i++)
    {
        delay_in_half_seconds(2);;
    }
}