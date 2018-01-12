/*
		File name				:	keyboard.h
		Discription				: 	header file for keyboard.c
		Date of creation		:	4-11-2011
		Author					:	Arjun Singh
		Code last update on		:
		Code last updated by	:	Arjun Singh
*/


//#define		KEYBOARD_KEY_1			P2^0			 
//#define		KEYBOARD_KEY_2			P2^1
//#define 		KEYBOARD_KEY_3			P2^2
//#define		KEYBOARD_KEY_4			P2^3
//#define		KEYBOARD_KEY_5			P2^4
//#define		KEYBOARD_KEY_6			P2^5
//#define		KEYBOARD_KEY_7			P2^6
//#define		KEYBOARD_KEY_8			P2^7
//#define		KEYBOARD_KEY_9			P0^7
//#define		KEYBOARD_KEY_10			P0^6
//#define		KEYBOARD_KEY_11			P0^5
//#define		KEYBOARD_KEY_12			P0^4 

#define		KEYBOARD_KEY_PRESSED	0
#define		KEYBOARD_KEY_RELEASE	1

//extern unsigned char key_pressed[6];


void chfn_keyboard_12x1(void);
void fn_keyboard_12x1for_password(unsigned char password_length)	 ;



