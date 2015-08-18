# Solar-charger

Solar-charger mit MPP-Tracking.

Make on Linux:
make <...>
	all	- obj, elf, lst, map, hex -> ready to program to MC
	elf
	obj
	hex
	lst
	map
	
	clear	- clear makefiles
	clean	- as clear
	
	program
	programe
	allpro
	allproe

Fuses: müssen im Makefile von Hand eingefügt werden. Sind im Moment vernünftig gesetzt um UART zu nutzen.
	
TODO
- change uart.c to interrupt operation
- play around with GitHub
- finalise Makefile (all program, all programe, allpro, allproe)
