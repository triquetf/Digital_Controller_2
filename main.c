// INCLUDE
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>	// Manipulation de chaînes de caractères
#include <stdlib.h> // pour utiliser la fonction itoa()

#include "Main.h"
#include "OS.h"
#include "Hardware.h"
#include "USART.h"
#include "lcd.h"
#include "ADC.h"
#include "ftoa.h"
#include "PWM.h"
#include "EEPROM_24LC1025.h"
#include "DS3232.h"
#include "I2C_Master.h"
#include "ACS723LLCTR_05AB_T.h"
#include "SRAM23LC1024.h"
#include "SPI.h"

// Mes variables globales
unsigned char IDCB_Led = 0;	// Identificateur callback timer pour le clignotement de la LED	
extern unsigned char RTC_buf[ TWI_BUFFER_SIZE ];
extern unsigned char TWI_buf[I2C_BUFFER_SIZE];
//current sensor value
extern uint16_t currentData;
//sampling frequency
int Sampling_Time_var=1;

// Seconds, Minutes, Hours, Days Selection for Duration parts of the data logger.

unsigned char seconds=0; // Seconds value from 0 to 255 because unsigned char, ok because seconds is between 0 and 60.
unsigned char minutes=0; // Minutes value from 0 to 255 because unsigned char, ok because minutes is between 0 and 60.
unsigned char hours=0;   // Hours value from 0 to 255 because unsigned char, ok because hours is between 0 and 24.
unsigned char days=0;	 // Days value from 0 to 255 because unsigned char, ok because days is between 0 and 31.
uint32_t total_time=0;   // Total time for duration in seconds on 32 bits because the maximum value can be 2.768.460 [s] for 60 [s]+60 [m]+24 [h]+31 [d]
char status_date =0;     // Index to know wich value is modified

// Sensor and memory selection default value for Sensor et Data_Storage part of the data logger

unsigned char sensor_selection = SENSOR_CURRENT; // Current sensor choice by default
unsigned char memory_selection = MEMORY_EEPROM; // EEPROM memory choice by default

// EEPROM and SRAM related

int eepromAddressMemory; // Last EEPROM memory address
unsigned long sramAddressMemory; // Last EEPROM memory address
int sramcallmesure ;

// Mesure batch lock 
char First_Mesure = TRUE;
// datalogger on off
unsigned char IDCB_DATALOG_ON = 0;    
unsigned char ON = FALSE;				 // State off the datalogger
extern unsigned char TIMEOUT;			 // variable to determine whether we have entered the INT0 interrupt, which is the irruption triggered when the "alarm sounds"".
extern uint16_t  nombredinterruption;    // the number of interruptions we 
// sleep ID
unsigned char IDCB_Sleepmodeoff = 0;
unsigned char IDCB_Sleepmodeon = 0;
extern uint16_t findelamesure;



//****************** fonction principale *****************
int main (void)
{
 	//Initialisation hardware 
	Init_Hardware();
	//Initilisation LCD
	lcd_init(LCD_DISP_ON); lcd_clrscr(); lcd_puts("LCD OK !");
	
	
	//Initialisation ADC
	ADC_init();
	// Initialisation PWM
	
	SPI_MasterInit();
	SRAM_ON;
	
	// Initialisation des Callbacks
	OS_Init();
 	/*IDCB_Led = Callbacks_Record_Timer(Switch_LED, 500);*/
	
	Callbacks_Record_Usart0_RX(USART0_RX);

 	// Lancement OS (Boucle infinie)
	OS_Start();
	//N'arrive jamais ici
	return 0; 
}

//**************** Switch LED ************************************
//  Signalisation par clignotement de la LED  pendant 500 ms 
//****************************************************************
void Switch_LED(void)
{
	TOGGLE_IO(PORTD,PORTD7); //  Debug LED
}


//****************USART0_RX*************************
// Appelé quand on a reçu un message du terminal PC
//**************************************************
void USART0_RX(volatile char *Trame_USART0)
{
        
}
		
	
	//*****************************************
	//           STATE MACHINE
	//*****************************************
	
	
char Date(char input)

{

	// Arnaud et Julien

	static unsigned char First_in_Function = TRUE;

	static int Date_Jour;static int Date_Moi;static int Date_Annee;

	static char temp;

	int IndJ;  int IndMoi; int IndAn;

	char String_Jour[4];char String_Moi[4];char String_Annee[4];

	

	if (First_in_Function){

		// Initialization of date data.

		Date_Jour = RTC_Clock_Read_Byte(DATE);if (Date_Jour<10){IndJ = 1;}else{IndJ = 0;}

		Date_Moi = RTC_Clock_Read_Byte(MOIS);if (Date_Moi<10){IndMoi = 4;}else{IndMoi = 3;}

		Date_Annee = RTC_Clock_Read_Byte(ANNEE);if (Date_Annee<10){IndAn = 7;}else{IndAn = 6;}

		temp = 0;

		

		// Process to display on the LCD.

		itoa(Date_Jour, String_Jour, 10);itoa(Date_Moi, String_Moi, 10);itoa(Date_Annee, String_Annee, 10);

		

		// Display on the LCD.

		cli();lcd_gotoxy(0,1);lcd_puts("  -  -          ");sei();

		cli();lcd_gotoxy(IndJ,1);lcd_puts(String_Jour);sei();

		cli();lcd_gotoxy(IndMoi,1);lcd_puts(String_Moi);sei();

		cli();lcd_gotoxy(IndAn,1);lcd_puts(String_Annee);sei();

		//cli();lcd_gotoxy(1,1);lcd_command(LCD_ON_CURSOR);lcd_command(LCD_ON_BLINK);sei();

		First_in_Function = FALSE;

	}

	else

	{

		if (input != ENTER)

		{

			// If left or right: choose the character to set.

			switch(input){

				case LEFT :

				temp--;

				if (temp<0){temp=0;}

				break;

				case RIGHT:

				temp++;

				if (temp>2){temp=2;}

				break;

			}

			// Increment and decrement values based on the character to set.

			switch(temp){

				case 0 :

				Date_Jour = EDIT_VALUE(Date_Jour, input,1,31);

				if (Date_Jour<10){IndJ = 1;cli();lcd_gotoxy(0,1);lcd_puts(0);sei();}else{IndJ = 0;}

				itoa(Date_Jour, String_Jour, 10);

				cli();lcd_gotoxy(IndJ,1);lcd_puts(String_Jour);sei();

				break;

				case 1 :

				Date_Moi = EDIT_VALUE(Date_Moi, input,1,12);

				if (Date_Moi<10){IndMoi = 4;cli();lcd_gotoxy(3,1);lcd_puts(0);sei();}else{IndMoi = 3;}

				itoa(Date_Moi, String_Moi, 10);

				cli();lcd_gotoxy(IndMoi,1);lcd_puts(String_Moi);sei();

				break;

				case 2 :

				Date_Annee = EDIT_VALUE(Date_Annee, input,0,99);

				if (Date_Annee<10){IndAn = 7;cli();lcd_gotoxy(6,1);lcd_puts(0);sei();}else{IndAn = 6;}

				itoa(Date_Annee, String_Annee, 10);

				cli();lcd_gotoxy(IndAn,1);lcd_puts(String_Annee);sei();

				break;

			}

			

		}

		else

		{

			RTC_Clock_Set_Date(Date_Jour,Date_Moi,Date_Annee); // Set the date

			First_in_Function = TRUE;

			return ST_TXT_TIME;

		}

	}

	return ST_FCT_DATE;

}



char Time(char input)

{

	static unsigned char First_in_Function = TRUE;

	static int Sec;static int Min;static int Heure;

	static char temp;

	int Inds;  int Indmin; int Indh;

	char String_s[4];char String_min[4];char String_Heure[4];

	

	if (First_in_Function){

		// Initialization of time data.

		Sec = RTC_Clock_Read_Byte(SECONDE);if (Sec<10){Inds = 1;}else{Inds = 0;}

		Min = RTC_Clock_Read_Byte(MINUTE);if (Min<10){Indmin = 4;}else{Indmin = 3;}

		Heure = RTC_Clock_Read_Byte(HEURE);if (Heure<10){Indh = 7;}else{Indh = 6;}

		temp = 0;

		// Process to display on the LCD.

		itoa(Sec, String_s, 10);itoa(Min, String_min, 10);itoa(Heure, String_Heure, 10);

		// Display on the LCD.

		cli();lcd_gotoxy(0,1);lcd_puts("  h  m  s        ");sei();

		cli();lcd_gotoxy(Inds,1);lcd_puts(String_Heure);sei();

		cli();lcd_gotoxy(Indmin,1);lcd_puts(String_min);sei();

		cli();lcd_gotoxy(Indh,1);lcd_puts(String_s);sei();

		First_in_Function = FALSE;

	}

	else

	{

		if (input != ENTER)

		{

			// If left or right: choose the character to set.

			switch(input){

				case LEFT :

				temp--;

				if (temp<0){temp=0;}

				break;

				case RIGHT:

				temp++;

				if (temp>2){temp=2;}

				break;

			}

			// Increment and decrement values based on the character to set.

			switch(temp){

				case 2 :

				Sec= EDIT_VALUE(Sec, input,0,59);

				if (Sec<10){Inds = 7;cli();lcd_gotoxy(6,1);lcd_puts(0);sei();}else{Inds = 6;}

				itoa(Sec, String_s, 10);

				cli();lcd_gotoxy(Inds,1);lcd_puts(String_s);sei();

				break;

				case 1 :

				Min = EDIT_VALUE(Min, input,0,59);

				if (Min<10){Indmin = 4;cli();lcd_gotoxy(3,1);lcd_puts(0);sei();}else{Indmin = 3;}

				itoa(Min, String_min, 10);

				cli();lcd_gotoxy(Indmin,1);lcd_puts(String_min);sei();

				break;

				case 0 :

				Heure = EDIT_VALUE(Heure, input,0,99);

				if (Heure<10){Indh = 1;cli();lcd_gotoxy(0,1);lcd_puts(0);sei();}else{Indh = 0;}

				itoa(Heure, String_Heure, 10);

				cli();lcd_gotoxy(Indh,1);lcd_puts(String_Heure);sei();

				break;

			}

			

		}

		else

		{

			RTC_Clock_Set_Heure(Sec,Min,Heure);// Set the hour

			First_in_Function = TRUE;

			return ST_TXT_SENSOR;

		}

	}

	return ST_FCT_TIME;

}
	
	char Adc(char input)
	{
		if (ON==FALSE) // If data logger is off, sensor choice can be modified.
		{
			sensor_selection = SENSOR_ADC;
			Usart0_Tx_String("ADC chosen");
		}
		else // If data logger is on, sensor choice is locked.
		{
			Usart0_Tx_String("locked ADC");
		}
		return ST_TXT_ACTUATOR;
	}
	
	char Current(char input)
	{	
		if (ON==FALSE) // If data logger is off, sensor choice can be modified.
		{
			sensor_selection = SENSOR_CURRENT;
			Usart0_Tx_String("CURRENT sensor chosen");
		}
		else // If data logger is on, sensor choice is locked.
		{
			Usart0_Tx_String("locked CURRENT");
		}
		return ST_TXT_ACTUATOR;
	}
	
	char Thermocouple(char input)
	{
		
		//Not implemented yet on the board.
		
		return ST_TXT_ACTUATOR;
	}
	
	char Dac(char input)
	{
		// Pierre B.
		return ST_TXT_STORAGE;
	}
	
	char Relay(char input)
	{
		// Someone
		return ST_TXT_STORAGE;
	}
	
	char Pwm_1(char input)

	{

		static unsigned char First_in_Function = TRUE;

		

		if (First_in_Function){

			cli();lcd_gotoxy(0,1);lcd_puts("1 PWM set");sei();

			First_in_Function = FALSE;

			PWM_init(1);

		}

		else{

			if (input == ENTER){

				First_in_Function = TRUE;

				return ST_TXT_PWM_SETUP;

			}

		}

		return ST_FCT_PWM_1;

	}

	

	char Pwm_1_2(char input)

	{

		static unsigned char First_in_Function = TRUE;

		

		if (First_in_Function){

			cli();lcd_gotoxy(0,1);lcd_puts("2 PWM set");sei();

			First_in_Function = FALSE;

			PWM_init(2);

		}

		else{

			if (input == ENTER){

				First_in_Function = TRUE;

				return ST_TXT_PWM_SETUP;

			}

		}

		return ST_FCT_PWM_1_2;

	}

	

	char Pwm_1_2_3(char input)

	{

		static unsigned char First_in_Function = TRUE;

		

		if (First_in_Function){

			cli();lcd_gotoxy(0,1);lcd_puts("3 PWM set");sei();

			First_in_Function = FALSE;

			PWM_init(3);

		}

		else{

			if (input == ENTER){

				First_in_Function = TRUE;

				return ST_TXT_PWM_SETUP;

			}

		}

		return ST_FCT_PWM_1_2_3;

	}

	

	char Periode(char input)

	{

		static int PeriodePWM = 125;

		char String[4];

		static unsigned char First_in_Function = TRUE;

		

		if (First_in_Function){

			itoa(PeriodePWM*12.8/100, String, 10); //Changer période

			cli();lcd_gotoxy(0,1);lcd_puts("Periode:   ms");lcd_gotoxy(9,1);lcd_puts(String);sei();

			First_in_Function = FALSE;

		}

		else{

			if (input != ENTER){

				switch(input){

					case DOWN :

					PeriodePWM -= 10;

					if (PeriodePWM<0){PeriodePWM=0;}

					itoa(PeriodePWM*12.8/100, String, 10);

					cli();lcd_gotoxy(9,1);lcd_puts("   ");lcd_gotoxy(8,1);lcd_puts(String);sei();

					break;

					case UP:

					PeriodePWM +=10;

					if (PeriodePWM>255){PeriodePWM=255;}

					itoa(PeriodePWM*12.8/100, String, 10);

					cli();lcd_gotoxy(9,1);lcd_puts("   ");lcd_gotoxy(8,1);lcd_puts(String);sei();

					break;

				}

				

			}

			else

			{

				SetPWMPeriode(PeriodePWM);

				First_in_Function = TRUE;

				return ST_TXT_STORAGE;

			}

		}

		return ST_FCT_PERIODE;

	}

	

	char Resolution(char input)

	{

		// Arnaud + Julien + Emre + Charles

		//return ST_FCT_RESOLUTION;

		static unsigned char First_in_Function = TRUE;

		if (First_in_Function){

			First_in_Function = FALSE;

			cli();lcd_gotoxy(0,1);lcd_puts("PAS à FAIRE");sei();

		}

		else{

			if (input != ENTER){

			}

			else{

				First_in_Function = TRUE;

				return ST_TXT_PWM_SETUP;

			}

		}

		return ST_FCT_RESOLUTION;

	}

	

	char Dent_Scie(char input)

	{

		// Arnaud + Julien + Emre + Charles

		static unsigned char First_in_Function = TRUE;

		if (First_in_Function){

			First_in_Function = FALSE;

			cli();lcd_gotoxy(0,1);lcd_puts("Dent de scie SET");sei();

		}

		else{

			if (input != ENTER){

			}

			else{

				DentDeScie_ON(TRUE);

				First_in_Function = TRUE;

				return ST_TXT_PWM_TRIANGLE;

			}

		}

		return ST_FCT_DENT_SCIE;

	}

	

	

	char Isocele(char input)

	{

		// Arnaud + Julien + Emre + Charles

		static unsigned char First_in_Function = TRUE;

		if (First_in_Function){

			First_in_Function = FALSE;

			cli();lcd_gotoxy(0,1);lcd_puts("Isocele SET");sei();

		}

		else{

			if (input != ENTER){

			}

			else{

				DentDeScie_ON(FALSE);

				First_in_Function = TRUE;

				return ST_TXT_PWM_TRIANGLE;

			}

		}

		return ST_FCT_ISOCELE;

	}

	


	
	
	char Eeprom(char input)
	{	
		if (ON==FALSE) // If data logger is off, memory choice can be modified.
		{
			memory_selection = MEMORY_EEPROM;
			Usart0_Tx_String("EEPROM chosen");
		}
		else // If data logger is on, memory choice is locked.
		{
			Usart0_Tx_String("EEPROM locked");
		}
		return ST_TXT_MONITORING;
	}
	
	char Sram(char input)
	{	
		if(ON==FALSE) // If data logger is off, memory choice can be modified.
		{
		memory_selection = MEMORY_SRAM;
		Usart0_Tx_String("SRAM chosen");
		}
		else // If data logger is on, memory choice is locked.
		{
			Usart0_Tx_String("SRAM locked");
		}
		return ST_TXT_MONITORING;
	}
	
	char Monitoring(char input)
	{
		
		// Not implemented yet
		
		return ST_TXT_CONTRAST;
	}
	
	char Contrast(char input)
	{
		// Not implemented yet
		
		return ST_TXT_DEBUG;
	}
	
	char Debug(char input)
	{
		// Not implemented yet
		return ST_TXT_SETUP;
	}
	
	// Sampling time should be define after the duration and should be <= duration.
	char Sampling_Time(char input)
	{	
		if (ON==FALSE) // If data logger is OFF we can modify the sampling time.
	{
		char String[4]; // String to display the sampling time on the LCD screen
		static unsigned char First_in_Function = TRUE; // Flag for first entry in the function.
		
		if (First_in_Function) // If first entry, display the memorised sampling time value, at the start sampling time =  1
		{
			itoa(Sampling_Time_var, String, 10);
			cli();lcd_gotoxy(0,1);lcd_puts("Periode:    s");lcd_gotoxy(8,1);lcd_puts(String);sei();
			First_in_Function = FALSE; // First entry finished
		}
		else{
			if (input != ENTER) // If no confirmation by pushing the ENTER button from the user.
			{
				switch(input)
				{
					case LEFT : // Decrement by 10
					Sampling_Time_var =Sampling_Time_var-10;
					if (Sampling_Time_var<=0) // If sampling time <=0 put 1 by default
					{
						Sampling_Time_var=1;
					}
					itoa(Sampling_Time_var, String, 10); // Conversion from integer to ASCII.
					cli();lcd_gotoxy(8,1);lcd_puts("    ");lcd_gotoxy(8,1);lcd_puts(String);sei(); // Display on the LCD
					break;
					case DOWN : // Decrement by 1
					Sampling_Time_var =Sampling_Time_var-1;
					if (Sampling_Time_var<=0){Sampling_Time_var=1;} // If sampling time <=0 put 1 by default
					itoa(Sampling_Time_var, String, 10); // Conversion from integer to ASCII.
					cli();lcd_gotoxy(8,1);lcd_puts("    ");lcd_gotoxy(8,1);lcd_puts(String);sei(); // Display on the LCD
					break;
					case UP: // Increment by 1
					Sampling_Time_var =Sampling_Time_var+1;
					if (Sampling_Time_var>=59){Sampling_Time_var=59;} // If sampling time >=59 put 59 by default
					itoa(Sampling_Time_var, String, 10); // Conversion from integer to ASCII.
					cli();lcd_gotoxy(8,1);lcd_puts("    ");lcd_gotoxy(8,1);lcd_puts(String);sei(); // Display on the LCD
					break;
					case RIGHT: // Increment by 10
					Sampling_Time_var =Sampling_Time_var+10;
					if (Sampling_Time_var>=59){Sampling_Time_var=59;} // If sampling time >=59 put 59 by default
					itoa(Sampling_Time_var, String, 10); // Conversion from integer to ASCII.
					cli();lcd_gotoxy(8,1);lcd_puts("    ");lcd_gotoxy(8,1);lcd_puts(String);sei(); // Display on the LCD
					break;
				}
				
			}
			else // If user wants to confirm his sampling time value
			{
				if(Sampling_Time_var<=total_time) // Sampling time in the range of duration, ok set back the flag andd go next menu.
				{
					First_in_Function = TRUE;
					return ST_TXT_ON_OFF;
				}
				else // User should modify the duration.
				{
					cli();lcd_clrscr();lcd_gotoxy(0,0);lcd_puts("DURATION");sei();
					return ST_TXT_DURATION;
				}

			}
		}
		return ST_FCT_SAMPLING_TIME;
		//return ST_FCT_SAMPLING_TIME;
	} 
	else // Data logger still ON, should turn off the data logger to modify sampling time.
	{
		return ST_TXT_ON_OFF;
	}
		
		
	}
	
	char Duration(char input)
	{	
		if(ON==FALSE) // If data logger is off we can modify the duration of the measurment.
		{
				
				int IndSec;		// Index value on the LCD display for displaying seconds.
				int IndMin;		// Index value on the LCD display for displaying minutes.
				int IndHour;	// Index value on the LCD display for displaying hours.
				int IndDay;		// Index value on the LCD display for displaying days.
				char String_Second[3]; // String to display the integer converted value of seconds in ASCII.
				char String_Minute[3]; // String to display the integer converted value of minutes in ASCII.
				char String_Hour[3];   // String to display the integer converted value of hours in ASCII.
				char String_Day[3];    // String to display the integer converted value of days in ASCII.
				static unsigned char First_in_Function = TRUE; // Flag to notice the first entry in the Duration fonction.
				
				if (First_in_Function) // If first entry in the function, display the memorised duration value. If first time, duration = 0.
				{
					if (seconds<10) // If seconds value is < 10 we need only one digit to display the value so we put a 0 before.
					{
						IndSec = 1;
						cli();lcd_gotoxy(0,1);lcd_puts("0");sei();
					}else // If seconds value is > 10 we need two digits to display the value.
					{
						IndSec = 0;
					}
					if (minutes<10) // If minutes value is < 10 we need only one digit to display the value so we put a 0 before.
					{
						IndMin = 4;
						cli();lcd_gotoxy(0,3);lcd_puts("0");sei();
					}else // If minutes value is > 10 we need two digits to display the value.
					{
						IndMin = 3;
					}
					if (hours<10) // If hours value is < 10 we need only one digit to display the value so we put a 0 before.
					{
						IndHour = 7;
						cli();lcd_gotoxy(0,6);lcd_puts("0");sei();
					}else // If hours value is > 10 we need two digits to display the value.
					{
						IndHour = 6;
					}
					if (days<10) // If days value is < 10 we need only one digit to display the value so we put a 0 before.
					{
						IndDay = 10;
						cli();lcd_gotoxy(0,9);lcd_puts("0");sei();
					}else // If days value is > 10 we need two digits to display the value.
					{
						IndDay = 9;
					}
					// Conversion of the integers values of seconds minutes hours and days into ASCII for display.
					itoa(seconds, String_Second, 10);
					itoa(minutes, String_Minute, 10);
					itoa(hours, String_Hour, 10);
					itoa(days, String_Day, 10);
					// Display of all the values on the LCD screen.
					cli();lcd_gotoxy(0,1);lcd_puts("  s  m  h  d");sei();
					cli();lcd_gotoxy(IndSec,1);lcd_puts(String_Second);sei();
					cli();lcd_gotoxy(IndMin,1);lcd_puts(String_Minute);sei();
					cli();lcd_gotoxy(IndHour,1);lcd_puts(String_Hour);sei();
					cli();lcd_gotoxy(IndDay,1);lcd_puts(String_Day);sei();
					// First entry in the function finished.
					First_in_Function = FALSE;
				}
				else // Other entry in the function than the first.
				{
					if (input != ENTER) // If no confirmation from the user by pushing the ENTER button.
					{
						switch(input)
						{
							case LEFT : // Navigation from the left to the right to modify seconds minutes hours and days values.
							
							if (status_date<=0) // If we are at the second value place, we go to the days value place
							{
								status_date=3;
							}
							else // If not, we go from the left to the right to modify other values.
							{
								status_date=status_date-1;
							}
							break;
							case RIGHT: // Navigation from the right to the left to modify seconds minutes hours and days values.
							status_date=status_date+1;
							if (status_date>3) // If we are at the days value place, we go to the seconds value place
							{
								status_date=0;
							}
							break;
							case UP: // If we want to increment a value.
							if (status_date==0) // If we want to modify the seconds value
							{
								if (seconds>=0 && seconds<=59) // Check the range of the seconds value
								{
									seconds+=1;
									if (seconds<10) // One digit is needed to display.
									{
										IndSec = 1;
										cli();lcd_gotoxy(0,1);lcd_puts("0");sei();
									}else // Two digit are needed to display.
									{
										IndSec = 0;
									}
									itoa(seconds, String_Second, 10); // Conversion from integer to ASCII.
									cli();lcd_gotoxy(IndSec,1);lcd_puts(String_Second);sei(); // Display on the LCD
								}
								else // Seconds higher than 60 we put 0 and we display back.
								{
									
									seconds=0;
									First_in_Function =TRUE;
								}
								
							}
							else if (status_date==1) // If we want to modify the minutes value
							{
								if (minutes>=0 && minutes<=59) // Check the range of the minutes value
								{
									minutes+=1;
									if (minutes<10) // One digit is needed to display.
									{
										IndMin = 4;
										cli();lcd_gotoxy(3,1);lcd_puts("0");sei();
									}else // Two digit are needed to display.
									{
										IndMin = 3;
									}
									itoa(minutes, String_Minute, 10); // Conversion from integer to ASCII.
									cli();lcd_gotoxy(IndMin,1);lcd_puts(String_Minute);sei(); // Display on the LCD
								}
								else // Minutes higher than 60 we put 0 and we display back.
								{
									
									minutes=0;
									First_in_Function =TRUE;
								}
								
							}
							else if (status_date==2) // If we want to modify the hours value
							{
								if (hours>=0 && hours<=23) // Check the range of the hours value
								{
									hours+=1;
									if (hours<10) // One digit is needed to display.
									{
										IndHour = 7;
										cli();lcd_gotoxy(6,1);lcd_puts("0");sei();
									}else // Two digit are needed to display.
									{
										IndHour = 6;
									}
									itoa(hours, String_Hour, 10); // Conversion from integer to ASCII.
									cli();lcd_gotoxy(IndHour,1);lcd_puts(String_Hour);sei(); // Display on the LCD
								}
								else // Hours higher than 24 we put 0 and we display back.
								{
									
									hours=0;
									First_in_Function =TRUE;
								}
								
							}
							else // If we want to modify the days value
							{
								if (days>=0 && days<=30) // Check the range of the days value
								{
									days+=1;
									if (days<10) // One digit is needed to display.
									{
										IndDay = 10;
										cli();lcd_gotoxy(9,1);lcd_puts("0");sei();
									}else // Two digit are needed to display.
									{
										IndDay = 9;
									}
									itoa(days, String_Day, 10); // Conversion from integer to ASCII.
									cli();lcd_gotoxy(IndDay,1);lcd_puts(String_Day);sei(); // Display on the LCD
								}
								else // Days higher than 31 we put 0 and we display back.
								{
									
									days=0;
									First_in_Function =TRUE;
								}
								
							}
							break;
							case DOWN: // If we want to increment a value.
							if (status_date==0) // If we want to modify the seconds value
							{
								if (seconds==0) // If seconds value equal to 0 we put 60. Two digits needed.
								{
									seconds=60;
									IndSec = 0;
									itoa(seconds, String_Second, 10); // Conversion from integer to ASCII.
									cli();lcd_gotoxy(IndSec,1);lcd_puts(String_Second);sei(); // Display on the LCD
								}
								else // Second between 1 and 60
								{
									seconds-=1;
									if (seconds<10) // One digit is needed to display.
									{
										IndSec = 1;
										cli();lcd_gotoxy(0,1);lcd_puts("0");sei();
									}else // Two digit are needed to display.
									{
										IndSec = 0;
									}
									itoa(seconds, String_Second, 10); // Conversion from integer to ASCII.
									cli();lcd_gotoxy(IndSec,1);lcd_puts(String_Second);sei(); // Display on the LCD
								}
								
							}
							else if (status_date==1) // If we want to modify the minutes value
							{
								if (minutes==0) // If minutes value equal to 0 we put 60. Two digits needed.
								{
									minutes=60;
									IndMin = 3;
									itoa(minutes, String_Minute, 10); // Conversion from integer to ASCII.
									cli();lcd_gotoxy(IndMin,1);lcd_puts(String_Minute);sei(); // Display on the LCD
								}
								else // Minutes between 1 and 60
								{
									minutes-=1;
									if (minutes<10) // One digit is needed to display.
									{
										IndMin = 4;
										cli();lcd_gotoxy(3,1);lcd_puts("0");sei();
									}else // Two digit are needed to display.
									{
										IndMin = 3;
									}
									itoa(minutes, String_Minute, 10); // Conversion from integer to ASCII.
									cli();lcd_gotoxy(IndMin,1);lcd_puts(String_Minute);sei(); // Display on the LCD
								}
							}
							else if (status_date==2) // If we want to modify the hours value
							{
								if (hours==0) // If hours value equal to 0 we put 24. Two digits needed.
								{
									hours=24;
									IndHour = 6;
									itoa(hours, String_Hour, 10); // Conversion from integer to ASCII.
									cli();lcd_gotoxy(IndHour,1);lcd_puts(String_Hour);sei(); // Display on the LCD
								}
								else // Hours between 1 and 24
								{
									hours-=1;
									if (hours<10) // One digit is needed to display.
									{
										IndHour = 7;
										cli();lcd_gotoxy(6,1);lcd_puts("0");sei();
									}else // Two digit are needed to display.
									{
										IndHour = 6;
									}
									itoa(hours, String_Hour, 10); // Conversion from integer to ASCII.
									cli();lcd_gotoxy(IndHour,1);lcd_puts(String_Hour);sei(); // Display on the LCD
								}
							}
							else // If we want to modify the days value
							{
								if (days==0) // If days value equal to 0 we put 31. Two digits needed.
								{
									days=31;
									IndDay = 9;
									itoa(days, String_Day, 10); // Conversion from integer to ASCII.
									cli();lcd_gotoxy(IndDay,1);lcd_puts(String_Day);sei(); // Display on the LCD
								}
								else // Days between 1 and 31
								{
									days-=1;
									if (days<10) // One digit is needed to display.
									{
										IndDay = 10;
										cli();lcd_gotoxy(9,1);lcd_puts("0");sei();
									}else // Two digit are needed to display.
									{
										IndDay = 9;
									}
									itoa(days, String_Day, 10); // Conversion from integer to ASCII.
									cli();lcd_gotoxy(IndDay,1);lcd_puts(String_Day);sei(); // Display on the LCD
								}
							}
							break;
						}
						
					}
					else // User push the ENTER button, so compute the total time of duration and set back the first in function flag. We go to the next menu.
					{
						total_time=seconds+minutes*60+hours*3600+days*24*3600;
						First_in_Function = TRUE;
						return ST_TXT_ON_OFF;
					}
				}
				return ST_FCT_DURATION;
		}
		else // If data logger is on we go back to the On/Off menu to turn off the data logger in order to modify the duration value.
		{
			return ST_TXT_ON_OFF;
		}
		
	}
	
char Datalogger_On_Off(char input)
{
	if(Sampling_Time_var>total_time){
		return ST_TXT_DURATION;		
	}
	if (input != ENTER)
	{
		if (ON)
		{
			cli();lcd_gotoxy(9,1);lcd_puts("ON  ");sei();	// the LCD display which can change the ON or OFF state of the datalogger. The status will not be applied until ENTER is pressed. 
			
		}
		
		else {
			cli();lcd_gotoxy(9,1);lcd_puts("OFF");sei();
		}
		switch(input)
		{
			case LEFT :
			cli();lcd_gotoxy(9,1);lcd_puts("ON  ");sei();
			ON = TRUE;
			break;
			case RIGHT:
			cli();lcd_gotoxy(9,1);lcd_puts("OFF");sei();
			ON =FALSE;
			break;
		}

		return ST_FCT_ON_OFF;
	}
	else {
		if (ON)
		{
			First_Mesure=TRUE;                                                          //We'll initialise two callbacks, one for the alarm and one for tchek the alarme
			tableau();
			mesure();
			IDCB_DATALOG_ON = Callbacks_Record_Timer(mesure,(Sampling_Time_var*1000)); // each callback will be called every samplingtime (*100 for ms -->s conversion) 
			IDCB_Sleepmodeon = Callbacks_Record_Timer(SleepModeOn, 5);                     // check if the INT0 interrupt has been entered
			RTC_Alarm_Set_Seconde(RTC_Clock_Read_Byte(SECONDE) + Sampling_Time_var);    //Initialisation of the alarm that will sound 1 samplingtime later
			nombredinterruption = 0;     // resets the number of times the interrupt has been entered so that several measurements can be taken in succession.
		}
		else if (ON == FALSE)  // We're going to remove the callbacks for taking measurements and initialising the alarm.
		{
			Callbacks_Remove_Timer(IDCB_DATALOG_ON);  
			Callbacks_Remove_Timer(SleepModeOn);
		}
		return ST_TXT_DATA_TRANSFER;
	}
}

	void tableau(void) {
		findelamesure = total_time/Sampling_Time_var;  //We're going to find out how many sampling times there are in the duration. We're going to round the result to an integer so that it makes sense. 
	}
void SleepModeOn(void){
	if(TIMEOUT){
		TIMEOUT = FALSE;
		RTC_Alarm_Set_Seconde((RTC_Clock_Read_Byte(SECONDE) + Sampling_Time_var) % 60);  //set the new alarm to sound 1 sampling time later
		//Usart0_Tx_String("SLEEP MODE ON");
		Usart0_Tx(0X0D);
		//IDCB_Sleepmodeoff = Callbacks_Record_Timer(Sleppmodeoff, Sampling_time_var-5); //callback to activate the modesleep that was unable to function
		//SMCR |= (1 << SE);
	}
}
	void Sleppmodeoff(void){
		SMCR &= ~(1 << SE);
		//Usart0_Tx_String("Sleep mode Off");
		Usart0_Tx(0X0D);
		Callbacks_Remove_Timer(IDCB_Sleepmodeoff);  // It removes itself

	}

		void mesure (void)
	{
		//variable declared to be use in function
		char buffer[10];
		int format_int[8];
		char format_char[16];
		// set the start addresses for each memory if it's the first time mesure is called
		if (First_Mesure==TRUE)
		{
			// set the start address of each memory
			sramAddressMemory=0x0007;
			eepromAddressMemory=0x01;
			// disable the first mesure and lock all external change in the sensor and memory choice
			First_Mesure=FALSE;
			// initialize the number of acquisition for the sram part
			sramcallmesure=0;
		} 
		/*
		Usart0_Tx_String(itoa((eepromAddressMemory/16),buffer,10));
		*/
		// check which sensor is currently used
		switch(sensor_selection)
		{
			case SENSOR_CURRENT:
			// distribute the save if eeprom is selected or not by default it is
			if (memory_selection==MEMORY_EEPROM)
			{	
				// EEPROM can write up to 128 byte 
				// Save acquisition on EEPROM
				// update the I²C buffer to save time parameter of the mesure
				RTC_Clock_Read_All();
				// save each required data into the table
				format_int[0]=(char)convertBCDtoChar(DATE,TWI_buf[5]);
				format_int[1]=(char)convertBCDtoChar(MOIS,TWI_buf[6]);
				format_int[2]=(char)convertBCDtoChar(ANNEE,TWI_buf[7]);
				format_int[3]=(char)convertBCDtoChar(HEURE,TWI_buf[3]);
				format_int[4]=(char) convertBCDtoChar(MINUTE,TWI_buf[2]);
				format_int[5]=(char)convertBCDtoChar(SECONDE,TWI_buf[1]);
				// Update the global variable of current data and identify the sensor used for the sample
				currentSensorRead();
				// identify which sensor is used (not required but it's considered in the transfer part)
				format_int[6]="current";
				// save the current value
				format_int[7]=currentData;
				// call the EEPROM to save the table
				EEPROM_Write_int(eepromAddressMemory,8,format_int);
				// increase the address of the eeprom bu 16 as we split each integer as 2 char
				eepromAddressMemory = eepromAddressMemory+16;
				
				/*
				Usart0_Tx_String("CURRENT_EEPROM");
				Usart0_Tx(0x0D);
				Usart0_Tx(0X0A);
				*/
			}
			else{
				// Save acquisition on SRAM
				// Sram write only byte per byte
				// split the required integer for the number of mesure into 2 char
				format_char[0]=((sramcallmesure>> 8) & 0xFF);    // MSB
				format_char[1]=(sramcallmesure & 0xFF); // LSB
				// Update the global variable of current data and identify the sensor used for the sample
				currentSensorRead();
				// save the current value by split 
				format_char[2]=((currentData>> 8) & 0xFF);    // MSB
				format_char[3]=(currentData & 0xFF); // LSB
				// send the table into the sram function to save it
				for(int i = 0 ; i <4;i++){
					SRAM_Write(sramAddressMemory+i,format_char[i]);
				}
				// increase the sram address to be an empty one 
				sramAddressMemory=sramAddressMemory+4;
				// increase the number of mesure done
				sramcallmesure++;
				
				// Debug for the number of acquisition already done
				/*
				Usart0_Tx_String(itoa(sramcallmesure,buffer,10));
				*/
				
				// Debug of the memory selection and the sensor selection
				/*
				Usart0_Tx_String("CURRENT_SRAM");
				Usart0_Tx(0x0D);
				Usart0_Tx(0X0A);
				*/
					
			}
			break;
			// same logic  can be use but the sensor is not implemented
			case SENSOR_ADC:
			if (memory_selection==MEMORY_EEPROM){
				// Save acquisition on EEPROM
				Usart0_Tx_String("ADC_EEPROM");
				Usart0_Tx(0x0D);
				Usart0_Tx(0X0A);
			}
			else{
				// Save acquisition on SRAM
				Usart0_Tx_String("ADC_SRAM");
				Usart0_Tx(0x0D);
				Usart0_Tx(0X0A);
			}
			break;
			
			default:
			break;
		}
			
	}
	
	char Transfer_Usart(char input)
	{	
		//variable declared to be use in function
		char buffer[10];
		int j=0;
		// check which sensor is currently used
		switch(sensor_selection)
		{
			case SENSOR_CURRENT:
			// check if eeprom is selected or not by default
			if (memory_selection==MEMORY_EEPROM)
			{
				//READ ON EEPROM
				Usart0_Tx(0x0D);
				// check the last eeprom address which was used and increment
				// i until it has reached that point
				for (int i =1; i <=  (eepromAddressMemory-1);) 
				{	
					// String treatment exception  as it is already in ascii
					if ( i==13+j*16)
					{
						j++; // number of mesure done help to check the position of the ascii in EEPROM
						int value=((int)Eeprom_Read_0(0x00+i) << 8) | (int)Eeprom_Read_0(0x00+(i+1));
						Usart0_Tx_String(value);
						Usart0_Tx(0x3B);
						// Debug for the position test in the transmission
						/*
						Usart0_Tx_String("sensor");
						*/
					} 
					// check if it's the date part 
					else if((i>=1+j*16)&&(i<6+j*16))
					{
						int value=((int)Eeprom_Read_0(0x00+i) << 8) | (int)Eeprom_Read_0(0x00+(i+1));
						Usart0_Tx_String(itoa(value,buffer,10));
						
						if(i==5+16*j){ // it's the end date part
							// send  ";" in the terminal at the end of the section 
							Usart0_Tx(0x3B);
						}
						else{
							// send '/' in the terminal
							Usart0_Tx(0x2F);
						}
						
					}
					// check if it's the hour part 
					else if((i>=7+j*16)&&(i<12+j*16))
					{
						int value=((int)Eeprom_Read_0(0x00+i) << 8) | (int)Eeprom_Read_0(0x00+(i+1));
						Usart0_Tx_String(itoa(value,buffer,10));
						
						if(i==11+16*j){ // it's the end of the hour part
							// send ";" at the end of the section
							Usart0_Tx(0x3B);
						}
						else{
							// sent ':' in the terminal
							Usart0_Tx(0x3A);
						}
						
					}
					// read the sensor value in the memory
					else
					{
						// rebuild the value as an integer
						int value=((int)Eeprom_Read_0(0x00+i) << 8) | (int)Eeprom_Read_0(0x00+(i+1));
						// convert the integer into an ascii 
						Usart0_Tx_String(itoa(value,buffer,10));
						// send ";" as it's the end of the section
						Usart0_Tx(0x3B);
						// send the carriage return
						Usart0_Tx(0x0D);
					}		
					//	Increase the I by 2 as we check memory addresses two by two				
					i=i+2;	
				}
		
			}
			else{
				// READ ON SRAM
				// Carriage return
				Usart0_Tx(0x0D); 
				// check the last sram address which was used and increment
				// i until it has reached that point
				// the minus 8 is there as we read 4 integer split into 8 char 
				// avoid one more loop
				for (int i =0; i <=  (sramAddressMemory-8);)
				{	
					// start at 0x0007 and increment it 
					// rebuild the value as an integer
					int value=((int)SRAM_Read(0x0007+i) << 8) | (int)SRAM_Read(0x0007+(i+1));
					// transform the integer into ascii and send it in USART
					Usart0_Tx_String(itoa(value,buffer,10));
					// send ';'
					Usart0_Tx(0x3B);
					//	Increase the I by 2 as we check memory addresses two by two	
					i=i+2;
					// if we use 4 char to rebuild all the format used we know that the fourth one is the last
					// one for that specific mesure
					if (i%4==0)
					{
						Usart0_Tx(0x0D);
					}
						
				}
				// Debug of the memory selection and the sensor selection
				/*
				Usart0_Tx_String("CURRENT_SRAM");
				Usart0_Tx(0x0D);
				Usart0_Tx(0X0A);
				*/
			}
			break;
			// same logic  can be use but the sensor is not implemented
			case SENSOR_ADC:
			if (memory_selection==MEMORY_EEPROM){
				// READ ON EEPROM
				Usart0_Tx_String("ADC_EEPROM");
				Usart0_Tx(0x0D);
				Usart0_Tx(0X0A);
			}
			else{
				//READ ON SRAM
				Usart0_Tx_String("ADC_SRAM");
				Usart0_Tx(0x0D);
				Usart0_Tx(0X0A);
			}
			break;
			
			default:
			break;
		}
		return ST_TXT_DATALOGGER;
	}
	
	char Sampling_CTRL(char input)
	{
		// Pierre C., Arnaud D., Thibault T.,  Maxime M., Maxime P., Florent M., Lukman N., Rémi G., Charles M., Denis B.
		//return ST_FCT_SAMPLING_CTRL;
		return ST_TXT_MANUAL_MODE;
	}

	char Manual_Mode(char input)
	{
		// Pierre C., Arnaud D., Thibault T.,  Maxime M., Maxime P., Florent M., Lukman N., Rémi G., Charles M., Denis B.
		//return ST_FCT_MANUAL_MODE;
		return ST_TXT_AUTO_MODE;
	}
	
	char Auto_Mode(char input)
	{
		// Pierre C., Arnaud D., Thibault T.,  Maxime M., Maxime P., Florent M., Lukman N., Rémi G., Charles M., Denis B.
		//return ST_FCT_AUTO_MODE;
		return ST_TXT_MANUAL_MODE;
	}
	
	char Pid_P(char input)
	{
		// Lukman N. & Rémi G.
		//return ST_FCT_P;
		return ST_TXT_I;
	}
	
	char Pid_I(char input)
	{
		// Lukman N. & Rémi G
		//return ST_FCT_I;
		return ST_TXT_D;
	}
	
	
	char Pid_D(char input)
	{
		// Lukman N. & Rémi G
		//return ST_FCT_D;
		return ST_TXT_WINDUP;
	}
	
	
	char Pid_WindUp(char input)
	{
		// Lukman N. & Rémi G
		//return ST_FCT_WINDUP;
		return ST_TXT_SAMPLING_CTRL;
	}
	
	char Recursion(char input)
	{
		// Pierre C., Arnaud D., Thibault T.,  Maxime M., Maxime P., Florent M., Charles M., Denis B.
		// Download fichier CSV commun !
		return ST_TXT_SAMPLING_CTRL;
	}
	
	char Smith(char input)
	{
		// Charles M. & Denis B.
		// Download fichier CSV commun !
		return ST_TXT_SAMPLING_CTRL;
	}
	
	char Internal_Model(char input)
	{
		// Maxime P. & Florent M.
		// Download fichier CSV commun !
		return ST_TXT_MODEL_SETTING;
	}
	
	char Internal_Model_Setting(char input)
	{
		// Maxime P. & Florent M.
		// Download fichier CSV commun !
		//return ST_FCT_MODEL_SETTING;
		return ST_TXT_SAMPLING_CTRL;
	}
	
	char Rst(char input)
	{
		// Thibault T. & Maxime M.
		// Download fichier CSV commun !
		return ST_TXT_SAMPLING_CTRL;
	}
	
	char Pwm_Manual(char input)

	{

		// Arnaud + Julien + Emre + Charles

		//return ST_FCT_MODE_SINUS;

		static unsigned char First_in_Function = TRUE;

		static int DutyCycle = 50;

		char String[4];

		if (First_in_Function){

			First_in_Function = FALSE;

			itoa(DutyCycle, String, 10);

			cli();lcd_gotoxy(0,1);lcd_puts("Duty cycle: ");sei();

			cli();lcd_gotoxy(11,1);lcd_puts(String);sei();

			PWM_ManualMode_Initialisation();

			PWM_ManualMode_DutyCycle(DutyCycle);

		}

		else{

			if (input != ENTER){

				switch(input){

					case DOWN:

					DutyCycle = EDIT_VALUE(DutyCycle, DOWN, 0, 100);

					PWM_ManualMode_DutyCycle(DutyCycle);

					itoa(DutyCycle, String, 10);

					cli();lcd_gotoxy(11,1);lcd_puts("   ");lcd_gotoxy(11,1);lcd_puts(String);sei();

					break;

					case LEFT :

					DutyCycle -= 10;

					if (DutyCycle<0){DutyCycle=0;}

					PWM_ManualMode_DutyCycle(DutyCycle);

					itoa(DutyCycle, String, 10);

					cli();lcd_gotoxy(11,1);lcd_puts("    ");lcd_gotoxy(11,1);lcd_puts(String);sei();

					break;

					case UP:

					DutyCycle = EDIT_VALUE(DutyCycle, UP, 0, 100);

					PWM_ManualMode_DutyCycle(DutyCycle);

					itoa(DutyCycle, String, 10);

					cli();lcd_gotoxy(11,1);lcd_puts("   ");lcd_gotoxy(11,1);lcd_puts(String);sei();

					break;

					case RIGHT:

					DutyCycle +=10;

					if (DutyCycle>100){DutyCycle=100;}

					PWM_ManualMode_DutyCycle(DutyCycle);

					itoa(DutyCycle, String, 10);

					cli();lcd_gotoxy(11,1);lcd_puts("    ");lcd_gotoxy(11,1);lcd_puts(String);sei();

					break;

				}

			}

			else{

				First_in_Function = TRUE;

				return ST_TXT_PWM;

			}

		}

		return ST_FCT_MANUAL_MODE_PWM;

	}

	

	

	char Pwm_Sinus(char input)

	{

		// Arnaud + Julien + Emre + Charles

		//return ST_FCT_MODE_SINUS;

		static unsigned char First_in_Function = TRUE;

		if (First_in_Function){

			First_in_Function = FALSE;

		}

		else{

			if (input != ENTER){

			}

			else{

				First_in_Function = TRUE;

				return ST_TXT_PWM;

			}

		}

		return ST_FCT_MODE_SINUS;

	}

	

	char Sinus_Amplitude(char input)

	{

		static unsigned char First_in_Function = TRUE;

		if (First_in_Function){

			First_in_Function = FALSE;

			cli();lcd_gotoxy(0,1);lcd_puts("PAS à FAIRE");sei();

		}

		else{

			if (input != ENTER){

			}

			else{

				First_in_Function = TRUE;

				return ST_TXT_SINUS_PERIODE;

			}

		}

		return ST_FCT_SINUS_AMPLITUDE;

	}

	

	char Sinus_Periode(char input)

	{

		// Arnaud + Julien + Emre + Charles

		//return ST_FCT_SINUS_PERIODE;static unsigned char First_in_Function = TRUE;

		static unsigned char First_in_Function = TRUE;

		static int divFreq = 0;

		char String[4];

		static unsigned char Freq[] ={50,25,17,13,10,8,7,6,5,5};

		

		if (First_in_Function){

			First_in_Function = FALSE;

			itoa(Freq[divFreq], String, 10);

			cli();lcd_gotoxy(0,1);lcd_puts("Frequence:   Hz");lcd_gotoxy(10,1);lcd_puts(String);sei();

		}

		else{

			if (input != ENTER){

				switch (input){

					case UP :

					divFreq--;

					if (divFreq<0){divFreq =0;}

					itoa(Freq[divFreq], String, 10);

					cli();lcd_gotoxy(10,1);lcd_puts("   ");lcd_gotoxy(10,1);lcd_puts(String);sei();

					break;

					case DOWN :

					divFreq++;

					if (divFreq>8){ divFreq = 8;}

					itoa(Freq[divFreq], String, 10);

					cli();lcd_gotoxy(10,1);lcd_puts("   ");lcd_gotoxy(10,1);lcd_puts(String);sei();

				}

			}

			else{

				if (divFreq == 8){ divFreq=9;}

				Set_Perdiode_sinus(divFreq+1);

				First_in_Function = TRUE;

				return ST_TXT_ON_OFF_SINUS;

			}

		}

		return ST_FCT_SINUS_PERIODE;

	}

 char Sinus_On_Off(char input)

 {

	 // Arnaud + Julien + Emre + Charles

	 //return ST_FCT_ON_OFF_SINUS;

	 // Arnaud + Julien + Emre + Charles

	 //return ST_FCT_SINUS_PERIODE;static unsigned char First_in_Function = TRUE;

	 static unsigned char First_in_Function = TRUE;

	 static unsigned char ON;

	 

	 if (First_in_Function){

		 First_in_Function = FALSE;

		 cli();lcd_gotoxy(0,1);lcd_puts("    ");lcd_gotoxy(0,1);lcd_puts("ON");sei();

		 ON = TRUE;

	 }

	 else{

		 if (input != ENTER){

			 switch(input){

				 case LEFT:

				 if(ON == FALSE){

					 cli();lcd_gotoxy(0,1);lcd_puts("    ");lcd_gotoxy(0,1);lcd_puts("ON");sei();

					 ON = TRUE;

				 }

				 break;

				 case RIGHT:

				 if(ON == TRUE){

					 cli();lcd_gotoxy(0,1);lcd_puts("    ");lcd_gotoxy(0,1);lcd_puts("OFF");sei();

					 ON = FALSE;

				 }

				 break;

			 }

		 }

		 else{

			 First_in_Function = TRUE;

			 Sinus_ON_off(ON);

			 return ST_TXT_SINUS;

		 }

	 }

	 return ST_FCT_ON_OFF_SINUS;

 }

// This function is used to set the time and date of the DS3232.

//  It allows staying between two defined values and processing

//  them based on the button pressed.

int EDIT_VALUE(int Initial_Value, char input, int Value_Min, int Value_Max)

{

	//char String[4];

	switch (input)

	{

		case DOWN :

		{

			Initial_Value--;

			if (Initial_Value == Value_Min-1) Initial_Value = Value_Max;

			break;

		}

		case UP :

		{

			Initial_Value++;

			if (Initial_Value == Value_Max+1) Initial_Value = Value_Min;

			break;

		}

	}

	return Initial_Value;

}


	
	
	int EDIT_VALUE_0_99(int Initial_Value, char input)
	{
		char String[4];
		switch (input)
		{
			case DOWN :
			{
				Initial_Value--;
				if (Initial_Value == -1) Initial_Value = 99;
				itoa(Initial_Value, String, 10);
				//cli();lcd_gotoxy(0,1);lcd_puts("                ");lcd_gotoxy(0,1);lcd_puts(String);sei();
				break;
			}
			case UP :
			{
				Initial_Value++;
				if (Initial_Value == 100) Initial_Value = 0;
				itoa(Initial_Value, String, 10);
				//cli();lcd_gotoxy(0,1);lcd_puts("                ");lcd_gotoxy(0,1);lcd_puts(String);sei();
				break;
			}
			case LEFT :
			{
				Initial_Value = Initial_Value - 10;
				if (Initial_Value <0) Initial_Value = 0;
				itoa(Initial_Value, String, 10);
				//cli();lcd_gotoxy(0,1);lcd_puts("                ");lcd_gotoxy(0,1);lcd_puts(String);sei();
				break;
			}
			case RIGHT :
			{
				Initial_Value = Initial_Value + 10;
				if (Initial_Value > 99) Initial_Value = 99;
				itoa(Initial_Value, String, 10);
				//cli();lcd_gotoxy(0,1);lcd_puts("                ");lcd_gotoxy(0,1);lcd_puts(String);sei();
				break;
			}
		}
		return Initial_Value;
	}
	
