/*
 * SensorAnalysis.c
 *
 *  Created on: 2019年7月25日
 *      Author: Breuning
 */

#include "string.h"
#include "HardwareInit.h"
#include "SensorAnalysis.h"
#include "adc.h"
#include "usart.h"
#include "SHT30.h"
#include "BH1750.h"
#include "Gas.h"
#include "FilterAlgorithm.h"
#include "tim.h"
#include "WaterMeter.h"

BOOL SensorReadTimerFlag = FALSE;
Sensor_Data_Struct Sensor_Data;

uint32_t Tick_GetSensorData = 0;                  //用于判断探头获取的数据是否很久未更新

char SensorData_Buff[30];



void SensorAnalysis(void)
{

	if(SensorReadTimerFlag == TRUE)              //通过TIM5设置为每5秒读取一次传感器数值
	{
		Get_SensorData();


/**********调试用, 将获取的传感器数值输出在SensorData_Buff数组内,并将数组内的数据通过USART2发送至PC**********/

		memset(SensorData_Buff, 0 , sizeof(SensorData_Buff));

//		sprintf(SensorData_Buff, "Tem:%d Hum:%d \r", Sensor_Data.Temperature, Sensor_Data.Humidity );
//		sprintf(SensorData_Buff, "Illu: %d \r", Sensor_Data.Illumination);
//		sprintf(SensorData_Buff, "NH3: %d \r", Sensor_Data.NH3_Data);
//		sprintf(SensorData_Buff, "CO2: %d \r", Sensor_Data.CO2_Data);
//		sprintf(SensorData_Buff, "Pressure: %d \r", Sensor_Data.NegativePressure);
//		sprintf(SensorData_Buff, "Windowpos: %d \r", Sensor_Data.WindowPosition);
		sprintf(SensorData_Buff, "WaterTem: %d \r", Sensor_Data.WaterTemperature);

		//将数组内的数据通过USART2发送至PC
//		HAL_UART_Transmit(&huart2, (uint8_t *)SensorData_Buff, strlen(SensorData_Buff), 100);

/********************************************************************************************************/
		SensorReadTimerFlag = FALSE;
	}

}

void Get_SensorData(void)
{
	switch (Sensor_Type)
	{

		case Temperature_Humidity_Type:

		case Outside_Temperature_Humidity_Type:
			GetValidDataFromSHT30();
			FiltetAlgorithmforSensors(Sensor_Data.Temperature_u, &filter[0]);
			TemperatureData_NegativeValueJudge();

			FiltetAlgorithmforSensors(Sensor_Data.Humidity, &filter[1]);
			break;
		case Water_Temperature_Type:
//			if(GetWaterTemFrom18B20())
//			{
//				GetWaterTemFromNTC();
//			}
			GetWaterTemFromNTC();
			FiltetAlgorithmforSensors(Sensor_Data.WaterTemperature, &filter[2]);
			LimitBreadthFilter(Sensor_Data.WaterTemperature);
			break;
		case Negative_Pressure_Type:
			GetValidDataFromPressure();
			FiltetAlgorithmforSensors(Sensor_Data.NegativePressure, &filter[3]);
			break;
		case Gas_CO2_Type:
			GetValidDataFromMHZ14CO2();
			FiltetAlgorithmforSensors(Sensor_Data.CO2_Data, &filter[4]);
			break;
		case Gas_NH3_Type:
			GetValidDataFromZE03GAS(Sensor_Type);
			FiltetAlgorithmforSensors(Sensor_Data.NH3_Data, &filter[5]);
			break;
		case Gas_O2_Type:
			GetValidDataFromZE03GAS(Sensor_Type);
			FiltetAlgorithmforSensors(Sensor_Data.O2_Data, &filter[5]);
			break;
		case Illumination_Type:
			GetValidDataFromBH1750();
			FiltetAlgorithmforSensors(Sensor_Data.Illumination, &filter[6]);
			break;
		case WindowPosition_Type:
			GetValidDataFromWindowPos();
//			Get_WindowPos_NewPushrod();             //新小窗推杆位移
			FiltetAlgorithmforSensors(Sensor_Data.WindowPosition, &filter[7]);
			break;
		case Water_Meter_Type:
			GetValidDataFromWaterMet();
			FiltetAlgorithmforSensors((uint16_t)Sensor_Data.WaterMeter, &filter[8]);
			break;
		default:
			break;
	}
}


float GetFloatSensorData(FloatSensorDataType type)
{
	float SensorData_Float[30];

	SensorData_Float[Temperature]      = ((float)Sensor_Data.Temperature)/10.0;
	SensorData_Float[Humidity]   	   = ((float)Sensor_Data.Humidity)/10.0;
	SensorData_Float[WaterTemperature] = (float)Sensor_Data.WaterTemperature;
	SensorData_Float[NegativePressure] = (float)Sensor_Data.NegativePressure;
	SensorData_Float[CO2_Data] 		   = (float)Sensor_Data.CO2_Data;
	SensorData_Float[NH3_Data]         = (float)Sensor_Data.NH3_Data;
	SensorData_Float[Illumination]     = (float)Sensor_Data.Illumination;
	SensorData_Float[WindDerection]    = (float)Sensor_Data.WindDerection;
	SensorData_Float[WaterMeter]       = (float)Sensor_Data.WaterMeter;


	return SensorData_Float[type];
}
