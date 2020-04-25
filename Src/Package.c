/*
 * Package.c
 *
 *  Created on: 2020年2月24日
 *      Author: Breuning
 */

#include "Package.h"
#include "string.h"
#include "cJSON.h"
#include "MD5.h"
#include "usart.h"
#include "HardwareInit.h"
#include "SensorTask.h"
#include "NBiotTask.h"

uint8_t CloudPackage[100] = { 0 };

static void AddMessageToJsonObj(cJSON *root);
static void _addIntToJson(cJSON *root, const char *name, uint32_t value, bool complement);
static void _addFloatToJson(cJSON *js_list, const char *name, float value);
static void _addDataToJson(cJSON *root);
static void AddDataToJson(cJSON *root, uint8_t datatype, uint8_t order, float dataValue1, float dataValue2);
static void AddDevNumToJson(cJSON *root, const char *name, uint8_t datatype, uint8_t order);


//static void _addTimeToJson(cJSON *root, const char *name);
//static void _addMd5ToJson(cJSON *root,const char *name);


void PackageComposition(void)
{
	/*json缓冲区(发送至NB)*/
	uint8_t jsonOutPutBuf[100] = { 0 };

	/*创建JSON对象*/
	cJSON *root;
	root = cJSON_CreateObject();
	if (root == NULL)
	{
		cJSON_Delete(root);
		return;
	}

	AddMessageToJsonObj(root);

	if (cJSON_PrintPreallocated(root, (char*) jsonOutPutBuf, sizeof(jsonOutPutBuf), FALSE))
	{

		strcpy((char *)CloudPackage, (const char *)jsonOutPutBuf);
//		HAL_UART_Transmit(&huart2, (uint8_t *)jsonOutPutBuf, sizeof(jsonOutPutBuf), 1000);  	//将数组内的数据通过USART2发送至NB
//		LED1_RJ45_Blink();																		//发一包数据闪一次灯
	}

	HAL_Delay(100);

	cJSON_Delete(root);					                  //指针用完了以后，需要手工调用 cJSON_Delete(root)进行内存回收，释放内存空间
	root = NULL;

	memset(jsonOutPutBuf, 0, sizeof(jsonOutPutBuf));      //建议使用后清零

}

/*把设备上传需要的数据添加到JSON中 */
static void AddMessageToJsonObj(cJSON *root)
{
	_addIntToJson(root, "mqttType", 0, false);
//	_addIntToJson(root, "upInterval", 600, false);       //上传间隔没必要传了
//	_addTimeToJson(root, "collectDt");                   //没有电池，无法获取RTC ，时间不传了
//	_addMd5ToJson(root, "apptoken");                     //无法获取时间，MD5不传

	_addDataToJson(root);
}

static void _addIntToJson(cJSON *root, const char *name, uint32_t value, bool complement)
{
	uint8_t num_buf[8]={0};
	if(complement)
	{
		sprintf((char *) num_buf, "%02ld", value);
	}
	else
	{
		sprintf((char *) num_buf, "%0ld", value);
	}

	cJSON_AddStringToObject(root, name, (const char*) num_buf);
}

static void _addFloatToJson(cJSON *js_list, const char *name, float value)
{
	uint8_t value_buf[8]={0};

	sprintf((char*)value_buf, "%3.1f", value);
	cJSON_AddStringToObject(js_list, name, (const char*)value_buf);
}

static void _addDataToJson(cJSON *root)
{
	switch(Sensor_Type)
	{
		case Temperature_Humidity_Type:
		case Outside_Temperature_Humidity_Type:
			AddDataToJson(root, 1, Sensor_ID, (float)Sensor_Data.Temperature/10.0, (float)Sensor_Data.Humidity/10.0);
			break;
		case Water_Temperature_Type:
			AddDataToJson(root, 1, Sensor_ID, (float)Sensor_Data.WaterTemperature/10.0, 0);
			break;
		case Negative_Pressure_Type:
			AddDataToJson(root, 7, 1, (float)Sensor_Data.NegativePressure, 0);
			break;
		case Gas_CO2_Type:
			AddDataToJson(root, 3, 1, (float)Sensor_Data.CO2_Data, 0);
			break;
		case Gas_NH3_Type:
			AddDataToJson(root, 2, 1, (float)Sensor_Data.NH3_Data, 0);
			break;
		case Water_Meter_Type:
			AddDataToJson(root, 6, 1, (float)Sensor_Data.WaterMeter, 0);
			break;
		case Alarm_Type:
			AddDevNumToJson(root, "devNum", 10, 1);
			break;
	}

}

static void AddDataToJson(cJSON *root, uint8_t datatype, uint8_t order, float dataValue1, float dataValue2)
{

	AddDevNumToJson(root, "devNum", datatype, order);
	_addFloatToJson(root, "dataValue1", dataValue1);

	if(Sensor_Type == Temperature_Humidity_Type || Sensor_Type == Outside_Temperature_Humidity_Type)
	{
		_addFloatToJson(root, "dataValue2", dataValue2);
	}
}

static void AddDevNumToJson(cJSON *root, const char *name, uint8_t datatype, uint8_t order)
{
	uint8_t typord[5]  = { 0 };
	uint8_t devnum[21] = { 0 };

	strcpy((char *)devnum, (const char *)NBiotIMEI);
	sprintf((char*)typord, "-%02d-%d", datatype, order);   //将NBiotIMEI与传感器类型、传感器编号放在一起
	strcat((char *)devnum,(const char *)typord);

	cJSON_AddStringToObject(root, name, (const char*)devnum);
}



/*
static void _addTimeToJson(cJSON *root, const char *name)
{
	uint8_t time_str_buf[26] = { '\0' };

	RTC_DateTypeDef date;
	RTC_TimeTypeDef time;

	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
	HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);

	sprintf((char *) time_str_buf, "%d-%02d-%02d %02d:%02d:%02d", 2000 + date.Year, date.Month, date.Date, time.Hours, time.Minutes, time.Seconds);
	cJSON_AddStringToObject(root, name, (const char*) time_str_buf);
}

static void _addMd5ToJson(cJSON *root,const char *name)
{
	uint8_t date_str_buf[42] = { '\0' };
	uint8_t md5_str[17];

	const char MD5_KEY[] = "ABCk4JBQWEL.SNWn2&(M)@kEY";

	MD5_CTX hmd5;
	RTC_DateTypeDef date;

	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
	sprintf((char *) date_str_buf, "%d%02d%02d", 2000 + date.Year, date.Month, date.Date);
	strcat((char*) date_str_buf, (char*) MD5_KEY);

	MD5Init(&hmd5);
	MD5Update(&hmd5, (uint8_t*) date_str_buf, strlen((const char*) date_str_buf));
	MD5Final(&hmd5, md5_str);

	memset(date_str_buf,0,sizeof(date_str_buf));
	for (uint32_t i = 0; i < 16; i++)              //MD5值转成字符
	{
		uint8_t temp = md5_str[i];
		sprintf((char*) (date_str_buf + 2 * i), "%02x", temp);
	}

	cJSON_AddStringToObject(root, name, (const char*) date_str_buf);
}
*/

