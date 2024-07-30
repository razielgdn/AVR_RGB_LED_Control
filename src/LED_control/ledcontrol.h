#include "avr/io.h"

enum RGBLED_STATES{       // BGR
    ALL_OFF = 0,          // 000 All LEDs off
    RGBLED_RED,           // 001 Red LED on 
    RGBLED_GREEN,         // 010 Green LED on
    RGBLED_RED_GREEN,     // 011 Red and Green LEDs on
    RGBLED_BLUE,          // 100 Blue LED on
    RGBLED_BLUE_RED,      // 101 Blue and Red LEDs on
    RGBLED_BLUE_GREEN,    // 110 Blue and Green LEDs on
    ALL_ON                // 111 All LEDs on
};

typedef struct States {
    uint8_t RGB_color;
    uint8_t next_state;
} LED_states;

void change_LED_status(int cycle_Flag);