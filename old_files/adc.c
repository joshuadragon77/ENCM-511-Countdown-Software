#include "adc.h"

void setup_ADC(void){    
    AD1PCFGbits.PCFG5 = 0;
    
    AD1CON1bits.FORM = 0b00;
    AD1CON1bits.SSRC = 0b111;
    AD1CON1bits.ASAM = 0;
    
    AD1CON2bits.VCFG = 0b000;
    AD1CON2bits.CSCNA = 0;
    AD1CON2bits.SMPI = 0b0000;
    AD1CON2bits.BUFM = 0;
    AD1CON2bits.ALTS = 0;
    
    AD1CON3bits.ADRC = 0;
    AD1CON3bits.SAMC = 0b01010;
    AD1CON3bits.ADCS = 0b00000;
    
    AD1CHSbits.CH0NA = 0;
    AD1CHSbits.CH0SA = 0b0101;
    AD1CSSL = 0;
}

uint16_t do_ADC(void){
    uint16_t ADCvalue;
    
    AD1CON1bits.ADON = 1;
    AD1CON1bits.SAMP = 1;
    
    while(AD1CON1bits.DONE == 0){
        
    }
    ADCvalue = ADC1BUF0;
    AD1CON1bits.SAMP = 0;
    AD1CON1bits.ADON = 0;
    return(ADCvalue);
}
