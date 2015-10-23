blink.elf: blink.c
	avr-gcc -mmcu=attiny45 -Wall -Os -o blink.elf blink.c

blink.hex: blink.elf
	avr-objcopy -j .text -j .data -O ihex blink.elf blink.hex

.PHONY: upload

upload: blink.hex
	avrdude -p t45 -c stk500 -P /dev/ttyS0 -e -U flash:w:blink.hex

clean: 
	rm blink.hex blink.elf
