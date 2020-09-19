#include <Arduino.h>
#include "led_strip_runtime.hpp"
#include "MODULES/MPU6050/mpu6050_imu.h"
#include "MODULES/PROTOCALLBACKS/teensy_coms.h"

void setup() {
  os_init();    
  message_callbacks_begin(); 
  start_led_strip_runtime(); 
}

void loop() {
  os_thread_delay_s(1); 
}