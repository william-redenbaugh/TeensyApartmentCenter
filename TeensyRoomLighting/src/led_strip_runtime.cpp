#include "led_strip_runtime.hpp"

/*!
*   @brief Drawing memory that we actually write the dma buffer to.  
*   @note 3 bytes per LED for RGB
*/
static byte matrix_drawing_memory[LED_STRIP_NUM_LEDS*3];         

/*!
*   @brief DMA buffer that we will use to send data out the serial bus
*   @note 12 bytes per LED for RGB
*/
static DMAMEM byte matrix_display_memory[LED_STRIP_NUM_LEDS*12]; 

/*!
*   @brief Strip DMA manipulation 
*/
static WS2812Serial strip_leds = WS2812Serial(LED_STRIP_NUM_LEDS, matrix_display_memory, matrix_drawing_memory, LED_STRIP_GPIO, WS2812_GRB);

/*!
*   @brief LED strip thread stack and stack size. 
*/
static const int LED_STRIP_THREAD_STACK_SIZE = 16384; 
static uint32_t led_strip_thread_stack[LED_STRIP_THREAD_STACK_SIZE]; 

/*!
*   @brief Hsv Array that serves as a helper for the animation
*/
static HsvColor hsv_helper_arr[LED_STRIP_NUM_LEDS]; 

/*!
*   @brief The signal that we check to see if we are changing animations
*/
OSSignal animation_change_signal; 

/*!
*   @brief Helper and setup function declarations. 
*/
void start_led_strip_runtime(void); 
static void led_strip_thread(void *parameters);
static void led_message_callback(MessageReq *msg_req); 
static void init_strip(void); 
static inline bool animation_changed(uint32_t ms); 
static void set_entire_strip(uint8_t r, uint8_t g, uint8_t b); 

/*!
*   @brief Starting led strip runtime thread. 
*/
void start_led_strip_runtime(void){
    os_add_thread(&led_strip_thread, NULL, LED_STRIP_THREAD_STACK_SIZE, led_strip_thread_stack);  
}

uint8_t arr[1456];
/*!
*   @brief Strip thread. 
*   @param void* general pointer. 
*/
static void led_strip_thread(void *parameters){
    init_strip(); 
    Serial.begin(115200);

    // Clearing out different animation data. 
    memset(hsv_helper_arr, 0, sizeof(hsv_helper_arr)); 
    
    // Goes on to work on the next animation
    for(;;){
        if(Serial.available()>=1456){ 
            Serial.readBytes((char*)arr, sizeof(arr)); 
            
            // If the message header is correct. 
            if(arr[0] == 24 && arr[1] == 48 && arr[2] == 69 && arr[3] == 52){
                // We load the rest of the message data into the buffer. 
                for(int n = 0; n < 484; n++){
                    strip_leds.setPixelColor(n, arr[n * 3 + 4], arr[n * 3 + 5], arr[n * 3 + 6]); 
                }
                strip_leds.show();
            }
            // Otherwise we flush the serial buffer so that we can wait for the next message frame to come in
            else{
                Serial.flush(); 
            }
        }
        
        // Little time slips between sending messages. 
        os_thread_sleep_ms(5);
    }
}

/*!
*   @brief Initialization code for setting up the strip. 
*/
static void init_strip(void){
    strip_leds.begin();
    strip_leds.setBrightness(20); 
    strip_leds.clear();
    strip_leds.show();
}


/*!
*   @brief Setting an entire strip. 
*/
static void set_entire_strip(uint8_t r, uint8_t g, uint8_t b){
    for(int n = 0; n < LED_STRIP_NUM_LEDS; n++)
        strip_leds.setPixelColor(n, r, g, b); 
}