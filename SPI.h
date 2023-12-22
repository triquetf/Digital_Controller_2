
#ifndef SPI_H_
#define SPI_H_


#define DDR_SPI DDRB
#define DD_SS 4
#define DD_MOSI 5
#define DD_MISO 6
#define DD_SCK  7
#define SS_PORT PORTB


void SPI_MasterInit(void);
unsigned char SPI_MasterTransmit(unsigned char Data);

#endif /* SPI_H_ */