/*
 * GAS.c
 *
 *  Created on: 2019��10��16��
 *      Author: Breuning
 */
#include "string.h"
#include "main.h"
#include "Gas.h"
#include "usart.h"
#include "SensorAnalysis.h"
#include "HardwareInit.h"



uint8_t Gas_ChangeToAskCommand[TransmitDataLen_Gas]    = {0xFF, 0x01, 0x78, 0x04, 0x00, 0x00, 0x00, 0x00, 0x83};
uint8_t Gas_ChangeToActiveCommand[TransmitDataLen_Gas] = {0xFF, 0x01, 0x78, 0x03, 0x00, 0x00, 0x00, 0x00, 0x84};
uint8_t Gas_ReadValueCommand[TransmitDataLen_Gas]      = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};

void MHZ14CO2_Init(void)
{
//	���ڽ��տ����жϳ�ʼ��ʱ�ѿ���
//	HAL_UART_Receive_IT(&huart3, GasUart_RX.RX_Buf, RecieveDataLen_Gas);                //��������3�����ж�
}

void ZE03GAS_Init(void)
{
	HAL_UART_Transmit(&huart3, Gas_ChangeToActiveCommand, TransmitDataLen_Gas, 0xff);  //���������ϴ�ģʽ�����ֹģ�����ʱδ����
//	���ڽ��տ����жϳ�ʼ��ʱ�ѿ���
//	HAL_UART_Receive_IT(&huart3, GasUart_RX.RX_Buf, RecieveDataLen_Gas);                //��������3�����ж�
}


void GetValidDataFromMHZ14CO2(void)
{

	HAL_UART_Transmit(&huart3, Gas_ReadValueCommand, TransmitDataLen_Gas, 0xff);       //���Ͷ�ȡ����Ũ��ֵ����

	if(GasUart_RX.RX_Buf[0] == 0xFF && GasUart_RX.RX_Buf[1] == 0x86)
	{
		if(GasReceiveDataCheckSum(GasUart_RX.RX_Buf) == GasUart_RX.RX_Buf[8])
		{
			Sensor_Data.CO2_Data = GasUart_RX.RX_Buf[2]*256 + GasUart_RX.RX_Buf[3];
			Tick_GetSensorData = HAL_GetTick();
		}
	}

	memset(GasUart_RX.RX_Buf, 0 , sizeof(GasUart_RX.RX_Buf));

}

void GetValidDataFromZE03GAS(uint16_t type)
{

//	HAL_UART_Transmit(&huart3, Gas_ReadValueCommand, TransmitDataLen_Gas, 0xff);      //NH3ģ��Ϊ1sһ�ε������ϴ�ģʽ�����跢�Ͷ�ȡ����

	if(GasUart_RX.RX_Buf[0] == 0xFF && GasUart_RX.RX_Buf[1] == 0x86)
	{
		if(GasReceiveDataCheckSum(GasUart_RX.RX_Buf) == GasUart_RX.RX_Buf[8])
		{
			if(type == Gas_NH3_Type)  //����
			{
				Sensor_Data.NH3_Data = GasUart_RX.RX_Buf[2]*256 + GasUart_RX.RX_Buf[3];
			}
			if(type == Gas_O2_Type)  //����
			{
				Sensor_Data.O2_Data = GasUart_RX.RX_Buf[2]*256 + GasUart_RX.RX_Buf[3];
			}

			Tick_GetSensorData = HAL_GetTick();
		}
	}

	memset(GasUart_RX.RX_Buf, 0 , sizeof(GasUart_RX.RX_Buf));

}

uint8_t GasReceiveDataCheckSum(uint8_t *array)
{
	uint8_t CheckValue = 0;
	uint8_t i;
	for(i = 1; i<8; i++)
	{
		CheckValue += array[i];
	}
	CheckValue = (~CheckValue) + 1;
	return CheckValue;
}
