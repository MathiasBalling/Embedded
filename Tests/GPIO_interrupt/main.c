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

void setLEDColor(enum LEDColor color) {
  GPIO_PORTF_DATA_R = (GPIO_PORTF_DATA_R & 0b11110001) | (color << 1);
}

void setupPortF() {
  // Enable the GPIO port that is used for the on-board LEDs and switches.
  SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOF;
  // Set the direction as output (PF1 - PF3).
  GPIO_PORTF_DIR_R = 0b00001110;
  // Enable the GPIO pins for digital function (PF1 - PF4)
  GPIO_PORTF_DEN_R = 0b00011110;
  // Enable internal pull-up (PF4).
  GPIO_PORTF_PUR_R = 0b00010000;

  // Set the interrupt type for GPIOF PF4 (SW1)
  GPIO_PORTF_IS_R &= ~(0b00010000);  // PF4 is edge-sensitive
  GPIO_PORTF_IBE_R &= ~(0b00010000); // PF4 not both edges
  GPIO_PORTF_IEV_R = 0b00010000;     // PF4 rising edge
  GPIO_PORTF_IM_R = 0b00010000;      // Interrupt on PF4
  GPIO_PORTF_ICR_R = 0b00010000;     // clear flag4

  // Enable interrupt for GPIO Port F (INT30)
  NVIC_EN0_R |= (1 << (INT_GPIOF - 16)); // Enable interrupt 30 in NVIC
  // Set priority to 5 (0-7)
  NVIC_PRI7_R = (NVIC_PRI7_R & 0xFF00FFFF) | (0b101 << 21);
}

volatile int count = 1; // Volatile to prevent optimization

// Interrupt handler for GPIO Port F
void PortF_Handler(void) {
  count = (count + 1) % 8;
  GPIO_PORTF_ICR_R = 0b00010000; // clear interrupt flag for PF4
} // GPIO Port F

int main(void) {
  setupPortF();

  // Loop forever.
  while (1) {
    setLEDColor(colors[count]);
  }
  return (0);
}
