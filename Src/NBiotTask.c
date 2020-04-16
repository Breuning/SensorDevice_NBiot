/*
 * NBiotTask.c
 *
 *  Created on: 2020年3月4日
 *      Author: Breuning
 */

#include "NBiotTask.h"
#include "usart.h"
#include "Package.h"

#include "string.h"

//static void NBiot_POWD_PEN();               //硬件关闭模组电源
//static uint8_t NBiot_ATAck(const char *str);
//static void NBiot_QMTPUBEX();
//static void NBiot_QMTOPEN();
//static void NBiot_QMTCONN();

static void NBiot_CleanRXBuf();
static void NBiot_Reset();
static void NBiot_USIMConfig();
static void NBiot_NetworkRegis();
static void NBiot_QMTInit();
static void NBiot_ATSend(const char *cmd);
static void NBiot_GetIMEI();

static void NBiot_QMTOPENCONN();
static void NBiot_QMTPUB();

static void Get_ATQMTCONN();
static void Get_MCUUID();


uint8_t NBiotIMEI[16] = { 0 };
uint8_t MCUID[25] = { 0 };
uint8_t ATCmds_QMTCONN[100] = { 0 };

const char *ATCmds[] =
{
	"AT+QSIMDET=0,0\r\n",
	"AT+QSIMSTAT=1\r\n",
	"AT+QSIMSTAT?\r\n",

	"AT+QCFG=\"nwscanseq\",030201,1\r\n",
	"AT+QNWINFO\r\n",
	"AT+CREG=1\r\n",
	"AT+CREG?\r\n",

	"AT+GSN\r\n",

	"AT+QMTCFG=\"version\",0,4\r\n",
	"AT+QMTCFG=\"pdpcid\",0,1\r\n",
	"AT+QMTCFG=\"keepalive\",0,120\r\n",
	"AT+QMTCFG=\"session\",0,1\r\n",
	"AT+QMTCFG=\"timeout\",0,5,3,0\r\n",

	"AT+QMTOPEN=0,\"121.42.31.73\",61613\r\n",
//	"AT+QMTCONN=0,\"clientExample\",\"admin\",\"ibs_admin\"\r\n",

	"AT+QMTOPEN?\r\n",
	"AT+QMTCONN?\r\n",

//	"AT+QMTPUB=0,0,0,1,\"mqttTopicIbs\"\r\n",
	"AT+QMTPUB=0,0,0,1,\"nbtest\"\r\n",
};

const char *ATAck[] =
{

	"AT+QMTCFG=\"version\",0,4\r\r\nOK\r\n",
	"AT+QMTCFG=\"pdpcid\",0,1\r\r\nOK\r\n",
	"AT+QMTCFG=\"keepalive\",0,120\r\r\nOK\r\n",
	"AT+QMTCFG=\"session\",0,1\r\r\nOK\r\n",
	"AT+QMTCFG=\"timeout\",0,5,3,0\r\r\nOK\r\n",
//	"AT+QMTOPEN=0,\"139.196.208.106\",61613\r\r\nOK\r\n",
//	"AT+QMTCONN=0,\"clientExample\",\"admin\",\"ibs_admin\"\r\r\nOK\r\n",
//	"AT+QMTPUBEX=0,0,0,1,\"nbtest\",\"hello breuning\"\r\r\nOK\r\n",

//	"AT+QMTOPEN?\r\r\n+QMTOPEN: 0,\"139.196.208.106\",61613\r\nOK\r\n",
//	"AT+QMTCONN?\r\r\n+QMTCONN: 0,3\r\nOK\r\n"
};

void NBiot_Init()
{
	Get_MCUUID();
	Get_ATQMTCONN();

	NBiot_Reset();
//	while(1);

	NBiot_USIMConfig();           //取消USIM卡热拔插功能
	NBiot_Reset();                //模组需重启USIM卡配置热拔插功能才能生效

	NBiot_NetworkRegis();

	NBiot_GetIMEI();

	NBiot_QMTInit();
}

void NBiot_QMTInit()
{
	NBiot_ATSend(ATCmds[AT_QMTVERSION]);
	NBiot_CleanRXBuf();
	NBiot_ATSend(ATCmds[AT_QMTPDPCID]);
	NBiot_CleanRXBuf();
	NBiot_ATSend(ATCmds[AT_QMTKEEPALIVE]);
	NBiot_CleanRXBuf();
	NBiot_ATSend(ATCmds[AT_QMTSESSION]);
	NBiot_CleanRXBuf();
	NBiot_ATSend(ATCmds[AT_QMTTIMEOUT]);
	NBiot_CleanRXBuf();

	NBiot_QMTOPENCONN();
}

void NBiot_Task()
{
	char *Ack_QMTOPENTEST = "+QMTOPEN: 0,\"121.42.31.73\",61613";
	char *Ack_QMTCONNTEST = "+QMTCONN: 0,3";
	char *a = NULL, *b = NULL;

	NBiot_ATSend(ATCmds[AT_QMTOPENTEST]);
	if(NBUart_RX.receive_flag)
	{
		NBUart_RX.receive_flag = 0;
		a = strstr((const char *)NBUart_RX.RX_Buf, Ack_QMTOPENTEST);
		memset(NBUart_RX.RX_Buf, 0 , sizeof(NBUart_RX.RX_Buf));
	}

	NBiot_ATSend(ATCmds[AT_QMTCONNTEST]);
	if(NBUart_RX.receive_flag)
	{
		NBUart_RX.receive_flag = 0;
		b = strstr((const char *)NBUart_RX.RX_Buf, Ack_QMTCONNTEST);
		memset(NBUart_RX.RX_Buf, 0 , sizeof(NBUart_RX.RX_Buf));
	}


	if(a != NULL && b != NULL)
	{
		NBiot_QMTPUB();
	}
	else
	{
		NBiot_QMTOPENCONN();
	}
}

void NBiot_QMTOPENCONN()
{
//	char *temp1 = "+QMTOPEN: 0,0";
//	char *temp2 = "+QMTOPEN: 0,2";
//	char *temp3 = "+QMTCONN: 0,0,0";

	NBiot_ATSend(ATCmds[AT_QMTOPEN]);
	HAL_Delay(1000);                     //办公室测试在800ms内
	NBiot_CleanRXBuf();

	NBiot_ATSend((const char *)ATCmds_QMTCONN);
	HAL_Delay(1000);
	NBiot_CleanRXBuf();
}

void NBiot_QMTPUB()
{
	char *temp1 = ">";
//	char *temp2 = "+QMTPUB: 0,0,0";

	uint8_t sub[] = {0x1A,0x1B};         //发送的数据包后面加CTRL+Z和ESC


	cmdAT_QMTPUB:
	NBiot_ATSend(ATCmds[AT_QMTPUB]);
	HAL_Delay(100);
	if(NBUart_RX.receive_flag)
	{
		NBUart_RX.receive_flag = 0;
		if(strstr((const char *)NBUart_RX.RX_Buf, temp1) == NULL)
			goto cmdAT_QMTPUB;
		memset(NBUart_RX.RX_Buf, 0 , sizeof(NBUart_RX.RX_Buf));
	}


	PackageComposition();
	strcat((char *)CloudPackage, (const char *)sub);
	NBiot_ATSend((char *)CloudPackage);
	memset(CloudPackage, 0 , sizeof(CloudPackage));

	HAL_Delay(3000);
	NBiot_CleanRXBuf();
}

void NBiot_Reset()
{
	HAL_GPIO_WritePin(NB_PWRKEY_GPIO_Port, NB_PWRKEY_Pin, GPIO_PIN_SET);
	HAL_Delay(2500);    //复位为2~3.8s，开机为500~1000ms
	HAL_GPIO_WritePin(NB_PWRKEY_GPIO_Port, NB_PWRKEY_Pin, GPIO_PIN_RESET);

	HAL_Delay(1000);
	NBiot_CleanRXBuf();
}

void NBiot_USIMConfig()
{
//	char *temp = "+QSIMSTAT: 1,1";

	NBiot_ATSend(ATCmds[AT_QSIMDET]);   		//不使能USIM卡热拔插功能
	NBiot_CleanRXBuf();
	NBiot_ATSend(ATCmds[AT_QSIMSTAT]);			//使能USIM卡插入状态返回
	NBiot_CleanRXBuf();
	NBiot_ATSend(ATCmds[AT_QSIMSTATTEST]);      //判断USIM卡插入状态
	NBiot_CleanRXBuf();
}

void NBiot_NetworkRegis()
{
	char *temp = "+CREG: 0,1";
	char *a = NULL;

	NBiot_ATSend(ATCmds[AT_nwscanseq]);
	NBiot_CleanRXBuf();
//	NBiot_ATSend(ATCmds[AT_CREG]);
//	NBiot_CleanRXBuf();

	do
	{
		HAL_Delay(1000);
		NBiot_ATSend(ATCmds[AT_CREGTEST]);
		if(NBUart_RX.receive_flag)
		{
			NBUart_RX.receive_flag = 0;
			a = strstr((const char *)NBUart_RX.RX_Buf, temp);
			memset(NBUart_RX.RX_Buf, 0 , sizeof(NBUart_RX.RX_Buf));
		}
	}
	while(a == NULL);

	NBiot_ATSend(ATCmds[AT_QNWINFO]);
	NBiot_CleanRXBuf();
}

void NBiot_GetIMEI()
{
	NBiot_ATSend(ATCmds[AT_GSN]);
	if(NBUart_RX.receive_flag)
	{
		uint8_t i;
		for(i = 0; i < 15; i++)
		{
			NBiotIMEI[i] = NBUart_RX.RX_Buf[i+9];
		}
		NBUart_RX.receive_flag = 0;
		memset(NBUart_RX.RX_Buf, 0 , sizeof(NBUart_RX.RX_Buf));
	}
}

void NBiot_ATSend(const char *cmd)
{
	HAL_UART_Transmit(&huart2, (uint8_t *)cmd, strlen(cmd), 100);
	HAL_Delay(200);
}

void NBiot_CleanRXBuf()
{
	if(NBUart_RX.receive_flag)
	{
		NBUart_RX.receive_flag = 0;
		memset(NBUart_RX.RX_Buf, 0 , sizeof(NBUart_RX.RX_Buf));
	}
}

/*获取stm32芯片ID*/
void Get_MCUUID()
{
	uint32_t UID[3];

	HAL_GetUID(UID);   //读取芯片的96位唯一标识ID
	sprintf((char*)MCUID, "%0*lx", 8, UID[0]);
	sprintf((char*)MCUID + 8, "%0*lx", 8, UID[1]);
	sprintf((char*)MCUID + 16, "%0*lx", 8, UID[2]);
}

/*形成AT_QMTCONN指令*/
void Get_ATQMTCONN()
{
//	"AT+QMTCONN=0,\"clientExample\",\"admin\",\"ibs_admin\"\r\n";

	char *temp1 = "AT+QMTCONN=0,\"";
	char *temp2 = "\",\"admin\",\"ibs_admin\"\r\n";

	strcpy((char *)ATCmds_QMTCONN, (const char *)temp1);
	strcat((char *)ATCmds_QMTCONN, (const char *)MCUID);
	strcat((char *)ATCmds_QMTCONN, (const char *)temp2);
}








/*
uint8_t NBiot_ATAck(const char *str)
{
	HAL_Delay(100);                  //办公室测试在10ms内
	if(NBUart_RX.receive_flag)
	{
		NBUart_RX.receive_flag = 0;
		if(strcmp((const char *)NBUart_RX.RX_Buf, str) == 0)
		{
			memset(NBUart_RX.RX_Buf, 0 , sizeof(NBUart_RX.RX_Buf));
			return 0;
		}
		else
		{
			memset(NBUart_RX.RX_Buf, 0 , sizeof(NBUart_RX.RX_Buf));
			return 1;
		}
	}
	else
	{
		memset(NBUart_RX.RX_Buf, 0 , sizeof(NBUart_RX.RX_Buf));
		return 1;
	}
}
*/


/*
void NBiot_POWD_PEN()
{
	HAL_GPIO_WritePin(NB_PWRKEY_GPIO_Port, NB_PWRKEY_Pin, GPIO_PIN_SET);
}
*/

/*
void NBiot_QMTPUBEX()
{

	char AT_QMTPUBEX[200] = { 0 };

	char *AT_QMTPUBEX_PRE = "AT+QMTPUBEX=0,0,0,1,\"nbtest\",\"{\"mqttType\":\"0\",\"devNum\":\"863859040032810-01-2\",dataValue1:21.2,dataValue2:36.5}\"\r\n";
	char *temp1 = "\"\r\n";
	char *temp2 = "+QMTPUBEX: 0,0,0";

	strcat(AT_QMTPUBEX, (const char *)AT_QMTPUBEX_PRE);
	PackageComposition();

	strcat((char *)CloudPackage, temp1);
	strcat(AT_QMTPUBEX, (const char *)CloudPackage);

	cmdAT_QMTPUBEX:
	NBiot_ATSend(AT_QMTPUBEX);


	HAL_Delay(3000);
	if(NBUart_RX.receive_flag)
	{
		NBUart_RX.receive_flag = 0;
		if(strstr((const char *)NBUart_RX.RX_Buf, temp2) == NULL)
			goto cmdAT_QMTPUBEX;
		memset(NBUart_RX.RX_Buf, 0 , sizeof(NBUart_RX.RX_Buf));
	}
}
*/
