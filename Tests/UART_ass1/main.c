/*****************************************************************************
 * University of Southern Denmark
 * Embedded Programming (EMP)
 *
 * MODULENAME.: main.c
 *
 * PROJECT....: EMP
 *
 * DESCRIPTION: Assignment 1, main module. No main.h file.
 *
 * Change Log:
 *****************************************************************************
 * Date    Id    Change
 * YYMMDD
 * --------------------
 * 150215  MoH   Module created.
 *
 *****************************************************************************/

/***************************** Include files *******************************/
#include "button.h"
#include "counter.h"
#include "countled.h"
#include "emp_type.h"
#include "gpio.h"
#include "swtimers.h"
#include "systick.h"
#include "tm4c123gh6pm.h"
#include "uart0.h"
#include <stdint.h>
#include <stdlib.h>

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
extern INT16S ticks;
/*****************************   Functions   *******************************/

int main(void)
/*****************************************************************************
 *   Input    :
 *   Output   :
 *   Function : The super loop.
 ******************************************************************************/
{
  INT8U event;
  INT8U counter_value;
  INT8U last_counter_value;
  INT8U alive_timer = TIM_500_MSEC;

  init_systick();
  init_gpio();
  uart0_init(9600, 8, 1, 'n');

  // Loop forever.
  while (1) {
    // System part of the super loop.
    // ------------------------------
    while (!ticks)
      ;

    // The following will be executed every 5ms
    ticks--;

    if (!--alive_timer) {
      alive_timer = TIM_500_MSEC;
      GPIO_PORTD_DATA_R ^= 0x40;
    }

    // Application part of the super loop.
    // -----------------------------------
    event = select_button();
    counter_value = counter(event);
    counter_leds(counter_value);
    if (counter_value != last_counter_value) {
      last_counter_value = counter_value;
      uart0_string("Counter: ", 9);
      while (!uart0_tx_rdy())
        ;
      uart0_putc(counter_value % 8 + '0');
      while (!uart0_tx_rdy())
        ;
      uart0_putc('\n');
    }
  }
  return (0);
}

/****************************** End Of Module *******************************/
