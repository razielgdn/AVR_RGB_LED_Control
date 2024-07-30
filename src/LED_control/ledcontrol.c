 #include "ledcontrol.h"  
 LED_states states_machine[8] = 
    {                                            
        { ALL_OFF,          RGBLED_RED  },       // Off to Red
        { RGBLED_RED,       RGBLED_GREEN},       // Red to green
        { RGBLED_GREEN,     RGBLED_BLUE },       // Green to Blue
        { RGBLED_RED_GREEN, RGBLED_BLUE},        // Not used 
        { RGBLED_BLUE,      RGBLED_RED},       	 // Blue to Red
        { RGBLED_BLUE_RED,  RGBLED_BLUE_GREEN},  // Not used 
        { ALL_ON,           ALL_OFF}             // All on to all Off
    }; 

void change_LED_status(int cycle_Flag){
    static int LED_color=0;
    
    PORTB = states_machine[LED_color].RGB_color;
    
    if(cycle_Flag)
    {
        LED_color = states_machine[LED_color].next_state;        
    }
}

