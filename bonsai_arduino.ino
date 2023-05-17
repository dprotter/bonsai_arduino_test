// commHandler is a script that takes the serial command from the raspberry pi and sends the correct pulse along the correct pin to the bonsai software.

// includes
#include <SoftwareSerial.h>
#include <Firmata.h>

byte pins[] = {13,10,9,4,5,6,7,11,8};
int vals[] = {25,50,100,150,200,250,300,350,400};

int lever_out_food_pin      = pins[0];
int lever_out_door_1_pin    = pins[1];
int lever_out_door_2_pin    = pins[2];
int lever_press_food_pin    = pins[3];
int lever_press_door_1_pin  = pins[4];
int lever_press_door_2_pin  = pins[5];
int cross_door_1_pin        = pins[6];
int cross_door_2_pin        = pins[7];
int pellet_retrieved_pin    = pins[8];
int messagePin              = A1;


String sdata = ""; // Initialize
byte pinCount = sizeof(pins);

// Software serial pins (rx, Tx)
int softRX = 2;
int softTX = 3;
SoftwareSerial myserial(softRX,softTX);

void setup () {
    // Set the right pins for the right commands
    for (byte i = 0; i < pinCount; i++) {
        pinMode(pins[i], OUTPUT);
    }

    // setup the serial
    Serial.begin(57600);
    Serial.println("Starting Serial Dialogue");

    // Setup the software serial port
    myserial.begin(9600);

    for(int i = 0; i<4; i++){
      digitalWrite(lever_out_food_pin, HIGH);
      digitalWrite(lever_press_door_1_pin, HIGH);
      delay(500);
      digitalWrite(lever_out_food_pin, LOW);
      digitalWrite(lever_press_door_1_pin, LOW);
      delay(500);
    }
    
    // Test the csv parsing
    //CSV_Parser cp('commands.csv', /*format*/, "ss", /*has_header*/, false);
    //cp.print();

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
        sdata += (char)ch;
            
        if (ch == '\r') { // End of the command, full line has been recieved and is ready to go
            sdata.trim();

            // Send to the command handler
            commands(sdata);

            // Re-initialize the variable
            sdata = "";
        }
        else {
          // Send a 0 value to bonsai
          //Firmata.sendAnalog(messagePin, 0);
        }
    }
    else {
      // Send a 0 value to bonsai
      //Firmata.sendAnalog(messagePin, 0);
    }
}

void commands (String command) {
    if (command == "lever_out_food") {
      // Food Lever is extended
      Firmata.sendAnalog(messagePin  , vals[0]);
      digitalWrite(lever_out_food_pin  , HIGH);
      //delay(50);
    }
    else if (command == "lever_out_door_1") {
      // Partner lever is extended
      Firmata.sendAnalog(messagePin, vals[1]);
      digitalWrite(lever_out_door_1_pin, HIGH);
      //delay(50);
    }
    else if (command == "lever_out_door_2") {
      // Novel lever is extended
      Firmata.sendAnalog(messagePin, vals[2]);
      digitalWrite(lever_out_door_2_pin, HIGH);
      //delay(50);
    }
    else if (command == "lever_press_food") {
      // Food lever is pressed
      Firmata.sendAnalog(messagePin, vals[3]);
      digitalWrite(lever_press_food_pin, HIGH);
      //delay(50);
    }
    else if (command == "lever_press_door_1") {
      // Partner lever is pressed
      Firmata.sendAnalog(messagePin, vals[4]);
      digitalWrite(lever_press_door_1_pin, HIGH);
      //delay(50);
    }
    else if (command == "lever_press_door_2") {
      // Novel lever is pressed
      Firmata.sendAnalog(messagePin, vals[5]);
      digitalWrite(lever_press_door_2_pin, HIGH);
      //delay(50);
    }
    else if (command == "cross_door_1") {
      // Animal entered the novel chamber
      Firmata.sendAnalog(messagePin, vals[6]);
      digitalWrite(cross_door_1_pin, HIGH);
      //delay(50);
    }
    else if (command == "cross_door_2") {
      // Animal entered the partner chamber
      Firmata.sendAnalog(messagePin, vals[7]);
      digitalWrite(cross_door_2_pin, HIGH);
      //delay(50);
    }
    else if (command == "pellet_retrieved") {
      // Animal entered the partner chamber
      Firmata.sendAnalog(messagePin, vals[8]);
      digitalWrite(cross_door_2_pin, HIGH);
    }
    else if (command == "startup_test") {
      // Animal entered the partner chamber
      for (int i = 0; i<5; i++){
        digitalWrite(lever_out_food_pin, HIGH);
        delay(250);
        digitalWrite(lever_out_food_pin, LOW);
      }
    }
    else {
      // Send a 0 value
    }

    // Reset all the values to LOW
    //delay(2000);
    for (byte i = 0; i < pinCount; i++) {
        digitalWrite(pins[i], LOW);
    }
}

void analogWriteCallback(byte pin, int value) {
  analogWrite(pin, value);
}
