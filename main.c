/*
 * File:   main.c
 * Author: Arush Sanghal, AbdulRahman AbdulJabar, Joshua Ounalom
 *
 * Created on: USE THE INFORMATION FROM THE HEADER MPLAB X IDE GENERATES FOR YOU
 */

// FBS
#pragma config BWRP = OFF               // Table Write Protect Boot (Boot segment may be written)
#pragma config BSS = OFF                // Boot segment Protect (No boot program Flash segment)

// FGS
#pragma config GWRP = OFF               // General Segment Code Flash Write Protection bit (General segment may be written)
#pragma config GCP = OFF                // General Segment Code Flash Code Protection bit (No protection)

// FOSCSEL
#pragma config FNOSC = FRC              // Oscillator Select (Fast RC oscillator (FRC))
#pragma config IESO = OFF               // Internal External Switch Over bit (Internal External Switchover mode disabled (Two-Speed Start-up disabled))

// FOSC
#pragma config POSCMOD = NONE           // Primary Oscillator Configuration bits (Primary oscillator disabled)
#pragma config OSCIOFNC = ON            // CLKO Enable Configuration bit (CLKO output disabled; pin functions as port I/O)
#pragma config POSCFREQ = HS            // Primary Oscillator Frequency Range Configuration bits (Primary oscillator/external clock input frequency greater than 8 MHz)
#pragma config SOSCSEL = SOSCHP         // SOSC Power Selection Configuration bits (Secondary oscillator configured for high-power operation)
#pragma config FCKSM = CSECMD           // Clock Switching and Monitor Selection (Clock switching is enabled, Fail-Safe Clock Monitor is disabled)

// FWDT
#pragma config WDTPS = PS32768          // Watchdog Timer Postscale Select bits (1:32,768)
#pragma config FWPSA = PR128            // WDT Prescaler (WDT prescaler ratio of 1:128)
#pragma config WINDIS = OFF             // Windowed Watchdog Timer Disable bit (Standard WDT selected; windowed WDT disabled)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))

// FPOR
#pragma config BOREN = BOR3             // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware; SBOREN bit disabled)
#pragma config PWRTEN = ON              // Power-up Timer Enable bit (PWRT enabled)
#pragma config I2C1SEL = PRI            // Alternate I2C1 Pin Mapping bit (Default location for SCL1/SDA1 pins)
#pragma config BORV = V18               // Brown-out Reset Voltage bits (Brown-out Reset set to lowest voltage (1.8V))
#pragma config MCLRE = ON               // MCLR Pin Enable bit (MCLR pin enabled; RA5 input pin disabled)

// FICD
#pragma config ICS = PGx2               // ICD Pin Placement Select bits (PGC2/PGD2 are used for programming and debugging the device)

// FDS
#pragma config DSWDTPS = DSWDTPSF       // Deep Sleep Watchdog Timer Postscale Select bits (1:2,147,483,648 (25.7 Days))
#pragma config DSWDTOSC = LPRC          // DSWDT Reference Clock Select bit (DSWDT uses LPRC as reference clock)
#pragma config RTCOSC = SOSC            // RTCC Reference Clock Select bit (RTCC uses SOSC as reference clock)
#pragma config DSBOREN = ON             // Deep Sleep Zero-Power BOR Enable bit (Deep Sleep BOR enabled in Deep Sleep)
#pragma config DSWDTEN = ON             // Deep Sleep Watchdog Timer Enable bit (DSWDT enabled)

// #pragma config statements should precede project file includes.

#include <xc.h>

#include "adc.h"
#include "block.h"
#include "clkchange.h"
#include "io.h"
#include "timer.h"
#include "uart.h"
#include "stdlib.h"

uint8_t operatingModeK = 0;
uint8_t operatingModeB = 0;
uint16_t buttonTriggered;
uint16_t time = 0;
uint8_t isPrinted = 0;
uint8_t ifT3Interrupt = 0;
uint8_t pause = 0;

#define BUTTON1_PRESS !(PORTA & 0b100)
#define BUTTON2_PRESS !(PORTB & 0b10000)
#define BUTTON3_PRESS !(PORTA & 0b10000) 
#define LED_OFF !(PORTB & 0b000000000)
#define LED_ON !(PORTB & 0b100000000)

void setup(){
    newClk(500);
    initTimer();
    initIO();
    initUART();
    initADC();
}

void IOcheck(){

    if (isButtonPress(Button1) && operatingModeB != 3){
        operatingModeB = 1;
    }
    if (isButtonPress(Button2) && operatingModeB != 3){
        operatingModeB = 2;
    }
    if (isButtonPress(Button3)){
        if (operatingModeB == 3){
            setBlockingDelay(2000);
            while (1){
                clearFlags(BLOCKFLAG_TimerDelayFinish | BLOCKFLAG_ButtonPress);
                orBlock(BLOCKFLAG_TimerDelayFinish | BLOCKFLAG_ButtonPress);

                if (getORFlagState(BLOCKFLAG_TimerDelayFinish)){
                    writeString("Timer is reset and timer was exited\n");
                    time = 0;
                    setTimer(0);
                    resetTimer();
                    operatingModeB = 0;
                    break;
                }else if(isButtonPress(Button3) == 0){
                    setBlockingDelay(200);
                    if(pause == 0){
                        pause = 1;

                    }
                    else if(pause == 1){
                        pause = 2;

                    }
                    break;
                }
            }
        }else{
            operatingModeB = 3;

        }
    }
 }

// void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void){
//     IFS1bits.CNIF = 0;
//     buttonTriggered = 1;
    
//  }

// void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void){
//     IFS0bits.T3IF = 0;
//     ifT3Interrupt = 1;

// }

#define TESTINGA

int main(void) {
    setup();

    // setTimer(120);
    // startTimer();

    // setDutyCycle(1);
    enablePWMLed();

    writeString("SAhit!\n");
    writeString("Fuck!\n");
    writeString("Bitch!\n");
    writeString("Idiot!\n");

    setInputMode(CustomInput);

    #ifdef TESTING
    while (1){
        setBlockingDelay(250);

        orBlock(BLOCKFLAG_TimerDelayFinish);



        orBlock(BLOCKFLAG_UARTReceive);
        writeUnsignedNumber(getUserCharacter());
        writeCharacter('\n');
        // setDutyCycle(getPercentageReading());
        // orBlock(BLOCKFLAG_ADCRead);
        // writeUnsignedNumber(getADCReading());

        // writeCharacter('\n');
        // orBlock(BLOCKFLAG_UARTFeedbackReceive);
        // char feedback[25];

        // getUserFeedback(feedback, 25);

        // writeString(feedback);
        // writeCharacter('\n');
        // andBlock(BLOCKFLAG_UARTTransmitComplete | BLOCKFLAG_ButtonPress);
        // toggleLED();
    }
    #else


    // setTimer(1);

    // (500 kHz / 2) / (4 × 19200 / s) − 1


    
    while (1){
        if (operatingModeB == 0){
            writeString("Press one of the push buttons to start\n\r");
            writeString("PB1 allows you to set the time you want in mm:ss\n\r");
            writeString("PB2 allows you to reset the timer to 0:00\n\r");
            writeString("PB3 counts down the timer with the led blinking along\n\r");

            while (operatingModeB == 0){
                clearFlags(BLOCKFLAG_ButtonPress);
                buttonBufferClear();
                orBlock(BLOCKFLAG_ButtonPress);
                IOcheck();
            }
        }
        
        if (operatingModeB == 1){
            clearUserUARTBuffer();
            writeString("Please enter the time you want in mm:ss format by typing in each number and pressing enter after\n\r");
            char feedbackArray[6];

            char val0 = 0;
            char val1 = 0;
            char val2 = 0;
            char val3 = 0;

            clearFlags(BLOCKFLAG_UARTReceive);
            orBlock(BLOCKFLAG_UARTReceive);
            clearFlags(BLOCKFLAG_UARTReceive);
            val0 = getUserCharacter();
            if (val0 < 48 || val0 > 57){
                writeString("Invalid Character, please input a number instead\n");
                operatingModeB = 0;
                continue;
            }
            
            orBlock(BLOCKFLAG_UARTReceive);
            clearFlags(BLOCKFLAG_UARTReceive);
            val1 = getUserCharacter();
            if (val1 < 48 || val1 > 57){
                writeString("Invalid Character, please input a number instead\n");
                operatingModeB = 0;
                continue;
            }

            orBlock(BLOCKFLAG_UARTReceive);
            clearFlags(BLOCKFLAG_UARTReceive);
            val2 = getUserCharacter();
            if (val2 < 48 || val2 > 55){
                writeString("Invalid Character, please input a number instead\n");
                operatingModeB = 0;
                continue;
            }

            orBlock(BLOCKFLAG_UARTReceive);
            clearFlags(BLOCKFLAG_UARTReceive);
            val3 = getUserCharacter();
            if(val2=='6' && val3!='0'){
                    writeString("Seconds cannot be greater than 60 seconds\n");
                    operatingModeB = 0;
                    continue;
                }
            if (val3 < '0' || val3 > '9'){
                
                writeString("Invalid Character, please input a number instead\n");
                operatingModeB = 0;
                continue;
            }
            

            // while(val0 == 0){
            //     val0 = getUserCharacter();
            // }
            // while(val1 == 0){
            //     val1 = getUserCharacter();
            // }
            // while(val2 == 0){
            //     val2 = getUserCharacter();
            // }
            // while(val3 == 0){
            //     val3 = getUserCharacter();
            // }
            char finalVal[6] = {val0, val1, ':', val2, val3, 0};
            
            writeString(finalVal);
            writeString("\n\r");
        
        
            time = ((val0 - 48)*10 + (val1-48))*60 + (val2-48)*10 + (val3-48);
            
            setTimer(time);
            resetTimer();
            operatingModeB = 0;

            
        }
        
        if (operatingModeB == 2){
            writeString("Timer Resetting back to 0:00\n\r");
            time = 0;
            setTimer(time);
            resetTimer();
            operatingModeB = 0;
            
        }
        
        if (operatingModeB == 3){
            clearUserUARTBuffer();
            //uint16_t originalTime = time;
            resetTimer();
            startTimer();
            TRISBbits.TRISB8 = 0;
            LATBbits.LATB8 = 0;

            uint16_t prevTime = getCurrentTimeSeconds()+1;
            uint8_t ledEnabled = 0;
            uint8_t input;
            setBlockingDelay(1000);
            disableAnimation();
            while(operatingModeB == 3){
                clearFlags(BLOCKFLAG_UARTReceive | BLOCKFLAG_TimerDelayFinish | BLOCKFLAG_ADCRead);
                orBlock(BLOCKFLAG_UARTReceive | BLOCKFLAG_TimerDelayFinish | BLOCKFLAG_ADCRead);
                setDutyCycle(getPercentageReading());
                IOcheck();

                while(pause == 1){
                    stopTimer();
                    Idle();
                    IOcheck();

                }
                if (pause ==2){
                    startTimer();
                    pause = 0;
                }


                if (getORFlagState(BLOCKFLAG_TimerDelayFinish)){
                    setBlockingDelay(1000);
                    ledEnabled = !ledEnabled;

                    if (ledEnabled){
                        enablePWMLed();
                    }else{
                        disablePWMLed();
                        turnOFFLed();
                    }
                }
                if (getORFlagState(BLOCKFLAG_UARTReceive)){

                    input = getUserCharacter();

                    if (input == 'i' && operatingModeK == 0){
                        input = 0;
                        operatingModeK = 1;
                    }

                    if (input == 'i' && operatingModeK == 1){
                        input = 0;
                        operatingModeK = 0;
                    }
                }
                if (getORFlagState(BLOCKFLAG_UARTReceive | BLOCKFLAG_TimerDelayFinish)){
                    if(operatingModeK == 0){
                        writeTimeToUARTConsole();
                        writeCharacter('\n');
                        //isPrinted = 1;
                        
                    }
                    if(operatingModeK==1){
                        writeTimeToUARTConsole();
                        writeCharacter(' ');
                        prevTime = getCurrentTimeSeconds();
                        writeUnsignedNumber(getADCReading());
                        writeCharacter(' ');
                        uint16_t val = (int)(getDutyCycle()*100);
                        
                        writeUnsignedNumber(val);
                        writeString("%\n");
                    }

                    // if(LED_ON){
                    //     isPrinted = 0;
                    // }

                    if(getCurrentTimeSeconds() == 0){
                        setTimer(5);
                        enablePWMLed();
                        startTimer();
                        while (getCurrentTimeSeconds() != 0){
                            orBlock(BLOCKFLAG_ADCRead);
                            clearFlags(BLOCKFLAG_ADCRead);
                            setDutyCycle(getPercentageReading());
                        }
                        setTimer(time);
                        operatingModeB = 0;
                        enableAnimation();
                    }
                }
            }
        }


    #endif
    }
    return 0;
}


