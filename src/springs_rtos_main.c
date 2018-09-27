// springs_rtos_main.c
// Runs on TI's TM4C123
// Real Time Operating System for Labs 2
// Lab2 Part 1: Testmain1 and Testmain2
// Lab2 Part 2: Testmain3 Testmain4  and main

#define RTOS_LAB2_PART_01 
//#define RTOS_LAB2_PART_02

#include <stdio.h>
#include <stdint.h>
#include "../inc/pll.h"
#include "../inc/os.h"
#include "../inc/timer.h"
#include "../inc/hw/tm4c123gh6pm.h"

#define PE0  (*((volatile unsigned long *)0x40024004))
#define PE1  (*((volatile unsigned long *)0x40024008))
#define PE2  (*((volatile unsigned long *)0x40024010))
#define PE3  (*((volatile unsigned long *)0x40024020))

#define THREAD_DEFAULT_STACK_SIZE (uint32_t) 128

static uint32_t NumThreadCreated;   // number of foreground threads created
static uint32_t Count1;             // number of times thread1 loops
static uint32_t Count2;             // number of times thread2 loops
static uint32_t Count3;             // number of times thread3 loops

// Internal functions - Prototypes
void Profile_Init(void);

#ifdef RTOS_LAB2_PART_01
void    Thread1_Lab2_Part1(void);
void    Thread2_Lab2_Part1(void);
void    Thread3_Lab2_Part1(void);
int16_t TestLab2_Part1(void);

#define Thread1_Lab2()  Thread1_Lab2_Part1()
#define Thread2_Lab2()  Thread2_Lab2_Part1()
#define Thread3_Lab2()  Thread3_Lab2_Part1()
#define TestLab2()      TestLab2_Part1()

#endif //RTOS_LAB2_PART_01

#ifdef RTOS_LAB2_PART_02

void    Thread1_Lab2_Part2(void);
void    Thread2_Lab2_Part2(void);
void    Thread3_Lab2_Part2(void);
int16_t TestLab2_Part2(void);

#define Thread1_Lab2()  Thread1_Lab2_Part2()
#define Thread2_Lab2()  Thread2_Lab2_Part2()
#define Thread3_Lab2()  Thread3_Lab2_Part2()
#define TestLab2()      TestLab2_Part2()

#endif //RTOS_LAB2_PART_02

#define TEST_MAIN() TestLab2()


// Internal functions - Implementation

#ifdef RTOS_LAB2_PART_01
void Thread1_Lab2_Part1(void)
{
  Count1 = (uint32_t) 0;          
  for(;;)
  {
    PE0 ^= 0x01;       // heartbeat
    Count1++;
		OS_Suspend();      // cooperative multitasking
  }
}
void Thread2_Lab2_Part1(void)
{
  Count2 = (uint32_t) 0;          
  for(;;)
  {
    PE1 ^= 0x02;       // heartbeat
    Count2++;
    OS_Suspend();      // cooperative multitasking
  }
}
void Thread3_Lab2_Part1(void)
{
  Count3 = (uint32_t) 0;          
  for(;;)
  {
    PE2 ^= 0x04;       // heartbeat
    Count3++;
    OS_Suspend();      // cooperative multitasking
  }
}

int16_t TestLab2_Part1(void)
{
  OS_Init();          // initialize, disable interrupts
  Profile_Init();       // profile user threads
  NumThreadCreated = 0 ;
  NumThreadCreated += OS_AddThread(&Thread1_Lab2_Part1,128,1); 
  NumThreadCreated += OS_AddThread(&Thread2_Lab2_Part1,128,2); 
  NumThreadCreated += OS_AddThread(&Thread3_Lab2_Part1,128,3); 
  // Count1 Count2 Count3 should be equal or off by one at all times
  OS_Launch(TIMER_2MS); // doesn't return, interrupts enabled in here
  return 0;            // this never executes
}

#endif // RTOS_LAB2_PART_01

#ifdef RTOS_LAB2_PART_02

//*******************Second TEST**********
// Once the initalize test runs, test this (Lab 2 part 1)
// no UART interrupts
// SYSTICK interrupts, with or without period established by OS_Launch
// no timer interrupts
// no switch interrupts
// no ADC serial port or LCD output
// no calls to semaphores
void Thread1_Lab2_Part2(void)
{
  Count1 = (uint32_t) 0;          
  for(;;)
  {
    PE0 ^= 0x01;       // heartbeat
    Count1++;
  }
}
void Thread2_Lab2_Part2(void)
{
  Count2 = (uint32_t) 0;          
  for(;;)
  {
    PE1 ^= 0x02;       // heartbeat
    Count2++;
  }
}
void Thread3_Lab2_Part2(void)
{
  Count3 = (uint32_t) 0;          
  for(;;)
  {
    PE2 ^= 0x04;       // heartbeat
    Count3++;
    /* if (Count3 == 1000)
      OS_Kill(); */ 
  }
}

int16_t TestLab2_Part2(void)
{
  OS_Init();           // initialize, disable interrupts
  Profile_Init();       // profile user threads
  NumThreadCreated = 0 ;
  NumThreadCreated += OS_AddThread(&Thread1_Lab2_Part2,128,1); 
  NumThreadCreated += OS_AddThread(&Thread2_Lab2_Part2,128,2); 
  NumThreadCreated += OS_AddThread(&Thread3_Lab2_Part2,128,3); 
  // Count1 Count2 Count3 should be equal on average
  // counts are larger than testmain1

  OS_Launch(TIMER_2MS); // doesn't return, interrupts enabled in here
  return 0;            // this never executes
}

#endif // RTOS_LAB2_PART_02


void Profile_Init(void)
{ 
	unsigned long volatile delay;
  SYSCTL_RCGCGPIO_R |= 0x10;        // activate port E
  delay = SYSCTL_RCGCGPIO_R;        
  delay = SYSCTL_RCGCGPIO_R;         
  GPIO_PORTE_DIR_R |= 0x0F;         // make PE3-0 output heartbeats
  GPIO_PORTE_AFSEL_R &= ~0x0F;      // disable alt funct on PE3-0
  GPIO_PORTE_DEN_R |= 0x0F;         // enable digital I/O on PE3-0
  GPIO_PORTE_PCTL_R = ~0x0000FFFF;
  GPIO_PORTE_AMSEL_R &= ~0x0F;;     // disable analog functionality on PF
}


int main(void) 
{
  TEST_MAIN();
}
  
