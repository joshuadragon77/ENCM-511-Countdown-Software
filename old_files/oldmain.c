
// void IOcheck(){
//     if (!buttonTriggered){
//         return;
//     }

//     // buttonTriggered = 0;
//     // if (BUTTON1_PRESS){
//     //     operatingModeB = 1;
//     // }
//     // if (BUTTON2_PRESS){
//     //     operatingModeB = 2;
//     // }
//     // if (BUTTON3_PRESS){
//     //     operatingModeB = 3;
//     // }
// }

// void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void){
//     IFS1bits.CNIF = 0;
//     buttonTriggered = 1;
    
// }

// void onTimerEnd(){
//     startTimer();
// };

// void program(){
    
//     // Clear the Serial Console
//     Disp2String("\x1B[1J\n\r");
    
//     while(1) {
        
//         if (operatingModeB == 0){
//             Disp2String("Press one of the push buttons to start\n\r");
//             Disp2String("PB1 allows you to set the time you want in mm:ss\n\r");
//             Disp2String("PB2 allows you to reset the timer to 0:00\n\r");
//             Disp2String("PB3 counts down the timer with the led blinking along\n\r");
            
            
//         }
        
//         if (operatingModeB == 1){
//             Disp2String("Please enter the time you want in mm:ss format by typing in each number and pressing enter after\n\r");
//             char val0 = RecvUartChar();
//             char val1 = RecvUartChar();
//             char val2 = RecvUartChar();
//             char val3 = RecvUartChar();
//             char finalVal[6] = {val0, val1, ':', val2, val3, 0};
            
//             Disp2String(finalVal);
//             Disp2String("\n\r");
            
            
//             time = ((val0 - 48)*10 + (val1-48))*60 + (val2-48)*10 + (val3-48);
//             operatingModeB = 0;

            
//         }
        
//         if (operatingModeB = 2){
//             Disp2String("Timer Resetting back to 0:00\n\r");
//             time = 0;
//             operatingModeB = 0;
//         }
        
//         if (operatingModeB = 3){
//             setTimer(time);
//             startTimer();
//             PR1 = 3904; 
//             TMR1 = 0;
//             T1CONbits.TON = 1;
            
//         }
        
        
        
//         // Read current UART reading.
//         uint16_t ADCvalue = do_ADC();

//         // Convert the ADC Reading into a percentage based off of its maximum value 1024.
//         double val = (double)ADCvalue / 1024;
        
//         // Determine the number of characters to show in the bar.
//         uint16_t len = val * 25;
        
//         // Render the characters to the bar string.
//         char total[25];
//         char* writeP2 = &total[0];
//         for (uint8_t l = 0; l < 25; l++ ){
//             total[l] = 0;
//         }
        
        
        
//         for (uint8_t k = 0; k < len; k++){
//             if (operatingModeB == 0){
//                 *writeP2 = '=';
//             }
//             if (operatingModeB == 1){
//                 *writeP2 = 'x';
//             }
//             if (operatingModeB == 2){
//                 *writeP2 = 'y';
//             }
//             if (operatingModeB == 3){
//                 *writeP2 = 'z';
//             }
//             writeP2++;
//         }
        
//         // Read from the Serial Console and determine what character was pressed and set operatingModes accordingly.
        
//         char inputCharacter = RecvUartCharNonBlocking();
        
//         if (inputCharacter == 'i'){
//             if (operatingModeK == 0){
//                 operatingModeK = 1;
//             }
//             if (operatingModeK == 1){
//                 operatingModeK = 0;
//             }
//         }
        
//         // Display the ADC value in terms of hexdecimal or decimal.
        
//         if (operatingModeK == 0){
//             Disp2Dec(ADCvalue);
//         }
        
//         if (operatingModeK == 1
// void program(){
    
//     // Clear the Serial Console
//     Disp2String("\x1B[1J\n\r");
    
//     while(1) {
        
//         if (operatingModeB == 0){
//             Disp2String("Press one of the push buttons to start\n\r");
//             Disp2String("PB1 allows you to set the time you want in mm:ss\n\r");
//             Disp2String("PB2 allows you to reset the timer to 0:00\n\r");
//             Disp2String("PB3 counts down the timer with the led blinking along\n\r");
            
            
//         }
        
//         if (operatingModeB == 1){
//             Disp2String("Please enter the time you want in mm:ss format by typing in each number and pressing enter after\n\r");
//             char val0 = RecvUartChar();
//             char val1 = RecvUartChar();
//             char val2 = RecvUartChar();
//             char val3 = RecvUartChar();
//             char finalVal[6] = {val0, val1, ':', val2, val3, 0};
            
//             Disp2String(finalVal);
//             Disp2String("\n\r");
            
            
//             time = ((val0 - 48)*10 + (val1-48))*60 + (val2-48)*10 + (val3-48);
//             operatingModeB = 0;

            
//         }
        
//         if (operatingModeB = 2){
//             Disp2String("Timer Resetting back to 0:00\n\r");
//             time = 0;
//             operatingModeB = 0;
//         }
        
//         if (operatingModeB = 3){
//             setTimer(time);
//             startTimer();
//             PR1 = 3904; 
//             TMR1 = 0;
//             T1CONbits.TON = 1;
            
//         }
        
        
        
//         // Read current UART reading.
//         uint16_t ADCvalue = do_ADC();

//         // Convert the ADC Reading into a percentage based off of its maximum value 1024.
//         double val = (double)ADCvalue / 1024;
        
//         // Determine the number of characters to show in the bar.
//         uint16_t len = val * 25;
        
//         // Render the characters to the bar string.
//         char total[25];
//         char* writeP2 = &total[0];
//         for (uint8_t l = 0; l < 25; l++ ){
//             total[l] = 0;
//         }
        
        
        
//         for (uint8_t k = 0; k < len; k++){
//             if (operatingModeB == 0){
//                 *writeP2 = '=';
//             }
//             if (operatingModeB == 1){
//                 *writeP2 = 'x';
//             }
//             if (operatingModeB == 2){
//                 *writeP2 = 'y';
//             }
//             if (operatingModeB == 3){
//                 *writeP2 = 'z';
//             }
//             writeP2++;Operation.
        
//         IOcheck();
        
//     }
// }
