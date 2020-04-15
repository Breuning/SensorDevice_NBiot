/*
 * SHT30.h
 *
 *  Created on: 2019年8月2日
 *      Author: Breuning
 */

#ifndef SHT30_H_
#define SHT30_H_

#include "main.h"
/*-----------------------------宏定义---------------------------------*/


#define	  SHT30_SlaveAddress	  		(0x44<<1)	         //7位地址0x44 左移1位
#define   SHT30Addr_Write   (SHT30_SlaveAddress + 0) 		 //从机地址+最后写方向位
#define   SHT30Addr_Read    (SHT30_SlaveAddress + 1) 		 //从机地址+最后读方向位



#define SHT30_SCL0_H  	HAL_GPIO_WritePin(I2C1_SCL_GPIO_Port, I2C1_SCL_Pin, GPIO_PIN_SET)
#define SHT30_SCL0_L  	HAL_GPIO_WritePin(I2C1_SCL_GPIO_Port, I2C1_SCL_Pin, GPIO_PIN_RESET)

#define SHT30_SDA0_H  	HAL_GPIO_WritePin(I2C1_SDA_GPIO_Port, I2C1_SDA_Pin, GPIO_PIN_SET)
#define SHT30_SDA0_L  	HAL_GPIO_WritePin(I2C1_SDA_GPIO_Port, I2C1_SDA_Pin, GPIO_PIN_RESET)

#define SHT30_SDA0_DAT  HAL_GPIO_ReadPin(I2C1_SDA_GPIO_Port, I2C1_SDA_Pin)
#define SHT30_SCL0_DAT  HAL_GPIO_ReadPin(I2C1_SCL_GPIO_Port, I2C1_SCL_Pin)


#define noACK 0                                 //用于判断是否结束通讯
#define ACK   1                                 //结束数据传输

//命令
#define SHT30_SINGLE_H_MEASURE_EN				0x2C06			//单次高精度测量使能
#define SHT30_SINGLE_M_MEASURE_EN				0x2C0D			//单次普通精度测量使能
#define SHT30_SINGLE_L_MEASURE_EN				0x2C10			//单次低精度测量使能
#define SHT30_SINGLE_H_MEASURE_DIS				0x2400			//单次高精度测量关闭
#define SHT30_SINGLE_M_MEASURE_DIS				0x240B			//单次普通精度测量关闭
#define SHT30_SINGLE_L_MEASURE_DIS				0x2416			//单次低精度测量关闭

#define SHT30_PERIODOC_H_MEASURE_500MS			0x2032			//间隔500Ms高精度测量        这边的注释全部错误，应该为每秒进行0.5次高精度测量
#define SHT30_PERIODOC_M_MEASURE_500MS			0x2024			//间隔500Ms中精度测量
#define SHT30_PERIODOC_L_MEASURE_500MS			0x202F			//间隔500Ms低精度测量
#define SHT30_PERIODOC_H_MEASURE_1S				0x2130			//间隔1s高精度测量
#define SHT30_PERIODOC_M_MEASURE_1S				0x2126			//间隔1s中精度测量
#define SHT30_PERIODOC_L_MEASURE_1S				0x212D			//间隔1s低精度测量
#define SHT30_PERIODOC_H_MEASURE_2S				0x2236			//间隔2s高精度测量
#define SHT30_PERIODOC_M_MEASURE_2S				0x2220			//间隔2s中精度测量
#define SHT30_PERIODOC_L_MEASURE_2S				0x222B			//间隔2s低精度测量
#define SHT30_PERIODOC_H_MEASURE_4S				0x2334			//间隔4s高精度测量
#define SHT30_PERIODOC_M_MEASURE_4S				0x2322			//间隔4s中精度测量
#define SHT30_PERIODOC_L_MEASURE_4S				0x2329			//间隔4s低精度测量
#define SHT30_PERIODOC_H_MEASURE_10S			0x2737			//间隔10s高精度测量
#define SHT30_PERIODOC_M_MEASURE_10S			0x2721			//间隔10s中精度测量
#define SHT30_PERIODOC_L_MEASURE_10S			0x272A			//间隔10s低精度测量

#define SHT30_PERIODOC_MEASURE_READ				0xE000			//读取测量数据

#define	SHT30_SOFT_RESET						0x30A2			//软复位

#define SHT30_HEATER_EN							0x306D			//加热使能
#define SHT30_HEATER_DIS						0x3066			//加热关闭

#define SHT30_READ_STATUS						0xF32D			//读状态寄存器
#define SHT30_CLEAR_STATUS						0x3041			//清状态寄存器

// CRC
#define POLYNOMIAL  0x131 // P(x) = x^8 + x^5 + x^4 + 1 = 100110001




/*-----------------------------函数声明-------------------------------*/
/*
void SHT30_Delayms(uint8_t k);
void SHT30_Delayus(uint16_t vTemp);
void SHT30_Init(void);
void SHT30_Start(void);                          											//起始信号
void SHT30_Stop(void);                           											//停止信号
void SHT30_SendACK(uint8_t ack);                  											//应答ACK
uint8_t SHT30_RecvACK(void);                      											//读ack
uint8_t SHT30_SendByte(uint8_t dat);               											//IIC单个字节写
uint8_t SHT30_RecvByte(void);                    										    //IIC单个字节读
uint8_t SHT30_CheckCrc8(uint8_t *vDat, uint8_t vLen);										//CRC8 校验

uint16_t SHT30_ReadSerialNumber(void);
uint8_t SHT30_Single_Measure(uint8_t *vBuf);												//单次采集数据
uint8_t SHT30_Periodic_Measure(uint16_t vCmd);												//设置周期采集
uint8_t SHT30_Periodic_Measure_Read(uint8_t *vBuf);											//周期采集读取数据
uint8_t SHT30_Soft_Reset(void);																//软复位
uint8_t SHT30_Heater(uint16_t vCmd);														//传感器加热
uint16_t SHT30_ReadStaus(void);																//读取状态寄存器
uint8_t SHT30_ClearStaus(void);																//清楚状态寄存器

void SHT30_Get_TH(uint8_t *vTemSymbol, uint16_t *vTem, uint16_t *vHum);						//获取温湿度数据
*/


void SHT30_Init(void);
void GetValidDataFromSHT30(void);
void SHT30_Calc(uint8_t *vBuf, uint8_t *vTemSymbol, uint16_t *vTem, uint16_t *vHum);		//计算采集数据
void TemperatureData_NegativeValueJudge(void);
uint8_t SHT30_CheckCrc8(uint8_t *vDat, uint8_t vLen);										//CRC8 校验








#endif /* SHT30_H_ */
