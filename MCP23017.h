#ifndef _MCP23017_h_
#define _MCP23017_h_

#include <stdint.h>

#define MCP23017Write		0b01000000
#define MCP23017Read		0b01000001
#define IODIRA				0x00
#define IPOLA				0X02
#define GPINTENA			0x04
#define DEFVALA				0x06
#define INTCONA				0x08
#define IOCON				0x0A
#define GPPUA				0x0C
#define INTFA				0x0E
#define	INTCAPA				0x10
#define GPIOA				0x12
#define OLATA				0x14

#define IODIRB				0x01
#define IPOLB				0X03
#define GPINTENB			0x05
#define DEFVALB				0x07
#define INTCONB				0x09
#define GPPUB				0x0D
#define INTFB				0x0F
#define	INTCAPB				0x11
#define GPIOB				0x13
#define OLATB				0x15

// Bits de IOCON REGISTER

#define BANK				7
#define MIRROR				6
#define SEQOP				5
#define DISSLW				4
#define HAEN				3
#define ODR					2
#define INTPOL				1

// Bits de GPIO

#define GPIOA0				0
#define GPIOB0				0
#define GPIOA1				1
#define GPIOB1				1
#define GPIOA2				2
#define GPIOB2				2
#define GPIOA3				3
#define GPIOB3				3
#define GPIOA4				4
#define GPIOB4				4
#define GPIOA5				5
#define GPIOB5				5
#define GPIOA6				6
#define GPIOB6				6
#define GPIOA7				7
#define GPIOB7				7


// fonction publique
void Init_MCP23017();
void LED_Manual_ON();
void LED_Auto_ON();
void LED_DataLogger_ON();
void LED_Manual_OFF();
void LED_Auto_OFF();
void LED_DataLogger_OFF();
void Relay_1_OFF();
void Relay_2_OFF();
void Relay_1_ON();
void Relay_2_ON();
void SPI_OFF();
void SPI_ON();

//Fonction privée
void Write_MCP23017(unsigned char adressintern,unsigned char cequonveutecrire);
uint8_t Read_MCP23017(unsigned char adressintern);

#endif