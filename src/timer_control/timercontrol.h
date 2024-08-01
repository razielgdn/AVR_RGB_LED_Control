#include "avr/io.h"
#include <avr/interrupt.h>

#define ENABLE_TIMER0_500mS (0)
#define ENABLE_TIMER0_1S (1)
//#define COUNTER_05_MS_VALUE 10U
//#define COUNTER_10_MS_VALUE 20U
//#define COUNTER_100_MS_VALUE 200U
#define COUNTER_500_MS_VALUE 1000U
#define COUNTER_1S_VALUE 2000U   // Max value of the main counter to get 
#define TRUE 1U
#define FALSE 0U

enum timerID{
    ID_TIMER_0=0,
    ID_TIMER_1,
    ID_TIMER_2,
};
/*Functions */
void start_timer(int timer_ID);
void configureTimer0(void);
//void configureTimer1();
//void configureTimer2();
// uint8_t _500ms_ready();

#if (ENABLE_TIMER0_500mS >0 )
uint8_t _500ms_ready();
#endif

#if (ENABLE_TIMER0_1S >0 )
uint8_t _1s_ready();
#endif