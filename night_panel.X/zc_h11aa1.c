/* 
 * File:   zc_h11aa1.c
 * Author: os_r_
 *
 * Created on 2026/10/01, 10:09
 */
#include <xc.h>
#include <stdint.h>
#include "zc_h11aa1.h"

#define _XTAL_FREQ 20000000
#define PHASE_MAX_TICKS 37500UL
#define GATE_HOLD_END_TICKS 40000UL

#define PHASE_CH_COUNT 4
#define PHASE_EVENT_MAX (PHASE_CH_COUNT * 2)
#define PHASE_OFF_ADC 5

static volatile uint16_t zc_rise_ticks = 0;
static volatile uint16_t zc_fall_ticks = 0;
static volatile uint16_t zc_center_ticks = 0;
static volatile uint16_t pulse_ticks = 0;
static volatile uint16_t half_cycle_ticks = 0;
static volatile uint16_t previous_zc_center_ticks = 0;
static volatile uint32_t full_cycle_ticks = 0;
static volatile uint16_t previous_half_cycle_ticks = 0;
static volatile uint8_t zc_valid_count = 0;
static volatile uint16_t corrected_half_cycle_ticks = 0;
static volatile uint16_t delay_to_next_zc;
static volatile uint16_t preload;

static volatile uint32_t half_a_sum = 0;
static volatile uint32_t half_b_sum = 0;
static volatile uint16_t half_a_avg = 0;
static volatile uint16_t half_b_avg = 0;
static volatile uint8_t half_a_count = 0;
static volatile uint8_t half_b_count = 0;
static volatile uint8_t zc_calibrated = 0;
static volatile uint8_t current_phase = 0;

static volatile uint16_t polarity_correction_ticks = 0;
static volatile uint8_t half_phase = 0;
static volatile uint8_t learn_count = 0;

static volatile uint16_t phase_delay_ticks[4];
static volatile uint16_t next_zc_target_ticks = 0;
static volatile uint16_t zc_target_ticks = 0;
static volatile uint8_t next_zc_pending = 0;
static volatile uint16_t remaining_ticks = 0;
static volatile uint8_t event_index = 0;
static volatile uint16_t debug_elapsed_ticks = 0;

static volatile uint8_t phase_enabled[PHASE_CH_COUNT];
static volatile uint8_t phase_event_count[2] = {0, 0};

typedef enum {
    PHASE_WAIT_ZC,
    PHASE_RUN_EVENTS
} phase_state_t;

typedef enum {
    EVENT_GATE_ON,
    EVENT_GATE_OFF
} phase_event_type_t;

typedef struct {
    uint16_t time_ticks;
    uint8_t channel;
    phase_event_type_t type;
} phase_event_t;

static volatile phase_state_t phase_state = PHASE_WAIT_ZC;
static phase_event_t phase_events[2][PHASE_EVENT_MAX];
static volatile uint8_t phase_event_ready = 0;
static volatile uint8_t active_buffer = 0;
static volatile uint8_t next_buffer = 1;

void Zc_Init(void) {
    TRISBbits.TRISB0 = 1; //RB0 input
    ANSELBbits.ANSELB0 = 0; //RB0 digital

    ANSELDbits.ANSELD4 = 0;
    TRISDbits.TRISD4 = 0;
    LATDbits.LATD4 = 0;

    ANSELDbits.ANSELD5 = 0;
    TRISDbits.TRISD5 = 0;
    LATDbits.LATD5 = 0;

    ANSELDbits.ANSELD6 = 0;
    TRISDbits.TRISD6 = 0;
    LATDbits.LATD6 = 0;

    ANSELDbits.ANSELD7 = 0;
    TRISDbits.TRISD7 = 0;
    LATDbits.LATD7 = 0;

    //IOC both edges
    IOCBPbits.IOCBP0 = 1; //rising edge
    IOCBNbits.IOCBN0 = 1; //falling edge
    IOCBFbits.IOCBF0 = 0; //clear flag

    PIE0bits.IOCIE = 1; //IOC interrupt enable



    //    T0CON0bits.EN = 1;
    //    T0CON0bits.MD16 = 1;
    //    T0CON1bits.CS = 0b010;
    //    T0CON1bits.ASYNC = 0;
    //    T0CON1bits.CKPS = 0b0000;

    //timer1 : H11AA1 timing measurement
    T1CONbits.ON = 0;
    T1CONbits.CKPS = 0b00; // 1:1
    T1CONbits.RD16 = 1; // 16-bit read/write

    T1CLKbits.CS = 0b00001; // FOSC/4

    TMR1 = 0;
    T1CONbits.ON = 1;

    //Clear interrupt flags
    IOCBFbits.IOCBF0 = 0;
    PIR0bits.IOCIF = 0;

    //global interrupt enable
    INTCON0bits.GIE = 1;
}

void Phase_Timer0_Init(void) {
    // Stop Timer0 while configuring
    T0CON0bits.EN = 0;

    // 16-bit mode
    T0CON0bits.MD16 = 1;

    // FOSC/4 = 5MHz
    T0CON1bits.CS = 0b010;

    // Synchronous
    T0CON1bits.ASYNC = 0;

    //Prescaler 1:1
    T0CON1bits.CKPS = 0b0000;

    //Timer0 interrupt flag clear
    PIR3bits.TMR0IF = 0;

    //Timer0 interrupt enable
    PIE3bits.TMR0IE = 1;
}

void __interrupt(irq(IRQ_IOC)) ZC_H11AA1_ISR(void) {
    if (IOCBFbits.IOCBF0) {
        LATAbits.LATA5 ^= 1;
        if (PORTBbits.RB0) {
            zc_rise_ticks = TMR1;

        } else {
            zc_fall_ticks = TMR1;

            pulse_ticks = zc_fall_ticks - zc_rise_ticks;

            zc_center_ticks = zc_rise_ticks + (pulse_ticks / 2);

            if (zc_valid_count == 0) {
                previous_zc_center_ticks = zc_center_ticks;
                zc_valid_count = 1;
            } else if (zc_valid_count == 1) {
                half_cycle_ticks = zc_center_ticks - previous_zc_center_ticks;

                previous_half_cycle_ticks = half_cycle_ticks;
                previous_zc_center_ticks = zc_center_ticks;
                zc_valid_count = 2;

            } else {
                half_cycle_ticks = zc_center_ticks - previous_zc_center_ticks;

                /*******************************************/
                current_phase = half_phase;

                if (half_phase == 0) {

                    if (half_a_count < 16) {
                        half_a_sum += half_cycle_ticks;
                        half_a_count++;
                    }

                } else {

                    if (half_b_count < 16) {
                        half_b_sum += half_cycle_ticks;
                        half_b_count++;
                    }
                }


                if ((!zc_calibrated) && (half_a_count >= 16) && (half_b_count >= 16)) {
                    half_a_avg = (uint16_t) (half_a_sum / 16);
                    half_b_avg = (uint16_t) (half_b_sum / 16);

                    corrected_half_cycle_ticks = (uint16_t) (((uint32_t) half_a_avg + half_b_avg) / 2);

                    if (half_a_avg > half_b_avg) {
                        polarity_correction_ticks = (half_a_avg - half_b_avg) / 4;
                    } else {
                        polarity_correction_ticks = (half_b_avg - half_a_avg) / 4;
                    }

                    zc_calibrated = 1;
                }





                /*******************************************/


                //                full_cycle_ticks = (uint32_t) previous_half_cycle_ticks + (uint32_t) half_cycle_ticks;
                //                corrected_half_cycle_ticks = (uint16_t) (full_cycle_ticks / 2);
                //
                //                previous_half_cycle_ticks = half_cycle_ticks;
                previous_zc_center_ticks = zc_center_ticks;

                //Timer0 setting
                if (zc_calibrated) {

                    if (half_a_avg > half_b_avg) {

                        // A is longer than B
                        if (current_phase == 0) {

                            delay_to_next_zc =
                                    corrected_half_cycle_ticks
                                    - (pulse_ticks / 2)
                                    - polarity_correction_ticks;

                        } else {

                            delay_to_next_zc =
                                    corrected_half_cycle_ticks
                                    - (pulse_ticks / 2)
                                    + polarity_correction_ticks;
                        }

                    } else {

                        // B is longer than A
                        if (current_phase == 0) {

                            delay_to_next_zc =
                                    corrected_half_cycle_ticks
                                    - (pulse_ticks / 2)
                                    + polarity_correction_ticks;

                        } else {

                            delay_to_next_zc =
                                    corrected_half_cycle_ticks
                                    - (pulse_ticks / 2)
                                    - polarity_correction_ticks;
                        }
                    }
                    next_zc_target_ticks =
                            zc_fall_ticks + delay_to_next_zc;

                    next_zc_pending = 1;

                    if (phase_state == PHASE_WAIT_ZC) {
                        preload =
                                (uint16_t) (65536UL - delay_to_next_zc);

                        T0CON0bits.EN = 0;
                        PIR3bits.TMR0IF = 0;

                        TMR0H = (uint8_t) (preload >> 8);
                        TMR0L = (uint8_t) (preload & 0xFF);

                        phase_state = PHASE_WAIT_ZC;

                        T0CON0bits.EN = 1;
                        next_zc_pending = 0;
                    }

                }

                half_phase ^= 1;

            }
        }
        IOCBFbits.IOCBF0 = 0;
    }
    PIR0bits.IOCIF = 0;
}

void __interrupt(irq(IRQ_TMR0)) TMR0_ISR(void) {

    T0CON0bits.EN = 0; //Stop Timer0
    PIR3bits.TMR0IF = 0; //flag clear

    if (phase_state == PHASE_WAIT_ZC) {

        uint16_t elapsed_ticks;

        zc_target_ticks = next_zc_target_ticks;

        if (phase_event_ready) {

            uint8_t temp;

            temp = active_buffer;
            active_buffer = next_buffer;
            next_buffer = temp;

            phase_event_ready = 0;

        }

        event_index = 0;

        elapsed_ticks = TMR1 - zc_target_ticks;

        debug_elapsed_ticks = elapsed_ticks;

        while ((event_index < phase_event_count[active_buffer]) &&
                (phase_events[active_buffer][event_index].time_ticks <= elapsed_ticks)) {

            if (phase_events[active_buffer][event_index].type == EVENT_GATE_ON) {
                Phase_GateOn(phase_events[active_buffer][event_index].channel);
            } else {
                Phase_GateOff(phase_events[active_buffer][event_index].channel);
            }

            event_index++;
        }

        if (event_index < phase_event_count[active_buffer]) {

            remaining_ticks = phase_events[active_buffer][event_index].time_ticks - elapsed_ticks;

            preload = (uint16_t) (65536UL - remaining_ticks);

            TMR0H = (uint8_t) (preload >> 8);
            TMR0L = (uint8_t) (preload & 0xFF);

            phase_state = PHASE_RUN_EVENTS;

            T0CON0bits.EN = 1;
        }

        //        uint16_t event_target_ticks;
        //        
        //        event_target_ticks = 
        //                zc_target_ticks + phase_events[0].time_ticks;
        //        
        //        remaining_ticks = event_target_ticks - TMR1;
        //        
        //        preload = (uint16_t) (65536UL - phase_events[0].time_ticks);
        //
        //        TMR0H = (uint8_t) (preload >> 8);
        //        TMR0L = (uint8_t) (preload & 0xFF);
        //
        //        phase_state = PHASE_RUN_EVENTS;
        //
        //        T0CON0bits.EN = 1;

    } else if (phase_state == PHASE_RUN_EVENTS) {

        uint16_t elapsed_ticks;

        elapsed_ticks = TMR1 - zc_target_ticks;

        while ((event_index < phase_event_count[active_buffer]) &&
                (phase_events[active_buffer][event_index].time_ticks <= elapsed_ticks)) {

            if (phase_events[active_buffer][event_index].type == EVENT_GATE_ON) {
                Phase_GateOn(phase_events[active_buffer][event_index].channel);
            } else {
                Phase_GateOff(phase_events[active_buffer][event_index].channel);
            }

            event_index++;
        }

        if (event_index < phase_event_count[active_buffer]) {
            remaining_ticks =
                    phase_events[active_buffer][event_index].time_ticks - elapsed_ticks;

            preload = (uint16_t) (65536UL - remaining_ticks);

            TMR0H = (uint8_t) (preload >> 8);
            TMR0L = (uint8_t) (preload & 0xFF);

            T0CON0bits.EN = 1;
        } else {

            phase_state = PHASE_WAIT_ZC;

            if (next_zc_pending) {

                remaining_ticks = next_zc_target_ticks - TMR1;
                preload = (uint16_t) (65536UL - remaining_ticks);

                TMR0H = (uint8_t) (preload >> 8);
                TMR0L = (uint8_t) (preload & 0xFF);

                next_zc_pending = 0;

                T0CON0bits.EN = 1;
            }
        }
    }
}


void Phase_SetADC(uint8_t ch, uint16_t adc) {
    uint16_t new_ticks;
    uint8_t gie_state;

    if (ch >= PHASE_CH_COUNT) {
        return;
    }



    phase_enabled[ch] = 1;

        if (adc < PHASE_OFF_ADC) {
            phase_enabled[ch] = 0;
            return;
        }

    phase_enabled[ch] = 1;

    new_ticks = (uint16_t) (((uint32_t) (4095 - adc) * PHASE_MAX_TICKS) / 4095UL);

    gie_state = INTCON0bits.GIE;
    INTCON0bits.GIE = 0;

    phase_delay_ticks[ch] = new_ticks;

    INTCON0bits.GIE = gie_state;

}

static void Phase_SortEvents(void) {
    phase_event_t temp;
    uint8_t count = phase_event_count[next_buffer];

    if (count < 2) {
        return;
    }

    for (uint8_t i = 0; i < count - 1; i++) {
        for (uint8_t j = 0; j < count - 1 - i; j++) {
            if (phase_events[next_buffer][j].time_ticks > phase_events[next_buffer][j + 1].time_ticks) {
                temp = phase_events[next_buffer][j];
                phase_events[next_buffer][j] = phase_events[next_buffer][j + 1];
                phase_events[next_buffer][j + 1] = temp;
            }
        }
    }
}

static void Phase_BuildEvents(void) {
    uint8_t count = 0;

    for (uint8_t ch = 0; ch < PHASE_CH_COUNT; ch++) {

        if (!phase_enabled[ch]) {
            continue;
        }

        // Gate ON
        phase_events[next_buffer][count].time_ticks =
                phase_delay_ticks[ch];

        phase_events[next_buffer][count].channel = ch;

        phase_events[next_buffer][count].type =
                EVENT_GATE_ON;

        count++;


        // Gate OFF
        phase_events[next_buffer][count].time_ticks = GATE_HOLD_END_TICKS;

        phase_events[next_buffer][count].channel = ch;

        phase_events[next_buffer][count].type =
                EVENT_GATE_OFF;

        count++;
    }

    phase_event_count[next_buffer] = count;

    Phase_SortEvents();
}

void Phase_PrepareNextEvents(void) {
    if (phase_event_ready) {
        return;
    }

    Phase_BuildEvents();

    phase_event_ready = 1;
}

static void Phase_GateOn(uint8_t ch) {
    switch (ch) {
        case 0:
            LATDbits.LATD4 = 1;
            break;

        case 1:
            LATDbits.LATD5 = 1;
            break;

        case 2:
            LATDbits.LATD6 = 1;
            break;

        case 3:
            LATDbits.LATD7 = 1;
            break;
    }
}

static void Phase_GateOff(uint8_t ch) {
    switch (ch) {
        case 0:
            LATDbits.LATD4 = 0;
            break;

        case 1:
            LATDbits.LATD5 = 0;
            break;

        case 2:
            LATDbits.LATD6 = 0;
            break;

        case 3:
            LATDbits.LATD7 = 0;
            break;
    }
}