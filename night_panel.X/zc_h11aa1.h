/* 
 * File:   zc_h11aa1.h
 * Author: os_r_
 *
 * Created on 2026/10/01, 10:09
 */

#ifndef ZC_H11AA1_H
#define	ZC_H11AA1_H

#include <stdint.h>

void Zc_Init(void);
void Phase_Timer0_Init(void);
void Phase_SetADC(uint8_t ch, uint16_t adc);
static void Phase_SortEvents(void);
void Phase_PrepareNextEvents(void);
static void Phase_GateOn(uint8_t ch);
static void Phase_GateOff(uint8_t ch);
//#ifdef	__cplusplus
//extern "C" {
//#endif
//
//
//
//
//#ifdef	__cplusplus
//}
//#endif

#endif	/* ZC_H11AA1_H */

