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
	HAL_GPIO_WritePin(LED2_NETWORK_GPIO_Port, LED2_NETWORK_Pin, GPIO_PIN_SET);
}

void LED1_RUNNING_Blink(void)
{
	HAL_GPIO_TogglePin(LED1_RUNNING_GPIO_Port, LED1_RUNNING_Pin);
	HAL_Delay(400);
	HAL_GPIO_TogglePin(LED1_RUNNING_GPIO_Port, LED1_RUNNING_Pin);
}


void LED2_NETWORK_Searching(void)
{
	HAL_GPIO_WritePin(LED2_NETWORK_GPIO_Port, LED2_NETWORK_Pin, GPIO_PIN_RESET);
	HAL_Delay(60);
	HAL_GPIO_WritePin(LED2_NETWORK_GPIO_Port, LED2_NETWORK_Pin, GPIO_PIN_SET);
	HAL_Delay(600);
}

void LED2_NETWORK_Registered(void)
{
	HAL_GPIO_WritePin(LED2_NETWORK_GPIO_Port, LED2_NETWORK_Pin, GPIO_PIN_RESET);
	HAL_Delay(600);
	HAL_GPIO_WritePin(LED2_NETWORK_GPIO_Port, LED2_NETWORK_Pin, GPIO_PIN_SET);
	HAL_Delay(60);
}

void LED2_NETWORK_DataTransfer(void)
{
	uint8_t i;
	for(i = 0; i < 6; i++)
	{
		HAL_GPIO_TogglePin(LED2_NETWORK_GPIO_Port, LED2_NETWORK_Pin);
		HAL_Delay(125);
		HAL_GPIO_TogglePin(LED2_NETWORK_GPIO_Port, LED2_NETWORK_Pin);
		HAL_Delay(125);
	}
}

