/*
    filename	:ProjectMain.c
	description	:transmit and receive data by serial port
	author		:apsis_team
	date		:

*/

#include <reg51.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ProjectMain.h"
#include "serial_codes.h"
#include "delay.h"
#include "strings.h"
#include "LCD.h"
#include "pinallocation.h"
#include "GPS_NMEA.h"
#include "GSMSIM300.h"
#include "keyboard.h"


#define PASSWORDLENGTH			5
#define REACHED_TO_DESTINATION	1
#define	NOT_A_DESTINATION 		0
#define RFID_CARD_NoLENGTH      11

extern unsigned char key_pressed[7];
extern unsigned char gdisplayuchBufffer[30];
extern unsigned char guchLatitude[11];
extern unsigned char NSIndicator;
extern unsigned char guchLongitude[11];
extern unsigned char EWIndicator;
extern unsigned char guchmessagepacket[40] ;
extern unsigned char guchphonenumberpacket[15],guchCompleteGSMDataPacket[160];

unsigned char uchTempVariable;

unsigned char uchBuff[30] ;
unsigned char destination_update;
unsigned char rfid_card_no[16];
unsigned char rfid_count;			  




unsigned char uchMsgStatus;
														
extern unsigned char unValueFromCounter[6];

extern unsigned char guchGSMPhoneNumber[15];
//unsigned char FlagVehicleLift,FlagVehicleHit,FlagSendSMS;
unsigned char code PHONE_NUMBERFOR_VB[20]=	{"9035135559"};
unsigned char code PHONE_NUMBERS[2][20]= {"7204931571" ,	 //sandeep 7406050252 ,	riyaz 9738731765, prashanth 9035135559
										  	"9738731765"}   ;
void fnModuleSelect(unsigned char uchModuleSelect)
{
	

	switch(uchModuleSelect)
	{
		case SELECT_PC_ON_UART:
					ModuleSelect0	= 1;
					ModuleSelect1	= 1;
					break;
		case SELECT_GPS_ON_UART:
					ModuleSelect0	= 0;
					ModuleSelect1	= 1;
					break;
		case SELECT_GSM_ON_UART:
					ModuleSelect0	= 1;
					ModuleSelect1	= 0;
					break;
		case SELECT_RFID_ON_UART:
					ModuleSelect0	= 0;
					ModuleSelect1	= 0;
					break;
		default:
					ModuleSelect0	= 1;
					ModuleSelect1	= 1;
					break;
	}
}


void fnProjectMain(void)
{
	unsigned int k;
	delay_in_seconds(5);
	destination_update = NOT_A_DESTINATION;
	fnModuleSelect(SELECT_GSM_ON_UART);// Giving time so that GSM Boots up properly	   	
	fn_GSM_Send_Init();				   //
	for(k=0; k<2;k++)
	{
 	strcpy(guchGSMPhoneNumber,PHONE_NUMBERS[k]);
	strcpy(guchCompleteGSMDataPacket,"Boot Up Message from Project - I am Active");
	fnGSMSendMessage(guchGSMPhoneNumber,guchCompleteGSMDataPacket);	
	}
//	strcpy(guchGSMPhoneNumber,PHONE_NUMBERFOR_VB);
//	strcpy(guchCompleteGSMDataPacket,"Boot Up Message from Project - I am Active");
//	fnGSMSendMessage(guchGSMPhoneNumber,guchCompleteGSMDataPacket);	
	fnModuleSelect(SELECT_PC_ON_UART);
	printf("\nMessage Sent to Predefined Number\n");
	RI=0;
	RI=0;	
	fn_GSM_Recieve_Init();
	fnModuleSelect(SELECT_PC_ON_UART);
	printf("\nMODEM Set in 2,1,0,0,0 Mode to Recieve Incoming SMSs\n");	
	fn_GETGPSCoordinates();
	fnModuleSelect(SELECT_PC_ON_UART);
	printf("guchCompleteGSMDataPacket = %s\n",guchCompleteGSMDataPacket);
	printf("Latitude =%s,%c\n",guchLatitude,NSIndicator);
	printf("Longitude =%s,%c\n",guchLongitude,EWIndicator);
	
	fn_lcd_select_line_and_location(LCD_LINE_1, LOCATION_0);
    fn_Display_String_LCD(string_project_name);
    fn_lcd_select_line_and_location(LCD_LINE_2, LOCATION_0);
    fn_Display_String_LCD(string_welcome);
   
	P2=0xff;
	
  while(1)
  {
  	Buzzer_pin=BUZZERDISABLE;
	if(EM_lock_pin==LOCKENABLED)

	{
		fn_lcd_select_line_and_location(LCD_LINE_1,LOCATION_0);
  		fn_Display_String_LCD(" PRESS LOCK KEY ");
		fn_lcd_select_line_and_location(LCD_LINE_2,LOCATION_0);
  		fn_Display_String_LCD("                 ");

		while(EM_lock_enabe_button!=0);
		EM_lock_pin=LOCKENABLED;
		fn_lcd_select_line_and_location(LCD_LINE_1,LOCATION_0);
		fn_Display_String_LCD("CARGO IS LOCKED ");
		fn_lcd_select_line_and_location(LCD_LINE_2,LOCATION_0);
  		fn_Display_String_LCD("                 ");
	    delay_in_seconds(1);
	}



	
	strcpy(uchBuff ,"cargo is at source station") ;
	fn_lcd_select_line_and_location(LCD_LINE_1,LOCATION_0);
  	fn_Display_String_LCD("Cargo Is GoingTo");
	fn_lcd_select_line_and_location(LCD_LINE_2,LOCATION_0);
  	fn_Display_String_LCD("Acharya College ");
	//delay_in_seconds(1);

//	fnToCheckInboxStatusAndDecryptCommandsIfAnySMS();	   /* Function to read messages if any*/ //fnToCheckInboxStatusAndDecryptIfAny
	
	while(!EM_lock_pin && !destination_update) 	  /* If  lock is not tampered on the way and if msg received to get status then send the status			*/
	{
		destination_update=chfnToCheckVehicleReachedToDestination();
		fnVehicleOnTheWay();	
		delay_in_seconds(2);
	}
	
	if(EM_lock_pin!=0) // To detect any temper cause to cargo
	{
		fnCargoTampered();   
	}	
       
			
	
	if(destination_update==REACHED_TO_DESTINATION)
	{	
		unsigned char uchserialdata;
		fnModuleSelect(SELECT_PC_ON_UART);		
		delay_in_seconds(1);
		printf("\nDestination Reached");
		fnInitialisationAtDestination();
		fnModuleSelect(SELECT_GSM_ON_UART);
		fnDeleteAllMessages();
		uchMsgStatus=uchfnCheckInboxStatus();
		fn_lcd_select_line_and_location(LCD_LINE_1,LOCATION_0);
		fn_Display_String_LCD("WAITING FOR MSG ");
		fn_lcd_select_line_and_location(LCD_LINE_2,LOCATION_0);
		fn_Display_String_LCD("  FROM SOURCE   ");

		while(uchMsgStatus!=NEWMESSAGE)
		{
			fnModuleSelect(SELECT_GSM_ON_UART);
			delay_in_seconds(1);
			uchMsgStatus=uchfnCheckInboxStatus();	
			fnModuleSelect(SELECT_PC_ON_UART);		
			delay_in_seconds(1);
			
			printf("\nWaiting for Authorising Message from Base Station");
			delay_in_seconds(1);
		}
		fnModuleSelect(SELECT_PC_ON_UART);
		printf("\nAuthorising Message RECIEVED");
		RI=0;
		RI=0;
		fn_lcd_select_line_and_location(LCD_LINE_1,LOCATION_0);
		fn_Display_String_LCD(" SWIPE RFID CARD ");
		fn_lcd_select_line_and_location(LCD_LINE_2,LOCATION_0);
		fn_Display_String_LCD("                 ");
		delay_in_seconds(1);

		fnModuleSelect(SELECT_RFID_ON_UART);
		while((fn_Receive_Serial())!=0x03);
		rfid_count=0;
		
		while(uchserialdata!=0x0a)										 
		{
			uchserialdata=fn_Receive_Serial();							   
			rfid_card_no[rfid_count]=uchserialdata;
			rfid_count++ ;
		}
		rfid_card_no[rfid_count-2]='\0';
		fnModuleSelect(SELECT_PC_ON_UART);
		printf("rfid card no is = %s",rfid_card_no);
		RI=0;
		RI=0;
		
   		fnExtractPhoneNoAndMessage();
		if(strstr( guchmessagepacket,rfid_card_no)) // RFID card detection
		{
			fnModuleSelect(SELECT_PC_ON_UART);
			printf("rfid card no is matched ");
			delay_in_seconds(2);
			fn_keyboard_12x1for_password(PASSWORDLENGTH);
			printf("key pressed = %s",key_pressed);
			if(strstr( guchmessagepacket,key_pressed))   // password detection
			{
				EM_lock_pin=LOCKDISABLE	;
				Buzzer_pin=BUZZERDISABLE;
				fnToSendAlertMSGFromDestination();				
			}
			else
			{
				fnInvalidAuthorisationAtDestination();	
			}
	     }
	    else
	 	{
			fnInvalidAuthorisationAtDestination();
	    }

		RI=0;
		RI=0;
			
	}
	
  }

}
		



void fnVehicleOnTheWay(void)
{
	strcpy(uchBuff, "cargo is on the way");
		if(EM_lock_pin!=0)
	{
		fnCargoTampered();
	}
	
	fn_lcd_select_line_and_location(LCD_LINE_1,LOCATION_0);
	fn_Display_String_LCD("CARGO IsOnWAY TO");
	fnToCheckInboxStatusAndDecryptCommandsIfAnySMS();
	

}		


unsigned char chfnToCheckVehicleReachedToDestination(void)
{

	if (EM_lock_enabe_button==0)
	{
		return (REACHED_TO_DESTINATION);
	}

	else
	return (NOT_A_DESTINATION);

}


void fnToSendAlertMSGFromDestination(void)
{
	fn_lcd_select_line_and_location(LCD_LINE_1,LOCATION_0);
	fn_Display_String_LCD("PASSWORD MATCHED");
	fn_lcd_select_line_and_location(LCD_LINE_2,LOCATION_0);
  	fn_Display_String_LCD("                 ");
	delay_in_seconds(5);
	fnModuleSelect(SELECT_GSM_ON_UART);

	strcpy(guchGSMPhoneNumber,PHONE_NUMBERFOR_VB);
    strcpy(guchCompleteGSMDataPacket,"CAUTION: YOUR VEHICLE no 111 \n ");
	strcat(guchCompleteGSMDataPacket, "LOCK IS OPENED SUCCESSFULLY AT DESTINATION ");
    strcat(guchCompleteGSMDataPacket, " ITS CORDINATES ARE @");
    strcat(guchCompleteGSMDataPacket, guchLatitude);
    strcat(guchCompleteGSMDataPacket,"N ");
    strcat(guchCompleteGSMDataPacket, guchLongitude);
    strcat(guchCompleteGSMDataPacket,"E");
    fnGSMSendMessage(guchGSMPhoneNumber,guchCompleteGSMDataPacket);
	
	fnModuleSelect(SELECT_GSM_ON_UART);
	strcpy(guchGSMPhoneNumber,PHONE_NUMBERS[0]);
    strcpy(guchCompleteGSMDataPacket,"CAUTION: YOUR VEHICLE no 111 \n ");
	strcat(guchCompleteGSMDataPacket, "LOCK IS OPENED SUCCESSFULLY AT DESTINATION\n");
    strcat(guchCompleteGSMDataPacket, " ITS CORDINATES ARE\n ");
    strcat(guchCompleteGSMDataPacket, guchLatitude);
    strcat(guchCompleteGSMDataPacket,"N\n");
    strcat(guchCompleteGSMDataPacket, guchLongitude);
    strcat(guchCompleteGSMDataPacket,"E\n");
    fnGSMSendMessage(guchGSMPhoneNumber,guchCompleteGSMDataPacket);
	
	
	fnModuleSelect(SELECT_PC_ON_UART);
	printf("guchCompleteGSMDataPacket = %s\n",guchCompleteGSMDataPacket);
	
}



void fnInvalidAuthorisationAtDestination(void)
{
	Buzzer_pin=BUZZERENABLED;
	fn_lcd_select_line_and_location(LCD_LINE_1,LOCATION_0);
	fn_Display_String_LCD(" ACCESS DENIED ");
	fn_lcd_select_line_and_location(LCD_LINE_2,LOCATION_0);
  	fn_Display_String_LCD("                 ");
	delay_in_seconds(2);
	fnModuleSelect(SELECT_GSM_ON_UART);
	strcpy(guchGSMPhoneNumber,PHONE_NUMBERFOR_VB);
    strcpy(guchCompleteGSMDataPacket,"CAUTION: YOUR VEHICLE no 111 \n ");
	strcat(guchCompleteGSMDataPacket, "UNAUTHORISED PERSION IS TRYING TO OPEN LOCK AT DESTINATION\n");
    strcat(guchCompleteGSMDataPacket, " ITS CORDINATES ARE @");
    strcat(guchCompleteGSMDataPacket, guchLatitude);
    strcat(guchCompleteGSMDataPacket,"N ");
    strcat(guchCompleteGSMDataPacket, guchLongitude);
    strcat(guchCompleteGSMDataPacket,"E");
    fnGSMSendMessage(guchGSMPhoneNumber,guchCompleteGSMDataPacket);
	
	fnModuleSelect(SELECT_PC_ON_UART);
	printf("guchCompleteGSMDataPacket = %s\n",guchCompleteGSMDataPacket);	
}


void fnCargoTampered(void)
{
	Buzzer_pin=BUZZERENABLED;
	fnModuleSelect(SELECT_GSM_ON_UART);
	strcpy(guchGSMPhoneNumber,PHONE_NUMBERS[0]);
    strcpy(guchCompleteGSMDataPacket,"CAUTION: YOUR VEHICLE no 111 \n ");
	strcat(guchCompleteGSMDataPacket, "IS TAMPERED!!  \n");
	strcat(guchCompleteGSMDataPacket, uchBuff);
    strcat(guchCompleteGSMDataPacket, " \r ITS CORDINATES ARE\n ");
    strcat(guchCompleteGSMDataPacket, guchLatitude);
    strcat(guchCompleteGSMDataPacket,"N\n");
    strcat(guchCompleteGSMDataPacket, guchLongitude);
    strcat(guchCompleteGSMDataPacket,"E\n");
    fnGSMSendMessage(guchGSMPhoneNumber,guchCompleteGSMDataPacket);

	strcpy(guchGSMPhoneNumber,PHONE_NUMBERFOR_VB);
    strcpy(guchCompleteGSMDataPacket,"CAUTION: YOUR VEHICLE no 111 \n ");
	strcat(guchCompleteGSMDataPacket, "IS TAMPERED!!  ");
	strcat(guchCompleteGSMDataPacket, uchBuff);
    strcat(guchCompleteGSMDataPacket, " ITS CORDINATES ARE @");
    strcat(guchCompleteGSMDataPacket, guchLatitude);
    strcat(guchCompleteGSMDataPacket,"N ");
    strcat(guchCompleteGSMDataPacket, guchLongitude);
    strcat(guchCompleteGSMDataPacket,"E");
    fnGSMSendMessage(guchGSMPhoneNumber,guchCompleteGSMDataPacket);

	fnModuleSelect(SELECT_PC_ON_UART);
	printf("guchCompleteGSMDataPacket = %s\n",guchCompleteGSMDataPacket);	
}


void fnInitialisationAtDestination(void)
{
	fnModuleSelect(SELECT_GSM_ON_UART);
	fnDeleteAllMessages();
	
	strcpy(guchGSMPhoneNumber,PHONE_NUMBERFOR_VB);
    strcpy(guchCompleteGSMDataPacket,"CAUTION: YOUR VEHICLE no 111 ");
	strcat(guchCompleteGSMDataPacket, "Has REACHED TO DESTINATION ");
    strcat(guchCompleteGSMDataPacket, " ITS CORDINATES ARE @");
    strcat(guchCompleteGSMDataPacket, guchLatitude);
    strcat(guchCompleteGSMDataPacket,"N ");
    strcat(guchCompleteGSMDataPacket, guchLongitude);
    strcat(guchCompleteGSMDataPacket,"E");
    fnGSMSendMessage(guchGSMPhoneNumber,guchCompleteGSMDataPacket);	

	fnModuleSelect(SELECT_PC_ON_UART);
	fn_lcd_select_line_and_location(LCD_LINE_1,LOCATION_0);
	fn_Display_String_LCD("MSG SendToBaseSt");	  
	
	printf("guchCompleteGSMDataPacket = %s\n",guchCompleteGSMDataPacket);	
}






void fnToCheckInboxStatusAndDecryptCommandsIfAnySMS(void)
{
	uchTempVariable = uchfnCheckInboxStatus();
		
		fnModuleSelect(SELECT_PC_ON_UART);	
				 
		switch(uchTempVariable)
		{
			case NEWMESSAGE:
				printf("\nNEW Message Present in INBOX");	
				fnExtractPhoneNoAndMessage();
				printf("\nPhone Number = %s\n", guchphonenumberpacket);	
				printf("\nMessage = %s\n", guchmessagepacket);
				fnDecryptCommandsInSMS();
				break;
			case NONEWMESSAGE:
				printf("\nNO new message in inbox");	
				fnExtractPhoneNoAndMessage();
				printf("\nPhone Number = %s\n", guchphonenumberpacket);	
				printf("\nMessage = %s\n", guchmessagepacket);
				break; 
			case INBOXEMPTY:
				printf("\nINBOX EMPTY\n");	
				break; 
		   	default:
				printf("\nERROR Reading Status");	
				break; 								
		}
		printf("\nInbox Status Checked and Message Decoded if any present\n");	
		RI=0;
		RI=0;
	
		fnDeleteAllMessages();
		fnModuleSelect(SELECT_PC_ON_UART);
		printf("\nAll Messages Deleted\n"); 
 	
 }

 void fnDecryptCommandsInSMS(void)
{
	if(strstr(guchmessagepacket,"1234"))
	{																
		if(strstr(guchmessagepacket,"STATUS"))
		{
			strcpy(guchGSMPhoneNumber,guchphonenumberpacket);
			strcpy(guchCompleteGSMDataPacket,"CAUTION:STATUS: YOUR VEHICLE no 111 ");
			strcat(guchCompleteGSMDataPacket, uchBuff);
			strcat(guchCompleteGSMDataPacket, " IT'S CORDINATES ARE @");
			strcat(guchCompleteGSMDataPacket, guchLatitude);
			strcat(guchCompleteGSMDataPacket,"N ");
			strcat(guchCompleteGSMDataPacket, guchLongitude);
			strcat(guchCompleteGSMDataPacket,"E");
			fnGSMSendMessage(guchGSMPhoneNumber,guchCompleteGSMDataPacket);	

			fnModuleSelect(SELECT_PC_ON_UART);
			printf("\n Message Sent with this Body \n");
			printf("%s\n", guchCompleteGSMDataPacket);		
		
		}
	}
}