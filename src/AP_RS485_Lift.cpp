//*****************************************************************************************************
//
// file:      AP_RS485_Lift.cpp
// author:    Aiko Pras
// history:   2021-02-23 V0.1 ap First version
//            2022-01-18 V1.1 ap Library name changed and IR added
//
// purpose:   Wrapper around Nick Gammon's Non-blocking RS485 class
//            All RS485 details will be hidden here.
//            The RS485 interface is connected to UART3 (= serial3)
//            With a baudrate of 115200 transmission of 2 databytes costs 700us, 3=870us and 4=1040us
//
//*****************************************************************************************************
#include <Arduino.h>
#include "AP_RS485_Lift.h"
#include "RS485_non_blocking.h"

#define BUFFER_SIZE   20           // used for Nick Gammon's NG485 object
#define USART_SPEED   115200       // baudrate - Results in 1040 microseconds
//#define USART_SPEED   230400       // baudrate

// USART specific settings - Do not change
#ifndef USART_NUMBER
  #warning USART is not defined
#elif (USART_NUMBER==0)
  #define USART       Serial       // Decide which Serial will be used
  #define UCSRXA      UCSR0A       // USART Control and Status Register A
  #define UDREX       UDRE0        // USART Data Register Empty
  #define TXCX        TXC0         // USART Transmit Complete
#elif (USART_NUMBER==1)
  #define USART       Serial1
  #define UCSRXA      UCSR1A
  #define UDREX       UDRE1
  #define TXCX        TXC1
#elif (USART_NUMBER==2)
  #define USART       Serial2
  #define UCSRXA      UCSR2A
  #define UDREX       UDRE2
  #define TXCX        TXC2
#elif (USART_NUMBER==3)
  #define USART       Serial3
  #define UCSRXA      UCSR3A
  #define UDREX       UDRE3
  #define TXCX        TXC3
#else
  #warning Invalid USART number (must be 0..3)
#endif


//******************************************************************************************************
// Define the call-back functions to use Nick Gammon's RS485_non_blocking library and instatiate NG485
// Nick Gammon's library is used by the main program for sending and receiving data. Underwater that
// library uses standard Arduino libraries for the actual transmission and reception. The call-back
// functions determine if we use hardware (USART) calls, or software (softserial) for that.
// We use hardware (USART) calls.
//******************************************************************************************************
int read () { return USART.read (); }
int available () { return USART.available (); }
size_t send (const byte what) { return USART.write (what); }


// Instantiate the NG485 object by providing the three call-back functions plus a maximum buffer length
// NG485 represents Nick Gammons RS485 object, which is "encapsulated" by my own RS485 object
RS485 NG485 (read, available, send, BUFFER_SIZE);


// SendMsg is a private method that calls the NG485 object for the actual sending
void RS485_Lift::sendMsg (const byte * data, const byte length) {
  digitalWrite(RS485_ENABLE, HIGH);   // Enable sending
  NG485.sendMsg (data, length);
  // As descibed on https://www.gammon.com.au/forum/?id=11428, the RS485 chip might be
  // "turned off" too quickly, because the last byte is still being sent from the serial
  // hardware port. Therefore we have to monitor the USART registers for completion
  // NOTE: the code below uses busy waiting. A "cleaner" approach might have been an ISR
  // See also Section "22.6.3 - Transmitter Flags and Interrupts" of the 2560 datasheet
  while (!(UCSRXA & (1 << UDREX)))    // Wait for empty transmit buffer
    UCSRXA |= 1 << TXCX;              // mark transmission not complete
  while (!(UCSRXA & (1 << TXCX)));    // Wait for the transmission to complete
  // Now we can disable the RS485 sender
  digitalWrite(RS485_ENABLE, LOW);
}


//******************************************************************************************************
//                                            PUBLIC METHODS
//******************************************************************************************************
// The constructor below initialises the object
RS485_Lift::RS485_Lift(const byte addres) {
  pinMode(RS485_ENABLE, OUTPUT);
  digitalWrite(RS485_ENABLE, LOW);     // Initialise as listening
  USART.begin (USART_SPEED);
  NG485.begin ();                      // Allocate memory for a buffer with size BUFFER_SIZE
  my_addres = addres;                  // Set the RS485 address of this decoder
}


// input schould be called from the main loop as often as possible
bool RS485_Lift::input() {
  byte result = false;
  if (NG485.update()) {
    if (NG485.getLength() >= 1) {
      byte *data = NG485.getData();
      to      = (data[0] &  0b11000000) >> 6;
      from    = (data[0] &  0b00110000) >> 4;
      command = (data[0] &  0b00001111);
      if (to == my_addres) result = true;
      if (NG485.getLength() == 2) {
        action = (data[1] &  0b11110000) >> 4;
        value  = (data[1] &  0b00001111);
      }
    }
  }
  return result;
}


// The Master sends Poll messages to the buttons or the IR_Led controller
void RS485_Lift::sendPoll(const byte address) {
  byte msg[1];
  msg[0] = (address << 6) + (MASTER_ADDR << 4) + POLL;
  RS485_Lift::sendMsg(msg, 1);
}


// Acks are always to the master, so an address parameter is not needed
void RS485_Lift::sendAck() {
  byte msg[1];
  msg[0] = (MASTER_ADDR << 6) + (my_addres << 4) + ACK;
  RS485_Lift::sendMsg(msg, 1);
}


void RS485_Lift::sendButtons(const byte action, const byte value) {
  byte msg[2];
  msg[0] = (MASTER_ADDR << 6) + (my_addres << 4) + BUTTON;
  msg[1] = (action << 4) + value;
  RS485_Lift::sendMsg(msg, 2);
}


void RS485_Lift::setButtonLEDs(const byte action, const byte value) {
  byte msg[2];
  msg[0] = (BUTTONS_ADDR << 6) + (MASTER_ADDR << 4) + BUTTON_LED;
  msg[1] = (action << 4) + value;
  RS485_Lift::sendMsg(msg, 2);
}


void RS485_Lift::sendIrSensorsFree() {
  byte msg[1];
  msg[0] = (MASTER_ADDR << 6) + (my_addres << 4) + IR_FREE;
  RS485_Lift::sendMsg(msg, 1);}


void RS485_Lift::sendIrSensorsBusy() {
  byte msg[1];
  msg[0] = (MASTER_ADDR << 6) + (my_addres << 4) + IR_BUSY;
  RS485_Lift::sendMsg(msg, 1);}
