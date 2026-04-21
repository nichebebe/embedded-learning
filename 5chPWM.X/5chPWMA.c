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


void PWM_Init(void){
    OSCCON = 0x01110100;
    
    TRISCbits.TRISC2 = 0;
    TRISCbits.TRISC1 = 0;
    TRISBbits.TRISB5 = 0;
    TRISBbits.TRISB0 = 0;
    TRISAbits.TRISA4 = 0;
    
    APFCONbits.CCP2SEL = 0;
    APFCONbits.CCP3SEL = 1;
    
}