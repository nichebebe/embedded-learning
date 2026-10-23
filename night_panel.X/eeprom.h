/* 
 * File:   eeprom.h
 * Author: os_r_
 *
 * Created on 2026/10/22, 8:54
 */

#ifndef EEPROM_H
#define	EEPROM_H

#include <stdint.h>

void EEPROM_WriteByte(uint16_t address, uint8_t data);
uint8_t EEPROM_ReadByte(uint16_t address);
void EEPROM_Write16(uint16_t address, uint16_t data);
uint16_t EEPROM_Read16(uint16_t address);

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
//#endif	/* EEPROM_H */

