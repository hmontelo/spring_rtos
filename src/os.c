// os.c
// Runs on LM4F120/TM4C123
// A very simple real time operating system with minimal features.
// Daniel Valvano
// January 29, 2015

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to ARM Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2015

   Programs 4.4 through 4.12, section 4.2

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
#include "../inc/os.h"
#include "../inc/pll.h"
#include "../inc/st7735.h"
#include "../inc/uart.h"
#include "../inc/timer.h"
#include "../inc/hw/tm4c123gh6pm.h"

#define NVIC_ST_CTRL_R          (*((volatile uint32_t *)0xE000E010))
#define NVIC_ST_CTRL_CLK_SRC    0x00000004  // Clock Source
#define NVIC_ST_CTRL_INTEN      0x00000002  // Interrupt enable
#define NVIC_ST_CTRL_ENABLE     0x00000001  // Counter mode
#define NVIC_ST_RELOAD_R        (*((volatile uint32_t *)0xE000E014))
#define NVIC_ST_CURRENT_R       (*((volatile uint32_t *)0xE000E018))
#define NVIC_INT_CTRL_R         (*((volatile uint32_t *)0xE000ED04))
#define NVIC_INT_CTRL_PENDSTSET 0x04000000  // Set pending SysTick interrupt
#define NVIC_SYS_PRI3_R         (*((volatile uint32_t *)0xE000ED20))  // Sys. Handlers 12 to 15 Priority

// Additional defines for Lab 2
#define NVIC_EN0_INT21          0x00200000  // Interrupt 21 enable

#define TIMER_CFG_32_BIT_TIMER  0x00000000  // 32-bit timer configuration
#define TIMER_TAMR_TACDIR       0x00000010  // GPTM Timer A Count Direction
#define TIMER_TAMR_TAMR_PERIOD  0x00000002  // Periodic Timer mode
#define TIMER_CTL_TAEN          0x00000001  // GPTM TimerA Enable
#define TIMER_IMR_TATOIM        0x00000001  // GPTM TimerA Time-Out Interrupt
                                            // Mask
#define TIMER_ICR_TATOCINT      0x00000001  // GPTM TimerA Time-Out Raw
                                            // Interrupt
#define TIMER_TAILR_M           0xFFFFFFFF  // GPTM Timer A Interval Load
 
#define MAX_NUMBER_OF_THREADS  (uint32_t) 5     // maximum number of threads
#define MAX_THREAD_STACKSIZE   (uint32_t) 192   // number of 32-bit words in stack
#define MAX_PRIORITY_NUMBER    (uint32_t) 7     // number of priorities
#define THREAD_NOT_AVAILABLE   (uint8_t) 0
#define THREAD_AVAILABLE       (uint8_t) 1
#define SUCCESS                (int16_t) 0
#define FAIL                   (int16_t) -1

typedef struct tcb
{
  int32_t     *sp;      // pointer to stack (valid for threads not running
  struct tcb  *next;    // linked-list pointer
  int32_t     *status;  // pointer to resource thread is blocked on (0 if not)
  uint32_t    sleepCt;	// sleep counter in MS
  uint32_t    age;      // how long the thread has been active
  uint32_t    id;       // thread #
  uint32_t    priority; // used in priority scheduling
}tcbType;
	 
void WaitForInterrupt(void);  // low power mode
void (*PeriodicTask)(void);   // user function

// function definitions in osasm.s
void    OS_DisableInterrupts(void);  // Disable interrupts
void    OS_EnableInterrupts(void);   // Enable interrupts
int32_t StartCritical(void);      // previous I bit, disable interrupts
void    EndCritical(int32_t status); // restore I bit to previous value
void    StartOS(void);
void    InitAvailableThreads(void);
void    InitializeThreadStack(int16_t index);
int16_t add_thread(void);
int16_t delete_thread(int16_t thread);
int16_t find_prev(int16_t thread);
int16_t find_next(int16_t thread);


////
tcbType tcbs[MAX_NUMBER_OF_THREADS];
tcbType *RunPt;
tcbType *NextRunPt;
int32_t Stacks[MAX_NUMBER_OF_THREADS][MAX_THREAD_STACKSIZE];
uint8_t AvailableThreads[MAX_NUMBER_OF_THREADS];
static uint32_t MSTime      = (uint32_t) 0x00;
static uint32_t NumThreads  = (uint32_t) 0x00;

////
void InitAvailableThreads(void) 
{
  uint16_t i =0;
  
	for (i = 0; i < MAX_NUMBER_OF_THREADS; ++i) 
  {
		AvailableThreads[i] = THREAD_AVAILABLE;
	}
}

void OS_Timer1A_Handler(void)
{ 
  TIMER1_ICR_R = TIMER_ICR_TATOCINT;// acknowledge timer1A timeout
  (*PeriodicTask)();
}

void OS_Timer2A_Handler(void)
{ 
  uint16_t i;

  TIMER2_ICR_R = TIMER_ICR_TATOCINT;// acknowledge timer2A timeout
  MSTime++;

  for(i = 0; i < MAX_NUMBER_OF_THREADS; ++i) 
  {
    if(!AvailableThreads[i] && tcbs[i].sleepCt) 
    {
      tcbs[i].sleepCt -= 1;
    }
  }
}

void OS_Timer3A_Handler(void)
{ 
	
  TIMER3_ICR_R = TIMER_ICR_TATOCINT;// acknowledge timer3A timeout
}

void InitializeThreadStack(int16_t index)
{
  tcbs[index].sp = &Stacks[index][MAX_THREAD_STACKSIZE-16]; // thread stack pointer
  Stacks[index][MAX_THREAD_STACKSIZE-1]  = 0x01000000;   // thumb bit
  Stacks[index][MAX_THREAD_STACKSIZE-3]  = 0x14141414;   // R14
  Stacks[index][MAX_THREAD_STACKSIZE-4]  = 0x12121212;   // R12
  Stacks[index][MAX_THREAD_STACKSIZE-5]  = 0x03030303;   // R3
  Stacks[index][MAX_THREAD_STACKSIZE-6]  = 0x02020202;   // R2
  Stacks[index][MAX_THREAD_STACKSIZE-7]  = 0x01010101;   // R1
  Stacks[index][MAX_THREAD_STACKSIZE-8]  = 0x00000000;   // R0
  Stacks[index][MAX_THREAD_STACKSIZE-9]  = 0x11111111;   // R11
  Stacks[index][MAX_THREAD_STACKSIZE-10] = 0x10101010;   // R10
  Stacks[index][MAX_THREAD_STACKSIZE-11] = 0x09090909;   // R9
  Stacks[index][MAX_THREAD_STACKSIZE-12] = 0x08080808;   // R8
  Stacks[index][MAX_THREAD_STACKSIZE-13] = 0x07070707;   // R7
  Stacks[index][MAX_THREAD_STACKSIZE-14] = 0x06060606;   // R6
  Stacks[index][MAX_THREAD_STACKSIZE-15] = 0x05050505;   // R5
  Stacks[index][MAX_THREAD_STACKSIZE-16] = 0x04040404;   // R4
}

int16_t add_thread(void) 
{
  int16_t ret = (int16_t) -1;
  int16_t i   = (uint16_t) 0;
  
  for (i=0; (i < MAX_NUMBER_OF_THREADS) && (ret < 0); ++i) 
  {
    if( AvailableThreads[i] != THREAD_NOT_AVAILABLE ) 
    {
      ret = i;
      AvailableThreads[i] = THREAD_NOT_AVAILABLE;
    }
  }
  return ret;
}

int16_t delete_thread(int16_t thread) 
{
  int16_t ret = (int16_t) -1;
  
  // Cannot release thread which is already available
  if (AvailableThreads[thread] == THREAD_NOT_AVAILABLE) 
  {
    AvailableThreads[thread] = THREAD_AVAILABLE;
    ret = (int16_t) 0;  
  }
  
  return ret;
}

int16_t find_prev(int16_t thread) 
{
  int16_t ret = (int16_t) -1;
  int16_t i   = (uint16_t) 0;
  
  for(i = (thread+MAX_NUMBER_OF_THREADS-1)%MAX_NUMBER_OF_THREADS; (i != thread) && (ret < 0) ; i = (i+MAX_NUMBER_OF_THREADS-1)%MAX_NUMBER_OF_THREADS ) 
  {
    if (AvailableThreads[i] == THREAD_NOT_AVAILABLE) 
    {
      ret = i;
    }
  }
  return ret;
}

int16_t find_next(int16_t thread) 
{
  int16_t ret = (int16_t) -1;
  int16_t i   = (uint16_t) 0;  

  for (i = (thread+1)%MAX_NUMBER_OF_THREADS; (i != thread) && (ret < 0); i = (i+1)%MAX_NUMBER_OF_THREADS ) 
  {
    if (AvailableThreads[i] == THREAD_NOT_AVAILABLE) 
    {
      ret = i;
    }
  }
  return ret;
}

void SysTick_Handler(void) 
{	
  NextRunPt = RunPt->next;

  while(NextRunPt->sleepCt) 
  {
    NextRunPt = NextRunPt->next;
  }
  NVIC_INT_CTRL_R = 0x10000000;		// trigger PendSV
}


// ******** OS_Init ************
// initialize operating system, disable interrupts until OS_Launch
// initialize OS controlled I/O: serial, ADC, systick, LaunchPad I/O and timers 
// input:  none
// output: none
void OS_Init(void) 
{
  OS_DisableInterrupts();
  PLL_Init(TIMER_BUS_FREQUENCY); // initialize PLL (Phase-Locked Loop) and set system clock to Frequency in MHz
  Output_Init();              // initialize liquid crystal display ST7735 
  UART_InitA(TIMER_BUS_FREQUENCY, UART_enBaudRate115200);  // initialize UART (Universal asynchronous receiver/transmitter)
  InitAvailableThreads();
  TIMER_Init_Timer2A(TIMER_1MS);
  TIMER_Init_Timer3A(TIMER_1S);
  
  NVIC_ST_CTRL_R = 0;         // disable SysTick during setup
  NVIC_ST_CURRENT_R = 0;      // any write to current clears it
                              // lowest PRI so only foreground interrupted
  NVIC_SYS_PRI3_R =(NVIC_SYS_PRI3_R&0x00FFFFFF)|0xE0000000; // priority 7

  // Use PendSV to trigger a context switch
  NVIC_SYS_PRI3_R =(NVIC_SYS_PRI3_R&0xFF00FFFF)|0x00D00000; // priority 6	
  
  MSTime = (uint32_t) 0;
}


//******** OS_AddThread *************** 
// add a foregound thread to the scheduler
// Inputs: pointer to a void/void foreground task
//         number of bytes allocated for its stack
//         priority, 0 is highest, 5 is the lowest
// Outputs: 1 if successful, 0 if this thread can not be added
// stack size must be divisable by 8 (aligned to double word boundary)
// In Lab 2, you can ignore both the stackSize and priority fields
// In Lab 3, you can ignore the stackSize fields

int16_t OS_AddThread(void(*task)(void), uint32_t stackSize, uint32_t priority) 
{	
  int16_t ret = FAIL;  
	int32_t status, prev;
  int32_t threadIndex = (int32_t) 0x00;
  
  if( (task != 0)
      && (priority <= MAX_PRIORITY_NUMBER)
      && (stackSize <= MAX_THREAD_STACKSIZE)
    )
  { 
    status = StartCritical();
      if(NumThreads == (uint32_t) 0x00) 
      {
        threadIndex = (int32_t) 0x00;
        add_thread();
        tcbs[0].next = &tcbs[0]; // 0 points to 0
        RunPt = &tcbs[0];     // thread 0 will run first
      }
      else 
      {
        threadIndex = add_thread();
        prev = find_prev(threadIndex);
        tcbs[threadIndex].next = tcbs[prev].next;
        tcbs[prev].next = &tcbs[threadIndex];
      }
      
      tcbs[threadIndex].status   = (int32_t)  0;
      tcbs[threadIndex].sleepCt  = (uint32_t) 0;
      tcbs[threadIndex].age      = (uint32_t) 0;
      tcbs[threadIndex].id       = threadIndex;
      
      InitializeThreadStack(threadIndex); 
      Stacks[threadIndex][MAX_THREAD_STACKSIZE-2] = (int32_t)(task); // PC
      NumThreads++;
      ret = SUCCESS;
    EndCritical(status);
  }
	
  return ret;
}
   
//******** OS_Id *************** 
// returns the thread ID for the currently running thread
// Inputs: none
// Outputs: Thread ID, number greater than zero 
uint32_t OS_Id(void) 
{ 
	return RunPt->id;
}


//******** OS_AddPeriodicThread *************** 
// add a background periodic task
// typically this function receives the highest priority
// Inputs: pointer to a void/void background function
//         period given in system time units (12.5ns)
//         priority 0 is the highest, 5 is the lowest
// Outputs: 1 if successful, 0 if this thread can not be added
// You are free to select the time resolution for this function
// It is assumed that the user task will run to completion and return
// This task can not spin, block, loop, sleep, or kill
// This task can call OS_Signal  OS_bSignal	 OS_AddThread
// This task does not have a Thread ID
// In lab 2, this command will be called 0 or 1 times
// In lab 2, the priority field can be ignored
// In lab 3, this command will be called 0 1 or 2 times
// In lab 3, there will be up to four background threads, and this priority field 
//           determines the relative priority of these four threads
int16_t OS_AddPeriodicThread(void(*task)(void), 
   uint32_t period, uint32_t priority)
{
  int16_t ret = FAIL;
  
  if( (task != 0)
      && (priority <= MAX_PRIORITY_NUMBER)
    )
  {
    PeriodicTask = task;
    TIMER_Init_Timer1A(period);
    ret = SUCCESS;    
  }
  return ret;
}

// ******** OS_Sleep ************
// place this thread into a dormant state
// input:  number of msec to sleep
// output: none
// You are free to select the time resolution for this function
// OS_Sleep(0) implements cooperative multitasking
void OS_Sleep(uint32_t sleepTime) 
{ 
  RunPt->sleepCt = sleepTime;
} 

// ******** OS_Kill ************
// kill the currently running thread, release its TCB and stack
// input:  none
// output: none
void OS_Kill(void) 
{ 
	uint32_t thread, prev;
  
	thread = RunPt->id;
	prev = find_prev(thread);
	delete_thread(thread);
	NumThreads--;
	tcbs[prev].next = tcbs[thread].next;
	OS_Suspend();	
	//	NVIC_INT_CTRL_R = 0x10000000;		// trigger PendSV
} 

// ******** OS_Suspend ************
// suspend execution of currently running thread
// scheduler will choose another thread to execute
// Can be used to implement cooperative multitasking 
// Same function as OS_Sleep(0)
// input:  none
// output: none
void OS_Suspend(void) 
{ 
   
	NVIC_ST_CURRENT_R = 0;			    // clear counter
	NVIC_INT_CTRL_R = 0x04000000;		// trigger SysTick
}



// ******** OS_TimeDifference ************
// Calculates difference between two times
// Inputs:  two times measured with OS_Time
// Outputs: time difference in 12.5ns units 
// The time resolution should be less than or equal to 1us, and the precision at least 12 bits
// It is ok to change the resolution and precision of this function as long as 
//   this function and OS_Time have the same resolution and precision 
uint32_t OS_TimeDifference(uint32_t start, uint32_t stop) 
{
	uint32_t difference;
	if (stop < start) 
  {
		difference = 0xFFFFFFFF - start + stop;
	} 
	else 
  {
		difference = stop-start;
	}
	return difference;
}

// ******** OS_ClearMsTime ************
// sets the system time to zero (from Lab 1)
// Inputs:  none
// Outputs: none
// You are free to change how this works
void OS_ClearMsTime(void) 
{
	MSTime = 0;
}

// ******** OS_MsTime ************
// reads the current time in msec (from Lab 1)
// Inputs:  none
// Outputs: time in ms units
// You are free to select the time resolution for this function
// It is ok to make the resolution to match the first call to OS_AddPeriodicThread
uint32_t OS_MsTime(void) 
{
  //uint32_t retVal = (OS_Time()-MSTime)/TIME_1MS;	
	return MSTime;
}


//******** OS_Launch *************** 
// start the scheduler, enable interrupts
// Inputs: number of 12.5ns clock cycles for each time slice
//         you may select the units of this parameter
// Outputs: none (does not return)
// In Lab 2, you can ignore the theTimeSlice field
// In Lab 3, you should implement the user-defined TimeSlice field
// It is ok to limit the range of theTimeSlice to match the 24-bit SysTick
void OS_Launch(uint32_t theTimeSlice) 
{
  NVIC_ST_RELOAD_R = theTimeSlice - 1; // reload value
  NVIC_ST_CTRL_R = 0x00000007; // enable, core clock and interrupt arm
  
  StartOS();                   // start on the first task
}
