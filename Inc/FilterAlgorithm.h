/*
 * FilterAlgorithm.h
 *
 *  Created on: 2019��8��3��
 *      Author: Breuning
 */

#ifndef FILTERALGORITHM_H_
#define FILTERALGORITHM_H_

#include "main.h"
#define SAMPLE_NUMBER	 10 			        //�����õ����ݸ���


typedef struct
{
	uint16_t SampleCount;
	uint16_t SampleBuff[SAMPLE_NUMBER]; 	 	//����洢���ݵ�����
}FILTER;

extern FILTER filter[10];
void FiltetAlgorithmforSensors(uint16_t SersorData, FILTER *Filter);
void LimitBreadthFilter(uint16_t SensorData);

#endif /* FILTERALGORITHM_H_ */
