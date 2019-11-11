/*
 * main.c
 *
 *  Created on: 2019 Nov 11 17:37:29
 *  Author: brm47801
 */




#include <DAVE.h>                 //Declarations from DAVE Code Generation (includes SFR declaration)

/**

 * @brief main() - Application entry point
 *
 * <b>Details of function</b><br>
 * This routine is the application entry point. It is invoked by the device startup code. It is responsible for
 * invoking the APP initialization dispatcher routine - DAVE_Init() and hosting the place-holder for user application
 * code.
 */

int main(void)
{
  DAVE_STATUS_t status;

  status = DAVE_Init();           /* Initialization of DAVE APPs  */

  if(status != DAVE_STATUS_SUCCESS)
  {
    /* Placeholder for error handler code. The while loop below can be replaced with an user error handler. */
    XMC_DEBUG("DAVE APPs initialization failed\n");

    while(1U)
    {

    }
  }

  /* Placeholder for user application code. The while loop below can be replaced with user application code. */


  while(1U)
  {
	  PWM_SetDutyCycle(&PWM_MOTOR_0, 750);
	  PWM_SetDutyCycle(&PWM_MOTOR_1, 750);
	  PWM_SetDutyCycle(&PWM_MOTOR_2, 750);

	  int i = 100000;

	  while(i--)
	  {
		  __NOP();
	  }

	  PWM_SetDutyCycle(&PWM_MOTOR_0, 1000);
	  PWM_SetDutyCycle(&PWM_MOTOR_1, 1000);
	  PWM_SetDutyCycle(&PWM_MOTOR_2, 1000);

	  i = 100000;

	 while(i--)
	 {
	  	__NOP();
	 }

  }
}
