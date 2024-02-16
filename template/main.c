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
#include "emp_type.h"
#include "tm4c123gh6pm.h"
#include <stdint.h>
/***************** Defines ********************/
/***************** Constants ******************/
enum LED_Color {
  OFF = 0b000,
  GREEN = 0b100,
  BLUE = 0b010,
  CYAN = 0b110,
  RED = 0b001,
  YELLOW = 0b101,
  MAGENTA = 0b011,
  WHITE = 0b111
};
/***************** Variables ******************/

/***************** Functions ******************/

void set_led_color(enum LED_Color color);
/***********************************************
 * Input: enum LED_Color color
 * Output: void
 * Function: Helper function to set the color of the LED
 **********************************************/

void setup_peripherals(void);
/***********************************************
 * Input: void
 * Output: void
 * Function: Helper function to setup the peripherals
 **********************************************/

/***************** End of module **************/

int main(void) {

  // Loop forever.
  while (1) {
    if ((GPIO_PORTF_DATA_R & (1 << 4)) == 0) {
      // If the button sw1 is pressed
      set_led_color(BLUE);
    } else {
      // If the button is not pressed
      set_led_color(OFF);
    }
  }
  return (0);
}

void set_led_color(enum LED_Color color) { GPIO_PORTF_DATA_R = color << 1; }

void setup_peripherals(void) {
  // Enable the GPIO port that is used for the on-board LEDs and switches.
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;

  // Set the direction as output (PF1 - PF3).
  GPIO_PORTF_DIR_R |= 0b00001110;

  // Enable the GPIO pins for digital function (PF1 - PF4)
  GPIO_PORTF_DEN_R |= 0b00011110;

  // Enable internal pull-up (PF4).
  GPIO_PORTF_PUR_R |= (1 << 4);
}
