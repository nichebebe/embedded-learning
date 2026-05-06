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

#define BUFFER_SIZE 16
volatile unsigned char buffer[BUFFER_SIZE];
volatile unsigned char head = 0;
volatile unsigned char tail = 0;

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
    if (RCSTAbits.OERR) {
            RCSTAbits.CREN = 0;
            RCSTAbits.CREN = 1;
        }
    
    if (PIR1bits.RCIF) {

        if(RCSTAbits.FERR){
            unsigned char dummy = RCREG;
            return;
        }

        unsigned char next = (head + 1) % BUFFER_SIZE;

        if (next != tail) {
            buffer[head] = RCREG;
            head = next;
        } else {
            unsigned char dummy = RCREG;
        }
    }
}

void main(void) {
    USART_Init();

    while (1) {
        if (head != tail) {
            unsigned char data = buffer[tail];
            tail = (tail + 1) % BUFFER_SIZE;

            while (!PIR1bits.TXIF);
            TXREG = data;


            if (data == 'a') {
                LATAbits.LATA1 = 1;
            } else if (data == 'b') {
                LATAbits.LATA1 = 0;
            } else if (data == 't') {
                LATAbits.LATA1 = !LATAbits.LATA1;
            }
        }

    }
}