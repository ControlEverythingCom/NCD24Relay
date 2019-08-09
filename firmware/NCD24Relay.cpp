#include "NCD24Relay.h"
#include "spark_wiring_usbserial.h"

//Comment line below out to turn off Serial logging
#define LOGGING

int address = 0x20;
int address2 = 0x21;
int retrys = 0;

//Constructor
NCD24Relay::NCD24Relay(){
}

//Retry added
void NCD24Relay::setAddress(int a0, int a1){
    address = 0x20;
    if(a0 == 1){
        address = address | 2;
        address2 = address2 | 2;
    }
    if(a1 == 1){
        address = address | 4;
        address2 = address2 | 4;
    }
    //Start I2C port
    Wire.begin();
    //Open connection to specified address
retryAddress1:
    Wire.beginTransmission(address);
    //Set all channels to outputs
    Wire.write(0);
    Wire.write(0);
    Wire.write(0);
    //Determine if device is present at that address
    byte status = Wire.endTransmission();
    if(status != 0){
        if(retrys < 3){
#ifdef LOGGING
            Serial.println("Retry set address command");
#endif
            retrys++;
            goto retryAddress1;
        }else{
#ifdef LOGGING
            Serial.println("Set Address failed");
#endif
            initialized = false;
            retrys = 0;
        }
        
    }else{
        // Serial.println("Command Successful");
        initialized = true;
        retrys = 0;
        readStatus(address);
    }
retryAddress2:
    //Open connection to specified address
    Wire.beginTransmission(address2);
    //Set all channels to outputs
    Wire.write(0);
    Wire.write(0);
    Wire.write(255);
    //Determine if device is present at that address
    status = Wire.endTransmission();
    if(status != 0){
        if(retrys < 3){
#ifdef LOGGING
            Serial.println("Retry set address command");
#endif
            retrys++;
            goto retryAddress2;
        }else{
#ifdef LOGGING
            Serial.println("Set Address2 failed");
#endif
            initialized = false;
            retrys = 0;
        }
    }else{
        // Serial.println("Command Successful");
        initialized = true;
        retrys = 0;
        readStatus(address2);

        retrySetInputPullUps:
        Wire.beginTransmission(address2);
        Wire.write(0x06);
	Wire.write(0);
        Wire.write(255);
        status = Wire.endTransmission();
        if(status != 0){
        	if(retrys < 3){
#ifdef LOGGING
        		Serial.println("Retry set pullups on address2 command");
#endif
        		retrys++;
        		goto retrySetInputPullUps;
        	}else{
#ifdef LOGGING
        		Serial.println("Set Address2 pullups failed");
#endif
        		initialized = false;
        		retrys = 0;
        	}
        }else{
        	retrys = 0;
        	initialized = true;
        }
    }
}

void NCD24Relay::turnOnRelay(int relay){
    byte bankValue = 0;
    byte registerAddress = 0;
    byte addr = 0;
    if(relay <= 8){
        bankValue = bankOneStatus;
        addr = address;
        registerAddress = 18;
    }else{
        if(relay > 8 && relay <=16){
            relay = relay - 8;
            bankValue = bankTwoStatus;
            addr = address;
            registerAddress = 19;
        }else{
            if(relay > 16 && relay <= 24){
                relay = relay - 16;
                bankValue = bankThreeStatus;
                addr = address2;
                registerAddress = 18;
            }else{
                if(relay > 24 && relay <=32){
                    relay = relay - 24;
                    bankValue = bankFourStatus;
                    addr = address2;
                    registerAddress = 19;
                }
            }
        }
        
    }
    switch(relay){
        case 1:
            bankValue = bankValue | 1;
            break;
        case 2:
            bankValue = bankValue | 2;
            break;
        case 3:
            bankValue = bankValue | 4;
            break;
        case 4:
            bankValue = bankValue | 8;
            break;
        case 5:
            bankValue = bankValue | 16;
            break;
        case 6:
            bankValue = bankValue | 32;
            break;
        case 7:
            bankValue = bankValue | 64;
            break;
        case 8:
            bankValue = bankValue | 128;
            break;
    }
turnOnRelayRetry:
    Wire.beginTransmission(addr);
    Wire.write(registerAddress);
    Wire.write(bankValue);
    byte status = Wire.endTransmission();
    if(status != 0){
        if(retrys < 3){
#ifdef LOGGING
            Serial.println("Retry Turn On Relay command");
#endif
            retrys++;
            goto turnOnRelayRetry;
        }else{
#ifdef LOGGING
            Serial.println("Turn On Relay failed");
#endif
            initialized = false;
            retrys = 0;
        }
    }else{
        initialized = true;
        retrys = 0;
        readStatus(addr);
    }
}

void NCD24Relay::turnOffRelay(int relay){
    byte bankValue = 0;
    byte registerAddress = 0;
    byte addr = 0;
    if(relay <= 8){
        bankValue = bankOneStatus;
        addr = address;
        registerAddress = 18;
    }else{
        if(relay > 8 && relay <=16){
            relay = relay - 8;
            bankValue = bankTwoStatus;
            addr = address;
            registerAddress = 19;
        }else{
            if(relay > 16 && relay <= 24){
                relay = relay - 16;
                bankValue = bankThreeStatus;
                addr = address2;
                registerAddress = 18;
            }else{
                if(relay > 24 && relay <=32){
                    relay = relay - 24;
                    bankValue = bankFourStatus;
                    addr = address2;
                    registerAddress = 19;
                }
            }
        }
        
    }
    switch(relay){
        case 1:
            bankValue = bankValue & ~1;
            break;
        case 2:
            bankValue = bankValue & ~2;
            break;
        case 3:
            bankValue = bankValue & ~4;
            break;
        case 4:
            bankValue = bankValue & ~8;
            break;
        case 5:
            bankValue = bankValue & ~16;
            break;
        case 6:
            bankValue = bankValue & ~32;
            break;
        case 7:
            bankValue = bankValue & ~64;
            break;
        case 8:
            bankValue = bankValue & ~128;
            break;
    }
turnOffRelaysRetry:
    Wire.beginTransmission(addr);
    Wire.write(registerAddress);
    Wire.write(bankValue);
    byte status = Wire.endTransmission();
    if(status != 0){
        if(retrys < 3){
#ifdef LOGGING
            Serial.println("Retry Turn Off Relay command");
#endif
            retrys++;
            goto turnOffRelaysRetry;
        }else{
#ifdef LOGGING
            Serial.println("Turn Off Relay command failed");
#endif
            initialized = false;
            retrys = 0;
        }
    }else{
        initialized = true;
        retrys = 0;
        readStatus(addr);
    }
}

void NCD24Relay::turnOnAllRelays(){
turnOnAllRelays1:
    Wire.beginTransmission(address);
    Wire.write(18);
    Wire.write(255);
    Wire.write(255);
    byte status = Wire.endTransmission();
    if(status != 0){
        if(retrys < 3){
#ifdef LOGGING
            Serial.println("Retry turn off all relays banks 1-2 command");
#endif
            retrys++;
            goto turnOnAllRelays1;
        }else{
#ifdef LOGGING
            Serial.println("Turn off all relays banks 1-2 command failed");
#endif
            initialized = false;
            retrys = 0;
        }
    }else{
        initialized = true;
        retrys = 0;
        readStatus(address);
    }
turnOnAllRelays2:
    Wire.beginTransmission(address2);
    Wire.write(18);
    Wire.write(255);
    Wire.write(255);
    status = Wire.endTransmission();
    if(status != 0){
        if(retrys < 3){
#ifdef LOGGING
            Serial.println("Retry turn off all relays banks 3-4 command");
#endif
            retrys++;
            goto turnOnAllRelays2;
        }else{
#ifdef LOGGING
            Serial.println("Turn off all relays banks 3-4 command failed");
#endif
            initialized = false;
            retrys = 0;
        }
    }else{
        initialized = true;
        retrys = 0;
        readStatus(address2);
    }
}

void NCD24Relay::turnOffAllRelays(){
turnOffAllRelaysRetry1:
    Wire.beginTransmission(address);
    Wire.write(18);
    Wire.write(0);
    Wire.write(0);
    byte status = Wire.endTransmission();
    if(status != 0){
        if(retrys < 3){
#ifdef LOGGING
            Serial.println("Retry turn off all relays banks 1-2 command");
#endif
            retrys++;
            goto turnOffAllRelaysRetry1;
        }else{
#ifdef LOGGING
            Serial.println("Turn off all relays banks 1-2 command failed");
#endif
            initialized = false;
            retrys = 0;
        }
    }else{
        initialized = true;
        retrys = 0;
        readStatus(address);
    }
turnOffAllRelaysRetry2:
    Wire.beginTransmission(address2);
    Wire.write(18);
    Wire.write(0);
    Wire.write(0);
    status = Wire.endTransmission();
    if(status != 0){
        if(retrys < 3){
#ifdef LOGGING
            Serial.println("Retry turn off all relays banks 3-4 command");
#endif
            retrys++;
            goto turnOffAllRelaysRetry2;
        }else{
#ifdef LOGGING
            Serial.println("Turn off all relays banks 3-4 commmand failed");
#endif
            initialized = false;
            retrys = 0;
        }
    }else{
        initialized = true;
        retrys = 0;
        readStatus(address2);
    }
}

void NCD24Relay::turnOnAllRelays(int bank){
    byte addr;
    if(bank == 1 || bank == 2){
        addr = address;
    }else{
        if(bank == 3 || bank == 4){
            addr = address2;
        }else{
            //Bad bank value
            return;
        }
    }
turnOffAllRelayssInBankRetry:
    Wire.beginTransmission(addr);
    bank = 17+bank;
    Wire.write(bank);
    Wire.write(255);
    byte status = Wire.endTransmission();
    if(status != 0){
        if(retrys < 3){
#ifdef LOGGING
            Serial.println("Retry turn on all relays in bank command");
#endif
            retrys++;
            goto turnOffAllRelayssInBankRetry;
        }else{
#ifdef LOGGING
            Serial.println("Turn on all relays in bank command failed");
#endif
            initialized = false;
            retrys = 0;
        }
    }else{
        initialized = true;
        retrys = 0;
        readStatus(addr);
    }
}

void NCD24Relay::turnOffAllRelays(int bank){
    byte addr;
    if(bank == 1 || bank == 2){
        addr = address;
    }else{
        if(bank == 3 || bank == 4){
            addr = address2;
        }else{
            //Bad bank value
            return;
        }
    }
turnOnAllRelayssInBankRetry:
    Wire.beginTransmission(addr);
    bank = 17+bank;
    Wire.write(bank);
    Wire.write(0);
    byte status = Wire.endTransmission();
    if(status != 0){
        if(retrys < 3){
#ifdef LOGGING
            Serial.println("Retry turn off all relays in bank command");
#endif
            retrys++;
            goto turnOnAllRelayssInBankRetry;
        }else{
#ifdef LOGGING
            Serial.println("Turn off all relays in bank command failed");
#endif
            initialized = false;
            retrys = 0;
        }
    }else{
        initialized = true;
        retrys = 0;
        readStatus(addr);
    }
}

void NCD24Relay::toggleRelay(int relay){
    byte bankValue = 0;
    byte registerAddress = 0;
    byte addr = 0;
    if(relay <= 8){
        bankValue = bankOneStatus;
        addr = address;
        registerAddress = 18;
    }else{
        if(relay > 8 && relay <=16){
            relay = relay - 8;
            bankValue = bankTwoStatus;
            addr = address;
            registerAddress = 19;
        }else{
            if(relay > 16 && relay <= 24){
                relay = relay - 16;
                bankValue = bankThreeStatus;
                addr = address2;
                registerAddress = 18;
            }else{
                if(relay > 24 && relay <=32){
                    relay = relay - 24;
                    bankValue = bankFourStatus;
                    addr = address2;
                    registerAddress = 19;
                }
            }
        }
        
    }
    switch(relay){
        case 1:
            bankValue = bankValue ^ 1;
            break;
        case 2:
            bankValue = bankValue ^ 2;
            break;
        case 3:
            bankValue = bankValue ^ 4;
            break;
        case 4:
            bankValue = bankValue ^ 8;
            break;
        case 5:
            bankValue = bankValue ^ 16;
            break;
        case 6:
            bankValue = bankValue ^ 32;
            break;
        case 7:
            bankValue = bankValue ^ 64;
            break;
        case 8:
            bankValue = bankValue ^ 128;
            break;
    }
toggleRelayRetry:
    Wire.beginTransmission(addr);
    Wire.write(registerAddress);
    Wire.write(bankValue);
    byte status = Wire.endTransmission();
    if(status != 0){
        if(retrys < 3){
#ifdef LOGGING
            Serial.println("Retry toggle relay command");
#endif
            retrys++;
            goto toggleRelayRetry;
        }else{
#ifdef LOGGING
            Serial.println("Toggle relay command failed");
#endif
            initialized = false;
            retrys = 0;
        }
    }else{
        initialized = true;
        retrys = 0;
        readStatus(addr);
    }
}

void NCD24Relay::setBankStatus(int status, int bank){
    byte addr;
    if(bank == 1 || bank == 2){
        addr = address;
    }else{
        if(bank == 3 || bank == 4){
            addr = address2;
        }else{
            //Bad bank value
            return;
        }
    }
setBankStatusRetry:
    Wire.beginTransmission(addr);
    bank = 17+bank;
    Wire.write(bank);
    Wire.write(status);
    byte s = Wire.endTransmission();
    if(s != 0){
        if(retrys < 3){
#ifdef LOGGING
            Serial.println("Retry set bank status command");
#endif
            retrys++;
            goto setBankStatusRetry;
        }else{
#ifdef LOGGING
            Serial.println("Set bank status command failed");
#endif
            initialized = false;
            retrys = 0;
        }
    }else{
        initialized = true;
        retrys = 0;
        readStatus(addr);
    }
}

void NCD24Relay::setAllRelayStatus(int bank1, int bank2, int bank3){
setAllRelayStatus1:
    Wire.beginTransmission(address);
    Wire.write(18);
    Wire.write(bank1);
    Wire.write(bank2);
    byte status = Wire.endTransmission();
    if(status != 0){
        if(retrys < 3){
#ifdef LOGGING
            Serial.println("Retry set banks 1-2 status command");
#endif
            retrys++;
            goto setAllRelayStatus1;
        }else{
#ifdef LOGGING
            Serial.println("Set banks 1-2 status command failed");
#endif
            initialized = false;
            retrys = 0;
        }
    }else{
        initialized = true;
        retrys = 0;
        readStatus(address);
    }
setAllRelayStatus2:
    Wire.beginTransmission(address2);
    Wire.write(18);
    Wire.write(bank3);
    status = Wire.endTransmission();
    if(status != 0){
        if(retrys < 3){
#ifdef LOGGING
            Serial.println("Retry set banks 3 status command");
#endif
            retrys++;
            goto setAllRelayStatus2;
        }else{
#ifdef LOGGING
            Serial.println("Set banks 3 status command failed");
#endif
            initialized = false;
            retrys = 0;
        }
    }else{
        initialized = true;
        retrys = 0;
        readStatus(address2);
    }
}

int NCD24Relay::readRelayStatus(int relay){
    byte registerAddress = 0;
    int value = 0;
    int addr = 0;
    if(relay <= 8){
        addr = address;
        registerAddress = 18;
    }else{
        if(relay > 8 && relay <=16){
            relay = relay - 8;
            addr = address;
            registerAddress = 19;
        }else{
            if(relay > 16 && relay <= 24){
                relay = relay - 16;
                addr = address2;
                registerAddress = 18;
            }else{
                if(relay > 24 && relay <=32){
                    relay = relay - 24;
                    addr = address2;
                    registerAddress = 19;
                }
            }
        }
        
    }
    switch(relay){
        case 1:
            value = 1;
            break;
        case 2:
            value = 2;
            break;
        case 3:
            value = 4;
            break;
        case 4:
            value = 8;
            break;
        case 5:
            value = 16;
            break;
        case 6:
            value = 32;
            break;
        case 7:
            value = 64;
            break;
        case 8:
            value = 128;
            break;
    }
getRelayStatusRetry:
    Wire.beginTransmission(addr);
    Wire.write(registerAddress);
    byte status = Wire.endTransmission();
    if(status != 0){
        if(retrys < 3){
#ifdef LOGGING
            Serial.println("Retry read relay status command");
#endif
            retrys++;
            goto getRelayStatusRetry;
        }else{
#ifdef LOGGING
            Serial.println("Read relay status command failed");
#endif
            initialized = false;
            retrys = 0;
            return 256;
        }
    }else{
        retrys = 0;
        initialized = true;
    }
    Wire.requestFrom(addr, 1);
    byte bankStatus = Wire.read();
    if(bankStatus & value){
        return 1;
    }else{
        return 0;
    }
}

int NCD24Relay::readBankStatus(int bank){
    int addr;
    if(bank == 1 || bank == 2){
        addr = address;
    }else{
        if(bank == 3){
            addr = address2;
        }else{
        	//Bad bank value
        	return 256;
        }
    }
    bank = bank + 17;
    readBankStatusRetry:
    Wire.beginTransmission(addr);
    Wire.write(bank);
    byte status = Wire.endTransmission();
    if(status != 0){
    	if(retrys < 3){
#ifdef LOGGING
            Serial.println("Retry read bank status command");
#endif
            retrys++;
            goto readBankStatusRetry;
        }else{
#ifdef LOGGING
            Serial.println("Read bank status command failed");
#endif
            initialized = false;
            retrys = 0;
            return 256;
        }
    }else{
        retrys = 0;
        initialized = true;
    }
    Wire.requestFrom(addr, 1);
    byte bankStatus = Wire.read();
    return bankStatus;
}

void NCD24Relay::readStatus(int addr){
readBankOneRetry:
    //Open Connection to controller
    Wire.beginTransmission(addr);
    //Get current status of relays 1-8
    Wire.write(18);
    byte status = Wire.endTransmission();
    if(status != 0){
        if(retrys < 3){
#ifdef LOGGING
            Serial.println("Retry read status banks 1-2 command");
#endif
            retrys++;
            goto readBankOneRetry;
        }else{
#ifdef LOGGING
            Serial.println("Read ststus banks 1-2 command failed");
#endif
            initialized = false;
            retrys = 0;
        }
    }else{
        retrys = 0;
        initialized = true;
    }
    Wire.requestFrom(addr, 1);
    if(addr == address){
        bankOneStatus = Wire.read();
    }else{
        if(addr == address2){
            bankThreeStatus = Wire.read();
        }
    }
    //Get current status of relays 9-16
readBankTwoRetry:
    Wire.write(19);
    status = Wire.endTransmission();
    if(status != 0){
        if(retrys < 3){
#ifdef LOGGING
            Serial.println("Retry read status banks 3-4 command");
#endif
            retrys++;
            goto readBankTwoRetry;
        }else{
#ifdef LOGGING
            Serial.println("Read ststus banks 1-2 command failed");
#endif
            initialized = false;
            retrys = 0;
        }
    }else{
        retrys = 0;
        initialized = true;
    }
    Wire.requestFrom(addr, 1);
    if(addr == address){
        bankTwoStatus = Wire.read();
    }else{
        if(addr == address2){
            bankFourStatus = Wire.read();
        }
    }
    
    
}

int NCD24Relay::readAllInputs(){
	int addr = address2;
	int bank = 19;
	readBankStatusRetry:
	Wire.beginTransmission(addr);
	Wire.write(bank);
	byte status = Wire.endTransmission();
	if(status != 0){
		if(retrys < 3){
#ifdef LOGGING
			Serial.println("Retry read inputs status command");
#endif
			retrys++;
			goto readBankStatusRetry;
		}else{
#ifdef LOGGING
			Serial.println("Read all inputs status command failed");
#endif
			initialized = false;
			retrys = 0;
			return 256;
		}
	}else{
		retrys = 0;
		initialized = true;
	}
	Wire.requestFrom(addr, 1);
	    byte bankStatus = Wire.read();
	    bankStatus = ~bankStatus;
	    return bankStatus;
}

int NCD24Relay::readInputStatus(int channel){
	int value = 0;
	int addr = address2;
	int bank = 19;

	switch(channel){
	case 1:
		value = 1;
		break;
	case 2:
		value = 2;
		break;
	case 3:
		value = 4;
		break;
	case 4:
		value = 8;
		break;
	case 5:
		value = 16;
		break;
	case 6:
		value = 32;
		break;
	case 7:
		value = 64;
		break;
	case 8:
		value = 128;
		break;
	}
	readInputStatusRetry:
	Wire.beginTransmission(addr);
	Wire.write(bank);
	byte status = Wire.endTransmission();
	if(status != 0){
		if(retrys < 3){
#ifdef LOGGING
Serial.println("Retry read inputs status command");
#endif
retrys++;
goto readInputStatusRetry;
		}else{
#ifdef LOGGING
Serial.println("Read all inputs status command failed");
#endif
initialized = false;
retrys = 0;
return 256;
		}
	}else{
		retrys = 0;
		initialized = true;
	}
	Wire.requestFrom(addr, 1);
	byte bankStatus = Wire.read();
	bankStatus = ~bankStatus;
	if(bankStatus & value){
		return 1;
	}else{
		return 0;
	}

}
