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
#pragma config LVP = OFF

#define _XTAL_FREQ 8000000UL

#define VOL1_CH 0b00001
#define VOL2_CH 0b00010
#define VOL3_CH 0b00011
#define VOL4_CH 0b00100
#define VOL5_CH 0b00101

unsigned char ch[5] = {VOL1_CH, VOL2_CH, VOL3_CH, VOL4_CH, VOL5_CH};

volatile unsigned int press_time = 0;
volatile unsigned char long_press = 0;

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

unsigned int Read_ADC(unsigned char ch_sel)
{
    ADCON0bits.CHS = ch_sel;
    __delay_us(10);
    ADCON0bits.GO_nDONE = 1;
    while(ADCON0bits.GO_nDONE);
    return ((unsigned int) ADRESH << 8) | ADRESL;
}

unsigned int PWM_Calc(unsigned int adc)
{
    if(adc < 4) adc = 0;
    if(adc > 1023) adc = 1023;
    
    return (adc * 499UL) / 1023UL;
}

void PWM_1_APPLY(unsigned int duty)
{
    CCPR1L = duty >> 2;
    CCP1CONbits.DC1B = duty & 0x03;
}

void PWM_2_APPLY(unsigned int duty)
{
    CCPR2L =duty >> 2;
    CCP2CONbits.DC2B = duty & 0x03;
}

void PWM_3_APPLY(unsigned int duty)
{
    CCPR3L =duty >> 2;
    CCP3CONbits.DC3B = duty & 0x03;
}

void PWM_4_APPLY(unsigned int duty)
{
    CCPR4L =duty >> 2;
    CCP4CONbits.DC4B = duty & 0x03;
}

void PWM_5_APPLY(unsigned int duty)
{
    CCPR5L =duty >> 2;
    CCP5CONbits.DC5B = duty & 0x03;
}

void EEPROM_Write(unsigned char addr, unsigned char data)
{
    EEADRL = addr;
    EEDATL = data;
    EECON1bits.CFGS = 0;
    EECON1bits.EEPGD = 0;
    EECON1bits.WREN = 1;
    
    INTCONbits.GIE = 0;
    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1;
        
    while(EECON1bits.WR);
    
    EECON1bits.WREN = 0;
    INTCON.GIE = 1;
}

void EEPROM_Read(unsigned char addr)
{
    EEADRL = addr;
    EECON1bits.CFGS = 0;
    EECON1bits.EEPGD = 0;
    EECON1bits.RD = 1;
    
    return EEDATL;
}

unsigned int Apply_offset(unsigned int adc_vol, unsigned int offset)
{
    if(adc_vol <= 2) return 0;
    if(adc_vol > 1023) return 1023;
    if(adc_vol >2 & adc_vol <= 1023)
    {
        return offset + ((unsigned int)(adc_vol - 2) * (1023UL - offset)) / (1023UL - 2);
    }
}

void main(void)
{
    unsigned int adc_raw[5];
    unsigned int offset[5];
    unsigned int ui_level[5] = {0, 0, 0, 0, 0};
    unsigned int ui_start[5] = {0, 0, 0, 0, 0};
    signed char ui_dir = 1;
    PWM_Init();
    ADC_Init();
    
    unsigned char val[5];
    for(char i; i < 5; i++){
        val[i] = EEPROM_Read(i);
        if(val[i] == 0xff)
        {
            offset[i] = 0;
        }else{
            offset[i] = ((unsigned int) val[i]) << 2;
        }
    }
    
    while(1)
    {
        unsigned char sw_now = PORTBbits.RB4;
        static unsigned char sw_prev = 1;
        
        if((sw_prev == 1) && (sw_now == 0))
        {
            press_time = 0;
            long_press = 0;
            
            for(char i; i < 4; i++)
            {
                ui_level[i] = offset[i];
                ui_start[i] = offset[i];
            }
            ui_dir = 1;
        }
    }
}