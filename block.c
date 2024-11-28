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

void clearFlags(BlockFlags blockFlags){
    currentBlockFlags &= !blockFlags;
}

void orBlock(BlockFlags blockFlags){
    while (!(blockFlags & currentBlockFlags)){
        getUserCharacter();
        animationWaveyTick();
        Idle();
    }
    currentBlockFlags &= !blockFlags;
}

void andBlock(BlockFlags blockFlags){
    while ((blockFlags & currentBlockFlags) != blockFlags){
        getUserCharacter();
        animationWaveyTick();
        Idle();
    }
    currentBlockFlags &= !blockFlags;
}