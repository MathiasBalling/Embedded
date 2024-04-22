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
#include "gpio.h"
#include "keyboard.h"
#include "tm4c123gh6pm.h"

/***************** Defines ********************/
/***************** Constants ******************/
/***************** Variables ******************/
/***************** Functions ******************/
void delay(int n) {
  int i;
  for (i = 0; i < n; i++) {
  }
}

void send_char(char str) {
  while (UART0_FR_R & (1 << 5))
    ;
  UART0_DR_R = str;
}
void send_string(char *str) {
  while (*str) {
    send_char(*str);
    str++;
  }
}
/***************** End of module **************/

int main(void) {
  setup_gpio();
  // Loop forever.
  INT8S key;
  INT8S last_key = -1;
  while (1) {
    key = get_keypress();
    if (key != -1) {
      // If the button sw1 is pressed
      set_led_color(BLUE);
      // Send the key pressed to the UART
      if (key != last_key)
        send_char(keymap[key]);
    } else if (key == -1) {
      // If the button is not pressed
      set_led_color(YELLOW);
    } else {
      // If the button is not pressed
      set_led_color(RED);
    }
    last_key = key;
    delay(100000);
  }
  return (0);
}
