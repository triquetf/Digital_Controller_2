#include <avr/io.h>
#include <avr/interrupt.h>
#include "main.h"
#include "SPI.h"
#include "SRAM23LC1024.h"
#include  "MCP23017.h"

#include "lcd.h"

// ***Variables Globales***//

unsigned char junk = 0;

//***Fonctions***//



void SRAM_Init(void){
	
	SRAM_OFF;
	SPI_MasterTransmit(SRAM_INSTR_WM);
	SPI_MasterTransmit(0b01000000);
	SRAM_ON;
	
	SRAM_OFF;
	SPI_MasterTransmit(SRAM_INSTR_RM);
	SPI_MasterTransmit(0b01000000);
	SRAM_ON;
	
	return;
}





//Address is the starting address of the sram where we are going to encode data
//Data is all the data we want to write on the sram
void WRITEonSRAM(unsigned long address, unsigned char data[]){
	unsigned int taille = sizeof(data);
	for(int i = 0;i<taille;i++){
		SRAM_Write(address+i,data[i]);
	}
	return;
}


//Address is the starting address of the data we want 
//taille is the number of bytes we want to receive beginning at this address
////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char READonSRAM(unsigned long address, unsigned char taille){
	unsigned char data[taille];
	for(int i = 0;i<taille;i++){
		data[i] = SRAM_Read(address+i);
	}
	return data;
}



void SRAM_Write(unsigned long address, unsigned char data){
	SRAM_OFF;
	SPI_MasterTransmit(WRITE);
	SRAM_SPIF_CLR;
	SPI_MasterTransmit((address & 0xff0000)>>16);
	SRAM_SPIF_CLR;
	SPI_MasterTransmit((address & 0x00ff00)>>8);
	SRAM_SPIF_CLR;
	SPI_MasterTransmit(address & 0x0000ff);
	SRAM_SPIF_CLR;
	SPI_MasterTransmit(data);
	SRAM_SPIF_CLR;
	SRAM_ON;
	return;	
}


//
unsigned char SRAM_Read(unsigned long address) {
	SRAM_OFF;
	SPI_MasterTransmit(READ); // Envoie d'abord l'instruction
	SRAM_SPIF_CLR;
	SPI_MasterTransmit((address & 0xff0000)>>16);
	SRAM_SPIF_CLR;
	SPI_MasterTransmit((address & 0x00ff00)>>8);
	SRAM_SPIF_CLR;
	SPI_MasterTransmit(address & 0x0000ff);
	SRAM_SPIF_CLR;
	unsigned char out= SPI_MasterTransmit(junk);
	SRAM_SPIF_CLR;
	SRAM_ON;
	return out;
}


void DelayPointer(long int rep){
	//Theses line of code are just here to create a little delay for the pointer.
	int a;
	for (int i=0;i<rep;i++){
		a++;
		a--;
	}
}