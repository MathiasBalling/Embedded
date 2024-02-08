#include "tm4c123gh6pm.h"
#include <stdbool.h>
#include <stdint.h>
#define SYSTICK_RELOAD_VALUE 16000 // 1 mS

// Missing definitions in tm4c123gh6pm.h file
#define NVIC_INT_CTRL_PEND_SYST 0x04000000   // Pend a systick int
#define NVIC_INT_CTRL_UNPEND_SYST 0x02000000 // Unpend a systick int

#define SYSTICK_PRIORITY 0x7E

volatile int ticks = 0;

void SysTick_Handler(void) {
  // Hardware clears systick int reguest
  ticks++;
}

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

int counter = 0;
bool increment = true;
bool autoMode = false;

int modulo(int x, int N) { return (x % N + N) % N; }

void ment() {
  if (increment) {
    counter++;
  } else {
    counter--;
  }
  counter = modulo(counter, 8);
}

void setupPortF() {
  int dummy;

  // Enable the GPIO port that is used for the on-board LEDs and switches.
  SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOF;

  // Do a dummy read to insert a few cycles after enabling the peripheral.
  dummy = SYSCTL_RCGC2_R;

  // Set the direction as output (PF1 - PF3).
  GPIO_PORTF_DIR_R = 0x0E;

  // Enable the GPIO pins for digital function (PF1 - PF4)
  GPIO_PORTF_DEN_R = 0x1E;

  // Enable internal pull-up (PF4).
  GPIO_PORTF_PUR_R = 0x10;
}

void init_systick() {

  // Disable systick timer
  NVIC_ST_CTRL_R &= ~(NVIC_ST_CTRL_ENABLE);

  // Set current systick counter to reload value
  NVIC_ST_CURRENT_R = SYSTICK_RELOAD_VALUE;
  // Set Reload value, Systick reload register
  NVIC_ST_RELOAD_R = SYSTICK_RELOAD_VALUE;

  // NVIC systick setup, vector number 15
  // Clear pending systick interrupt request
  NVIC_INT_CTRL_R |= NVIC_INT_CTRL_UNPEND_SYST;

  // Set systick priority to 0x10, first clear then set.
  NVIC_SYS_PRI3_R &= ~(NVIC_SYS_PRI3_TICK_M);
  NVIC_SYS_PRI3_R |=
      (NVIC_SYS_PRI3_TICK_M & (SYSTICK_PRIORITY << NVIC_SYS_PRI3_TICK_S));

  // Select systick clock source, Use core clock
  NVIC_ST_CTRL_R |= NVIC_ST_CTRL_CLK_SRC;

  // Enable systick interrupt
  NVIC_ST_CTRL_R |= NVIC_ST_CTRL_INTEN;

  // Enable and start timer
  NVIC_ST_CTRL_R |= NVIC_ST_CTRL_ENABLE;
}

void setLEDColor(enum LEDColor color) { GPIO_PORTF_DATA_R = color << 1; }

int main(void) {
  setupPortF();
  init_systick();

  // Loop forever.
  while (1) {
    if (~(GPIO_PORTF_DATA_R) & 0x10) {
      ticks = 0;
      while (~(GPIO_PORTF_DATA_R) & 0b00010000)
        ;                 // Wait for button release
      int temp = ticks;   // Store the current value of ticks
      if (ticks > 2000) { // If the button is pressed for more than 2 seconds
        autoMode = !autoMode;
      }
      // if (increment) {
      //   increment = false;
      // } else {
      //   increment = true;
      // }
    } else if (ticks > 5) { // If the button is pressed for more than 5ms to
                            // remove debounce
      if (autoMode) {
        autoMode = false; // Disable auto mode
      }
      ment(); // Increment or decrement counter
    } else if (autoMode) {
      ticks = 0;
      while (ticks < 200)
        ;     // Wait for 200ms
      ment(); // Increment or decrement counter
    }
    setLEDColor(colors[counter]); // Set the LED color
  }
  return (0);
}
