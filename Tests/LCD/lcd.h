#ifndef _MODULE_H
#define _MODULE_H
/***********************************************
 * Univeristy of Southern Denmark
 * Embedded Programming (EMP)
 *
 * MODULENAME: lcd.h
 * PROJECT: template h-file
 * DESCRIPTION: Empty header template
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
/***************** Defines ********************/
/***************** Variables ******************/
/***************** Functions ******************/
extern void delay(volatile int time);
extern void lcd_init(void);
extern void lcd_send_cmd(INT8U cmd);
extern void lcd_send_data(INT8U data);
extern void lcd_send_string(INT8U *str, INT8U len);
extern void lcd_print_data(INT8U character);
/***************** End of module **************/

#endif // _MODULE_H
