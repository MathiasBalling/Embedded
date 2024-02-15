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

void setup() {
  // Enable the clock
  // Enable the UART0 clock
  SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R0;
  // Enable the clock for GPIOF for LED and button
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;
  // Enable the clock for GPIOA for UART RX and TX
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R0;

  // GPIOF
  // Set the direction as output (PF1 - PF3).
  GPIO_PORTF_DIR_R |= 0b00001110;
  // Enable the GPIO pins for digital function (PF1 - PF4)
  GPIO_PORTF_DEN_R |= 0b00011110;
  // Enable internal pull-up (PF4).
  GPIO_PORTF_PUR_R |= 0b00010000;

  // GPIOA with alternate function
  // Set the alternate function for PA0 and PA1
  GPIO_PORTA_AFSEL_R |= 0b00000011;
  // Enable the alternate function for PA0 and PA1
  GPIO_PORTA_PCTL_R |= 0x11;
  // Enable the GPIO pins for digital function (PA0 and PA1)
  GPIO_PORTA_DEN_R |= 0b00000011;

  // UART0
  // Disable UART0
  UART0_CTL_R &= ~(UART_CTL_UARTEN);
  // Set the baud rate
  UART0_IBRD_R = 104; // IBRD = int(16,000,000 / (16 * 9600)) = int(104.1667)

  // Set the fractional part of the baud rate
  UART0_FBRD_R = 11; // FBRD = 0.1667 * 64 + 0.5 = int(11,16688)

  // Set the transmission parameters
  UART0_LCRH_R = 0x60; // 8-bit word length

  // Set the clock source
  UART0_CC_R = 0;
  // Enable the UART0
  UART0_CTL_R |=
      (UART_CTL_RXE | UART_CTL_TXE | UART_CTL_UARTEN); // Enable UART0
}

int main(void) {
  setup();

  // Loop forever.
  while (1) {
    if ((GPIO_PORTF_DATA_R & 0b00010000) == 0) {
      while (UART0_FR_R & (1 << 5))
        ;
      UART0_DR_R = 'H';
    }
    if (UART0_FR_R & (1 << 6)) {
      setLEDColor(BLUE);
    }
  }
  return (0);
}
