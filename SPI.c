
#include <avr/io.h>
#include <avr/interrupt.h>
#include "main.h"
#include "SPI.h"


void SPI_MasterInit(void)
{
	// Set MOSI and SCK output, all others input 
	DDR_SPI |= (1<<DD_MOSI)|(1<<DD_SCK)|(1<<DD_SS);
	SET_BIT(SS_PORT,DD_SS);//met bien le niveau haut sur le SS
	//Enable SPI, Master, set clock rate fOSC/16, mode 0,0
	//SPCR |= (1<<SPE)|(1<<MSTR)|(0<<CPOL)|(0<<CPHA)|(0<<SPR1)|(1<<SPR0);
	SET_BIT(SPCR, SPE);
	SET_BIT(SPCR, MSTR);
	//CLR_BIT(SPCR, POL);
	//CLR_BIT(SPCR, SPR1);
	SET_BIT(SPCR, SPR0);
	unsigned char clr;
	clr = SPSR;
	clr = SPDR;
	//SET_BIT(SPSR, SPI2X);
}


unsigned char SPI_MasterTransmit(unsigned char Data)
{
	SPDR = Data;
	
	//Wait until transmission complete
	while(!(SPSR & (1<<SPIF)));
	
	// Return received data
	return(SPDR);
}

