# <a name="README.md"></a>AP_RS485_Lift #

This Arduino library is a wrapper around [Nick Gammon's non-blocking RS485 library](https://github.com/Sthing/Nick-Gammon-RS485/tree/master/ArduinoLibraries/RS485_non_blocking). A detailed description of Nick Gammon's library  can be found [here](http://www.gammon.com.au/forum/?id=11428). It handles transmission of multiple bytes packets, and includes an error checking protocol.  

## Goal of this library ##
The goal of this library is to combine the RS-485 specific code for the three Lift controllers into a single library. The advantage of putting this code into a special library is that, by doing so, we facility code sharing between the software for the Main Lift controller, Button controller and the IR controller. So changes to the RS-485 interface can now be made at a single place only, and thereby reducing the chance of errors.

## Installation ##
After downloading, this code should be put into your local Arduino Library folder.

## RS485 Packet structure for the lift decoders ##
The RS485 library provided by Nick Gammon sends user data "packets" of sizes between 1 and 255 bytes. Each user data packet is preceded by a Start of Text byte and appended by an End of Text byte. User data is send  in a "doubled/inverted" form, and a CRC check is added at the end of the packet to allow error detection. For details see [here](http://www.gammon.com.au/forum/?id=11428).

Data to control the lift decoder is send on top of this structure, in the form of one or two user data bytes (see figure below).
```
+-------+-------+---------------+       +---------------+---------------+
|       |       |               |       |               |               |
+---+---+---+---+---+---+---+---+       +---+---+---+---+---+---+---+---+
   TO     FROM       COMMAND                 NUMBER           VALUE
  0..3    0..3        0..15                  0..15            0..15
```
The TO and FROM fields are two bits each, to address the Main controller (Master address: 0), Button controller (1) or IR controller (2). The COMMAND field differentiates between poll messages, button messages, IR-Free messages etc. The NUMBER field identifies the button / IR-sensor to which the data in VALUE belongs. For details see [AP_RS485_Lift.h](src/AP_RS485_Lift.h)

## Required hardware ##
The software is written for ATMega 2560 processors, such as used by the Arduino MEGA. The code is has been tested on the following two lift controller boards
- SMD board: https://oshwlab.com/aikopras/support-lift-controller
- THT board: https://oshwlab.com/aikopras/lift-decoder-arduino-mega-tht

## RS485 wiring and installation ##
General wiring and installation of RS485 is described on a [dedicate page](extras/Wiring.md). Two additional pages exists to provide the details for RS485 usage on the [SMD board](extras/SMD-board.md) and [THT board](extras/THT-board.md).

## Testing ##
By using the sketches that can be found in the examples section of this library, two different tests can be performed. The first is an initial test of the RS485 hardware and wiring; this test doesn't use anything from this library yet. After successful completion of the first test, the second test can be performed, in which the actual library's software is tested.
Both tests require (at least) two boards to be connected: one as master and the other as slave.

#### First test: Hardware and Wiring ####
- **[Master:](examples/Basic-test-Master/Basic-test-Master.ino)** At startup, the red onboard LED is switched on, to indicate there is power and the program runs. Every second, the master sends the "A" character via RS485 to the slave(s). Tests should be performed using different baudrates (see the sketch for details).
- **[Slave:](examples/Basic-test-Slaves/Basic-test-Slaves.ino)** Everytime this "A" character is received, the green LED changes between ON andf OFF. If there is an error, the yellow LED is switched ON; if there are multiple errors, also the blue LED is switched ON. It is possible to connect multiple slaves in parallel to the RS485 bus.

#### Second test: Library ####
- **[Master:](examples/Test-Master/Test-Master.ino)** at startup, the red onboard LED will be switched on. Every 100 ms the master will send a RS485 message to the slave, and switch its own red LED off. After the slave receives the RS485 message, it will immediately return a reply message. Once the reply message from the slave is received by the master, the red master LED will be switched on again. Since the reply message should be received within a few ms, the red LED seems to be continuously on, provided the RS485 interface is working well. In case of problems, the red LED will stay off, however.

- **[Slave:](examples/Test-Slave/Test-Slave.ino)** before compilation, the address of the slave must be set. The address with value 1 is for the Button Controller (the default test); value 2 is for the IR-Sensor. <br>
At startup, the slave will turn its red (onboard) LED off. Every 100ms the slave should receive from the master a RS485 message. Immediately after reception of that RS485 message, the slave will toggle its red LED. Therefore, if the RS485 interface is working well, the red LED should blink 10 times per second. In case of problems, the red LED will stay on or off, however.

## Dependencies ##
None. Nick Gammon's non-blocking RS485 library is already included with this software.

## References ##
- [Lift controller software](https://github.com/aikopras/Lift_Vitrine)
- [Lift controller hardware - SMD board](https://oshwlab.com/aikopras/support-lift-controller)
- [Lift controller hardware - THT board](https://oshwlab.com/aikopras/lift-decoder-arduino-mega-tht)
- [Lift controller videos](https://youtube.com/playlist?list=PLT8t59Iv1uii5TKU-r4GmLXoBTifbTNtb)
- [Nick Gammon's webpages on RS485 communications](http://www.gammon.com.au/forum/?id=11428)
- [Problems with cheap Chinese RS485 modules (Hackaday)](https://hackaday.io/project/179020-10mbps-over-1km-on-a-single-pair-of-wires/log/210449-resistors-for-rs485-ics-not-necessary)
