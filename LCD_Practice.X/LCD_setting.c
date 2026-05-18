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

#define LCD_ADRS 0x3E

unsigned char str[]= "Hello";

void I2C_Init(void)
{
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC4 = 1;
    
    SSPSTATbits.SMP = 1;   //I2C nomal mode
    SSPSTATbits.CKE = 0;   //SMBus input disabled
    
    SSPCON1bits.SSPM = 1000;
    SSPADD = 19;
    
    SSPCON1bits.SSPEN = 1;
}

void I2C_Start(void)
{
    SSPCON2bits.SEN = 1;
    while(SSPCON2bits.SEN);
}

void I2C_Write(unsigned char data)
{
    PIR1bits.SSPIF = 0;
    SSPBUF = data;
    while(!PIR1bits.SSPIF);
}

void I2C_Stop(void)
{
    SSPCON2bits.PEN = 1;
    while(SSPCON2bits.PEN);
}

//void write_I2C(unsigned char data) {
//    SSPCON2bits.SEN = 1;
//    while (SSPCON2bits.SEN);
//    PIR1bits.SSPIF = 0;
//    SSPBUF = data;
//    while (!PIR1bits.SSPIF);
//    SSPCON2bits.PEN = 1;
//    while (SSPCON2bits.PEN);
//
//}

void write_Command(unsigned char command)
{
    I2C_Start();
    I2C_Write(LCD_ADRS << 1);
    I2C_Write(0x00);
    I2C_Write(command);
    I2C_Stop();
}

void write_Data(unsigned char data)
{
    I2C_Start();
    I2C_Write(LCD_ADRS << 1);
    I2C_Write(0x40);
    I2C_Write(data);
    I2C_Stop();
}


void LCD_Init(void) {
    __delay_ms(100);
    write_Command(0x38);
    __delay_ms(30);
    write_Command(0x39);
    __delay_ms(30);
    write_Command(0x14);
    __delay_ms(30);
    write_Command(0x73);
    __delay_ms(30);
    write_Command(0x56);
    __delay_ms(30);
    write_Command(0x6C);
    __delay_ms(250);
    write_Command(0x38);
    __delay_ms(30);
    write_Command(0x01);
    __delay_ms(30);
    write_Command(0x0C);
    __delay_ms(30);
    
}

void main(void)
{
    I2C_Init();
    LCD_Init();
    
    for(unsigned char i = 0; str[i] != '\0'; i++)
    {
        write_Data(str[i]);
    }
    write_Command(0xC0); //2?????
    
    while(1)
    {
        
    }
}