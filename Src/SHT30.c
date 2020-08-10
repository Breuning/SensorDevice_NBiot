/*
 * SHT30.c
 *
 *  Created on: 2019年8月2日
 *      Author: Breuning
 */
#include <string.h>
#include "stm32f1xx_hal.h"
#include "SHT30.h"
#include "tim.h"
#include "SensorTask.h"
#include "FilterAlgorithm.h"
#include "i2c.h"


uint8_t SHT30_Modecommand_Buffer[2]={0x20, 0x32}; 			//每秒进行0.5次高精度测量命令
uint8_t SHT30_Fetchcommand_Buffer[2]={0xE0,0x00};           //读取测量结果命令
uint8_t SHT30_Data_Buffer[6] = {0};                         //Byte0和1为温度,Byte2为温度CRC校验值; Byte3和4为湿度,Byte5为温度CRC校验值


void SHT30_Init(void)
{//初始化移到循环
 //	HAL_I2C_Master_Transmit(&hi2c1, SHT30Addr_Write, SHT30_Modecommand_Buffer, 2, 1000);      //SHT30传感器周期性的进行温湿度转换
}

void GetValidDataFromSHT30(void)
{
	MX_I2C1_Init();
	HAL_I2C_Master_Transmit(&hi2c1, SHT30Addr_Write, SHT30_Modecommand_Buffer, 2, 1000);
	HAL_Delay(5);
	HAL_I2C_Master_Transmit(&hi2c1, SHT30Addr_Write, SHT30_Fetchcommand_Buffer, 2, 1000);     //发送获取传感器数据的命令
	HAL_I2C_Master_Receive (&hi2c1, SHT30Addr_Read, SHT30_Data_Buffer, 6, 1000);			  //接收传感器返回的数据值放到SHT30_Data_Buffer

	if(strlen(SHT30_Data_Buffer) != 0)
		Tick_GetSensorData = HAL_GetTick();
	else
		return;

	SHT30_Calc(SHT30_Data_Buffer, &Sensor_Data.Temperature_Flag, &Sensor_Data.Temperature_u, &Sensor_Data.Humidity);

	memset(SHT30_Data_Buffer, 0 , sizeof(SHT30_Data_Buffer));
}

/*---------------------------------------------------------------------
 功能描述: SHT30 测量结果计算
 参数说明:  vBuf [in] - 测量读取结果
 	 	   vTemSymbol [out] - 返回温度正负符号
 	 	   vTem [out] - 温度
 	 	   vHum [out] - 湿度
 函数返回: 无
 ---------------------------------------------------------------------*/
void SHT30_Calc(uint8_t *vBuf, uint8_t *vTemSymbol, uint16_t *vTem, uint16_t *vHum)
{

	uint8_t vCrc = 0x00;
	float vTemp = 0.00;

	//温度
	vCrc = SHT30_CheckCrc8(vBuf, 2);
	if (vCrc == vBuf[2])  //Crc校验
	{
		vTemp = 175.0 * ((vBuf[0] << 8) + vBuf[1]) / (65536.0 - 1.0);
		if(vTemp >= 45)
		{
			*vTemSymbol = 1;                     //正温度值
			*vTem = (vTemp - 45.0) * 10.0;       //得到摄氏度温度值x10
		}
		if(vTemp < 45)
		{
			*vTemSymbol = 0;                     //负温度值
			*vTem = (45.0 - vTemp) * 10.0;       //得到摄氏度温度值x10
		}
	}


	//湿度
	vBuf += 3;
	vCrc = SHT30_CheckCrc8(vBuf, 2);
	if (vCrc == vBuf[2])  //Crc校验
	{
		vTemp = 100.0 * ((vBuf[0] << 8) + vBuf[1]) / (65536.0 - 1.0);
		*vHum = (vTemp * 10);				 //得到相对湿度值x10
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


//SHT30测量结果crc计算
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








/*****************************GPIO模拟IIC驱动程序(SHT30)***********************************/

///*---------------------------------------------------------------------
// 功能描述: SHT30 测量结果计算
// 参数说明:  vTemSymbol [out] - 返回温度符号
// vTem [out] - 温度
// vHum [out] - 湿度
//
// 函数返回: 无
// ---------------------------------------------------------------------*/
//void SHT30_Get_TH(uint8_t *vTemSymbol, uint16_t *vTem, uint16_t *vHum)
//{
//	uint8_t vDat[8];
//	uint8_t vRval = 0;
//
//	vRval = SHT30_Single_Measure(vDat);                 //SHT30单次测量,將測量結果讀取至vDat[8];
//	if (!vRval)
//	SHT30_calc(vDat, vTemSymbol, vTem, vHum);       	//將vDat内的数据分开存放，温度数据放入vTem，湿度数据放入vHum
//}
//
///*---------------------------------------------------------------------
// 功能描述: SHT30单次测量
// 参数说明: vBuf [out] - 测量读取结果
// 函数返回: 0 - 成功  大于1出错
// ---------------------------------------------------------------------*/
//uint8_t SHT30_Single_Measure(uint8_t *vBuf)
//{
//	uint8_t vRval = 0;
//	uint8_t i = 0;
//
//	SHT30_Start();
//
//	vRval |= SHT30_SendByte(SHT30_SlaveAddress + 0);					 //地址写
//	if (!vRval)
//		vRval |= SHT30_SendByte((SHT30_SINGLE_H_MEASURE_EN >> 8) & 0xFF);//使能高精度采集
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
//		vRval |= SHT30_SendByte(SHT30_SlaveAddress + 1);				//地址读
//
//	if (vRval)
//		return vRval;
//
//	for (i = 0; i < 6; i++)
//	{
//		vBuf[i] = SHT30_RecvByte();                	                    //存储数据
//		if (i == 0x06)
//		{
//
//			SHT30_SendACK(1);                                           //最后一个数据需要回NOACK
//		}
//		else
//		{
//			SHT30_SendACK(0);                                           //回应ACK
//		}
//	}
//
//	SHT30_Stop();
//
//	SHT30_Start();
//	if (!vRval)
//		vRval |= SHT30_SendByte(SHT30_SlaveAddress + 0);				//地址写
//	if (!vRval)
//		vRval |= SHT30_SendByte((SHT30_SINGLE_H_MEASURE_DIS >> 8) & 0xFF);//关闭高精度采集
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
// 功能描述：函数初始化
// 参    数：无
// 返 回 值：无
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
//	vRval |= SHT30_SendByte(SHT30_SlaveAddress + 0);			//地址写
//	vRval |= SHT30_SendByte(0x37);			                    //读取状态寄存器
//	vRval |= SHT30_SendByte(0x80);
//
//	SHT30_Start();
//	vRval |= SHT30_SendByte(SHT30_SlaveAddress + 1);			//地址读
//
//	for (i = 0; i < 3; i++)
//	{
//		vBuf[i] = SHT30_RecvByte();                				//存储数据
//		if (i == 0x03)
//		{
//			SHT30_SendACK(1);                         			//最后一个数据需要回NOACK
//		}
//		else
//		{
//			SHT30_SendACK(0);                         			//回应ACK
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
// 功能描述: 起始信号
// 参数说明: 无
// 函数返回: 无
// ---------------------------------------------------------------------*/
//void SHT30_Start()
//{
//	SHT30_SDA0_H;                         //拉高数据线
//	SHT30_Delayus(1);
//	SHT30_SCL0_H;                         //拉高时钟线
//	SHT30_Delayus(1);                     //延时
//	SHT30_SDA0_L;                         //产生下降沿
//	SHT30_Delayus(10);                     //延时
//	SHT30_SCL0_L;                         //拉低时钟线
//	SHT30_Delayus(10);
//}
//
///*---------------------------------------------------------------------
// 功能描述: 停止信号
// 参数说明: 无
// 函数返回: 无
// ---------------------------------------------------------------------*/
//void SHT30_Stop()
//{
//	SHT30_SCL0_L;                         //拉低时钟线
//	SHT30_Delayus(1);
//	SHT30_SDA0_L;                         //拉低数据线
//	SHT30_Delayus(1);                     //延时
//	SHT30_SCL0_H;
//	SHT30_Delayus(10);                     //延时
//	SHT30_SDA0_H;                         //产生上升沿
//	SHT30_Delayus(10);
//}
//
///*---------------------------------------------------------------------
// 功能描述: 发送应答信号
// 参数说明: ack [in] - 应答信号(0:ACK 1:NAK)
// 函数返回: 无
// ---------------------------------------------------------------------*/
//void SHT30_SendACK(uint8_t ack)
//{
//	if (ack & 0x01)
//		SHT30_SDA0_H;		              //写应答信号
//	else
//		SHT30_SDA0_L;
//
//	SHT30_Delayus(1);
//	SHT30_SCL0_H;                        //拉高时钟线
//	SHT30_Delayus(5);                    //延时
//	SHT30_SCL0_L;                        //拉低时钟线
//	SHT30_SDA0_H;
//	SHT30_Delayus(20);                   //延时
//}
//
///*---------------------------------------------------------------------
// 功能描述: 接收应答信号
// 参数说明: 无
// 函数返回: 返回应答信号
// ---------------------------------------------------------------------*/
//uint8_t SHT30_RecvACK()
//{
//	uint8_t CY = 0x00;
//	uint16_t vConter = 1000;
//	SHT30_SDA0_H;
//
////	I2C_SDA_IN();
//
//	SHT30_SCL0_H;                             //拉高时钟线
//	SHT30_Delayus(1);                         //延时
//	while (vConter)
//	{
//		vConter--;
//		CY |= SHT30_SDA0_DAT;                 //读应答信号
//
//		if (!CY)
//			break;
//	}
//	SHT30_Delayus(20);                        //延时
//	SHT30_SCL0_L;
////	I2C_SDA_OUT();
//
//	return CY;
//}
//
///*---------------------------------------------------------------------
// 功能描述: 向IIC总线发送一个字节数据
// 参数说明: dat [in] - 写字节
// 函数返回: 0 - 成功   1- 出错
// ---------------------------------------------------------------------*/
//uint8_t SHT30_SendByte(uint8_t dat)
//{
//	uint8_t vRval = 0x01;
//	uint8_t i;
//
//	for (i = 0; i < 8; i++)         		//8位计数器
//	{
//		if (dat & 0x80)
//			SHT30_SDA0_H;
//		else
//			SHT30_SDA0_L;               	//送数据口
//
//		SHT30_Delayus(1);             		//延时
//		SHT30_SCL0_H;                		//拉高时钟线
//		SHT30_Delayus(5);             		//延时
//		SHT30_SCL0_L;                		//拉低时钟线
//		SHT30_Delayus(1);             		//延时
//		dat <<= 1;              			//移出数据的最高位
//	}
//
//	vRval = SHT30_RecvACK();
//
//	return vRval;
//}
//
///*---------------------------------------------------------------------
// 功能描述: 从IIC总线接收一个字节数据
// 参数说明: 无
// 函数返回: 接收字节
// ---------------------------------------------------------------------*/
//uint8_t SHT30_RecvByte()
//{
//	uint8_t i;
//	uint8_t dat = 0;
//	uint16_t vConter = 1000;
//
//	SHT30_SDA0_H;                         	//使能内部上拉,准备读取数据,
//
////	I2C_SDA_IN();
//	for (i = 0; i < 8; i++)         	    //8位计数器
//	{
//		SHT30_SCL0_H;                       //拉高时钟线
//		SHT30_Delayus(1);             		//延时
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
//		dat |= SHT30_SDA0_DAT;              //读数据
//		SHT30_SCL0_L;                       //拉低时钟线
//		SHT30_Delayus(1);             		//延时
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
// 功能描述: SHT30传感器软复位 (周期模式下关闭周期运行)
// 参数说明: 无
// 函数返回: 0 - 成功  大于1出错
// ---------------------------------------------------------------------*/
//uint8_t SHT30_Soft_Reset()
//{
//	uint8_t vRval = 0;
//
//	SHT30_Start();
//
//	if (!vRval)
//		vRval |= SHT30_SendByte(SHT30_SlaveAddress + 0);				//地址写
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
// 功能描述: SHT30清除状态寄存器
// 参数说明: 无
// 函数返回: 0 - 成功  大于1出错
// ---------------------------------------------------------------------*/
//uint8_t SHT30_ClearStaus()
//{
//	uint8_t vRval = 0;
//	SHT30_Start();
//
//	if (!vRval)
//		vRval |= SHT30_SendByte(SHT30_SlaveAddress + 0);				//地址写
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
// 功能描述: SHT30设置周期测量
// 参数说明: vBuf [in] - 测量命令
// 函数返回: 0 - 成功  大于1出错
// ---------------------------------------------------------------------*/
//uint8_t SHT30_Periodic_Measure(uint16_t vCmd)
//{
//	uint8_t vRval = 0;
//
//	SHT30_Start();
//	vRval |= SHT30_SendByte(SHT30_SlaveAddress + 0);					//地址写
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
// 功能描述: SHT30周期测量 读取数据
// 参数说明: vBuf [out] - 测量命令
// 函数返回: 0 - 成功  大于1出错
// ---------------------------------------------------------------------*/
//uint8_t SHT30_Periodic_Measure_Read(uint8_t *vBuf)
//{
//	uint8_t vRval = 0;
//
//	uint8_t i = 0;
//
//	SHT30_Start();
//	vRval |= SHT30_SendByte(SHT30_SlaveAddress + 0);					    //地址写
//	if (!vRval)
//		vRval |= SHT30_SendByte((SHT30_PERIODOC_MEASURE_READ >> 8) & 0xFF);	//读取周期采集数据
//	if (!vRval)
//		vRval |= SHT30_SendByte((SHT30_PERIODOC_MEASURE_READ) & 0xFF);
//
//	SHT30_Start();
//	if (!vRval)
//		vRval |= SHT30_SendByte(SHT30_SlaveAddress + 1);					//地址读
//
//	if (vRval)
//		return vRval;
//
//	for (i = 0; i < 6; i++)
//	{
//		vBuf[i] = SHT30_RecvByte();                							//存储数据
//		if (i == 0x06)
//		{
//
//			SHT30_SendACK(1);                         						//最后一个数据需要回NOACK
//		}
//		else
//		{
//			SHT30_SendACK(0);                        						//回应ACK
//		}
//	}
//
//	SHT30_Stop();
//
//	return vRval;
//}
//
///*---------------------------------------------------------------------
// 功能描述: SHT30加热命令
// 参数说明: vCmd [in] - 0x306D打开加热  0x3066关闭加热
// 函数返回: 0 - 成功  大于1出错
// ---------------------------------------------------------------------*/
//uint8_t SHT30_Heater(uint16_t vCmd)
//{
//	uint8_t vRval = 0;
//
//	SHT30_Start();
//
//	if (!vRval)
//		vRval |= SHT30_SendByte(SHT30_SlaveAddress + 0);				//地址写
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
// 功能描述: SHT30获取状态寄存器
// 参数说明: 无
// 函数返回: 返回状态寄存器
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
//		vRval |= SHT30_SendByte(SHT30_SlaveAddress + 0);				//地址写
//	if (!vRval)
//		vRval |= SHT30_SendByte((SHT30_READ_STATUS >> 8) & 0xFF);		//读取状态寄存器
//	if (!vRval)
//		vRval |= SHT30_SendByte((SHT30_READ_STATUS) & 0xFF);
//
//	SHT30_Start();
//	if (!vRval)
//		vRval |= SHT30_SendByte(SHT30_SlaveAddress + 1);				//地址读
//
//	if (vRval > 0)
//	{
//		return 0xFFFF;
//	}
//
//	for (i = 0; i < 3; i++)
//	{
//		vBuf[i] = SHT30_RecvByte();                						//存储数据
//		if (i == 0x03)
//		{
//
//			SHT30_SendACK(1);                         					//最后一个数据需要回NOACK
//		}
//		else
//		{
//			SHT30_SendACK(0);                         					//回应ACK
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




