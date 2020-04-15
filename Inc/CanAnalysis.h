/*
 * CanAnalysis.h
 *
 *  Created on: 2019年8月13日
 *      Author: Breuning
 */

#ifndef CANANALYSIS_H_
#define CANANALYSIS_H_

/*
 *  | b |t t t t t t |i i i i i|
 *  b:板类型位，1bit，=0位扩展控制板，=1为传感器板。
 *  t:设备类型，5bit，范围0-63，支持扩展64种不同设备。
 *  i:设备编号，5bit，范围0-31，同一个类型设备可支持32种。
 */


#define StandardFrame 0            //标准帧 11位ID
#define ExtendedFrame 1			   //扩展帧 29位ID

typedef enum
{
	CanBoard_TYPE_CONCROL = 0,     //控制板
	CanBoard_TYPE_SENSOR  = 1,     //传感器板
}CanBoard_t;

typedef enum
{
	CanSensor_TYPE_NULL,
	CanSensor_TYPE_PRESSURE,
	CanSensor_TYPE_WINDOW_POSITION,
	CanSensor_TYPE_TEMPERATURE,
	CanSensor_TYPE_HUMIDITY,
	CanSensor_TYPE_GAS_NH3,
	CanSensor_TYPE_LUX ,
	CanSensor_TYPE_GAS_CO2,
	CanSensor_TYPE_GAS_CO,
	CanSensor_TYPE_ILLUMINATION,
	CanSensor_TYPE_WATER_METER,
	CanSensor_TYPE_WIND_DIRECTION,
	CanSensor_TYPE_WIND_SPEED,
	CanSensor_TYPE_WATER_TEMPERATURE,

	CanSensor_TYPE_TEMPERATURE_HUMIDITY,

}CanSensor_t;

typedef struct
{

	CanBoard_t   CanBoardType;
	CanSensor_t  CanSensorType;
	uint8_t      CanSensorNum;

}CanIdConstruction_t;

typedef union
{
	uint16_t             OriginId;
	CanIdConstruction_t  Resolve;
}StdIdResolve_t;

extern BOOL CanDataSendTimerFlag;
void CanAnalysis(void);

#endif /* CANANALYSIS_H_ */
