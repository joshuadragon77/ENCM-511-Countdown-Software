/**
 * @file timer.c
 * @author TheJades (thejadesisfat77@gmail.com)
 * @brief Utilizies Timer 2 and Timer 3 to perform "mostly accurate" countdown and to add delay blocks.
 * @version 0.1
 * @date 2024-11-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "timer.h"

/**
 * @bug There is some time deviation. After 2 minutes, there is a 1 second deviation in which the PIC24 counts faster than the top of the line shitty iPhone.
 * 
 */
// 1 seconds × (500000Hz / 2 / 8)

uint16_t currentTimerDuration = 0;
uint16_t setTimerDuration = 0;

uint8_t minutes = 0;
uint8_t seconds = 0;

uint8_t running = 0;

void setTimer(uint16_t seconds){
    setTimerDuration = seconds;
}

void startTimer(){

    if (currentTimerDuration == 0){
        resetTimer();
    }

    running = 1;
    T2CONbits.TON = 1;
}

void stopTimer(){
    running = 0;
    T2CONbits.TON = 0;
}

void resetTimer(){
    TMR2 = 0;
    currentTimerDuration = setTimerDuration;

    minutes = currentTimerDuration / 60;
    seconds = currentTimerDuration % 60;

}

void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void){
    IFS0bits.T2IF = 0;

    if (currentTimerDuration > 0){
        currentTimerDuration --;
        if (seconds == 0){
            seconds = 60;
            if (minutes > 0)
                minutes --;
        }
        seconds --;
        SET_BLOCKING_FLAG(BLOCKFLAG_ARRAY_TimerCountdownInterval);
    }else{
        SET_BLOCKING_FLAG(BLOCKFLAG_ARRAY_TimerCountdownFinish);
        T2CONbits.TON = 0;
    }
}

void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void){
    IFS0bits.T3IF = 0;
    T3CONbits.TON = 0;
    SET_BLOCKING_FLAG(BLOCKFLAG_ARRAY_TimerDelayFinish);
}

void writeTimeToUARTConsole(){
    writeCharacter(minutes / 10 + 48);
    writeCharacter(minutes % 10 + 48);
    writeCharacter(':');
    writeCharacter(seconds / 10 + 48);
    writeCharacter(seconds % 10 + 48);
}

void setBlockingDelay(uint16_t msDelay){
    TMR3 = 0;
    PR3 = (uint32_t)msDelay * 125 / 128;
    T3CONbits.TON = 1;
}

void disableBlockingDelay(){
    T3CONbits.TON = 0;
}

uint16_t getCurrentTimeSeconds(){
    return minutes * 60 + seconds;
}

void initTimer(){
    T2CONbits.T32 = 0;
    T2CONbits.TCKPS = 0b01;
    T2CONbits.TCS = 0;

    T3CONbits.TCKPS = 3;
    T3CONbits.TCS = 0;
    // (1 + 0.00840336) seconds × (500000Hz / 2 / 8)
    // includes calibration offset for 31250 + 262.605
    PR2 = 31513;

    IEC0bits.T2IE = 1;
    IPC1bits.T2IP = 7;

    IEC0bits.T3IE = 1;
    IPC2bits.T3IP = 2;
};