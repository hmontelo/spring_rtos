// gpio.c
// Runs on LM4F120/TM4C123

#include "../inc/gpio.h"
#include "../inc/hw/tm4c123gh6pm.h"

int16_t GPIO_ActivatePort(GPIO_enGPIOPort_t port)
{
  int16_t returnValue = (int16_t) 0x00;
  
  switch(port)
  {
    case GPIO_enGPIOPort_A:
    {
      SYSCTL_RCGCGPIO_R |= (uint32_t) 0x01;     // Activate Port A
      break;
    }
    case GPIO_enGPIOPort_B:
    {
      SYSCTL_RCGCGPIO_R |= (uint32_t) 0x02;     // Activate Port B
      break;
    }
    case GPIO_enGPIOPort_C:
    {
      SYSCTL_RCGCGPIO_R |= (uint32_t) 0x04;     // Activate Port C
      break;
    }
    case GPIO_enGPIOPort_D:
    {
      SYSCTL_RCGCGPIO_R |= (uint32_t) 0x08;     // Activate Port D
      break;
    }
    case GPIO_enGPIOPort_E:
    {
      SYSCTL_RCGCGPIO_R |= (uint32_t) 0x10;     // Activate Port E
      break;
    }
    case GPIO_enGPIOPort_F:
    {
      SYSCTL_RCGCGPIO_R |= (uint32_t) 0x20;     // Activate Port F
      break;
    }
    default:
    {
      returnValue = -1;
      break;
    }
  }
  return returnValue;
}

int16_t GPIO_UnlockPort(GPIO_enGPIOPort_t port)
{
  int16_t returnValue = (int16_t) 0x00;
  
  switch(port)
  {
    case GPIO_enGPIOPort_A:
    {
      GPIO_PORTA_LOCK_R = GPIO_LOCK_KEY;        // Unlock Port A
      break;
    }
    case GPIO_enGPIOPort_B:
    {
      GPIO_PORTB_LOCK_R = GPIO_LOCK_KEY;        // Unlock Port B
      break;
    }
    case GPIO_enGPIOPort_C:
    {
      GPIO_PORTC_LOCK_R = GPIO_LOCK_KEY;        // Unlock Port C
      break;
    }
    case GPIO_enGPIOPort_D:
    {
      GPIO_PORTD_LOCK_R = GPIO_LOCK_KEY;        // Unlock Port D
      break;
    }
    case GPIO_enGPIOPort_E:
    {
      GPIO_PORTE_LOCK_R = GPIO_LOCK_KEY;        // Unlock Port E
      break;
    }
    case GPIO_enGPIOPort_F:
    {
      GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;        // Unlock Port F    
      break;
    }
    default:
    {
      returnValue = -1;
      break;
    }
  }
  return returnValue;
}

int16_t GPIO_AllowChangesToPins(GPIO_enGPIOPort_t port, uint32_t port_pins_mask)
{
  int16_t returnValue = (int16_t) 0x00;
  
  switch(port)
  {
    case GPIO_enGPIOPort_A:
    {
      GPIO_PORTA_CR_R = (uint32_t) port_pins_mask; // Changes are allowed on specific pin of Port A
      break;
    }
    case GPIO_enGPIOPort_B:
    {
      GPIO_PORTB_CR_R = (uint32_t) port_pins_mask; // Changes are allowed on specific pin of Port B
      break;
    }
    case GPIO_enGPIOPort_C:
    {
      GPIO_PORTC_CR_R = (uint32_t) port_pins_mask; // Changes are allowed on specific pin of Port C
      break;
    }
    case GPIO_enGPIOPort_D:
    {
      GPIO_PORTD_CR_R = (uint32_t) port_pins_mask; // Changes are allowed on specific pin of Port D
      break;
    }
    case GPIO_enGPIOPort_E:
    {
      GPIO_PORTE_CR_R = (uint32_t) port_pins_mask; // Changes are allowed on specific pin of Port E
      break;
    }
    case GPIO_enGPIOPort_F:
    {
      GPIO_PORTF_CR_R = (uint32_t) port_pins_mask; // Changes are allowed on specific pin of Port F
                                                   // only PF0 needs to be unlocked, other bits can't be locked
      break;
    }
    default:
    {
      returnValue = -1;
      break;
    }
  }
  return returnValue;  
}

uint8_t GPIO_IsPortReady(GPIO_enGPIOPort_t port)
{
  uint8_t returnValue = (uint8_t) 0x00;
  
  switch(port)
  {
    case GPIO_enGPIOPort_A:
    {       
      returnValue = (SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R0)?1:0; // GPIO Port A Ready
      break;
    }
    case GPIO_enGPIOPort_B:
    {
      returnValue = (SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R1)?1:0; // GPIO Port B Ready
      break;
    }
    case GPIO_enGPIOPort_C:
    {
      returnValue = (SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R2)?1:0; // GPIO Port C Ready
      break;
    }
    case GPIO_enGPIOPort_D:
    {
      returnValue = (SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R3)?1:0; // GPIO Port D Ready
      break;
    }
    case GPIO_enGPIOPort_E:
    {
      returnValue = (SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R4)?1:0; // GPIO Port E Ready
      break;
    }
    case GPIO_enGPIOPort_F:
    {
      returnValue = (SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R5)?1:0; // GPIO Port F Ready    
      break;
    }
    default:
    {
      returnValue = (uint8_t) 0x00;
      break;
    }
  }
  return returnValue;
}

int16_t GPIO_AnalogMode(GPIO_enGPIOPort_t port, uint8_t port_pins_mask)
{
  int16_t returnValue = (int16_t) 0x00;
  
  switch(port)
  {
    case GPIO_enGPIOPort_A:
    {
      GPIO_PORTA_AMSEL_R = (uint32_t) port_pins_mask; // Enable analogic mode for the specific pins Port A
      break;
    }
    case GPIO_enGPIOPort_B:
    {
      GPIO_PORTB_AMSEL_R = (uint32_t) port_pins_mask; // Enable analogic mode for the specific pins Port B
      break;
    }
    case GPIO_enGPIOPort_C:
    {
      GPIO_PORTC_AMSEL_R = (uint32_t) port_pins_mask; // Enable analogic mode for the specific pins Port C
      break;
    }
    case GPIO_enGPIOPort_D:
    {
      GPIO_PORTD_AMSEL_R = (uint32_t) port_pins_mask; // Enable analogic mode for the specific pins Port D
      break;
    }
    case GPIO_enGPIOPort_E:
    {
      GPIO_PORTE_AMSEL_R = (uint32_t) port_pins_mask; // Enable analogic mode for the specific pins Port E
      break;
    }
    case GPIO_enGPIOPort_F:
    {
      GPIO_PORTF_AMSEL_R = (uint32_t) port_pins_mask; // Enable analogic mode for the specific pins Port F
      break;
    }
    default:
    {
      returnValue = -1;
      break;
    }
  }
  return returnValue;  
}

int16_t GPIO_SetPortControl(GPIO_enGPIOPort_t port, uint32_t port_pmc_bit_mask)
{
  int16_t returnValue = (int16_t) 0x00;
  
  switch(port)
  {
    case GPIO_enGPIOPort_A:
    {
      GPIO_PORTA_PCTL_R = (uint32_t) port_pmc_bit_mask; //  Set the PMCx bits associated to the port using specific bit mask.
      break;
    }
    case GPIO_enGPIOPort_B:
    {
      GPIO_PORTB_PCTL_R = (uint32_t) port_pmc_bit_mask; //  Set the PMCx bits associated to the port using specific bit mask.
      break;
    }
    case GPIO_enGPIOPort_C:
    {
      GPIO_PORTC_PCTL_R = (uint32_t) port_pmc_bit_mask; //  Set the PMCx bits associated to the port using specific bit mask.
      break;
    }
    case GPIO_enGPIOPort_D:
    {
      GPIO_PORTD_PCTL_R = (uint32_t) port_pmc_bit_mask; //  Set the PMCx bits associated to the port using specific bit mask.
      break;
    }
    case GPIO_enGPIOPort_E:
    {
      GPIO_PORTE_PCTL_R = (uint32_t) port_pmc_bit_mask; //  Set the PMCx bits associated to the port using specific bit mask.
      break;
    }
    case GPIO_enGPIOPort_F:
    {
      GPIO_PORTF_PCTL_R = (uint32_t) port_pmc_bit_mask; //  Set the PMCx bits associated to the port using specific bit mask.
      break;
    }
    default:
    {
      returnValue = -1;
      break;
    }
  }
  return returnValue;  
}

int16_t GPIO_SetOutputDirection(GPIO_enGPIOPort_t port, uint8_t port_pins_mask)
{
  int16_t returnValue = (int16_t) 0x00;
  
  switch(port)
  {
    case GPIO_enGPIOPort_A:
    {
      GPIO_PORTA_DIR_R = (uint32_t) port_pins_mask; // Set specific pins to output in port A according with bit mask.
      break;
    }
    case GPIO_enGPIOPort_B:
    {
      GPIO_PORTB_DIR_R = (uint32_t) port_pins_mask; // Set specific pins to output in port B according with bit mask.
      break;
    }
    case GPIO_enGPIOPort_C:
    {
      GPIO_PORTC_DIR_R = (uint32_t) port_pins_mask; // Set specific pins to output in port C according with bit mask.
      break;
    }
    case GPIO_enGPIOPort_D:
    {
      GPIO_PORTD_DIR_R = (uint32_t) port_pins_mask; // Set specific pins to output in port D according with bit mask.
      break;
    }
    case GPIO_enGPIOPort_E:
    {
      GPIO_PORTE_DIR_R = (uint32_t) port_pins_mask; // Set specific pins to output in port E according with bit mask.
      break;
    }
    case GPIO_enGPIOPort_F:
    {
      GPIO_PORTF_DIR_R = (uint32_t) port_pins_mask; // Set specific pins to output in port F according with bit mask.
      break;
    }
    default:
    {
      returnValue = -1;
      break;
    }
  }
  return returnValue;  
}

int16_t GPIO_SetinputDirection(GPIO_enGPIOPort_t port, uint8_t port_pins_mask)
{
  int16_t returnValue = (int16_t) 0x00;
  
  switch(port)
  {
    case GPIO_enGPIOPort_A:
    {
      GPIO_PORTA_DIR_R = (uint32_t) ~port_pins_mask; // Set specific pins to input in port A according with bit mask.
      break;
    }
    case GPIO_enGPIOPort_B:
    {
      GPIO_PORTB_DIR_R = (uint32_t) ~port_pins_mask; // Set specific pins to input in port B according with bit mask.
      break;
    }
    case GPIO_enGPIOPort_C:
    {
      GPIO_PORTC_DIR_R = (uint32_t) ~port_pins_mask; // Set specific pins to input in port C according with bit mask.
      break;
    }
    case GPIO_enGPIOPort_D:
    {
      GPIO_PORTD_DIR_R = (uint32_t) ~port_pins_mask; // Set specific pins to input in port D according with bit mask.
      break;
    }
    case GPIO_enGPIOPort_E:
    {
      GPIO_PORTE_DIR_R = (uint32_t) ~port_pins_mask; // Set specific pins to input in port E according with bit mask.
      break;
    }
    case GPIO_enGPIOPort_F:
    {
      GPIO_PORTF_DIR_R = (uint32_t) ~port_pins_mask; // Set specific pins to input in port F according with bit mask.
      break;
    }
    default:
    {
      returnValue = -1;
      break;
    }
  }
  return returnValue;  
}

int16_t GPIO_AlternateFunctions(GPIO_enGPIOPort_t port, uint8_t port_pins_mask)
{
  int16_t returnValue = (int16_t) 0x00;
  
  switch(port)
  {
    case GPIO_enGPIOPort_A:
    {
      GPIO_PORTA_AFSEL_R = (uint32_t) port_pins_mask; // Enable alternate functions for pins of Port A
      break;
    }
    case GPIO_enGPIOPort_B:
    {
      GPIO_PORTB_AFSEL_R = (uint32_t) port_pins_mask; // Enable alternate functions for pins of Port B
      break;
    }
    case GPIO_enGPIOPort_C:
    {
      GPIO_PORTC_AFSEL_R = (uint32_t) port_pins_mask; // Enable alternate functions for pins of Port C
      break;
    }
    case GPIO_enGPIOPort_D:
    {
      GPIO_PORTD_AFSEL_R = (uint32_t) port_pins_mask; // Enable alternate functions for pins of Port D
      break;
    }
    case GPIO_enGPIOPort_E:
    {
      GPIO_PORTE_AFSEL_R = (uint32_t) port_pins_mask; // Enable alternate functions for pins of Port E
      break;
    }
    case GPIO_enGPIOPort_F:
    {
      GPIO_PORTF_AFSEL_R = (uint32_t) port_pins_mask; // Enable alternate functions for pins of Port F
      break;
    }
    default:
    {
      returnValue = -1;
      break;
    }
  }
  return returnValue;  
}

int16_t GPIO_DigitalFunctions(GPIO_enGPIOPort_t port, uint8_t port_pins_mask)
{
  int16_t returnValue = (int16_t) 0x00;
  
  switch(port)
  {
    case GPIO_enGPIOPort_A:
    {
      GPIO_PORTA_DEN_R = (uint32_t) port_pins_mask; // Enable alternate functions for pins of Port A
      break;
    }
    case GPIO_enGPIOPort_B:
    {
      GPIO_PORTB_DEN_R = (uint32_t) port_pins_mask; // Enable alternate functions for pins of Port B
      break;
    }
    case GPIO_enGPIOPort_C:
    {
      GPIO_PORTC_DEN_R = (uint32_t) port_pins_mask; // Enable alternate functions for pins of Port C
      break;
    }
    case GPIO_enGPIOPort_D:
    {
      GPIO_PORTD_DEN_R = (uint32_t) port_pins_mask; // Enable alternate functions for pins of Port D
      break;
    }
    case GPIO_enGPIOPort_E:
    {
      GPIO_PORTE_DEN_R = (uint32_t) port_pins_mask; // Enable alternate functions for pins of Port E
      break;
    }
    case GPIO_enGPIOPort_F:
    {
      GPIO_PORTF_DEN_R = (uint32_t) port_pins_mask; // Enable alternate functions for pins of Port F
      break;
    }
    default:
    {
      returnValue = -1;
      break;
    }
  }
  return returnValue;  
}

int16_t GPIO_PullUpResistors(GPIO_enGPIOPort_t port, uint8_t port_pins_mask)
{
  int16_t returnValue = (int16_t) 0x00;
  
  switch(port)
  {
    case GPIO_enGPIOPort_A:
    {
      GPIO_PORTA_PUR_R = (uint32_t) port_pins_mask; // Enable weak pull-upon specific pin of Port A
      break;
    }
    case GPIO_enGPIOPort_B:
    {
      GPIO_PORTB_PUR_R = (uint32_t) port_pins_mask; // Enable weak pull-upon specific pin of Port B
      break;
    }
    case GPIO_enGPIOPort_C:
    {
      GPIO_PORTC_PUR_R = (uint32_t) port_pins_mask; // Enable weak pull-upon specific pin of Port C
      break;
    }
    case GPIO_enGPIOPort_D:
    {
      GPIO_PORTD_PUR_R = (uint32_t) port_pins_mask; // Enable weak pull-upon specific pin of Port D
      break;
    }
    case GPIO_enGPIOPort_E:
    {
      GPIO_PORTE_PUR_R = (uint32_t) port_pins_mask; // Enable weak pull-upon specific pin of Port E
      break;
    }
    case GPIO_enGPIOPort_F:
    {
      GPIO_PORTF_PUR_R = (uint32_t) port_pins_mask; // Enable weak pull-upon specific pin of Port F
      break;
    }
    default:
    {
      returnValue = -1;
      break;
    }
  }
  return returnValue;  
}

//------------GPIO_WritePort------------
// Write data on specific GPIO port
// Input: 
//         port: Ports between A-F
//         data: A 32 bit data (each port has it own limits)
// Output: 
//         0: Sucess, -1: Fail
//
int16_t GPIO_WritePort(GPIO_enGPIOPort_t port, uint32_t data)
{
  int16_t returnValue = (int16_t) 0x00;
  
  switch(port)
  {
    case GPIO_enGPIOPort_A:
    {
      GPIO_PORTA_DATA_R = data; // Write 'data' on Port A
      break;
    }
    case GPIO_enGPIOPort_B:
    {
      GPIO_PORTB_DATA_R = data; // Write 'data' on Port B
      break;
    }
    case GPIO_enGPIOPort_C:
    {
      GPIO_PORTC_DATA_R = data; // Write 'data' on Port C
      break;
    }
    case GPIO_enGPIOPort_D:
    {
      GPIO_PORTD_DATA_R = data; // Write 'data' on Port D
      break;
    }
    case GPIO_enGPIOPort_E:
    {
      GPIO_PORTE_DATA_R = data; // Write 'data' on Port E
      break;
    }
    case GPIO_enGPIOPort_F:
    {
      GPIO_PORTF_DATA_R = data; // Write 'data' on Port F
      break;
    }
    default:
    {
      returnValue = -1;
      break;
    }
  }
  return returnValue;  
}

//------------GPIO_ReadPort------------
// Write data on specific GPIO port
// Input: 
//         port: Ports between A-F
//         data: A 32 bit data to store the values reading from the port
// Output: 
//         0: Sucess, -1: Fail
//        
int16_t GPIO_ReadPort(GPIO_enGPIOPort_t port, uint32_t *data)
{
  int16_t returnValue = (int16_t) 0x00;
  
  if(data != 0x00)
  { 
    switch(port)
    {
      case GPIO_enGPIOPort_A:
      {
        *data = GPIO_PORTA_DATA_R; // Read 'data' on Port A
        break;
      }
      case GPIO_enGPIOPort_B:
      {
        *data = GPIO_PORTB_DATA_R; // Read 'data' on Port B
        break;
      }
      case GPIO_enGPIOPort_C:
      {
        *data = GPIO_PORTC_DATA_R; // Read 'data' on Port C
        break;
      }
      case GPIO_enGPIOPort_D:
      {
        *data = GPIO_PORTD_DATA_R; // Read 'data' on Port D
        break;
      }
      case GPIO_enGPIOPort_E:
      {
        *data = GPIO_PORTE_DATA_R; // Read 'data' on Port E
        break;
      }
      case GPIO_enGPIOPort_F:
      {
        *data = GPIO_PORTF_DATA_R; // Read 'data' on Port F
        break;
      }
      default:
      {
        returnValue = -1;
        break;
      }
    }
  }
  return returnValue;  
}
