// ***********************************************************************************************************
//
//                  Test the RS485 connections for the Lift Decoder - Slave Version
//
// purpose:   To tests the RS485 between a Master and a Slave. This sketch is for the slave!!
// variant:   Slave 
// author:    Aiko Pras
// version:   2023-11-14 V1.0 ap initial version
//
// The code is has been tested on the following lift controller boards 
// - SMD board: https://oshwlab.com/aikopras/support-lift-controller
// - THT board: https://oshwlab.com/aikopras/lift-decoder-arduino-mega-tht
//
// Instructions for compiling, see
// - SMD board: https://github.com/aikopras/Lift_Vitrine/blob/main/extras/Board-SMD/Compile.md
// - THT board: https://github.com/aikopras/Lift_Vitrine/blob/main/extras/Board-THT/Compile.md
//
// hardware:  - For the RS485 transmit and receive: USART 3 (see AP_RS485_Lift.h)
//            - For RS485_ENABLE: PIN_PJ2 or PIN_PE4
//
// About the test:
// Before compilation, the address of the slave must be set. The address with value 1 is for the
// Button Controller (the default test); value 2 is for the IR-Sensor.
// At startup, the slave will turn its red (onboard) LED off. Every 100ms the slave should receive from
// the master a RS485 message. Immediately after reception of that RS485 message, the slave will toggle
// its red LED. Therefore, if the RS485 interface is working well, the red LED should blink 10 times
// per second. In case of problems, the red LED will stay on or off, however.
//
// ***********************************************************************************************************
// Set the address of the slave that should be tested
#define SLAVE_ADDRESS 1          // The address with value 1 is for the Button Controller board
//#define SLAVE_ADDRESS 2          // The address with value 2 for the IR-Sensor Controller board


// ***********************************************************************************************************
//                                           Do not edit below this line
// ***********************************************************************************************************
#include <Arduino.h>
#include <AP_RS485_Lift.h>        // The library that is tested by this sketch
// The library uses for the RS485 transmit and receive: USART 3 (see AP_RS485_Lift.h)
// The library uses for RS485_ENABLE: PIN_PJ2 (SMD board) as well as PIN_PE4 (THT board)

RS485_Lift myRS485(SLAVE_ADDRESS);    // Instantiate from the library the RS485 object

void setup() {
  // Turn on the internal LED, to signal we have power
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  if (myRS485.input()) { 
    myRS485.sendAck();
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
}
