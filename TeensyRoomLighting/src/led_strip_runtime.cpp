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
static WS2812Serial strip_leds = WS2812Serial(LED_STRIP_NUM_LEDS, matrix_display_memory, matrix_drawing_memory, LED_STRIP_GPIO, WS2812_GRBW);

/*!
*   @brief LED strip thread stack and stack size. 
*/
static const int LED_STRIP_THREAD_STACK_SIZE = 8192; 
static uint32_t led_strip_thread_stack[LED_STRIP_THREAD_STACK_SIZE]; 

/*!
*   @brief Hsv Array that serves as a helper for the animation
*/
static HsvColor hsv_helper_arr[LED_STRIP_NUM_LEDS]; 

/*!
*   @brief Animation Data Struct that directs all the other animation patterns. 
*/
LEDStripAnimationData latest_led_strip_animation_data; 

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
static void set_entire_strip(uint8_t r, uint8_t g, uint8_t b, uint8_t w); 

/*!
*   @brief Animation function declarations
*/
static void twinkle_random(void); 
static void twinkle(void); 
static void fade(void); 
static void fade_random(void); 
static void set(void); 
static void set_random(void); 
static void fade_to(void); 
static void fade_to_random(void); 
static void knight_ridder(void); 
static void knight_ridder_random(void); 
static void rainbow_glitter(void); 
static void rainbow(void); 
static void juggle(void); 

/*!
*   @brief List of animation functions so it's easy to lookup and cycle through them 
*   @note Matches with the enumerated values in the protomessage
*/
typedef void (*AnimationFunctionList[])(void);
static AnimationFunctionList ani_list = {
    twinkle_random, 
    twinkle, 
    fade, 
    fade_random, 
    set, 
    set_random, 
    fade_to, 
    fade_to_random, 
    knight_ridder, 
    knight_ridder_random, 
    rainbow_glitter, 
    rainbow, 
    juggle
}; 

/*!
*   @brief Starting led strip runtime thread. 
*/
void start_led_strip_runtime(void){
    os_add_thread(&led_strip_thread, NULL, LED_STRIP_THREAD_STACK_SIZE, led_strip_thread_stack);  
    add_message_callback(MessageData_MessageType_LED_STRIP_ANI, &led_message_callback); 
}

/*!
*   @brief Strip thread. 
*   @param void* general pointer. 
*/
static void led_strip_thread(void *parameters){
    init_strip(); 

    // Clearing out different animation data. 
    memset(hsv_helper_arr, 0, sizeof(hsv_helper_arr)); 
    memset(&latest_led_strip_animation_data, 0, sizeof(latest_led_strip_animation_data)); 

    // Goes on to work on the next animation
    for(;;)
        ani_list[(uint16_t)latest_led_strip_animation_data.strip_animation_enum](); 

}

/*!
*   @brief Callback everytime we have information coming from the serial bus. 
*   @param MessageReq* message pointer information
*/
static void led_message_callback(MessageReq *msg_req){
    latest_led_strip_animation_data = unpack_led_strip_animation_data(msg_req->data_ptr, msg_req->data_len); 
    // Flags the animation changed bits. 
    animation_change_signal.signal(THREAD_SIGNAL_0); 
}

/*!
*   @brief Initialization code for setting up the strip. 
*/
static void init_strip(void){
    strip_leds.begin(); 
    strip_leds.clear();
    strip_leds.show();
}

/*!
*   @brief Animation changed helper function since we put a bunch of these 
*   @brief always checking for next animation
*/
static inline bool animation_changed(uint32_t ms){
    if(animation_change_signal.wait(THREAD_SIGNAL_0, ms))
        return true; 
    
    return false; 
}

/*!
*   @brief Setting an entire strip. 
*/
static void set_entire_strip(uint8_t r, uint8_t g, uint8_t b, uint8_t w){
    for(int n = 0; n < LED_STRIP_NUM_LEDS; n++)
        strip_leds.setPixelColor(n, r, g, b, w); 
}

/*!
*   @brief  Helper function that deals with twinkle
*   @note   Helper functions that deals with 
*/
static void twinkle_random(void){
    for(;;){
        for(int n = 0; n < LED_STRIP_NUM_LEDS; n++){
            RgbColor col = HsvToRgb(hsv_helper_arr[n]); 
            strip_leds.setPixelColor(n, col.r, col.g, col.b, 0); 
        }

        for(int n = 0; n < (LED_STRIP_NUM_LEDS/173) + 1; n++)
            hsv_helper_arr[random(LED_STRIP_NUM_LEDS)] = {random(255), 255, 255};

        for(int n = 0; n < LED_STRIP_NUM_LEDS; n++)
            if(hsv_helper_arr[n].v >= 15)
                hsv_helper_arr[n].v -= 15; 

        strip_leds.show(); 
        if(animation_changed(15))
            return; 
    }    
}

static void twinkle(void){

}

static void fade(void){

}

static void fade_random(void){

}

static void set(void){

} 

static void set_random(void){

}

static void fade_to(void){

}

static void fade_to_random(void){

}

static void knight_ridder(void){

}

static void knight_ridder_random(void){

}

static void rainbow_glitter(void){

}

static void rainbow(void){

}

static void juggle(void){

}