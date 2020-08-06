/*
 * CanAnalysis.c
 *
 *  Created on: 2019年11月20日
 *      Author: Breuning
 */
#include <string.h>
#include "main.h"
#include "can.h"
#include "CanAnalysis.h"
#include "HardwareInit.h"
#include "SensorTask.h"

static uint16_t Get_CanSensor_FrameID(uint8_t sensorid);
static CanSensor_t Get_CanSensor_Type(uint8_t sensorid);
static void Can_TxSensorData(FloatSensorDataType type);

uint16_t CanSensor_FrameID;
uint16_t CanSensor_FrameID_Hum;
CanSensor_t  CanSensor_Type;
CanSensor_t  CanSensor_Type_Hum = 0;

BOOL CanDataSendTimerFlag = FALSE;

void CanAnalysis(void)
{
	uint32_t Tick_CanTx;
	uint32_t SensorTimeout = 1000*10;

	Tick_CanTx = HAL_GetTick();

	if(CanDataSendTimerFlag == TRUE)
	{
		if(Tick_CanTx - Tick_GetSensorData < SensorTimeout)
		{

			Get_CanSensor_FrameID(Sensor_ID);

			switch(CanSensor_Type)
			{
				case CanSensor_TYPE_TEMPERATURE:
					Can_TxSensorData(Temperature);
					break;
				case CanSensor_TYPE_PRESSURE:
					Can_TxSensorData(NegativePressure);
					break;
				case CanSensor_TYPE_GAS_CO2:
					Can_TxSensorData(CO2_Data);
					break;
				case CanSensor_TYPE_GAS_NH3:
					Can_TxSensorData(NH3_Data);
					break;
				default:
					break;
			}

			HAL_Delay(100);

			if(CanSensor_Type_Hum == CanSensor_TYPE_HUMIDITY)
			{
				Can_TxSensorData(Humidity);
			}

		}

		CanDataSendTimerFlag = FALSE;
	}
}

static void Can_TxSensorData(FloatSensorDataType type)
{
	FloatDataToArr_t CanSensorData;

	CanSensorData.FloatData = GetFloatSensorData(type);
	if(type == Humidity)
	{
		Can_TxMessage(StandardFrame, CanSensor_FrameID_Hum, 4, CanSensorData.TransformedDataBuf);
	}
	else
	{
		Can_TxMessage(StandardFrame, CanSensor_FrameID, 4, CanSensorData.TransformedDataBuf);
	}

//	HAL_GPIO_TogglePin(LED2_MSGTX_GPIO_Port, LED2_MSGTX_Pin);      //发送1包数据后LED2反转一次

	memset(CanSensorData.TransformedDataBuf, 0 , sizeof(CanSensorData.TransformedDataBuf));
}

static uint16_t Get_CanSensor_FrameID(uint8_t sensorid)
{
	CanIdConstruction_t StdCanId;

	StdCanId.CanBoardType  = CanBoard_TYPE_SENSOR;
	StdCanId.CanSensorType = Get_CanSensor_Type(sensorid);
	StdCanId.CanSensorNum  = sensorid;

	CanSensor_FrameID = (StdCanId.CanBoardType<<10) + (StdCanId.CanSensorType<<5) + StdCanId.CanSensorNum;

	if(CanSensor_Type_Hum == CanSensor_TYPE_HUMIDITY)
	{
		StdCanId.CanSensorType = CanSensor_Type_Hum;
		CanSensor_FrameID_Hum = (StdCanId.CanBoardType<<10) + (StdCanId.CanSensorType<<5) + StdCanId.CanSensorNum;
	}

	return CanSensor_FrameID;
}

static CanSensor_t Get_CanSensor_Type(uint8_t sensorid)
{
	const CanSensor_t CanSensorType[] =
	{
		0,
		CanSensor_TYPE_TEMPERATURE_HUMIDITY,
		CanSensor_TYPE_TEMPERATURE_HUMIDITY,
		CanSensor_TYPE_TEMPERATURE_HUMIDITY,
		CanSensor_TYPE_TEMPERATURE_HUMIDITY,
		CanSensor_TYPE_TEMPERATURE_HUMIDITY,
		CanSensor_TYPE_WATER_TEMPERATURE,
		CanSensor_TYPE_PRESSURE,
		CanSensor_TYPE_GAS_CO2,
		CanSensor_TYPE_GAS_NH3,
		CanSensor_TYPE_ILLUMINATION,
		CanSensor_TYPE_WINDOW_POSITION,
	};

	CanSensor_Type = CanSensorType[sensorid];

	if(CanSensor_Type == CanSensor_TYPE_TEMPERATURE_HUMIDITY)
	{
		CanSensor_Type = CanSensor_TYPE_TEMPERATURE;
		CanSensor_Type_Hum = CanSensor_TYPE_HUMIDITY;
	}


	return CanSensor_Type;
}

