/* UART test program */

#include<avr/io.h>			//
#define F_CPU 8000000UL 	// default F_CPU (CLKSEL = 0001) 
#include<util/delay.h>		//

#define BAUD 9600UL			//
#include<util/setbaud.h> 	//


//	initialise UART:
//loading UBRRH, L; set by setbaud.h
//UCSRA: USART Control Register A
//UCSRB: USART Control Register B
//UCSRC: USART Control Register C
//TODO:  - interrupt implementation for UART
//
void UART_init(void)
{
	UCSRC = (0<<URSEL);		// URSEL need to be 0, when writing UBRRH
	UBRRH = UBRRH_VALUE;	// ..._VALUE defined in setbaud.h
	UBRRL = UBRRL_VALUE;	//
	UCSRA = (0<<MPCM);		//
		//MPCM   - disable multi processor comm. mode
		//U2X    - double USART speed, see table in datasheet
	#if USE_2X  // if U2X necessary
   		UCSRA |= (1<<U2X);
	#else	// else, if U2X unnecessary
   		UCSRA |= (0<<U2X);
	#endif
	UCSRB = (1<<TXEN) | (1<<RXEN) | (0<<TXCIE) | (0<<RXCIE);
		//TXEN   - enable transmitter
		//RXEN   - enable receiver
		//TXCIE  - enable transmitter interrupt
		//RXCIE  - enable receiver interrupt
	//UCSRC = (0<<UCSZ2)|(1<<UCSZ1)|(1<<UCSZ0)|(0<<USBS)|(0<<UPM1)|(0<<UPM0)|(0<<UMSEL);
	UCSRC = (1<<URSEL) | (0<<UCSZ2) | (1<<UCSZ1) | (1<<UCSZ0) | (0<<USBS) | (0<<UPM1) | (0<<UPM0) | (0<<UMSEL);  // Asynchron 8N1 
		//UCSZ2:0- character size 8 bit
		//USBS   - 1 stop bit
		//UPM1:0 - parity check disabled
		//UMSEL  - asynchronous mode
}


// -------------------------------
// UART SENDING CARACTER
//UDRE:	USART Data Register Empty
//TXC:	Transmit Complete
//
//TODO: everything	
int UART_send_c(unsigned char c)
{
	while (!(UCSRA & (1<<UDRE)))	// wait for empty transmitt data register
	{
	}
	UDR = c;
	return 0;
}

// -------------------------------
// UART SENDING STRING
//	sending a string via UART
//TODO: andere Implementierung, da Endlosschleife möglich -> FIFO-Puffer nutzen
//
int UART_send_s(char *s)
{
	while(*s)	// as long as *s != '/0' (no char in string)
	{
		UART_send_c(*s);
		s++;
	}
}

// -------------------------------
// UART RECEIVE CARACTER
//
//TODO: everything	
uint8_t UART_read_c(void)
{
	while( !(UCSRA & (1<<RXC)))
		;
	return UDR;
}

// -------------------------------
// SLEEP
void sleep(uint8_t millisec)
{
        while(millisec) {
                _delay_ms(1);  	// 1ms delay, defined in delay.h
                millisec--;		// decrement millisec
        }
}

// -------------------------------
// MAIN
// -------------------------------
int main()
{
	uint8_t c;
	
    DDRC = (1<<PC5);	// Port C 5 as output
    PORTC = (1<<PC5);	// LED off
    UART_init();		//
    UART_send_c(0x79);	//
    for(uint8_t i=0;i<=9;++i) {
    	c = i + 0x30;	// 'c' = x30
    	UART_send_c(c); // send via UART
    }
    while(1)
    {
    	c = UART_read_c();	// wait for received message
    	if(c==0x31)			// if char '1' received
    	{
    		PORTC = (0<<PC5);	// switch LED on
    	}
    	else if(c==0x30)	// if '0'
    	{
    		PORTC = (1<<PC5);	// switch LED off
    	}
    	else {				// if any other char received:
    						// toggle LED status after sleeping 250ms
    		if(PORTC & (1<<PC5)) {	
    			sleep(250);			
    			PORTC = (0<<PC5);
    		}
    		else {
    			sleep(250);
    			PORTC = (1<<PC5);
    		}
    	}
    	
    }
    return 0;	// never reached
}
