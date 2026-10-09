/* 
 * File:   pwm.c
 * Author: os_r_
 *
 * Created on 2026/10/01, 8:00
 */

#include <xc.h>
#include <stdint.h>
#include "pwm.h"

static uint16_t adc_old = 0;
static uint16_t duty;

void PWM_Init(void) {
    ANSELFbits.ANSELF0 = 0; //digital mode
    TRISFbits.TRISF0 = 0; //Output mode

    RF0PPS = 0x18; //RF0 => PPS

    PWM1CONbits.EN = 0; //PWM module is not enabled

    PWM1CLKbits.CLK = 0b00010; //PWM clock source is FOSC

    //prescaler = 19 & PR1 = 1000 => 1kHz, prescaler = 0 & PR1 = 20000 => 1kHz
    PWM1CPRE = 0; //1 : 1 prescaler
    PWM1PR = 20000; //PR1 setting 1kHz

    PWM1S1CFGbits.MODE = 0b000; // Left Aligned
    PWM1S1CFGbits.POL1 = 0; //Active High
    PWM1S1CFGbits.PPEN = 0; //Push-Pull off

    PWM1S1P1 = 0; //Initial duty = 0%

    PWM1CONbits.LD = 1; //Reload PR/P1/P2
    PWM1CONbits.EN = 1; //PWM1 enabled
}

void PWM_Apply(uint16_t adc) {

    if ((adc > adc_old + 2) || (adc + 2 < adc_old)) {
        adc_old = adc;

        duty = (uint16_t) (((uint32_t) adc * 20000UL) / 4095UL);
        PWM1S1P1 = duty;
        PWM1CONbits.LD = 1;
    }

}