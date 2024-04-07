#ifndef KEYBOARD_H
#define KEYBOARD_H
/***********************************************
 * Univeristy of Southern Denmark
 * Embedded Programming (EMP)
 *
 * MODULENAME: keyboard.h
 * PROJECT: template
 * DESCRIPTION: Empty project template
 * Change log:
 ***********************************************
 * Date of Change
 * YYMMDD
 * ----------------
 * 240328 MB Module created.
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
void init_keyboard(void);
void keyboard_task(INT8U my_id, INT8U my_state, INT8U event, INT8U data);
void adjust_rtc_task(INT8U my_id, INT8U my_state, INT8U event, INT8U data);

/***************** End of module **************/
#endif // KEYBOARD_H
