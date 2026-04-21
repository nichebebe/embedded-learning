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
    
    T2CONbits.T2CKPS = 0b10;
    T4CONbits.T4CKPS = 0b10;
    T6CONbits.T6CKPS = 0b10;
    
    T2CONbits.TMR2ON = 1;
    T4CONbits.TMR4ON = 1;
    T6CONbits.TMR6ON = 1;
    
    PR2 = 124;
    PR4 = 124;
    PR6 = 124;
    
    CCP1CON = 0b00001100;
    CCP2CON = 0b00001100;
    CCP3CON = 0b00001100;
    CCP4CON = 0b00001100;
    CCP5CON = 0b00001100;
}

void ADC_Init(void)
{
    TRISA = 0b00101111;
    ANSELA = 0b00101111;
    ADCON0bits.ADON = 1;
}