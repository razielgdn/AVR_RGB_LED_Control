#include "avr/io.h"
#include <avr/interrupt.h>

//#define COUNTER_05_MS_VALUE 10U
//#define COUNTER_10_MS_VALUE 20U
//#define COUNTER_100_MS_VALUE 200U
#define COUNTER_500_MS_VALUE 1000U
//#define COUNTER_1S_VALUE 2000U   // Max value of the main counter to get 
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
int _500ms_ready();