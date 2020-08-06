/*
 * Hardware_Init.c
 *
 *  Created on: 2019年10月16日
 *      Author: Breuning
 */

#include "HardwareInit.h"
#include "LoraNode.h"
#include "SHT30.h"
#include "BH1750.h"
#include "Gas.h"
#include "WaterMeter.h"
#include "Led.h"
#include "can.h"

uint8_t Sensor_ID;
uint8_t Sensor_Type;

void Hardware_Init(void)
{
	Get_SensorID();
	Get_SensorType();						//根据拨码获取传感器类型
	Sensor_Init();
	RS485_Init();
	CAN_Config_Init();
	LoraNode_Init_Mode(MODE_CMD);
	Led_Init();
    HAL_Delay(50);
}

//硬件拨码 -> 传感器类型 的映射表
void Get_SensorType(void)
{
	const uint8_t Type[] =
	{
		0,
		Temperature_Humidity_Type,			// 1  室内温湿度
		Temperature_Humidity_Type,			// 2  室内温湿度
		Temperature_Humidity_Type,			// 3  室内温湿度
		Temperature_Humidity_Type,			// 4  室内温湿度
		Outside_Temperature_Humidity_Type,	// 5  室外温湿度
		Water_Temperature_Type,				// 6  水温
		Negative_Pressure_Type,				// 7  负压
		Gas_CO2_Type,						// 8  二氧化碳
		Gas_NH3_Type,						// 9  氨气
		Illumination_Type,					//10  光照
		WindowPosition_Type,				//11  小窗推杆位移
		WindowPosition_Type,				//12  小窗推杆位移
		Gas_O2_Type,						//13 氧气
		Water_Meter_Type,                   //14 水表
		Alarm_Type,                         //15报警器
		0,
		0,
		0,
		0,
		0,
		Temperature_Humidity_Type,			//21  室内温湿度
		Temperature_Humidity_Type,			//22  室内温湿度
		Temperature_Humidity_Type,			//23  室内温湿度
		Temperature_Humidity_Type,			//24  室内温湿度
		Temperature_Humidity_Type,			//25  室内温湿度
		Temperature_Humidity_Type,			//26  室内温湿度
		Temperature_Humidity_Type,			//27  室内温湿度
		Temperature_Humidity_Type,			//28  室内温湿度
		Temperature_Humidity_Type,			//29  室内温湿度
		Temperature_Humidity_Type,			//30  室内温湿度
		Temperature_Humidity_Type,			//31  室内温湿度
		Temperature_Humidity_Type,			//32  室内温湿度
		Temperature_Humidity_Type,			//33  室内温湿度
		Temperature_Humidity_Type,			//34  室内温湿度
		Temperature_Humidity_Type,			//35  室内温湿度
		Temperature_Humidity_Type,			//36  室内温湿度
		Temperature_Humidity_Type,			//37  室内温湿度
		Temperature_Humidity_Type,			//38  室内温湿度
		Temperature_Humidity_Type,			//39  室内温湿度
		Temperature_Humidity_Type,			//40  室内温湿度

	};

	Sensor_Type = Type[Sensor_ID];
}

//获取传感器的硬件拨码
void Get_SensorID(void)
{
	Sensor_ID = 1 + (SW1 * 1  + SW2 * 2  + SW3 * 4  + SW4 * 8 + SW5 * 16 +  + SW6 * 32);
}

void Sensor_Init(void)
{
	switch (Sensor_Type)
	{
		case Outside_Temperature_Humidity_Type:
			SHT30_Init();
			break;
		case Temperature_Humidity_Type:
			SHT30_Init();
			break;
		case Gas_CO2_Type:
			MHZ14CO2_Init();
			break;
		case Gas_NH3_Type:
			ZE03GAS_Init();
			break;
		case Gas_O2_Type:
			ZE03GAS_Init();
			break;
		case Illumination_Type:
			BH1750_Init();
			break;
		default:
			break;
	}
}

void RS485_Init(void)
{
	RS485Uart_RX_TX_Switch(RECEIVE);                                 //485接收打开，发送截止
}







