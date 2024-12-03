/**
 * @file block.c
 * @author TheJades (thejadesisfat77@gmail.com)
 * @brief Enables software-like interrupts which allows the CPU to idle until software interrupts occur.
 * @version 0.1
 * @date 2024-11-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "block.h"

BlockFlags currentBlockFlags = 0;
uint8_t optimizedCurrentBlockFlags[] = {0, 0, 0, 0, 0, 0, 0, 0};
uint8_t mustUpdateFlags = 0;

void clearFlags(BlockFlags blockFlags){
    currentBlockFlags &= !blockFlags;
}

uint8_t getORFlagState(BlockFlags blockFlags){
    generateCurrentBlockFlags();
    return blockFlags & currentBlockFlags;
}

uint8_t getANDFlagState(BlockFlags blockFlags){
    generateCurrentBlockFlags();
    return (blockFlags & currentBlockFlags) == blockFlags;
}

void generateCurrentBlockFlags(){
    // if (mustUpdateFlags == 0)
    //     return;

    mustUpdateFlags = 0;
    
    uint8_t newBlockFlags = 0;

    newBlockFlags |= optimizedCurrentBlockFlags[7];
    optimizedCurrentBlockFlags[7] = 0; 

    newBlockFlags <<= 1;

    newBlockFlags |= optimizedCurrentBlockFlags[6];
    optimizedCurrentBlockFlags[6] = 0; 

    newBlockFlags <<= 1;

    newBlockFlags |= optimizedCurrentBlockFlags[5];
    optimizedCurrentBlockFlags[5] = 0; 

    newBlockFlags <<= 1;

    newBlockFlags |= optimizedCurrentBlockFlags[4];
    optimizedCurrentBlockFlags[4] = 0; 

    newBlockFlags <<= 1;

    newBlockFlags |= optimizedCurrentBlockFlags[3];
    optimizedCurrentBlockFlags[3] = 0; 

    newBlockFlags <<= 1;

    newBlockFlags |= optimizedCurrentBlockFlags[2];
    optimizedCurrentBlockFlags[2] = 0; 

    newBlockFlags <<= 1;

    newBlockFlags |= optimizedCurrentBlockFlags[1];
    optimizedCurrentBlockFlags[1] = 0; 

    newBlockFlags <<= 1;

    newBlockFlags |= optimizedCurrentBlockFlags[0];
    optimizedCurrentBlockFlags[0] = 0; 

    currentBlockFlags |= newBlockFlags;
}

void orBlock(BlockFlags blockFlags){
    generateCurrentBlockFlags();
    while (!(blockFlags & currentBlockFlags)){
        if (getInputMode() == FeedbackOnInput)
            getUserCharacter();
        animationWaveyTick();
        Idle();
        generateCurrentBlockFlags();
    }
}

void orBlockClear(BlockFlags blockFlags){
    generateCurrentBlockFlags();
    uint8_t acceptedFlags = 0;
    while (!(acceptedFlags = blockFlags & currentBlockFlags)){
        if (getInputMode() == FeedbackOnInput)
            getUserCharacter();
        animationWaveyTick();
        Idle();
        generateCurrentBlockFlags();
    }
    clearFlags(acceptedFlags);
}

void andBlock(BlockFlags blockFlags){
    generateCurrentBlockFlags();
    while ((blockFlags & currentBlockFlags) != blockFlags){
        if (getInputMode() == FeedbackOnInput)
            getUserCharacter();
        animationWaveyTick();
        Idle();
        generateCurrentBlockFlags();
    }
}