#ifndef _LED_STRIP_RUNTIME_HPP
#define _LED_STRIP_RUNTIME_HPP

// Arduino runtime
#include <Arduino.h>

// Operating System stuff
#include "OS/OSMutexKernel.h"
#include "OS/OSSignalKernel.h"
#include "OS/OSThreadKernel.h"

// Proto Message Callbacks 
#include "MODULES/PROTOCALLBACKS/teensy_coms.h"

// Serial DMA buffer
#include <WS2812Serial.h> 

// HSV RGB Stuff
#include "hsv_rgb_conv.hpp"

// Protomessage Unpacking stuff 
#include "protomessage_unpack.h"

/*!
*   @brief Number of leds in our strip. 
*/
const int LED_STRIP_NUM_LEDS = 520;

/*!
*   @brief Which GPIO pin we are using
*/
static const uint8_t LED_STRIP_GPIO = 1;

void start_led_strip_runtime(void); 

#endif