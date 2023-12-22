/**************************************************************************************************************************
 Title	:   23LC1024 SRAM Library
 Author:    Janssen Charles et Koc Emre
 File:	    23LC1024.h
 Software:  Microchip studio 
 Target:    ATmega 1284P    
***************************************************************************************************************************/
//
#include <avr/io.h>
#include "main.h"
#include "SPI.h"
#include  "MCP23017.h"
#ifndef SRAM23LC1024_H_
#define SRAM23LC1024_H_

#define READ 3
#define WRITE 2
#define SRAM_INSTR_WM 1
#define SRAM_INSTR_RM 5


//MACRO RESET FLAG
#define SRAM_SPIF_CLR CLR_BIT(SPSR,SPIF)
#define SRAM_OFF CLR_BIT(PORTD,PORTD7)
#define SRAM_ON SET_BIT(PORTD,PORTD7)

void SRAM_Init(void);

void WRITEonSRAM(unsigned long address, unsigned char data[]);

unsigned char READonSRAM(unsigned long address, unsigned char taille);

unsigned char SRAM_Read(unsigned long address);

void SRAM_Write(unsigned long address, unsigned char data);
void DelayPointer(long int rep);

#endif 



