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

#define DMX_WAIT_BREAK 0
#define DMX_WAIT_START 1
#define DMX_RECEIVING 2

unsigned char current_state;
unsigned int edit_address = 1;
unsigned char encoder_ready = 1;
volatile unsigned long system_ms = 0;
unsigned long sw_change_time;
unsigned char dmx_addr_text[] = "ch ADDR :";
unsigned const char lcd_line[8] = {0x80, 0x8B, 0xC0, 0xCB, 0x94, 0x9F, 0xD4, 0xDF};
unsigned int stored_address[4];
unsigned long last_encoder_time = 0;
unsigned long encoder_interval;
unsigned int encoder_increment = 1;
unsigned int dmx_count;
volatile unsigned char offset_save_request = 0;
unsigned char address_save_request = 0;
volatile unsigned char dimmer[4];
unsigned char dummy;
unsigned char start_code;
unsigned char stored_lower_offset[4];
unsigned char stored_upper_offset[4];
volatile unsigned char lower_offset[4];
volatile unsigned char upper_offset[4];
volatile unsigned char valid_frame_count = 0;
volatile unsigned char dmx_ready = 0;
volatile unsigned char dmx_state = DMX_WAIT_BREAK;
unsigned int highest_address;
unsigned int dmx_loss_ms = 0;

const unsigned char gammaTable[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7,
    7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14,
    14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20, 20, 21, 22, 22, 23,
    23, 24, 25, 25, 26, 26, 27, 28, 28, 29, 30, 30, 31, 32, 33, 33, 34,
    35, 35, 36, 37, 38, 39, 39, 40, 41, 42, 43, 43, 44, 45, 46, 47, 48,
    49, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64,
    65, 66, 67, 68, 69, 70, 71, 73, 74, 75, 76, 77, 78, 79, 81, 82, 83,
    84, 85, 87, 88, 89, 90, 91, 93, 94, 95, 97, 98, 99, 100, 102, 103,
    105, 106, 107, 109, 110, 111, 113, 114, 116, 117, 119, 120, 121,
    123, 124, 126, 127, 129, 130, 132, 133, 135, 137, 138, 140, 141,
    143, 145, 146, 148, 149, 151, 153, 154, 156, 158, 159, 161, 163,
    165, 166, 168, 170, 172, 173, 175, 177, 179, 181, 182, 184, 186,
    188, 190, 192, 194, 196, 197, 199, 201, 203, 205, 207, 209, 211,
    213, 215, 217, 219, 221, 223, 225, 227, 229, 231, 234, 236, 238,
    240, 242, 244, 246, 248, 251, 253, 255
};

void Pin_Init(void) {
    OSCCON = 0b01111010;

    ANSELA = 0x00;
    ANSELB = 0x00;

    OPTION_REGbits.TMR0CS = 0;
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS0 = 1;
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS2 = 0;
    PIE1bits.RCIE = 1;

    IOCBPbits.IOCBP4 = 0;
    IOCBNbits.IOCBN4 = 1;
    IOCBFbits.IOCBF4 = 0;
    INTCONbits.IOCIF = 0;
    INTCONbits.IOCIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.T0IF = 0;
    INTCONbits.T0IE = 1;
    INTCONbits.GIE = 1;
    TMR0 = 6;

    TRISAbits.TRISA2 = 0;
    LATAbits.LATA2 = 0;
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

    INTCONbits.GIE = 1;

    while (EECON1bits.WR);

    EECON1bits.WREN = 0;

}

unsigned char EEPROM_Read(unsigned char addr) {
    EEADRL = addr;
    EECON1bits.CFGS = 0;
    EECON1bits.EEPGD = 0;
    EECON1bits.RD = 1;

    return EEDATL;
}

void PWM_Init(void) {
    //    OSCCON = 0b01110010; //8MHz
    OSCCON = 0b01111010; //16MHz

    TRISCbits.TRISC2 = 0; //CCP1 OUT
    TRISCbits.TRISC1 = 0; //CCP2 OUT
    TRISBbits.TRISB5 = 0; //CCP3 OUT
    TRISBbits.TRISB0 = 0; //CCP4 OUT

    ANSELA = 0x00;
    ANSELB = 0x00; //RBxbits digital

    APFCONbits.CCP2SEL = 0; //0:RC1, 1:RB3
    APFCONbits.CCP3SEL = 1; //0:RC6, 1:RB5

    T2CONbits.T2CKPS = 0b10; //prescaler 1:16
    T2CONbits.TMR2ON = 1; //Timer2 ON
    PR2 = 249; // 1kHz

    CCP1CON = 0b00001100; //CCP1 PWM mode
    CCP2CON = 0b00001100; //CCP2 PWM mode
    CCP3CON = 0b00001100; //CCP3 PWM mode
    CCP4CON = 0b00001100; //CCP4 PWM mode
}

unsigned int Apply_offset(unsigned char data_vol,
        unsigned char min,
        unsigned char max) {
    unsigned int span;
    unsigned int value;

    if (data_vol <= 2) return 0;
    if (max <= min) {
        return 0;
    }

    span = max - min;

    value = min + ((unsigned int) (data_vol - 3) * span) / 252;

    return (unsigned char) gammaTable[value];
}

void pwm_apply(unsigned char i, unsigned char data) {
    unsigned int duty;

    duty = ((unsigned int) data << 2) - (data >> 6);
    //    duty = ((unsigned long)data * 999UL) / 255UL;

    if (duty >= 999) {
        duty = 999;
    }

    switch (i) {
        case 0:
            CCPR1L = duty >> 2;
            CCP1CONbits.DC1B = duty & 0x03;
            break;

        case 1:
            CCPR2L = duty >> 2;
            CCP2CONbits.DC2B = duty & 0x03;
            break;

        case 2:
            CCPR3L = duty >> 2;
            CCP3CONbits.DC3B = duty & 0x03;
            break;

        case 3:
            CCPR4L = duty >> 2;
            CCP4CONbits.DC4B = duty & 0x03;
            break;
    }
}

void USART_Init(void) {
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 1;
    BAUDCONbits.BRG16 = 1;

    //    SPBRG = 7;
    SPBRG = 15;
    SPBRGH = 0;

    RCSTAbits.SPEN = 1;
    RCSTAbits.CREN = 1;
}

void __interrupt() isr(void) {
    if (INTCONbits.T0IF) {
        TMR0 = 6;
        system_ms++;

        if(dmx_loss_ms < 1000){
            dmx_loss_ms++;
        }
        
        INTCONbits.T0IF = 0;
    }

    if (INTCONbits.IOCIF) {

        if (IOCBFbits.IOCBF4) {
            offset_save_request = 1;
            IOCBFbits.IOCBF4 = 0;
        }

        INTCONbits.IOCIF = 0;
    }

    if (PIR1bits.RCIF) {

        // RCREG?????????????
        unsigned char oerr = RCSTAbits.OERR;
        unsigned char ferr = RCSTAbits.FERR;

        // ??????FIFO?1byte???
        unsigned char data = RCREG;


        if (oerr) {

            LATCbits.LATC4 = 1; // OERR???

            RCSTAbits.CREN = 0;
            RCSTAbits.CREN = 1;

            dmx_state = DMX_WAIT_BREAK;
            dmx_count = 0;
            valid_frame_count = 0;
            dmx_ready = 0;

            return;
        }


        if (ferr) {

            dmx_count = 0;
            dmx_state = DMX_WAIT_START;

            return;
        }


        switch (dmx_state) {

            case DMX_WAIT_BREAK:
                break;


            case DMX_WAIT_START:

                if (data == 0x00) {
                    dmx_count = 1;
                    dmx_state = DMX_RECEIVING;
                } else {
                    dmx_state = DMX_WAIT_BREAK;
                    valid_frame_count = 0;
                    dmx_ready = 0;
                }

                break;


            case DMX_RECEIVING:

                if (dmx_count == stored_address[0]) {
                    dimmer[0] = data;
                } 
                if (dmx_count == stored_address[1]) {
                    dimmer[1] = data;
                }
                if (dmx_count == stored_address[2]) {
                    dimmer[2] = data;
                }
                if (dmx_count == stored_address[3]) {
                    dimmer[3] = data;
                }
                //                for (unsigned char i = 0; i < 4; i++) {
                //                    if (dmx_count == stored_address[i]) {
                //                        dimmer[i] = data;
                //                    }
                //                }

                if (dmx_count >= highest_address) {

                    dmx_loss_ms = 0;
                    
                    if (valid_frame_count < 3) {
                        valid_frame_count++;
                    }

                    if (valid_frame_count >= 3) {
                        dmx_ready = 1;
                    }

                    dmx_state = DMX_WAIT_BREAK;
                }

                dmx_count++;

                break;
        }
    }
}

void Update_Highest_Address(void) {
    highest_address = stored_address[0];

    for (unsigned char i = 1; i < 4; i++) {
        if (stored_address[i] > highest_address) {
            highest_address = stored_address[i];
        }
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
    unsigned char addr_high;
    unsigned char addr_low;
    unsigned char output_index = 0;
    unsigned char i = 0;
    unsigned char lcd_update_request = 0;

    Pin_Init();
    lcd_init();
    USART_Init();
    PWM_Init();

    unsigned char write_val[4];

    for (char i = 0; i < 4; i++) {
        stored_lower_offset[i] = EEPROM_Read(EEPROM_MIN_OFFSET_ADDR + i);
        stored_upper_offset[i] = EEPROM_Read(EEPROM_MAX_OFFSET_ADDR + i);

        if (stored_lower_offset[i] == 0xFF) {
            stored_lower_offset[i] = 0;
        }

        if (stored_upper_offset[i] == 0xFF) {
            stored_upper_offset[i] = 255;
        }

        if (stored_upper_offset[i] <= stored_lower_offset[i]) {
            stored_lower_offset[i] = 0;
            stored_upper_offset[i] = 255;
        }

        lower_offset[i] = stored_lower_offset[i];
        upper_offset[i] = stored_upper_offset[i];
    }

    for (unsigned char i = 0; i < 4; i++) {
        addr_high = EEPROM_Read(EEPROM_DMX_ADDR_BASE + (i * 2));
        addr_low = EEPROM_Read(EEPROM_DMX_ADDR_BASE + (i * 2) + 1);
        stored_address[i] = ((unsigned int) addr_high << 8) | addr_low;

        if (stored_address[i] < 1 || stored_address[i] > 512) {
            stored_address[i] = i + 1;
        }
    }

    Update_Highest_Address();

    edit_address = stored_address[0];

    for (unsigned char ch = 0; ch < 4; ch++) {
        lcd_cmd(lcd_line[ch * 2]);

        lcd_data((unsigned char) ((ch + 1) + '0'));

        for (unsigned char j = 0; dmx_addr_text[j] != '\0'; j++) {
            lcd_data(dmx_addr_text[j]);
        }

        lcd_cmd(lcd_line[ch * 2 + 1]);

        lcd_data((unsigned char) ((stored_address[ch] / 100) + '0'));
        lcd_data((unsigned char) (((stored_address[ch] / 10) % 10) + '0'));
        lcd_data((unsigned char) ((stored_address[ch] % 10) + '0'));
    }



    while (1) {
        if (offset_save_request) {
            offset_save_request = 0;

            for (unsigned char i = 0; i < 4; i++) {
                //lower offset
                if (dimmer[i] <= 102) {
                    lower_offset[i] = dimmer[i];
                    EEPROM_Write(i, lower_offset[i]);
                    stored_lower_offset[i] = lower_offset[i];
                }

                //upper offset
                if (dimmer[i] >= 153) {
                    upper_offset[i] = dimmer[i];
                    EEPROM_Write(i + 4, upper_offset[i]);
                    stored_upper_offset[i] = upper_offset[i];
                }
            }
        }

        if(dmx_loss_ms >= 500){
            dmx_ready = 0;
        }
        
        if (dmx_ready) {
            
            LATCbits.LATC3 = 1;
            
            for (unsigned char i = 0; i < 4; i++) {

                write_val[i] = Apply_offset(dimmer[i], stored_lower_offset[i], stored_upper_offset[i]);
                pwm_apply(i, write_val[i]);
            }
        } else {
            
            LATCbits.LATC3 = 0;
            
            for (unsigned char i = 0; i < 4; i++) {
                pwm_apply(i, 0);
            }
        }

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
                    address_save_request = 1;
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

        if (address_save_request) {

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

            Update_Highest_Address();

            address_save_request = 0;
        }
    }
}