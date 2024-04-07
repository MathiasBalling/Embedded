#ifndef RTC_H_
#define RTC_H_

#include "emp_type.h"

#define LEFT_PADDING 4
#define FIRST_LINE 0
#define SECOND_LINE 1

#define SEC_MAX 59
#define MIN_MAX 59
#define HOUR_MAX 23

void rtc_task(INT8U, INT8U, INT8U, INT8U);
void display_rtc_task(INT8U, INT8U, INT8U, INT8U);

#endif /*RTC_H_*/
