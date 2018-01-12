/*
    filename	:GSMSIM300.c
	description	:transmit and receive GSM data by SIM300 MODEM
	author		:Nitin
	date		:13.11.11		  
*/


#include <reg52.h>
#include <stdio.h>                /* prototype declarations for I/O functions */
#include <string.h>
#include <stdlib.h>

#include "PinAllocation.h"
#include "delay.h"
#include "serial_codes.h"
#include "GSMSIM300.h"
#include "ProjectMain.h"
#include "lcd.h"

unsigned char guchmessagepacket[40],guchphonenumberpacket[15],guchCompleteGSMDataPacket[160];
unsigned char guchGSMPhoneNumber[15],displaybuffer[70];

#define	CR								0x0D // Carriage Return = Enter
#define LF								0x0A // Line Feed = New Line Symbol

void fn_GSM_Send_Init(void)
{
	unsigned char i;
	fnModuleSelect(SELECT_GSM_ON_UART);
	for(i=0;i<30;i++)
	{
		printf("AT");
		fnSendCarriageReturn();
		delay_in_ms(30);
	}
	printf("AT");
	fnSendCarriageReturn();
	printf("AT+CMGF=1");
	fnSendCarriageReturn();
	// printf("AT+CSCA=");
	// printf("%s",msg_centre_num);
	fnSendCarriageReturn();
	delay_in_ms(30);
	fn_Disable_Echo();
	delay_in_ms(30);
}
 

  											
void fnGSMSendMessage(unsigned char *guchGSMPhoneNumber, unsigned char *guchCompleteGSMDataPacket)
{	 
    fnModuleSelect(SELECT_GSM_ON_UART);
	printf("AT+CMGS="); // Command to send Message
	fnSendDoubleQuotes();
	printf("%s",guchGSMPhoneNumber);
	fnSendDoubleQuotes();
	fnSendCarriageReturn();
	delay_ms(20);
	printf("%s",guchCompleteGSMDataPacket);
	fnSendSUBCtrlZ();
	//	while((fn_Receive_Serial)!= '+');
   	delay_ms(100);				   	 
	// Waiting for Carriage Return Symbol to appear from GSM as response
	// Response expected here is          OK <CR><LF>
}


void fn_GSM_Recieve_Init(void)
{
	fnModuleSelect(SELECT_GSM_ON_UART);
	printf("AT+CNMI=2,1,0,0,0"); // Command to save to memory to and raise interrrupt	
	fnSendCarriageReturn();
	fnSendLineFeed();

	while((fn_Receive_Serial())!=0x0a);
	// Response expected here is          OK <CR><LF>
}

void fn_GSM_Send_Recieve_Init(void)
{
	fnModuleSelect(SELECT_GSM_ON_UART);
	fn_GSM_Send_Init();
	fn_GSM_Recieve_Init();
}


void fnSetMODEMToReceive(void) //New message indication +CNMI
{
	fnModuleSelect(SELECT_GSM_ON_UART);
	printf("AT+CNMI=2,1,0,0,0"); // Command to save to memory to and raise interrrupt	
	fnSendCarriageReturn();
	fnSendLineFeed();

	while((fn_Receive_Serial())!=0x0a);
	// Response expected here is          OK <CR><LF>

}
void fnDeleteMessageIndex(unsigned char Index)
{
	fnModuleSelect(SELECT_GSM_ON_UART);
	printf("AT+CMGD="); // Command to delete
	printf("%d",Index);
	fnSendCarriageReturn();
	fnSendLineFeed();

  // Waiting for Carriage Return Symbol to appear from GSM as response
	// Response expected here is          OK <CR><LF>
}

void fnDeleteAllMessages(void)
{
	fnModuleSelect(SELECT_GSM_ON_UART);
	printf("AT+CMGDA="); // Command to delete all messages
	fnSendDoubleQuotes();
	printf("DEL ALL");
	fnSendDoubleQuotes();

	fnSendCarriageReturn();
	fnSendLineFeed();

	// Waiting for Carriage Return Symbol to appear from GSM as response
	// Response expected here is          OK <CR><LF>
}

void fnDeleteInboxMessages(void)
{
	fnModuleSelect(SELECT_GSM_ON_UART);
	printf("AT+CMGDA="); // Command to delete all messages
	fnSendDoubleQuotes();
	printf("DEL INBOX");
	fnSendDoubleQuotes();

	fnSendCarriageReturn();

	fnSendLineFeed();

	while((fn_Receive_Serial())!= CR);
	delay_in_seconds(1);
	
  // Waiting for Carriage Return Symbol to appear from GSM as response

	// Response expected here is          OK <CR><LF>

}
void fnDeleteAllUnReadMessages(void)
{
	fnModuleSelect(SELECT_GSM_ON_UART);
	printf("AT+CMGDA="); // Command to delete all messages
	fnSendDoubleQuotes();
	printf("DEL UNREAD");
	fnSendDoubleQuotes();

	fnSendCarriageReturn();
	fnSendLineFeed();


 // Waiting for Carriage Return Symbol to appear from GSM as response
	// Response expected here is          OK <CR><LF>
}
void fnDeleteAllSentMessages(void)
{
	fnModuleSelect(SELECT_GSM_ON_UART);
	printf("AT+CMGDA="); // Command to delete all messages
	fnSendDoubleQuotes();
	printf("DEL SENT");
	fnSendDoubleQuotes();

	fnSendCarriageReturn();
	fnSendLineFeed();
	
  //Waiting for Carriage Return Symbol to appear from GSM as response
	// Response expected here is          OK <CR><LF>
}
void fnDeleteAllUnSentMessages(void)
{
	fnModuleSelect(SELECT_GSM_ON_UART);
	printf("AT+CMGDA="); // Command to delete all messages;
	fnSendDoubleQuotes();
	printf("DEL UNSENT");
	fnSendDoubleQuotes();

	fnSendCarriageReturn();
	fnSendLineFeed();

	while((fn_Receive_Serial())!='K');  // Waiting for Carriage Return Symbol to appear from GSM as response
	// Response expected here is          OK <CR><LF>
}
void fnDeleteAllReceivedMessages(void)
{
	fnModuleSelect(SELECT_GSM_ON_UART);
	printf("AT+CMGDA="); // Command to delete all messages
	fnSendDoubleQuotes();
	printf("DEL INBOX");
	fnSendDoubleQuotes();

	fnSendCarriageReturn();
	fnSendLineFeed();

	while((fn_Receive_Serial())!= CR);  // Waiting for Carriage Return Symbol to appear from GSM as response
	// Response expected here is          OK <CR><LF>
}


char chfnGSMNetworkReadyCheck(void)
{
	unsigned char uchResponseFromGSM[15];
	unsigned char uchCount=0;
	unsigned char uchTemp;

	fnModuleSelect(SELECT_GSM_ON_UART);
	printf("AT+CPIN?");
	fnSendCarriageReturn();
	fnSendLineFeed();

	while((fn_Receive_Serial())!='+');  // Waiting for '+' Symbol to appear from GSM as response

	uchCount=0;
	uchTemp = fn_Receive_Serial(); 	// First char of Response
	while(uchTemp!=0x0D)  // Fill Response till carriage return Symbol appears
	{
		uchResponseFromGSM[uchCount] = uchTemp;
		uchCount++;
		uchTemp = fn_Receive_Serial(); 	
	}
	uchResponseFromGSM[uchCount] = 0;
	
			//fnModuleSelect(SELECT_PC_ON_UART);
			//printf("uchResponseFromGSM= %s",uchResponseFromGSM);
			
	if(strstr(uchResponseFromGSM,"READY"))
	{
		return(NETWORK_READY);			
	}
	return(NETWORK_NOT_READY);
}



unsigned char uchfnCheckInboxStatus(void)
{

	unsigned char uchSerialData,uchStatus;
	
	fnModuleSelect(SELECT_GSM_ON_UART);
	printf("AT+CMGR=1");   // Reading message at location No 1 in Inbox
	fnSendCarriageReturn();									  
	fnSendLineFeed();

	uchSerialData=fn_Receive_Serial();
	while(((uchSerialData!='O')&&(uchSerialData!='+')))	// If OK only is received the NO message present at Location 1
								// incase any message is present it will start with +CMGR:			
	{
		uchSerialData=fn_Receive_Serial(); 
   	}

   switch(uchSerialData)
   {
					   
	case 'O':  
		return(INBOXEMPTY);
	    break;

    case '+':  
		uchStatus=uchfnCheckFirstMessageStatus();
	    return(uchStatus);
	    break;

    default:  
		return(ERROR);
	    break;
	}

	return(0);
}  


unsigned char uchfnCheckFirstMessageStatus(void)
{
	unsigned char i=0,uchSerialData=0;
	uchSerialData=fn_Receive_Serial();
	while((uchSerialData)!=0x0d)
	{  
		uchSerialData=fn_Receive_Serial();
		displaybuffer[i++]=uchSerialData;
	}
	uchSerialData = fn_Receive_Serial();
	displaybuffer[i++] = uchSerialData;						  
	while((uchSerialData=fn_Receive_Serial())!= '\r')
	{
		displaybuffer[i++] = uchSerialData;
	}   	   
	
	displaybuffer[i]='\r';
	
	if (strstr(displaybuffer,"UNREAD") )
	{	 
		return(NEWMESSAGE);
	}
	
	if (strstr(displaybuffer,"READ") )	
	{	
		return(NONEWMESSAGE);
	}						 	
	else
	{						 	
	return(ERROR);			 	
	}						 
}						   

void fnExtractPhoneNoAndMessage(void)
{									   	
	unsigned char i=0,databuff[20],index;  
		
	while((displaybuffer[i])!='+')
	{
		i++;		
	}
	index=i; 
	
	for(i=0;i<=12;i++)
	{				
		databuff[i]=displaybuffer[index+i];	
	}
	databuff[i]='\0';					   	
	strcpy(guchphonenumberpacket,databuff);	
	i=0;								   	
	while(displaybuffer[i]!=0x0a)
	{
		i++;
	}
	index = ++i;
	i=0;
	while(displaybuffer[index+i]!='\r')
	{
		databuff[i]=displaybuffer[index+i];
		i++;
	}
	databuff[i]='\0';				   	
	strcpy(guchmessagepacket,databuff);	
}
 


void  fn_Disable_Echo(void)
{
	printf("ATE0");
	fnSendCarriageReturn();
}
void fnSendCarriageReturn(void)						   
{	 
	char c=0x0d;
	printf("%c",c);
}

void fnSendLineFeed(void)
{  	char c=0X0A;
	printf("%c",c);
}

void fnSendSUBCtrlZ(void)
{   char c= 0x1A;
	printf("%c", c);
}

void fnSendDoubleQuotes(void)
{
    char c= 0x22;
	printf("%c",c);
}
