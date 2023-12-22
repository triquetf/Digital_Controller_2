/*
 * ACS723LLCTR_05AB_T.c
 *
 * Created: 17-11-23 11:23:50
 *  Author: Gebruiker
 */ 
#include "I2C_Master.h"
#include "USART.h"
#include "ACS723LLCTR_05AB_T.h"
#include <stdint.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Main.h"
#include "Hardware.h"
#include "ftoa.h"

char currentDataToRead [analogCurrentDataSize];
uint16_t currentData = 0;
extern unsigned char TWI_buf[ TWI_BUFFER_SIZE ];    // Transceiver buffer

void currentSensorRead(void)
{
	float currentDataFloat;
	TOGGLE_IO(PORTD,PD7);
	SET_BIT(PORTD,PD6); // Flanc montant
	char Display_Current [10]="         ";
	char String[6];
	currentDataToRead[0]= ((currentSensorAddress<<1)|0);
	TWI_Start_Transceiver_With_Data(currentDataToRead,1);
	currentDataToRead[0]= ((currentSensorAddress<<1)|1);
	TWI_Start_Transceiver_With_Data(currentDataToRead,3);
	while(TWI_Transceiver_Busy())
	{
		
	}
	currentData = ((TWI_buf[1]<<8) | (TWI_buf[2]));
	/*Usart0_Tx_String(itoa(currentData,String,10));
	Usart0_Tx(0x0D);
	Usart0_Tx(0x0A);
	*/
	/*currentDataFloat= ((float)(currentData)*5000/4096*1/400)-currentSensorOffset;
 	My_ftoa(currentDataFloat,Display_Current,2,'f');
	Usart0_Tx_String(Display_Current);
	Usart0_Tx(0x0D);
	Usart0_Tx(0x0A);
	CLR_BIT (PORTD,PD6);*/
		
}