/**************************Includes**********************************/
#include "tm4c123gh6pm.h"
#include <stdbool.h>
#include <stdint.h>

/**************************Defines**********************************/
#define SYSTICK_RELOAD_VALUE 16000 // SysTick interrupt every 1ms

#define DOUBLE_CLICK_TIMEOUT 100 // Timeout for double click
#define LONG_PRESS_TIMEOUT 2000  // Timeout for long press
#define DEBOUNCE_TIMEOUT 10      // Debounce timeout
#define AUTO_MODE_INTERVAL 200   // Auto mode interval

// LED colors
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

// Modes of the program
enum Mode {
  AUTO,
  MANUAL,
};

// Direction of the increment
enum Direction {
  UP = true,
  DOWN = false,
};

// Define the order of led colors
enum LEDColor colors[] = {OFF, GREEN, BLUE, CYAN, RED, YELLOW, MAGENTA, WHITE};

/**************************Variables**********************************/
// Counter values. This is used as an intex into `colors`
volatile int counter = 0;

// Initial contitions
enum Mode mode = MANUAL;
enum Direction direction = UP;

// The ticks counter is used for timing and updated by the `systick.h` module
volatile uint32_t ticks;

// The elapsed_ticks counter is used for timing
volatile uint32_t last_press_tick = 0;
volatile uint32_t last_release_tick = 0;

/**************************Functions**********************************/
// Check if the button is currently pressed. Returns 0 if not.
int button_sw1_pressed() { return ~(GPIO_PORTF_DATA_R) & 0b00010000; }

// Get the modulo of a number (even negative ones)
int mod(int x, int N) { return (x % N + N) % N; }

// Increment or decrement the counter
void ment() {
  if (direction)
    counter++;
  else
    counter--;
  counter = mod(counter, 8);
}

// Set the color of the LED
void setLEDColor(enum LEDColor color) { GPIO_PORTF_DATA_R = color << 1; }

void init_systick() {

  // Disable systick timer
  NVIC_ST_CTRL_R &= ~(NVIC_ST_CTRL_ENABLE);

  // Set current systick counter to reload value
  NVIC_ST_CURRENT_R = SYSTICK_RELOAD_VALUE;
  // Set Reload value, Systick reload register
  NVIC_ST_RELOAD_R = SYSTICK_RELOAD_VALUE;

  // Set systick priority to 0x10, first clear then set.
  /* NVIC_SYS_PRI3_R &= ~(NVIC_SYS_PRI3_TICK_M); */
  /* NVIC_SYS_PRI3_R |= */
  /*     (NVIC_SYS_PRI3_TICK_M & (SYSTICK_PRIORITY << NVIC_SYS_PRI3_TICK_S)); */

  // Select systick clock source, Use core clock
  NVIC_ST_CTRL_R |= NVIC_ST_CTRL_CLK_SRC;

  // Enable systick interrupt
  NVIC_ST_CTRL_R |= NVIC_ST_CTRL_INTEN;

  // Enable and start timer
  NVIC_ST_CTRL_R |= NVIC_ST_CTRL_ENABLE;
}

void setup() {
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

  init_systick();

  GPIO_PORTF_IS_R &= ~(1 << 4); // PF4 is edge-sensitive
  GPIO_PORTF_IBE_R |= (1 << 4); // PF4 is both edges
  GPIO_PORTF_IM_R = (1 << 4);   // Interrupt on PF4
  GPIO_PORTF_ICR_R = (1 << 4);  // clear flag4

  // Enable GPIOF interupt
  NVIC_EN0_R |= (1 << 30);

  // Set priority to 5 (lower priority than systick)
  NVIC_PRI7_R = (NVIC_PRI7_R & 0xFF00FFFF) | 0x00A00000;
}

void process_press() {
  uint32_t press_duration = last_release_tick - last_press_tick;
  if (press_duration < DEBOUNCE_TIMEOUT) {
    // Filter out debouncing
  } else if (press_duration >= LONG_PRESS_TIMEOUT) {
    // Long press
    mode = AUTO;
  } else {
    // Short press

    // Temporary disable the interrupt on PF4
    GPIO_PORTF_IM_R = (0 << 4); // Disable interrupt on PF4

    // Turn off the auto mode
    mode = MANUAL;

    // Reset the counter
    ticks = 0;
    bool double_click = false;

    // Wait for a potential double click
    while (ticks < DOUBLE_CLICK_TIMEOUT) {
      if (button_sw1_pressed() && ticks > DEBOUNCE_TIMEOUT) {
        double_click = true;
        while (button_sw1_pressed()) {
          // Wait for the button to be released
        }
        break;
      }
    }
    // Handle the press type
    if (double_click) {
      direction = direction == UP ? DOWN : UP;
    } else {
      ment();
    }

    // Re-enable the interrupt on PF4
    GPIO_PORTF_ICR_R |= 0x10;   // Clear interrupt bit
    GPIO_PORTF_IM_R = (1 << 4); // Interrupt on PF4
  }
}

void PortF_Handler(void) {
  // Check if the interrupt is for rising edge (Press)
  if (button_sw1_pressed()) {
    last_press_tick = ticks;
  } else { // Interrupt for falling edge (Release)
    last_release_tick = ticks;
    process_press();
  }
  GPIO_PORTF_ICR_R |= 0x10; // Clear interrupt bit
}

void SysTick_Handler(void) {
  // Hardware clears systick int reguest
  ticks++; // Increment ticks every 1ms

  // Handle auto mode
  if (mode == AUTO) {
    if (ticks % AUTO_MODE_INTERVAL == 0) // If 0.2 second has passed
      ment();                            // Increment or decrement counter
  }
}

int main(void) {
  setup();

  // Main loop
  while (1) {
    setLEDColor(colors[counter]);
  }

  return (0);
}
