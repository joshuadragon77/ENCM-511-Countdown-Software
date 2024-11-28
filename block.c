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