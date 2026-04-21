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

void SYSTEM_Init(void)
{
    OSCCON = 0b01110010;    //8MHz
}

void main(void)
{
    SYSTEM_Init();
    
    while(1)
    {
        
    }
}