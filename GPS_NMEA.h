/*
    filename	:GPS_NMEA.h
	description	:Receives values from the GPS
	author		:apsis_team
	date		:11.08.07
*/

#define GPSPatternLength 5


unsigned char fn_GPSPatternDetect(void);
void fn_DecodeGPS(void);
void fnDecodeGPGGA(void);
void fnDecodeGPGSA(void);
void fnDecodeGPGSV(void);
void fnDecodeGPRMC(void);
void fnDecodeGPVTG(void);
void fn_GETGPSCoordinates(void);
