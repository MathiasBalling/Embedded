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
#define N_ROW 4
#define N_COL 3
/***************** Constants ******************/
extern const char keymap[];
/***************** Variables ******************/
/***************** Functions ******************/
INT8S get_keypress(void);

/***************** End of module **************/
#endif // KEYBOARD_H
