/*
Library for the current sensor ACS723LLCTR_05AB_T
Currrent measure range : -5 to 5 [A]
Sensitivity : 400 mV/A
ADC resolution : 12 bits
Communication protocol : I2C
Microcontroller used : atmega1284p
Conversion done thanks to the formula :
I [A] = ADC_Value * Resolution * 1/Sensitivity - Current offset
where :
ADC_Value is a unsigned int value from 0 to 4095.
Resolution is equal to the voltage reference (5 [V]= 5000 [mV]) divided by 2^12 (= 4096), so resolution = 5000/4096
Sensitivity is equal to 400 [mV]/ 1 [A]
Current offset is equal to 6.25 [A] because the range is from -5 to 5 A, so in real when we have 0 [A], we have 2047 from the ADC.

From a dimensional point of view it's correct :

[A] = unsigned int * ([mV]/ unsigned int) * ([A]/[mV]) - [A]
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

char currentDataToRead [analogCurrentDataSize]; //Memory buffer for I2C communication functions.
uint16_t currentData = 0; // current value from the 12  bits ADC, so on 16 bits but always 4 zeros at the beginning.
extern unsigned char TWI_buf[ TWI_BUFFER_SIZE ];    // Transceiver buffer

void currentSensorRead(void)
{
	float currentDataFloat; // Real current value from -5 to 5 [A]
	// TOGGLE_IO(PORTD,PD7); // Debugging Blinking LED
	//SET_BIT(PORTD,PD6); // Debugging Enter LED, and rising edge on the scope for trigger.
	char Display_Current [10]="         "; // String for displaying real current value on the serial terminal thanks to USART or LCD.
	char String[6]; // String for displaying ADC current value on the serial terminal thanks to USART or LCD.
	currentDataToRead[0]= ((currentSensorAddress<<1)|0); // Put sensor address in byte to send with the writing instruction(0)
	TWI_Start_Transceiver_With_Data(currentDataToRead,1); // Send Sending request to the sensor on I2C
	currentDataToRead[0]= ((currentSensorAddress<<1)|1); // Put sensor address in byte to send with the reading instruction(1)
	TWI_Start_Transceiver_With_Data(currentDataToRead,3); // Receiving 3 bytes from the sensor (Address | data_byte_1 | data_byte_2)
	while(TWI_Transceiver_Busy()) 
	{
		// Waiting the end of transmission
	}
	currentData = ((TWI_buf[1]<<8) | (TWI_buf[2])); // Compiling buffer received data in 2 bytes on a int on 16 bits.
	/*
	Usart0_Tx_String(itoa(currentData,String,10)); // USART debugging with received ADC data from the sensor
	Usart0_Tx(0x0D); // Return Carrier
	Usart0_Tx(0x0A); // New line
	*/
	
	/*
	currentDataFloat= ((float)(currentData)*5000/4096*1/400)-currentSensorOffset; // Converting received data from the sensor in real current value.
 	My_ftoa(currentDataFloat,Display_Current,2,'f'); // Converting real current value (float) into ASCII for display.
	Usart0_Tx_String(Display_Current); // USART debugging with real current data converted
	Usart0_Tx(0x0D); // Return Carrier
	Usart0_Tx(0x0A); // New line
	CLR_BIT (PORTD,PD6); // Debugging Exit LEDs
	*/
		
}