TARGET = watchdog
MMCU = attiny13

CC = avr-gcc
CFLAGS = -Os -Wall -Wall -Werror -Wswitch-default -Wno-error=uninitialized -mcall-prologues -fdata-sections -ffunction-sections -Wl,--gc-sections
SRC = watchdog.c

all: hex

hex: $(SRC) $(INC)
	$(CC) -mmcu=$(MMCU) $(CFLAGS) -o $(TARGET).bin $(SRC)
	avr-objcopy -j .text -j .data -O ihex $(TARGET).bin $(TARGET).hex
	avr-size $(TARGET).bin

asm: $(SRC) $(INC)
	$(CC) -mmcu=$(MMCU) $(CFLAGS) -S -o $(TARGET).s $<

clean:
	rm -rf $(TARGET).hex $(TARGET).bin $(TARGET).s
