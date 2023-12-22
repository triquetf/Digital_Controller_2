#include  "MCP23017.h"
#include "I2C_Master.h"
unsigned char Extander_buf[TWI_BUFFER_SIZE];
unsigned char registreComplet1 = 0xFF;
unsigned char registreComplet0 = 0x00;

void Init_MCP23017(){
	
	Write_MCP23017(IODIRB,registreComplet0);
	Write_MCP23017(IODIRA,registreComplet0);

	
}
void Write_MCP23017(unsigned char adressintern,unsigned char cequonveutecrire){
	Extander_buf[0]=MCP23017Write;
	Extander_buf[1]=adressintern;
	Extander_buf[2]= cequonveutecrire;
	TWI_Start_Transceiver_With_Data(Extander_buf, 3);
	
}
uint8_t Read_MCP23017(unsigned char adressintern){
	Extander_buf[0]=MCP23017Write;
	Extander_buf[1]=adressintern;
	
	TWI_Start_Transceiver_With_Data(Extander_buf, 2);
	Extander_buf[0]=MCP23017Read;
	TWI_Start_Transceiver_With_Data(Extander_buf,2);

	while ( TWI_Transceiver_Busy() );

	unsigned char cequonveutlire = Extander_buf[1];

	return cequonveutlire;

}
void LED_Manual_ON(){
	unsigned char GPIOB5On = (1<<GPIOB5);
	Write_MCP23017(GPIOB,GPIOB5On);
}
void LED_Auto_ON(){
	unsigned char GPIOB6On = (1<<GPIOB6);
	Write_MCP23017(GPIOB,GPIOB6On);

}
void LED_DataLogger_ON(){
	unsigned char GPIOB7On = (1<<GPIOB7);
	Write_MCP23017(GPIOB,GPIOB7On);
}
void Relay_1_ON(){
	unsigned char GPIOB0On = (1<<GPIOB0);
	Write_MCP23017(GPIOB,GPIOB0On);
}
void Relay_2_ON(){
	unsigned char GPIOB1On = (1<<GPIOB1);
	Write_MCP23017(GPIOB,GPIOB1On);
}


void SPI_ON(){
	unsigned char GPIOAOn = (1<<GPIOA0);
	Write_MCP23017(GPIOA,GPIOAOn);
}
void SPI_OFF(){
	unsigned char GPIOAOn = (0<<GPIOA0);
	Write_MCP23017(GPIOA,GPIOAOn);
}





void LED_Manual_OFF(){
	unsigned char GPIOB7Off = (0<<GPIOB5);
	Write_MCP23017(GPIOB,GPIOB7Off);
}
void LED_Auto_OFF(){
	unsigned char GPIOB7Off = (0<<GPIOB6);
	Write_MCP23017(GPIOB,GPIOB7Off);
}
void LED_DataLogger_OFF(){
	unsigned char GPIOB7Off = (0<<GPIOB7);
	Write_MCP23017(GPIOB,GPIOB7Off);
}
void Relay_1_OFF(){
	unsigned char GPIOB0Off = (0<<GPIOB0);
	Write_MCP23017(GPIOB,GPIOB0Off);
}
void Relay_2_OFF(){
	unsigned char GPIOB1Off = (0<<GPIOB1);
	Write_MCP23017(GPIOB,GPIOB1Off);
}
