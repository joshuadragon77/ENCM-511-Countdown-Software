#!/bin/env sh

IPCMD=/opt/microchip/mplabx/v6.20/mplab_platform/mplab_ipe/ipecmd.jar

HEXFILE=./dist/default/production/FinalAssignment.X.production.hex


java -jar $IPCMD -P24F16KA101 -TPSNAP -M -OL -F"$HEXFILE"