/*
 * Led.c
 *
 *  Created on: 2019年10月24日
 *      Author: Breuning
 */

#include "main.h"

void Led_Init(void) //初始化灯全灭
{
	HAL_GPIO_WritePin(LED1_RUNNING_GPIO_Port, LED1_RUNNING_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED2_MSGTX_GPIO_Port, LED2_MSGTX_Pin, GPIO_PIN_SET);
}

void LED1_RUNNING_Blink(void)
{
	HAL_GPIO_TogglePin(LED1_RUNNING_GPIO_Port, LED1_RUNNING_Pin);
	HAL_Delay(400);
	HAL_GPIO_TogglePin(LED1_RUNNING_GPIO_Port, LED1_RUNNING_Pin);
}

void LED2_MSGTX_Blink(void)
{
	HAL_GPIO_TogglePin(LED2_MSGTX_GPIO_Port, LED2_MSGTX_Pin);
	HAL_Delay(100);
	HAL_GPIO_TogglePin(LED2_MSGTX_GPIO_Port, LED2_MSGTX_Pin);
}

