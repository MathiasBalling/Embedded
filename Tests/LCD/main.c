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
#include "lcd.h"
#include "tm4c123gh6pm.h"
/***************** Defines ********************/
/***************** Constants ******************/
/***************** Variables ******************/
/***************** Functions ******************/
void setup_gpio(void) {
  // Enable the GPIO port
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5; // Enable the GPIO port F.
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R2; // Enable the GPIO port C.
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R3; // Enable the GPIO port D.

  // GPIOF:
  // Enable the GPIO pins for digital function (PF1 - PF4)
  GPIO_PORTF_DEN_R |= (1 << 4);

  // Enable internal pull-up (PF4).
  GPIO_PORTF_PUR_R |= (1 << 4);

  // GPIOC:
  // Set the direction as output (PC4 - PC7).
  GPIO_PORTC_DIR_R |= 0b11110000;
  // Enable the GPIO pins for digital function (PC4 - PC7).
  GPIO_PORTC_DEN_R |= 0b11110000;

  // GPIOD:
  // Set the direction as output (PD2 - PD3).
  GPIO_PORTD_DIR_R |= 0b00001100;
  // Enable the GPIO pins for digital function (PD2 - PD3).
  GPIO_PORTD_DEN_R |= 0b00001100;
}
/***************** End of module **************/

int main(void) {
  setup_gpio();
  setup_lcd();

  // Loop forever.
  while (1) {

    if ((GPIO_PORTF_DATA_R & (1 << 4)) == 0) {
      // If the button sw1 is pressed
      lcd_send_cmd(0x80); // Set cursor to first line
      lcd_send_string("Mathias Balling", 15);
      lcd_send_cmd(0xC0); // Set cursor to second line
      lcd_send_string("SDU Robotics", 12);
      while ((GPIO_PORTF_DATA_R & (1 << 4)) == 0) {
        // Wait for the button to be released
      }
    } else {
      // If the button is not pressed
      lcd_send_cmd(0x01); // Clear display
    }
  }
  return (0);
}


