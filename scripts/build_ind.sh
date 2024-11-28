#!/bin/env sh

XC11_GCC=$(which xc16-gcc);

HEADERS=$(ls *.h)
SOURCES=$(ls *.c)

echo $HEADERS

$XC11_GCC -mcpu=24F16KA101 -D__PIC24F16KA101__ -D__DEBUG -D_GNU_SOURCE -I/opt/microchip/xc16/v2.10/include/ -I/opt/microchip/mplabx/v6.20/packs/Microchip/PIC24F-KA-KL-KM_DFP/1.5.253/xc16/support/PIC24F/h/ -I/opt/microchip/mplabx/v6.20/packs/Microchip/PIC24F-KA-KL-KM_DFP/1.5.253/xc16/support/generic/h/ $SOURCES $HEADERS -otest