/*
 * File:   main.c
 * Author: Joshua Ounalom, Arush Sanghal, AbdulRahman AbdulJabar
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

#include <stdio.h>

#include "adc.h"
#include "block.h"
#include "clkchange.h"
#include "io.h"
#include "timer.h"
#include "uart.h"

uint8_t operatingModeK = 0;
uint8_t operatingModeB = 0;
uint16_t buttonTriggered;
uint16_t time = 0;
uint8_t isPrinted = 0;
uint8_t ifT3Interrupt = 0;

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

    if (isButtonPress(Button1)){
        operatingModeB = 1;
    }
    if (isButtonPress(Button2)){
        operatingModeB = 2;
    }
    if (isButtonPress(Button3)){
        if(operatingModeB == 3){
            operatingModeB = 4;

        }
        if(operatingModeB == 4){
            operatingModeB = 3;

        }
        else{
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

    setTimer(120);
    startTimer();

    // setDutyCycle(1);
    enablePWMLed();

    writeString("SAhit!\n");
    writeString("Fuck!\n");
    writeString("Bitch!\n");
    writeString("Idiot!\n");

    setInputMode(CustomInput);

    #ifdef TESTING
    while (1){
        // setBlockingDelay(250);

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

            orBlock(BLOCKFLAG_ButtonPress);
        }
        
        if (operatingModeB == 1){
            writeString("Please enter the time you want in mm:ss format by typing in each number and pressing enter after\n\r");
            char feedbackArray[6];

            if(getUserFeedback(feedbackArray, 6)!= -1){
                char val0 = 0;
                char val1 = 0;
                char val2 = 0;
                char val3 = 0;
                
                while(val0 == 0){
                    val0 = getUserCharacter();
                }
                while(val1 == 0){
                    val1 = getUserCharacter();
                }
                while(val2 == 0){
                    val2 = getUserCharacter();
                }
                while(val3 == 0){
                    val3 = getUserCharacter();
                }
                char finalVal[6] = {val0, val1, ':', val2, val3, 0};
                
                writeString(finalVal);
                writeString("\n\r");
            
            
                time = ((val0 - 48)*10 + (val1-48))*60 + (val2-48)*10 + (val3-48);
            }
            
            setTimer(time);
            resetTimer();
            operatingModeB = 0;

            
        }
        
        if (operatingModeB == 2){
            writeString("Timer Resetting back to 0:00\n\r");
            time = 0;
            operatingModeB = 0;
        }
        
        if (operatingModeB == 3){
            //uint16_t originalTime = time;
            startTimer();
            T3CONbits.TCKPS = 2;
            TRISBbits.TRISB8 = 0;
            LATBbits.LATB8 = 0;


            PR3 = 3904; 
            TMR3 = 0;
            T3CONbits.TON = 1;
            uint16_t prevTime = getCurrentTimeSeconds()+1;
            disablePWMLed();
            while(operatingModeB == 3){


                while(ifT3Interrupt){
                    Idle();
                }
                ifT3Interrupt = 0;

                if(operatingModeK == 0){
                    if(prevTime != getCurrentTimeSeconds()){
                        writeTimeToUARTConsole();
                        prevTime = getCurrentTimeSeconds();
                        toggleLED();
                        //isPrinted = 1;
                    }
                    
                }
                if(operatingModeK==1){
                    // if(LED_OFF && isPrinted == 0){
                        writeTimeToUARTConsole();
                        //isPrinted = 1;
                    //}
                }
                // if(LED_ON){
                //     isPrinted = 0;
                // }

                if(getCurrentTimeSeconds() == 0){
                    setTimer(5);
                    enablePWMLed();
                    startTimer();
                    setTimer(time);
                    resetTimer();
                    operatingModeB = 0;
                }

            }
        }

        if(operatingModeB == 4){
            stopTimer();
            Idle();
        }
        
        
        
        // Read current UART reading.
        char inputCharacter = getUserCharacter();
        
        if (inputCharacter == 'i'){
            if(operatingModeK == 0){
                operatingModeK = 1;
            }
            if(operatingModeK == 1){
                operatingModeK = 0;
            }
        }
        
    
        IOcheck();
        writeUnsignedNumber(getADCReading());
        writeCharacter('\n');

        


        // blockUntilUserFeedback();

        // char a[10];
        // for (int i = 0;i<10;i++){fffffff
        //     a[i] = 0;
        // }
        // getUserFeedback(a, 10);

        // if (strcmp(a, "reset") == 0){
        //     resetTimer();
        // } 
        // if (strcmp(a, "stop") == 0){
        //     stopTimer();
        // } 
        // if (strcmp(a, "start") == 0){
        //     startTimer();
        // } 
        // writeTimeToUARTConsole();
        

        // for (int i = 0;i<5;i++){
            getUserCharacter();
            animationWaveyTick();
        // };
        
        
    }

    // while (1){
    //     animationWaveyTick();
    //     // staticLEDOff();
    // }

    return 0;
    #endif


}


