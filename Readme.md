# SR1612Z1

Platform-agnostic library for the SR1612Z1 GNSS module.

## Library usage

To use the library, just add source files *SR1612Z1.c* and *SR1612Z1.h* to target project. **Don't use** *CMakeLists.txt* file as it for testing purpose only.

## Notes on module usage

* 3.3V power supply is supported.
    * Datasheet for *SR1612Z1* requires 3.3V power supply.
    * Meanwhile module description on Aliexpress (*QUESCAN GNSS GPS GLONASS BDS*) suggests using 5V power supply as *typical*.
* Default UART baud rate is 38400.
* Default GNSS module operation mode is combined GNSS (GPS, GLONASS and Beidou).
* Protocol is NMEA0183.
* Supported NMEA messages are:
    * *GGA*, *GLL*, *GSA*, *GSV*, *RMC* and *VTG* and *TXT*, according to datasheet.
    * Some more messages are also transmitted actually.
* Module configuration is performed through UART with NMEA-like messages.
* PPS signal is connected to GPIO:
    * GPIO mode is pull-down.
    * When position is acquired, PPS signal is transmitted once per second in the form of a short pulse.

## Testing

Integrated unit-testing is implemented using *MinUnit.h* and *SR1612Z1_Test.c* files. Tests are compiled and run by *CTest* and automated using GitHub Actions.

## Author

Yuriy Kapoyko - ykapoyko@vk.com
