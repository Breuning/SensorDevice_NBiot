/*
 * SHT30.h
 *
 *  Created on: 2019��8��2��
 *      Author: Breuning
 */

#ifndef SHT30_H_
#define SHT30_H_

#include "main.h"
/*-----------------------------�궨��---------------------------------*/


#define	  SHT30_SlaveAddress	  		(0x44<<1)	         //7λ��ַ0x44 ����1λ
#define   SHT30Addr_Write   (SHT30_SlaveAddress + 0) 		 //�ӻ���ַ+���д����λ
#define   SHT30Addr_Read    (SHT30_SlaveAddress + 1) 		 //�ӻ���ַ+��������λ



#define SHT30_SCL0_H  	HAL_GPIO_WritePin(I2C1_SCL_GPIO_Port, I2C1_SCL_Pin, GPIO_PIN_SET)
#define SHT30_SCL0_L  	HAL_GPIO_WritePin(I2C1_SCL_GPIO_Port, I2C1_SCL_Pin, GPIO_PIN_RESET)

#define SHT30_SDA0_H  	HAL_GPIO_WritePin(I2C1_SDA_GPIO_Port, I2C1_SDA_Pin, GPIO_PIN_SET)
#define SHT30_SDA0_L  	HAL_GPIO_WritePin(I2C1_SDA_GPIO_Port, I2C1_SDA_Pin, GPIO_PIN_RESET)

#define SHT30_SDA0_DAT  HAL_GPIO_ReadPin(I2C1_SDA_GPIO_Port, I2C1_SDA_Pin)
#define SHT30_SCL0_DAT  HAL_GPIO_ReadPin(I2C1_SCL_GPIO_Port, I2C1_SCL_Pin)


#define noACK 0                                 //�����ж��Ƿ����ͨѶ
#define ACK   1                                 //�������ݴ���

//����
#define SHT30_SINGLE_H_MEASURE_EN				0x2C06			//���θ߾��Ȳ���ʹ��
#define SHT30_SINGLE_M_MEASURE_EN				0x2C0D			//������ͨ���Ȳ���ʹ��
#define SHT30_SINGLE_L_MEASURE_EN				0x2C10			//���ε;��Ȳ���ʹ��
#define SHT30_SINGLE_H_MEASURE_DIS				0x2400			//���θ߾��Ȳ����ر�
#define SHT30_SINGLE_M_MEASURE_DIS				0x240B			//������ͨ���Ȳ����ر�
#define SHT30_SINGLE_L_MEASURE_DIS				0x2416			//���ε;��Ȳ����ر�

#define SHT30_PERIODOC_H_MEASURE_500MS			0x2032			//���500Ms�߾��Ȳ���        ��ߵ�ע��ȫ������Ӧ��Ϊÿ�����0.5�θ߾��Ȳ���
#define SHT30_PERIODOC_M_MEASURE_500MS			0x2024			//���500Ms�о��Ȳ���
#define SHT30_PERIODOC_L_MEASURE_500MS			0x202F			//���500Ms�;��Ȳ���
#define SHT30_PERIODOC_H_MEASURE_1S				0x2130			//���1s�߾��Ȳ���
#define SHT30_PERIODOC_M_MEASURE_1S				0x2126			//���1s�о��Ȳ���
#define SHT30_PERIODOC_L_MEASURE_1S				0x212D			//���1s�;��Ȳ���
#define SHT30_PERIODOC_H_MEASURE_2S				0x2236			//���2s�߾��Ȳ���
#define SHT30_PERIODOC_M_MEASURE_2S				0x2220			//���2s�о��Ȳ���
#define SHT30_PERIODOC_L_MEASURE_2S				0x222B			//���2s�;��Ȳ���
#define SHT30_PERIODOC_H_MEASURE_4S				0x2334			//���4s�߾��Ȳ���
#define SHT30_PERIODOC_M_MEASURE_4S				0x2322			//���4s�о��Ȳ���
#define SHT30_PERIODOC_L_MEASURE_4S				0x2329			//���4s�;��Ȳ���
#define SHT30_PERIODOC_H_MEASURE_10S			0x2737			//���10s�߾��Ȳ���
#define SHT30_PERIODOC_M_MEASURE_10S			0x2721			//���10s�о��Ȳ���
#define SHT30_PERIODOC_L_MEASURE_10S			0x272A			//���10s�;��Ȳ���

#define SHT30_PERIODOC_MEASURE_READ				0xE000			//��ȡ��������

#define	SHT30_SOFT_RESET						0x30A2			//��λ

#define SHT30_HEATER_EN							0x306D			//����ʹ��
#define SHT30_HEATER_DIS						0x3066			//���ȹر�

#define SHT30_READ_STATUS						0xF32D			//��״̬�Ĵ���
#define SHT30_CLEAR_STATUS						0x3041			//��״̬�Ĵ���

// CRC
#define POLYNOMIAL  0x131 // P(x) = x^8 + x^5 + x^4 + 1 = 100110001




/*-----------------------------��������-------------------------------*/
/*
void SHT30_Delayms(uint8_t k);
void SHT30_Delayus(uint16_t vTemp);
void SHT30_Init(void);
void SHT30_Start(void);                          											//��ʼ�ź�
void SHT30_Stop(void);                           											//ֹͣ�ź�
void SHT30_SendACK(uint8_t ack);                  											//Ӧ��ACK
uint8_t SHT30_RecvACK(void);                      											//��ack
uint8_t SHT30_SendByte(uint8_t dat);               											//IIC�����ֽ�д
uint8_t SHT30_RecvByte(void);                    										    //IIC�����ֽڶ�
uint8_t SHT30_CheckCrc8(uint8_t *vDat, uint8_t vLen);										//CRC8 У��

uint16_t SHT30_ReadSerialNumber(void);
uint8_t SHT30_Single_Measure(uint8_t *vBuf);												//���βɼ�����
uint8_t SHT30_Periodic_Measure(uint16_t vCmd);												//�������ڲɼ�
uint8_t SHT30_Periodic_Measure_Read(uint8_t *vBuf);											//���ڲɼ���ȡ����
uint8_t SHT30_Soft_Reset(void);																//��λ
uint8_t SHT30_Heater(uint16_t vCmd);														//����������
uint16_t SHT30_ReadStaus(void);																//��ȡ״̬�Ĵ���
uint8_t SHT30_ClearStaus(void);																//���״̬�Ĵ���

void SHT30_Get_TH(uint8_t *vTemSymbol, uint16_t *vTem, uint16_t *vHum);						//��ȡ��ʪ������
*/


void SHT30_Init(void);
void GetValidDataFromSHT30(void);
void SHT30_Calc(uint8_t *vBuf, uint8_t *vTemSymbol, uint16_t *vTem, uint16_t *vHum);		//����ɼ�����
void TemperatureData_NegativeValueJudge(void);
uint8_t SHT30_CheckCrc8(uint8_t *vDat, uint8_t vLen);										//CRC8 У��








#endif /* SHT30_H_ */
