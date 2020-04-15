/*
 * LoraAnalysis.h
 *
 *  Created on: 2019Äê7ÔÂ25ÈÕ
 *      Author: Breuning
 */

#ifndef LORAANALYSIS_H_
#define LORAANALYSIS_H_

#include "usart.h"

typedef enum
{
    CMD_MODE,
    TRANSPARENT_MODE,
    P2P_MODE

} LORA_SATE;


void LoraAnalysis(void);
uint32_t LoraNode_SetMINIRF(uint32_t value);



#endif /* LORAANALYSIS_H_ */
