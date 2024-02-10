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

void setLEDColor(enum LEDColor color) { GPIO_PORTF_DATA_R = color << 1; }

int main(void) {
  // Enable the GPIO port that is used for the on-board LEDs and switches.
  SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOF;

  // Unlock the GPIO commit control register.
  GPIO_PORTF_LOCK_R = 0x4C4F434B;

  // Allow changes to PF0
  GPIO_PORTF_CR_R |= 0x01;

  // Set the direction as output (PF1 - PF3).
  GPIO_PORTF_DIR_R = 0b00001110;

  // Enable the GPIO pins for digital function (PF0 - PF4)
  GPIO_PORTF_DEN_R = 0b00011111;

  // Enable internal pull-up (PF4).
  GPIO_PORTF_PUR_R = 0b00010001;

  // Loop forever.
  while (1) {
    if ((GPIO_PORTF_DATA_R & 0x10) == 0) // SW1 is pressed
      setLEDColor(GREEN);
    else if ((GPIO_PORTF_DATA_R & 0x01) == 0) // SW2 is pressed
      setLEDColor(BLUE);
    else // Neither switch is pressed
      setLEDColor(RED);
  }

  return (0);
}
