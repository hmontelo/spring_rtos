// Timer.c
// Runs on TM4C123
// Provide pulse measurement functions for Timer0, Timer1, Timer2 and Timer3,

// Daniel Valvano, Jonathan Valvano
// January 7, 2017 

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2015

 Copyright 2015 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

#include <stdint.h>
#include "../inc/timer.h"
#include "../inc/hw/tm4c123gh6pm.h"

#define NVIC_EN0_INT21          0x00200000  // Interrupt 21 enable
#define NVIC_EN0_INT23          0x00800000  // Interrupt 23 enable
#define NVIC_EN1_INT35					0x00000008  // Interrupt 35 enable

int32_t StartCritical(void);
void EndCritical(int32_t primask);

// Subroutine to wait 'time' usec
// Inputs:  None
// Outputs: None
// Notes: ...
void TIMER_Delay(uint32_t time)
{
  time = (uint32_t) (time * TIMER_1US);
  while(time)
  {
    time--;
  }
}

void TIMER_Init_Timer1A(uint32_t period)  
{
	int32_t sr;
	volatile uint32_t delay;
	
	sr = StartCritical();
    SYSCTL_RCGCTIMER_R |= 0x02;    
    delay = SYSCTL_RCGCTIMER_R;
    delay = SYSCTL_RCGCTIMER_R;    
    TIMER1_CTL_R &= ~TIMER_CTL_TAEN;        // 1) disable timer1A during setup
                                            // 2) configure for 32-bit timer mode
    TIMER1_CFG_R = TIMER_CFG_32_BIT_TIMER;
                                            // 3) configure for periodic mode, default down-count settings
    TIMER1_TAMR_R = TIMER_TAMR_TAMR_PERIOD;
    TIMER1_TAILR_R = period - 1;            // 4) reload value
                                            // 5) clear timer1A timeout flag
    TIMER1_ICR_R = TIMER_ICR_TATOCINT;
    TIMER1_IMR_R |= TIMER_IMR_TATOIM;       // 6) arm timeout interrupt
                                            // 7) priority shifted to bits 15-13 for timer1A
    NVIC_PRI5_R = (NVIC_PRI5_R&0xFFFF00FF)|(3 << 13);	//3
    NVIC_EN0_R = NVIC_EN0_INT21;            // 8) enable interrupt 21 in NVIC
    TIMER1_TAPR_R = 0;
    TIMER1_CTL_R |= TIMER_CTL_TAEN;         // 9) enable timer1A	
  EndCritical(sr);
}

void TIMER_Init_Timer2A(uint32_t period) 
{
  uint32_t sr;
  volatile uint32_t delay;

  sr = StartCritical();
  SYSCTL_RCGCTIMER_R |= 0x04;

  delay = SYSCTL_RCGCTIMER_R;
  delay = SYSCTL_RCGCTIMER_R;

  TIMER2_CTL_R &= ~TIMER_CTL_TAEN;      // 1) disable timer1A during setup
                                        // 2) configure for 32-bit timer mode
  TIMER2_CFG_R = TIMER_CFG_32_BIT_TIMER;
                                        // 3) configure for periodic mode, default down-count settings
  TIMER2_TAMR_R = TIMER_TAMR_TAMR_PERIOD;
  TIMER2_TAILR_R = period - 1;          // 4) reload value
                                        // 5) clear timer1A timeout flag
  TIMER2_ICR_R = TIMER_ICR_TATOCINT;
  TIMER2_IMR_R |= TIMER_IMR_TATOIM;     // 6) arm timeout interrupt
                                        // 7) priority shifted to bits 31-29 for timer2A
  NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|(2 << 29);	
  NVIC_EN0_R = NVIC_EN0_INT23;          // 8) enable interrupt 23 in NVIC
  TIMER2_TAPR_R = 0;
  TIMER2_CTL_R |= TIMER_CTL_TAEN;       // 9) enable timer2A

  EndCritical(sr);
}

void TIMER_Init_Timer3A(uint32_t period) 
{
	long sr;
	volatile unsigned long delay;
	
	sr = StartCritical();
  
    SYSCTL_RCGCTIMER_R |= 0x08;    
    delay = SYSCTL_RCGCTIMER_R;
    delay = SYSCTL_RCGCTIMER_R;    
    TIMER3_CTL_R &= ~TIMER_CTL_TAEN;        // 1) disable timer3A during setup
                                            // 2) configure for 32-bit timer mode
    TIMER3_CFG_R = TIMER_CFG_32_BIT_TIMER;
                                            // 3) configure for periodic mode, default down-count settings
    TIMER3_TAMR_R = TIMER_TAMR_TAMR_PERIOD;
    TIMER3_TAILR_R = 0xFFFFFFFF - 1;        // 4) reload value
                                            // 5) clear timer3A timeout flag
    TIMER3_ICR_R = TIMER_ICR_TATOCINT;
    TIMER3_IMR_R |= TIMER_IMR_TATOIM;       // 6) arm timeout interrupt
                                            // 7) priority shifted to bits 15-13 for timer3A
    NVIC_PRI8_R = (NVIC_PRI8_R&0x00FFFFFF)|(1 << 29);	//3
    NVIC_EN1_R = NVIC_EN1_INT35;            // 8) enable interrupt 35 in NVIC
    TIMER3_TAPR_R = 0;
    TIMER3_CTL_R |= TIMER_CTL_TAEN;         // 9) enable timer3A
    
  EndCritical(sr);
}

