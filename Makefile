#
#	MAKEFILE fuer atmega8
#	weitere Hilfe unter:
#	http://www.mikrocontroller.net/articles/AVR-GCC-Tutorial/Exkurs_Makefiles
#	http://www.nongnu.org/avr-libc/user-manual/group__demo__project.html
#
#	Bei Fehler: "missing separator. stop."
#	http://stackoverflow.com/questions/14109724/makefile-missing-separator
#
##### AUS ZU FUELLEN	*(zwingend)
F = ledblink#*	filename
MCU_TARGET = atmega8#*	mikrocontroller
OPTIMIZE = -O2# optimierlevel -O[1,2,3]
DEFS =#noch keine Ahnung
LIBS =#noch keine Ahnung
FUSES =#noch keine Ahnung

##### VORGABEN
CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
AD = avrdude
CFLAGS = -g -Wall $(OPTIMIZE) -mmcu=$(MCU_TARGET) $(DEFS) #CFLAGS=Compile FLAGS
LDFLAGS = -Wl,-Map,$(F).map

# Regeln
#	all: 		macht alle nötigen Dateien und zusaetzliches zum Flashen
#	program: 	flasht auf MC
#	clean, clear: 	loescht alle generierten Dateien ausser die .c Dateien
#	allpro, allproe:baut Dateien und flasht ohne/mit EEPROM
##### ALL
all: obj elf lst map hex
obj:
	$(CC) -g -Os -std=gnu99 -mmcu=$(MCU_TARGET) -c $(F).c
elf:
	$(CC) -g -mmcu=$(MCU_TARGET) -o $(F).elf $(F).o
lst:
	$(OBJDUMP) -h -S $(F).elf > $(F).lst
map:
	$(CC) -g -mmcu=$(MCU_TARGET) $(LDFLAGS) -o $(F).elf $(F).o
hex:
	$(OBJCOPY) -j .text -j .data -O ihex $(F).elf $(F).hex

##### CLEAN oder CLEAR
clean:
	rm -rf *.o *.elf *.eps *.png *.pdf *.bak 
	rm -rf *.lst *.map *.hex $(EXTRA_CLEAN_FILES)
clear:
	rm -rf *.o *.elf *.eps *.png *.pdf *.bak 
	rm -rf *.lst *.map *.hex $(EXTRA_CLEAN_FILES)
##### PROGRAM
program:
	$(AD) -p m8 -c avrispmkII -P usb -U flash:w:$(F).hex:i -B100 # -U lfuse:w:0xe4:m -U hfuse:w:0xd9:m # default, but 8Mhz internal clk
# -U lfuse:w:0xe6:m -U hfuse:w:0xd9:m 

##### PROGRAM WITH EEPROM
programe:

##### MAKE AND PROGRAM without EEPROM
allpro: all program

##### MAKE AND PROGRAM with EEPROM
allproe: all programe
