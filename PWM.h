#ifndef PWM_H_
#define PWM_H_

//Change le dutyCycle du PWM_1B
void setDutyCycle_1B(int Duty_cycle);
//Change le dutyCycle du PWM_1A
void setDutyCycle_1A(int Duty_cycle);
//Change le dutyCycle du PWM_2A
void setDutyCycle_2B(int Duty_cycle);

void PWM_1_A_B_initOPPOSEE(unsigned char Prescaler, unsigned int Top_1);

//Initialisation du PWM_1
void PWM_1_A_B_init(unsigned char Prescaler, unsigned int Top_1);
//Initialisation du PWM_2
void PWM_2_B_init(unsigned char Prescaler, unsigned int Top_2);
//Change le dutyCycle du PWM_3A
void setDutyCycle_3A(int Duty_cycle);
//Initialisation du PWM_3
void PWM_3_A_init(unsigned char Prescaler, unsigned int Top_3);

void PWM_init(char NmbrPWM);
void PWM_1_A_init(unsigned char Prescaler, unsigned int Top_1);

void incrementSin(void);

#endif 