// - - - - -
// DmxSerial - A hardware supported interface to DMX.
// DmxSerialSend.ino: Sample DMX application for sending 3 DMX values.
// There colors in the 3 lists (RedList, GreenList and BlueList) are placed into the DMX buffer with a slow fade.
// DMXSerial works in the background and constantly sends the actual values over the DMX interface.
// The actual values are also available on the built in PWM ports:
// address 1 (red) -> also available on PWM Port 9
// address 2 (green) -> also available on PWM Port 6
// address 3 (blue) -> also available on PWM Port 5
// 
// Copyright (c) 2011-2015 by Matthias Hertel, http://www.mathertel.de
// This work is licensed under a BSD style license. See http://www.mathertel.de/License.aspx
// 
// Documentation and samples are available at http://www.mathertel.de/Arduino
// The repository is on github : https://github.com/mathertel/DMXSerial
// The repository on github is made available in the Arduino Library Manager.
//
// 25.07.2011 creation of the DmxSerial library.
// 10.09.2011 fully control the serial hardware register
//            without using the Arduino Serial (HardwareSerial) class to avoid ISR implementation conflicts.
// 01.12.2011 include file and extension changed to work with the Arduino 1.0 environment
// - - - - -
#include <DMXSerial.h>

// Constants for demo program

const int RedPin =    12;  // PWM output pin for Red Light.
const int GreenPin =  11;  // PWM output pin for Green Light.
const int BluePin =   10;  // PWM output pin for Blue Light.

// The color fading pattern

int RedList[]   = {255, 128,   0,   0,   0, 128};
int GreenList[] = {  0, 128, 255, 128,   0,   0};
int BlueList[]  = {  0,   0,   0, 128, 255, 128};

int RedLevel, GreenLevel, BlueLevel;

int RedNow = 0;
int GreenNow = 0;
int BlueNow = 0;

int state = 0;

void setup() {
  DMXSerial.init(DMXController,2);

  Serial.begin(115200);
  Serial.println("awaiting data..");

  pinMode(RedPin,   OUTPUT); // sets the digital pin as output
  pinMode(GreenPin, OUTPUT);
  pinMode(BluePin,  OUTPUT);
  //pinMode(LED_BUILTIN, OUTPUT);
  //digitalWrite(LED_BUILTIN, LOW);
} // setup


// loop through the rainbow colors 
void loop() {
  while (Serial.available() > 0) {
    serialBuffer[serialPos]=Serial.read();
    switch (serialPos){
      
      case 0: //await startByte
        if(serialBuffer[0]==startByte)
          serialPos=1;
        break;
      
      case 33: //expect endByte, otherwise drop packet
        serialPos=0;
        if(serialBuffer[33]==endByte){
          Serial.print("dmx> ");
          for (int i = 1; i < 32; i++)
          {
            Serial.print(serialBuffer[i], HEX);
            Serial.print(" ");
            DMXSerial.write(i, serialBuffer[i]);
          }
          Serial.println();
        }else{
          Serial.println("malformed packet.. drop");
        }
        break;
      
      default:
        serialPos++;
        break;
      }
  }
} // loop

#define startByte 0x2F // "/"
#define endByte   0x5C // "\"

byte serialBuffer[34];
int  serialPos=0;
int  lastBuffer=0;
void serialRead(){
}
