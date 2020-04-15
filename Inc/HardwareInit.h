/*
 * Hardware.h
 *
 *  Created on: 2019年10月17日
 *      Author: Breuning
 */

#ifndef HARDWAREINIT_H_
#define HARDWAREINIT_H_

#include "main.h"

//定义拨码值，SW1~SW5用于传感器设备类型确定
#define  SW1  HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin)
#define  SW2  HAL_GPIO_ReadPin(SW2_GPIO_Port, SW2_Pin)
#define  SW3  HAL_GPIO_ReadPin(SW3_GPIO_Port, SW3_Pin)
#define  SW4  HAL_GPIO_ReadPin(SW4_GPIO_Port, SW4_Pin)
#define  SW5  HAL_GPIO_ReadPin(SW5_GPIO_Port, SW5_Pin)
#define  SW6  HAL_GPIO_ReadPin(SW6_GPIO_Port, SW6_Pin)
#define  SW7  HAL_GPIO_ReadPin(SW7_GPIO_Port, SW7_Pin)
#define  SW8  HAL_GPIO_ReadPin(SW8_GPIO_Port, SW8_Pin)


#define  Temperature_Type 					0xA0
#define  Humidity_Type 						0xA1
#define  Gas_NH3_Type 						0xA2
#define  Gas_CO2_Type 						0xA3
#define  Gas_CO_Type 						0xA4
#define  Illumination_Type 					0xA5
#define  Water_meter_Type 					0xA6
#define  Negative_Pressure_Type 			0xA7
#define  Wind_Direction_Type 				0xA8
#define  Wind_Speed_Type 					0xA9
#define  Outside_Temperature_Type 			0xAA
#define  Outside_Humidity_Type 				0xAB
#define  Water_Temperature_Type 			0xAC
#define  Temperature_Humidity_Type 			0xC3
#define  Outside_Temperature_Humidity_Type 	0xC4
#define  WindowPosition_Type 				0xAD
#define  Gas_O2_Type 						0xB1
#define  Water_Meter_Type                   0xB2

void Hardware_Init(void);
void Get_SensorType(void);
void Get_SensorID();
void Sensor_Init(void);
void RS485_Init(void);

extern uint8_t Sensor_Type;
extern uint8_t Sensor_ID;

#endif /* HARDWAREINIT_H_ */
