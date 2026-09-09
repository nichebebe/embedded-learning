/*
 * File:   dmx_free_patch.c
 * Author: Ryota
 *
 * Created on September 8, 2026, 3:59 PM
 */

#include <xc.h>
#include <pic16f1938.h>

#pragma config FOSC = INTOSC
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config CP = OFF
#pragma config MCLRE = OFF
#pragma config BOREN = ON
#pragma config CLKOUTEN = OFF
#pragma config IESO = OFF
#pragma config FCMEN = OFF
#pragma config LVP = OFF

#define _XTAL_FREQ 16000000UL

#define ENC_SW PORTBbits.RB3
#define ENC_A  PORTBbits.RB1
#define ENC_B  PORTBbits.RB2

unsigned char current_state;
unsigned int direction = 0x00;
unsigned char encoder_ready = 1;
volatile unsigned long system_ms = 0;
unsigned long sw_change_time;

void Pin_Init(void) {
    OSCCON = 0b01111010;

    ANSELA = 0x00;
    ANSELB = 0x00;

    OPTION_REGbits.TMR0CS = 0;
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS0 = 1;
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS2 = 0;
    INTCONbits.T0IF = 0;
    INTCONbits.T0IE = 1;
    INTCONbits.GIE = 1;
    TMR0 = 6;


    TRISBbits.TRISB3 = 1; //encoder SW
    TRISBbits.TRISB1 = 1; //encoder A
    TRISBbits.TRISB2 = 1; //encoder B
    TRISCbits.TRISC2 = 0; //LED1 OUT
    TRISCbits.TRISC1 = 0; //LED2 OUT
    TRISBbits.TRISB5 = 0; //LED3 OUT
    TRISBbits.TRISB0 = 0; //LED4 OUT
    TRISCbits.TRISC3 = 0; //LED5 OUT

    LATCbits.LATC2 = 0; //LED1 LOW
    LATCbits.LATC1 = 0; //LED2 LOW
    LATBbits.LATB5 = 0; //LED3 LOW
    LATBbits.LATB0 = 0; //LED4 LOW
    LATCbits.LATC3 = 0; //LED5 LOW

}

void __interrupt() _isr(void) {
    if (INTCONbits.T0IF) {
        TMR0 = 6;
        system_ms++;

        INTCONbits.T0IF = 0;
    }
}

void main(void) {

    Pin_Init();
    unsigned char sw_raw = 1;
    unsigned char sw_last = 1;
    unsigned char sw_stable = 1;
    unsigned char enc_old = 3;
    unsigned char enc_new;
    unsigned char transition;
    signed char encoder_step = 0;

    while (1) {


        //        current_state = (ENC_A << 1) | ENC_B;
        //        if (encoder_ready) {
        //            if (current_state == 1) {
        //                direction++;
        //                encoder_ready = 0;
        //            } else if (current_state == 2) {
        //                direction--;
        //                encoder_ready = 0;
        //            }
        //        }
        //        if (current_state == 3) {
        //            encoder_ready = 1;
        //        }

        enc_new = (ENC_A << 1) | ENC_B;
        if (enc_new != enc_old) {
            transition = (enc_old << 2) | enc_new;

            switch (transition) {
                case 0b1110:
                case 0b1000:
                case 0b0001:
                case 0b0111:
                    encoder_step--;
                    if (encoder_step <= -4) {
                        direction--;
                        encoder_step = 0;
                    }
                    break;

                case 0b1101:
                case 0b0100:
                case 0b0010:
                case 0b1011:
                    encoder_step++;
                    if (encoder_step >= +4) {
                        direction++;
                        encoder_step = 0;
                    }
                    break;

                default:
                    encoder_step = 0;
                    break;
            }

            enc_old = enc_new;
        }


        sw_raw = ENC_SW;

        if (sw_raw != sw_last) {
            sw_last = sw_raw;
            sw_change_time = system_ms;
        }

        if ((system_ms - sw_change_time) >= 1) {

            if (sw_stable != sw_raw) {
                sw_stable = sw_raw;

                if (sw_stable == 0) {
                    direction = 0;
                }
            }
        }

        //        if(sw_ready && ENC_SW == 0){
        //            sw_ready = 0;
        //            direction = 0;
        //        }

        LATCbits.LATC3 = direction >> 4 & 0x01; //LED1 LOW
        LATBbits.LATB0 = direction >> 3 & 0x01; //LED4 LOW
        LATBbits.LATB5 = direction >> 2 & 0x01; //LED3 LOW
        LATCbits.LATC1 = direction >> 1 & 0x01; //LED2 LOW
        LATCbits.LATC2 = direction & 0x01; //LED1 LOW

    }
}