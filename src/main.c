//header sections
//#define F_CPU 1000000  uncomment if the frequency is not defined
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//project headers 
#define TRUE 1U
#define FALSE 0U
#include "timer_control/timercontrol.h"
#include "LED_control/ledcontrol.h"

uint8_t flankDetector(uint8_t last_signal, uint8_t now_signal);
void init_device();
uint8_t last_signal;
uint8_t now_signal;

int main (){
    init_device();

    while (1)    
    {
        change_LED_status (flankDetector( _1s_ready(), _1s_ready()) );
    }    
    return 0;
}

void init_device(){
    //initializing portB first nibble as output and all off.
    DDRB = 0x0F;
    PORTB = 0x00; 
    configureTimer0();    
    start_timer(0);    
    sei();   
    PORTB = 0x00; 
}

uint8_t flankDetector(uint8_t last_signal, uint8_t now_signal){
    if (last_signal==FALSE && now_signal == TRUE)
    {
        return TRUE;
    }
    return FALSE;   
}
