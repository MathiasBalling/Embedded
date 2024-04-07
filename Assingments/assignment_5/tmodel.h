/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: tmodel.h
*
* PROJECT....: ECP
*
* DESCRIPTION: Defines the elemtn of the task model..
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 101004  MoH   Module created.
*
*****************************************************************************/

#ifndef _TMODEL_H_
#define _TMODEL_H_

#include "rtcs.h"

// Tasks.
// ------
#define TASK_RTC         USER_TASK
#define TASK_DISPLAY_RTC USER_TASK+1
#define TASK_LCD         USER_TASK+2
#define TASK_UART_TX     USER_TASK+3
#define TASK_KEYBOARD    USER_TASK+4
#define TASK_ADJUST_RTC  USER_TASK+5


// Interrupt Service Routines.
// ---------------------------
#define ISR_TIMER 21

// Semaphores.
// -----------
#define SEM_LCD          USER_SEM
#define SEM_RTC          USER_SEM+1 // For setting the time
#define SEM_RTC_UPDATED  USER_SEM+2 // Signal when RTC is updated to show on display


// Shared State Memory.
// --------------------
#define SSM_RTC_SEC            0
#define SSM_RTC_MIN            1
#define SSM_RTC_HOUR           2

// QUEUEs.
// -------
#define Q_UART_TX   USER_QUEUE
#define Q_LCD       USER_QUEUE+1
#define Q_KEYBOARD  USER_QUEUE+2

#endif /* _TMODEL_H_ */
