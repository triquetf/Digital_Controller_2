/*
 * _24LC1025.c
 *
 * Created: 01-12-23 10:19:23
 *  Author: flori
 */ 
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include "EEPROM_24LC1025.h"
#include "I2C_Master.h"
#include "USART.h"


unsigned char EEPROM_buf[4];
extern unsigned char TWI_buf[I2C_BUFFER_SIZE];

// READ in first bloc
unsigned char Eeprom_Read_0(uint16_t address) {
	EEPROM_buf[0] = ((EEPROM_ADDRESS_0<<1)|0);		//Salve Address + W
	EEPROM_buf[1]=(unsigned char)((address >> 8) & 0xFF);
	EEPROM_buf[2]=(unsigned char)(address & 0xFF);
	TWI_Start_Transceiver_With_Data(EEPROM_buf, 3);
	EEPROM_buf[0] = ((EEPROM_ADDRESS_0<<1)|1);		//Salve Address + R
	TWI_Start_Transceiver_With_Data(EEPROM_buf, 2);
	while ( TWI_Transceiver_Busy() );
	// Wait until TWI is ready for next transmission.
	// FOR DEBUG
	//Usart0_Tx(TWI_buf[1]);
	//Usart0_Tx_String("inside read");

	return TWI_buf[1];
}
// READ in second bloc
unsigned char Eeprom_Read_1(uint16_t address) {
	EEPROM_buf[0] = ((EEPROM_ADDRESS_1<<1)|0);		//Salve Address + W
	EEPROM_buf[1]=(unsigned char)((address >> 8) & 0xFF);
	EEPROM_buf[2]=(unsigned char)(address & 0xFF);
	TWI_Start_Transceiver_With_Data(EEPROM_buf, 3);
	EEPROM_buf[0] = ((EEPROM_ADDRESS_1<<1)|1);		//Salve Address + R
	TWI_Start_Transceiver_With_Data(EEPROM_buf, 2);
	while ( TWI_Transceiver_Busy() );
	// Wait until TWI is ready for next transmission.
	// FOR DEBUG
	//Usart0_Tx(TWI_buf[1]);
	//Usart0_Tx_String("inside read");

	return TWI_buf[1];
}
// WRITE in the first bloc
void EEPROM_Write_0(unsigned int ad,unsigned int nbr_byte,char msg[]){
	int buffersize=nbr_byte+3;
	unsigned char EEPROM_BUFF_2[buffersize];
	EEPROM_BUFF_2[0]=((EEPROM_ADDRESS_0<<1)|0);
	EEPROM_BUFF_2[1]=(unsigned char)((ad >> 8) & 0xFF);
	EEPROM_BUFF_2[2]=(unsigned char)(ad & 0xFF);
	for(int i = 0;i<nbr_byte;i++){
		EEPROM_BUFF_2[i+3]=msg[i];
	}
	
	TWI_Start_Transceiver_With_Data(EEPROM_BUFF_2,buffersize);
}
// WRITE in the second bloc
void EEPROM_Write_1(unsigned int ad,unsigned int nbr_byte,char msg[]){
	int buffersize=nbr_byte+3;
	unsigned char EEPROM_BUFF_2[buffersize];
	EEPROM_BUFF_2[0]=((EEPROM_ADDRESS_0<<1)|0);
	EEPROM_BUFF_2[1]=(unsigned char)((ad >> 8) & 0xFF);
	EEPROM_BUFF_2[2]=(unsigned char)(ad & 0xFF);
	for(int i = 0;i<nbr_byte;i++){
		EEPROM_BUFF_2[i+3]=msg[i];
	}
	
	TWI_Start_Transceiver_With_Data(EEPROM_BUFF_2,buffersize);
}
// WRITE only in the first bloc of the eeprom
void EEPROM_Write_String( char *word, unsigned int ad) {
	unsigned int nbr_byte = strlen(word);
	Usart0_Tx(nbr_byte);
	//word[2] = 10;
	EEPROM_Write_0(ad, nbr_byte, word);
}

void EEPROM_Write_int(unsigned int ad, unsigned int nbr_int, int msg[]) {
	char buffersize = nbr_int * 2 + 3; // Chaque entier prend 2 octets dans la mémoire EEPROM
	unsigned char EEPROM_BUFF_2[buffersize];
	EEPROM_BUFF_2[0] = ((EEPROM_ADDRESS_0 << 1) | 0);
	EEPROM_BUFF_2[1] = (unsigned char)((ad >> 8) & 0xFF);
	EEPROM_BUFF_2[2] = (unsigned char)(ad & 0xFF);

	for (int i = 0; i < nbr_int; i++) {
		EEPROM_BUFF_2[i * 2 + 3] = (unsigned char)((msg[i] >> 8) & 0xFF); // Octet de poids fort
		EEPROM_BUFF_2[i * 2 + 4] = (unsigned char)(msg[i] & 0xFF);        // Octet de poids faible
	}

	TWI_Start_Transceiver_With_Data(EEPROM_BUFF_2, buffersize);
}
