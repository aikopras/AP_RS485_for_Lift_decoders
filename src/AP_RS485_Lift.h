//*****************************************************************************************************
//
// file:      AP_RS485_Lift.h
// author:    Aiko Pras
// history:   2021-03-06 V0.1 ap First version
//            2022-01-18 V1.1 ap Library name changed. IR added
//            2024-01-07 V1.2 ap Added a second Enable pin, to allow two boards (SMD amd THT)
//
// purpose:   Wrapper around Nick Gammon's Non-blocking RS485 class
//            All RS485 details will be hidden here.
//
//*****************************************************************************************************
#pragma once

// Hardware settings. May be changed for other hardware
#define USART_NUMBER  3            // Decide which USART) will be used: 0, 1, 2 or 3
#define RS485_ENABLE_SMD  PIN_PJ2  // SMD Board: Pin to enable / disable the 485 hardware chip
#define RS485_ENABLE_THT  PIN_PE4  // THT Board: Pin to enable / disable the 485 hardware chip


//*****************************************************************************************************
//  RS485 Packet structure
//  +-----+-----+-----------+     +-----------+-----------+
//  |     |     |           |     |           |           |
//  +--+--+--+--+--+--+--+--+     +--+--+--+--+--+--+--+--+
//    TO    FROM   COMMAND           NUMBER       VALUE
//   0..3   0..3    0..15             0..15       0..15
//
// TO and FROM address fields:
#define MASTER_ADDR    0      // Address of the main Lift controller
#define BUTTONS_ADDR   1      // Address of the buttons controller
#define IR_LEDS_ADDR   2      // Address of the IR-LED controller

// COMMAND field
#define POLL           0      // Single byte poll command from Master to Slave
#define ACK            1      // Single byte Acknowledgement from Slave to Master
#define BUTTON         2      // Two byte reply from Slave to Master to indicate a button action
#define BUTTON_LED     3      // Two byte command from Master to Slave to indicate a LED action
#define IR_FREE        4      // One byte reply from Slave to Master that IR sensor status is free
#define IR_BUSY        5      // One byte reply from Slave to Master that IR sensor status is occupied

// VALUE field for BUTTON commands
#define SHORTPRESS     0
#define LONGPRESS      1
#define PRESSED        2
#define RELEASED       3

// VALUE field for BUTTON_LED replies
#define LED_OFF        0
#define LED_ON         1
#define SINGLE_FLASH   2      // Flash of 500 ms
#define FLASH_SLOW     3      // 500ms on, followed by 500ms off
#define FLASH_FAST     4      // 100ms on, followed by 200ms off
#define DELAYED_OFF    5      // ON, but after 2 seconds OFF



class RS485_Lift {
public:
  // ****** Attributes ******
  byte my_addres;             // Stores the address of this controller
  byte to;                    // Field in receive packet
  byte from;                  // Field in receive packet
  byte command;               // Field in receive packet
  byte action;                // Field in receive packet
  byte value;                 // Field in receive packet
  
  // ****** Methods ******
  // Constructor for initialisation
  RS485_Lift(const byte addres);
  
  // input schould be called from the main loop as often as possible
  bool input();     // Check if we received a RS485 message
  
  // Methods to send the various commands
  void sendPoll(const byte address);
  void sendAck();
  void sendButtons(const byte action, const byte number);
  void setButtonLEDs(const byte action, const byte value);
  void sendIrSensorsFree();
  void sendIrSensorsBusy();

  
private:
  void sendMsg(const byte * data, const byte length);
};

//*****************************************************************************************************





//*****************************************************************************************************


