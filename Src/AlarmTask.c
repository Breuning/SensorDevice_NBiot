/*
 * AlarmTask.c
 *
 *  Created on: 2020年4月17日
 *      Author: Breuning
 */

#include "WaterMeter.h"
#include "NBiotTask.h"
#include "HardwareInit.h"
#include "usart.h"
#include "string.h"

static void Alarm_Control();

uint8_t AlarmOn_Command [12] = {0x01,0x10,0x00,0x1A,0x00,0x04,0x05,0x08,0x00,0x03,0x67,0x0C};
uint8_t AlarmOff_Command[12] = {0x01,0x10,0x00,0x1A,0x00,0x04,0x00,0x00,0x00,0x00,0xA6,0x03};

void Alarm_Task()
{
	char *Msg_AlarmSub = "+QMTRECV: 0,0,\"mqttTopicAlarm\"";

	if(Sensor_Type != Alarm_Type)
		return;

	if(NBUart_RX.receive_flag)
	{
		char *a = NULL, *b = NULL;
		a = strstr((const char *)NBUart_RX.RX_Buf, (const char *)Msg_AlarmSub);
		b = strstr((const char *)NBUart_RX.RX_Buf, (const char *)NBiotIMEI);

		if(a != NULL && b != NULL)
		{
			Alarm_Control();
		}

		NBUart_RX.receive_flag = 0;
		memset(NBUart_RX.RX_Buf, 0 , sizeof(NBUart_RX.RX_Buf));
	}

}

void Alarm_Control()
{
	char *AlarmOn  = "\"alarmState\":\"1\"";
	char *AlarmOff = "\"alarmState\":\"0\"";

	RS485Uart_RX_TX_Switch(TRANSMIT);   //485发送打开
	HAL_Delay(5);

	if(strstr((const char *)NBUart_RX.RX_Buf, AlarmOn) != NULL)
	{
		HAL_UART_Transmit(&huart4, AlarmOn_Command, 12, 0xff);       //发送打开报警器命令
	}

	else if(strstr((const char *)NBUart_RX.RX_Buf, AlarmOff) != NULL)
	{
		HAL_UART_Transmit(&huart4, AlarmOff_Command, 12, 0xff);       //发送关闭报警器命令
	}

	RS485Uart_RX_TX_Switch(RECEIVE);    //485接收打开，发送截止
	HAL_Delay(100);
}


