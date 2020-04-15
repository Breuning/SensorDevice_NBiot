/*
 * heap_4.h
 *
 *  Created on: 2018Äê10ÔÂ9ÈÕ
 *      Author: Administrator
 */

#ifndef HEAP_4_H_
#define HEAP_4_H_

#define configTOTAL_HEAP_SIZE 1024
#define configSUPPORT_DYNAMIC_ALLOCATION 1
#define portBYTE_ALIGNMENT			8
#define portBYTE_ALIGNMENT_MASK ( 0x0007 )
#define traceFREE( pvAddress, uiSize )
#define traceMALLOC( pvAddress, uiSize )

void *pvPortMalloc( size_t xWantedSize );
void vPortFree( void *pv );

#endif /* HEAP_4_H_ */
