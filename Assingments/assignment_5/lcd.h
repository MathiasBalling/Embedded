/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: leds.h
*
* PROJECT....: ECP
*
* DESCRIPTION: Test.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 050128  KA    Module created.
*
*****************************************************************************/

#ifndef _LCD_H
  #define _LCD_H

/***************************** Include files *******************************/
#include "emp_type.h"

/*****************************    Defines    *******************************/
// Special ASCII characters
// ------------------------

#define LF		0x0A
#define FF		0x0C
#define CR		0x0D

#define ESC		0x1B


/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/
INT8U clear_LCD();
void wr_str_LCD( char* );
void move_LCD( INT8U, INT8U );
INT8U wr_ch_LCD( INT8U Ch );
void enable_cursor();
void disable_cursor();

void lcd_task(INT8U, INT8U, INT8U, INT8U);
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Test function
******************************************************************************/


/****************************** End Of Module *******************************/
#endif

