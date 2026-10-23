/* 
 * File:   offset.c
 * Author: os_r_
 *
 * Created on 2026/10/21, 13:04
 */

#include <xc.h>
#include <stdint.h>
#include "offset.h"
#include "eeprom.h"

#define OFFSET_LOW_LIMIT 1638
#define OFFSET_HIGH_LIMIT 2457

#define EEPROM_LOWER_BASE 0
#define EEPROM_UPPER_BASE 16

uint16_t lower_offset[OFFSET_CH_COUNT] = {
    0, 0, 0, 0,
    0, 0, 0, 0
};

uint16_t upper_offset[OFFSET_CH_COUNT] = {
    4095, 4095, 4095, 4095,
    4095, 4095, 4095, 4095
};

uint16_t Offset_Rescale(uint8_t ch, uint16_t adc) 
{
    uint16_t data_vol;
    uint16_t lower;
    uint16_t upper;
    uint16_t span;
    uint16_t value;

    if (ch >= OFFSET_CH_COUNT) {
        return adc;
    }

    data_vol = adc >> 4;
    
    lower = lower_offset[ch] >> 4;
    upper = upper_offset[ch] >> 4;

    if(data_vol <= 2){
        return 0;
    }
    
    if (upper <= lower) {
        return adc;
    }
    
    span = upper - lower;
    
    value = (uint16_t)(lower + ((uint32_t)(data_vol - 3) * span) / 252UL);
    
    return (uint16_t)(value << 4);

//    if (adc <= lower) {
//        return 0;
//    }
//
//    if (adc >= upper) {
//        return 4095;
//    }
//
//    return (uint16_t) (((uint32_t) (adc - lower) * 4095UL) / (upper - lower));
}


offset_result_t Offset_SetFromADC(uint8_t ch, uint16_t adc)
{
    if(ch >= OFFSET_CH_COUNT){
        return OFFSET_NO_CHANGE;
    }
    
    if(adc <= OFFSET_LOW_LIMIT){
        lower_offset[ch] = adc;
        return OFFSET_LOWER_CHANGED;
    }
    
    else if(adc >= OFFSET_HIGH_LIMIT){
        upper_offset[ch] = adc;
        return OFFSET_UPPER_CHANGED;
    }
    
    return OFFSET_NO_CHANGE;
}

void Offset_LoadAll(void)
{
    for(uint8_t ch = 0; ch < OFFSET_CH_COUNT; ch++){
        
        lower_offset[ch] = EEPROM_Read16(EEPROM_LOWER_BASE + (ch * 2));
        
        upper_offset[ch] = EEPROM_Read16(EEPROM_UPPER_BASE + (ch * 2));
        
    }
}

void Offset_SaveLower(uint8_t ch)
{
    if(ch >= OFFSET_CH_COUNT){
        return;
    }
    
    EEPROM_Write16(EEPROM_LOWER_BASE + (ch * 2), lower_offset[ch]);

}

void Offset_SaveUpper(uint8_t ch)
{
    if(ch >= OFFSET_CH_COUNT){
        return;
    }
    
    EEPROM_Write16(EEPROM_UPPER_BASE + (ch * 2), upper_offset[ch]);
}
