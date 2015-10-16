/* Includes ------------------------------------------------------------------*/
#include "NCD24Relay/NCD24Relay.h"

NCD24Relay relayController;

SYSTEM_MODE(AUTOMATIC);

int triggerRelay(String command);

bool tripped[8];

int debugTrips[8];

int minTrips = 5;

/* This function is called once at start up ----------------------------------*/
void setup()
{
	Serial.begin(115200);
	delay(3000);
	Serial.println("About to set address");
	relayController.setAddress(0, 0);
	Serial.println("Address set done");
	Particle.function("controlRelay", triggerRelay);
}

/* This function loops forever --------------------------------------------*/
void loop()
{
	int status = relayController.readAllInputs();
	int a = 0;
	for(int i = 1; i < 129; i*=2){
		if(status & i){
			debugTrips[a]++;
			if(debugTrips[a] >= minTrips){
				if(!tripped[a]){
					tripped[a] = true;
					//set input trip event to true
					String eventName = "Input_";
					eventName+=(a+1);
					Particle.publish(eventName, "ON");
					Serial.print("eventName: ");
					Serial.println(eventName);
					Serial.print("eventContents: ");
					Serial.println("ON");
				}
			}
		}else{
			debugTrips[a] = 0;
			if(tripped[a]){
				tripped[a] = false;
				//set input trip event to false
				String eventName = "Input_";
				eventName+=(a+1);
				Particle.publish(eventName, "OFF");
				Serial.print("eventName: ");
				Serial.println(eventName);
				Serial.print("eventContents: ");
				Serial.println("OFF");
			}
		}
		a++;
	}
}
int triggerRelay(String command){
	if(command.equalsIgnoreCase("turnonallrelays")){
		relayController.turnOnAllRelays();
		return 1;
	}
	if(command.equalsIgnoreCase("turnoffallrelays")){
		relayController.turnOffAllRelays();
		return 1;
	}
	if(command.startsWith("turnonallrelaysinbank:")){
		int bank = command.substring(22).toInt();
		relayController.turnOnAllRelays(bank);
		return 1;
	}
	if(command.startsWith("turnoffallrelaysinbank:")){
		int bank = command.substring(23).toInt();
		relayController.turnOffAllRelays(bank);
		return 1;
	}
	if(command.startsWith("setallrelaystatus:")){
		String status = command.substring(18,command.length());
		int splitIndex = status.indexOf(",");
		int secondSplit = status.lastIndexOf(",");
		int bank1 = status.substring(0, splitIndex).toInt();
		int bank2 = status.substring(splitIndex+1,secondSplit).toInt();
		int bank3 = status.substring(secondSplit+1, status.length()).toInt();
		Serial.println("Setting status of all relays: "+String(bank1)+", "+String(bank2)+", "+String(bank3));
		relayController.setAllRelayStatus(bank1, bank2, bank3);
		return 1;
	}
	if(command.startsWith("setbankstatus:")){
		Serial.println("Set Bank Status Command");
		String status = command.substring(14,command.length());
		int splitIndex = status.indexOf(",");
		int bank = status.substring(0,splitIndex).toInt();
		int bankStatus = status.substring(splitIndex+1, status.length()).toInt();
		if(bankStatus < 0 || bankStatus > 255){
			return 0;
		}
		Serial.println("Setting bank "+String(bank)+ " status to: "+String(bankStatus));
		relayController.setBankStatus(bankStatus,bank);
		Serial.println("done");
		return 1;
	}
	//Relay Specific Command
	int splitIndex = command.indexOf(",");
	int relayNumber = command.substring(0,splitIndex).toInt();
	Serial.print("relayNumber: ");
	Serial.println(relayNumber);
	String relayCommand = command.substring(splitIndex+1);
	Serial.print("relayCommand:");
	Serial.print(relayCommand);
	Serial.println(".");
	if(relayCommand.equalsIgnoreCase("on")){
		Serial.println("Turning on relay");
		relayController.turnOnRelay(relayNumber);
		Serial.println("returning");
		return 1;
	}
	if(relayCommand.equalsIgnoreCase("off")){
		relayController.turnOffRelay(relayNumber);
		return 1;
	}
	if(relayCommand.equalsIgnoreCase("toggle")){
		relayController.toggleRelay(relayNumber);
		return 1;
	}
	if(relayCommand.equalsIgnoreCase("momentary")){
		relayController.turnOnRelay(relayNumber);
		delay(300);
		relayController.turnOffRelay(relayNumber);
		return 1;
	}
	return 0;
}
