/*
 * File:   dmx_receive_to_offset_pwm.c
 *
 * Created on June 26, 2026, 8:13 AM
 */


#include <xc.h>

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

#define _XTAL_FREQ 8000000UL

unsigned int dmx_count;
unsigned int my_address[4] = {1, 2, 3, 4};
volatile unsigned char dimmer[4];
unsigned char dummy;
unsigned char start_code;
volatile unsigned char save_request = 0;

void Pin_Init(void) {
    ANSELA = 0x00;
    ANSELB = 0x00;

    TRISAbits.TRISA2 = 0;
    TRISAbits.TRISA1 = 0;
    TRISBbits.TRISB4 = 1;

    TRISCbits.TRISC3 = 0;

    TRISCbits.TRISC6 = 1;
    TRISCbits.TRISC7 = 1;

    PIE1bits.RCIE = 1;

    IOCBPbits.IOCBP4 = 0;
    IOCBNbits.IOCBN4 = 1;
    IOCBFbits.IOCBF4 = 0;
    INTCONbits.IOCIF = 0;
    INTCONbits.IOCIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
}

void PWM_Init(void) {
    OSCCON = 0b01110010; //8MHz

    TRISAbits.TRISA5 = 1;
    TRISCbits.TRISC2 = 0; //CCP1 OUT
    TRISCbits.TRISC1 = 0; //CCP2 OUT
    TRISBbits.TRISB5 = 0; //CCP3 OUT
    TRISBbits.TRISB0 = 0; //CCP4 OUT

    ANSELA = 0x00;
    ANSELB = 0x00; //RBxbits digital

    APFCONbits.CCP2SEL = 0; //0:RC1, 1:RB3
    APFCONbits.CCP3SEL = 1; //0:RC6, 1:RB5

    T2CONbits.T2CKPS = 0b10; //prescaler 1:16
    T2CONbits.TMR2ON = 1; //Timer2 ON
    PR2 = 124; // 1kHz

    CCP1CON = 0b00001100; //CCP1 PWM mode
    CCP2CON = 0b00001100; //CCP2 PWM mode
    CCP3CON = 0b00001100; //CCP3 PWM mode
    CCP4CON = 0b00001100; //CCP4 PWM mode
}

void EEPROM_Write(unsigned char addr, unsigned char data) {
    EEADRL = addr;
    EEDATL = data;
    EECON1bits.CFGS = 0;
    EECON1bits.EEPGD = 0;
    EECON1bits.WREN = 1;

    INTCONbits.GIE = 0;
    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1;

    while (EECON1bits.WR);

    EECON1bits.WREN = 0;
    INTCONbits.GIE = 1;
}

unsigned char EEPROM_Read(unsigned char addr) {
    EEADRL = addr;
    EECON1bits.CFGS = 0;
    EECON1bits.EEPGD = 0;
    EECON1bits.RD = 1;

    return EEDATL;
}

unsigned int Apply_offset(unsigned char data_vol, unsigned char offset) {
    unsigned int span;
    unsigned int value;

    if (data_vol <= 2) return 0;
    if (data_vol >= 255) return 255;

    span = 255 - offset;

    value = offset + (((unsigned int) (data_vol - 2) * span) >> 8);

    if (value > 255) value = 255;

    return (unsigned char) value;

}

void pwm_apply(unsigned char i, unsigned char data) {
    unsigned int duty;

    duty = ((unsigned int) data << 1) - (data >> 5);

    if (duty >= 500) {
        duty = 499;
    }

    switch (i) {
        case 0:
            CCPR1L = duty >> 2;
            CCP1CONbits.DC1B = duty & 0x03;
            break;

        case 1:
            CCPR2L = duty >> 2;
            CCP2CONbits.DC2B = duty & 0x03;
            break;

        case 2:
            CCPR3L = duty >> 2;
            CCP3CONbits.DC3B = duty & 0x03;
            break;

        case 3:
            CCPR4L = duty >> 2;
            CCP4CONbits.DC4B = duty & 0x03;
            break;
    }
}

void USART_Init(void) {
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 1;
    BAUDCONbits.BRG16 = 1;

    SPBRG = 7;
    SPBRGH = 0;

    RCSTAbits.SPEN = 1;
    RCSTAbits.CREN = 1;
}

void __interrupt() isr(void) {

    if (INTCONbits.IOCIF) {
        if (IOCBFbits.IOCBF4) {
            save_request = 1;
            IOCBFbits.IOCBF4 = 0;
        }
        INTCONbits.IOCIF = 0;
    }

    if (!PIR1bits.RCIF) {
        LATCbits.LATC3 = 0;
        return;
    }
    if (RCSTAbits.OERR) {
        RCSTAbits.CREN = 0;
        RCSTAbits.CREN = 1;
    }

    if (RCSTAbits.FERR) {
        dummy = RCREG;
        dmx_count = 0;
        LATCbits.LATC3 = !LATCbits.LATC3;
        return;
    }

    unsigned char data = RCREG;

    if (dmx_count == 0) {
        start_code = data;
    } else if ((dmx_count <= 512) && (start_code == 0x00)) {

        for (unsigned char i = 0; i < 4; i++) {
            if (dmx_count == my_address[i]) {
                dimmer[i] = data;
            }
        }
    }
    dmx_count++;


}

void main(void) {
    Pin_Init();
    PWM_Init();
    USART_Init();

    unsigned char stored_offset[4];
    unsigned char write_val[4];

    for (char i = 0; i < 4; i++) {
        stored_offset[i] = EEPROM_Read(i);

    }

    while (1) {
        if (save_request) {
            save_request = 0;

            for (unsigned char i = 0; i < 4; i++) {
                EEPROM_Write(i, dimmer[i]);
                stored_offset[i] = dimmer[i];
            }
        }


        for (unsigned char i = 0; i < 4; i++) {
            write_val[i] = Apply_offset(dimmer[i], stored_offset[i]);
            pwm_apply(i, write_val[i]);
        }
    }
}