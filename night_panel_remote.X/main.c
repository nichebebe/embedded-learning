/*
 * MAIN Generated Driver File
 * 
 * @file main.c
 * 
 * @defgroup main MAIN
 * 
 * @brief This is the generated driver implementation file for the MAIN driver.
 *
 * @version MAIN Driver Version 1.0.2
 *
 * @version Package Version: 3.1.2
 */

/*
? [2026] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
 */
#include "mcc_generated_files/system/system.h"

#define ADC_CHANNEL_COUNT   8
#define OFFSET_NONE         0x00
#define OFFSET_SET          0x01
#define OFFSET_RESET        0x02
#define START_CODE          0xAA
#define PERIODIC_SEND_MS    1000UL
#define OFFSET_DEBOUNCE_MS  20UL
#define OFFSET_LONG_PRESS_MS       3000UL
#define LCD_ADDRESS         0x3E

#define LCD_PAGE_CHANGE_MS  2000UL
#define LCD_UPDATE_MS       100UL

volatile adc_result_t input_value[8] = {0, 0, 0, 0, 0, 0, 0, 0};
volatile adc_result_t last_sent_value[8] = {0, 0, 0, 0, 0, 0, 0, 0};

volatile uint8_t command;
volatile uint8_t check = 0;
volatile uint8_t send_request = 0;

volatile uint32_t tick_ms = 0;
uint32_t last_send_time_ms = 0;

uint8_t offset_raw_state = 1;
uint8_t offset_previous_raw_state = 1;
uint8_t offset_stable_state = 1;

uint32_t offset_debounce_start_ms = 0;
uint32_t offset_press_start_ms = 0;

uint8_t offset_long_press_done = 0;

uint32_t lcd_page_start_ms = 0;

static uint8_t lcd_page = 0;
static uint32_t lcd_update_start_ms = 0;

void LCD_Command(uint8_t cmd);
void LCD_Data(uint8_t data);
void LCD_Initialize(void);
void LCD_Print(const char *str);

void LCD_SetCursor(uint8_t row, uint8_t col);
void LCD_PrintNumber(uint16_t value);
void LCD_ShowInputs(uint8_t page);
uint8_t ADC_ToPercent(adc_result_t value);

const adc_channel_t input_channel[ADC_CHANNEL_COUNT] = {
    INPUT1, INPUT2, INPUT3, INPUT4, INPUT5, INPUT6, INPUT7, INPUT8
};

adc_result_t ADC_ReadAverage(adc_channel_t channel);

int main(void) {
    SYSTEM_Initialize();
    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts 
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global and Peripheral Interrupts 
    // Use the following macros to: 
    PIR4bits.TMR2IF = 0;
    PIE4bits.TMR2IE = 1;

    // Enable the Peripheral Interrupts 
    INTERRUPT_PeripheralInterruptEnable();

    // Enable the Global Interrupts 
    INTERRUPT_GlobalInterruptEnable();

    // Disable the Global Interrupts 
    //INTERRUPT_GlobalInterruptDisable(); 

    // Disable the Peripheral Interrupts 
    //INTERRUPT_PeripheralInterruptDisable(); 

    __delay_ms(100);

    LCD_Initialize();

    while (1) {
        /* 1ms clock */
//        if (PIR4bits.TMR2IF == 1) {
//            PIR4bits.TMR2IF = 0;
//            tick_ms++;
//        }

        /* Periodic transmission every 1 second */
        if ((uint32_t) (tick_ms - last_send_time_ms) >= PERIODIC_SEND_MS) {
            last_send_time_ms = tick_ms;
            send_request = 1;
        }

        /* Retrieve raw OFFSET SW value */
        offset_raw_state = OFFSET_SW_GetValue();

        /* The moment the raw value changed */
        if (offset_raw_state != offset_previous_raw_state) {
            offset_previous_raw_state = offset_raw_state;
            offset_debounce_start_ms = tick_ms;
        }

        /* Has it stabilized at 20ms? */
        if ((uint32_t) (tick_ms - offset_debounce_start_ms) >= OFFSET_DEBOUNCE_MS) {
            if (offset_stable_state != offset_raw_state) {
                offset_stable_state = offset_raw_state;

                if (offset_stable_state == 0) {
                    /* Confirm by pressing */
                    offset_press_start_ms = tick_ms;
                    offset_long_press_done = 0;
                } else {
                    /*Release confirmed*/
                    if (offset_long_press_done == 0) {
                        command = OFFSET_SET;
                        send_request = 1;
                    }
                }
            }
        }

        /* Long-press detection */
        if ((offset_stable_state == 0) && (offset_long_press_done == 0)) {
            if ((uint32_t) (tick_ms - offset_press_start_ms) >= OFFSET_LONG_PRESS_MS) {
                command = OFFSET_RESET;
                send_request = 1;

                offset_long_press_done = 1;
            }
        }

        // Get ADC value
        for (uint8_t i = 0; i < ADC_CHANNEL_COUNT; i++) {
            input_value[i] = ADC_ReadAverage(input_channel[i]);
            if (abs(input_value[i] - last_sent_value[i]) >= 10) {
                send_request = 1;
            }
        }

        // RS-485 send
        if (send_request) {
//            //test code
//            RS485_DE_SetHigh();
//
//            EUSART1_Write(START_CODE);
//            while (!TX1STAbits.TRMT) {
//            }
//            
//            for(uint8_t i = 0; i < 10; i++){
//                EUSART1_Write(0x00);
//                while(!TX1STAbits.TRMT){
//                    
//                }
//            }
//            
//            RS485_DE_SetLow();
//            
//            __delay_ms(10);

            check = 0;

            RS485_DE_SetHigh();

            EUSART1_Write(START_CODE);
            while (!TX1STAbits.TRMT) {
            }

            EUSART1_Write(command);
            check += command;
            while (!TX1STAbits.TRMT) {
            }

            for (uint8_t i = 0; i < ADC_CHANNEL_COUNT; i++) {
                uint8_t data_h = (uint8_t) (input_value[i] >> 8);
                uint8_t data_l = (uint8_t) (input_value[i] & 0xFF);

                EUSART1_Write(data_h);
                check += data_h;
                while (!TX1STAbits.TRMT) {
                }

                EUSART1_Write(data_l);
                check += data_l;
                while (!TX1STAbits.TRMT) {
                }
            }

            EUSART1_Write(check);
            while (!TX1STAbits.TRMT) {
            }

            RS485_DE_SetLow();

            for (uint8_t i = 0; i < ADC_CHANNEL_COUNT; i++) {
                last_sent_value[i] = input_value[i];
            }

            send_request = 0;
        }



        command = OFFSET_NONE;

        //        sw_prev = OFFSET_SW_GetValue();

        /*Switch pages every 2 seconds*/
        if ((uint32_t) (tick_ms - lcd_page_start_ms) >= LCD_PAGE_CHANGE_MS) {
            lcd_page_start_ms = tick_ms;

            lcd_page++;

            if (lcd_page >= 2) {
                lcd_page = 0;
            }
        }

        if ((uint32_t) (tick_ms - lcd_update_start_ms) >= LCD_UPDATE_MS) {
            lcd_update_start_ms = tick_ms;
            LCD_ShowInputs(lcd_page);
        }
    }
}

adc_result_t ADC_ReadAverage(adc_channel_t channel) {
    uint32_t total = 0;

    for (uint8_t i = 0; i < 8; i++) {
        total += (uint16_t) ADC_ChannelSelectAndConvert(channel);
    }

    return (adc_result_t) (total / 8);
}

void LCD_Command(uint8_t cmd) {
    static uint8_t data[2];

    data[0] = 0x00;
    data[1] = cmd;

    while (I2C1_IsBusy()) {
    }

    while(!I2C1_Write(LCD_ADDRESS, data, 2)){}

    while (I2C1_IsBusy()) {
    }
}

void LCD_Data(uint8_t data_value) {
    static uint8_t data[2];

    data[0] = 0x40;
    data[1] = data_value;

    while (I2C1_IsBusy()) {
    }

    while(!I2C1_Write(LCD_ADDRESS, data, 2)){
        
    }

    while (I2C1_IsBusy()) {
    }
    
    __delay_us(100);
}

void LCD_Initialize(void) {
    __delay_ms(100);

    LCD_Command(0x38);
    __delay_ms(20);

    LCD_Command(0x39);
    __delay_ms(20);

    LCD_Command(0x14);
    __delay_ms(20);

    LCD_Command(0x73);
    __delay_ms(20);

    LCD_Command(0x52);
    __delay_ms(20);

    LCD_Command(0x6C);
    __delay_ms(200);

    LCD_Command(0x38);
    __delay_ms(20);

    LCD_Command(0x01);
    __delay_ms(20);

    LCD_Command(0x0C);
    __delay_ms(20);
}

void LCD_Print(const char *str) {
    while (*str != '\0') {
        LCD_Data((uint8_t) * str);
        str++;
    }
}

void LCD_SetCursor(uint8_t row, uint8_t col) {
    uint8_t address;

    if (row == 0) {
        address = 0x00 + col;
    } else {
        address = 0x40 + col;
    }

    LCD_Command(0x80 | address);
}

uint8_t ADC_ToPercent(adc_result_t value) {
    return (uint8_t) (((uint32_t) value * 100) / 1023);
}

void LCD_PrintPercent(uint8_t value) {
//    LCD_Data('A');
//    LCD_Data('B');
//    LCD_Data('C');
//    LCD_Data('D');
    if (value >= 100) {
        LCD_Data('1');
        LCD_Data('0');
        LCD_Data('0');
    } else {
        if (value >= 10) {
            LCD_Data(' ');
            LCD_Data((value / 10) + '0');
            LCD_Data((value % 10) + '0');
        } else {
            LCD_Data(' ');
            LCD_Data(' ');
            LCD_Data(value + '0');
        }
    }

    LCD_Data('%');
}

void LCD_ShowInputs(uint8_t page) {

    if (page == 0) {
        LCD_SetCursor(0, 0);
        LCD_Print("1:");
        LCD_PrintPercent(ADC_ToPercent(input_value[0]));

        LCD_SetCursor(0, 8);
        LCD_Print("2:");
        LCD_PrintPercent(ADC_ToPercent(input_value[1]));

        LCD_SetCursor(1, 0);
        LCD_Print("3:");
        LCD_PrintPercent(ADC_ToPercent(input_value[2]));

        LCD_SetCursor(1, 8);
        LCD_Print("4:");
        LCD_PrintPercent(ADC_ToPercent(input_value[3]));
    

    } else {
        LCD_SetCursor(0, 0);
        LCD_Print("5:");
        LCD_PrintPercent(ADC_ToPercent(input_value[4]));

        LCD_SetCursor(0, 8);
        LCD_Print("6:");
        LCD_PrintPercent(ADC_ToPercent(input_value[5]));

        LCD_SetCursor(1, 0);
        LCD_Print("7:");
        LCD_PrintPercent(ADC_ToPercent(input_value[6]));

        LCD_SetCursor(1, 8);
        LCD_Print("8:");
        LCD_PrintPercent(ADC_ToPercent(input_value[7]));
    }
}