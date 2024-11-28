/**
 * @file uart.c
 * @author TheJades (thejadesisfat77@gmail.com)
 * @brief UART Driver for the PIC24F16KA101. Features a mostly interrupted architecture and basic terminal-like features.
 * @version 0.1
 * @date 2024-11-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "uart.h"

// Buffer increased to 512 from 256 due to insufficient space for long strings being transmitted.
char cpuUARTBuffer[512];
char userUARTBuffer[256];

char feedbackBuffer[256];
char submittedFeedbackBuffer[256];

uint16_t writePointer = 0;
uint16_t readPointer = 0;

uint8_t feedbackReady = 0;
uint8_t currentInputMode = FeedbackOnInput;
uint8_t userWritePointer = 0;
uint8_t userReadPointer = 0;

uint8_t currentFeedbackLength = 0;

void setInputMode(InputMode inputMode){
    currentInputMode = inputMode;
}

void writeCharacter(char character){
    cpuUARTBuffer[writePointer ++] = character;
    writePointer &= 511;
    if (character == '\n'){
        cpuUARTBuffer[writePointer ++] = '\r';
        writePointer &= 511;
    }
    
    if (U2STAbits.UTXBF == 0 && U2STAbits.TRMT){
        U2TXREG = 0;
    }
}

uint16_t power10(uint16_t power){
    switch(power){
        case 0:{
            return 1;
        }
        case 1:{
            return 10;
        }
        case 2:{
            return 100;
        }
        case 3:{
            return 1000;
        }
        case 4:{
            return 10000;
        }
        case 5:{
            return 100000;
        }
    }
}

void writeUnsignedNumber(uint16_t number){
    uint8_t digits = 0;//floor(log10(number)) + 1;
    while (number / power10(++ digits) > 0);
    if (digits == 0)
        digits = 1;
    for (int i = 0;i<digits;i++){
        uint8_t sampledDigit = number / power10(digits - i - 1) % 10;
        writeCharacter(sampledDigit + 48);
    }
}

void writeString(const char * stdString){
    while (1){
        char currentCharacter = *(stdString ++);

        if (!currentCharacter)
            break;

        writeCharacter(currentCharacter);
    }
}

char getUserCharacter(){
    uint8_t currentWritePointer = userWritePointer;

    if (userReadPointer != currentWritePointer){
        char userCharacter = userUARTBuffer[userReadPointer ++];

        if (currentInputMode == FeedbackOnInput){
            if (userCharacter >= 32 && userCharacter <= 126 && currentFeedbackLength < 255){
                writeCharacter(userCharacter);
                feedbackBuffer[currentFeedbackLength ++] = userCharacter;
            }
            if (userCharacter == 127 && currentFeedbackLength > 0){
                writeString("\x08 \x08");
                currentFeedbackLength --;
            }
            if (userCharacter == 13){
                writeString("\n\r");
                strncpy(submittedFeedbackBuffer, feedbackBuffer, currentFeedbackLength);
                feedbackReady = 1;
                currentFeedbackLength = 0;
                SET_BLOCKING_FLAG(BLOCKFLAG_UARTFeedbackReceive);
            }
        }

        if (userCharacter == 3 || userCharacter == 4){
            asm("RESET");
        }

        return userCharacter;
    }else{
        return 0;
    }
}

void blockUntilUserFeedback(){
    while (!feedbackReady){
        Idle();
        getUserCharacter();
    }
}

uint8_t getUserFeedback(char * userFeedback, uint8_t length){
    if (!feedbackReady){
        return -1;
    }

    strncpy(userFeedback, submittedFeedbackBuffer, length);

    for (int i = 0;i<256;i++){
        submittedFeedbackBuffer[i] = 0;
    }

    feedbackReady = 0;

    return 0;
}

void __attribute__((interrupt, no_auto_psv)) _U2RXInterrupt(void){
    IFS1bits.U2RXIF = 0;
    userUARTBuffer[userWritePointer ++] = U2RXREG;

    SET_BLOCKING_FLAG(BLOCKFLAG_UARTReceive);
}

void __attribute__((interrupt, no_auto_psv)) _U2TXInterrupt(void){
    IFS1bits.U2TXIF = 0;
    if (writePointer != readPointer){
        U2TXREG = cpuUARTBuffer[readPointer ++];
        readPointer &= 511;
    }else{
        SET_BLOCKING_FLAG(BLOCKFLAG_UARTTransmitComplete);
    }
}

void initUART(){

    // (500 kHz / 2) / (4 × 4800 / s) − 1
    U2BRG = 12;

    U2MODEbits.PDSEL = 0;
    U2MODEbits.BRGH = 1;
    U2MODEbits.STSEL = 0;

    U2STAbits.UTXISEL0 = 0;
    U2STAbits.UTXISEL1 = 1;
    IEC1bits.U2TXIE = 1;
    IEC1bits.U2RXIE = 1;
    IPC7bits.U2TXIP = 5;

    U2MODEbits.UARTEN = 1;
    U2STAbits.UTXEN = 1;

    writeString("\x1B[1JUART Initialized\n\n");

    if (RCONbits.SWR){
        RCONbits.SWR = 0;
        writeString("!A User Interrupt Reset was performed!\n\n");
    }
}