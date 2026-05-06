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
}

void main(void) {
    USART_Init();

    while (1) {
        LATAbits.LATA1 = 1;
        
        unsigned char Strings[] = "Hello!!\r\n";
        unsigned int len = strlen(Strings);

        for (unsigned char i = 0; i < len; i++) {
            while (!PIR1bits.TXIF);
            TXREG = Strings[i];

        }
        __delay_ms(500);
    }
}