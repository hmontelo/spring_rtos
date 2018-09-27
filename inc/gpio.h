// gpio.h
// Runs on LM4F120/TM4C123
// Provide functions that initialize and handle the GPIO.
// Daniel and Jonathan Valvano
// September 12, 2013

/* This example accompanies the book
   "Embedded Systems: Introduction to ARM Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2013
   Section 4.2    Program 4.2
  "Embedded Systems: Real Time Interfacing to ARM Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2013
   Example 2.3, Program 2.9, Figure 2.36
 Copyright 2013 by Jonathan W. Valvano, valvano@mail.utexas.edu
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

// negative logic switches connected to PF0 and PF4 on the Launchpad
// red LED connected to PF1 on the Launchpad
// blue LED connected to PF2 on the Launchpad
// green LED connected to PF3 on the Launchpad
// NOTE: The NMI (non-maskable interrupt) is on PF0.  That means that
// the Alternate Function Select, Pull-Up Resistor, Pull-Down Resistor,
// and Digital Enable are all locked for PF0 until a value of 0x4C4F434B
// is written to the Port F GPIO Lock Register.  After Port F is
// unlocked, bit 0 of the Port F GPIO Commit Register must be set to
// allow access to PF0's control registers.  On the LM4F120, the other
// bits of the Port F GPIO Commit Register are hard-wired to 1, meaning
// that the rest of Port F can always be freely re-configured at any
// time.  Requiring this procedure makes it unlikely to accidentally
// re-configure the JTAG and NMI pins as GPIO, which can lock the
// debugger out of the processor and make it permanently unable to be
// debugged or re-programmed.

#include <stdint.h>

#ifndef __GPIO_H__
#define __GPIO_H__

// enumeration that contains available gpio ports
typedef enum{
  GPIO_enGPIOPort_A = 0x00,
  GPIO_enGPIOPort_B = 0x01,
  GPIO_enGPIOPort_C = 0x02,
  GPIO_enGPIOPort_D = 0x03,
  GPIO_enGPIOPort_E = 0x04,
  GPIO_enGPIOPort_F = 0x05
} GPIO_enGPIOPort_t;

typedef enum{
  GPIO_enPortControl_GPIO      = 0x00,
  GPIO_enPortControl_Alternate = 0x01
}GPIO_enPortControl_t;

typedef enum{
  GPIO_enPortDirectionInput  = 0x00,
  GPIO_enPortDirectionOutput = 0x01
} GPIO_enPortDirection_t;


//------------GPIO_ActivatePort------------
//
// Input: 
//
// Output: 
//
//
int16_t GPIO_ActivatePort(GPIO_enGPIOPort_t port);

//------------GPIO_UnlockPort------------
//
// Input: 
//
// Output: 
//
//
int16_t GPIO_UnlockPort(GPIO_enGPIOPort_t port);

//------------GPIO_AllowChangesToPins------------
//
// Input: 
//
// Output: 
//
//
int16_t GPIO_AllowChangesToPins(GPIO_enGPIOPort_t port, uint32_t port_pins_mask);

//------------GPIO_IsPortReady------------
//
// Input: 
//
// Output: 
//
//
uint8_t GPIO_IsPortReady(GPIO_enGPIOPort_t port);

//------------GPIO_AnalogMode------------
//
// Input: 
//
// Output: 
//
//
int16_t GPIO_AnalogMode(GPIO_enGPIOPort_t port, uint8_t port_pins_mask);

//------------GPIO_SetPortControl------------
//
// Input: 
//
// Output: 
//
//
int16_t GPIO_SetPortControl(GPIO_enGPIOPort_t port, uint32_t port_pmc_bit_mask);

//------------GPIO_SetOutputDirection------------
//
// Input: 
//
// Output: 
//
//
int16_t GPIO_SetOutputDirection(GPIO_enGPIOPort_t port, uint8_t port_pins_mask);

//------------GPIO_SetinputDirection------------
//
// Input: 
//
// Output: 
//
//
int16_t GPIO_SetinputDirection(GPIO_enGPIOPort_t port, uint8_t port_pins_mask);

//------------GPIO_AlternateFunctions------------
//
// Input: 
//
// Output: 
//
//
int16_t GPIO_AlternateFunctions(GPIO_enGPIOPort_t port, uint8_t port_pins_mask);

//------------GPIO_DigitalFunctions------------
//
// Input: 
//
// Output: 
//
//
int16_t GPIO_DigitalFunctions(GPIO_enGPIOPort_t port, uint8_t port_pins_mask);

//------------GPIO_PullUpResistors------------
//
// Input: 
//
// Output: 
//
//
int16_t GPIO_PullUpResistors(GPIO_enGPIOPort_t port, uint8_t port_pins_mask);

//------------GPIO_WritePort------------
// Write data on specific GPIO port
// Input: 
//         port: Ports between A-F
//         data: A 32 bit data (each port has it own limits)
// Output: 
//         0: Sucess, -1: Fail
//
int16_t GPIO_WritePort(GPIO_enGPIOPort_t port, uint32_t data);

//------------GPIO_ReadPort------------
// Write data on specific GPIO port
// Input: 
//         port: Ports between A-F
//         data: A 32 bit data to store the values reading from the port
// Output: 
//         0: Sucess, -1: Fail
//   
int16_t GPIO_ReadPort(GPIO_enGPIOPort_t port, uint32_t *data);

#endif
