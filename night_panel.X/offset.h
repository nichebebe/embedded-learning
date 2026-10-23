/* 
 * File:   offset.h
 * Author: os_r_
 *
 * Created on 2026/10/21, 13:05
 */

#ifndef OFFSET_H
#define	OFFSET_H

#include <stdint.h>

#define OFFSET_CH_COUNT 8

extern uint16_t lower_offset[OFFSET_CH_COUNT];
extern uint16_t upper_offset[OFFSET_CH_COUNT];

uint16_t Offset_Rescale(uint8_t ch, uint16_t adc);

typedef enum {
    OFFSET_NO_CHANGE,
    OFFSET_LOWER_CHANGED,
    OFFSET_UPPER_CHANGED
}offset_result_t;

offset_result_t Offset_SetFromADC(uint8_t ch, uint16_t adc);

void Offset_LoadAll(void);
void Offset_SaveLower(uint8_t ch);
void Offset_SaveUpper(uint8_t ch);

#endif

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
//
//#endif	/* OFFSET_H */

