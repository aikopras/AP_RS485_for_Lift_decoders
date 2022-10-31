# <a name="README.md"></a>AP_RS485_Lift #

This Arduino library is a wrapper around [Nick Gammon's non-blocking RS485 library](https://github.com/Sthing/Nick-Gammon-RS485/tree/master/ArduinoLibraries/RS485_non_blocking). A detailed description of that library  can be found [here](http://www.gammon.com.au/forum/?id=11428). It handles transmission of multiple bytes packets, and includes an error checking protocol.  

## Goal of this library ##
The goal of this library is to combine the RS-485 specific code for the three Lift controllers into a single library. The advantage of putting this code into a special library is that by doing so we facility code sharing between the software for the main controller, Button controller and the IR controller. So changes to the RS-485 interface need to be made at a single place only, and thereby reducing the possibility of errors.

## Installation ##
After downloading, this code should be put into your local Arduino Library folder.

## RS485 Packet structure for the lift decoders ##
The RS485 library provided by Nick Gammon sends user data "packets" of sizes between 1 and 255 bytes. Each user data packet is preceded by a Start of Text byte and appended by an End of Text byte. User data is send  in a "doubled/inverted" form, and a CRC check is added at the end of the packet to allow error detection. For details see [here](http://www.gammon.com.au/forum/?id=11428).

Data to control the lift decoder is send on top of this structure as one or two bytes of user data (see figure below).
```
+-------+-------+---------------+       +---------------+---------------+
|       |       |               |       |               |               |
+---+---+---+---+---+---+---+---+       +---+---+---+---+---+---+---+---+
   TO     FROM       COMMAND                 NUMBER           VALUE
  0..3    0..3        0..15                  0..15            0..15
```
The TO and FROM fields are two bits each to address the Main controller (Master address: 0), Button controller (1) or IR controller (2). The COMMAND field differentiates between poll messages, button messages, IR-Free messages etc. The NUMBER field identifies the button / IR-sensor to which the data in VALUE belongs. For details see [AP_RS485_Lift.h](src/AP_RS485_Lift.h)

## Required hardware ##
The software is intended for ATMega 2560 processors, such as used on Arduino MEGA 2560 boards or the [Lift decoder board](https://oshwlab.com/aikopras/support-lift-controller). In case of Arduino MEGA 2560 boards, additional RS485 to TTL (UART) converter modules are needed (costs for 3 converters less than 1 Euro).

## Dependencies ##
None. Nick Gammon's non-blocking RS485 library is already included with this software.

## References ##
- [Lift controller software](https://github.com/aikopras/Lift_Vitrine)
- [Lift controller hardware](https://oshwlab.com/aikopras/support-lift-controller)
- [Lift controller videos](https://youtube.com/playlist?list=PLT8t59Iv1uii5TKU-r4GmLXoBTifbTNtb)
