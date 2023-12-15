#include <avr/io.h>
#include <avr/interrupt.h>
#include "PWM.h"
#include "Main.h"

const unsigned char sinewave_data[]= {
	128, 134, 140, 146, 152, 158, 164, 170, 176, 182, 188, 194, 200, 206, 212, 218, //16
	224, 230, 236, 242, 248, 254, 255, 254, 248, 242, 236, 230, 224, 218, 212, 206, //16
	200, 194, 188, 182, 176, 170, 164, 158, 152, 146, 140, 134, 128, 122, 116, 110, //16
	104, 98, 92, 86, 80, 74, 68, 62, 56, 50, 44, 38, 32, 26, 20, 14, 8, 2, 1, 2, 8, //21
	14, 20, 26, 32, 38, 44, 50, 56, 62, 68, 74, 80, 86, 92, 98, 104, 110, 116, 122  //19
};
int NombrePWM = 1;
unsigned char SINUS_ON = FALSE;
int PWMPeriode = 255;
int AmplSin = 5;
unsigned char DentDeScie = TRUE;

void incrementSin(){
	if(SINUS_ON){
		static char index = 0;
		switch(NombrePWM){
			case 1 : 
				setDutyCycle_1A((sinewave_data[index%88]*AmplSin)/5);
				break;
			case 2 :
				setDutyCycle_1A((sinewave_data[index%88]*AmplSin)/5);
				setDutyCycle_1B((sinewave_data[(index+40)%88]*AmplSin)/5);
				break;
			case 3:
				setDutyCycle_1A((sinewave_data[index%88]*AmplSin)/5);
				setDutyCycle_1B((sinewave_data[(index+26)%88]*AmplSin)/5);
				setDutyCycle_2B((sinewave_data[(index+52)%88]*AmplSin)/5);
				break;
		}
		index++;
	}	
}
//Initialisation du PWM_1
void PWM_1_A_B_init(unsigned char Prescaler, unsigned int Top_1)
{
unsigned char Tampon;
	TCCR1A = 0b00000000;
	TCCR1B = 0b00000000;
	//Clear OC1A, OC1B on Compare Match, set OC1A, OC1B at BOTTOM (non-inverting mode) --> see Table 14-3
	//fast PWM --> see Table 14-5 , mode = 14
	TCCR1A  |= ((1 << COM1A1) | (1 << COM1B1) | (1 << WGM11));
	TCCR1B |=  (1 << WGM13)| (1 << WGM12);
	// Set prescaler, see Table 14-6
	Tampon = TCCR1B & 0B11111000;
	TCCR1B = Tampon | Prescaler;
	ICR1 = Top_1; //overflow tous les Top_1 ticks
	// Initialisation des valeurs de seuil à 0
	OCR1A = 0;
	OCR1B = 0;
}

void PWM_1_A_init(unsigned char Prescaler, unsigned int Top_1)
{
	unsigned char Tampon;
	//Clear OC1A, OC1B on Compare Match, set OC1A, OC1B at BOTTOM (non-inverting mode) --> see Table 14-3
	//fast PWM --> see Table 14-5 , mode = 14
	TCCR1A  = ((1 << COM1A1) | (1 << WGM11));
	TCCR1B |=  (1 << WGM13)| (1 << WGM12);
	// Set prescaler, see Table 14-6
	Tampon = TCCR1B & 0B11111000;
	TCCR1B = Tampon | Prescaler;
	ICR1 = Top_1; //overflow tous les Top_1 ticks
	// Initialisation des valeurs de seuil à 0
	OCR1A = 0;
	SET_BIT(TIMSK1,TOIE1);
}

void PWM_1_A_B_initOPPOSEE(unsigned char Prescaler, unsigned int Top_1)
{
	unsigned char Tampon;

	//Clear OC1A, OC1B on Compare Match, set OC1A, OC1B at BOTTOM (non-inverting mode) --> see Table 14-3
	//fast PWM --> see Table 14-5 , mode = 14
	TCCR1A  |= ((1 << COM1A1) | (1 << COM1B1) | (1 << WGM11)|(1 << COM1B1)|(1 << COM1B0));
	TCCR1B |=  (1 << WGM13)| (1 << WGM12);
	// Set prescaler, see Table 14-6
	Tampon = TCCR1B & 0B11111000;
	TCCR1B = Tampon | Prescaler;
	ICR1 = Top_1; //overflow tous les Top_1 ticks
	// Initialisation des valeurs de seuil à 0
	OCR1A = 0;
	OCR1B = 0;
}

//Initialisation du PWM_2
void PWM_2_B_init(unsigned char Prescaler, unsigned int Top_2)
{
	unsigned char Tampon;
	//Clear OC2A, OC2B on Compare Match, set OC2A at BOTTOM (non-inverting mode) --> see Table 15-6
	//fast PWM --> see Table 14-5 , mode = 14
	TCCR2A  |= ((1 << WGM21)| (1 << WGM20) | (1 << COM2A1) | (1 << COM2B1));
	TCCR2B |=  (1 << WGM22);
	// Set prescaler, see Table 14-6
	Tampon = TCCR2B & 0B11111000;
	TCCR2B = Tampon | Prescaler;
	OCR2A = Top_2; //overflow tous les Top_2 ticks
	// Initialisation des valeurs de seuil à 0
	OCR2B = 0;
}

//Initialisation du PWM_3
void PWM_3_A_init(unsigned char Prescaler, unsigned int Top_3)
{
	unsigned char Tampon;
	//Clear OC3A, OC3B on Compare Match, set OC3A at BOTTOM (non-inverting mode) --> see Table 14-3
	//fast PWM --> see Table 14-5 , mode = 14
	TCCR3A  |= ((1 << COM3A1) | (1 << WGM31));
	TCCR3B |=  (1 << WGM33)| (1 << WGM32);
	// Set prescaler, see Table 14-6
	Tampon = TCCR3B & 0B11111000;
	TCCR3B = Tampon | Prescaler;
	ICR3 = Top_3; //overflow tous les Top_1 ticks
	// Initialisation des valeurs de seuil à 0
	OCR3A = 0;
}

//Change le duty cycle du PWM_1A
void setDutyCycle_1A(double Duty_cycle)
{
	if((Duty_cycle < ICR1) && (Duty_cycle > 0))
	{
		OCR1A = Duty_cycle;
	}
	else if(Duty_cycle >= ICR1)
	{
		OCR1A = ICR1;
	}
	else
	{
		OCR1A = 0;
	}
}

//Change le duty cycle du PWM_1B
void setDutyCycle_1B(double Duty_cycle)
{
	if((Duty_cycle < ICR1) && (Duty_cycle >0)) 
		{
			OCR1B = Duty_cycle;
		}
	else if(Duty_cycle >= ICR1)
		{ 
			OCR1B = ICR1;
		}	
		 else 
			{
				OCR1B = 0;
			}
}

//Change le duty cycle du PWM_2B
void setDutyCycle_2B(double Duty_cycle)
{
	if((Duty_cycle < OCR2B) && (Duty_cycle > 0))
	{
		OCR2B = Duty_cycle;
	}
	else if( Duty_cycle >= OCR2B)
	{
		OCR2B = Duty_cycle;
	}
	else
	{
		OCR2B = 0;
	}
}

//Change le duty cycle du PWM_3A
void setDutyCycle_3A(int Duty_cycle)
{
	if((Duty_cycle < ICR3) && (Duty_cycle > 0))
	{
		OCR3A = Duty_cycle;
	}
	else if( Duty_cycle >= ICR3)
	{
		OCR3A = Duty_cycle;
	}
	else
	{
		OCR3A = 0;
	}
}


void PWM_init(char NmbrPWM){
	NombrePWM = NmbrPWM;
}


void PWM_ManualMode_Initialisation(void){
	if (DentDeScie)
	{
		switch(NombrePWM){
			case 1:
			PWM_1_A_init(0x5,PWMPeriode);
			break;
			case 2:
			PWM_1_A_B_init(0x5,PWMPeriode);
			break;
			case 3:
			PWM_1_A_B_init(0x5,PWMPeriode);
			PWM_2_B_init(0x7,PWMPeriode); // TOP = 255; 8 bits
			break;
		}
	}
	else{
		
	}		
}
void PWM_ManualMode_DutyCycle(int DutyCycle){
	switch(NombrePWM){
		case 1:
			setDutyCycle_1A(DutyCycle*PWMPeriode/100);
			setDutyCycle_1B(0);
			setDutyCycle_2B(0);
			break;
		case 2:
			setDutyCycle_1A(DutyCycle*PWMPeriode/100);
			setDutyCycle_1B(DutyCycle*PWMPeriode/100);
			setDutyCycle_2B(0);
			break;
		case 3:
			setDutyCycle_1A(DutyCycle*PWMPeriode/100);
			setDutyCycle_1B(DutyCycle*PWMPeriode/100);
			setDutyCycle_2B(DutyCycle*PWMPeriode/100);
			break;
	}	
}

void Sinus_ON_off(char State){
	switch(NombrePWM){
		case 1:
		PWM_1_A_init(0x2,227);
		break;
		case 2:
		PWM_1_A_B_init(0x2,227);
		break;
		case 3:
		PWM_1_A_B_init(0x2,227);
		PWM_2_B_init(0x2,227); // TOP = 255; 8 bits
		break;
	}
	SINUS_ON = State;
	if (SINUS_ON)
	{
		SET_BIT(TIMSK1,TOIE1);
	}
}

void SetPWMPeriode(int PWM){
	PWMPeriode = PWM;
}

void SetPWMAmplitude(int Amplitude){
	AmplSin = Amplitude;
}

void DentDeScie_ON(char State){
	DentDeScie = State;
}

