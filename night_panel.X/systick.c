/* 
 * File:   systick.c
 * Author: os_r_
 *
 * Created on 2026/11/25, 16:29
 */

#include <xc.h>
#include <stdint.h>

static volatile uint32_t tick_ms = 0;

void Timer2_Init(void)
{
    T2CONbits.ON = 0;
    T2CLKCONbits.CS = 0b00001;
    T2CONbits.CKPS = 0b010;
    T2CONbits.OUTPS = 0b0100;
    T2HLTbits.MODE = 0b00000;
    
    T2PR = 249;
    T2TMR = 0;
    
    PIR3bits.TMR2IF = 0;
    IPR3bits.TMR2IP = 0;
    PIE3bits.TMR2IE = 1;
    
    T2CONbits.ON = 1;
}

void __interrupt(irq(IRQ_TMR2)) Timer2_ISR(void)
{
    PIR3bits.TMR2IF = 0;
    tick_ms++;
}

uint32_t GetTick_ms(void)
{
    uint32_t now;
    uint8_t ie_state;
    
    ie_state = PIE3bits.TMR2IE;
    
    PIE3bits.TMR2IE = 0;
    
    now = tick_ms;
    
    PIE3bits.TMR2IE = ie_state;
    
    return now;
}