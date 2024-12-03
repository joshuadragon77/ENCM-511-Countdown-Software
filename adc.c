/**
 * @file adc.c
 * @author TheJades (thejadesisfat77@gmail.com)
 * @brief Simple ADC Driver for PIC24F16KA101. Redesigned from the previous iteration to remove unneccessary code and to make it more clean.
 * @version 0.1
 * @date 2024-11-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "adc.h"

uint16_t currentADCValue = 0;

void __attribute__((interrupt, no_auto_psv))  _ADC1Interrupt(void){
    IFS0bits.AD1IF = 0;
    currentADCValue = ADC1BUF0;
    SET_BLOCKING_FLAG(BLOCKFLAG_ARRAY_ADCRead);
}

uint16_t getADCReading(){
    return currentADCValue;
}

float getPercentageReading(){
    return currentADCValue / 1024.f;
}

void initADC(){
    
    AD1PCFGbits.PCFG5 = 0;
    AD1CON2bits.VCFG = 0;
    AD1CON3bits.ADCS = 0;
    AD1CON1bits.SSRC = 0b111;
    AD1CON3bits.SAMC = 31;
    AD1CON1bits.FORM = 0;
    AD1CON2bits.SMPI = 0;

    AD1CHSbits.CH0SA = 0b0101;
    AD1CHSbits.CH0NA = 0;
    AD1CHSbits.CH0SB = 0b0101;
    AD1CHSbits.CH0NB = 0;

    IFS0bits.AD1IF = 0;
    IEC0bits.AD1IE = 1;
    IPC3bits.AD1IP = 3;


    AD1CON1bits.ADON = 1;
    AD1CON1bits.ASAM = 1;


}