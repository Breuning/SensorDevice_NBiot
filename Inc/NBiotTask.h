/*
 * NBiotTask.h
 *
 *  Created on: 2020Äê3ÔÂ11ÈÕ
 *      Author: Breuning
 */

#ifndef NBIOTTASK_H_
#define NBIOTTASK_H_

#include "main.h"
#include "stm32f1xx_hal.h"

extern BOOL NBiotTaskTimerFlag;
extern uint8_t NBiotIMEI[16];
extern void NBiot_Task();
extern void NBiot_Init();


typedef enum
{
	AT_QSIMDET = 0,
	AT_QSIMSTAT,
	AT_QSIMSTATTEST,

	AT_nwscanseq,
	AT_QNWINFO,
	AT_CREG,
	AT_CREGTEST,

	AT_GSN,

	AT_QMTVERSION,
	AT_QMTPDPCID,
	AT_QMTKEEPALIVE,
	AT_QMTSESSION,
	AT_QMTTIMEOUT,

	AT_QMTOPEN,
//	AT_QMTCONN,

	AT_QMTSUB,

	AT_QMTOPENTEST,
	AT_QMTCONNTEST,

	AT_QMTPUB,



}teATCmdNum;


#endif /* NBIOTTASK_H_ */
