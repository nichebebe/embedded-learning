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

#define _XTAL_FREQ 8000000UL 

void PWM_Init(void){
    OSCCON = 0b01110100;
    
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
    
    CCPTMRS0bits.C1TSEL = 0b00;
    CCPTMRS0bits.C2TSEL = 0b00;
    CCPTMRS0bits.C3TSEL = 0b01;
    CCPTMRS0bits.C4TSEL = 0b01;
    CCPTMRS1bits.C5TSEL = 0b10;
    
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
    
    ADCON1 = 0b11010000;   
}

unsigned int ADC_to_Duty(unsigned int adc)
{
    if(adc < 4) adc = 0;
    if(adc > 1023) adc = 1023;
    
    return (adc * 499UL) / 1023UL;
}


void pwm1_set(void)
{
    ADCON0bits.CHS = 0b00000;
    __delay_us(10);
    ADCON0bits.GO_nDONE = 1;
    while(ADCON0bits.GO_nDONE);
    
    unsigned int adc = ((unsigned int) ADRESH << 8) | ADRESL;
    
    unsigned int duty = ADC_to_Duty(adc);
    
    CCPR1L = duty >> 2;
    CCP1CONbits.DC1B = duty & 0x03;
}

void pwm2_set(void)
{
    ADCON0bits.CHS = 0b00001;
    __delay_us(10);
    ADCON0bits.GO_nDONE = 1;
    while(ADCON0bits.GO_nDONE);
    
    unsigned int adc = ((unsigned int) ADRESH << 8) | ADRESL;
    
    unsigned int duty = ADC_to_Duty(adc);
    
    CCPR2L = duty >> 2;
    CCP2CONbits.DC2B = duty & 0x03;
}

void pwm3_set(void)
{
    ADCON0bits.CHS = 0b00010;
    __delay_us(10);
    ADCON0bits.GO_nDONE = 1;
    while(ADCON0bits.GO_nDONE);
    
    unsigned int adc = ((unsigned int) ADRESH << 8) | ADRESL;
    
    unsigned int duty = ADC_to_Duty(adc);
    
    CCPR3L = duty >> 2;
    CCP3CONbits.DC3B = duty & 0x03;
}

void pwm4_set(void)
{
    ADCON0bits.CHS = 0b00011;
    __delay_us(10);
    ADCON0bits.GO_nDONE = 1;
    while(ADCON0bits.GO_nDONE);
    
    unsigned int adc = ((unsigned int) ADRESH << 8) | ADRESL;
    
    unsigned int duty = ADC_to_Duty(adc);
    
    CCPR4L = duty >> 2;
    CCP4CONbits.DC4B = duty & 0x03;
}

void pwm5_set(void)
{
    ADCON0bits.CHS = 0b00100;
    __delay_us(10);
    ADCON0bits.GO_nDONE = 1;
    while(ADCON0bits.GO_nDONE);
    
    unsigned int adc = ((unsigned int) ADRESH << 8) | ADRESL;
    
    unsigned int duty = ADC_to_Duty(adc);
    
    CCPR5L = duty >> 2;
    CCP5CONbits.DC5B = duty & 0x03;
}

void main(void)
{
    PWM_Init();
    ADC_Init();
    
    while(1)
    {
        pwm1_set();
        pwm2_set();
        pwm3_set();
        pwm4_set();
        pwm5_set();
    }
}