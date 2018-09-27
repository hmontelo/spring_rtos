// Timer.h
// Runs on TM4C123
// Provide pulse measurement functions for Timer0, Timer1, Timer2 and Timer3,

// Daniel Valvano, Jonathan Valvano
// January 11, 2017 

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
 
#ifndef _TIMER_H_
#define _TIMER_H_
 
#define TIMER_BUS_FREQUENCY  (uint32_t) Bus80MHz   // default bus frequency used in the system - 80 MHz system clock 
#define TIMER_BUS_CLOCK      (uint32_t) 80000000   // default clock 80 MHz system clock 

#define TIMER_1S     (uint32_t)(TIMER_BUS_CLOCK/1000000)
#define TIMER_1MS    (uint32_t)(TIMER_BUS_CLOCK/1000) 
#define TIMER_2MS    (uint32_t)(2*TIMER_1MS)
#define TIMER_1US    (uint32_t)(TIMER_1MS/1000)
#define TIMER_10US   (uint32_t)(10*TIME_1US)
#define TIMER_100US  (uint32_t)(100*TIME_1US)
#define TIMER_500US  (uint32_t)(500*TIME_1US)  

//------------TIMER_Delay------------
// Subroutine to wait 10 usec
// Inputs: None
// Outputs: None
// Notes: ...
void TIMER_Delay(uint32_t time);

//------------TIMER_Init_Timer1A------------
// Initialize Timer1A
// Input: none
// Output: none
void TIMER_Init_Timer1A(uint32_t period) ;

//------------TIMER_Init_Timer2A------------
// Initialize Timer2A
// Input: none
// Output: none
void TIMER_Init_Timer2A(uint32_t period);

//------------TIMER_Init_Timer3A------------
// Initialize Timer3A
// Input: none
// Output: none
void TIMER_Init_Timer3A(uint32_t period);

#endif //_TIMER_H_

