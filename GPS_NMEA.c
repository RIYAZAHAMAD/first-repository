/*
    filename	:serial_codes.c
	description	:transmit and receive data by serial port
	author		:apsis_team
	date		:11.08.07


*/


#include <reg52.h>
#include <stdio.h>                /* prototype declarations for I/O functions */
#include <string.h>
#include <stdlib.h>

#include "PinAllocation.h"
#include "delay.h"
#include "serial_codes.h"
#include "GPS_NMEA.h"
#include "ProjectMain.h"

extern unsigned char uchSerialData;
unsigned char guchCompleteDataPacket[65];

unsigned char guchLatitude[11];
unsigned char NSIndicator;
unsigned char guchLongitude[11];
unsigned char EWIndicator;

#define GPSQualityIndicatorPOSITIONINPACKET 	0
#define NoSatellitesUsedPOSITIONINPACKET 		1
#define GPSNoOfSatellitesInViewPOSITIONINPACKET 3
#define GPSUTCTimePOSITIONINPACKET 				28
#define GPSStatusPOSITIONINPACKET 				5
#define GPSLatitudePOSITIONINPACKET 			6
#define GPSN_SPOSITIONINPACKET 					16
#define GPSLongitudePOSITIONINPACKET 			17
#define GPSE_WPOSITIONINPACKET 					27
#define GPSUTCDatePOSITIONINPACKET 				34
#define GPSSpeedInKnotsPOSITIONINPACKET 		40
#define GPSSpeedInKMPHPOSITIONINPACKET 			45
#define GPSModePOSITIONINPACKET 				51



#define GPGGAGPSQUALITYINDICATORFIELD 		6
#define GPGGANOOFSATELLITESUSEDFIELD  		7
#define GPGSVNOOFSATELLITESINVIEWFIELD		3
#define GPVTGSPEEDINKNOTSFIELD				3
#define GPVTGSPEEDINKMPHFIELD				4
#define GPRMCUTCTIMEFIELD					1
#define GPRMCSTATUSFIELD					2
#define GPRMCLATITUDEFIELD					3
#define GPRMCNSINDICATORFIELD				4
#define GPRMCLONGITUDEFIELD					5
#define GPRMCEWINDICATORFIELD				6
#define GPRMCUTCDATEFIELD					9



#define LENGTHOFGPSPATTERN 6
#define NOOFGPSPATTERN 5
#define GPGGA 	1
#define GPGSA 	2
#define GPGSV 	3
#define GPRMC 	4
#define GPVTG 	5
char GPSPattern[NOOFGPSPATTERN][LENGTHOFGPSPATTERN] = 
					{
						"GPGGA", // Pattern 0 LATITUDE AND LONGITUDE, WITH TIME OF POSITION FIX AND STATUS
						"GPGSA", // Pattern 1 GPS DOP AND ACTIVE SATELLITES
						"GPGSV", // Pattern 2 GPS SATELLITE IN VIEW
						"GPRMC", // Pattern 3 RECOMMENDED MINIMUM SPECIFIC GPS/TRANSIT DATA
						"GPVTG"  // Pattern 4 COURSE OVER GROUND AND GROUND SPEED
					};



unsigned char fn_GPSPatternDetect(void)
{
	unsigned char uchCount=0;
	unsigned char uchPatternReceived[6];

	for(uchCount=0; uchCount<GPSPatternLength;uchCount++)
	{
		uchPatternReceived[uchCount]= fn_Receive_Serial();
	}
	uchPatternReceived[uchCount]= 0;

	for (uchCount=0; uchCount < NOOFGPSPATTERN;uchCount++)
	{
		if (!strcmp(uchPatternReceived, GPSPattern[uchCount]))
			return (uchCount+1);
			
	}
	return 20;
}
/*
The GPS locations are in minutes and seconds while google maps uses degrees. 
for example,
S 40°48.452' 
E 147°37.843' 
In google map it should be: 
-(40 + 48.452/60) = -40.8075 
147 + 37.843/60 = 147.6307 

N 17°18.3806' 
E 78°44.6059'
College in Hyd
In google map it should be: 
17 + 18.3806/60 = 17.3063 
78 + 44.6059/60 = 78.7434 
*/
void fn_GETGPSCoordinates(void)
{
	fnModuleSelect(SELECT_GPS_ON_UART);
	fn_DecodeGPS();
	fn_DecodeGPS();
	fn_DecodeGPS();
	fn_DecodeGPS();
	fn_DecodeGPS();
	fn_DecodeGPS();
	fn_DecodeGPS();
	fn_DecodeGPS();
	GPSDBGPRINTF("guchCompleteDataPacket = %s\n",guchCompleteDataPacket);
	GPSDBGPRINTF("Latitude =%s,%c\n",guchLatitude,NSIndicator);
	GPSDBGPRINTF("Longitude =%s,%c\n",guchLongitude,EWIndicator);


}


void fn_DecodeGPS(void)
{

	while((fn_Receive_Serial())!='$');  // Waiting for '$' Symbol to appear from the GPS

	uchSerialData = fn_GPSPatternDetect();

	switch(uchSerialData)
	{
		case GPGGA:
					fnDecodeGPGGA();
					GPSDBGPRINTF("A");
					break;
		case GPGSA:
					fnDecodeGPGSA();
					GPSDBGPRINTF("S");
					break;
		case GPGSV:
					fnDecodeGPGSV();
					GPSDBGPRINTF("V");
					break;
		case GPRMC:
					fnDecodeGPRMC();
					GPSDBGPRINTF("R");
					break;
		case GPVTG:
					fnDecodeGPVTG();
					GPSDBGPRINTF("guchCompleteDataPacket = %s\n",guchCompleteDataPacket);
					GPSDBGPRINTF("G");
					break;
		default:
					break;
	}
}




void fnDecodeGPGGA(void) //9.1 GGA-GLOBAL POSITIONING SYSTEM FIX DATA
// From here we will pick GPS Quality Indicator (Field 6)
// No of satellites used (Field 7)
{
	unsigned char uchGPSQualityIndicator;
	unsigned char uchNoSatellitesUsedArray[3];
	unsigned char uchNoSatellitesUsed;
	unsigned char uchCount=0;
	unsigned char uchTemp;
	uchNoSatellitesUsedArray[0] = '0';
	uchNoSatellitesUsedArray[1] = '0';
	uchNoSatellitesUsedArray[2] = 0;


	for (uchCount=0; uchCount < GPGGAGPSQUALITYINDICATORFIELD;uchCount++)
		while((fn_Receive_Serial())!=',');  // Waiting for ',' Symbol to appear from the GPS
	uchGPSQualityIndicator = fn_Receive_Serial();
	//DBGPRINTF("uchGPSQualityIndicator = %c\n",uchGPSQualityIndicator);
	uchCount=0;
	uchTemp = fn_Receive_Serial(); // To take care of the ',' symbol 
	uchTemp = fn_Receive_Serial();
	while(uchTemp!=',')  // Fill No of satellites till ',' Symbol appears
	{
		uchNoSatellitesUsedArray[uchCount] = uchTemp;	
		uchCount++;
		uchTemp = fn_Receive_Serial();
	}
	
	//DBGPRINTF("$GPGGA uchGPSQualityIndicator = %c\n",uchGPSQualityIndicator);
	uchNoSatellitesUsed = (char)atoi(uchNoSatellitesUsedArray);
	GPSDBGPRINTF("$GPGGASATUsed=%d\n",(int)uchNoSatellitesUsed);
	
	guchCompleteDataPacket[GPSQualityIndicatorPOSITIONINPACKET]= uchGPSQualityIndicator;
	strncpy(&guchCompleteDataPacket[NoSatellitesUsedPOSITIONINPACKET],uchNoSatellitesUsedArray, 2);
	//DBGPRINTF("guchCompleteDataPacket = %s\n",guchCompleteDataPacket);

	
}
void fnDecodeGPGSA(void) //9.3 GSA - GPS DOP AND ACTIVE SATELLITES
{
	// Nothing to pick now
}
void fnDecodeGPGSV(void) //9.4 GSV - GPS SATELLITE IN VIEW
{
	unsigned char uchGPSNoOfSatellitesInView; 
	unsigned char uchGPSNoOfSatellitesInViewArray[3];
	unsigned char uchCount=0;
	unsigned char uchTemp;

	uchGPSNoOfSatellitesInViewArray[0] = '0';
	uchGPSNoOfSatellitesInViewArray[1] = '0';
	uchGPSNoOfSatellitesInViewArray[2] = 0;

	for (uchCount=0; uchCount < GPGSVNOOFSATELLITESINVIEWFIELD;uchCount++)
		while((fn_Receive_Serial())!=',');  // Waiting for ',' Symbol to appear from the GPS
	uchCount=0;
	uchTemp = fn_Receive_Serial(); 	
	while(uchTemp!=',')  // Fill No of satellites till ',' Symbol appears
	{
		uchGPSNoOfSatellitesInViewArray[uchCount] = uchTemp;
		uchCount++;
		uchTemp = fn_Receive_Serial(); 	
	}
	uchGPSNoOfSatellitesInViewArray[2] = 0;
	uchGPSNoOfSatellitesInView = (char)atoi(uchGPSNoOfSatellitesInViewArray);
	//uchGPSNoOfSatellitesInView = (char)atoi(uchGPSNoOfSatellitesInViewArray);
	//DBGPRINTF("$GPGSV uchGPSNoOfSatellitesInView = %d\n",(int)uchGPSNoOfSatellitesInView);
	//DBGPRINTF("$GPGSV uchGPSNoOfSatellitesInViewArray = %s\n",uchGPSNoOfSatellitesInViewArray);


	strncpy(&guchCompleteDataPacket[GPSNoOfSatellitesInViewPOSITIONINPACKET],uchGPSNoOfSatellitesInViewArray, 2);
	//DBGPRINTF("guchCompleteDataPacket = %s\n",guchCompleteDataPacket);
}
void fnDecodeGPRMC(void) //9.5 RMC - RECOMMANDED MINIMUM SPECIFIC GPS/TRANSIT DATA
//Time, date, position, course and speed data provided by a GNSS navigation receiver.
{
	unsigned char uchGPSUTCTimearray[7]; 
	unsigned char uchStatus=0;
	unsigned char uchGPSLatitudearray[11];
	unsigned char uchN_S_Indicator=0;
	unsigned char uchGPSLongitudearray[11];
	unsigned char uchE_W_Indicator=0;
	unsigned char uchGPSUTCDatearray[7]; 

	unsigned char uchCount=0;
	unsigned char uchTemp;

	for(uchCount=0; uchCount < 11;uchCount++)
	{
		uchGPSLatitudearray[uchCount]='0';
		uchGPSLongitudearray[uchCount]='0';
		if (uchCount < 6)
		{
			uchGPSUTCTimearray[uchCount]='0';
			uchGPSUTCDatearray[uchCount]='0';
		}
	}
	uchGPSLatitudearray[10]=0;
	uchGPSLongitudearray[10]=0;
	uchGPSUTCTimearray[6]=0;
	uchGPSUTCDatearray[6]=0;

	uchTemp = fn_Receive_Serial(); 	// Compensate for the first ',' symbol
	for(uchCount=0; uchCount < 6;uchCount++)
	{
		uchGPSUTCTimearray[uchCount]=fn_Receive_Serial(); 
	}
	while((fn_Receive_Serial())!=',');  // Waiting for ',' Symbol to appear from the GPS

	uchStatus = fn_Receive_Serial(); 

	uchTemp = fn_Receive_Serial(); 	// Compensate for the  ',' symbol

	uchCount=0;
	uchTemp = fn_Receive_Serial(); 	// First char of Latitude
	while(uchTemp!=',')  // Fill Latitude till ',' Symbol appears
	{
		uchGPSLatitudearray[uchCount] = uchTemp;
		uchCount++;
		uchTemp = fn_Receive_Serial(); 	
	}
	uchN_S_Indicator = fn_Receive_Serial(); 
	while((fn_Receive_Serial())!=',');  // Waiting for ',' Symbol to appear from the GPS
	uchCount=0;
	uchTemp = fn_Receive_Serial(); 	// First char of Longitude
	while(uchTemp!=',')  // Fill Longitude till ',' Symbol appears
	{
		uchGPSLongitudearray[uchCount] = uchTemp;
		uchCount++;
		uchTemp = fn_Receive_Serial(); 	
	}
	uchGPSLongitudearray[uchCount] = 0;
	uchE_W_Indicator = fn_Receive_Serial(); 

	while((fn_Receive_Serial())!=',');  // Waiting for ',' Symbol to appear from the GPS
	while((fn_Receive_Serial())!=',');  // Waiting for ',' Symbol to appear from the GPS
	while((fn_Receive_Serial())!=',');  // Waiting for ',' Symbol to appear from the GPS

	uchCount=0;
	uchTemp = fn_Receive_Serial(); 	// First char of UTCDate
	while(uchTemp!=',')  // Fill UTCDate till ',' Symbol appears
	{
		uchGPSUTCDatearray[uchCount] = uchTemp;
		uchCount++;
		uchTemp = fn_Receive_Serial(); 	
	}

	strncpy(&guchCompleteDataPacket[GPSUTCTimePOSITIONINPACKET],uchGPSUTCTimearray, 6);
	guchCompleteDataPacket[GPSStatusPOSITIONINPACKET]= uchStatus;
	strncpy(&guchCompleteDataPacket[GPSLatitudePOSITIONINPACKET],uchGPSLatitudearray, 10);

	strncpy(&guchLatitude[0],uchGPSLatitudearray, 10);
	guchLatitude[10] = 0;

	guchCompleteDataPacket[GPSN_SPOSITIONINPACKET]= uchN_S_Indicator;
	NSIndicator = uchN_S_Indicator;

	strncpy(&guchCompleteDataPacket[GPSLongitudePOSITIONINPACKET],uchGPSLongitudearray, 10);

	strncpy(&guchLongitude[0],uchGPSLongitudearray, 10);
	guchLongitude[10] = 0;

	guchCompleteDataPacket[GPSE_WPOSITIONINPACKET]= uchE_W_Indicator;
	EWIndicator = uchE_W_Indicator;
	strncpy(&guchCompleteDataPacket[GPSUTCDatePOSITIONINPACKET],uchGPSUTCDatearray, 6);
	
}
void fnDecodeGPVTG(void)// 9.6 VTG - COURSE OVER GROUND AND GROUND SPEED
{
	unsigned char uchGPSSpeedInKnotsarray[6]; 
	unsigned char uchGPSSpeedInKMPHarray[7];
	unsigned char uchCount=0;
	unsigned char uchTemp;

	for(uchCount=0; uchCount < 5;uchCount++)
	{
		uchGPSSpeedInKnotsarray[uchCount]='0';
		uchGPSSpeedInKMPHarray[uchCount]='0';
	}
	uchGPSSpeedInKMPHarray[5]='0';
	uchGPSSpeedInKMPHarray[6]=0;
	uchGPSSpeedInKnotsarray[5]=0;

	for (uchCount=0; uchCount < GPVTGSPEEDINKNOTSFIELD;uchCount++)
		while((fn_Receive_Serial())!=',');  // Waiting for ',' Symbol to appear from the GPS
	uchCount=0;
	uchTemp = fn_Receive_Serial(); 	
	while(uchTemp!=',')  // Fill Speed in Knots till ',' Symbol appears
	{
		uchGPSSpeedInKnotsarray[uchCount] = uchTemp;
		uchCount++;
		uchTemp = fn_Receive_Serial(); 	
	}
	uchCount=0;
	uchTemp = fn_Receive_Serial(); 	
	while(uchTemp!=',')  // Fill Speed in KMPH till ',' Symbol appears
	{
		uchGPSSpeedInKMPHarray[uchCount] = uchTemp;
		uchCount++;
		uchTemp = fn_Receive_Serial(); 	
	}
	//DBGPRINTF("$GPVTG uchGPSSpeedInKnotsarray = %s\n",uchGPSSpeedInKnotsarray);
	//DBGPRINTF("$GPVTG uchGPSSpeedInKMPHarray = %s\n",uchGPSSpeedInKMPHarray);

	strncpy(&guchCompleteDataPacket[GPSSpeedInKnotsPOSITIONINPACKET],uchGPSSpeedInKnotsarray, 6);
	strncpy(&guchCompleteDataPacket[GPSSpeedInKMPHPOSITIONINPACKET],uchGPSSpeedInKMPHarray, 7);
	//DBGPRINTF("guchCompleteDataPacket = %s\n",guchCompleteDataPacket);
}

/*
unsigned char uchReceive_From_UART[32]; 
*/


