#ifndef _LCD_H
#define _LCD_H
/***********************************************
 * Univeristy of Southern Denmark
 * Embedded Programming (EMP)
 *
 * MODULENAME: lcd.h
 * PROJECT: LCD Control
* DESCRIPTION: Helper functions for controlling an LCD display
 * Change log:
 ***********************************************
 * Date of Change
 * YYMMDD
 * ----------------
 * 260224 MB Module created.
 **********************************************/

/***************** Header *********************/
/***************** Include files **************/
#include "emp_type.h"
/***************** Defines ********************/
/***************** Variables ******************/
/***************** Functions ******************/
extern void delay(volatile int time);
extern void setup_lcd(void);
extern void lcd_send_cmd(INT8U cmd);
extern void lcd_send_data(INT8U data);
extern void lcd_send_string(INT8U *str, INT8U len);
extern void lcd_print_data(INT8U character);
/***************** End of module **************/

#endif // _LCD_H
