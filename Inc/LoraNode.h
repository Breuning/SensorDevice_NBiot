/*
 * LoraNode.h
 *
 *  Created on: 2019年7月25日
 *      Author: Breuning
 */

#ifndef LORANODE_H_
#define LORANODE_H_



#include "usart.h"

#define LORANODE_NRST_HIGH()    HAL_GPIO_WritePin(LORANODE_NRST_GPIO_Port, LORANODE_NRST_Pin, GPIO_PIN_SET)
#define LORANODE_NRST_LOW()     HAL_GPIO_WritePin(LORANODE_NRST_GPIO_Port, LORANODE_NRST_Pin, GPIO_PIN_RESET)

#define LORANODE_WAKE_HIGH()    HAL_GPIO_WritePin(LORANODE_WAKE_GPIO_Port, LORANODE_WAKE_Pin, GPIO_PIN_SET)
#define LORANODE_WAKE_LOW()     HAL_GPIO_WritePin(LORANODE_WAKE_GPIO_Port, LORANODE_WAKE_Pin, GPIO_PIN_RESET)

#define LORANODE_MODE_HIGH()    HAL_GPIO_WritePin(LORANODE_MODE_GPIO_Port, LORANODE_MODE_Pin, GPIO_PIN_SET)
#define LORANODE_MODE_LOW()     HAL_GPIO_WritePin(LORANODE_MODE_GPIO_Port, LORANODE_MODE_Pin, GPIO_PIN_RESET)

typedef enum
{
	MODE_INT,
	MODE_CMD,		//mode pin high
	MODE_TRANSPARENT
} LoraNode_Mode_T;

typedef enum
{
	MODE_WAKEUP,
	MODE_SLEEP,
} LoraNode_SleepMode_T;

typedef struct
{
	uint8_t P2P_SF;                  //Lora通信速率(12-P2P_SF)
	uint16_t P2P_FRQ;                //Lora工作频率（MHz） 发送
	uint8_t P2P_FRQ_step;
	uint8_t P2P_PWR;                 //Lora发射功率
	uint8_t P2P_PWR_code;
	uint8_t P2P_TIME;
	uint8_t P2P_RSSI[5];             //Lora接收到的信号强度
	uint8_t P2P_SNR[5];              //Lora接收到的信噪比
} Node_Info;

extern Node_Info LoRa_Node;



void LoraNode_Init_Mode(LoraNode_Mode_T mode);
void LoraNode_Reset(void);
void LoraNode_Wake_Sleep(LoraNode_SleepMode_T mode);
uint32_t LoraNode_IO_State(uint32_t pin);
void LoraNode_Mode_Set(LoraNode_Mode_T mode);
void Int2Str(uint8_t* str, int32_t intnum);
void LoraNode_Send_AT(char *at_buf);
void LoraNode_Read(uint8_t *str);
uint8_t *StringCat(uint8_t *str, const uint8_t *string);
uint8_t *StringCat2(uint8_t *str, const uint8_t *string);
char *StringStr(char *str, char *dest);
uint32_t LoraNode_SetP2P(uint32_t f,uint8_t a,uint8_t b,uint8_t c,uint8_t d,uint8_t e,uint8_t ff,uint8_t g,uint16_t h);
uint32_t LoraNode_SetPERIPH(uint32_t f);
uint32_t LoraNode_SetMINIRF(uint32_t value);

#endif /* LORANODE_H_ */
