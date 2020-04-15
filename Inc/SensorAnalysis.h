/*
 * SensorAnalysis.h
 *
 *  Created on: 2019年7月25日
 *      Author: Breuning
 */

#ifndef SENSORANALYSIS_H_
#define SENSORANALYSIS_H_

#include "stm32f1xx_hal.h"
#include "main.h"


typedef struct
{
	uint16_t Temperature_u;      //温度绝对值，无符号型，值放大10倍传输
	uint8_t  Temperature_Flag;
	int16_t  Temperature;
	uint16_t Temperature_m;      //环控养殖大师传输，若为负数在最高位加1

	uint16_t Humidity;           //值放大10倍传输
	uint16_t WaterTemperature;   //值放大10倍传输
    uint16_t NegativePressure;

    uint16_t CO2_Data;
    uint16_t NH3_Data;
    uint16_t O2_Data;

    uint16_t Illumination;
    uint16_t WindowPosition;
    float    WaterMeter;

    uint16_t WindSpeed;
    uint16_t WindDerection;

} Sensor_Data_Struct;


typedef union
{
	float    FloatData;
	uint8_t  TransformedDataBuf[4];
}FloatDataToArr_t;


typedef enum
{
	Temperature = 0,
	Humidity,
	WaterTemperature,
	NegativePressure,
	CO2_Data,
	NH3_Data,
	Illumination,
	WindDerection,
	WaterMeter,

}FloatSensorDataType;



void Get_SensorData();
void SensorAnalysis();

float GetFloatSensorData(FloatSensorDataType type);

extern BOOL SensorReadTimerFlag;
extern Sensor_Data_Struct Sensor_Data;
extern uint32_t Tick_GetSensorData;


#endif /* SENSORANALYSIS_H_ */
