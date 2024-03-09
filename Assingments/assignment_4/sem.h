#ifndef SEM_H_
#define SEM_H_
/*****************************************************************************
 * University of Southern Denmark
 * Embedded C Programming (ECP)
 *
 * MODULENAME.: sem.h
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
 * 090315  MoH   Module created.
 *
 *****************************************************************************/

/***************************** Include files *******************************/
#include "emp_type.h"
/*****************************    Defines    *******************************/
#define MAX_SEM 10

/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/

BOOLEAN wait(INT8U sem_no);
/*****************************************************************************
 *   Input    : -
 *   Output   : -
 *   Function : Test function
 ******************************************************************************/
void signal(INT8U sem_no);
/*****************************************************************************
 *   Input    : -
 *   Output   : -
 *   Function : Test function
 ******************************************************************************/

/****************************** End Of Module *******************************/

#endif /*SEM_H_*/
