/***********************************************
 * Univeristy of Southern Denmark
 * Embedded Programming (EMP)
 *
 * MODULENAME: main.c
 * PROJECT: template
 * DESCRIPTION: Empty project template
 * Change log:
 ***********************************************
 * Date of Change
 * YYMMDD
 * ----------------
 * 160224 MB Module created.
 **********************************************/

/***************** Header *********************/
/***************** Include files **************/
#include "gpio.h"
#include "systick.h"
#include "tm4c123gh6pm.h"
#include <stdint.h>

/***************** Defines ********************/
#define DOUBLE_CLICK_TIMEOUT 100 // Timeout for double click
#define LONG_PRESS_TIMEOUT 2000  // Timeout for long press
#define DEBOUNCE_TIMEOUT 10      // Debounce timeout

#define LIGHT_DURATION 1000 // Duration of the light
#define LIGHT_TRANSITION_DURATION 500 // Duration from red to green and vice versa
#define NORWEGIAN_LIGHT_DURATION 1000 // Duration of the light in norwegian mode

enum TraficLightState {
  NORMAL,
  NORWEGIAN,
  EMERGENCY,
};

enum Event {
  SINGLE,
  DOUBLE,
  LONG,
};

enum ButtonState {
  FIRST_PUSH,
  FIRST_RELEASE,
  SECOND_PUSH,
};

enum NormalLightState {
  STOP,
  READY,
  GO,
  YIELD,
};

/***************** Constants ******************/
/***************** Variables ******************/
enum TraficLightState state = NORMAL;
volatile uint32_t button_ticks = 0;
volatile uint32_t ticks = 0;
/***************** Functions ******************/
/***************** End of module **************/
void handle_event(enum Event event) {
  switch (event) {
  case SINGLE:
    state = NORWEGIAN;
    break;
  case DOUBLE:
    state = EMERGENCY;
    break;
  case LONG:
    state = NORMAL;
    break;
  }
  ticks = 0;
}
void process_press() {
  button_ticks = LONG_PRESS_TIMEOUT;
  while (button_sw1_pressed()) {
  }
  uint32_t current_ticks = button_ticks;
  if (current_ticks > (LONG_PRESS_TIMEOUT - DEBOUNCE_TIMEOUT)) {
    // Filter out debouncing
  } else if (current_ticks == 0) {
    // Long press
    handle_event(LONG);
  } else {
    // Short press
    // Reset the counter
    button_ticks = DOUBLE_CLICK_TIMEOUT;

    enum Event event = SINGLE;
    // Wait for a potential double click
    while (button_ticks) {
      if (button_sw1_pressed() &&
          button_ticks < (DOUBLE_CLICK_TIMEOUT - DEBOUNCE_TIMEOUT)) {
        event = DOUBLE;
        // setLEDColor(GREEN);
        while (button_sw1_pressed()) {
          // Wait for the button to be released
        }
        break;
      }
    }
    // Handle the press type
    handle_event(event);
  }
}

void norwegian_lights() {
  if (!ticks) {
    GPIO_PORTF_DATA_R |= 0b00001010;
    ticks = NORWEGIAN_LIGHT_DURATION;
    GPIO_PORTF_DATA_R ^= 0b00000100;
  }
}

void normal_lights() {
  static enum NormalLightState light_state = STOP;

  switch (light_state) {
  case STOP: // Red light
    GPIO_PORTF_DATA_R = ~0b00000010;
    ticks = LIGHT_DURATION;
    while (ticks)
      ;
    light_state = READY;
    break;
  case READY: // Red and yellow light
    GPIO_PORTF_DATA_R = ~0b00000110;
    ticks = LIGHT_TRANSITION_DURATION;
    while (ticks)
      ;
    light_state = GO;
    break;
  case GO: // Green light
    GPIO_PORTF_DATA_R = ~0b00001000;
    ticks = LIGHT_DURATION;
    while (ticks)
      ;
    light_state = YIELD;
    break;
  case YIELD: // Yellow light
    GPIO_PORTF_DATA_R = ~0b00000100;
    ticks = LIGHT_TRANSITION_DURATION;
    while (ticks)
      ;
    light_state = STOP;
    break;
  }
}

void GPIOF_handler(void) {
  // Temporary disable the interrupt on PF4
  GPIO_PORTF_IM_R &= ~(1 << 4); // Disable interrupt on PF4

  // Process the press
  process_press();

  // Re-enable the interrupt on PF4
  GPIO_PORTF_ICR_R |= 0x10;    // Clear interrupt bit
  GPIO_PORTF_IM_R |= (1 << 4); // Interrupt on PF4
}

void SysTick_handler(void) {
  // Use for determining the button press
  if (button_ticks)
    button_ticks--;

  // Use for timing the trafic lights
  if (ticks)
    ticks--;
}

int main(void) {
  setup_GPIOF();
  setup_systick();
  // Loop forever.
  while (1) {
    // continue;
    switch (state) {
    case NORMAL:
      normal_lights();
      break;
    case NORWEGIAN:
      norwegian_lights();
      break;
    case EMERGENCY:
      GPIO_PORTF_DATA_R |= 0b00001100;
      // Toggle the YELLOW light on emp board
      GPIO_PORTF_DATA_R &= ~0b00000010;
      break;
    default:
      break;
    }
  }
  return (0);
}
