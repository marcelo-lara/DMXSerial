// - - - - -
// DmxSerial - A hardware supported interface to DMX.
// DmxSerialRecv.ino: Sample DMX application for retrieving 3 DMX values:
// address 1 (red) -> PWM Port 9
// address 2 (green) -> PWM Port 6
// address 3 (blue) -> PWM Port 5
// 
// Copyright (c) 2011-2015 by Matthias Hertel, http://www.mathertel.de
// This work is licensed under a BSD style license. See http://www.mathertel.de/License.aspx
// 
// Documentation and samples are available at http://www.mathertel.de/Arduino
// 25.07.2011 creation of the DmxSerial library.
// 10.09.2011 fully control the serial hardware register
//            without using the Arduino Serial (HardwareSerial) class to avoid ISR implementation conflicts.
// 01.12.2011 include file and extension changed to work with the Arduino 1.0 environment
// 28.12.2011 changed to channels 1..3 (RGB) for compatibility with the DmxSerialSend sample.
// 10.05.2012 added some lines to loop to show how to fall back to a default color when no data was received since some time.
// - - - - -

#include <DMXSerial.h>

// Constants for demo program

const int RedPin =    12;  // PWM output pin for Red Light.
const int GreenPin =  11;  // PWM output pin for Green Light.
const int BluePin =   10;  // PWM output pin for Blue Light.

#define RedDefaultLevel   0
#define GreenDefaultLevel 0
#define BlueDefaultLevel  0

void setup () {
  DMXSerial.init(DMXReceiver);
  
  // set some default values
  DMXSerial.write(1, 255);
  DMXSerial.write(2, 255);
  DMXSerial.write(3, 255);
  
  pinMode(LED_BUILTIN, OUTPUT);

  // enable pwm outputs
  pinMode(RedPin,   OUTPUT); // sets the digital pin as output
  pinMode(GreenPin, OUTPUT);
  pinMode(BluePin,  OUTPUT);
}


void loop() {
  // Calculate how long no data backet was received
  unsigned long lastPacket = DMXSerial.noDataSince();
  
  if (lastPacket < 5000) {
    digitalWrite(LED_BUILTIN, LOW);    
    // read recent DMX values and set pwm levels 
    analogWrite(RedPin,   DMXSerial.read(1));
    analogWrite(GreenPin, DMXSerial.read(2));
    analogWrite(BluePin,  DMXSerial.read(3));

  } else {
    digitalWrite(LED_BUILTIN, HIGH);
    // Show pure red color, when no data was received since 5 seconds or more.
    analogWrite(RedPin,   RedDefaultLevel);
    analogWrite(GreenPin, GreenDefaultLevel);
    analogWrite(BluePin,  BlueDefaultLevel);
  } // if
}

// End.
