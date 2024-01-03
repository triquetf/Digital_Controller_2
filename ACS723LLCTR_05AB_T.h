/*
 * ACS723LLCTR_05AB_T.h
 *
 * Created: 17-11-23 11:24:08
 *  Author: Gebruiker
 */ 


#ifndef ACS723LLCTR_05AB_T_H_
#define ACS723LLCTR_05AB_T_H_
#define analogCurrentDataSize  3 // 1 address byte, 2 data bytes one for MSB and the other for LSB.
//#define currentSensorAddress 0x9B // 100101/1 -> (100101 <<1)|1 // Only reading data, no writable registers.
#define currentSensorAddress 0b1001101
#define currentSensorOffset 6.25
void currentSensorRead(void);



#endif /* ACS723LLCTR_05AB_T_H_ */