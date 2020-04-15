/*
 * LoraNode.c
 *
 *  Created on: 2019年7月25日
 *      Author: Breuning
 */

#include "LoraNode.h"
#include "main.h"
#include "usart.h"
#include "string.h"

#define RXLEN	256
uint8_t AT_Data_buf[RXLEN];

Node_Info LoRa_Node;

//--------------------------------------------------------------------------------------------
//	@funtion:	  initialization the lora module mode, command mode or transparent mode
//	@parameters:  MODE_CMD, or MODE_TRANSPARENT
//	@return:	  void
//--------------------------------------------------------------------------------------------
void LoraNode_Init_Mode(LoraNode_Mode_T mode)
{
	LoraNode_Reset();
        HAL_Delay(150);

	LoraNode_Wake_Sleep(MODE_WAKEUP);
        HAL_Delay(50);

	LoraNode_Mode_Set(mode);
}

//--------------------------------------------------------------------------------------------
//	@funtion:	  reset the lora module
//	@parameters:  void
//	@return:	  void
//--------------------------------------------------------------------------------------------
void LoraNode_Reset(void)
{
	LORANODE_NRST_LOW();
	HAL_Delay(50);
	LORANODE_NRST_HIGH();
}

//--------------------------------------------------------------------------------------------
//	@funtion:	  set the lora module mode, wakeup or sleep mode
//	@parameters:  MODE_WAKEUP, or MODE_SLEEP
//	@return:	  void
//--------------------------------------------------------------------------------------------
void LoraNode_Wake_Sleep(LoraNode_SleepMode_T mode)
{
	if (mode == MODE_WAKEUP)
	{
		/* wake signal, high for module wakeup, low for module sleep */
		if (HAL_GPIO_ReadPin(LORANODE_WAKE_GPIO_Port, LORANODE_WAKE_Pin) != GPIO_PIN_SET)
		{
			/* wake module first, and wait 10 ms */
			LORANODE_WAKE_HIGH();
			HAL_Delay(10);
		}
	}
	if (mode == MODE_SLEEP)
	{
		if (HAL_GPIO_ReadPin(LORANODE_WAKE_GPIO_Port, LORANODE_WAKE_Pin) != GPIO_PIN_RESET)
		{
			LORANODE_WAKE_LOW();
		}
	}
}
/*
uint32_t LoraNode_IO_State(uint32_t pin)
{
	return HAL_GPIO_ReadPin(LORANODE_GPIO_Port, pin);	//读取引脚值
}
*/

//--------------------------------------------------------------------------------------------
//	@funtion:	  set the lora module mode, command mode or transparent mode
//	@parameters:  MODE_CMD, or MODE_TRANSPARENT
//	@return:	  void
//--------------------------------------------------------------------------------------------
void LoraNode_Mode_Set(LoraNode_Mode_T mode)
{
	if (mode == MODE_CMD)
		LORANODE_MODE_HIGH();
	else if (mode == MODE_TRANSPARENT)
		LORANODE_MODE_LOW();
}


//--------------------------------------------------------------------------------------------
//	@function:   AT指令 设置 miniRF工作模式
//	@parameters: 0, 1
//	@return:     0  success ,  -1 bad paramter
//--------------------------------------------------------------------------------------------
uint32_t LoraNode_SetMINIRF(uint32_t value)
{
	uint8_t MINI[20] = "AT+MINIRF=";
	uint8_t buf[10] = {0};
	char *temp = "OK";

	Int2Str(buf, value);                   //将整数value转换为字符串buf
	StringCat(MINI, buf);                  //连接MINI和buf两个字符串

	memset(AT_Data_buf,0,RXLEN);	       //清空缓存区AT_Data_buf

	LoraNode_Send_AT(MINI);
	LoraNode_Read(AT_Data_buf);	       	   //接收AT指令的返回值放到AT_Data_buf中

	if(StringStr((char *)AT_Data_buf, temp) != NULL)
	{
		return 0;
	}
	return -1;
}

//--------------------------------------------------------------------------------------------
//	@function:    AT指令 设置 miniRF模式下的射频参数
//	@parameters:  0, 1
//	@return:      0 success,  -1 bad paramter
//--------------------------------------------------------------------------------------------
uint32_t LoraNode_SetRADIO(uint32_t f)
{
	static uint8_t SetRADIO[40] = "AT+RADIO=";
	uint8_t buf[10] = {0};

	char *temp = "OK";

	Int2Str(buf, f);
	StringCat(SetRADIO, buf);

	memset(AT_Data_buf,0,RXLEN);	       //清空缓存区AT_Data_buf

	LoraNode_Send_AT(SetRADIO);

	LoraNode_Read(AT_Data_buf);	     	   //接收AT指令的返回值放到AT_Data_buf中

	if(StringStr((char *)AT_Data_buf, temp) != NULL)
	{
		return 0;
	}

	return -1;
}

//--------------------------------------------------------------------------------------------
//	@function:    AT指令 使能或关闭P1\P2引脚脉冲输出功能
//  @parameters:  0, 1
//	@return:      0 success,  -1 bad paramter
//--------------------------------------------------------------------------------------------
uint32_t LoraNode_SetPERIPH(uint32_t f)
{
	static uint8_t SetPERIPH[40] = "AT+PERIPH=1,";
	uint8_t buf[10] = {0};

	char *temp = "OK";

	Int2Str(buf, f);
	StringCat(SetPERIPH, buf);

	memset(AT_Data_buf,0,RXLEN);	       //清空缓存区AT_Data_buf

	LoraNode_Send_AT(SetPERIPH);

	LoraNode_Read(AT_Data_buf);	     	   //接收AT指令的返回值放到AT_Data_buf中

	if(StringStr((char *)AT_Data_buf, temp) != NULL)
	{
		return 0;
	}

	return -1;
}

//--------------------------------------------------------------------------------------------
//	 @function:
//--------------------------------------------------------------------------------------------
//void LoraNode_GetSNR(Node_Info *LoRa_temp)
//{
//	char GetSNR[] = "AT+SNR?\r\n";
//	char *temp = ",";
//	char *temp1 = "]";
//	char *ptr = NULL;
//	char *ptr1 = NULL;
//
//	memset(AT_Data_buf,0,RXLEN);
//
//	LoraNode_Send_AT(GetSNR);
//
//	LoraNode_Read(AT_Data_buf);
//
//	if((ptr1 = StringStr((char *)AT_Data_buf, temp1)) != NULL)
//	{
//		for(uint8_t i = 0;i<5;i++)
//		{
//			if(*(ptr1+i+3) == 0x2c)
//			{
//				break;
//			}else
//				{
//					LoRa_temp->P2P_RSSI[i] = *(ptr1+i+3);
//				}
//		}
//	}
//
//	if((ptr = StringStr((char *)AT_Data_buf, temp)) != NULL)
//	{
//		for(uint8_t i = 0;i<5;i++)
//		{
//			if(*(ptr+i+2) == 0x0d)
//			{
//				break;
//			}else
//				{
//					LoRa_temp->P2P_SNR[i] = *(ptr+i+2);
//				}
//		}
//
//	}
//
//}




//--------------------------------------------------------------------------------------------
//	 @function:    向Lora模块发送AT指令
//--------------------------------------------------------------------------------------------
void LoraNode_Send_AT(char *at_buf)
{
	UART1_SendString(at_buf);
}
//--------------------------------------------------------------------------------------------
//	 @function:    接收AT指令的返回值
//--------------------------------------------------------------------------------------------
void LoraNode_Read(uint8_t *str)
{
        uint32_t i = 0;
	HAL_Delay(200);

	if(LoraUart_RX.receive_flag == 1)
	{
		for(i=0 ; i < LoraUart_RX.rx_len; i++)
		{
			str[i] = LoraUart_RX.RX_Buf[i];
		}

		LoraUart_RX.receive_flag = 0;
		LoraUart_RX.rx_len = 0;
	}
}

//--------------------------------------------------------------------------------------------
//	 @function:    AT指令 设置点对点 miniRF模式下的射频参数
//--------------------------------------------------------------------------------------------
uint32_t LoraNode_SetP2P(uint32_t f,uint8_t a,uint8_t b,uint8_t c,uint8_t d,uint8_t e,uint8_t ff,uint8_t g,uint16_t h)
{
    static uint8_t SetDebug[50] = "AT+RADIO=";
	uint8_t buf [10] = {0};
	uint8_t buf1[10] = {0};
	uint8_t buf2[10] = {0};
	uint8_t buf3[10] = {0};
	uint8_t buf4[10] = {0};
	uint8_t buf5[10] = {0};
	uint8_t buf6[10] = {0};
	uint8_t buf7[10] = {0};
	uint8_t buf8[10] = {0};

	uint8_t dou[2] = ",";
	char *temp = "OK";

	Int2Str(buf, f);
	StringCat2(SetDebug, buf);
	StringCat2(SetDebug, dou);

	Int2Str(buf1, a);
	StringCat2(SetDebug, buf1);
	StringCat2(SetDebug, dou);

	Int2Str(buf2, b);
	StringCat2(SetDebug, buf2);
	StringCat2(SetDebug, dou);

	Int2Str(buf3, c);
	StringCat2(SetDebug, buf3);
	StringCat2(SetDebug, dou);

	Int2Str(buf4, d);
	StringCat2(SetDebug, buf4);
	StringCat2(SetDebug, dou);

	Int2Str(buf5, e);
	StringCat2(SetDebug, buf5);
	StringCat2(SetDebug, dou);

	Int2Str(buf6, ff);
	StringCat2(SetDebug, buf6);
	StringCat2(SetDebug, dou);

	Int2Str(buf7, g);
	StringCat2(SetDebug, buf7);
	StringCat2(SetDebug, dou);

	Int2Str(buf8, h);
	StringCat(SetDebug, buf8);

	memset(AT_Data_buf,0,RXLEN);	       //清空缓存区AT_Data_buf

	LoraNode_Send_AT(SetDebug);

	LoraNode_Read(AT_Data_buf);	     	   //接收AT指令的返回值放到AT_Data_buf中

	if(StringStr((char *)AT_Data_buf, temp) != NULL)
	{
		return 0;
	}

	return -1;
}




//--------------------------------------------------------------------------------------------
//* @function:       Convert an Integer to a string
//* @param  str:     The string
//* @param  intnum:  The integer to be converted
//* @return:         void
//--------------------------------------------------------------------------------------------
void Int2Str(uint8_t* str, int32_t intnum)
{
        uint32_t i, Div = 1000000000, j = 0, Status = 0;

        if(intnum < 0)
        {
                intnum = intnum*(-1);
                str[j++] = '-';
        }

        for (i = 0; i < 10; i++)
        {
                str[j++] = (intnum / Div) + 48;	       /* '0' */

                intnum = intnum % Div;
                Div /= 10;
                if ((str[j-1] == '0') & (Status == 0))
                {
                        j = 0;
                }
                else
                {
                        Status++;
                }
        }
}
//--------------------------------------------------------------------------------------------
//	 @function:    string concat
//--------------------------------------------------------------------------------------------
uint8_t *StringCat(uint8_t *str, const uint8_t *string)
{
      uint8_t *s = str;

      while(*s)
      {
            s++;
      }

      while(*string)
      {
            *s++ = *string++;
      }

      *s++ = '\r';
      *s++ = '\n';
      *s = '\0';

      return str;
}

uint8_t *StringCat2(uint8_t *str, const uint8_t *string)
{
      uint8_t *s = str;

      while(*s)
      {
              s++;
      }

      while(*string)
      {
              *s++ = *string++;
      }

      return str;
}

//--------------------------------------------------------------------------------------------
//	 @function: 比较两个字符串
//--------------------------------------------------------------------------------------------
char *StringStr(char *str, char *dest)
{
		#define STR_BUFF_LEN	0x100
		int i = STR_BUFF_LEN;
		char *cp = str;
		char *s1, *s2;

		if (*dest == 0)
		{
					return str;
		}

		while(i--)
		{
					s1 = cp;
					s2 = dest;

					while((*s1 == *s2) && *s1 && *s2)
					{
							s1++;
							s2++;
					}
					if(!*s2)
							return cp;
					cp++;
		}

		return NULL;
}

