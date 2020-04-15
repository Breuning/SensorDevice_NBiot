/*
 * LoraAnalysis.c
 *
 *  Created on: 2019年7月25日
 *      Author: Breuning
 */

#include "LoraAnalysis.h"
#include "LoraNode.h"
#include "usart.h"

#include "stm32f1xx_hal.h"

LORA_SATE Lora_Sate =  P2P_MODE;

uint8_t P2P_TEST[8]={0x02,0x0b,0x02,0x04,0x02};
uint8_t P2P_rece[50]={0};
uint8_t P2P_lenth = 0;
uint8_t LoraNode_Slave_Master = 0;
uint8_t num=0;

void LoraAnalysis(void)
{

        switch(Lora_Sate)
        {
                case CMD_MODE:
                        LoraNode_Wake_Sleep(MODE_WAKEUP);
                        LoraNode_Mode_Set(MODE_CMD);                             //MODE引脚拉高，进入并驻留在命令模式
                        HAL_Delay(50);

//                      LoraNode_SetMINIRF(0);                                   //发送AT+MINIRF=0，即设置工作模式为LoraWAN模式
                        HAL_Delay(100);

                        if(LoraUart_RX.receive_flag == 1)
                        {
                                LoraUart_RX.receive_flag = 0;
                                PCUart_SendData(LoraUart_RX.RX_Buf,LoraUart_RX.rx_len);
                        }

                        if(NBUart_RX.receive_flag == 1)
                        {
                                NBUart_RX.receive_flag = 0;
                                LoraUart_SendData(NBUart_RX.RX_Buf,NBUart_RX.rx_len);
                        }
                        break;

                 case TRANSPARENT_MODE:
                        LoraNode_Wake_Sleep(MODE_WAKEUP);
                        LoraNode_Mode_Set(MODE_CMD);


                        break;


                 case P2P_MODE:


						if(num == 0)
						{
							LoraNode_Wake_Sleep(MODE_WAKEUP);

							LoraNode_Mode_Set(MODE_CMD);               //MODE引脚拉高，进入并驻留在命令模式

//							LoraNode_SetPERIPH(1);					   //使能P1\P2引脚脉冲输出功能
//							HAL_Delay(100);

//							LoRa_Node.P2P_FRQ = 4700;      	  		   //Lora工作频率（MHz）
//							LoRa_Node.P2P_PWR = 20;                    //Lora发射功率
//							LoRa_Node.P2P_SF  = 4;                     //Lora通信速率(12-P2P_SF)
//							LoraNode_SetP2P(LoRa_Node.P2P_FRQ*100000,1,LoRa_Node.P2P_PWR,12-LoRa_Node.P2P_SF,0,1,0,0,2600);  //设置miniRF模式射频参数

							LoraNode_SetP2P(475300000,1,20,9,0,1,0,0,0);
							HAL_Delay(100);

							LoraNode_SetMINIRF(1);                    //发送AT+MINIRF=1，设置工作模式为私有透传模式
							HAL_Delay(50);

	                        LoraNode_Mode_Set(MODE_TRANSPARENT);      //MODE引脚拉低，进入并驻留在透传模式
	                        HAL_Delay(50);

							num = 1;
						 }




                        //主机
                        if(LoraNode_Slave_Master == 1)
                        {
                                //进行发送
//								if(NBUart_RX.receive_flag == 1)
//								{
//										NBUart_RX.receive_flag = 0;
//										LoraUart_SendData(NBUart_RX.RX_Buf,NBUart_RX.rx_len);
//								}
                                 LoraUart_SendData(P2P_TEST,8);
                                 HAL_Delay(3000);


                                 //进行接收
                                 if(LoraUart_RX.receive_flag != 0)
                                 {
                                        LoraUart_RX.receive_flag = 0;
                                        PCUart_SendData(LoraUart_RX.RX_Buf,LoraUart_RX.rx_len);
//                                        if(LoraUart_RX.rx_len == 8)
//                                        {
//                                                LoraNode_Mode_Set(MODE_CMD);            //MODE引脚拉高，进入并驻留在命令模式
//                                                HAL_Delay(10);
//
//                                                LoraNode_SetMINIRF(0);
//                                                LoraNode_GetSNR(LoRa_Node_str);
//
//                                                LoraNode_SetMINIRF(1);                  //发送AT+MINIRF=1，设置工作模式为私有透传模式
//                                                LoraNode_Mode_Set(MODE_TRANSPARENT);    //MODE引脚拉低，进入并驻留在透传模式
//                                                HAL_Delay(10);
//                                         }
                                 }
                        }


                        //从机
                        if(LoraNode_Slave_Master == 0)
                        {
                                if(LoraUart_RX.receive_flag == 1)
                                {
                                        LoraUart_RX.receive_flag = 0;
                                        PCUart_SendData(LoraUart_RX.RX_Buf,LoraUart_RX.rx_len);


//                                        P2P_lenth = LoraUart_RX.rx_len;
//                                        for(uint8_t jj=0;jj<P2P_lenth;jj++)
//                                        {
//                                                P2P_rece[jj] = LoraUart_RX.RX_Buf[jj];
//                                        }

//                                        LoraNode_Mode_Set(MODE_CMD);
//                                        HAL_Delay(10);

//                                        LoraNode_SetMINIRF(0);
//                                        LoraNode_GetSNR(LoRa_Node_str);
//
//                                        LoraNode_SetMINIRF(1);
//                                        LoraNode_Mode_Set(MODE_TRANSPARENT);
//                                        HAL_Delay(10);

//                                        PCUart_SendData(P2P_rece,P2P_lenth);
                                }

                        }
                        break;
                default :break;

        }

}
