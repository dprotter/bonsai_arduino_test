#include<stdlib.h>

String sdata = ""; // Initialize

void setup() {
  // Begin the Serial at 9600 Baud
  Serial.begin(9600);
  // Setup the Firmata


}

void loop() {
  // Read the serial for the command
  byte ch;
  
  if (Serial.available()>0) {
    
    ch = Serial.read();

    if (ch == '\r') { // End of the command, full line has been recieved and is ready to go

      Serial.println(sdata);

      // Re-initialize the variable
      sdata = "";
    }
    else {
      sdata += (char)ch;
    }
  }
}
