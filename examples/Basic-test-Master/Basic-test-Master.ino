//******************************************************************************************************
//
//            Very basic test of RS485 - Master Version 
//
// purpose:   Most simplistic test of RS485 between a Master and a Slave. This sketch is for the master!!
// variant:   Master:
// author:    Aiko Pras
// version:   2024-02-08 V1.0 ap initial version
//
//
// About the test:
// This test does not use any of the RS485 libraries, and is basically a simple RS485 hardware test.
// At startup, the red onboard LED is switched on, to indicate there is power and the program runs. 
// Every second the master sends the "A" character via RS485 to the slave. 
//
// The RS485 Enable Pin stays high (5V), so its value can be checked with a volt meter.
// The best place to measure, are the pins 2 and 3 (which are interconnected) of the Max485 chip.
//
// For testing, you should try different baudrates (see the #define below). Start with a low value
// (such as 9600 baud), which should work even if the wiring between both boards is of bad quality.
// Increase the baudrate to 230400 baud and test correct operation. If it works at 230400 baud it will
// also run at 115200 baud, which is used within the lift decoder software.
// Off course the baudrate at master and slave should have the same value.
//
//******************************************************************************************************
#include <Arduino.h>
// Baudrate used for communication. Lower values are more reliable.
// For testing, increase the value to 115200 and 230400
// Ensure that the master and slave operate at the same baudrate
#define BAUDRATE 9600
//#define BAUDRATE 230400

//******************************************************************************************************
//                                     No need to edit the code below
//******************************************************************************************************
// Declaration of some variables that are needed for sending every second 
unsigned long currentMillis;
unsigned long previousMillis;
unsigned long interval = 1000;


//******************************************************************************************************
//                                                  Setup
//******************************************************************************************************
void setup() {
  // Turn on the internal red LED, to signal we have power
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  // Initialise the USART to be used for RS485. The lift decoder boards use USART 3 for RS485.
  Serial3.begin(BAUDRATE); 
  // Initialise the RS485 Enable Pins 
  pinMode(PIN_PJ2, OUTPUT);      // for the SMD board
  pinMode(PIN_PE4, OUTPUT);      // for the THT board
  digitalWrite(PIN_PJ2, HIGH);   // Enable sending
  digitalWrite(PIN_PE4, HIGH);   // Enable sending
  // Store the current time, to facilitate 1 second ticks
  previousMillis = millis();
}


//******************************************************************************************************
//                                               Main loop
//******************************************************************************************************
void loop() {
  // We send one command per 100 ms
  currentMillis = millis();
  if(currentMillis - previousMillis > interval) { 
    previousMillis = currentMillis;
    Serial3.print('A');
  };
};
