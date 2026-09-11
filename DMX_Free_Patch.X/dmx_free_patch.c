/*
 * File:   dmx_free_patch.c
 * Author: Ryota
 *
 * Created on September 8, 2026, 3:59 PM
 */

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

#define _XTAL_FREQ 16000000UL

#define ENC_SW PORTBbits.RB3
#define ENC_A  PORTBbits.RB1
#define ENC_B  PORTBbits.RB2

#define EEPROM_MIN_OFFSET_ADDR  0
#define EEPROM_MAX_OFFSET_ADDR  4
#define EEPROM_DMX_ADDR_BASE    8

unsigned char current_state;
unsigned int edit_address = 1;
unsigned char encoder_ready = 1;
volatile unsigned long system_ms = 0;
unsigned long sw_change_time;
unsigned char dmx_addr_text[] = "DMX ADDR :";
unsigned const char lcd_line[8] = {0x80, 0x8B, 0xC0, 0xCB, 0x94, 0x9F, 0xD4, 0xDF};
unsigned int stored_address[4];
unsigned long last_encoder_time = 0;
unsigned long encoder_interval;
unsigned int encoder_increment = 1;

void Pin_Init(void) {
    OSCCON = 0b01111010;

    ANSELA = 0x00;
    ANSELB = 0x00;

    OPTION_REGbits.TMR0CS = 0;
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS0 = 1;
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS2 = 0;
    INTCONbits.T0IF = 0;
    INTCONbits.T0IE = 1;
    INTCONbits.GIE = 1;
    TMR0 = 6;

    TRISAbits.TRISA0 = 0; //RS
    TRISAbits.TRISA1 = 0; //R/W
    TRISAbits.TRISA3 = 0; //E
    TRISA &= 0x0F;
    TRISBbits.TRISB3 = 1; //encoder SW
    TRISBbits.TRISB1 = 1; //encoder A
    TRISBbits.TRISB2 = 1; //encoder B
    TRISCbits.TRISC2 = 0; //LED1 OUT
    TRISCbits.TRISC1 = 0; //LED2 OUT
    TRISBbits.TRISB5 = 0; //LED3 OUT
    TRISBbits.TRISB0 = 0; //LED4 OUT
    TRISCbits.TRISC3 = 0; //LED5 OUT

    LATCbits.LATC2 = 0; //LED1 LOW
    LATCbits.LATC1 = 0; //LED2 LOW
    LATBbits.LATB5 = 0; //LED3 LOW
    LATBbits.LATB0 = 0; //LED4 LOW
    LATCbits.LATC3 = 0; //LED5 LOW

}

void lcd_send_nibble(unsigned char nibble) {
    LATA &= 0x0F;
    LATA |= (nibble << 4);

    LATAbits.LATA3 = 1;
    __delay_us(1);
    LATAbits.LATA3 = 0;
}

unsigned char lcd_read_busy(void) {
    unsigned char busy;
    TRISA |= 0xF0; //RA4-7 input

    LATAbits.LATA0 = 0; //RS=0
    LATAbits.LATA1 = 1; //R/W read mode

    LATAbits.LATA3 = 1; //E=1
    __delay_us(1);
    busy = PORTAbits.RA7;
    LATAbits.LATA3 = 0; //E=0

    LATAbits.LATA3 = 1; //E=1
    __delay_us(1);
    LATAbits.LATA3 = 0; //E=0

    LATAbits.LATA1 = 0; //R/W write mode
    TRISA &= 0x0F; //RA4-7 output

    return busy;
}

void lcd_wait_busy(void) {
    while (lcd_read_busy()) {
    }
}

void lcd_cmd(unsigned char cmd) {

    LATAbits.LATA0 = 0; //RS=0
    LATAbits.LATA1 = 0; //R/W write mode

    lcd_send_nibble(cmd >> 4);
    lcd_send_nibble(cmd & 0x0F);
    lcd_wait_busy();
    //    __delay_ms(2);
}

void lcd_cmd_no_busy(unsigned char cmd) {
    LATAbits.LATA0 = 0; //RS=0

    lcd_send_nibble(cmd >> 4);
    lcd_send_nibble(cmd & 0x0F);
}

void lcd_data(unsigned char data) {
    LATAbits.LATA0 = 1; //RS=1
    LATAbits.LATA1 = 0; //R/W write mode

    lcd_send_nibble(data >> 4);
    lcd_send_nibble(data & 0x0F);

    lcd_wait_busy();
    //    __delay_ms(50);
}

void lcd_init(void) {
    __delay_ms(50);
    LATAbits.LATA0 = 0; //RS=0
    LATAbits.LATA1 = 0; //R/W write mode
    LATAbits.LATA3 = 0; //E=0
    lcd_send_nibble(0x03);
    __delay_ms(5);
    lcd_send_nibble(0x03);
    __delay_us(150);
    lcd_send_nibble(0x03); //0011
    __delay_us(150);
    lcd_send_nibble(0x02); //0010 4bit mode
    __delay_us(150);
    lcd_cmd(0x28); //4bit, 2-line, 5*8
    __delay_us(40);
    lcd_cmd(0x0F); //Display ON, cursor ON
    __delay_us(40);
    lcd_cmd(0x01); //clear
    __delay_ms(2);
    lcd_cmd(0x06); //entry mode
}

void EEPROM_Write(unsigned char addr, unsigned char data) {
    EEADRL = addr;
    EEDATL = data;
    EECON1bits.CFGS = 0;
    EECON1bits.EEPGD = 0;
    EECON1bits.WREN = 1;

    INTCONbits.GIE = 0;
    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1;

    while (EECON1bits.WR);

    EECON1bits.WREN = 0;
    INTCONbits.GIE = 1;
}

unsigned char EEPROM_Read(unsigned char addr) {
    EEADRL = addr;
    EECON1bits.CFGS = 0;
    EECON1bits.EEPGD = 0;
    EECON1bits.RD = 1;

    return EEDATL;
}

void __interrupt() _isr(void) {
    if (INTCONbits.T0IF) {
        TMR0 = 6;
        system_ms++;

        INTCONbits.T0IF = 0;
    }
}

void main(void) {
    unsigned char sw_raw = 1;
    unsigned char sw_last = 1;
    unsigned char sw_stable = 1;
    unsigned char enc_old = 3;
    unsigned char enc_new;
    unsigned char transition;
    signed char encoder_step = 0;
    unsigned char save_request = 0;
    unsigned char addr_high;
    unsigned char addr_low;
    unsigned char output_index = 0;
    unsigned char i = 0;
    unsigned char lcd_update_request = 0;

    Pin_Init();
    lcd_init();

    for (unsigned char i = 0; i < 4; i++) {
        addr_high = EEPROM_Read(EEPROM_DMX_ADDR_BASE + (i * 2));
        addr_low = EEPROM_Read(EEPROM_DMX_ADDR_BASE + (i * 2) + 1);
        stored_address[i] = ((unsigned int) addr_high << 8) | addr_low;

        if (stored_address[i] < 1 || stored_address[i] > 512) {
            stored_address[i] = i + 1;
        }
    }

    edit_address = stored_address[0];

    for (unsigned char ch = 0; ch < 4; ch++) {
        lcd_cmd(lcd_line[ch * 2]);

        for (unsigned char j = 0; dmx_addr_text[j] != '\0'; j++) {
            lcd_data(dmx_addr_text[j]);
        }

        lcd_cmd(lcd_line[ch * 2 + 1]);

        lcd_data((unsigned char) ((stored_address[ch] / 100) + '0'));
        lcd_data((unsigned char) (((stored_address[ch] / 10) % 10) + '0'));
        lcd_data((unsigned char) ((stored_address[ch] % 10) + '0'));
    }


    while (1) {

        enc_new = (ENC_A << 1) | ENC_B;
        if (enc_new != enc_old) {
            transition = (enc_old << 2) | enc_new;

            switch (transition) {
                case 0b1110:
                case 0b1000:
                case 0b0001:
                case 0b0111:
                    encoder_step--;
                    if (encoder_step <= -4) {
                        encoder_interval = system_ms - last_encoder_time;
                        last_encoder_time = system_ms;

                        if (encoder_interval < 40) {
                            encoder_increment = 20;
                        } else if (encoder_interval < 100) {
                            encoder_increment = 5;
                        } else {
                            encoder_increment = 1;
                        }

                        if (edit_address <= encoder_increment) {
                            edit_address = 512;
                        } else {
                            edit_address -= encoder_increment;
                        }

                        lcd_update_request = 1;
                        encoder_step = 0;
                    }
                    break;

                case 0b1101:
                case 0b0100:
                case 0b0010:
                case 0b1011:
                    encoder_step++;
                    if (encoder_step >= 4) {
                        encoder_interval = system_ms - last_encoder_time;
                        last_encoder_time = system_ms;
                        if (encoder_interval < 40) {
                            encoder_increment = 20;
                        } else if (encoder_interval < 100) {
                            encoder_increment = 5;
                        } else {
                            encoder_increment = 1;
                        }

                        if (edit_address + encoder_increment > 512) {
                            edit_address = 1;
                        } else {
                            edit_address += encoder_increment;
                        }

                        lcd_update_request = 1;
                        encoder_step = 0;
                    }

                    break;

                default:
                    encoder_step = 0;
                    break;
            }

            enc_old = enc_new;
        }


        sw_raw = ENC_SW;

        if (sw_raw != sw_last) {
            sw_last = sw_raw;
            sw_change_time = system_ms;
        }

        if ((system_ms - sw_change_time) >= 1) {

            if (sw_stable != sw_raw) {
                sw_stable = sw_raw;

                if (sw_stable == 0) {
                    save_request = 1;
                }
            }
        }

        if (lcd_update_request) {
            lcd_update_request = 0;
            lcd_cmd(lcd_line[output_index * 2 + 1]);

            lcd_data((unsigned char) ((edit_address / 100) + '0'));
            lcd_data((unsigned char) (((edit_address / 10) % 10) + '0'));
            lcd_data((unsigned char) ((edit_address % 10) + '0'));

            lcd_cmd(lcd_line[output_index * 2 + 1]);
        }

        if (save_request) {

            stored_address[output_index] = edit_address;
            addr_high = stored_address[output_index] >> 8;
            addr_low = stored_address[output_index] & 0xFF;

            EEPROM_Write((EEPROM_DMX_ADDR_BASE + (output_index * 2)), addr_high);
            EEPROM_Write((EEPROM_DMX_ADDR_BASE + (output_index * 2) + 1), addr_low);

            output_index++;

            if (output_index >= 4) {
                output_index = 0;
            }

            edit_address = stored_address[output_index];

            lcd_update_request = 1;

            save_request = 0;

        }
    }
}