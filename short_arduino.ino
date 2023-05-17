// commHandler is a script that takes the serial command from the raspberry pi and sends the correct pulse along the correct pin to the bonsai software.

// includes
#include <SoftwareSerial.h>
#include <Firmata.h>


char* sdata = ""; // Initialize


// Software serial pins (rx, Tx)
int softRX = 2;
int softTX = 3;
SoftwareSerial myserial(softRX, softTX);

void setup () {


  // setup the serial
  Serial.begin(57600);
  Serial.println("Starting Serial Dialogue");

  // Setup the software serial port
  myserial.begin(9600);

  // Setup the Firmata
  Firmata.setFirmwareVersion(FIRMATA_MAJOR_VERSION, FIRMATA_MINOR_VERSION);
  Firmata.attach(ANALOG_MESSAGE, analogWriteCallback);
  Firmata.begin(57600);
}

void loop () {
  // Read the serial for the command
  byte ch;
  if (myserial.available()) {
    ch = myserial.read();
    

    if (ch == '\r') { // End of the command, full line has been recieved and is ready to go
      char *com_ptr = strtok(sdata, (char)'|');
      byte pin = (byte)com_ptr;
      com_ptr = strtok(NULL, (char)'|');
      int value = (int)com_ptr;
      commands(pin, value);

      // Re-initialize the variable
      sdata = "";
    }
    else {
      sdata += (char)ch;
    }
  }
  else {
    // Send a 0 value to bonsai
    //Firmata.sendAnalog(messagePin, 0);
  }
}

void commands (byte command_pin, int command_value) {

  Firmata.sendAnalog(command_pin, command_value);

}

void analogWriteCallback(byte pin, int value) {
  analogWrite(pin, value);
}