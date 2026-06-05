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

#define RS485_EN_LAT LATAbits.LATA2
#define NODE_TRANSMITTER 1

void Pin_Init(void) {
    ANSELA = 0x00;
    ANSELB = 0x00;

    TRISAbits.TRISA2 = 0;
    TRISAbits.TRISA1 = 0;
    LATAbits.LATA2 = 0;
    LATAbits.LATA1 = 0;

    TRISCbits.TRISC6 = 1;
    TRISCbits.TRISC7 = 1;

}

void USART_Init(void) {
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 1;
    BAUDCONbits.BRG16 = 1;

    SPBRG = 207;
    SPBRGH = 0;

    RCSTAbits.SPEN = 1;
    TXSTAbits.TXEN = 1;

    RCSTAbits.CREN = 1;
}

void RS485_TransmitMode(void) {
    RS485_EN_LAT = 1;
}

void RS485_ReceiveMode(void) {
    RS485_EN_LAT = 0;
}

void __interrupt() isr(void) {

}

void main(void) {
    OSCCON = 0b01110010;
    Pin_Init();
    USART_Init();

#if NODE_TRANSMITTER

    RS485_TransmitMode();

    while (1) {
        while (!PIR1bits.TXIF);
        TXREG = 'A';
        while (!TXSTAbits.TRMT);
        __delay_ms(1000);
    }

#else
    RS485_ReceiveMode();
    while (1) {
        if (PIR1bits.RCIF) {
            char data = RCREG;
            
            if (data == 'A') {
                LATAbits.LATA1 = !LATAbits.LATA1;
            }
        }

    }

#endif
}