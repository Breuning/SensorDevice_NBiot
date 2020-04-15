/*
 * SHT30.c
 *
 *  Created on: 2019��8��2��
 *      Author: Breuning
 */
#include <string.h>
#include "stm32f1xx_hal.h"
#include "SHT30.h"
#include "tim.h"
#include "SensorAnalysis.h"
#include "FilterAlgorithm.h"
#include "i2c.h"


uint8_t SHT30_Modecommand_Buffer[2]={0x20, 0x32}; 			//ÿ�����0.5�θ߾��Ȳ�������
uint8_t SHT30_Fetchcommand_Buffer[2]={0xE0,0x00};           //��ȡ�����������
uint8_t SHT30_Data_Buffer[6] = {0};                         //Byte0��1Ϊ�¶�,Byte2Ϊ�¶�CRCУ��ֵ; Byte3��4Ϊʪ��,Byte5Ϊ�¶�CRCУ��ֵ


void SHT30_Init(void)
{//��ʼ���Ƶ�ѭ��
 //	HAL_I2C_Master_Transmit(&hi2c1, SHT30Addr_Write, SHT30_Modecommand_Buffer, 2, 1000);      //SHT30�����������ԵĽ�����ʪ��ת��
}

void GetValidDataFromSHT30(void)
{
	HAL_I2C_Master_Transmit(&hi2c1, SHT30Addr_Write, SHT30_Modecommand_Buffer, 2, 1000);
	HAL_Delay(5);
	HAL_I2C_Master_Transmit(&hi2c1, SHT30Addr_Write, SHT30_Fetchcommand_Buffer, 2, 1000);     //���ͻ�ȡ���������ݵ�����
	HAL_I2C_Master_Receive (&hi2c1, SHT30Addr_Read, SHT30_Data_Buffer, 6, 1000);			  //���մ��������ص�����ֵ�ŵ�SHT30_Data_Buffer

	if(strlen(SHT30_Data_Buffer) != 0)
		Tick_GetSensorData = HAL_GetTick();
	else
		return;

	SHT30_Calc(SHT30_Data_Buffer, &Sensor_Data.Temperature_Flag, &Sensor_Data.Temperature_u, &Sensor_Data.Humidity);

	memset(SHT30_Data_Buffer, 0 , sizeof(SHT30_Data_Buffer));
}

/*---------------------------------------------------------------------
 ��������: SHT30 �����������
 ����˵��:  vBuf [in] - ������ȡ���
 	 	   vTemSymbol [out] - �����¶���������
 	 	   vTem [out] - �¶�
 	 	   vHum [out] - ʪ��
 ��������: ��
 ---------------------------------------------------------------------*/
void SHT30_Calc(uint8_t *vBuf, uint8_t *vTemSymbol, uint16_t *vTem, uint16_t *vHum)
{

	uint8_t vCrc = 0x00;
	float vTemp = 0.00;

	//�¶�
	vCrc = SHT30_CheckCrc8(vBuf, 2);
	if (vCrc == vBuf[2])  //CrcУ��
	{
		vTemp = 175.0 * ((vBuf[0] << 8) + vBuf[1]) / (65536.0 - 1.0);
		if(vTemp >= 45)
		{
			*vTemSymbol = 1;                     //���¶�ֵ
			*vTem = (vTemp - 45.0) * 10.0;       //�õ����϶��¶�ֵx10
		}
		if(vTemp < 45)
		{
			*vTemSymbol = 0;                     //���¶�ֵ
			*vTem = (45.0 - vTemp) * 10.0;       //�õ����϶��¶�ֵx10
		}
	}


	//ʪ��
	vBuf += 3;
	vCrc = SHT30_CheckCrc8(vBuf, 2);
	if (vCrc == vBuf[2])  //CrcУ��
	{
		vTemp = 100.0 * ((vBuf[0] << 8) + vBuf[1]) / (65536.0 - 1.0);
		*vHum = (vTemp * 10);				 //�õ����ʪ��ֵx10
	}
}


void TemperatureData_NegativeValueJudge(void)
{
	if(Sensor_Data.Temperature_Flag)
	{
		Sensor_Data.Temperature   = (int16_t)Sensor_Data.Temperature_u;
		Sensor_Data.Temperature_m = (uint16_t)Sensor_Data.Temperature_u;
	}
	else
	{
		Sensor_Data.Temperature   = (int16_t)(Sensor_Data.Temperature_u*(-1));
		Sensor_Data.Temperature_m = (uint16_t)(Sensor_Data.Temperature_u | 0x8000);
	}
}


//SHT30�������crc����
uint8_t SHT30_CheckCrc8(uint8_t *vDat, uint8_t vLen)
{
	uint8_t vBit;        // bit mask
	uint8_t vCrc = 0xFF; // calculated checksum
	uint8_t vByteCtr;    // byte counter

	// calculates 8-Bit checksum with given polynomial
	for (vByteCtr = 0; vByteCtr < vLen; vByteCtr++)
	{
		vCrc ^= (vDat[vByteCtr]);
		for (vBit = 8; vBit > 0; --vBit)
		{
			if (vCrc & 0x80)
				vCrc = (vCrc << 1) ^ POLYNOMIAL;
			else
				vCrc = (vCrc << 1);
		}
	}
	return vCrc;
}








/*****************************GPIOģ��IIC��������(SHT30)***********************************/

///*---------------------------------------------------------------------
// ��������: SHT30 �����������
// ����˵��:  vTemSymbol [out] - �����¶ȷ���
// vTem [out] - �¶�
// vHum [out] - ʪ��
//
// ��������: ��
// ---------------------------------------------------------------------*/
//void SHT30_Get_TH(uint8_t *vTemSymbol, uint16_t *vTem, uint16_t *vHum)
//{
//	uint8_t vDat[8];
//	uint8_t vRval = 0;
//
//	vRval = SHT30_Single_Measure(vDat);                 //SHT30���β���,���y���Y���xȡ��vDat[8];
//	if (!vRval)
//	SHT30_calc(vDat, vTemSymbol, vTem, vHum);       	//��vDat�ڵ����ݷֿ���ţ��¶����ݷ���vTem��ʪ�����ݷ���vHum
//}
//
///*---------------------------------------------------------------------
// ��������: SHT30���β���
// ����˵��: vBuf [out] - ������ȡ���
// ��������: 0 - �ɹ�  ����1����
// ---------------------------------------------------------------------*/
//uint8_t SHT30_Single_Measure(uint8_t *vBuf)
//{
//	uint8_t vRval = 0;
//	uint8_t i = 0;
//
//	SHT30_Start();
//
//	vRval |= SHT30_SendByte(SHT30_SlaveAddress + 0);					 //��ַд
//	if (!vRval)
//		vRval |= SHT30_SendByte((SHT30_SINGLE_H_MEASURE_EN >> 8) & 0xFF);//ʹ�ܸ߾��Ȳɼ�
//	if (!vRval)
//		vRval |= SHT30_SendByte((SHT30_SINGLE_H_MEASURE_EN) & 0xFF);
//	SHT30_Stop();
//
//	if (vRval)
//		return vRval;
//
//	SHT30_SCL0_H;
//	SHT30_Delayms(15);													//15Ms
//
//	SHT30_Start();
//	if (!vRval)
//		vRval |= SHT30_SendByte(SHT30_SlaveAddress + 1);				//��ַ��
//
//	if (vRval)
//		return vRval;
//
//	for (i = 0; i < 6; i++)
//	{
//		vBuf[i] = SHT30_RecvByte();                	                    //�洢����
//		if (i == 0x06)
//		{
//
//			SHT30_SendACK(1);                                           //���һ��������Ҫ��NOACK
//		}
//		else
//		{
//			SHT30_SendACK(0);                                           //��ӦACK
//		}
//	}
//
//	SHT30_Stop();
//
//	SHT30_Start();
//	if (!vRval)
//		vRval |= SHT30_SendByte(SHT30_SlaveAddress + 0);				//��ַд
//	if (!vRval)
//		vRval |= SHT30_SendByte((SHT30_SINGLE_H_MEASURE_DIS >> 8) & 0xFF);//�رո߾��Ȳɼ�
//	if (!vRval)
//		vRval |= SHT30_SendByte((SHT30_SINGLE_H_MEASURE_DIS) & 0xFF);
//	SHT30_Stop();
//
//	return vRval;
//}
//
//
//void SHT30_Delayms(uint8_t k)
//{
//	HAL_Delay(k);
//}
//
//void SHT30_Delayus(uint16_t vTemp)
//{
//	Delay_us(vTemp);
//}
//
///*-------------------------------------------------------
// ����������������ʼ��
// ��    ������
// �� �� ֵ����
// ---------------------------------------------------------*/
//void SHT30_Init(void)
//{
//	SHT30_SCL0_H;
//	SHT30_SCL0_L;
//
//	SHT30_SDA0_H;
//	SHT30_SDA0_L;
//
//	SHT30_Soft_Reset();
//
//	SHT30_ReadSerialNumber();
//
//	SHT30_Delayms(1);
//
//	SHT30_ClearStaus();
//
//	SHT30_Delayms(1);
//
//	//SHT30_Periodic_Measure(SHT30_PERIODOC_H_MEASURE_1S);
//
//}
//
//uint16_t SHT30_ReadSerialNumber()
//{
//	uint8_t vRval = 0x00;
//	uint16_t vVal = 0x00;
//	uint8_t vCrc = 0x00;
//
//	uint8_t i = 0;
//	uint8_t vBuf[3];
//
//	SHT30_Start();
//	vRval |= SHT30_SendByte(SHT30_SlaveAddress + 0);			//��ַд
//	vRval |= SHT30_SendByte(0x37);			                    //��ȡ״̬�Ĵ���
//	vRval |= SHT30_SendByte(0x80);
//
//	SHT30_Start();
//	vRval |= SHT30_SendByte(SHT30_SlaveAddress + 1);			//��ַ��
//
//	for (i = 0; i < 3; i++)
//	{
//		vBuf[i] = SHT30_RecvByte();                				//�洢����
//		if (i == 0x03)
//		{
//			SHT30_SendACK(1);                         			//���һ��������Ҫ��NOACK
//		}
//		else
//		{
//			SHT30_SendACK(0);                         			//��ӦACK
//		}
//	}
//
//	SHT30_Stop();
//
//	if (vRval > 0)
//	{
//		return 0xFFFF;
//	}
//
//	vCrc = SHT30_CheckCrc8(vBuf, 2);
//	if (vCrc == vBuf[2])
//	{
//		vVal |= vBuf[0];
//		vVal <<= 8;
//		vVal |= vBuf[1];
//	}
//
//	return vVal;
//}
//
///*---------------------------------------------------------------------
// ��������: ��ʼ�ź�
// ����˵��: ��
// ��������: ��
// ---------------------------------------------------------------------*/
//void SHT30_Start()
//{
//	SHT30_SDA0_H;                         //����������
//	SHT30_Delayus(1);
//	SHT30_SCL0_H;                         //����ʱ����
//	SHT30_Delayus(1);                     //��ʱ
//	SHT30_SDA0_L;                         //�����½���
//	SHT30_Delayus(10);                     //��ʱ
//	SHT30_SCL0_L;                         //����ʱ����
//	SHT30_Delayus(10);
//}
//
///*---------------------------------------------------------------------
// ��������: ֹͣ�ź�
// ����˵��: ��
// ��������: ��
// ---------------------------------------------------------------------*/
//void SHT30_Stop()
//{
//	SHT30_SCL0_L;                         //����ʱ����
//	SHT30_Delayus(1);
//	SHT30_SDA0_L;                         //����������
//	SHT30_Delayus(1);                     //��ʱ
//	SHT30_SCL0_H;
//	SHT30_Delayus(10);                     //��ʱ
//	SHT30_SDA0_H;                         //����������
//	SHT30_Delayus(10);
//}
//
///*---------------------------------------------------------------------
// ��������: ����Ӧ���ź�
// ����˵��: ack [in] - Ӧ���ź�(0:ACK 1:NAK)
// ��������: ��
// ---------------------------------------------------------------------*/
//void SHT30_SendACK(uint8_t ack)
//{
//	if (ack & 0x01)
//		SHT30_SDA0_H;		              //дӦ���ź�
//	else
//		SHT30_SDA0_L;
//
//	SHT30_Delayus(1);
//	SHT30_SCL0_H;                        //����ʱ����
//	SHT30_Delayus(5);                    //��ʱ
//	SHT30_SCL0_L;                        //����ʱ����
//	SHT30_SDA0_H;
//	SHT30_Delayus(20);                   //��ʱ
//}
//
///*---------------------------------------------------------------------
// ��������: ����Ӧ���ź�
// ����˵��: ��
// ��������: ����Ӧ���ź�
// ---------------------------------------------------------------------*/
//uint8_t SHT30_RecvACK()
//{
//	uint8_t CY = 0x00;
//	uint16_t vConter = 1000;
//	SHT30_SDA0_H;
//
////	I2C_SDA_IN();
//
//	SHT30_SCL0_H;                             //����ʱ����
//	SHT30_Delayus(1);                         //��ʱ
//	while (vConter)
//	{
//		vConter--;
//		CY |= SHT30_SDA0_DAT;                 //��Ӧ���ź�
//
//		if (!CY)
//			break;
//	}
//	SHT30_Delayus(20);                        //��ʱ
//	SHT30_SCL0_L;
////	I2C_SDA_OUT();
//
//	return CY;
//}
//
///*---------------------------------------------------------------------
// ��������: ��IIC���߷���һ���ֽ�����
// ����˵��: dat [in] - д�ֽ�
// ��������: 0 - �ɹ�   1- ����
// ---------------------------------------------------------------------*/
//uint8_t SHT30_SendByte(uint8_t dat)
//{
//	uint8_t vRval = 0x01;
//	uint8_t i;
//
//	for (i = 0; i < 8; i++)         		//8λ������
//	{
//		if (dat & 0x80)
//			SHT30_SDA0_H;
//		else
//			SHT30_SDA0_L;               	//�����ݿ�
//
//		SHT30_Delayus(1);             		//��ʱ
//		SHT30_SCL0_H;                		//����ʱ����
//		SHT30_Delayus(5);             		//��ʱ
//		SHT30_SCL0_L;                		//����ʱ����
//		SHT30_Delayus(1);             		//��ʱ
//		dat <<= 1;              			//�Ƴ����ݵ����λ
//	}
//
//	vRval = SHT30_RecvACK();
//
//	return vRval;
//}
//
///*---------------------------------------------------------------------
// ��������: ��IIC���߽���һ���ֽ�����
// ����˵��: ��
// ��������: �����ֽ�
// ---------------------------------------------------------------------*/
//uint8_t SHT30_RecvByte()
//{
//	uint8_t i;
//	uint8_t dat = 0;
//	uint16_t vConter = 1000;
//
//	SHT30_SDA0_H;                         	//ʹ���ڲ�����,׼����ȡ����,
//
////	I2C_SDA_IN();
//	for (i = 0; i < 8; i++)         	    //8λ������
//	{
//		SHT30_SCL0_H;                       //����ʱ����
//		SHT30_Delayus(1);             		//��ʱ
//
////		I2C_SCL_IN();
//		while (SHT30_SCL0_DAT == 0)
//		{
//			vConter--;
//			if (vConter == 0)
//				break;
//		}
////		I2C_SCL_OUT();
//		SHT30_Delayus(3);
//
//		dat |= SHT30_SDA0_DAT;              //������
//		SHT30_SCL0_L;                       //����ʱ����
//		SHT30_Delayus(1);             		//��ʱ
//
//		if (i < 7)
//			dat <<= 1;
//	}
////	I2C_SDA_OUT();
//
//	return dat;
//}
//
///*---------------------------------------------------------------------
// ��������: SHT30��������λ (����ģʽ�¹ر���������)
// ����˵��: ��
// ��������: 0 - �ɹ�  ����1����
// ---------------------------------------------------------------------*/
//uint8_t SHT30_Soft_Reset()
//{
//	uint8_t vRval = 0;
//
//	SHT30_Start();
//
//	if (!vRval)
//		vRval |= SHT30_SendByte(SHT30_SlaveAddress + 0);				//��ַд
//	if (!vRval)
//		vRval |= SHT30_SendByte((SHT30_SOFT_RESET >> 8) & 0xFF);
//	if (!vRval)
//		vRval |= SHT30_SendByte((SHT30_SOFT_RESET) & 0xFF);
//	SHT30_Stop();
//
//	return vRval;
//}
//
///*---------------------------------------------------------------------
// ��������: SHT30���״̬�Ĵ���
// ����˵��: ��
// ��������: 0 - �ɹ�  ����1����
// ---------------------------------------------------------------------*/
//uint8_t SHT30_ClearStaus()
//{
//	uint8_t vRval = 0;
//	SHT30_Start();
//
//	if (!vRval)
//		vRval |= SHT30_SendByte(SHT30_SlaveAddress + 0);				//��ַд
//	if (!vRval)
//		vRval |= SHT30_SendByte((SHT30_CLEAR_STATUS >> 8) & 0xFF);
//	if (!vRval)
//		vRval |= SHT30_SendByte((SHT30_CLEAR_STATUS) & 0xFF);
//	SHT30_Stop();
//
//	return vRval;
//}
//

//
///*---------------------------------------------------------------------
// ��������: SHT30�������ڲ���
// ����˵��: vBuf [in] - ��������
// ��������: 0 - �ɹ�  ����1����
// ---------------------------------------------------------------------*/
//uint8_t SHT30_Periodic_Measure(uint16_t vCmd)
//{
//	uint8_t vRval = 0;
//
//	SHT30_Start();
//	vRval |= SHT30_SendByte(SHT30_SlaveAddress + 0);					//��ַд
//	if (!vRval)
//		vRval |= SHT30_SendByte((vCmd >> 8) & 0xFF);
//	if (!vRval)
//		vRval |= SHT30_SendByte((vCmd) & 0xFF);
//	SHT30_Stop();
//
//	return vRval;
//}
//
///*---------------------------------------------------------------------
// ��������: SHT30���ڲ��� ��ȡ����
// ����˵��: vBuf [out] - ��������
// ��������: 0 - �ɹ�  ����1����
// ---------------------------------------------------------------------*/
//uint8_t SHT30_Periodic_Measure_Read(uint8_t *vBuf)
//{
//	uint8_t vRval = 0;
//
//	uint8_t i = 0;
//
//	SHT30_Start();
//	vRval |= SHT30_SendByte(SHT30_SlaveAddress + 0);					    //��ַд
//	if (!vRval)
//		vRval |= SHT30_SendByte((SHT30_PERIODOC_MEASURE_READ >> 8) & 0xFF);	//��ȡ���ڲɼ�����
//	if (!vRval)
//		vRval |= SHT30_SendByte((SHT30_PERIODOC_MEASURE_READ) & 0xFF);
//
//	SHT30_Start();
//	if (!vRval)
//		vRval |= SHT30_SendByte(SHT30_SlaveAddress + 1);					//��ַ��
//
//	if (vRval)
//		return vRval;
//
//	for (i = 0; i < 6; i++)
//	{
//		vBuf[i] = SHT30_RecvByte();                							//�洢����
//		if (i == 0x06)
//		{
//
//			SHT30_SendACK(1);                         						//���һ��������Ҫ��NOACK
//		}
//		else
//		{
//			SHT30_SendACK(0);                        						//��ӦACK
//		}
//	}
//
//	SHT30_Stop();
//
//	return vRval;
//}
//
///*---------------------------------------------------------------------
// ��������: SHT30��������
// ����˵��: vCmd [in] - 0x306D�򿪼���  0x3066�رռ���
// ��������: 0 - �ɹ�  ����1����
// ---------------------------------------------------------------------*/
//uint8_t SHT30_Heater(uint16_t vCmd)
//{
//	uint8_t vRval = 0;
//
//	SHT30_Start();
//
//	if (!vRval)
//		vRval |= SHT30_SendByte(SHT30_SlaveAddress + 0);				//��ַд
//	if (!vRval)
//		vRval |= SHT30_SendByte((vCmd >> 8) & 0xFF);
//	if (!vRval)
//		vRval |= SHT30_SendByte((vCmd) & 0xFF);
//	SHT30_Stop();
//
//	return vRval;
//}
//
///*---------------------------------------------------------------------
// ��������: SHT30��ȡ״̬�Ĵ���
// ����˵��: ��
// ��������: ����״̬�Ĵ���
// ---------------------------------------------------------------------*/
//uint16_t SHT30_ReadStaus()
//{
//	uint8_t vRval = 0x00;
//	uint16_t vVal = 0x00;
//	uint8_t vCrc = 0x00;
//
//	uint8_t i = 0;
//	uint8_t vBuf[3];
//
//	SHT30_Start();
//	if (!vRval)
//		vRval |= SHT30_SendByte(SHT30_SlaveAddress + 0);				//��ַд
//	if (!vRval)
//		vRval |= SHT30_SendByte((SHT30_READ_STATUS >> 8) & 0xFF);		//��ȡ״̬�Ĵ���
//	if (!vRval)
//		vRval |= SHT30_SendByte((SHT30_READ_STATUS) & 0xFF);
//
//	SHT30_Start();
//	if (!vRval)
//		vRval |= SHT30_SendByte(SHT30_SlaveAddress + 1);				//��ַ��
//
//	if (vRval > 0)
//	{
//		return 0xFFFF;
//	}
//
//	for (i = 0; i < 3; i++)
//	{
//		vBuf[i] = SHT30_RecvByte();                						//�洢����
//		if (i == 0x03)
//		{
//
//			SHT30_SendACK(1);                         					//���һ��������Ҫ��NOACK
//		}
//		else
//		{
//			SHT30_SendACK(0);                         					//��ӦACK
//		}
//	}
//
//	SHT30_Stop();
//
//	if (vRval > 0)
//	{
//		return 0xFFFF;
//	}
//
//	vCrc = SHT30_CheckCrc8(vBuf, 2);
//	if (vCrc == vBuf[2])
//	{
//		vVal |= vBuf[0];
//		vVal <<= 8;
//		vVal |= vBuf[1];
//	}
//
//	return vVal;
//}




