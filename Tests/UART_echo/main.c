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

void send_char(INT8U chr) {
  while (UART0_FR_R & (1 << 5))
    ;
  UART0_DR_R = chr;
}
void send_string(char *str) {
  while (*str) {
    send_char(*str);
    str++;
  }
}

void send_angle_txt(INT16S angle) {
  if (angle < 0) {
    send_char('-');
    angle = -angle;
  }
  send_char('0' + (angle) / 1000);
  send_char('0' + (angle % 1000) / 100);
  send_char('0' + (angle % 100) / 10);
  send_char('0' + (angle % 10));
}

void uart_data_conversion(INT8U data, INT16S *angle) {
  // ABOSDDDD
  // A: Angle (1 = Pan, 0 = Tilt)
  // B: High bit (1 = High, 0 = Low)
  // O: Other (Data if B=0, Request if B=1)
  // S: Sign (Data if B=0, Sign if B=1)
  // D: Data
  if (data & (1 << 6)) {   // High bits
    if (data & (1 << 5)) { // Request data
      send_angle_txt(*angle);
    } else { // No request (Data and sign)
      *angle = ((data & 0x0F) << 6) | (*angle & 0x3F);
      if (data & (1 << 4)) { // Negative
        *angle = -(*angle);
      }
    }
  } else { // Low bits
    if (*angle < 0) {
      *angle = -((data & 0x3F) | ((-*angle) & 0xFFC0));
    } else {
      *angle = (data & 0x3F) | (*angle & 0xFFC0);
    }
  }
}
/***************** End of module **************/

int main(void) {
  INT16S pan_angle = 0;
  INT16S tilt_angle = 0;
  INT8U uart_data = 0;
  setup_gpio();

  // Loop forever.
  while (1) {
    if (!(UART0_FR_R & (1 << 4))) { // Check if there is data
      uart_data = UART0_DR_R;       // Read the data
      if (uart_data & (1 << 7)) {
        send_char('P');
        uart_data_conversion(uart_data, &pan_angle);
      } else {
        send_char('T');
        uart_data_conversion(uart_data, &tilt_angle);
      }
    }
    delay(10000);
  }
  return (0);
}
