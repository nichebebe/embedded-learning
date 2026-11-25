/* 
 * File:   rs485.c
 * Author: os_r_
 *
 * Created on 2026/11/24, 14:04
 */

#include <xc.h>
#include <stdint.h>
#include "rs485.h"

#define RS485_RX_BUFFER_SIZE    64
#define RS485_START_CODE    0xAA
#define RS485_FRAME_SIZE    19
#define RS485_ADC_CHANNEL_COUNT 8

static volatile uint8_t rx_buffer[RS485_RX_BUFFER_SIZE];

static volatile uint8_t rx_head = 0;
static volatile uint8_t rx_tail = 0;

static volatile uint16_t framing_error_count = 0;
static volatile uint16_t overrun_error_count = 0;
static volatile uint16_t checksum_error_count = 0;

static volatile uint16_t buffer_full_count = 0;
static uint8_t checksum = 0;

typedef enum {
    RS485_WAIT_START,
    RS485_RECEIVING
}rs485_state_t;

static rs485_state_t rs485_state = RS485_WAIT_START;

static uint8_t frame_buffer[RS485_FRAME_SIZE];
static uint8_t frame_index = 0;

static uint16_t remote_adc[RS485_ADC_CHANNEL_COUNT];

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
    
    PIE4bits.U1RXIE = 1;

}

void RS485_Task(void)
{
    while (rx_head != rx_tail){
        
        uint8_t data = rx_buffer[rx_tail];
        
        rx_tail++;
        
        if(rx_tail >= RS485_RX_BUFFER_SIZE){
            rx_tail = 0;
        }
        
        switch(rs485_state){
            case RS485_WAIT_START:
                if(data == RS485_START_CODE){
                    frame_index = 0;
                    checksum = 0;
                    
                    frame_buffer[frame_index] = data;
                    frame_index++;
                    
                    rs485_state = RS485_RECEIVING;
                }
                break;
                
            case RS485_RECEIVING:
                
                frame_buffer[frame_index] = data;
                
                if(frame_index < RS485_FRAME_SIZE - 1){
                    checksum += data;
                }
                
                frame_index++;
                
                if(frame_index >= RS485_FRAME_SIZE){
                    
                    if(checksum != frame_buffer[RS485_FRAME_SIZE - 1]){
                        checksum_error_count++;
                    }
                    else{
                        LATBbits.LATB1 ^= 1;
                        
                        for(uint8_t i = 0; i < RS485_ADC_CHANNEL_COUNT; i++){
                            
                            uint8_t index = i * 2 + 2;
                            
                            remote_adc[i] = ((uint16_t)frame_buffer[index] << 8)
                                            | frame_buffer[index + 1];
                        }
                    }
                    
                    frame_index = 0;
                    rs485_state = RS485_WAIT_START;
                }
                break;
        }
    }
}

void __interrupt(irq(IRQ_U1RX)) RS485_RX_ISR(void)
{
    while(PIR4bits.U1RXIF){
        uint8_t ferr = U1ERRIRbits.FERIF;
        uint8_t data = U1RXB;
        
        //Framing error
        if(ferr){
            framing_error_count++;
            continue;
        }
        
        //Calculate next head
        uint8_t next_head = rx_head + 1;
        
        if(next_head >= RS485_RX_BUFFER_SIZE){
            next_head = 0;
        }
        
        // Ring Buffer
        if(next_head != rx_tail){
            rx_buffer[rx_head] = data;
            rx_head = next_head;
        }
        else{
            buffer_full_count++;
        }        
    }
    // Hardware FIFO overflow
    if(U1ERRIRbits.RXFOIF) {
        U1ERRIRbits.RXFOIF = 0;
        overrun_error_count++;
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