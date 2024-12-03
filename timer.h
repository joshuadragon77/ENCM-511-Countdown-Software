#ifndef TIMER_HEADERS
#define TIMER_HEADERS

#include "vscodefix.h"

#include "uart.h"

#include <xc.h> 

void setTimer(uint16_t seconds);
/**
 * @brief Starts the timer
 * 
 */
void startTimer();
/**
 * @brief Stops the timer
 * 
 */
void stopTimer();
/**
 * @brief Setup the Timer so it can operate and count.
 * 
 * Module Initialization Function
 */
void initTimer();


/**
 * @deprecated Do not use due to not meeting the requirements.
 */
void onTimerEnd();

/**
 * @brief Get the current time of the timer in seconds.
 */
uint16_t getCurrentTimeSeconds();

/**
 * @brief Efficiently prints the current time to the UART timer.
 * 
 */
void writeTimeToUARTConsole();
/**
 * @brief Resets the timer back to the set timer regardless if it is running or not
 * 
 */
void resetTimer();

/**
 * @brief Start the Timer 3 Hardware for a Blocking Delay Interrupt.
 * 
 * To cause a "delay", you would need to call this function with the desired delay. 
 * 
 * Then, use the orBlock or andBlock to cause a block for BLOCKFLAG_TimerDelayFinish.
 * 
 * @param msDelay 
 */
void setBlockingDelay(uint16_t msDelay);

void disableBlockingDelay();
#endif