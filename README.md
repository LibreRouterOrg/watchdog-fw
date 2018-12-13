# LibreRouter Watchdog code

The LibreRouter-v1 has a microcontroller, an AVR ATTiny13,
that provides hardware watchdog functionality and this is the source
code.

The watchdog implementation is very easy:
* waits for GPIO2 pin to kick (toggle it's value)
* if more than N seconds pass and GPIO2 doesn't kick then:
    * Generates a low pulse in RST_B pin (this resets the CPU and peripherals)

## Build the code

To build the source code you need:
* gcc-avr (avr-gcc in archlinux)
* avr-libc

run make and it outputs watchdog.hex

A compiled version of this code is also provided.

## Program the watchdog microcontroller

To use the LibreRouter to program the hardware watchdog you need to install
avrdude-6.1 installed in the LibreRouter. Version 6.3 of avrdude does not
work to program from the LibreRouter. You may use the avrdude_6.1-3_mips_24kc.ipk
that is in this repository in the misc directory, installing its dependencies
from the openwrt repositories.

Copy avrdude.conf to /root/.avrduderc in the LibreRouter (is important to use this name!)
This has the gpio configurations of the programmer.

Connect the GPIO pins of the LibreRouter GPIO header to the watchdog header
using jumper wires as stated in avrdude.conf:
```
  reset = GPIO12
  sck   = GPIO21
  mosi  = GPIO19
  miso  = GPIO20
```

Copy watchdog.hex to the LibreRouter /tmp directory (using scp).

Program the microcontroller
```
root@OpenWrt:~# avrdude  -p t13 -c linuxgpio  -v -U f:w:/tmp/watchdog.hex
```

The output should be something like this:

```
root@OpenWrt:~# avrdude  -p t13 -c linuxgpio  -U f:w:/tmp/watchdog.hex
avrdude: AVR device initialized and ready to accept instructions

Reading | ################################################## | 100% 0.00s

avrdude: Device signature = 0x1e9007
avrdude: NOTE: "flash" memory has been specified, an erase cycle will be performed
         To disable this feature, specify the -D option.
avrdude: erasing chip
avrdude: reading input file "/tmp/watchdog.hex"
avrdude: input file /tmp/watchdog.hex auto detected as Intel Hex
avrdude: writing flash (224 bytes):

Writing | ################################################## | 100% 0.50s

avrdude: 224 bytes of flash written
avrdude: verifying flash memory against /tmp/watchdog.hex:
avrdude: load data flash data from input file /tmp/watchdog.hex:
avrdude: input file /tmp/watchdog.hex auto detected as Intel Hex
avrdude: input file /tmp/watchdog.hex contains 224 bytes
avrdude: reading on-chip flash data:

Reading | ################################################## | 100% 0.79s

avrdude: verifying ...
avrdude: 224 bytes of flash verified

avrdude: safemode: Fuses OK (E:FF, H:FF, L:6A)

avrdude done.  Thank you.
```

Ready! make sure you have the jumper JP8 connected or soldered, this is 
the RST_B signal. If the JP8 is not connected the watchdog won't reset the 
board.


## License

This code is distributed under GPLv3 license.
