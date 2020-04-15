/*
 * FilterAlgorithm.c
 *
 *  Created on: 2019��8��3��
 *      Author: Breuning
 */

#include "FilterAlgorithm.h"
#include "usart.h"
#include "SHT30.h"


FILTER filter[10] = {0};

static uint16_t smoothaverage_lib_X(uint16_t smootnum,uint16_t *arraydata);

//--------------------------------------------------------------------------------------------
//	 @function:    �����������˲�
//--------------------------------------------------------------------------------------------
void FiltetAlgorithmforSensors(uint16_t SersorData, FILTER *Filter)
{
	uint8_t i=0;

    if(Filter->SampleCount<SAMPLE_NUMBER)
    {//���ϵ��������
		*(Filter->SampleBuff + Filter->SampleCount) = (uint16_t)SersorData;
		Filter->SampleCount++;
	}
    else
    {//�����Ѿ�������
		Filter->SampleCount = SAMPLE_NUMBER;

		for(i=0; i<SAMPLE_NUMBER-1; i++)
		{
			*(Filter->SampleBuff+i) = *(Filter->SampleBuff+i+1);   //ÿ�ν�������һ������ֵ�����������ֵ���������µķŽ�ȥ
		}
		*(Filter->SampleBuff+SAMPLE_NUMBER-1) = (uint16_t)SersorData;
	}


	if(smoothaverage_lib_X(Filter->SampleCount, Filter->SampleBuff) == false)
	{
		SersorData = SersorData;
	}
	else
	{
		SersorData = smoothaverage_lib_X(Filter->SampleCount, Filter->SampleBuff);
	}
}

//--------------------------------------------------------------------------------------------
//	 @function:    ��ֵ�˲��㷨 (��ȥ���ֵ����Сֵ��ƽ��)
//--------------------------------------------------------------------------------------------
static uint16_t smoothaverage_lib_X(uint16_t smootnum, uint16_t *arraydata)
{
    uint16_t i;
	uint16_t maxdata1,mindata2;
    unsigned long averagedata = 0;

    if(smootnum<3)
    {
    	return false;
    }

    maxdata1=*(arraydata+0);
    mindata2=*(arraydata+0);

    for(i=0; i<smootnum; i++)
    {
        if(*(arraydata+i)>maxdata1)
        {
            maxdata1=(uint16_t)*(arraydata+i);            	//�����
        }
        else
        {
            if(*(arraydata+i)<mindata2)
            {
                mindata2=(uint16_t)*(arraydata+i);          //��С��
            }
        }
        averagedata+=(unsigned long)*(arraydata+i);
    }
    averagedata-=maxdata1;
    averagedata-=mindata2;
    averagedata=averagedata/(smootnum-2);           		//��ƽ����

    return (averagedata);
}

//--------------------------------------------------------------------------------------------
//	 @function:    �޷��˲��㷨
//--------------------------------------------------------------------------------------------
void LimitBreadthFilter(uint16_t SensorData)
{

	#define A 20
	static uint16_t SensorData_Last;
	static uint32_t cnt = 0;
	if(cnt == 0)
	{
		SensorData_Last = SensorData;
	}
	else
	{
		if ( ( SensorData - SensorData_Last > A ) || ( SensorData_Last - SensorData > A ))
			SensorData = SensorData_Last;
		SensorData_Last = SensorData;
	}
	cnt++;
}

