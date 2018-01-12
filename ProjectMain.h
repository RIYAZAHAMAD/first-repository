
extern unsigned char bdata uch_Lcd_Data;
extern unsigned char uchSerialData;

void fnModuleSelect(unsigned char uchModuleSelect);
void fnProjectMain(void);
void fnEmpty_guchCompleteDataPacket(void);
void fnEmpty_guchCompleteGSMDataPacket(void);


void fnSendmultiplemessages(unsigned char *guchmessagedata);

void fnVehicleOnTheWay(void);

unsigned char chfnToCheckVehicleReachedToDestination(void);
void fnInitialisationAtDestination(void);
void fnCargoTampered(void);
void fnInvalidAuthorisationAtDestination(void);
void fnToSendAlertMSGFromDestination(void);

void fnDecryptCommandsInSMS(void);
void fnToCheckInboxStatusAndDecryptCommandsIfAnySMS(void);

