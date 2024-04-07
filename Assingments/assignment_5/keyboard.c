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
#include "glob_def.h"
#include "gpio.h"
#include "lcd.h"
#include "messages.h"
#include "rtc.h"
#include "rtcs.h"
#include "tm4c123gh6pm.h"
#include "tmodel.h"
/***************** Defines ********************/
typedef enum {
  INIT = 0,
  IDLE,
  RTC_WAIT,
  LCD_WAIT,
  SET_TIME,
} RTCAdjustState;

#define NO_PRESS 0xFF
#define N_ROW 4
#define N_COL 3
#define X_COL_START 2
#define LCD_FIRST_COLON_POS 2
#define LCD_SECOND_COLON_POS 4
#define N_DIGITS 6

/***************** Constants ******************/
const char keymap[] = "123456789*0#";
/***************** Variables ******************/
/***************** Functions ******************/

INT8U get_keypress(void) {
  // columns (x) and rows (y)
  INT8U x, y;
  for (x = 0; x < N_COL; x++) {
    // Clear the outputs
    GPIO_PORTA_DATA_R &= ~0b00011100;
    // Set the output
    GPIO_PORTA_DATA_R |= 1 << (X_COL_START + x);

    for (y = 0; y < N_ROW; y++) {
      // Scan all rows
      if ((GPIO_PORTE_DATA_R & (1 << y))) {
        return (N_COL - 1 - x) + ((N_ROW - 1 - y) * (N_ROW - 1));
      }
    }
  }
  return NO_PRESS;
}

void init_keyboard(void) {
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4; // GPIO Port E
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R0; // GPIO Port A

  // Set Y as input and X as output
  GPIO_PORTE_DIR_R &= ~(0b00001111);

  GPIO_PORTE_PDR_R |= 0b00001111;
  GPIO_PORTE_DEN_R |= 0b00001111;

  GPIO_PORTA_DIR_R |= 0b00011100;
  GPIO_PORTA_DEN_R |= 0b00011100;
}

void exit_set_time_ui() {
  disable_cursor();
  clear_LCD();
}

void enter_set_time_ui() {
  enable_cursor();
  move_LCD(0, SECOND_LINE);
  wr_str_LCD("Set current time!");
  move_LCD(LEFT_PADDING, FIRST_LINE);
}

void update_lcd_cursor(INT8U cursor) {
  INT8U offset = 0;
  if (cursor >= LCD_FIRST_COLON_POS)
    offset++; // Skip first colon
  if (cursor >= LCD_SECOND_COLON_POS)
    offset++; // Skip second colon
  INT8U lcd_cursor = LEFT_PADDING + cursor + offset;
  move_LCD(lcd_cursor, FIRST_LINE);
}

void keyboard_task(INT8U my_id, INT8U my_state, INT8U event, INT8U data) {
  static INT8U last_press = NO_PRESS;

  INT8U press = get_keypress();

  if (press == last_press) return;
  last_press = press;

  if (press == NO_PRESS) return;

  put_queue(Q_KEYBOARD, keymap[press], WAIT_FOREVER);
  put_queue(Q_UART_TX, keymap[press], WAIT_FOREVER);
}

void adjust_rtc_task(INT8U my_id, INT8U my_state, INT8U event, INT8U data){
  INT8U key;
  BOOLEAN key_was_pressed = get_queue(Q_KEYBOARD, &key, WAIT_FOREVER);

  // Static variables for setting time.
  static INT8U cursor = 0;
  static INT8U sec_high = 0;
  static INT8U sec_low = 0;
  static INT8U min_high = 0;
  static INT8U min_low = 0;
  static INT8U hour_high = 0;
  static INT8U hour_low = 0;

  switch (my_state) {
  case INIT:
    set_state(IDLE);
    break;
  case IDLE: {
    if (!key_was_pressed) return;
    if (key != '*') return;

    set_state(RTC_WAIT);
    break;
  }
  case RTC_WAIT:
    // In this state we wait for the LCD semaphore
    if (wait_sem(SEM_RTC, WAIT_FOREVER)) {
      // Get time variables
      INT8U sec = get_msg_state(SSM_RTC_SEC);
      INT8U min = get_msg_state(SSM_RTC_MIN);
      INT8U hour = get_msg_state(SSM_RTC_HOUR);

      // Get the individual digits
      sec_high = sec / 10;
      sec_low = sec % 10;
      min_high = min / 10;
      min_low = min % 10;
      hour_high = hour / 10;
      hour_low = hour % 10;

      set_state(LCD_WAIT);
    }
    break;
  case LCD_WAIT:
    // In this state we wait for the LCD semaphore
    if (wait_sem(SEM_LCD, WAIT_FOREVER)) {
      enter_set_time_ui();
      cursor = 0;
      set_state(SET_TIME);
    }
    break;
  case SET_TIME: {
      if (!key_was_pressed) return;
      update_lcd_cursor(cursor);

      // Exit set time UI
      if (key == '*') {
        set_state(IDLE);
        // Construct numbers
        INT8U sec = sec_high * 10 + sec_low;
        INT8U min = min_high * 10 + min_low;
        INT8U hour = hour_high * 10 + hour_low;

        put_msg_state(SSM_RTC_SEC, sec);
        put_msg_state(SSM_RTC_MIN, min);
        put_msg_state(SSM_RTC_HOUR, hour);

        exit_set_time_ui();

        // Release all the semaphores
        signal_sem(SEM_RTC);
        signal_sem(SEM_LCD);
      }
      // Delete digit
      else if (key == '#') {
        cursor--;
        if (cursor > N_DIGITS)
          cursor = N_DIGITS - 1;
        update_lcd_cursor(cursor);
      }
      // Set digit
      else {
        INT8U digit = key - '0';
        BOOLEAN increment = TRUE;

        // Store the digit
        switch (cursor) {
        case 0:
          if (digit > HOUR_MAX / 10) {
            increment = FALSE;
            digit = hour_high;
          } else
            hour_high = digit;
          break;
        case 1: {
          if (digit > HOUR_MAX % 10) {
            increment = FALSE;
            digit = hour_low;
          } else
            hour_low = digit;
          break;
        }
        case 2: {

          if (digit > MIN_MAX / 10) {
            increment = FALSE;
            digit = min_high;
          } else
            min_high = digit;
          break;
        }
        case 3:
          min_low = digit;
          break;
        case 4: {
          if (digit > SEC_MAX / 10) {
            increment = FALSE;
            digit = sec_high;
          } else
            sec_high = digit;
          break;
        }
        case 5:
          sec_low = digit;
          break;
        }
                
        if (increment) cursor = (cursor + 1) % 6;

        wr_ch_LCD(digit + '0');

        update_lcd_cursor(cursor);
      }
      break;
    }
  }
}
  /***************** End of module **************/
