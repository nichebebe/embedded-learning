/**
 * ADC Type Definitions Header File
 *
 * @file adc_types.h
 *
 * @defgroup adc ADC
 *
 * @brief This file provides type definitions for the Analog-to-Digital Converter (ADC) module.
 *
 * @version ADC Driver Version 3.0.0
 * 
 * @version ADC Package Version 4.0.0
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

#ifndef ADC_TYPES_H
#define	ADC_TYPES_H

// Section: Macros

/**
 @ingroup adc
 @brief ADC resolution value.
*/
#define ADC_RESOLUTION 10U

/**
 @ingroup adc
 @brief ADC bit set value.
*/
#define ADC_BIT_SET 1U

/**
 @ingroup adc
 @brief ADC bit clear value.
*/
#define ADC_BIT_CLEAR 0U

// Section: Types

/**
 @ingroup adc
 @typedef adc_result_t
 @brief ADC conversion result type.
*/
typedef int16_t adc_result_t;

// Section: Enumerations

/**
 * @ingroup adc
 * @enum adc_channel_t
 * @brief Describes the available Analog-to-Digital Converter (ADC) channels.
*/
typedef enum
{
    ADC_CHANNEL_AVSS =  0x3b, /**< AVSS*/ 
    ADC_CHANNEL_TEMP =  0x3c, /**< Temp*/ 
    ADC_CHANNEL_DAC1 =  0x3d, /**< DAC1*/ 
    ADC_CHANNEL_FVR_BUF1 =  0x3e, /**< FVR_BUF1*/ 
    ADC_CHANNEL_FVR_BUF2 =  0x3f, /**< FVR_BUF2*/ 
    ADC_CHANNEL_ANA0 =  0x0, /**< OFFSET_SW: RA0*/ 
    ADC_CHANNEL_ANC0 =  0x10, /**< INPUT1: RC0*/ 
    ADC_CHANNEL_ANC1 =  0x11, /**< INPUT2: RC1*/ 
    ADC_CHANNEL_ANC2 =  0x12, /**< INPUT3: RC2*/ 
    ADC_CHANNEL_ANC3 =  0x13, /**< INPUT4: RC3*/ 
    ADC_CHANNEL_ANC4 =  0x14, /**< INPUT5: RC4*/ 
    ADC_CHANNEL_ANC5 =  0x15, /**< INPUT6: RC5*/ 
    ADC_CHANNEL_ANC6 =  0x16, /**< INPUT7: RC6*/ 
    ADC_CHANNEL_ANC7 =  0x17 /**< INPUT8: RC7*/ 
} adc_channel_t;

/**
 * @ingroup adc
 * @typedef adc_trigger_source_t 
 * @brief Lists the ADC auto-trigger sources.
 */
typedef enum
{
    ADC_TRIGGER_SOURCE_DISABLED = 0x0, /**< Disabled*/  
    ADC_TRIGGER_SOURCE_ADACTPPS = 0x1, /**< ADACTPPS*/  
    ADC_TRIGGER_SOURCE_TMR0 = 0x2, /**< TMR0*/  
    ADC_TRIGGER_SOURCE_TMR1 = 0x3, /**< TMR1*/  
    ADC_TRIGGER_SOURCE_TMR2 = 0x4, /**< TMR2*/  
    ADC_TRIGGER_SOURCE_CCP1 = 0x5, /**< CCP1*/  
    ADC_TRIGGER_SOURCE_CCP2 = 0x6, /**< CCP2*/  
    ADC_TRIGGER_SOURCE_PWM3 = 0x7, /**< PWM3*/  
    ADC_TRIGGER_SOURCE_PWM4 = 0x8, /**< PWM4*/  
    ADC_TRIGGER_SOURCE_PWM5 = 0x9, /**< PWM5*/  
    ADC_TRIGGER_SOURCE_PWM6 = 0xa, /**< PWM6*/  
    ADC_TRIGGER_SOURCE_NCO1 = 0xb, /**< NCO1*/  
    ADC_TRIGGER_SOURCE_CMP1 = 0xc, /**< CMP1*/  
    ADC_TRIGGER_SOURCE_CMP2 = 0xd, /**< CMP2*/  
    ADC_TRIGGER_SOURCE_IOCIF = 0xe, /**< IOCIF*/  
    ADC_TRIGGER_SOURCE_CLC1 = 0xf, /**< CLC1*/  
    ADC_TRIGGER_SOURCE_CLC2 = 0x10, /**< CLC2*/  
    ADC_TRIGGER_SOURCE_CLC3 = 0x11, /**< CLC3*/  
    ADC_TRIGGER_SOURCE_CLC4 = 0x12 /**< CLC4*/  
} adc_trigger_source_t ;

#endif // ADC_TYPES_H