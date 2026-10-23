/*
 * File:   eeprom.c
 * Author: os_r_
 *
 * Created on 2026/10/22, 8:51
 */


#include <xc.h>
#include <stdint.h>
#include "eeprom.h"

#define DFM_BASE_ADDRESS 0x380000UL

volatile uint8_t debug_wrerr = 0;
volatile uint24_t debug_nvmadr;
volatile uint8_t debug_nvmdat;

void EEPROM_WriteByte(uint16_t address, uint8_t data){
    uint8_t gie_state;
    
    gie_state = INTCON0bits.GIE;
    
    NVMCON1bits.WRERR = 0;
    
    NVMADR = DFM_BASE_ADDRESS + address;
    NVMDATL = data;
    NVMCON1bits.CMD = 0x03;
    debug_nvmadr = NVMADR;
    debug_nvmdat = NVMDATL;    
    INTCON0bits.GIE = 0;
    
    NVMLOCK = 0x55;
    NVMLOCK = 0xAA;
    NVMCON0bits.GO = 1;
    
    while(NVMCON0bits.GO);
    
    debug_wrerr = NVMCON1bits.WRERR;
    
    INTCON0bits.GIE = gie_state;
    NVMCON1bits.CMD = 0x00;
}

uint8_t EEPROM_ReadByte(uint16_t address){
    
    NVMADR = DFM_BASE_ADDRESS + address;
    NVMCON1bits.CMD = 0x00;
    NVMCON0bits.GO = 1;
    while(NVMCON0bits.GO);
    
    return NVMDATL;
}

void EEPROM_Write16(uint16_t address, uint16_t data){
    uint8_t data_h = (uint8_t)(data >> 8);
    uint8_t data_l = (uint8_t)(data & 0xFF);
    
    EEPROM_WriteByte(address, data_h);
    EEPROM_WriteByte(address + 1, data_l);
}

uint16_t EEPROM_Read16(uint16_t address){
    uint16_t data;
    
    data = ((uint16_t)EEPROM_ReadByte(address) << 8);
    data |= EEPROM_ReadByte(address + 1);
    
    return data;
}
