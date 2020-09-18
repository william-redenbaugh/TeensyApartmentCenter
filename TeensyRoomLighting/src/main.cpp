#include <Arduino.h>

#include <WS2812Serial.h>

// default number of matrix_leds. 
const int num_matrix_led = 520;
// Default GPIO value. 
uint8_t led_matrix_gpio = 1;

byte matrix_drawing_memory[num_matrix_led*3];         //  3 bytes per LED for RGB
DMAMEM byte matrix_display_memory[num_matrix_led*12]; // 12 bytes per LED for RGB

WS2812Serial matrix_leds = WS2812Serial(num_matrix_led, matrix_display_memory, matrix_drawing_memory, led_matrix_gpio, WS2812_GRBW);

void setup() {
  matrix_leds.begin(); 
  matrix_leds.clear();
  matrix_leds.show();
}

int k = 0; 

void loop() {

}