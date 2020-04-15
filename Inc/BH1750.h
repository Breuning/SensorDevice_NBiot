/*
 * BH1750.h
 *
 *  Created on: 2019��10��15��
 *      Author: Breuning
 */

#ifndef BH1750_H_
#define BH1750_H_


#define	BH1750_SlaveAddress	 (0x23<<1)	                     //7λ��ַ  0x23����1λ

#define   BH1750Addr_Write   (BH1750_SlaveAddress + 0) 		 //�ӻ���ַ+���д����λ
#define   BH1750Addr_Read    (BH1750_SlaveAddress + 1) 		 //�ӻ���ַ+��������λ
#define   BH1750_PowDOWN    0x00 		 // �ϵ�ָ��
#define   BH1750_PowON      0x01 		 // ͨ��ָ��
#define   BH1750_RSET     	0x07 		 // ����ָ��
#define	  BH1750_CON_H  	0x10 		 // �����߷ֱ���ģʽ��1lx��120ms
#define	  BH1750_CON_H2  	0x11 		 // �����߷ֱ���ģʽ��0.5lx��120ms
#define	  BH1750_CON_L  	0x13 		 // �����ͷֱ���ģʽ��4lx��16ms
#define   BH1750_ONE_H  	0x20 		 // һ�θ߷ֱ���ģʽ��1lx��120ms
#define   BH1750_ONE_H2		0x21 		 // һ�θ߷ֱ���ģʽ��0.5lx��120ms
#define	  BH1750_ONE_L 		0x23 		 // һ�εͷֱ���ģʽ��4lx��16ms

void BH1750_Init(void);
void GetValidDataFromBH1750(void);




#endif /* BH1750_H_ */
