#include "tm4c123gh6pm.h"
#include <stdint.h>

enum LEDColor {
  OFF = 0b000,
  GREEN = 0b100,
  BLUE = 0b010,
  CYAN = 0b110,
  RED = 0b001,
  YELLOW = 0b101,
  MAGENTA = 0b011,
  WHITE = 0b111
};

enum LEDColor colors[] = {OFF, GREEN, BLUE, CYAN, RED, YELLOW, MAGENTA, WHITE};

int8_t counter = 3;

void setupPortF() {
  // Enable the GPIO port that is used for the on-board LEDs and switches.
  SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOF;
  // Set the direction as output (PF1 - PF3).
  GPIO_PORTF_DIR_R = 0b00001110;
  // Enable the GPIO pins for digital function (PF1 - PF4)
  GPIO_PORTF_DEN_R = 0b00011110;
  // Enable internal pull-up (PF4).
  GPIO_PORTF_PUR_R = 0b00010000;

  // Set the interrupt type for PF4 (SW1)
  NVIC_EN1_R |= (1 << 14);        // enable interrupt in NVIC
  NVIC_PEND1_R |= (1 << 14);      // enable interrupt in NVIC
  GPIO_PORTF_IS_R |= 0b00010000;  // PF4 is edge-sensitive
  GPIO_PORTF_IBE_R |= 0b00010000; // PF4 is both edges
  GPIO_PORTF_IEV_R |= 0b00010000; // PF4 falling edge event
  GPIO_PORTF_IM_R = 0b00010000;   // Interrupt on PF4
  GPIO_PORTF_ICR_R = 0b00010000;  // clear flag4
}

void setupSysTick() {}

void PortF_Handler(void) {
  counter++;
  GPIO_PORTF_ICR_R = 0b00010000; // clear flag4
} // GPIO Port F

void setLEDColor(enum LEDColor color) { GPIO_PORTF_DATA_R = color << 1; }

int main(void) {
  setupPortF();

  // Loop forever.

  while (1) {
    counter %= 8;
    while (counter < 0) {
      counter += 8;
    }
    setLEDColor(colors[counter]);
  }

  return (0);
}
