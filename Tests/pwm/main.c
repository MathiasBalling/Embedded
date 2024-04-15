#include "tm4c123gh6pm.h"
#include <stdint.h>
// PWM on the RGB led
int main(void) {
  // Enable the PWM module 1
  SYSCTL_RCGCPWM_R |= SYSCTL_RCGCPWM_R1;
  // Enable the GPIO port that is used for the on-board LEDs and switches.
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;
  // Set the system clock as the clock source for the PWM module
  SYSCTL_RCC_R &= ~(SYSCTL_RCC_USEPWMDIV);

  while (!(SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R5)) {
  }
  // Setup the pins for the PWM module
  // Enable the alternate function for the PWM pins
  GPIO_PORTF_AFSEL_R |= 0b00000100;
  GPIO_PORTF_AMSEL_R &= ~0b00000100;
  // Set the alternate function for the PWM pins
  GPIO_PORTF_PCTL_R &= ~0x00000F00;
  GPIO_PORTF_PCTL_R |= 0x00000500;
  // Enable the digital function for the PWM pins
  GPIO_PORTF_DEN_R |= 0b00000100;

  // PWM configuration for 10 kHz frequency
  // Load value for the PWM generator
  // 16.000.000 / 16.000 = 1000
  // For PF2 (A) and PF3 (B)
  // Disable the PWM generator (gen 3)
  PWM1_3_CTL_R &= ~PWM_3_CTL_ENABLE;
  // Select the down-count mode
  PWM1_3_CTL_R &= ~PWM_3_CTL_MODE;
  PWM1_3_GENA_R |= 0x0000008C; // Set the action to drive the output high
  // Set the PWM load value
  PWM1_3_LOAD_R = 1000;
  // Set the PWM compare value
  PWM1_3_CMPA_R = 500; // Here 50% duty cycle for PF2 (change in program to
                       // control the duty cycle)
  // Enable the PWM generator (gen 3)
  PWM1_3_CTL_R |= PWM_3_CTL_ENABLE;
  // Enable the PWM output
  PWM1_ENABLE_R |= PWM_ENABLE_PWM6EN;

  // Loop forever.
  while (1) {
    // Increase the duty cycle of the RGB led
    for (int i = 0; i < 1000; i++) {
      // PWM1_2_CMPB_R = i; // PF1
      PWM1_3_CMPA_R = i; // PF2
      // PWM1_3_CMPB_R = i; // PF3
      for (int j = 0; j < 1000; j++){}
    }
    // Decrease the duty cycle of the RGB led
    for (int i = 1000; i > 0; i--) {
      // PWM1_2_CMPB_R = i; // PF1
      PWM1_3_CMPA_R = i; // PF2
      // PWM1_3_CMPB_R = i; // PF3
      for (int j = 0; j < 1000; j++){}
    }
  }

  return (0);
}
