#ifndef IO_HEADERS
#define IO_HEADERS

#include "vscodefix.h"

#include "block.h"
#include "uart.h"

#include <xc.h>
#include <math.h>

typedef enum _Button{
    Button1,
    Button2,
    Button3
} Button;

void buttonBufferClear();
void enableAnimation();
void disableAnimation();

/**
 * @brief Setup IO Peripherals and timers for the PWM component.
 * 
 * Module Initialization Function
 */
void initIO();

/**
 * @brief A function that is implemented in a "superloop" to create an PWM based wavey animation.
 * 
 * Modulates the PWM and requires the PWM to be enabled.
 */
void animationWaveyTick();

/**
 * @brief Enable the PWM component.
 * 
 */
void enablePWMLed();

/**
 * @brief Disable the PWM Component
 * 
 */
void disablePWMLed();

/**
 * @brief Get the current PWM LED State
 * 
 * Returns a 1 if it is enabled or 0 otherwise.
 * 
 * @return uint8_t 
 */
uint8_t getPWMLedState();

/**
 * @brief Enable the LED via the Latch B8.
 * 
 * Recommended to turn off the PWM function before using this function.
 */
void turnONLed();

/**
 * @brief Disable the LED via the Latch B8.
 * 
 * Recommended to turn off the PWM function before using this function.
 */
void turnOFFLed();

/**
 * @brief Toggle the LED state via the Latch B8.
 * 
 */
void toggleLED();

/**
 * @brief Get the Led Power State.
 * 
 * Return 1 if it is enabled or 0 otherwise.
 * 
 * @return uint8_t 
 */
uint8_t getLedState();

/**
 * @brief Sets the duty cycle of the PWM Component. 
 * 
 * Requires the PWM to be enabled.
 * 
 * @note The duty cycle is limited and will not reach 100% or 0% due to software implementations in interrupts.
 * @param dutyCycle 
 */
void setDutyCycle(float dutyCycle);

/**
 * @brief Get the Current Duty Cycle of the PWM module
 * 
 * @return uint8_t 
 */
float getDutyCycle();

/**
 * @brief Checks to see if a specific button is pressed. Returns a 1 if pressed and 0 otherwise.
 * 
 * Three buttons are supported:
 * 
 * - Button1
 * 
 * - Button2
 * 
 * - Button3
 * 
 * The interrupts are handled internally and allows a safer way to get button states.
 * 
 * @param button 
 * @return uint8_t (Press State)
 */
uint8_t isButtonPress(Button button);

#endif