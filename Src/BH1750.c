/*
 * BH1750.c
 *
 *  Created on: 2019年10月15日
 *      Author: Breuning
 */

#include <string.h>
#include "BH1750.h"
#include "i2c.h"
#include "SensorTask.h"

uint8_t BH1750_PowON_Command = BH1750_PowON;
uint8_t BH1750_RSET_Command = BH1750_RSET;
uint8_t BH1750_ONE_H_Command = BH1750_ONE_H;
uint8_t BH1750_CON_H_Command = BH1750_CON_H;

uint8_t BH1750_Data_Buffer[2];

void BH1750_Init(void)
{
	HAL_I2C_Master_Transmit(&hi2c1, BH1750Addr_Write, &BH1750_PowON_Command, 1, 0xff);      //发送通电指令
	HAL_I2C_Master_Transmit(&hi2c1, BH1750Addr_Write, &BH1750_RSET_Command,  1, 0xff);      //发送复位指令

	HAL_I2C_Master_Transmit(&hi2c1, BH1750Addr_Write, &BH1750_CON_H_Command, 1, 0xff);      //发送测量模式指令, 连续高分辨率模式, 1lx, 120ms
	HAL_Delay(180); //至少延时120ms
}

void GetValidDataFromBH1750(void)
{

	HAL_I2C_Master_Receive(&hi2c1, BH1750Addr_Read, BH1750_Data_Buffer, 2, 0xff);    		//接收传感器返回的数据值放到BH1750_Data_Buffer

	if(BH1750_Data_Buffer[0] == 0 && BH1750_Data_Buffer[1] == 0)
		return;
	else
		Tick_GetSensorData = HAL_GetTick();

	Sensor_Data.Illumination = (float)((BH1750_Data_Buffer[0] << 8) + BH1750_Data_Buffer[1])/1.2;
	Sensor_Data.Illumination = (uint16_t)(Sensor_Data.Illumination*1.4);

}
