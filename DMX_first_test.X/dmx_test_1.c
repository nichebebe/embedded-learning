#include <xc.h>
#include <string.h>
#include <stdio.h>

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

unsigned char dmx_data[] = {255, 125, 0, 255};
unsigned int dmx_length = sizeof(dmx_data) / sizeof(dmx_data[0]);

void Pin_Init(void) {
    ANSELA = 0x00;
    ANSELB = 0x00;

    TRISAbits.TRISA2 = 0;
    TRISAbits.TRISA1 = 0;
    LATAbits.LATA2 = 1;
    LATAbits.LATA1 = 0;

    TRISCbits.TRISC6 = 1;
    TRISCbits.TRISC7 = 1;

}

void USART_Init(void) {
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 1;
    BAUDCONbits.BRG16 = 1;

    SPBRG = 7;
    SPBRGH = 0;
    
    RCSTAbits.SPEN = 1;
    TXSTAbits.TXEN = 1;
}

void dmx_send_break_mab(void)
{
    TXSTAbits.TXEN = 0;
    TRISCbits.TRISC6 = 0;
    LATCbits.LATC6 = 0;
    __delay_us(100);
    
    LATCbits.LATC6 = 1;
    __delay_us(12);
    
    TXSTAbits.TXEN = 1;
    __delay_us(1);
}

void dmx_send_byte(unsigned char data)
{
    while(!PIR1bits.TXIF);
    TXREG = data;
}

void dmx_send_frame(void)
{
    dmx_send_break_mab();
    
    dmx_send_byte(0x00);
    
    for(unsigned char i = 0; i < dmx_length; i++){
        dmx_send_byte(dmx_data[i]);
    }
    
    while(!TXSTAbits.TRMT);
}

void main(void) {
    OSCCON = 0b01110010;
    Pin_Init();
    USART_Init();

    while (1) {
        dmx_send_frame();
        __delay_ms(25);
    }
}