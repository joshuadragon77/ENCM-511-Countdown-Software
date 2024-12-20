/**
 * @file io.c
 * @author TheJades (thejadesisfat77@gmail.com)
 * @brief IO Driver Component for the PIC24F16KA101. Handles PWM, IO Operations of LEDs and Buttons.
 * @version 0.1
 * @date 2024-11-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "io.h"

uint8_t pwmLEDEnabled = 0;
//1/75Hz×(500000Hz / 2)
// 75Hz cycle
uint16_t maximumHardwareCycle = 3333;
uint16_t onHardwareDutyCycle = 25;
uint16_t offHardwareDutyCycle = 25;

uint8_t animationState = 1;

float currentDutyCycle = 0;

uint8_t animationInterval = 0;

uint8_t buttonStateQueue = 0;
uint8_t buttonState = 0;


void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void){
    IFS0bits.T1IF = 0;
    animationInterval += 1;

    // optimized the fuck out of this one to reduce execution time.
    // This utilizes a bistable interval that changes state each time an interrupt happens.
    // This allows the creation of the software controlled PWM.
    // However, due its frequent nature, precaution and immense efficiency must be performed to reduce impact on CPU.
    if (!pwmLEDEnabled)
        return;

    if (!LATBbits.LATB8){
        PR1 = offHardwareDutyCycle;
    }else{
        PR1 = onHardwareDutyCycle;
    }
    LATBbits.LATB8 = !LATBbits.LATB8;
}

void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void){
    IFS1bits.CNIF = 0;
    buttonState = (!(PORTA & 0b10000) << 2) | (!(PORTB & 0b10000) << 1) | !(PORTA & 0b100);
    buttonStateQueue |= (!(PORTA & 0b10000) << 2) | (!(PORTB & 0b10000) << 1) | !(PORTA & 0b100);
    SET_BLOCKING_FLAG(BLOCKFLAG_ARRAY_ButtonPress);
}

void enablePWMLed(){
    T1CONbits.TON = 1;
    pwmLEDEnabled = 1;
}

void disablePWMLed(){
    T1CONbits.TON = 0;
    pwmLEDEnabled = 0;
}

uint8_t getPWMLedState(){
    return pwmLEDEnabled;
}

void turnONLed(){
    LATBbits.LATB8 = 0;
}

void toggleLED(){
    LATBbits.LATB8 = !LATBbits.LATB8;
}

void turnOFFLed(){
    LATBbits.LATB8 = 1;
}

uint8_t getLedState(){
    return !LATBbits.LATB8;
}

void setDutyCycle(float dutyCycle){

    float linearLikeProfileFactor = dutyCycle * dutyCycle;
    currentDutyCycle = dutyCycle;


    // The value 2500 is calculated from the PWM optimally performs well at 100Hz to the human eye.
    // Minimum of 50 ticks between edge to avoid the interrupt robbing the program of all CPU.
    onHardwareDutyCycle = (maximumHardwareCycle - 100) * linearLikeProfileFactor + 50;
    offHardwareDutyCycle = maximumHardwareCycle - onHardwareDutyCycle;
}

float getDutyCycle(){
    return currentDutyCycle;
}

void animationWaveyTick(){
    if (animationState){
        float timeFactor = animationInterval / 255.f;

        // Uses the onboard animationInterval calculcated by Timer 1 operations.
        // Shove that into a mathematical function to get some coolio animations.

        float animationFactor = (cosf(2 * M_PI * timeFactor) + 1) / 2;

        setDutyCycle(animationFactor);
    }
}

void enableAnimation(){
    animationState = 1;
}

void disableAnimation(){
    animationState = 0;
}

void buttonBufferClear(){
    buttonStateQueue = 0;
}

uint8_t isButtonPress(Button button){
    uint8_t pressed = (1 << button) & buttonStateQueue;
    if (pressed){
        buttonStateQueue &= !(1 << button);
    }
    return pressed;
}

uint8_t isButtonHeldDown(Button button){
    uint8_t pressed = (1 << button) & buttonState;
    return pressed;
}


void initIO(){
    AD1PCFG = 0xFFFF; /* keep this line as it sets I/O pins that can also be analog to be digital */

    T1CONbits.TCKPS0 = 0;
    T1CONbits.TCKPS1 = 0;
    T1CONbits.TCS = 0;
    T1CONbits.TON = 0;

    IPC0bits.T1IP = 6;
    IEC0bits.T1IE = 1;
    
    LATBbits.LATB8 = 1;
    TRISBbits.TRISB8 = 0;
    
    TRISAbits.TRISA4 = 1;
    CNPU1bits.CN0PUE = 1;
    CNEN1bits.CN0IE = 1;
    
    TRISBbits.TRISB4 = 1;
    CNPU1bits.CN1PUE = 1;
    CNEN1bits.CN1IE = 1;
    
    TRISAbits.TRISA2 = 1;
    CNPU2bits.CN30PUE = 1;
    CNEN2bits.CN30IE = 1;
    
    IPC4bits.CNIP = 1;
    IFS1bits.CNIF = 0;
    IEC1bits.CNIE = 1;
    

}