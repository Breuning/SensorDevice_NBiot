/**
  ******************************************************************************
  * File Name          : CAN.c
  * Description        : This file provides code for the configuration
  *                      of the CAN instances.
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
#include "can.h"

/* USER CODE BEGIN 0 */
#include "usart.h"
/* USER CODE END 0 */

CAN_HandleTypeDef hcan;

/* CAN init function */
void MX_CAN_Init(void)
{

  hcan.Instance = CAN1;
  hcan.Init.Prescaler = 90;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_6TQ;
  hcan.Init.TimeSeg2 = CAN_BS2_1TQ;
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.AutoBusOff = DISABLE;
  hcan.Init.AutoWakeUp = DISABLE;
  hcan.Init.AutoRetransmission = DISABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspInit 0 */

  /* USER CODE END CAN1_MspInit 0 */
    /* CAN1 clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**CAN GPIO Configuration    
    PA11     ------> CAN_RX
    PA12     ------> CAN_TX 
    */
    GPIO_InitStruct.Pin = CAN_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(CAN_RX_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = CAN_TX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(CAN_TX_GPIO_Port, &GPIO_InitStruct);

    /* CAN1 interrupt Init */
    HAL_NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
  /* USER CODE BEGIN CAN1_MspInit 1 */

  /* USER CODE END CAN1_MspInit 1 */
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspDeInit 0 */

  /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN1_CLK_DISABLE();
  
    /**CAN GPIO Configuration    
    PA11     ------> CAN_RX
    PA12     ------> CAN_TX 
    */
    HAL_GPIO_DeInit(GPIOA, CAN_RX_Pin|CAN_TX_Pin);

    /* CAN1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USB_LP_CAN1_RX0_IRQn);
  /* USER CODE BEGIN CAN1_MspDeInit 1 */

  /* USER CODE END CAN1_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */


//--------------------------------------------------------------------------------------------
//	@function:    can�������ݺ���
//  @parameters:  ide (0 ��׼֡,  1 ��չ֡)    id (֡ID)     len (���ݳ���)    data (����)
//	@return:      0 (�ɹ�)   1 (ʧ��)
//--------------------------------------------------------------------------------------------
uint8_t Can_TxMessage(uint8_t ide, uint32_t id, uint8_t len, uint8_t *data)
{
	uint32_t TxMailbox;
	CAN_TxHeaderTypeDef CAN_TxHeader;
	uint8_t  FreeTxNum = 0;

	if(ide == 0)
	{
		CAN_TxHeader.IDE = CAN_ID_STD;	  //��׼֡
		CAN_TxHeader.StdId = id;
	}
	else
	{
		CAN_TxHeader.IDE = CAN_ID_EXT;	  //��չ֡
		CAN_TxHeader.ExtId = id;
	}
	CAN_TxHeader.DLC = len;
	CAN_TxHeader.RTR = CAN_RTR_DATA;      //����֡,CAN_RTR_REMOTEң��֡
	CAN_TxHeader.TransmitGlobalTime = DISABLE;

	while(FreeTxNum == 0)
	{
		FreeTxNum = HAL_CAN_GetTxMailboxesFreeLevel(&hcan);
	}

	HAL_Delay(1);                        //û����ʱ���п��ܻᷢ��ʧ��

	if(HAL_CAN_AddTxMessage(&hcan,&CAN_TxHeader,data,&TxMailbox) != HAL_OK)
		return 1;
	return 0;
}

//--------------------------------------------------------------------------------------------
//	 @function:    can�����жϻص�����
//--------------------------------------------------------------------------------------------
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_RxHeaderTypeDef CAN_RxHeader;

	uint8_t Rx_Data[8];
	uint8_t Data_Len = 0;
	uint32_t ID = 0;

	if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &CAN_RxHeader, Rx_Data) == HAL_OK)
	{
		Data_Len = CAN_RxHeader.DLC;
		if(CAN_RxHeader.IDE)
			ID = CAN_RxHeader.ExtId;
		else
			ID = CAN_RxHeader.StdId;


		HAL_UART_Transmit_DMA(&huart2, (uint8_t *)Rx_Data, sizeof(Rx_Data));      //�������ڵ�����ͨ��USART2������PC
	}
}

//--------------------------------------------------------------------------------------------
//	 @function:    CAN���ܿ������ó�ʼ��,����������(������),����Ϊ�������б������ݣ�����ʱ�����ˣ�
//--------------------------------------------------------------------------------------------
void CAN_Config_Init(void)
{
	CAN_FilterTypeDef CAN_FilterType;
	CAN_FilterType.FilterBank=0;							//ѡ���������0
	CAN_FilterType.FilterIdHigh=0x0000;                     //Ҫ���˵�ID��16λ
	CAN_FilterType.FilterIdLow=0x0000;                      //Ҫ���˵�ID��16λ
	CAN_FilterType.FilterMaskIdHigh=0x0000;            		//���������μĴ�����16λ
	CAN_FilterType.FilterMaskIdLow=0x0000;					//���������μĴ�����16λ
	CAN_FilterType.FilterFIFOAssignment=CAN_RX_FIFO0;		//�趨��ָ���������FIFOΪ0����������ID���Ĺ�����FIFO0�������У�����ֻ�ܴ����ﵼ��
	CAN_FilterType.FilterMode=CAN_FILTERMODE_IDMASK;		//ָ��������Ϊ��ʶ������λģʽ��CAN_FILTERMODE_IDLISTΪ��ʶ���б�ģʽ
	CAN_FilterType.FilterScale=CAN_FILTERSCALE_32BIT; 		//������λ��Ϊ32λ
	CAN_FilterType.FilterActivation=ENABLE;					//������ʹ��
	CAN_FilterType.SlaveStartFilterBank=14;					//ѡ��CAN���ߴӻ���ʼ�Ĺ�������Ϊ14

	if(HAL_CAN_ConfigFilter(&hcan,&CAN_FilterType)!=HAL_OK)                      //CAN���չ����������Ƿ�ɹ�
	{
		Error_Handler();
	}
	if(HAL_CAN_ActivateNotification(&hcan,CAN_IT_RX_FIFO0_MSG_PENDING)!=HAL_OK)  //���������ж������Ƿ�ɹ�
	{
		Error_Handler();
	}
	if(HAL_CAN_Start(&hcan)!=HAL_OK)                                             //����CAN�����Ƿ�ɹ�
	{
		Error_Handler();
	}
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
