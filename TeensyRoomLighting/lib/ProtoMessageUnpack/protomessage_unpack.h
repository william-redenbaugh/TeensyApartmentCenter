#ifndef _PROTOMESSAGE_UNPACK_H
#define _PROTOMESSAGE_UNPACK_H

#include <Arduino.h> 
#include "MODULES/PROTOCALLBACKS/teensy_coms.h"

LEDStripAnimationData unpack_led_strip_animation_data(uint8_t *arr, uint32_t len); 

#endif  