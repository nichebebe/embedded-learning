/*
 * File:   Upper_Lower_Offset.c
 * Author: os_r_
 *
 * Created on August 18, 2026, 12:30 PM
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

#define _XTAL_FREQ 16000000UL
#define DMX_WAIT_BREAK 0
#define DMX_WAIT_START 1
#define DMX_RECEIVING 2

unsigned int dmx_count;
unsigned int my_address[4] = {1, 2, 3, 4};
volatile unsigned char dimmer[4];
unsigned char dummy;
unsigned char start_code;
unsigned char stored_lower_offset[4];
unsigned char stored_upper_offset[4];
volatile unsigned char lower_offset[4];
volatile unsigned char upper_offset[4];
volatile unsigned char save_request = 0;
volatile unsigned char valid_frame_count = 0;
volatile unsigned char dmx_ready = 0;
volatile unsigned char dmx_state = DMX_WAIT_BREAK;


const unsigned char gammaTable[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7,
    7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14,
    14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20, 20, 21, 22, 22, 23,
    23, 24, 25, 25, 26, 26, 27, 28, 28, 29, 30, 30, 31, 32, 33, 33, 34,
    35, 35, 36, 37, 38, 39, 39, 40, 41, 42, 43, 43, 44, 45, 46, 47, 48,
    49, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64,
    65, 66, 67, 68, 69, 70, 71, 73, 74, 75, 76, 77, 78, 79, 81, 82, 83,
    84, 85, 87, 88, 89, 90, 91, 93, 94, 95, 97, 98, 99, 100, 102, 103,
    105, 106, 107, 109, 110, 111, 113, 114, 116, 117, 119, 120, 121,
    123, 124, 126, 127, 129, 130, 132, 133, 135, 137, 138, 140, 141,
    143, 145, 146, 148, 149, 151, 153, 154, 156, 158, 159, 161, 163,
    165, 166, 168, 170, 172, 173, 175, 177, 179, 181, 182, 184, 186,
    188, 190, 192, 194, 196, 197, 199, 201, 203, 205, 207, 209, 211,
    213, 215, 217, 219, 221, 223, 225, 227, 229, 231, 234, 236, 238,
    240, 242, 244, 246, 248, 251, 253, 255
};

void Pin_Init(void) {
    ANSELA = 0x00;
    ANSELB = 0x00;

    TRISAbits.TRISA2 = 0;
    TRISAbits.TRISA1 = 0;
    TRISBbits.TRISB4 = 1;

    TRISCbits.TRISC3 = 0;
    LATCbits.LATC3 = 0;

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
//    OSCCON = 0b01110010; //8MHz
    OSCCON = 0b01111010; //16MHz

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
    PR2 = 249; // 1kHz

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

unsigned int Apply_offset(unsigned char data_vol,
                          unsigned char min,
                          unsigned char max)
{
    unsigned int span;
    unsigned int value;

    if (data_vol <= 2) return 0;
    if(max <= min){
        return 0;
    }
    
    span = max - min;

    value = min + ((unsigned int) (data_vol - 1) * span) / 254 ;

    return (unsigned char) gammaTable[value];
}

void pwm_apply(unsigned char i, unsigned char data) {
    unsigned int duty;

    duty = ((unsigned int) data << 2) - (data >> 6);
//    duty = ((unsigned long)data * 999UL) / 255UL;

    if (duty >= 999) {
        duty = 999;
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

//    SPBRG = 7;
    SPBRG = 15;
    SPBRGH = 0;

    RCSTAbits.SPEN = 1;
    RCSTAbits.CREN = 1;
}

void __interrupt() isr(void) {
    LATAbits.LATA1 = 1;

    if (INTCONbits.IOCIF) {

        if (IOCBFbits.IOCBF4) {
            save_request = 1;
            IOCBFbits.IOCBF4 = 0;
        }

        INTCONbits.IOCIF = 0;
    }

    if (PIR1bits.RCIF) {

        // RCREG?????????????
        unsigned char oerr = RCSTAbits.OERR;
        unsigned char ferr = RCSTAbits.FERR;

        // ??????FIFO?1byte???
        unsigned char data = RCREG;


        if (oerr) {

            LATCbits.LATC3 = 1; // OERR???

            RCSTAbits.CREN = 0;
            RCSTAbits.CREN = 1;

            dmx_state = DMX_WAIT_BREAK;
            dmx_count = 0;
            valid_frame_count = 0;
            dmx_ready = 0;

            // LATCbits.LATC3 = 0;
            LATAbits.LATA1 = 0;
//            LATCbits.LATC3 = 0; //
            return;
        }


        if (ferr) {

            dmx_count = 0;
            dmx_state = DMX_WAIT_START;

            LATAbits.LATA1 = 0;

            return;
        }


        switch (dmx_state) {

            case DMX_WAIT_BREAK:
                break;


            case DMX_WAIT_START:

                if (data == 0x00) {
                    dmx_count = 1;
                    dmx_state = DMX_RECEIVING;
                } else {
                    dmx_state = DMX_WAIT_BREAK;
                    valid_frame_count = 0;
                    dmx_ready = 0;
                }

                break;


            case DMX_RECEIVING:

                for (unsigned char i = 0; i < 4; i++) {
                    if (dmx_count == my_address[i]) {
                        dimmer[i] = data;
                    }
                }

                if (dmx_count == my_address[3]) {

                    if (valid_frame_count < 3) {
                        valid_frame_count++;
                    }

                    if (valid_frame_count >= 3) {
                        dmx_ready = 1;
                    }

                    dmx_state = DMX_WAIT_BREAK;
                }

                dmx_count++;

                break;
        }
    }

    LATAbits.LATA1 = 0;
}

void main(void) {
    Pin_Init();
    PWM_Init();
    USART_Init();

    unsigned char write_val[4];

    for (char i = 0; i < 4; i++) {
        stored_lower_offset[i] = EEPROM_Read(i);
        stored_upper_offset[i] = EEPROM_Read(i + 4);
        
        if(stored_lower_offset[i] == 0xFF){
            stored_lower_offset[i] = 0;
        }
        
        if(stored_upper_offset[i] <= stored_lower_offset[i]){
            stored_lower_offset[i] = 0;
            stored_upper_offset[i] = 255;
        }
    }

    while (1) {
        if (save_request) {
            save_request = 0;

            for (unsigned char i = 0; i < 4; i++) {
                //lower offset
                if(dimmer[i] <= 102){
                    lower_offset[i] = dimmer[i];
                    EEPROM_Write(i, lower_offset[i]);
                    stored_lower_offset[i] = lower_offset[i];
                }
                
                //upper offset
                if(dimmer[i] >= 153){
                    upper_offset[i] = dimmer[i];
                    EEPROM_Write(i + 4, upper_offset[i]);
                    stored_upper_offset[i] = upper_offset[i];
                }
            }
        }

        if (dmx_ready) {
            for (unsigned char i = 0; i < 4; i++) {

                write_val[i] = Apply_offset(dimmer[i], stored_lower_offset[i], stored_upper_offset[i]);
                pwm_apply(i, write_val[i]);
            }
        } else {
            for (unsigned char i = 0; i < 4; i++) {
                pwm_apply(i, 0);
            }
        }

    }
}