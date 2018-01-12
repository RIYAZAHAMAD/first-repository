/*
    filename	:GPS_NMEA.h
	description	:Receives values from the GPS
	author		:apsis_team
	date		:11.08.07
*/


#define NEWMESSAGE					 	0
#define NONEWMESSAGE				 	1
#define ERROR						 	2
#define INBOXEMPTY 					 	3 
	
#define NETWORK_READY					1
#define NETWORK_NOT_READY				0
///////////////						  
void fn_GSM_Recieve_Init(void);
void fn_GSM_Send_Recieve_Init(void);
///////////////


////void fnGSMCommands(void);
void fnSetMODEMToReceive(void);
void fnDeleteMessageIndex(unsigned char Index);
void fnDeleteAllMessages(void);
void fnDeleteAllReadMessages(void);
void fnDeleteAllUnReadMessages(void);
void fnDeleteAllSentMessages(void);
void fnDeleteAllUnSentMessages(void);
void fnDeleteAllReceivedMessages(void);
char chfnGSMNetworkReadyCheck(void);
void fnSendCarriageReturn(void);
void fnSendLineFeed(void);
void fnSendSUBCtrlZ(void);
void fnSendDoubleQuotes(void);
void fnGSMSendMessage(unsigned char *guchGSMPhoneNumber, unsigned char *guchCompleteDataPacket);
void fnTempSMSSendTrial(void);
//void fnTempSMSRecieveTrial(void);
void fn_GSM_Send_Init(void);
void fnDeleteInboxMessages(void);
void fn_read_message(void);
int fnGsmreadunread(void);								 
void fnCheckMessage(unsigned char temp1);
void fnDisplayError(void);
void fnInboxEmpty(void);
void fnDisplayMessageNum(void);
void fn_seperate_message_num(unsigned char *smbuff);
void fn_rec_ok(void);
void fn_message_present(void);				
unsigned char uchfnCheckFirstMessageStatus(void);


void fnExtractPhoneNoAndMessage(void);
void  fn_Disable_Echo(void);
unsigned char uchfnCheckInboxStatus(void);






