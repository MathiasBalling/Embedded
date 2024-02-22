#ifndef _GPIO_H
#define _GPIO_H
#include "tm4c123gh6pm.h"
void setup_GPIOF() {
  // Enable the GPIO port that is used for the on-board LEDs and switches.
  SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOF;
  // Set the direction as output (PF1 - PF3).
  GPIO_PORTF_DIR_R = 0b00001110;
  // Enable the GPIO pins for digital function (PF1 - PF4)
  GPIO_PORTF_DEN_R = 0b00011110;
  // Enable internal pull-up (PF4).
  GPIO_PORTF_PUR_R = (1 << 4);

  // Set the interrupt type for PF4 (SW1)
  NVIC_EN0_R |= (1 << (INT_GPIOF - 16)); // enable interrupt in NVIC
  NVIC_PRI7_R = (NVIC_PRI7_R & 0xFF00FFFF) | 0x00A00000; // priority 5
  GPIO_PORTF_IS_R |= (1 << 4); // PF4 is edge-sensitive
  // GPIO_PORTF_IBE_R |= (1 << 4); // PF4 is both edges
  GPIO_PORTF_IEV_R &= ~(1 << 4); // PF4 falling edge event
  GPIO_PORTF_IM_R = (1 << 4);    // Interrupt on PF4
  GPIO_PORTF_ICR_R = (1 << 4);   // clear flag4
}

int button_sw1_pressed() { return ~(GPIO_PORTF_DATA_R) & 0b00010000; }

#endif // !DEBUG
