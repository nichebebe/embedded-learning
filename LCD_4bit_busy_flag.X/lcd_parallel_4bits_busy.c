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

unsigned char str1[] = "Hello";
unsigned char str2[] = "World";

void pin_init(void)
{
    TRISA = 0x00;
    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC2 = 0;
    TRISCbits.TRISC3 = 0;
    ANSELA = 0x00;
    
    
}

void lcd_send_nibble(unsigned char nibble)
{
    LATA &= 0x0F;
    LATA |= (nibble << 4);
    
    LATCbits.LATC2 = 1;
    __delay_us(1);
    LATCbits.LATC2 = 0;
}

unsigned char lcd_read_busy(void)
{
    unsigned char busy;
    TRISA |= 0xF0;  //RA4-7 input
    
    LATCbits.LATC1 = 0; //RS=1
    LATCbits.LATC3 = 1; //R/W read mode
    
    LATCbits.LATC2 = 1; //E=1
    __delay_us(1);
    busy = PORTAbits.RA7;
    LATCbits.LATC2 = 0; //E=0
    
    LATCbits.LATC2 = 1;
    __delay_us(1);
    LATCbits.LATC2 = 0;
    
    LATCbits.LATC3 = 0; //R/W write mode
    TRISA &= 0x0F;  //RA4-7 output
    
    return busy;
}

void lcd_wait_busy(void)
{
    while(lcd_read_busy())
    {
    }
}

void lcd_cmd(unsigned char cmd)
{
    
    LATCbits.LATC1 = 0; //RS=0
    LATCbits.LATC3 = 0; //write
    
    lcd_send_nibble(cmd >> 4);
    lcd_send_nibble(cmd & 0x0F);
    lcd_wait_busy();
}

void lcd_cmd_no_busy(unsigned char cmd)
{
    LATCbits.LATC1 = 0; //RS=0
    
    lcd_send_nibble(cmd >> 4);
    lcd_send_nibble(cmd & 0x0F);
}

void lcd_data(unsigned char data)
{
    LATCbits.LATC1 = 1; //RS=1
    LATCbits.LATC3 = 0; //write
    
    lcd_send_nibble(data >> 4);
    lcd_send_nibble(data & 0x0F);
    
    lcd_wait_busy();
}

void lcd_init(void)
{
    __delay_ms(50);
    LATCbits.LATC1 = 0; //RS=0
    LATCbits.LATC3 = 0; //write
    
    lcd_send_nibble(0x03); //0011
    __delay_us(40);
    
    lcd_send_nibble(0x02); //0010 4bit mode
    __delay_us(40);

    lcd_cmd_no_busy(0x28);  //4bit, 2-line, 5*8
    __delay_us(40);
    
    lcd_cmd_no_busy(0x0F);  //Display ON, cursor ON
    __delay_us(40);
    
    lcd_cmd_no_busy(0x01);  //clear
    __delay_ms(2);
    
    lcd_cmd_no_busy(0x06);  //entry mode
}

void main(void)
{
    pin_init();
    lcd_init();
    
    lcd_cmd(0x80);
    for(unsigned char i = 0; str1[i] != '\0'; i++)
    {
        lcd_data(str1[i]);
    }
    
    lcd_cmd(0xC0);
    for(unsigned char i = 0; str2[i] != '\0'; i++)
    {
        lcd_data(str2[i]);
    }
    
    while(1)
    {
        
    }
}