#include <xc.h>

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

unsigned char str[] = "Hello"

void pin_init(void)
{
    TRISA = 0x00;
    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC2 = 0;
    ANSELA = 0x00;
    
}
void lcd_cmd(unsigned char cmd)
{
    
    LATCbits.LATC1 = 0; //RS=0
    LATA = cmd;
    
    LATCbits.LATC2 = 1; //E=1
    __delay_us(1);
    LATCbits.LATC2 = 0; //E=0
    
    __delay_us(50);
}

void lcd_data(unsigned char data)
{
    LATCbits.LATC1 = 1; //RS=1
    LATA = data;
    
    LATCbits.LATC2 = 1;
    __delay_us(1);
    LATCbits.LATC2 = 0;
    
    __delay_us(50);
}

void lcd_init(void)
{
    __delay_ms(50);
    lcd_cmd(0x38);
    __delay_us(40);
    lcd_cmd(0x38);
    __delay_us(40);
    lcd_cmd(0x0C);
    __delay_us(40);
    lcd_cmd(0x01);
    __delay_ms(20);
    lcd_cmd(0x06);
}

void main(void)
{
    pin_init();
    lcd_init();
    for(unsigned char i = 0; str[i] < '\0'; i++)
    {
        lcd_data(str[i]);
    }
}