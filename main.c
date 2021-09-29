#define F_CPU 8000000UL

#include <avr/io.h>

void PORT_INIT(void);
void SPI_MASTER_INIT(void);
void SPI_SEND_STRING(char *data);

int main(void){
	DDRA = 0xFF;
	
	PORT_INIT();
	SPI_MASTER_INIT();
	
	char *transmit = "Hello there!";			///< Data to be sent.
	
	SPI_SEND_STRING(transmit);
}

/*!
 *	@brief Initialize ports.
 */

void PORT_INIT(void){
	DDRB |= (1<<PB4)|(1<<PB5)|(1<<PB7);		///< Set MOSI, SCK and SS pins to output.
	DDRB &= ~(1<<PB6);							///< Set MISO pin to input.
	PORTB |= (1<<PB4);							///< Set SS pin logic HIGH before beginning to transmit data.
}

/*!
 *	@brief Initialize SPI in master mode.
 */

void SPI_MASTER_INIT(void){
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);		///< Enable SPI, select Master SPI mode and set SCK frequency to Fosc/16.
}

/*!
 *	@brief Transmit a message over SPI.
 *	@param Data to be transmitted (char *).
 */

void SPI_SEND_STRING(char *data){
	PORTB &= ~(1<<PB4);						///< Set SS pin LOW to wake up slave device.
	for(uint8_t i=0;data[i]!=0;i++){
		SPDR = data[i];							///< Load the data byte to the SPDR shift register to transmit.
		while(!(SPSR & (1<<SPIF)));			///< Wait until the full data byte is received.
		PORTA = SPDR;							///< Display ACK byte received from slave.
	}
	PORTB |= (1<<PB4);
}