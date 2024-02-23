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

void setup_peripherals(void);

/***************** End of module **************/

int main(void) {
  setup_peripherals();
  lcd_init();

  // lcd_send_cmd(0x80);
  // lcd_send_string("Mathias", 7);
  // lcd_send_cmd(0xC0);
  // lcd_send_string("Balling", 7);
  // Loop forever.
  while (1) {

    if ((GPIO_PORTF_DATA_R & (1 << 4)) == 0) {
      // If the button sw1 is pressed
      lcd_send_cmd(0x80);// Set cursor to first line
      lcd_send_string("Mathias", 7);
    } else {
      // If the button is not pressed
      lcd_send_cmd(0x01); // Clear display
    }
  }
  return (0);
}

void set_led_color(enum LED_Color color) { GPIO_PORTF_DATA_R = color << 1; }

void setup_peripherals(void) {
  // Enable the GPIO port
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5; // Enable the GPIO port F.
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R2; // Enable the GPIO port C.
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R3; // Enable the GPIO port D.

  // GPIOF:
  //  Set the direction as output (PF1 - PF3).
  GPIO_PORTF_DIR_R |= 0b00001110;

  // Enable the GPIO pins for digital function (PF1 - PF4)
  GPIO_PORTF_DEN_R |= 0b00011110;

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
