/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support 
 * ----------------------------------------------------------------------------
 * Copyright (c) 2008, Atmel Corporation
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */

//------------------------------------------------------------------------------
/// \unit
///
/// !Purpose
///
/// Interface for configuration the Analog-to-Digital Converter (ADC) peripheral.
///
/// !Usage
///
/// -# Configurate the pins for ADC
/// -# Initialize the ADC with ADC_Initialize().
/// -# Select the active channel using ADC_EnableChannel()
/// -# Start the conversion with ADC_StartConversion()
//  -# Wait the end of the conversion by polling status with ADC_GetStatus()
//  -# Finally, get the converted data using ADC_GetConvertedData()
///
//------------------------------------------------------------------------------
#ifndef ADC_H
#define ADC_H

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//         Definitions
//------------------------------------------------------------------------------
#define ADC_CHANNEL_0 0
#define ADC_CHANNEL_1 1
#define ADC_CHANNEL_2 2
#define ADC_CHANNEL_3 3
#define ADC_CHANNEL_4 4
#define ADC_CHANNEL_5 5
#define ADC_CHANNEL_6 6
#define ADC_CHANNEL_7 7

//------------------------------------------------------------------------------
//         Exported functions
//------------------------------------------------------------------------------
extern void ADC_Initialize (AT91S_ADC *pAdc,
                     unsigned char idAdc,
                     unsigned char trgEn,
                     unsigned char trgSel,
                     unsigned char sleepMode,
                     unsigned char resolution,        
                     unsigned int mckClock,
                     unsigned int adcClock,
                     unsigned int startupTime,
                     unsigned int sampleAndHoldTime);
extern unsigned int ADC_GetModeReg(AT91S_ADC *pAdc);
extern void ADC_EnableChannel(AT91S_ADC *pAdc, unsigned int channel);
extern void ADC_DisableChannel (AT91S_ADC *pAdc, unsigned int channel);
extern unsigned int ADC_GetChannelStatus(AT91S_ADC *pAdc);
extern void ADC_StartConversion(AT91S_ADC *pAdc);
extern void ADC_SoftReset(AT91S_ADC *pAdc);
extern unsigned int ADC_GetLastConvertedData(AT91S_ADC *pAdc);
extern unsigned int ADC_GetConvertedData(AT91S_ADC *pAdc, unsigned int channel);
extern void ADC_EnableIt(AT91S_ADC *pAdc, unsigned int flag);
extern void ADC_EnableDataReadyIt(AT91S_ADC *pAdc);
extern void ADC_DisableIt(AT91S_ADC *pAdc, unsigned int flag);
extern unsigned int ADC_GetStatus(AT91S_ADC *pAdc);
extern unsigned int ADC_GetInterruptMaskStatus(AT91S_ADC *pAdc);
extern unsigned int ADC_IsInterruptMasked(AT91S_ADC *pAdc, unsigned int flag);
extern unsigned int ADC_IsStatusSet(AT91S_ADC *pAdc, unsigned int flag);
extern unsigned char ADC_IsChannelInterruptStatusSet(unsigned int adc_sr, 
                                              unsigned int channel);

#endif //#ifndef ADC_H
