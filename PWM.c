#include <avr/io.h>
#include <avr/interrupt.h>
#include "PWM.h"
#include "Main.h"

// Initialization of sine values
// MatLab :
// t = linspace(0,2*pi)
// x = floor((sin(t)+1)*0.5*201)
const unsigned char sinewave_data[]= {
100,   106,   113,   119,   125,   131,   137,   143,   149,   154,
160,   165,   169,   174,   178,   182,   185,   189,   191,   194,
196,   198,   199,   200,   200,   200,   200,   199,   198,   197,
195,   193,   190,   187,   184,   180,   176,   172,   167,   162,
157,   152,   146,   140,   134,   128,   122,   116,   110,   103,
 97,    90,    84,    78,    72,    66,    60,    54,    48,    43,
 38,    33,    28,    24,    20,    16,    13,    10,     7,     5,
  3,     2,     1,     0,     0,     0,     0,     1,     2,     4,
  6,     9,    11,    15,    18,    22,    26,    31,    35,    40,
 46,    51,    57,    63,    69,    75,    81,    87,    94,   100
};

// Initialization of default values
unsigned char DentDeScie = TRUE;
int NombrePWM = 1;
int PWMPeriode = 255;
unsigned char SINUS_ON = FALSE;
int AmplSin = 1;
int DiviseurFrequence = 1;

// Initialization of PWM_1 A
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
	// Initialization of threshold values to 0
	OCR1A = 0;
	SET_BIT(TIMSK1,TOIE1);
}

// Initialization of PWM_1 A and B
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
	// Initialization of threshold values to 0
	OCR1A = 0;
	OCR1B = 0;
}

// Initialization of PWM_2
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
	// Initialization of threshold values to 0
	OCR2B = 0;
}

// Change the duty cycle of PWM_1A
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

// Change the duty cycle of PWM_1A
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

// Change the duty cycle of PWM_1A
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

void PWM_init(char NmbrPWM){
	NombrePWM = NmbrPWM;
}

void SetPWMPeriode(int PWM){
	PWMPeriode = PWM;
}

void DentDeScie_ON(char State){
	DentDeScie = State;
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

void  SetSinAmplitude(int Amplitude){
	AmplSin = Amplitude/5;
}

void Set_Perdiode_sinus(char divFreq){
	DiviseurFrequence = divFreq;
}

void Sinus_ON_off(char State){
	switch(NombrePWM){
		case 1:
		PWM_1_A_init(0x2,200);
		break;
		case 2:
		PWM_1_A_B_init(0x2,200);
		break;
		case 3:
		PWM_1_A_B_init(0x2,200);
		PWM_2_B_init(0x2,200);
		break;
	}
	SINUS_ON = State;
	if (SINUS_ON)
	{
		SET_BIT(TIMSK1,TOIE1);
	}
	else{
		CLR_BIT(TIMSK1,TOIE1);
		PWM_1_A_B_init(0x5,PWMPeriode);
		PWM_2_B_init(0x7,PWMPeriode); // TOP = 255; 8 bits
		setDutyCycle_1A(0);
		setDutyCycle_1B(0);
		setDutyCycle_2B(0);
	}
}

// This function is called every time the counter 1 reaches the top of the sawtooth wave.
// When the user sets the sine wave, this function modifies the duty cycle to generate a sine wave.
void incrementSin(){
	if(SINUS_ON){
		static int index = 0;
		static int DivFreq = 0;
		switch(NombrePWM){
			case 1 : 
				 OCR1A =(sinewave_data[index%100])*AmplSin;
				break;
			case 2 :
				OCR1A = (sinewave_data[index%100])*AmplSin;
				OCR1B = (sinewave_data[(index+50)%100])*AmplSin;
				break;
			case 3:
				OCR1A = (sinewave_data[index%100])*AmplSin;
				OCR1B = (sinewave_data[(index+33)%100])*AmplSin;
				OCR2B = (sinewave_data[(index+66)%100])*AmplSin;
				break;
		}
		DivFreq++;
		if (DivFreq == DiviseurFrequence)
		{
			index++;
			DivFreq = 0;
		}
	}	
}
