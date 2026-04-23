#include <xc.h>
#include <pic16f1938.h>
#pragma config FOSC = INTOSC
#pragma config PWRTE = OFF
#pragma config WDTE = OFF
#pragma config MCLRE = ON
#pragma config CP = OFF
#pragma config BOREN = ON
#pragma config CLKOUTEN = OFF
#pragma config IESO = OFF
#pragma config FCMEN = OFF

#define _XTAL_FREQ 8000000UL

#define VOL1_CH 0b00001
#define VOL2_CH 0b00010
#define VOL3_CH 0b00011

unsigned char ch[3] = {VOL1_CH, VOL2_CH, VOL3_CH};
volatile unsigned int write_adc[3] = {0, 0, 0};

volatile unsigned int count = 0;
volatile unsigned int soft_duty = 0;
volatile unsigned int hard_duty = 0;
volatile unsigned char long_press = 0;
volatile unsigned int press_time = 0;
//unsigned char sw_prev = 1;

void PWM_Init(void) {
    OSCCON = 0b01110010; //8MHz

    TRISC = 0xFF; //RC reset 
    TRISCbits.TRISC2 = 0; //RC2 OUT

    ANSELB = 0x00; //RBxbits digital

    T2CONbits.T2CKPS = 0b10; //prescaler 1:16
    T2CONbits.TMR2ON = 1; //Timer2 ON
    PR2 = 124; // 1kHz

    CCP1CON = 0b00001100; //CCP1 PWM mode

}

void ADC_Init(void) {
    TRISAbits.TRISA1 = 1;
    TRISAbits.TRISA2 = 1;

    ANSELAbits.ANSA1 = 1;
    ANSELAbits.ANSA2 = 1;

    ADCON0bits.ADON = 1;

    ADCON1bits.ADNREF = 0;
    ADCON1bits.ADPREF = 0;
    ADCON1bits.ADCS = 0b101;
    ADCON1bits.ADFM = 1;
}

void SOFT_Pin_Init(void) {
    TRISBbits.TRISB1 = 0;
    LATBbits.LATB1 = 0;

    TRISBbits.TRISB4 = 1;
    ANSELBbits.ANSB4 = 0;

    OPTION_REGbits.TMR0CS = 0;
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS = 0b000;

    TMR0 = 235;
    INTCONbits.TMR0IF = 0;
    INTCONbits.TMR0IE = 1;

    //    IOCBPbits.IOCBP4 = 0;
    //    IOCBNbits.IOCBN4 = 1;
    //    IOCBFbits.IOCBF4 = 0;
    //    INTCONbits.IOCIF = 0;
    //    INTCONbits.IOCIE = 1;

    INTCONbits.GIE = 1;

}

unsigned int Read_ADC(unsigned char ch_sel) {
    ADCON0bits.CHS = ch_sel;
    __delay_us(10);
    ADCON0bits.GO_nDONE = 1;
    while (ADCON0bits.GO_nDONE);

    return ((unsigned int) ADRESH << 8) | ADRESL;
}

unsigned int Apply_Offset(unsigned int adc_val, unsigned int offset) {
    if (adc_val <= 2) {
        return 0;
    }
    if (offset >= 1023) {
        return 1023;
    }
    //    if(adc_val <= offset){
    //        return 0;
    //    }
    return offset + ((unsigned long) (adc_val - 2) * (1023UL - offset)) / (1023UL - 2);

}
//((adc_val - offset) * 1023) / (1023 - offset);
//((unsigned long)(adc_val - offset) * 1023UL) / (1023UL - offset);

void HARD_PWM_APPLY(unsigned int adc_hard) {
    if (adc_hard < 4) adc_hard = 0;
    if (adc_hard > 1015) adc_hard = 1023;

    hard_duty = (adc_hard * 499UL) / 1023UL;

    CCPR1L = hard_duty >> 2;
    CCP1CONbits.DC1B = hard_duty & 0x03;
}

void SOFT_PWM_APPLY(unsigned int adc_soft) {
    if (adc_soft > 1023) adc_soft = 1023;
    soft_duty = (adc_soft * 34UL) / 1023UL;
}

void __interrupt() isr(void) {
    if (INTCONbits.TMR0IF == 1) {
        TMR0 = 235;
        count++;
        if (count >= 34) count = 0;
        if (count < soft_duty) {
            LATBbits.LATB1 = 1;
        } else {
            LATBbits.LATB1 = 0;
        }
        INTCONbits.TMR0IF = 0;

    }

    
}

void EEPROM_Write(unsigned char addr, unsigned char data) {
    EEADRL = addr; //????
    EEDATL = data; //???

    EECON1bits.EEPGD = 0; //???EEPROM
    EECON1bits.CFGS = 0; //
    EECON1bits.WREN = 1; //??????

    INTCONbits.GIE = 0; //??????

    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1; //??????

    INTCONbits.GIE = 1; //??????

    while (EECON1bits.WR);

    EECON1bits.WREN = 0;
}

unsigned char EEPROM_Read(unsigned char addr) {
    EEADRL = addr;

    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;

    EECON1bits.RD = 1;

    return EEDATL;
}

void main(void) {
    unsigned int adc_hard_raw;
    unsigned int adc_soft_raw;
    unsigned int hard_offset;
    unsigned int soft_offset;
    unsigned int ui_level = 0;
    unsigned int ui_start = 0;
    signed char ui_dir = 1;
    PWM_Init();
    ADC_Init();
    SOFT_Pin_Init();

    unsigned char val_hard = EEPROM_Read(0);
    unsigned char val_soft = EEPROM_Read(1);

    if (val_hard == 0xFF) {
        hard_offset = 0;
    } else {
        hard_offset = ((unsigned int) val_hard) << 2;
    }

    if (val_soft == 0xFF) {
        soft_offset = 0;
    } else {
        soft_offset = ((unsigned int) val_soft) << 2;
    }


    while (1) {
        unsigned char sw_now = PORTBbits.RB4;
        static unsigned char sw_prev = 1;

        if ((sw_prev == 1) && (sw_now == 0)) {
            //when the button is pressed. detect button press(falling edge)
            press_time = 0;
            long_press = 0;
            ui_level = hard_offset;
            ui_start = hard_offset;
            ui_dir = 1;
        }

        if (sw_now == 0) {
//            __delay_ms(10);

            if (press_time < 1000) {
                press_time++;
            }
            if (press_time > 50) {
                long_press = 1;
            }

            if (long_press) {
//                if ((press_time % 3) == 0) {
                    if (ui_dir > 0) {
                        if (ui_level < 1023 - 4) {
                            ui_level += 2;
                        } else {
                            ui_level = 1023;
                            ui_dir = -1;
                        }
                    } else {
                        if (ui_level > ui_start + 4) {
                            ui_level -= 2;
                        } else {
                            ui_level = ui_start;
                            ui_dir = 1;
                        }
                    }

//                }

            }
        }





        if ((sw_prev == 0) && (sw_now == 1)) {
            //when the button is released. detect button press(rising edge)
            if (long_press == 0) {
                //short press
                hard_offset = Read_ADC(ch[0]);
                soft_offset = Read_ADC(ch[1]);
                EEPROM_Write(0, (unsigned char) (hard_offset >> 2));
                EEPROM_Write(1, (unsigned char) (soft_offset >> 2));
            }
            press_time = 0;
            long_press = 0;
            //            sw_lock = 0;
        }
        sw_prev = sw_now;

        adc_hard_raw = Read_ADC(ch[0]);
        adc_soft_raw = Read_ADC(ch[1]);

        write_adc[0] = Apply_Offset(adc_hard_raw, hard_offset);
        write_adc[1] = Apply_Offset(adc_soft_raw, soft_offset);

        if (long_press) {
            HARD_PWM_APPLY(ui_level);
        } else {
            HARD_PWM_APPLY(write_adc[0]);
        }

        SOFT_PWM_APPLY(write_adc[1]);

    }

}