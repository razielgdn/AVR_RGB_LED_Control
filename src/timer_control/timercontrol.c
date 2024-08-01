/* timercounter.c will contains the control of the timer counters. creating functions to setup and manage its behavior.*/

/* ******************************************************************************************
*  Timer counter 0 is controlled by next registers. 
*  ******************************************************************************************
*
* RCCR0A: Timer counter Control Register A
*       |    7   |     6  |    5   |    4   |  3  |  2  |   1   |    0  |    
*       | COM0A1 | COM0A0 | COM0B1 | COM0B0 |  -  |  -  | WGM01 | WGM00 | 
*       - COM0A: Compare Match output A mode: 
*               -> non - PWM Mode
*                  00 Normal port operation OC0A disconnected.
*                  01 Toggle OC0A pin on Compare Match.
*                  10 Clear OC0A pin, 11 Set OC0A PIN.
*                  11 Set OC0A PIN.
*               -> Fast PWM Mode
*                  00 Normal port operation OC0A disconnected.
*                  01 WGM02=0:  Normal port operation OC0A disconnected
*                     WGM02=1:  Toggle OC0A on Compare Match.
*                  10 Clear OC0A on Compare Match, set OC0A at bottom. (non-inverting mode)
*                  11 Set OC0A on Compare Match, clear OC0A at bottom. (inverting mode)
*               -> Phase correct PWM Mode
*                  00 Normal port operation OC0A disconnected.
*                  01 WGM02=0:  Normal port operation OC0A disconnected
*                     WGM02=1:  Toggle OC0A on Compare Match.
*                  10 Clear OC0A on Compare Match when up-counting. Clear OC0A on Compare Match when down-counting.
*                  11 Set OC0A on Compare Match when up-counting. Clear OC0A on Compare Match when down-counting.                
*
*       - COM0B: Same functions as COM0A to control OC0B pin.     
*       - WGM: Waveform Generation Mode: combined with WGM02 bit found in the TCCR0B Register, these bits control the counting sequence of the counter. 
*               | Mode | WGM02 | WGM01 | WGM00 | Timer/counter Mode |  TOP  | Update of OCRx at | TOV Flag set on |
*               |   0  |   0   |   0   |   0   |       Normal       |  0xFF |     Immediate     |       MAX       | 
*               |   1  |   0   |   0   |   1   | PWM, Phase Correct |  0xFF |        TOP        |     BOTTOM      | 
*               |   2  |   0   |   1   |   0   |         CTC        |  OCRA |     Immediate     |       MAX       |  
*               |   3  |   0   |   1   |   1   |      Fast PWM      |  0xFF |      BOTTOM       |       MAX       |
*               |   4  |   1   |   0   |   0   |      Reserved      |   -   |         -         |        -        |
*               |   5  |   1   |   0   |   1   | PWM, Phase Correct |  OCRA |        TOP        |     BOTTOM      |
*               |   6  |   1   |   1   |   0   |      Reserved      |   -   |         -         |        -        |
*               |   7  |   1   |   1   |   1   |      Fast PWM      |  OCRA |      BOTTON       |       TOP       |
* TCCR0B: Timer/Counter Control Register B       
*       |   7   |   6   | 5 | 4 |   3   |   2  |   1  |   0  |
*       | FOC0A | FOC0B | - | - | WGM02 | CS02 | CS01 | CS00 |   
*       FOC0A: Force Output compare A. The FOC0A bit is only active when the WGM bits specify a non-PWM mode.    
*       FOC0B: Force Output Compare B. 
*       WGM02: Waveform Generation Mode.
*       CS: Clock Select -> |CS02|CS01|CS00|
*           000: No clock source
*           001: clk_IO (No Prescaling)
*           010: clk/8 (From prescaler)
*           011: clk/64 (From prescaler)
*           100: clk/256 (From prescaler)
*           101: clk/1024 (From prescaler)
*           110: External clock source on T0 pin. Clock on falling edge.
*           111: External clock source on T0 pin. Clock on rising edge.
* TCNT0: Timer/Counter Register. Modifying the counter while is running, introduces a risk of missing 
*        a compare Match between TCNT0 and the OCR0x Registers.
* OCR0A: Output Compare Register A. The value on this register is compared with the counter value and a match can be used to 
*        generate an interrupt
* OCR0B: Output Compare Register B. Same function as Register A.
* TIMSK0: Timer/Counter Interrupt Mask Register.
        | 7 | 6 | 5 | 4 | 3 |   2    |   1    |   0   |
        | - | - | - | - | - | OCIE0B | OCIE0A | TOIE0 |

*       OCIE0B: Timer/Counter Output Compare Match B Interrupt Enable.
*       OCIE0A: Timer/Counter0 Output Compare Match A Interrupt Enable.
*       TOIE0: Timer/Counter0 Overflow Interrupt Enable.                             
*/ 


/****************************************** Includes *******************************************/
#include "timercontrol.h"
/* Timer variables and definitions*/
//static  uint8_t counter_5ms=COUNTER_05_MS_VALUE;
//static  uint8_t counter_10ms=COUNTER_10_MS_VALUE;
//static  uint8_t counter_100ms=COUNTER_100_MS_VALUE;
// 
//uint8_t tc_flag_05ms=0;
//uint8_t tc_flag_10ms=0;
//uint8_t tc_flag_100ms=0;
// 
#if (ENABLE_TIMER0_500mS >0)
static uint8_t tc_flag_500ms=0;
static  uint16_t counter_500ms=COUNTER_500_MS_VALUE;
#endif

#if (ENABLE_TIMER0_1S >0 )
static  uint16_t counter_1s=COUNTER_1S_VALUE;
static uint8_t tc_flag_1s=0;
#endif

/* The idea of this project is do a timer base to manage the MCU resources and process. 
   Timer 0 will be the main timer to manage process in the MCU
 */
void configureTimer0(){
    
   TCCR0A = (0x0<<COM0A1) | (0x0<<COM0A0) | (0x0<<COM0B1) |(0x0<<COM0B1) | (0x1<<WGM01) | (0x0<<WGM00); //0x02
   TCCR0B = (0x0<<FOC0A) | (0x0<<FOC0B) | (0x0<<WGM02) | (0x0<<CS02) | (0x1<<CS01) | (0x0<<CS00);       //0x02
   TCNT0 = 0x00;
   OCR0A = 249;
   OCR0B = 0x00;   
   // The timer is configured with a clk div of 8 and  the TOP of 249 to create a frequency of 2Khz (0.0005s)
}

#if (ENABLE_TIMER0_500mS > 0)
uint8_t _500ms_ready()
{
    return tc_flag_500ms;
}
#endif

#if (ENABLE_TIMER0_1S >0 )
uint8_t _1s_ready()
{
    return tc_flag_1s;
}
#endif

ISR(TIMER0_COMPA_vect)
{
    // the algorithm will use descendant counts. 
    // 5ms BaseTimer
 #if (ENABLE_TIMER0_500mS)   
     if (counter_500ms)
     {
         counter_500ms--; 
         tc_flag_500ms=FALSE;          
     }
     else
     {
          counter_500ms=COUNTER_500_MS_VALUE;        
          tc_flag_500ms=TRUE;
     }
 #endif   
// 1s BaseTimer
#if (ENABLE_TIMER0_1S >0 )

    if (counter_1s)
    {
        counter_1s--; 
        tc_flag_1s=FALSE;          
    }
    else
    {
         counter_1s=COUNTER_500_MS_VALUE;        
         tc_flag_1s=TRUE;
    }
#endif
}

/*
void configureTimer2(){
   TCCR2A = (0x0<<COM0A1)|(0x0<<COM0A0)| (0x0<<COM0B1)|(0x0<<COM0B1)|(0x0<<WGM01)|(0x0<<WGM00);
   TCCR2B = (0x0<<FOC2A)|(0x0<<FOC2B)| (0x0<<WGM22)|(0x0<<CS22)|(0x0<<CS21)|(0x0<<CS20);
   TCNT2 = 0x00;
   OCR2A = 0x00;
   OCR2B = 0x00;
   TIMSK2 =(0x0<<OCIE2B)|(0x0<<OCIE2A)|(0x0<<TOIE2);     
}
*/

void start_timer(int timer_ID)
{
     switch (timer_ID)
     {
     case ID_TIMER_0:
         TIMSK0 =(0x0<<OCIE0B)|(0x1<<OCIE0A)|(0x0<<TOIE0); // Enable interruptions related to the timer 0
         break;
//     case ID_TIMER_1:        
//         break;
//     case ID_TIMER_2:
//         TIMSK2 =(0x0<<OCIE2B)|(0x0<<OCIE2A)|(0x0<<TOIE2);     
//         break;    
     default:
        TIMSK0=0x00;
        TIMSK1=0x00;
        TIMSK2=0x00;
         break;
     }
}     FALSE