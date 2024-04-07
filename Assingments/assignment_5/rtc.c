/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: rtc.c
*
* PROJECT....: ECP
*
* DESCRIPTION: See module specification file (.h-file).
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 090926  MoH   Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include "emp_type.h"
#include "gpio.h"
#include "rtc.h"
#include "tmodel.h"
#include "lcd.h"
#include "messages.h"
#include "tmodel.h"
#include "rtcs.h"
#include "glob_def.h"


/*****************************    Defines    *******************************/
typedef enum {
    INIT = 0,
    RUNNING,
    HALTED,
} RTCState ;

/*****************************   Constants   *******************************/
#define MAX_SEC 59
#define MAX_MIN 59
#define MAX_HOUR 23

/*****************************   Variables   *******************************/



/*****************************   Functions   *******************************/

// Increments the RTC time. This function should be guarded by a semaphore.
void increment_time() {
  INT8U sec, min, hour;
  sec = get_msg_state( SSM_RTC_SEC );
  min = get_msg_state( SSM_RTC_MIN );
  hour = get_msg_state( SSM_RTC_HOUR );

  sec++;
  if( sec > MAX_SEC )
  {
    min++;
    if( min > MAX_MIN )
    {
      hour++;
      if( hour > MAX_HOUR )
        hour = 0;
      min = 0;
    }
    sec = 0;
  }

  put_msg_state( SSM_RTC_SEC, sec );
  put_msg_state( SSM_RTC_MIN, min );
  put_msg_state( SSM_RTC_HOUR, hour );
}

void rtc_task(INT8U my_id, INT8U my_state, INT8U event, INT8U data)
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Disable global interrupt
******************************************************************************/
{

  switch( my_state )
  {

    case INIT:
      set_state( RUNNING );
      wait( 100 );
      break;

    case HALTED:
      // We only get here if we were halted and now got the semaphore meaning that
      // we should start running again

      // Signal because the count was increased upon entering task from a halted state
      signal_sem(SEM_RTC);

      set_state( RUNNING );
      break;

    case RUNNING:
      if (!wait_sem( SEM_RTC, WAIT_FOREVER )) {
          set_state(HALTED);
          return;
      }

      increment_time();
      signal_sem(SEM_RTC);

      wait(200); // Wait 1 second

      signal_sem( SEM_RTC_UPDATED );

      break;
  }
}

void display_rtc_task(INT8U my_id, INT8U my_state, INT8U event, INT8U data)
{
  // If time is not available, wait until it is
  if(!wait_sem( SEM_RTC, WAIT_FOREVER )) return;
  INT8U sec = get_msg_state( SSM_RTC_SEC );
  INT8U min = get_msg_state( SSM_RTC_MIN );
  INT8U hour = get_msg_state( SSM_RTC_HOUR );
  signal_sem( SEM_RTC );

  // If LCD is not available, wait until it is
  if(!wait_sem( SEM_LCD, WAIT_FOREVER )) return;
  move_LCD( LEFT_PADDING, FIRST_LINE );
  wr_ch_LCD( (INT8U)(hour/10+'0') );
  wr_ch_LCD( (INT8U)(hour%10+'0') );
  if( sec & 0x01 )
    wr_ch_LCD( ':' );
  else
    wr_ch_LCD( ' ' );
  wr_ch_LCD( (INT8U)(min/10+'0') );
  wr_ch_LCD( (INT8U)(min%10+'0') );
  if( sec & 0x01 )
    wr_ch_LCD( ' ' );
  else
    wr_ch_LCD( ':' );
  wr_ch_LCD( (INT8U)(sec/10+'0') );
  wr_ch_LCD( (INT8U)(sec%10+'0') );
  signal_sem( SEM_LCD );

  // Wait for the RTC to be updated
  wait_sem( SEM_RTC_UPDATED, WAIT_FOREVER );
}
/****************************** End Of Module *******************************/












