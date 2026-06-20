/*
 * File:   dmx_receive_pwm.c
 * Author: Ryota Murakami
 *
 * Created on June 19, 2026, 3:15 PM
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
unsigned int my_address[4] = {1, 12, 25, 39};
unsigned char dimmer[4];
unsigned char dummy;
unsigned char start_code;

void Pin_Init(void) {
    ANSELA = 0x00;
    ANSELB = 0x00;

    TRISAbits.TRISA2 = 0;
    TRISAbits.TRISA1 = 0;
    LATAbits.LATA2 = 0;

    TRISCbits.TRISC6 = 1;
    TRISCbits.TRISC7 = 1;
}

void PWM_Init(void)
{
    OSCCON = 0b01110010;    //8MHz
    
    TRISCbits.TRISC2 = 0;   //CCP1 OUT
    TRISCbits.TRISC1 = 0;   //CCP2 OUT
    TRISBbits.TRISB5 = 0;   //CCP3 OUT
    TRISBbits.TRISB0 = 0;   //CCP4 OUT
    
    ANSELB = 0x00;  //RBxbits digital
    
    APFCONbits.CCP2SEL = 0;     //0:RC1, 1:RB3
    APFCONbits.CCP3SEL = 1;     //0:RC6, 1:RB5
    
    T2CONbits.T2CKPS = 0b10;    //prescaler 1:16
    T2CONbits.TMR2ON = 1;       //Timer2 ON
    PR2 = 124;              // 1kHz
    
    CCP1CON = 0b00001100;   //CCP1 PWM mode
    CCP2CON = 0b00001100;   //CCP2 PWM mode
    CCP3CON = 0b00001100;   //CCP3 PWM mode
    CCP4CON = 0b00001100;   //CCP4 PWM mode
}

void pwm_apply(unsigned char i, unsigned char data)
{
    switch (i)
    {
        case 0:
            CCPR1L = data;
            break;
            
        case 1:
            CCPR2L = data;
            break;
            
        case 2:
            CCPR3L = data;
            break;
            
        case 3:
            CCPR4L = data;
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

void dmx_receive(void) {
    if (!PIR1bits.RCIF) {
        return;
    }

    if (RCSTAbits.FERR) {
        dummy = RCREG;
        dmx_count = 0;
        return;
    }

    unsigned char data = RCREG;

    if (dmx_count == 0) {
        start_code = data;
    } 

    else if ((dmx_count <= 512) && (start_code == 0x00)) {
        for (unsigned char i = 0; i < 4; i++) {
            if (dmx_count == my_address[i]) {
                dimmer[i] = data;
            }
        }

    }

    dmx_count++;

}

void main(void) {
    PWM_Init();
    Pin_Init();
    USART_Init();

    while (1) {
        if (RCSTAbits.OERR) {
            RCSTAbits.CREN = 0;
            RCSTAbits.CREN = 1;
        }

        dmx_receive();
        
        for(unsigned char i = 0; i < 4; i++)
        {
            pwm_apply(i, dimmer[i]);
        }
    }
}

