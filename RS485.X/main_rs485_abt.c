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

#define RS485_EN_LAT LATAbits.LATA1

void Pin_Init(void)
{
    ANSELA = 0x00;
    ANSELB = 0x00;
    
    TRISAbits.TRISA1 = 0;
    LATAbits.LATA1 = 0;
    
    TRISCbits.TRISC6 = 1;
    TRISCbits.TRISC7 = 1;
    
}
void USART_Init(void){
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 1;
    BAUDCONbits.BRG16 = 1;
    
    SPBRG = 51;
    SPBRGH = 0;
    
    RCSTAbits.SPEN = 1;
    TXSTAbits.TXEN = 1;

    RCSTAbits.CREN = 1;
}
void __interrupt() isr(void)
{
    
}

void main(void)
{
    OSCCON = 0b01110010;
    Pin_Init();
    
    RS485_EN_LAT = 1;
    __delay_ms(500);
    
    RS485_EN_LAT = 0;
    __delay_ms(500);
}