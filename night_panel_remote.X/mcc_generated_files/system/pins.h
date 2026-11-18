/**
 * Generated Pins header File
 * 
 * @file pins.h
 * 
 * @defgroup  pinsdriver Pins Driver
 * 
 * @brief This is generated driver header for pins. 
 *        This header file provides APIs for all pins selected in the GUI.
 *
 * @version Driver Version  3.0.0
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

#ifndef PINS_H
#define PINS_H

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set IO_RA0 aliases
#define OFFSET_SW_TRIS                 TRISAbits.TRISA0
#define OFFSET_SW_LAT                  LATAbits.LATA0
#define OFFSET_SW_PORT                 PORTAbits.RA0
#define OFFSET_SW_WPU                  WPUAbits.WPUA0
#define OFFSET_SW_OD                   ODCONAbits.ODCA0
#define OFFSET_SW_ANS                  ANSELAbits.ANSA0
#define OFFSET_SW_SetHigh()            do { LATAbits.LATA0 = 1; } while(0)
#define OFFSET_SW_SetLow()             do { LATAbits.LATA0 = 0; } while(0)
#define OFFSET_SW_Toggle()             do { LATAbits.LATA0 = ~LATAbits.LATA0; } while(0)
#define OFFSET_SW_GetValue()           PORTAbits.RA0
#define OFFSET_SW_SetDigitalInput()    do { TRISAbits.TRISA0 = 1; } while(0)
#define OFFSET_SW_SetDigitalOutput()   do { TRISAbits.TRISA0 = 0; } while(0)
#define OFFSET_SW_SetPullup()          do { WPUAbits.WPUA0 = 1; } while(0)
#define OFFSET_SW_ResetPullup()        do { WPUAbits.WPUA0 = 0; } while(0)
#define OFFSET_SW_SetPushPull()        do { ODCONAbits.ODCA0 = 0; } while(0)
#define OFFSET_SW_SetOpenDrain()       do { ODCONAbits.ODCA0 = 1; } while(0)
#define OFFSET_SW_SetAnalogMode()      do { ANSELAbits.ANSA0 = 1; } while(0)
#define OFFSET_SW_SetDigitalMode()     do { ANSELAbits.ANSA0 = 0; } while(0)
// get/set IO_RA1 aliases
#define UP_SW_TRIS                 TRISAbits.TRISA1
#define UP_SW_LAT                  LATAbits.LATA1
#define UP_SW_PORT                 PORTAbits.RA1
#define UP_SW_WPU                  WPUAbits.WPUA1
#define UP_SW_OD                   ODCONAbits.ODCA1
#define UP_SW_ANS                  ANSELAbits.ANSA1
#define UP_SW_SetHigh()            do { LATAbits.LATA1 = 1; } while(0)
#define UP_SW_SetLow()             do { LATAbits.LATA1 = 0; } while(0)
#define UP_SW_Toggle()             do { LATAbits.LATA1 = ~LATAbits.LATA1; } while(0)
#define UP_SW_GetValue()           PORTAbits.RA1
#define UP_SW_SetDigitalInput()    do { TRISAbits.TRISA1 = 1; } while(0)
#define UP_SW_SetDigitalOutput()   do { TRISAbits.TRISA1 = 0; } while(0)
#define UP_SW_SetPullup()          do { WPUAbits.WPUA1 = 1; } while(0)
#define UP_SW_ResetPullup()        do { WPUAbits.WPUA1 = 0; } while(0)
#define UP_SW_SetPushPull()        do { ODCONAbits.ODCA1 = 0; } while(0)
#define UP_SW_SetOpenDrain()       do { ODCONAbits.ODCA1 = 1; } while(0)
#define UP_SW_SetAnalogMode()      do { ANSELAbits.ANSA1 = 1; } while(0)
#define UP_SW_SetDigitalMode()     do { ANSELAbits.ANSA1 = 0; } while(0)
// get/set IO_RA2 aliases
#define DOWN_SW_TRIS                 TRISAbits.TRISA2
#define DOWN_SW_LAT                  LATAbits.LATA2
#define DOWN_SW_PORT                 PORTAbits.RA2
#define DOWN_SW_WPU                  WPUAbits.WPUA2
#define DOWN_SW_OD                   ODCONAbits.ODCA2
#define DOWN_SW_ANS                  ANSELAbits.ANSA2
#define DOWN_SW_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
#define DOWN_SW_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
#define DOWN_SW_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define DOWN_SW_GetValue()           PORTAbits.RA2
#define DOWN_SW_SetDigitalInput()    do { TRISAbits.TRISA2 = 1; } while(0)
#define DOWN_SW_SetDigitalOutput()   do { TRISAbits.TRISA2 = 0; } while(0)
#define DOWN_SW_SetPullup()          do { WPUAbits.WPUA2 = 1; } while(0)
#define DOWN_SW_ResetPullup()        do { WPUAbits.WPUA2 = 0; } while(0)
#define DOWN_SW_SetPushPull()        do { ODCONAbits.ODCA2 = 0; } while(0)
#define DOWN_SW_SetOpenDrain()       do { ODCONAbits.ODCA2 = 1; } while(0)
#define DOWN_SW_SetAnalogMode()      do { ANSELAbits.ANSA2 = 1; } while(0)
#define DOWN_SW_SetDigitalMode()     do { ANSELAbits.ANSA2 = 0; } while(0)
// get/set IO_RA3 aliases
#define ENT_SW_TRIS                 TRISAbits.TRISA3
#define ENT_SW_LAT                  LATAbits.LATA3
#define ENT_SW_PORT                 PORTAbits.RA3
#define ENT_SW_WPU                  WPUAbits.WPUA3
#define ENT_SW_OD                   ODCONAbits.ODCA3
#define ENT_SW_ANS                  ANSELAbits.ANSA3
#define ENT_SW_SetHigh()            do { LATAbits.LATA3 = 1; } while(0)
#define ENT_SW_SetLow()             do { LATAbits.LATA3 = 0; } while(0)
#define ENT_SW_Toggle()             do { LATAbits.LATA3 = ~LATAbits.LATA3; } while(0)
#define ENT_SW_GetValue()           PORTAbits.RA3
#define ENT_SW_SetDigitalInput()    do { TRISAbits.TRISA3 = 1; } while(0)
#define ENT_SW_SetDigitalOutput()   do { TRISAbits.TRISA3 = 0; } while(0)
#define ENT_SW_SetPullup()          do { WPUAbits.WPUA3 = 1; } while(0)
#define ENT_SW_ResetPullup()        do { WPUAbits.WPUA3 = 0; } while(0)
#define ENT_SW_SetPushPull()        do { ODCONAbits.ODCA3 = 0; } while(0)
#define ENT_SW_SetOpenDrain()       do { ODCONAbits.ODCA3 = 1; } while(0)
#define ENT_SW_SetAnalogMode()      do { ANSELAbits.ANSA3 = 1; } while(0)
#define ENT_SW_SetDigitalMode()     do { ANSELAbits.ANSA3 = 0; } while(0)
// get/set IO_RA4 aliases
#define ESC_SW_TRIS                 TRISAbits.TRISA4
#define ESC_SW_LAT                  LATAbits.LATA4
#define ESC_SW_PORT                 PORTAbits.RA4
#define ESC_SW_WPU                  WPUAbits.WPUA4
#define ESC_SW_OD                   ODCONAbits.ODCA4
#define ESC_SW_ANS                  ANSELAbits.ANSA4
#define ESC_SW_SetHigh()            do { LATAbits.LATA4 = 1; } while(0)
#define ESC_SW_SetLow()             do { LATAbits.LATA4 = 0; } while(0)
#define ESC_SW_Toggle()             do { LATAbits.LATA4 = ~LATAbits.LATA4; } while(0)
#define ESC_SW_GetValue()           PORTAbits.RA4
#define ESC_SW_SetDigitalInput()    do { TRISAbits.TRISA4 = 1; } while(0)
#define ESC_SW_SetDigitalOutput()   do { TRISAbits.TRISA4 = 0; } while(0)
#define ESC_SW_SetPullup()          do { WPUAbits.WPUA4 = 1; } while(0)
#define ESC_SW_ResetPullup()        do { WPUAbits.WPUA4 = 0; } while(0)
#define ESC_SW_SetPushPull()        do { ODCONAbits.ODCA4 = 0; } while(0)
#define ESC_SW_SetOpenDrain()       do { ODCONAbits.ODCA4 = 1; } while(0)
#define ESC_SW_SetAnalogMode()      do { ANSELAbits.ANSA4 = 1; } while(0)
#define ESC_SW_SetDigitalMode()     do { ANSELAbits.ANSA4 = 0; } while(0)
// get/set IO_RA5 aliases
#define SET_RUN_SW_TRIS                 TRISAbits.TRISA5
#define SET_RUN_SW_LAT                  LATAbits.LATA5
#define SET_RUN_SW_PORT                 PORTAbits.RA5
#define SET_RUN_SW_WPU                  WPUAbits.WPUA5
#define SET_RUN_SW_OD                   ODCONAbits.ODCA5
#define SET_RUN_SW_ANS                  ANSELAbits.ANSA5
#define SET_RUN_SW_SetHigh()            do { LATAbits.LATA5 = 1; } while(0)
#define SET_RUN_SW_SetLow()             do { LATAbits.LATA5 = 0; } while(0)
#define SET_RUN_SW_Toggle()             do { LATAbits.LATA5 = ~LATAbits.LATA5; } while(0)
#define SET_RUN_SW_GetValue()           PORTAbits.RA5
#define SET_RUN_SW_SetDigitalInput()    do { TRISAbits.TRISA5 = 1; } while(0)
#define SET_RUN_SW_SetDigitalOutput()   do { TRISAbits.TRISA5 = 0; } while(0)
#define SET_RUN_SW_SetPullup()          do { WPUAbits.WPUA5 = 1; } while(0)
#define SET_RUN_SW_ResetPullup()        do { WPUAbits.WPUA5 = 0; } while(0)
#define SET_RUN_SW_SetPushPull()        do { ODCONAbits.ODCA5 = 0; } while(0)
#define SET_RUN_SW_SetOpenDrain()       do { ODCONAbits.ODCA5 = 1; } while(0)
#define SET_RUN_SW_SetAnalogMode()      do { ANSELAbits.ANSA5 = 1; } while(0)
#define SET_RUN_SW_SetDigitalMode()     do { ANSELAbits.ANSA5 = 0; } while(0)
// get/set IO_RA6 aliases
#define LED_RUN_TRIS                 TRISAbits.TRISA6
#define LED_RUN_LAT                  LATAbits.LATA6
#define LED_RUN_PORT                 PORTAbits.RA6
#define LED_RUN_WPU                  WPUAbits.WPUA6
#define LED_RUN_OD                   ODCONAbits.ODCA6
#define LED_RUN_ANS                  ANSELAbits.ANSA6
#define LED_RUN_SetHigh()            do { LATAbits.LATA6 = 1; } while(0)
#define LED_RUN_SetLow()             do { LATAbits.LATA6 = 0; } while(0)
#define LED_RUN_Toggle()             do { LATAbits.LATA6 = ~LATAbits.LATA6; } while(0)
#define LED_RUN_GetValue()           PORTAbits.RA6
#define LED_RUN_SetDigitalInput()    do { TRISAbits.TRISA6 = 1; } while(0)
#define LED_RUN_SetDigitalOutput()   do { TRISAbits.TRISA6 = 0; } while(0)
#define LED_RUN_SetPullup()          do { WPUAbits.WPUA6 = 1; } while(0)
#define LED_RUN_ResetPullup()        do { WPUAbits.WPUA6 = 0; } while(0)
#define LED_RUN_SetPushPull()        do { ODCONAbits.ODCA6 = 0; } while(0)
#define LED_RUN_SetOpenDrain()       do { ODCONAbits.ODCA6 = 1; } while(0)
#define LED_RUN_SetAnalogMode()      do { ANSELAbits.ANSA6 = 1; } while(0)
#define LED_RUN_SetDigitalMode()     do { ANSELAbits.ANSA6 = 0; } while(0)
// get/set IO_RA7 aliases
#define LED_ERROR_TRIS                 TRISAbits.TRISA7
#define LED_ERROR_LAT                  LATAbits.LATA7
#define LED_ERROR_PORT                 PORTAbits.RA7
#define LED_ERROR_WPU                  WPUAbits.WPUA7
#define LED_ERROR_OD                   ODCONAbits.ODCA7
#define LED_ERROR_ANS                  ANSELAbits.ANSA7
#define LED_ERROR_SetHigh()            do { LATAbits.LATA7 = 1; } while(0)
#define LED_ERROR_SetLow()             do { LATAbits.LATA7 = 0; } while(0)
#define LED_ERROR_Toggle()             do { LATAbits.LATA7 = ~LATAbits.LATA7; } while(0)
#define LED_ERROR_GetValue()           PORTAbits.RA7
#define LED_ERROR_SetDigitalInput()    do { TRISAbits.TRISA7 = 1; } while(0)
#define LED_ERROR_SetDigitalOutput()   do { TRISAbits.TRISA7 = 0; } while(0)
#define LED_ERROR_SetPullup()          do { WPUAbits.WPUA7 = 1; } while(0)
#define LED_ERROR_ResetPullup()        do { WPUAbits.WPUA7 = 0; } while(0)
#define LED_ERROR_SetPushPull()        do { ODCONAbits.ODCA7 = 0; } while(0)
#define LED_ERROR_SetOpenDrain()       do { ODCONAbits.ODCA7 = 1; } while(0)
#define LED_ERROR_SetAnalogMode()      do { ANSELAbits.ANSA7 = 1; } while(0)
#define LED_ERROR_SetDigitalMode()     do { ANSELAbits.ANSA7 = 0; } while(0)
// get/set IO_RB0 aliases
#define LED_RS485_TRIS                 TRISBbits.TRISB0
#define LED_RS485_LAT                  LATBbits.LATB0
#define LED_RS485_PORT                 PORTBbits.RB0
#define LED_RS485_WPU                  WPUBbits.WPUB0
#define LED_RS485_OD                   ODCONBbits.ODCB0
#define LED_RS485_ANS                  ANSELBbits.ANSB0
#define LED_RS485_SetHigh()            do { LATBbits.LATB0 = 1; } while(0)
#define LED_RS485_SetLow()             do { LATBbits.LATB0 = 0; } while(0)
#define LED_RS485_Toggle()             do { LATBbits.LATB0 = ~LATBbits.LATB0; } while(0)
#define LED_RS485_GetValue()           PORTBbits.RB0
#define LED_RS485_SetDigitalInput()    do { TRISBbits.TRISB0 = 1; } while(0)
#define LED_RS485_SetDigitalOutput()   do { TRISBbits.TRISB0 = 0; } while(0)
#define LED_RS485_SetPullup()          do { WPUBbits.WPUB0 = 1; } while(0)
#define LED_RS485_ResetPullup()        do { WPUBbits.WPUB0 = 0; } while(0)
#define LED_RS485_SetPushPull()        do { ODCONBbits.ODCB0 = 0; } while(0)
#define LED_RS485_SetOpenDrain()       do { ODCONBbits.ODCB0 = 1; } while(0)
#define LED_RS485_SetAnalogMode()      do { ANSELBbits.ANSB0 = 1; } while(0)
#define LED_RS485_SetDigitalMode()     do { ANSELBbits.ANSB0 = 0; } while(0)
// get/set IO_RB1 aliases
#define IO_RB1_TRIS                 TRISBbits.TRISB1
#define IO_RB1_LAT                  LATBbits.LATB1
#define IO_RB1_PORT                 PORTBbits.RB1
#define IO_RB1_WPU                  WPUBbits.WPUB1
#define IO_RB1_OD                   ODCONBbits.ODCB1
#define IO_RB1_ANS                  ANSELBbits.ANSB1
#define IO_RB1_SetHigh()            do { LATBbits.LATB1 = 1; } while(0)
#define IO_RB1_SetLow()             do { LATBbits.LATB1 = 0; } while(0)
#define IO_RB1_Toggle()             do { LATBbits.LATB1 = ~LATBbits.LATB1; } while(0)
#define IO_RB1_GetValue()           PORTBbits.RB1
#define IO_RB1_SetDigitalInput()    do { TRISBbits.TRISB1 = 1; } while(0)
#define IO_RB1_SetDigitalOutput()   do { TRISBbits.TRISB1 = 0; } while(0)
#define IO_RB1_SetPullup()          do { WPUBbits.WPUB1 = 1; } while(0)
#define IO_RB1_ResetPullup()        do { WPUBbits.WPUB1 = 0; } while(0)
#define IO_RB1_SetPushPull()        do { ODCONBbits.ODCB1 = 0; } while(0)
#define IO_RB1_SetOpenDrain()       do { ODCONBbits.ODCB1 = 1; } while(0)
#define IO_RB1_SetAnalogMode()      do { ANSELBbits.ANSB1 = 1; } while(0)
#define IO_RB1_SetDigitalMode()     do { ANSELBbits.ANSB1 = 0; } while(0)
// get/set IO_RB2 aliases
#define IO_RB2_TRIS                 TRISBbits.TRISB2
#define IO_RB2_LAT                  LATBbits.LATB2
#define IO_RB2_PORT                 PORTBbits.RB2
#define IO_RB2_WPU                  WPUBbits.WPUB2
#define IO_RB2_OD                   ODCONBbits.ODCB2
#define IO_RB2_ANS                  ANSELBbits.ANSB2
#define IO_RB2_SetHigh()            do { LATBbits.LATB2 = 1; } while(0)
#define IO_RB2_SetLow()             do { LATBbits.LATB2 = 0; } while(0)
#define IO_RB2_Toggle()             do { LATBbits.LATB2 = ~LATBbits.LATB2; } while(0)
#define IO_RB2_GetValue()           PORTBbits.RB2
#define IO_RB2_SetDigitalInput()    do { TRISBbits.TRISB2 = 1; } while(0)
#define IO_RB2_SetDigitalOutput()   do { TRISBbits.TRISB2 = 0; } while(0)
#define IO_RB2_SetPullup()          do { WPUBbits.WPUB2 = 1; } while(0)
#define IO_RB2_ResetPullup()        do { WPUBbits.WPUB2 = 0; } while(0)
#define IO_RB2_SetPushPull()        do { ODCONBbits.ODCB2 = 0; } while(0)
#define IO_RB2_SetOpenDrain()       do { ODCONBbits.ODCB2 = 1; } while(0)
#define IO_RB2_SetAnalogMode()      do { ANSELBbits.ANSB2 = 1; } while(0)
#define IO_RB2_SetDigitalMode()     do { ANSELBbits.ANSB2 = 0; } while(0)
// get/set IO_RB3 aliases
#define IO_RB3_TRIS                 TRISBbits.TRISB3
#define IO_RB3_LAT                  LATBbits.LATB3
#define IO_RB3_PORT                 PORTBbits.RB3
#define IO_RB3_WPU                  WPUBbits.WPUB3
#define IO_RB3_OD                   ODCONBbits.ODCB3
#define IO_RB3_ANS                  ANSELBbits.ANSB3
#define IO_RB3_SetHigh()            do { LATBbits.LATB3 = 1; } while(0)
#define IO_RB3_SetLow()             do { LATBbits.LATB3 = 0; } while(0)
#define IO_RB3_Toggle()             do { LATBbits.LATB3 = ~LATBbits.LATB3; } while(0)
#define IO_RB3_GetValue()           PORTBbits.RB3
#define IO_RB3_SetDigitalInput()    do { TRISBbits.TRISB3 = 1; } while(0)
#define IO_RB3_SetDigitalOutput()   do { TRISBbits.TRISB3 = 0; } while(0)
#define IO_RB3_SetPullup()          do { WPUBbits.WPUB3 = 1; } while(0)
#define IO_RB3_ResetPullup()        do { WPUBbits.WPUB3 = 0; } while(0)
#define IO_RB3_SetPushPull()        do { ODCONBbits.ODCB3 = 0; } while(0)
#define IO_RB3_SetOpenDrain()       do { ODCONBbits.ODCB3 = 1; } while(0)
#define IO_RB3_SetAnalogMode()      do { ANSELBbits.ANSB3 = 1; } while(0)
#define IO_RB3_SetDigitalMode()     do { ANSELBbits.ANSB3 = 0; } while(0)
// get/set IO_RB4 aliases
#define IO_RB4_TRIS                 TRISBbits.TRISB4
#define IO_RB4_LAT                  LATBbits.LATB4
#define IO_RB4_PORT                 PORTBbits.RB4
#define IO_RB4_WPU                  WPUBbits.WPUB4
#define IO_RB4_OD                   ODCONBbits.ODCB4
#define IO_RB4_ANS                  ANSELBbits.ANSB4
#define IO_RB4_SetHigh()            do { LATBbits.LATB4 = 1; } while(0)
#define IO_RB4_SetLow()             do { LATBbits.LATB4 = 0; } while(0)
#define IO_RB4_Toggle()             do { LATBbits.LATB4 = ~LATBbits.LATB4; } while(0)
#define IO_RB4_GetValue()           PORTBbits.RB4
#define IO_RB4_SetDigitalInput()    do { TRISBbits.TRISB4 = 1; } while(0)
#define IO_RB4_SetDigitalOutput()   do { TRISBbits.TRISB4 = 0; } while(0)
#define IO_RB4_SetPullup()          do { WPUBbits.WPUB4 = 1; } while(0)
#define IO_RB4_ResetPullup()        do { WPUBbits.WPUB4 = 0; } while(0)
#define IO_RB4_SetPushPull()        do { ODCONBbits.ODCB4 = 0; } while(0)
#define IO_RB4_SetOpenDrain()       do { ODCONBbits.ODCB4 = 1; } while(0)
#define IO_RB4_SetAnalogMode()      do { ANSELBbits.ANSB4 = 1; } while(0)
#define IO_RB4_SetDigitalMode()     do { ANSELBbits.ANSB4 = 0; } while(0)
// get/set IO_RB5 aliases
#define RS485_DE_TRIS                 TRISBbits.TRISB5
#define RS485_DE_LAT                  LATBbits.LATB5
#define RS485_DE_PORT                 PORTBbits.RB5
#define RS485_DE_WPU                  WPUBbits.WPUB5
#define RS485_DE_OD                   ODCONBbits.ODCB5
#define RS485_DE_ANS                  ANSELBbits.ANSB5
#define RS485_DE_SetHigh()            do { LATBbits.LATB5 = 1; } while(0)
#define RS485_DE_SetLow()             do { LATBbits.LATB5 = 0; } while(0)
#define RS485_DE_Toggle()             do { LATBbits.LATB5 = ~LATBbits.LATB5; } while(0)
#define RS485_DE_GetValue()           PORTBbits.RB5
#define RS485_DE_SetDigitalInput()    do { TRISBbits.TRISB5 = 1; } while(0)
#define RS485_DE_SetDigitalOutput()   do { TRISBbits.TRISB5 = 0; } while(0)
#define RS485_DE_SetPullup()          do { WPUBbits.WPUB5 = 1; } while(0)
#define RS485_DE_ResetPullup()        do { WPUBbits.WPUB5 = 0; } while(0)
#define RS485_DE_SetPushPull()        do { ODCONBbits.ODCB5 = 0; } while(0)
#define RS485_DE_SetOpenDrain()       do { ODCONBbits.ODCB5 = 1; } while(0)
#define RS485_DE_SetAnalogMode()      do { ANSELBbits.ANSB5 = 1; } while(0)
#define RS485_DE_SetDigitalMode()     do { ANSELBbits.ANSB5 = 0; } while(0)
// get/set IO_RC0 aliases
#define INPUT1_TRIS                 TRISCbits.TRISC0
#define INPUT1_LAT                  LATCbits.LATC0
#define INPUT1_PORT                 PORTCbits.RC0
#define INPUT1_WPU                  WPUCbits.WPUC0
#define INPUT1_OD                   ODCONCbits.ODCC0
#define INPUT1_ANS                  ANSELCbits.ANSC0
#define INPUT1_SetHigh()            do { LATCbits.LATC0 = 1; } while(0)
#define INPUT1_SetLow()             do { LATCbits.LATC0 = 0; } while(0)
#define INPUT1_Toggle()             do { LATCbits.LATC0 = ~LATCbits.LATC0; } while(0)
#define INPUT1_GetValue()           PORTCbits.RC0
#define INPUT1_SetDigitalInput()    do { TRISCbits.TRISC0 = 1; } while(0)
#define INPUT1_SetDigitalOutput()   do { TRISCbits.TRISC0 = 0; } while(0)
#define INPUT1_SetPullup()          do { WPUCbits.WPUC0 = 1; } while(0)
#define INPUT1_ResetPullup()        do { WPUCbits.WPUC0 = 0; } while(0)
#define INPUT1_SetPushPull()        do { ODCONCbits.ODCC0 = 0; } while(0)
#define INPUT1_SetOpenDrain()       do { ODCONCbits.ODCC0 = 1; } while(0)
#define INPUT1_SetAnalogMode()      do { ANSELCbits.ANSC0 = 1; } while(0)
#define INPUT1_SetDigitalMode()     do { ANSELCbits.ANSC0 = 0; } while(0)
// get/set IO_RC1 aliases
#define INPUT2_TRIS                 TRISCbits.TRISC1
#define INPUT2_LAT                  LATCbits.LATC1
#define INPUT2_PORT                 PORTCbits.RC1
#define INPUT2_WPU                  WPUCbits.WPUC1
#define INPUT2_OD                   ODCONCbits.ODCC1
#define INPUT2_ANS                  ANSELCbits.ANSC1
#define INPUT2_SetHigh()            do { LATCbits.LATC1 = 1; } while(0)
#define INPUT2_SetLow()             do { LATCbits.LATC1 = 0; } while(0)
#define INPUT2_Toggle()             do { LATCbits.LATC1 = ~LATCbits.LATC1; } while(0)
#define INPUT2_GetValue()           PORTCbits.RC1
#define INPUT2_SetDigitalInput()    do { TRISCbits.TRISC1 = 1; } while(0)
#define INPUT2_SetDigitalOutput()   do { TRISCbits.TRISC1 = 0; } while(0)
#define INPUT2_SetPullup()          do { WPUCbits.WPUC1 = 1; } while(0)
#define INPUT2_ResetPullup()        do { WPUCbits.WPUC1 = 0; } while(0)
#define INPUT2_SetPushPull()        do { ODCONCbits.ODCC1 = 0; } while(0)
#define INPUT2_SetOpenDrain()       do { ODCONCbits.ODCC1 = 1; } while(0)
#define INPUT2_SetAnalogMode()      do { ANSELCbits.ANSC1 = 1; } while(0)
#define INPUT2_SetDigitalMode()     do { ANSELCbits.ANSC1 = 0; } while(0)
// get/set IO_RC2 aliases
#define INPUT3_TRIS                 TRISCbits.TRISC2
#define INPUT3_LAT                  LATCbits.LATC2
#define INPUT3_PORT                 PORTCbits.RC2
#define INPUT3_WPU                  WPUCbits.WPUC2
#define INPUT3_OD                   ODCONCbits.ODCC2
#define INPUT3_ANS                  ANSELCbits.ANSC2
#define INPUT3_SetHigh()            do { LATCbits.LATC2 = 1; } while(0)
#define INPUT3_SetLow()             do { LATCbits.LATC2 = 0; } while(0)
#define INPUT3_Toggle()             do { LATCbits.LATC2 = ~LATCbits.LATC2; } while(0)
#define INPUT3_GetValue()           PORTCbits.RC2
#define INPUT3_SetDigitalInput()    do { TRISCbits.TRISC2 = 1; } while(0)
#define INPUT3_SetDigitalOutput()   do { TRISCbits.TRISC2 = 0; } while(0)
#define INPUT3_SetPullup()          do { WPUCbits.WPUC2 = 1; } while(0)
#define INPUT3_ResetPullup()        do { WPUCbits.WPUC2 = 0; } while(0)
#define INPUT3_SetPushPull()        do { ODCONCbits.ODCC2 = 0; } while(0)
#define INPUT3_SetOpenDrain()       do { ODCONCbits.ODCC2 = 1; } while(0)
#define INPUT3_SetAnalogMode()      do { ANSELCbits.ANSC2 = 1; } while(0)
#define INPUT3_SetDigitalMode()     do { ANSELCbits.ANSC2 = 0; } while(0)
// get/set IO_RC3 aliases
#define INPUT4_TRIS                 TRISCbits.TRISC3
#define INPUT4_LAT                  LATCbits.LATC3
#define INPUT4_PORT                 PORTCbits.RC3
#define INPUT4_WPU                  WPUCbits.WPUC3
#define INPUT4_OD                   ODCONCbits.ODCC3
#define INPUT4_ANS                  ANSELCbits.ANSC3
#define INPUT4_SetHigh()            do { LATCbits.LATC3 = 1; } while(0)
#define INPUT4_SetLow()             do { LATCbits.LATC3 = 0; } while(0)
#define INPUT4_Toggle()             do { LATCbits.LATC3 = ~LATCbits.LATC3; } while(0)
#define INPUT4_GetValue()           PORTCbits.RC3
#define INPUT4_SetDigitalInput()    do { TRISCbits.TRISC3 = 1; } while(0)
#define INPUT4_SetDigitalOutput()   do { TRISCbits.TRISC3 = 0; } while(0)
#define INPUT4_SetPullup()          do { WPUCbits.WPUC3 = 1; } while(0)
#define INPUT4_ResetPullup()        do { WPUCbits.WPUC3 = 0; } while(0)
#define INPUT4_SetPushPull()        do { ODCONCbits.ODCC3 = 0; } while(0)
#define INPUT4_SetOpenDrain()       do { ODCONCbits.ODCC3 = 1; } while(0)
#define INPUT4_SetAnalogMode()      do { ANSELCbits.ANSC3 = 1; } while(0)
#define INPUT4_SetDigitalMode()     do { ANSELCbits.ANSC3 = 0; } while(0)
// get/set IO_RC4 aliases
#define INPUT5_TRIS                 TRISCbits.TRISC4
#define INPUT5_LAT                  LATCbits.LATC4
#define INPUT5_PORT                 PORTCbits.RC4
#define INPUT5_WPU                  WPUCbits.WPUC4
#define INPUT5_OD                   ODCONCbits.ODCC4
#define INPUT5_ANS                  ANSELCbits.ANSC4
#define INPUT5_SetHigh()            do { LATCbits.LATC4 = 1; } while(0)
#define INPUT5_SetLow()             do { LATCbits.LATC4 = 0; } while(0)
#define INPUT5_Toggle()             do { LATCbits.LATC4 = ~LATCbits.LATC4; } while(0)
#define INPUT5_GetValue()           PORTCbits.RC4
#define INPUT5_SetDigitalInput()    do { TRISCbits.TRISC4 = 1; } while(0)
#define INPUT5_SetDigitalOutput()   do { TRISCbits.TRISC4 = 0; } while(0)
#define INPUT5_SetPullup()          do { WPUCbits.WPUC4 = 1; } while(0)
#define INPUT5_ResetPullup()        do { WPUCbits.WPUC4 = 0; } while(0)
#define INPUT5_SetPushPull()        do { ODCONCbits.ODCC4 = 0; } while(0)
#define INPUT5_SetOpenDrain()       do { ODCONCbits.ODCC4 = 1; } while(0)
#define INPUT5_SetAnalogMode()      do { ANSELCbits.ANSC4 = 1; } while(0)
#define INPUT5_SetDigitalMode()     do { ANSELCbits.ANSC4 = 0; } while(0)
// get/set IO_RC5 aliases
#define INPUT6_TRIS                 TRISCbits.TRISC5
#define INPUT6_LAT                  LATCbits.LATC5
#define INPUT6_PORT                 PORTCbits.RC5
#define INPUT6_WPU                  WPUCbits.WPUC5
#define INPUT6_OD                   ODCONCbits.ODCC5
#define INPUT6_ANS                  ANSELCbits.ANSC5
#define INPUT6_SetHigh()            do { LATCbits.LATC5 = 1; } while(0)
#define INPUT6_SetLow()             do { LATCbits.LATC5 = 0; } while(0)
#define INPUT6_Toggle()             do { LATCbits.LATC5 = ~LATCbits.LATC5; } while(0)
#define INPUT6_GetValue()           PORTCbits.RC5
#define INPUT6_SetDigitalInput()    do { TRISCbits.TRISC5 = 1; } while(0)
#define INPUT6_SetDigitalOutput()   do { TRISCbits.TRISC5 = 0; } while(0)
#define INPUT6_SetPullup()          do { WPUCbits.WPUC5 = 1; } while(0)
#define INPUT6_ResetPullup()        do { WPUCbits.WPUC5 = 0; } while(0)
#define INPUT6_SetPushPull()        do { ODCONCbits.ODCC5 = 0; } while(0)
#define INPUT6_SetOpenDrain()       do { ODCONCbits.ODCC5 = 1; } while(0)
#define INPUT6_SetAnalogMode()      do { ANSELCbits.ANSC5 = 1; } while(0)
#define INPUT6_SetDigitalMode()     do { ANSELCbits.ANSC5 = 0; } while(0)
// get/set IO_RC6 aliases
#define INPUT7_TRIS                 TRISCbits.TRISC6
#define INPUT7_LAT                  LATCbits.LATC6
#define INPUT7_PORT                 PORTCbits.RC6
#define INPUT7_WPU                  WPUCbits.WPUC6
#define INPUT7_OD                   ODCONCbits.ODCC6
#define INPUT7_ANS                  ANSELCbits.ANSC6
#define INPUT7_SetHigh()            do { LATCbits.LATC6 = 1; } while(0)
#define INPUT7_SetLow()             do { LATCbits.LATC6 = 0; } while(0)
#define INPUT7_Toggle()             do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define INPUT7_GetValue()           PORTCbits.RC6
#define INPUT7_SetDigitalInput()    do { TRISCbits.TRISC6 = 1; } while(0)
#define INPUT7_SetDigitalOutput()   do { TRISCbits.TRISC6 = 0; } while(0)
#define INPUT7_SetPullup()          do { WPUCbits.WPUC6 = 1; } while(0)
#define INPUT7_ResetPullup()        do { WPUCbits.WPUC6 = 0; } while(0)
#define INPUT7_SetPushPull()        do { ODCONCbits.ODCC6 = 0; } while(0)
#define INPUT7_SetOpenDrain()       do { ODCONCbits.ODCC6 = 1; } while(0)
#define INPUT7_SetAnalogMode()      do { ANSELCbits.ANSC6 = 1; } while(0)
#define INPUT7_SetDigitalMode()     do { ANSELCbits.ANSC6 = 0; } while(0)
// get/set IO_RC7 aliases
#define INPUT8_TRIS                 TRISCbits.TRISC7
#define INPUT8_LAT                  LATCbits.LATC7
#define INPUT8_PORT                 PORTCbits.RC7
#define INPUT8_WPU                  WPUCbits.WPUC7
#define INPUT8_OD                   ODCONCbits.ODCC7
#define INPUT8_ANS                  ANSELCbits.ANSC7
#define INPUT8_SetHigh()            do { LATCbits.LATC7 = 1; } while(0)
#define INPUT8_SetLow()             do { LATCbits.LATC7 = 0; } while(0)
#define INPUT8_Toggle()             do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define INPUT8_GetValue()           PORTCbits.RC7
#define INPUT8_SetDigitalInput()    do { TRISCbits.TRISC7 = 1; } while(0)
#define INPUT8_SetDigitalOutput()   do { TRISCbits.TRISC7 = 0; } while(0)
#define INPUT8_SetPullup()          do { WPUCbits.WPUC7 = 1; } while(0)
#define INPUT8_ResetPullup()        do { WPUCbits.WPUC7 = 0; } while(0)
#define INPUT8_SetPushPull()        do { ODCONCbits.ODCC7 = 0; } while(0)
#define INPUT8_SetOpenDrain()       do { ODCONCbits.ODCC7 = 1; } while(0)
#define INPUT8_SetAnalogMode()      do { ANSELCbits.ANSC7 = 1; } while(0)
#define INPUT8_SetDigitalMode()     do { ANSELCbits.ANSC7 = 0; } while(0)
/**
 * @ingroup  pinsdriver
 * @brief GPIO and peripheral I/O initialization
 * @param none
 * @return none
 */
void PIN_MANAGER_Initialize (void);

/**
 * @ingroup  pinsdriver
 * @brief Interrupt on Change Handling routine
 * @param none
 * @return none
 */
void PIN_MANAGER_IOC(void);


#endif // PINS_H
/**
 End of File
*/