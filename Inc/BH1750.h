/*
 * BH1750.h
 *
 *  Created on: 2019年10月15日
 *      Author: Breuning
 */

#ifndef BH1750_H_
#define BH1750_H_


#define	BH1750_SlaveAddress	 (0x23<<1)	                     //7位地址  0x23左移1位

#define   BH1750Addr_Write   (BH1750_SlaveAddress + 0) 		 //从机地址+最后写方向位
#define   BH1750Addr_Read    (BH1750_SlaveAddress + 1) 		 //从机地址+最后读方向位
#define   BH1750_PowDOWN    0x00 		 // 断电指令
#define   BH1750_PowON      0x01 		 // 通电指令
#define   BH1750_RSET     	0x07 		 // 重置指令
#define	  BH1750_CON_H  	0x10 		 // 连续高分辨率模式，1lx，120ms
#define	  BH1750_CON_H2  	0x11 		 // 连续高分辨率模式，0.5lx，120ms
#define	  BH1750_CON_L  	0x13 		 // 连续低分辨率模式，4lx，16ms
#define   BH1750_ONE_H  	0x20 		 // 一次高分辨率模式，1lx，120ms
#define   BH1750_ONE_H2		0x21 		 // 一次高分辨率模式，0.5lx，120ms
#define	  BH1750_ONE_L 		0x23 		 // 一次低分辨率模式，4lx，16ms

void BH1750_Init(void);
void GetValidDataFromBH1750(void);




#endif /* BH1750_H_ */
