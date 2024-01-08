//******************************************************************************************************
//
//            Test the RS485 connections for the Lift Decoder - Master Version for a single slave
//
// purpose:   To tests the RS485 between a Master and a Slave. This sketch is for the master!!
// variant:   Master: to test a single slave (with address 1)
// author:    Aiko Pras
// version:   2023-11-14 V1.0 ap initial version
//            2024-01-07 V1.1 ap 
//
//
// About the test:
// At startup, the red onboard LED will be switched on. Every 100 ms the master will send
// a RS485 message to the slave, and switch its own red LED off. After the slave receives
// the RS485 message, it will immediately return a reply message. Once the reply message
// from the slave is received by the master, the red master LED will be switched on again.
// Since the reply message should be received within a few ms, the red LED seems to be 
// continuously on, provided the RS485 interface is working well. 
// In case of problems, the red LED will stay off, however.
//
//******************************************************************************************************
#include <Arduino.h>
#include <AP_RS485_Lift.h>        // The library that is tested by this sketch
// The library uses for the RS485 transmit and receive: USART 3 (see AP_RS485_Lift.h)
// The library uses for RS485_ENABLE: PIN_PJ2 (SMD board) as well as PIN_PE4 (THT board)

// Defines and objects used by this script
#define ADDRESS 1                 // RS485 destination address
RS485_Lift myRS485(MASTER_ADDR);
unsigned long currentMillis;
unsigned long previousMillis;
unsigned long interval = 100;


//******************************************************************************************************
//                                                  Setup
//******************************************************************************************************
void setup() {
  // Turn on the internal red LED, to signal we have power
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
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
    digitalWrite(LED_BUILTIN, LOW);
    myRS485.sendPoll(ADDRESS);
  }
  if (myRS485.input()) { 
    digitalWrite(LED_BUILTIN, HIGH);
  }
}
