/* Includes ------------------------------------------------------------------*/
#include "application.h"

NCD24Relay relayController;

SYSTEM_MODE(MANUAL);

/* This function is called once at start up ----------------------------------*/
void setup()
{
	Serial.begin(115200);
	relayController.setAddress(0,0);
}

/* This function loops forever --------------------------------------------*/
void loop()
{
	//Test relays
	for(int i = 1; i < 25; i++){
		relayController.turnOnRelay(i);
		int rOnStatus = relayController.readRelayStatus(i);
		Serial.print("Relay status after on: ");
		Serial.println(rOnStatus);
		delay(100);
	}
	for(int i = 1; i < 25; i++){
		relayController.turnOffRelay(i);
		int rOffStatus = relayController.readRelayStatus(i);
			Serial.print("Relay status after off: ");
			Serial.println(rOffStatus);
		delay(100);
	}

	relayController.turnOnAllRelays();
	delay(500);
	relayController.turnOffAllRelays();
	delay(500);

	for(int i = 1; i < 25; i++){
		relayController.toggleRelay(i);
		delay(100);
		relayController.toggleRelay(i);
		delay(100);
	}
	for(int i = 1; i < 4; i++){
		relayController.turnOnAllRelays(i);
		delay(500);
		relayController.turnOffAllRelays(i);
		delay(500);
	}
	relayController.setBankStatus(85, 1);
	int rStatus85 = relayController.readBankStatus(1);
	Serial.print("Status after setting relay bank status to 85: ");
	Serial.println(rStatus85);
	delay(500);
	relayController.setBankStatus(170, 1);
	delay(500);
	relayController.turnOffAllRelays();
	delay(500);

//	//Read all inputs test
//	int status = relayController.readAllInputs();
//	Serial.print("Input Status: ");
//	Serial.println(status);
//	delay(1000);

//	//Read single input test
//	int inputStatus = relayController.readInputStatus(1);
//	if(inputStatus == 1){
//		Serial.println("Input Closed");
//	}
//	delay(1000);
}
