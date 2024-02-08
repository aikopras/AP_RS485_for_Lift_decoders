//******************************************************************************************************
//
//            Very basic test of RS485 - Slaves Version 
//
// purpose:   Most simplistic test of RS485 between a Master and a Slave. This sketch is for the slaves!!
// variant:   Slave. Multiple slave boards may be connected to the same RS485 bus!
// author:    Aiko Pras
// version:   2024-02-08 V1.0 ap initial version
//
//
// About the test:
// This test does not use any of the RS485 libraries, and is basically a simple RS485 hardware test
// At startup, the red onboard LED is switched on, to indicate there is power and the program runs. 
// Every second the master sends the "A" character via RS485 to the slave. 
//
// Everytime this "A" character is received, the green LED changes between ON andf OFF.
// If there is an error, the yellow LED is switched ON.
// If there are more errors, also the blue LED is switched ON. The number of errors that
// triggers the blue LED can be changed via the #deine BLUE_ERRORS.
//
// If there are no errors, the green LED blinks with a frequency of 1 Hz, and the 
// yellow and blue LEDs remain off.
//
// The RS485 Enable Pin stays low (0V), so its value can be checked with a volt meter.
// The best place to measure, are the pins 2 and 3 (which are interconnected) of the Max485 chip.
//
// For testing, you should try different baudrates (see the explanation in the master's sketch)
//
//******************************************************************************************************
#include <Arduino.h>
// Baudrate used for communication. Lower values are more reliable.
// For testing, increase the value to 115200 or 230400
// Ensure that the master and slave operate at the same baudrate
#define BAUDRATE 9600
//#define BAUDRATE 230400
#define BLUE_ERRORS 3


//******************************************************************************************************
//                                     No need to edit the code below
//******************************************************************************************************
const byte LED_BLUE1 = PIN_PD4;
const byte LED_BLUE2 = PIN_PE3;
const byte LED_GREEN1 = PIN_PD5;
const byte LED_GREEN2 = PIN_PG5; 
const byte LED_YELLOW1 = PIN_PD6;
const byte LED_YELLOW2 = PIN_PE5; 

int numberOfErrors;
//******************************************************************************************************
//                                                  Setup
//******************************************************************************************************
void setup() {
  // Initialise the Enable pins 
  pinMode(PIN_PJ2, OUTPUT);            // for the SMD board
  pinMode(PIN_PE4, OUTPUT);            // for the THT board
  digitalWrite(PIN_PJ2, LOW);          // Enable sending
  digitalWrite(PIN_PE4, LOW);          // Enable sending
  // Turn on the internal red LED, to signal we have power
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  // Initialize the green, yellow and blue LEDs, turn it on for 500 ms, followed by off 
  // We do this for the pins on both boards!
  pinMode(LED_BLUE1, OUTPUT);          // Set pins to output
  pinMode(LED_BLUE2, OUTPUT);
  pinMode(LED_GREEN1, OUTPUT);
  pinMode(LED_GREEN2, OUTPUT);
  pinMode(LED_YELLOW1, OUTPUT);
  pinMode(LED_YELLOW2, OUTPUT);
  digitalWrite(LED_BLUE1, HIGH);       // turn LEDs on
  digitalWrite(LED_BLUE2, HIGH);
  digitalWrite(LED_GREEN1, HIGH);
  digitalWrite(LED_GREEN2, HIGH);
  digitalWrite(LED_YELLOW1, HIGH);
  digitalWrite(LED_YELLOW2, HIGH);
  delay(500);                          // wait for 500 milliseconds
  digitalWrite(LED_BLUE1, LOW);        // turn LEDs off
  digitalWrite(LED_BLUE2, LOW);
  digitalWrite(LED_GREEN1, LOW);
  digitalWrite(LED_GREEN2, LOW);
  digitalWrite(LED_YELLOW1, LOW);
  digitalWrite(LED_YELLOW2, LOW); 
  // Initialise the USART. The lift decoder boards use USART 3
  Serial3.begin(BAUDRATE); 
  // Serial monitor
  Serial.begin(115200); 
  Serial.println();
  Serial.println("Test RS485 - Slave");
  numberOfErrors = 0;
}


//******************************************************************************************************
//                                               Main loop
//******************************************************************************************************
void loop() {
  if (Serial3.available()) {
  char inChar = Serial3.read();
    if (inChar == 'A') {
      digitalWrite(LED_GREEN1, !digitalRead(LED_GREEN1));
      digitalWrite(LED_GREEN2, !digitalRead(LED_GREEN2));
    }
    else {
      numberOfErrors++;
      digitalWrite(LED_YELLOW1, HIGH);
      digitalWrite(LED_YELLOW2, HIGH);
      Serial.println(inChar,HEX);
      if (numberOfErrors >= BLUE_ERRORS) {
        digitalWrite(LED_BLUE1, HIGH);
        digitalWrite(LED_BLUE2, HIGH); 
      };
    };
  };
};
