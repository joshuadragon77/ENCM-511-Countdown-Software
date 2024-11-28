#include "vscodefix.h"
#include <xc.h>

void InitUART2(void);
void Disp2String(char *str);
void XmitUART2(char CharNum, unsigned int repeatNo);
void RecvUart(char* input, uint8_t buf_size);
char RecvUartChar(void);
void Disp2Hex(unsigned int DispData);
void Disp2Dec(uint16_t Dec_num);
char RecvUartCharNonBlocking();