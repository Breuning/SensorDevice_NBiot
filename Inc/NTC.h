/*
 * NTC.h
 *
 *  Created on: 2019��12��10��
 *      Author: Breuning
 */

#ifndef NTC_H_
#define NTC_H_

typedef struct
{
	float t;
	float r;
}ntcData_t;

void GetWaterTemFromNTC(void);

#endif /* NTC_H_ */
