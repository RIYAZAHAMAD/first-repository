// Pin allocation.h						   
#define DEBUGMODE		1
#define RELEASEMODE	 	0
#define MODE			DEBUGMODE
#define DBGPRINTF if(MODE) printf

#define GPSMODE			RELEASEMODE
#define GPSDBGPRINTF	if(GPSMODE) printf



#define SET_INPUT 	1
#define SET_OUTPUT 	0


#define SET_TRUE 	1
#define SET_FALSE 	0


#define SELECT_PC_ON_UART 	3
#define SELECT_GPS_ON_UART	2
#define SELECT_GSM_ON_UART	1
#define SELECT_RFID_ON_UART	0
//11 - PC
//10 - GPS
//01 - GSM
//00 - No selection

sbit ModuleSelect0		=	P3^7;
sbit ModuleSelect1		=	P3^6;


sbit RS_lcd				=	P1^2;
sbit Enable_lcd			=	P1^3;
sbit lcd_d4_lsb			=	P1^4;   // pin 11 of LCD
sbit lcd_d5				=	P1^5;   // pin 12 of LCD
sbit lcd_d6				=	P1^6;   // pin 13 of LCD
sbit lcd_d7_msb			=	P1^7;   // pin 14 of LCD


sbit		Keyboard_key_1	=		P2^0 ;			 
sbit		Keyboard_key_2	=		P2^1 ;
sbit 		Keyboard_key_3	=		P2^2 ;
sbit		Keyboard_key_4	=		P2^3 ;
sbit		Keyboard_key_5	=		P2^4 ;
sbit		Keyboard_key_6	=		P2^5 ;
sbit		Keyboard_key_7	=		P2^6 ;
sbit		Keyboard_key_8	=		P2^7 ;
sbit		Keyboard_key_9	=		P0^7 ;
sbit		Keyboard_key_10	=		P0^6 ;
sbit		Keyboard_key_11	=		P0^5 ;
sbit		Keyboard_key_12	=		P0^4 ;

sbit 		EM_lock_pin		=		P0^1 ;
sbit		Buzzer_pin		=		P0^0 ;

#define LOCKENABLED		0
#define BUZZERENABLED	1
#define LOCKDISABLE		1
#define BUZZERDISABLE	0
sbit		EM_lock_enabe_button=	P0^2 ;




