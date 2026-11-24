/* 
 * File:   rs485.c
 * Author: os_r_
 *
 * Created on 2026/11/24, 14:04
 */

#include <xc.h>
#include <stdint.h>

void RS485_Init(void)
{
    /*
     * UART1 250kbps
     * RC6 = TX
     * RC7 = RX
     * RC5 = DE/RE
     */
    
    // RB1
    TRISBbits.TRISB1 = 0;
    ANSELBbits.ANSELB1 = 0;
    
    
    // RC5 -> LTC1485 DE/RE
    ANSELCbits.ANSELC5 = 0;
    TRISCbits.TRISC5 = 0;
    LATCbits.LATC5 = 0;         // Recive mode
    
    // 1. Baud rate
    U1CON0bits.BRGS = 0;
    U1BRG = 4;                  // 20MHz -> 250kbps
    
    // 2. RX pin PPS
    U1RXPPS = 0x17;
    
    
    // 3. RX pin digital input  RC7 -> UART1 RX
    ANSELCbits.ANSELC7 = 0;
    TRISCbits.TRISC7 = 1;
    
    // 4. Asynchronous 8-bit mode
    U1CON0bits.MODE = 0b0000;
    
    // 5. Not inverted
    U1CON2bits.RXPOL = 0;
    
    // 6. UART ON
    U1CON1bits.ON = 1;          // UART1 enable
    
    // 7. No interrupt
    
    // 8. Receive enable
    U1CON0bits.RXEN = 1;
    
    // UART1
    U1CON0bits.ABDEN = 0;       //Auto-baud is not enabled or auto-baud is complete
    U1CON0bits.TXEN = 0;

}

void RS485_Task(void){
    if(PIR4bits.U1RXIF){
        volatile uint8_t data = U1RXB;
        
        if(data == 0xAA){
            LATBbits.LATB1 ^= 1;
        }     
    }
    
    if(U1ERRIRbits.RXFOIF){
        U1ERRIRbits.RXFOIF = 0;
    }
}

/*
1. Initialize the UxBRG register pair and the BRGS bit to achieve the desired baud rate. 
2. Configure the RXPPS register for the desired RX pin.
3. Clear the ANSEL bit for the RX pin (if applicable).
4. Set the MODE bits to the desired Asynchronous mode.
5. Set the RXPOL bit if the data stream is inverted. 
6. Enable the serial port by setting the ON bit.
7. If interrupts are desired, set the UxRXIE bit in the PIEx register and enable global interrupts. 
8. Enable reception by setting the RXEN bit.
9. Read the UxERRIR register to get the error flags. 
10. The UxRXIF Interrupt Flag bit will be set when a character is transferred from the RSR to the receive buffer. An interrupt will be generated if the UxRXIE interrupt enable bit is also set. 
11. Read the UxRXB register to get the received byte. 
12. If an overrun occurred, clear the RXFOIF bit.
 */