#include <xc.h>
#include <string.h>
#include <stdio.h>
//#include <pic16F1938.h>

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

volatile unsigned char rx_data = 0;
volatile unsigned char rx_flag = 0;

void USART_Init(void) {
    OSCCON = 0b01110010; //8MHz
    TRISCbits.TRISC6 = 1;
    TRISCbits.TRISC7 = 1;
    TRISAbits.TRISA1 = 0;
    ANSELAbits.ANSA1 = 0;

    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 1;
    BAUDCONbits.BRG16 = 0;
    SPBRG = 51;

    RCSTAbits.SPEN = 1;
    TXSTAbits.TXEN = 1;

    RCSTAbits.CREN = 1;

    PIE1bits.RCIE = 1;
    //    PIR1bits.RCIF = 0;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
}

void __interrupt() isr(void) {
    if (PIR1bits.RCIF) {


        rx_data = RCREG;
        rx_flag = 1;
    }
}

void main(void) {
    USART_Init();

    while (1) {
        if (rx_flag) {
            rx_flag = 0;
            while (PIR1bits.TXIF) {
                TXREG = rx_data;
            }
            if (rx_data == 'a') {
                LATAbits.LATA1 = 1;
            } else if (rx_data == 'b') {
                LATAbits.LATA1 = 0;
            } else if (rx_data == 't') {
                LATAbits.LATA1 = !LATAbits.LATA1;
            }
        }

    }
}