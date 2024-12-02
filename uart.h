#ifndef UART_HEADERS
#define UART_HEADERS

#include "vscodefix.h"

#include "block.h"

#include <xc.h>
#include <string.h>

typedef enum _InputMode {
    FeedbackOnInput,
    CustomInput
} InputMode;


void clearUserUARTBuffer();

/**
 * Set the Input Mode for the UART Driver.
 * 
 * Avaliable Input Modes
 * - CustomInput (No type feedback to user)
 * - FeedbackOnInput (Type feedback to user, default value)
 */
void setInputMode(InputMode inputMode);

/**
 * @brief Writes the character to the UART Transmit Buffer. 
 * 
 * This module can store up to 256 characters before it overflows. This buffer slowly reduces as the UART module sends data.
 * 
 * @param character 
 */
void writeCharacter(char character);

/**
 * @brief Writes an array of character to the UART Transmit Buffer.
 * 
 * This module can store up to 256 characters before it overflows. This buffer slowly reduces as the UART module sends data.
 * 
 * @param stdString 
 */
void writeString(const char * stdString);

/**
 * @brief Setup the UART Module so it can receive and transmit.
 * 
 * Module Initialization Function
 */
void initUART();

/**
 * @brief Write an unsigned 16 bit number to the UART efficiently.
 * 
 * @param number 
 */
void writeUnsignedNumber(uint16_t number);

/**
 * @brief Gets the current ASCII character that the device has received.
 * 
 * This function is required to be called constantly to process "feedback" input and a smooth typing experience.
 * 
 * @return char 
 */
char getUserCharacter();

/**
 * @brief Gets the current feedback string the user has entered.
 * 
 * This function requires a pointer to write the feedback string and the length context. Returns a status code indicating success.
 * 
 * - Unsuccess: -1
 * 
 * - Success: 0
 * 
 * @note Does not include the new line character at the end of the string.
 * 
 * @param userFeedback 
 * @param length 
 * @return uint8_t 
 */
uint8_t getUserFeedback(char * userFeedback, uint8_t length);

#endif