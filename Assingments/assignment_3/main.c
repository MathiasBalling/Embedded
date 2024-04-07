#include "button.h"
#include "emp_type.h"
#include "events.h"
#include "gpio.h"
#include "lcd.h"
#include "messages.h"
#include "rtc.h"
#include "sem.h"
#include "swtimers.h"
#include "systick.h"
#include "tm4c123gh6pm.h"
#include "tmodel.h"
#include <stdint.h>


extern volatile INT16S ticks;
INT16S alive_timer = MILLISEC(500);

int main(void)
/*****************************************************************************
 *   Input    : NONE
 *   Output   : Returns 0
 *   Function : Init hardware and then loop forever
 ******************************************************************************/
{
  init_systick();
  init_gpio();

  signal(MUTEX_SYSTEM_RTC);
  signal(MUTEX_LCD_DISPLAY);
  signal(SEM_RTC_UPDATED);

  // Loop forever.
  while (1) {
    // System part of the super loop.
    // ------------------------------
    while (!ticks)
      ;

    // The following will be executed every 5mS
    ticks--;

    if (!--alive_timer) {
      alive_timer = MILLISEC(500);
      GPIO_PORTD_DATA_R ^= 0x40;
    }

    // Protected operating system mode
    swt_ctrl();

    // Application mode
    button_task(TASK_BUTTON);
    rtc_task(TASK_RTC);
    display_rtc_task(TASK_RTC_DISPLAY);
    ajust_rtc_task(TASK_RTC_ADJUST);
    lcd_task(TASK_LCD);
  }
  return (0);
}
