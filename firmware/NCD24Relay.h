#include "spark_wiring_i2c.h"
#include "spark_wiring_usbserial.h"
#include "spark_wiring_constants.h"

class NCD24Relay{
public:
    //Constructor
    NCD24Relay(void);
    //Set Address.  Indicate status of jumpers on board.  Send 0 for not installed, send 1 for installed
    void setAddress(int a0, int a1);
    //Turn on Relay
    void turnOnRelay(int Relay);
    //Turn off Relay
    void turnOffRelay(int Relay);
    //Turn On all Relays
    void turnOnAllRelays();
    //Turn Off All Relays
    void turnOffAllRelays();
    //Turn On All Relays in bank
    void turnOnAllRelays(int bank);
    //Turn Off All Relays in bank
    void turnOffAllRelays(int bank);
    //Toggle Relay
    void toggleRelay(int relay);
    //Set status of all relays in bank
    void setBankStatus(int status, int bank);
    //Set status of all relays on board
    void setAllRelayStatus(int bank1, int bank2, int bank3);
    //Read status of relay. Valid return 0 for off 1 for on.  256 returned if there is an error
    int readRelayStatus(int relay);
    //Read status of all relays in bank.  0-255 valid.  256 returned on error
    int readBankStatus(int bank);
    //Read status of input. Valid return 0 for open 1 for closed. 256 returned if there is an error
    int readInputStatus(int channel);
    //Read status of all inputs
    int readAllInputs();
    
    //User Accessible Variables
    //Whether or not the controller is ready to accept commands
    bool initialized;
    
private:
    //internal use method for refreshing bank status variables
    void readStatus(int addr);
    //Status of relays in bank 1
    byte bankOneStatus;
    //Status of relays in bank 2
    byte bankTwoStatus;
    //Status of relays in bank 3
    byte bankThreeStatus;
    //Status of relays in bank 4
    byte bankFourStatus;
};
