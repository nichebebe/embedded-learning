/*
 * File:   pic18f57q43_first_operation.c
 * Author: os_r_
 *
 * Created on September 14, 2026, 4:42 PM
 */


#include <xc.h>
#include <stdint.h>
#include "pwm.h"
#include "zc_h11aa1.h"
#include "offset.h"
#include "gamma.h"
#include "eeprom.h"

// CONFIG1
#pragma config FEXTOSC = HS     // External Oscillator Selection (HS (crystal oscillator) above 8 MHz)
#pragma config RSTOSC = EXTOSC  // Reset Oscillator Selection (EXTOSC operating per FEXTOSC bits (device manufacturing default))

// CONFIG2
#pragma config CLKOUTEN = OFF   // Clock out Enable bit (CLKOUT function is disabled)
#pragma config PR1WAY = ON      // PRLOCKED One-Way Set Enable bit (PRLOCKED bit can be cleared and set only once)
#pragma config CSWEN = ON       // Clock Switch Enable bit (Writing to NOSC and NDIV is allowed)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor enabled)

// CONFIG3
#pragma config MCLRE = EXTMCLR  // MCLR Enable bit (If LVP = 0, MCLR pin is MCLR; If LVP = 1, RE3 pin function is MCLR )
#pragma config PWRTS = PWRT_OFF // Power-up timer selection bits (PWRT is disabled)
#pragma config MVECEN = ON      // Multi-vector enable bit (Multi-vector enabled, Vector table used for interrupts)
#pragma config IVT1WAY = ON     // IVTLOCK bit One-way set enable bit (IVTLOCKED bit can be cleared and set only once)
#pragma config LPBOREN = OFF    // Low Power BOR Enable bit (Low-Power BOR disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled , SBOREN bit is ignored)

// CONFIG4
#pragma config BORV = VBOR_1P9  // Brown-out Reset Voltage Selection bits (Brown-out Reset Voltage (VBOR) set to 1.9V)
#pragma config ZCD = OFF        // ZCD Disable bit (ZCD module is disabled. ZCD can be enabled by setting the ZCDSEN bit of ZCDCON)
#pragma config PPS1WAY = ON     // PPSLOCK bit One-Way Set Enable bit (PPSLOCKED bit can be cleared and set only once; PPS registers remain locked after one clear/set cycle)
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (Low voltage programming enabled. MCLR/VPP pin function is MCLR. MCLRE configuration bit is ignored)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Extended Instruction Set and Indexed Addressing Mode disabled)

// CONFIG5
#pragma config WDTCPS = WDTCPS_31// WDT Period selection bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF       // WDT operating mode (WDT Disabled; SWDTEN is ignored)

// CONFIG6
#pragma config WDTCWS = WDTCWS_7// WDT Window Select bits (window always open (100%); software control; keyed access not required)
#pragma config WDTCCS = SC      // WDT input clock selector (Software Control)

// CONFIG7
#pragma config BBSIZE = BBSIZE_512// Boot Block Size selection bits (Boot Block size is 512 words)
#pragma config BBEN = OFF       // Boot Block enable bit (Boot block disabled)
#pragma config SAFEN = OFF      // Storage Area Flash enable bit (SAF disabled)
#pragma config DEBUG = OFF      // Background Debugger (Background Debugger disabled)

// CONFIG8
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block not Write protected)
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers not Write protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not Write protected)
#pragma config WRTSAF = OFF     // SAF Write protection bit (SAF not Write Protected)
#pragma config WRTAPP = OFF     // Application Block write protection bit (Application Block not write protected)

// CONFIG10
#pragma config CP = OFF         // PFM and Data EEPROM Code Protection bit (PFM and Data EEPROM code protection disabled)
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#define AIN_CH_1 0x00
#define AIN_CH_2 0x01
#define AIN_CH_3 0x02
#define AIN_CH_4 0x03
#define AIN_CH_5 0x04

#define OFFSET_LOW_LIMIT 1638   // 40%
#define OFFSET_HIGH_LIMIT 2457  // 60%

#define EEPROM_MAGIC_ADDR 32
#define EEPROM_MAGIC_VALUE 0xA5

#define _XTAL_FREQ 20000000

volatile uint16_t adc_value;
volatile uint16_t phase_adc[4];
uint16_t duty;
volatile uint16_t debug_phase_adc0 = 0;
volatile uint16_t debug_phase_adc1 = 0;
static uint8_t sw_prev = 1;
uint8_t sw_now;

volatile uint16_t debug_eeprom_lower0;
volatile uint8_t debug_ee0;
volatile uint8_t debug_ee1;
volatile uint8_t debug_wrerr;

void PIN_Init(void) {
    ANSELBbits.ANSELB1 = 0;
    TRISBbits.TRISB1 = 1;

    //Offset sw setting
    ANSELBbits.ANSELB2 = 0; //digital
    TRISBbits.TRISB2 = 1; //input
    WPUBbits.WPUB2 = 1; //weak pull-up enable
    TRISBbits.TRISB3 = 0;
    LATBbits.LATB3 = 0;

}

void ADC_Init(void) {
    // RA0 = AIN1 PWM01
    ANSELAbits.ANSELA0 = 1;
    TRISAbits.TRISA0 = 1;
    /*ADC input channel = RA0 / ANA0 datasheet:p.749
      ADPCH = 0x00;*/

    // RA1 = AIN2 Phase01
    ANSELAbits.ANSELA1 = 1;
    TRISAbits.TRISA1 = 1;

    // RA2 = AIN3 Phase02
    ANSELAbits.ANSELA2 = 1;
    TRISAbits.TRISA2 = 1;

    // RA3 = AIN3 Phase03
    ANSELAbits.ANSELA3 = 1;
    TRISAbits.TRISA3 = 1;

    // RA4 = AIN4 Phase04
    ANSELAbits.ANSELA4 = 1;
    TRISAbits.TRISA4 = 1;

    // RA5 test
    ANSELAbits.ANSELA5 = 0;
    TRISAbits.TRISA5 = 0;

    //Basic ADC mode datasheet:p.744
    ADCON2bits.MD = 0b000;

    // ADC clock source
    ADCON0bits.CS = 1;

    //Right justified
    ADCON0bits.FM = 1;

    //Continuous conversion off
    ADCON0bits.CONT = 0;

    //ADC enable
    ADCON0bits.ON = 1;
}

uint16_t ADC_Read(uint8_t channel) {
    ADPCH = channel; //ADC channel select

    ADCON0bits.GO = 1; //Start ADC
    while (ADCON0bits.GO); //wait GO=0

    return ADRES;
}

uint16_t ADC_Read_Average(uint8_t channel) {
    uint32_t total = 0;

    ADC_Read(channel);

    for (uint8_t i = 0; i < 8; i++) {
        total += ADC_Read(channel);
    }

    return (uint16_t) (total / 8);
}

//void __interrupt() isr(void){
//    
//}

void main(void) {
    PIN_Init();
    ADC_Init();
    PWM_Init();
    Zc_Init();
    Phase_Timer0_Init();

    if (EEPROM_ReadByte(EEPROM_MAGIC_ADDR) == EEPROM_MAGIC_VALUE) {
        Offset_LoadAll();
    }

    while (1) {
        sw_now = PORTBbits.RB2;
        offset_result_t result[4];
        uint8_t offset_changed = 0;

        if ((sw_prev == 1) && (sw_now == 0)) {
            __delay_ms(20);

            if (PORTBbits.RB2 == 0) {

                offset_result_t result[4];
                uint8_t offset_changed = 0;

                for (uint8_t ch = 0; ch < 4; ch++) {

                    result[ch] = Offset_SetFromADC(ch, phase_adc[ch]);

                    if (result[ch] != OFFSET_NO_CHANGE) {
                        offset_changed = 1;
                    }
                }
                if (offset_changed) {
                    Phase_Suspend();
                    for (uint8_t ch = 0; ch < 4; ch++) {
                        if (result[ch] == OFFSET_LOWER_CHANGED) {
                            Offset_SaveLower(ch);
                        } 
                        else if (result[ch] == OFFSET_UPPER_CHANGED) {
                            Offset_SaveUpper(ch);
                        }
                    }
                    EEPROM_WriteByte(EEPROM_MAGIC_ADDR, EEPROM_MAGIC_VALUE);
                    Phase_Resume();
                }
            }
        }
        sw_prev = sw_now;




        adc_value = ADC_Read_Average(AIN_CH_1);
        PWM_Apply(adc_value);

        phase_adc[0] = ADC_Read_Average(AIN_CH_2);
        Phase_SetADC(0, phase_adc[0]);

        phase_adc[1] = ADC_Read_Average(AIN_CH_3);
        Phase_SetADC(1, phase_adc[1]);

        phase_adc[2] = ADC_Read_Average(AIN_CH_4);
        Phase_SetADC(2, phase_adc[2]);

        phase_adc[3] = ADC_Read_Average(AIN_CH_5);
        Phase_SetADC(3, phase_adc[3]);

        Phase_PrepareNextEvents();

        //        debug_lower0 = lower_offset[0];
        //        debug_upper0 = upper_offset[0];
    }
}

