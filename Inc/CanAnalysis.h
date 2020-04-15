/*
 * CanAnalysis.h
 *
 *  Created on: 2019��8��13��
 *      Author: Breuning
 */

#ifndef CANANALYSIS_H_
#define CANANALYSIS_H_

/*
 *  | b |t t t t t t |i i i i i|
 *  b:������λ��1bit��=0λ��չ���ư壬=1Ϊ�������塣
 *  t:�豸���ͣ�5bit����Χ0-63��֧����չ64�ֲ�ͬ�豸��
 *  i:�豸��ţ�5bit����Χ0-31��ͬһ�������豸��֧��32�֡�
 */


#define StandardFrame 0            //��׼֡ 11λID
#define ExtendedFrame 1			   //��չ֡ 29λID

typedef enum
{
	CanBoard_TYPE_CONCROL = 0,     //���ư�
	CanBoard_TYPE_SENSOR  = 1,     //��������
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
