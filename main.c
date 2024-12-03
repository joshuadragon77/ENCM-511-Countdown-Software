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

void setup(){
    newClk(500);
    initTimer();
    initIO();
    initUART();
    initADC();
}

void IOcheck(){
    // Check to see if an input state is occured and react to those states. Change states as well.
    if (isButtonPress(Button1) && operatingModeB != 3){
        operatingModeB = 1;
    }
    if (isButtonPress(Button2) && operatingModeB != 3){
        operatingModeB = 2;
    }
    if (operatingModeB == 3){
        if (isButtonPress(Button3)){
            setBlockingDelay(2000);

            pause = !pause;

            if (pause){
                stopTimer();
                writeString("Timer's Paused!\n");
            }else{
                startTimer();
                writeString("Timer's Resumed!\n");
            }
        }else{
            disableBlockingDelay();
        }
    }else
        if (isButtonPress(Button3)){
            operatingModeB = 3;
        }
}


int main(void) {
    setup();

    enablePWMLed();
    setInputMode(CustomInput);

    while (1){
        // Main Menu
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
        
        // Timer Character Input
        if (operatingModeB == 1){
            clearUserUARTBuffer();
            writeString("Please enter the time you want in mmss format by typing in each number\n");
            char feedbackArray[6];

            char val0 = 0;
            char val1 = 0;
            char val2 = 0;
            char val3 = 0;

            // First Digit MSB
            clearFlags(BLOCKFLAG_UARTReceive);
            orBlock(BLOCKFLAG_UARTReceive);
            clearFlags(BLOCKFLAG_UARTReceive);
            val0 = getUserCharacter();
            if (val0 < 48 || val0 > 57){
                writeString("Invalid Character, please input a number instead\n");
                operatingModeB = 0;
                continue;
            }

            writeCharacter(val0);
            
            // Second Digit
            orBlock(BLOCKFLAG_UARTReceive);
            clearFlags(BLOCKFLAG_UARTReceive);
            val1 = getUserCharacter();
            if (val1 < 48 || val1 > 57){
                writeString("Invalid Character, please input a number instead\n");
                operatingModeB = 0;
                continue;
            }

            writeCharacter(val1);
            writeCharacter(':');

            // Third Digit
            orBlock(BLOCKFLAG_UARTReceive);
            clearFlags(BLOCKFLAG_UARTReceive);
            val2 = getUserCharacter();
            if (val2 < 48 || val2 > 55){
                writeString("Invalid Character, please input a number instead\n");
                operatingModeB = 0;
                continue;
            }

            writeCharacter(val2);


            // Fourth Digit
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
            
            writeCharacter(val3);
            
            writeString("\n\r\n\r");

            // Set timer...
        
            time = ((val0 - 48)*10 + (val1-48))*60 + (val2-48)*10 + (val3-48);
            
            setTimer(time);
            resetTimer();
            operatingModeB = 0;

            
        }
        
        // Reset Timer
        if (operatingModeB == 2){
            writeString("Timer Resetting back to 0:00\n");
            time = 0;
            setTimer(time);
            resetTimer();
            operatingModeB = 0;
            
        }
        
        // Start Timer
        if (operatingModeB == 3){
            if (time == 0){
                writeString("Cannot start timer with empty time.\n");
                operatingModeB = 0;
                continue;
            }
            writeString("Timer's Started\n");
            clearUserUARTBuffer();

            resetTimer();
            buttonBufferClear();
            startTimer();
            TRISBbits.TRISB8 = 0;
            LATBbits.LATB8 = 0;

            pause = 0;
            uint8_t ledEnabled = 0;
            uint8_t input;
            
            disableAnimation();
            while(operatingModeB == 3){

                // Check to see if a UART or Interval occurs, then show the text and toggle LED accordingly.
                if (getORFlagState(BLOCKFLAG_UARTReceive | BLOCKFLAG_TimerCountdownInterval)){

                    // Toggle LED.
                    if (getORFlagState(BLOCKFLAG_TimerCountdownInterval)){
                        // setBlockingDelay(1000);
                        ledEnabled = !ledEnabled;

                        if (ledEnabled){
                            enablePWMLed();
                        }else{
                            disablePWMLed();
                            turnOFFLed();
                        }
                    }

                    // Menu I control.
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

                    clearFlags(BLOCKFLAG_TimerCountdownInterval);
                    clearFlags(BLOCKFLAG_UARTReceive);

                    // Print only time
                    if(operatingModeK == 0){
                        writeTimeToUARTConsole();
                        writeCharacter('\n');
                        //isPrinted = 1;
                        
                    }

                    //Print more information such as ADC reading and duty cycle.
                    if(operatingModeK==1){
                        writeTimeToUARTConsole();
                        writeCharacter(' ');
                        writeUnsignedNumber(getADCReading());
                        writeCharacter(' ');
                        uint16_t val = (int)(getDutyCycle()*100);
                        
                        writeUnsignedNumber(val);
                        writeString("%\n");
                    }


                    // Logic for finishing the Timer.
                    if(getCurrentTimeSeconds() == 0){
                        writeString("Timer's Finished\n");
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


                // Logic for stopping the timer due to Button 3.
                if (getORFlagState(BLOCKFLAG_TimerDelayFinish) && isButtonHeldDown(Button3)){
                    writeString("Timer's Stopped!\n");
                    operatingModeB = 0;
                    break;
                }

                // If an interrupt for a button state change occur. Check what Button 3 states are.
                if (getORFlagState(BLOCKFLAG_ButtonPress)){
                    IOcheck();
                    clearFlags(BLOCKFLAG_ButtonPress);
                }
                setDutyCycle(getPercentageReading());
                orBlock(BLOCKFLAG_UARTReceive | BLOCKFLAG_TimerCountdownInterval | BLOCKFLAG_TimerDelayFinish | BLOCKFLAG_ADCRead | BLOCKFLAG_ButtonPress);
            }
        }
    }
    return 0;
}


