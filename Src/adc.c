/**
  ******************************************************************************
  * File Name          : ADC.c
  * Description        : This file provides code for the configuration
  *                      of the ADC instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "adc.h"

/* USER CODE BEGIN 0 */
#include "SensorAnalysis.h"
/* USER CODE END 0 */

ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc3;
DMA_HandleTypeDef hdma_adc1;

/* ADC1 init function */
void MX_ADC1_Init(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};

  /** Common config 
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel 
  */
  sConfig.Channel = ADC_CHANNEL_9;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

}
/* ADC3 init function */
void MX_ADC3_Init(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};

  /** Common config 
  */
  hadc3.Instance = ADC3;
  hadc3.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc3.Init.ContinuousConvMode = DISABLE;
  hadc3.Init.DiscontinuousConvMode = DISABLE;
  hadc3.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc3.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc3) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel 
  */
  sConfig.Channel = ADC_CHANNEL_12;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspInit 0 */

  /* USER CODE END ADC1_MspInit 0 */
    /* ADC1 clock enable */
    __HAL_RCC_ADC1_CLK_ENABLE();
  
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**ADC1 GPIO Configuration    
    PB1     ------> ADC1_IN9 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* ADC1 DMA Init */
    /* ADC1 Init */
    hdma_adc1.Instance = DMA1_Channel1;
    hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_adc1.Init.Mode = DMA_NORMAL;
    hdma_adc1.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(adcHandle,DMA_Handle,hdma_adc1);

  /* USER CODE BEGIN ADC1_MspInit 1 */

  /* USER CODE END ADC1_MspInit 1 */
  }
  else if(adcHandle->Instance==ADC3)
  {
  /* USER CODE BEGIN ADC3_MspInit 0 */

  /* USER CODE END ADC3_MspInit 0 */
    /* ADC3 clock enable */
    __HAL_RCC_ADC3_CLK_ENABLE();
  
    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**ADC3 GPIO Configuration    
    PC2     ------> ADC3_IN12 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* USER CODE BEGIN ADC3_MspInit 1 */

  /* USER CODE END ADC3_MspInit 1 */
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspDeInit 0 */

  /* USER CODE END ADC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC1_CLK_DISABLE();
  
    /**ADC1 GPIO Configuration    
    PB1     ------> ADC1_IN9 
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_1);

    /* ADC1 DMA DeInit */
    HAL_DMA_DeInit(adcHandle->DMA_Handle);
  /* USER CODE BEGIN ADC1_MspDeInit 1 */

  /* USER CODE END ADC1_MspDeInit 1 */
  }
  else if(adcHandle->Instance==ADC3)
  {
  /* USER CODE BEGIN ADC3_MspDeInit 0 */

  /* USER CODE END ADC3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC3_CLK_DISABLE();
  
    /**ADC3 GPIO Configuration    
    PC2     ------> ADC3_IN12 
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_2);

  /* USER CODE BEGIN ADC3_MspDeInit 1 */

  /* USER CODE END ADC3_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */

//--------------------------------------------------------------------------------------------
//	 @function:获取ADC采集数据
//--------------------------------------------------------------------------------------------
uint16_t ADC_ReadData(ADC_HandleTypeDef* hadc)
{
    static uint16_t ad_data;

    HAL_ADC_Start(hadc);						 //软件开始ADC转换
    HAL_ADC_PollForConversion(hadc,100);	     //等待ADC转换完成，超时时间为100ms
    ad_data=(uint16_t)HAL_ADC_GetValue(hadc);    //调用HAL_ADC_GetValue获取采样值赋给ad_data
    HAL_ADC_Stop(hadc);						     //软件停止ADC转换
    HAL_Delay(5);
    return ad_data;
}

//--------------------------------------------------------------------------------------------
//	 @function:获取ADC采集数据平均值
//--------------------------------------------------------------------------------------------
uint16_t ADC_Average(ADC_HandleTypeDef* hadc, uint8_t times)
{
	uint16_t sum=0,i,adc_average;
    for(i=0;i<times;i++)
    {
    	sum+=ADC_ReadData(hadc);
    }
    adc_average=sum/times;
    return adc_average;
}

//--------------------------------------------------------------------------------------------
//	 @function:获取负压探头数值 (PB1-ADC1 电压信号)
//--------------------------------------------------------------------------------------------
void GetValidDataFromPressure(void)
{
	float v;
	float p;

    v = (float)ADC_Average(&hadc1, 10);          //10次数据的平均值
    v = v * 3.3/4095.0;			                 //采集到的实际电压值

    if(v < 0.4)               //电压如果小于0.64V认为探头离线
    	return;
    else
    	Tick_GetSensorData = HAL_GetTick();

    p = (v - 0.64) * ((100 - 0) / (3.2 - 0.64)); //电压值转换为相应的负压值  4-20mA,160R对应0.64-3.2V

    if(p > 100)
    	p = 100;
    if(p < 0)
    	p = 0;

    Sensor_Data.NegativePressure = (uint16_t)p;

}

//--------------------------------------------------------------------------------------------
//	 @function:获取小窗位移探头数值  (PB1-ADC1 电压信号)
//--------------------------------------------------------------------------------------------
void GetValidDataFromWindowPos(void)
{
	float v = 0;
	float l = 0;

	v =(float)ADC_Average(&hadc1, 10);      		            //10次数据的平均值
	v = v * 3.3 / 4095.0;                                       //采集到的实际电压值

    if(v < 0.4)                 //电压如果小于0.64V认为探头离线
    	return;
    else
    {
    	Tick_GetSensorData = HAL_GetTick();
    }

	l = (((v / 160) * 1000 )- 4) * ((1000 - 0) / (20 - 4));     //电压值转换为相应的位移值

	l = l + 20;             //实际测量校正，拉线头有20mm

//    if(l > 600)
//    {
//    	l = 600;
//    }
    if(l < 0)
    {
    	l = 0;
    }

	Sensor_Data.WindowPosition = (uint16_t)l;
}

//--------------------------------------------------------------------------------------------
//	 @function:获取新小窗推杆位移探头数值 (PC2-ADC3 电阻信号)
//--------------------------------------------------------------------------------------------
void Get_WindowPos_NewPushrod(void)
{

	float ret = 0;
	float r = 0;

	ret = (float)ADC_Average(&hadc3, 10);                   //10次数据的平均值
	ret = (float)(ret * 3.3 / 4095.0);                      //采集到的实际电压值
	ret = NTC_PUSH_UP_RESISTOR_KR  / (3.3/ret - 1);			//电压值转化为电阻值

	r = 600 / (8.56 - 0.23) * (ret - 0.23);                 //电阻值转换为相应的位移值

	if(r > 600)
	{
		r = 600;
	}
	if(r < 0)
	{
		r = 0;
	}

	Tick_GetSensorData = HAL_GetTick();
	Sensor_Data.WindowPosition = (uint16_t)r;

}



/*
static float calPostionPercent(uint32_t origin,ref_stype_t type)
{
	hcfg_t *hcfg = CFG_get();

	float ret = 0;
	double r = 0;

	if(type == ref_SENSOR_POSITION1)
	{
		//计算电压
		ret =  ((float)origin / 4096.0 * 3.3);
		//计算电阻
		r = ntc_PUSH_UP_RESISTOR_KR  / (3.3/ret - 1);
		//计算电阻增量
		r -= hcfg->sw.align.Base1;
		//计算电阻变化的百分比
		ret = r / (hcfg->sw.align.full1 - hcfg->sw.align.Base1) * 100.0;
	}
	else if (type == ref_SENSOR_POSITION2)
	{
		ret =  ((float)origin / 4096.0 * 3.3);
		r = ntc_PUSH_UP_RESISTOR_KR  / (3.3/ret - 1);
		r -= hcfg->sw.align.Base2;
		ret = r / (hcfg->sw.align.full2 - hcfg->sw.align.Base2) * 100.0;
	}

	if(ret < 0)
	{
		ret =0;
	}
	if(ret > 100)
	{
		ret = 100;
	}

	if(hcfg->sw.align.revDir)
	{
		return ret;
	}
	else
	{
		return  100.0 - ret;;
	}
}

#define ntc_PUSH_UP_RESISTOR_KR (float)10.0
*/


/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
