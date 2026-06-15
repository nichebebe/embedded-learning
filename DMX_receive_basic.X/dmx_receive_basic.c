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
#define DMX_CHANNELS 255

unsigned char dmx_count[];
unsigned char dummy;

void dmx_receive(void) {

    if (RCSTAbits.FERR) {
        dummy = RCREG;
        dmx_count = 0;
    }
    else if (PIR1bits.RCIF) {
        unsigned char data = RCREG;

        if (dmx_count == 0) {
            //Start Code
        } else {
            //Channel Data
        }

        dmx_count++;
    }
}
