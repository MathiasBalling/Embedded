/***********************************************
 * Univeristy of Southern Denmark
 * Embedded Programming (EMP)
 *
 * MODULENAME: emp_type.h
 * PROJECT: EMP
 * DESCRIPTION: Type definitions for embedded programming
 * Change log:
 ***********************************************
 * Date of Change
 * YYMMDD
 * ----------------
 * 160224 MB Module created.
 **********************************************/

/***************** Header *********************/
/***************** Include files **************/
#include "keyboard.h"
#include "emp_type.h"
#include "tm4c123gh6pm.h"
/***************** Defines ********************/
/***************** Constants ******************/
const char keymap[] = "123456789*0#";
/***************** Variables ******************/
/***************** Functions ******************/
INT8S get_keypress(void) {
  INT8U colX, rowY;
  for (colX = 0; colX < N_COL; colX++) {
    // Clear the outputs
    GPIO_PORTA_DATA_R &= ~0b00011100;
    // Set the output
    GPIO_PORTA_DATA_R |= 1 << (2 + colX);
    for (rowY = 0; rowY < N_ROW; rowY++) {
      if ((GPIO_PORTE_DATA_R & (1 << rowY))) {
        return (N_COL - 1 - colX) + ((N_ROW - 1 - rowY) * (N_ROW - 1));
      }
    }
  }
  return -1;
}
/***************** End of module **************/
