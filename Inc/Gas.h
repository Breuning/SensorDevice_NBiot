/*
 * Gas.h
 *
 *  Created on: 2019Äê10ÔÂ16ÈÕ
 *      Author: Breuning
 */

#ifndef GAS_H_
#define GAS_H_



#define RecieveDataLen_Gas  9
#define TransmitDataLen_Gas 9



void ZE03GAS_Init(void);
void MHZ14CO2_Init(void);
void GetValidDataFromZE03GAS(uint16_t type);
void GetValidDataFromMHZ14CO2(void);
uint8_t GasReceiveDataCheckSum(uint8_t *array);

extern uint8_t GasUsart_Receive[20];
extern uint8_t Gas_ReadValueCommand[TransmitDataLen_Gas];


#endif /* GAS_H_ */
