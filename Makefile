tinyamp.elf: tinyamp.c
	avr-gcc -mmcu=attiny45 -Wall -Os -o tinyamp.elf tinyamp.c

tinyamp.hex: tinyamp.elf
	avr-objcopy -j .text -j .data -O ihex tinyamp.elf tinyamp.hex

.PHONY: upload

upload: tinyamp.hex
	avrdude -p t45 -c stk500 -P /dev/ttyS0 -e -U flash:w:tinyamp.hex

clean: 
	rm tinyamp.hex tinyamp.elf
