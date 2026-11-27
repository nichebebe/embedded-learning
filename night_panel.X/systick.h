/* 
 * File:   systick.h
 * Author: os_r_
 *
 * Created on 2026/11/25, 16:28
 */

#ifndef SYSTICK_H
#define	SYSTICK_H

#include <xc.h>
#include <stdint.h>

void Timer2_Init(void);
uint32_t GetTick_ms(void);

#endif	/* SYSTICK_H */

