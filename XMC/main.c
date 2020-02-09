#include "globals.h"
#include "uartCom.h"

int main(void)
{
  DAVE_STATUS_t status;

  status = DAVE_Init();
  servos.servo0 = MOTOR_0_NEUTRAL;
  servos.servo1 = MOTOR_1_NEUTRAL;
  servos.servo2 = MOTOR_2_NEUTRAL;

  if(status != DAVE_STATUS_SUCCESS)
  {
    XMC_DEBUG("DAVE APPs initialization failed\n");

    while(1U)
    {

    }
  }

  /* Start listening for UART */
  UART_StartReceiveIRQ(&UART_0, read_data, BUFFERSIZE);

  while(1U)
  {
  }
}

void irTimerSetPwmValues() {

/*
	if (servos.servo0 > MOTOR_0_MAX) servos.servo0 = MOTOR_0_MAX;
	if (servos.servo1 > MOTOR_1_MAX) servos.servo1 = MOTOR_1_MAX;
	if (servos.servo2 > MOTOR_2_MAX) servos.servo2 = MOTOR_2_MAX;

	// Minimum 500, if lower = min gets set
	if(servos.servo0 < MOTOR_0_MIN) servos.servo0 = MOTOR_0_MIN;
	if(servos.servo1 < MOTOR_1_MIN) servos.servo1 = MOTOR_1_MIN;
	if(servos.servo2 < MOTOR_2_MIN) servos.servo2 = MOTOR_2_MIN;

	 PWM_SetDutyCycle(&PWM_MOTOR_0, MOTOR_0_NEUTRAL);
	 PWM_SetDutyCycle(&PWM_MOTOR_1, MOTOR_1_NEUTRAL);
	 PWM_SetDutyCycle(&PWM_MOTOR_2, MOTOR_2_NEUTRAL);
*/

	 PWM_SetDutyCycle(&PWM_MOTOR_0, servos.servo0);
	 PWM_SetDutyCycle(&PWM_MOTOR_1, servos.servo1);
	 PWM_SetDutyCycle(&PWM_MOTOR_2, servos.servo2);

	 TIMER_ClearEvent(&TIMER_SET_PWM_VALUES);
}
