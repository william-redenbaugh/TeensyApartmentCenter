#include "protomessage_unpack.h"

/*!
* @brief Function Declarations
*/
LEDStripAnimationData unpack_led_strip_animation_data(uint8_t *arr, uint32_t len); 

/*!
* @brief Allows us to quickly unpack the message data.
* @param uint8_t *arr(pointer to array that contains the message)
* @param uint32_t len(length of the array)
* @returns LEDStripAnimationData struct
*/
LEDStripAnimationData unpack_led_strip_animation_data(uint8_t *arr, uint32_t len){
    LEDStripAnimationData msg_data; 
    pb_istream_t msg_in = pb_istream_from_buffer(arr, len);
    pb_decode(&msg_in, LEDStripAnimationData_fields, &msg_data); 
    return msg_data; 
}