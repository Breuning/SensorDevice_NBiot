/**
  ******************************************************************************
  * File Name          : USART.h
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __usart_H
#define __usart_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

/* USER CODE BEGIN Private defines */

#define RECEIVELEN 100

typedef struct  
{  
  uint8_t receive_flag;			//空闲接收标记  
  uint8_t dmaSend_flag;			//发送完成标记  
  uint16_t rx_len;				//接收长度
  uint8_t RX_Buf[RECEIVELEN];   //DMA接收缓存
}UART_RECEIVETYPE; 

/* USER CODE END Private defines */

void MX_UART4_Init(void);
void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);
void MX_USART3_UART_Init(void);

/* USER CODE BEGIN Prototypes */
extern UART_RECEIVETYPE NBUart_RX;
extern UART_RECEIVETYPE LoraUart_RX;
extern UART_RECEIVETYPE GasUart_RX;
extern UART_RECEIVETYPE RS485Uart_RX;

void UART_IT_IDLE_InitAll(void);
void UART_IT_IDLE_Init(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
void UARTReceive_IDLECallback(UART_HandleTypeDef *huart, UART_RECEIVETYPE *Uart_RX, uint16_t Size);

void UART1_SendString(char *str);
void UART2_SendString(char *str);
void UART1_SendByte(uint8_t data);
void UART2_SendByte(uint8_t data);
void UART1_SendData(uint8_t *pdata, uint16_t Length);
void UART2_SendData(uint8_t *pdata, uint16_t Length);
void PCUart_SendData(uint8_t *pdata, uint16_t Length);
void LoraUart_SendData(uint8_t *pdata, uint16_t Length);
extern uint8_t UsartAllowSend;
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
