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

unsigned int dmx_count;
unsigned int my_address[4] = {1, 12, 25, 39};
unsigned char dimmer[4];
unsigned char dummy;
unsigned char start_code;

void Pin_Init(void) {
    ANSELA = 0x00;
    ANSELB = 0x00;

    TRISAbits.TRISA2 = 0;
    TRISAbits.TRISA1 = 0;
    LATAbits.LATA2 = 0;

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
    RCSTAbits.CREN = 1;
}

void dmx_receive(void) {
    if (!PIR1bits.RCIF) {
        return;
    }

    if (RCSTAbits.FERR) {
        dummy = RCREG;
        dmx_count = 0;
        // LATAbits.LATA1 = !LATAbits.LATA1;
        return;
    }

    unsigned char data = RCREG;

    if (dmx_count == 0) {
        //Start Code
        start_code = data;
        //        if(start_code == 0x00){
        //            LATAbits.LATA1 = 1;
        //        }else {
        //            LATAbits.LATA1 = 0;
        //        }
        
    } 
    else if (dmx_count == 1) {
            if (data > 127) {
                LATAbits.LATA1 = 1;
            } else {
                LATAbits.LATA1 = 0;
            }
        }
    else if (dmx_count <= 512) {
        for (unsigned char i = 0; i < 4; i++) {
            if (dmx_count == my_address[i]) {
                dimmer[i] = data;
            }
        }

    }

    dmx_count++;

}

void main(void) {
    OSCCON = 0b01110010;
    Pin_Init();
    USART_Init();

    while (1) {
        if (RCSTAbits.OERR) {
            RCSTAbits.CREN = 0;
            RCSTAbits.CREN = 1;
        }

        dmx_receive();
    }
}
