# SR1612Z1

Platform-agnostic library for the SR1612Z1 GNSS module.

## Notes on usage

* 3.3V power supply is supported.
    * Datasheet for *SR1612Z1* requires 3.3V power supply.
    * Meanwhile module description on Aliexpress (*QUESCAN GNSS GPS GLONASS BDS*) suggests using 5V power supply as *typical*.
* UART baud rate is 38400.
* Default GNSS module operation mode is combined GNSS (GPS, GLONASS and Beidou).
* Protocol is NMEA0183.
* Supported NMEA messages are *GGA*, *GLL*, *GSA*, *GSV*, *RMC* and *VTG*. Also, *TXT* message is transmitted.
* Module configuration is performed through UART with NMEA-like messages.
* PPS signal is connected to GPIO.

## Author

Yuriy Kapoyko ykapoyko@vk.com
