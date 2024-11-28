#ifndef BLOCK_HEADERS
#define BLOCK_HEADERS

#include "vscodefix.h"

#include "io.h"
#include "uart.h"

#include <xc.h>


typedef uint8_t BlockFlags;

extern BlockFlags currentBlockFlags;

#define SET_BLOCKING_FLAG(BLOCKFLAG) currentBlockFlags |= BLOCKFLAG

#define BLOCKFLAG_ButtonPress 1
#define BLOCKFLAG_UARTTransmitComplete 2
#define BLOCKFLAG_UARTReceive 4
#define BLOCKFLAG_UARTFeedbackReceive 8
#define BLOCKFLAG_ADCRead 16
#define BLOCKFLAG_TimerCountdownFinish 32
#define BLOCKFLAG_TimerDelayFinish 64

/**
 * @brief Function that clears the selected block flags.
 * 
 * This is to ensure that orBlocks and andBlocks are not triggered due to an already flagged event.
 * 
 * @param blockFlags 
 */
void clearFlags(BlockFlags blockFlags);

/**
 * Function that "blocks" and idles the CPU until the CPU wakes up and receives the correct block flag.
 * 
 * This is an OR operation block. Therefore, AT LEAST one block flag needs to be satasified.
 */
void orBlock(BlockFlags blockFlags);

/**
 * Function that "blocks" and idles the CPU until the CPU wakes up and receives the correct block flag.
 * 
 * This is an AND operation block. Therefore, ALL block flag needs to be satasified.
 */
void andBlock(BlockFlags blockFlags);

#endif