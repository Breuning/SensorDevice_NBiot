/*
 * WaterMeter.h
 *
 *  Created on: 2020Äê3ÔÂ31ÈÕ
 *      Author: Breuning
 */

#ifndef WATERMETER_H_
#define WATERMETER_H_

#include "stm32f1xx_hal.h"

typedef enum
{
	RECEIVE = 0,
    TRANSMIT = 1

}RS485_STATE;

void GetValidDataFromWaterMet(void);
unsigned BCDtoDecimal (unsigned bVal);
uint16_t CRC16(uint8_t *puchMsg, uint8_t usDataLen);
void RS485Uart_RX_TX_Switch(RS485_STATE RS485state);



#endif /* WATERMETER_H_ */
