/***********************************************
 * Univeristy of Southern Denmark
 * Embedded Programming (EMP)
 *
 * MODULENAME: lcd.c
 * PROJECT: template c-file
 * DESCRIPTION: Empty module template
 * Change log:
 ***********************************************
 * Date of Change
 * YYMMDD
 * ----------------
 * 160224 MB Module created.
 ************************************************/

/***************** Header *********************/
/***************** Include files **************/
#include "lcd.h"
#include "tm4c123gh6pm.h"
/***************** Defines ********************/
// LCD control pins in GPIO port D
#define LCD_RS 2
#define LCD_E 3
// LCD data pins in GPIO port C
#define LCD_DB4 4
#define LCD_DB5 5
#define LCD_DB6 6
#define LCD_DB7 7
/***************** Constants ******************/
/***************** Variables ******************/
/***************** Functions ******************/
/***************** End of module **************/
void delay(volatile int time) {
  while (time > 0) {
    time--;
  }
}

void setup_lcd(void) {
  lcd_send_cmd(0x28); // Set 4-bit mode
  lcd_send_cmd(0x06); // Auto increment cursor
  lcd_send_cmd(0x0C); // Display on, cursor off
  lcd_send_cmd(0x01); // Clear display
}

void lcd_print_data(INT8U character) {
  GPIO_PORTC_DATA_R = 0xF0 & character;
}

void lcd_send_cmd(INT8U cmd) {
  lcd_print_data(cmd);
  GPIO_PORTD_DATA_R &= ~(1 << LCD_RS); // Write data to LCD screen
  GPIO_PORTD_DATA_R |= (1 << LCD_E);   // Enable LCD clock
  delay(7000);
  GPIO_PORTD_DATA_R &= ~(1 << LCD_E); // Disable LCD clock

  lcd_print_data(cmd << 4);
  GPIO_PORTD_DATA_R &= ~(1 << LCD_RS); // Write data to LCD screen
  GPIO_PORTD_DATA_R |= (1 << LCD_E);   // Enable LCD clock
  delay(7000);
  GPIO_PORTD_DATA_R &= ~(1 << LCD_E); // Disable LCD clock
}

void lcd_send_data(INT8U data) {
  lcd_print_data(data);
  GPIO_PORTD_DATA_R |= (1 << LCD_RS); // Write data to LCD screen
  GPIO_PORTD_DATA_R |= (1 << LCD_E);  // Enable LCD clock
  delay(7000);
  GPIO_PORTD_DATA_R &= ~(1 << LCD_E); // Disable LCD clock

  lcd_print_data(data << 4);
  GPIO_PORTD_DATA_R |= (1 << LCD_RS); // Write data to LCD screen
  GPIO_PORTD_DATA_R |= (1 << LCD_E);  // Enable LCD clock
  delay(7000);
  GPIO_PORTD_DATA_R &= ~(1 << LCD_E); // Disable LCD clock
}

void lcd_send_string(INT8U *str, INT8U len) {
  INT8U i;
  for (i = 0; i < len; i++) {
    lcd_send_data(str[i]);
  }
}
