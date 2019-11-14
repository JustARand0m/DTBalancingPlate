/*
 * main.c
 *
 *  Created on: 2019 Nov 12 10:18:39
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

void busy_waitMs(int ms)
{
	int i;

	for(int c = 0; c < ms; c++)
	{
		i = 10000;
		while(--i)
		{
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
		}
	}
}

#define BUFFERSIZE 1024

uint8_t read_data[BUFFERSIZE];
uint8_t testdata[] = "testdata";

int main(void)
{
  DAVE_STATUS_t status;
  uint8_t init_data[] = "startasfdasfdasfsadfasdfsafdasfdsadfasdfsadfasf";

  memset(read_data, 0, BUFFERSIZE);
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

  // Minimal position 450  ^ 4,5  duty ^ 0,9ms
  // Neutral position 750  ^ 7,5  duty ^ 1,5ms
  // Maximal position 1050 ^ 10,5 duty ^ 2,1ms

  UART_Transmit(&UART_0, init_data, sizeof(init_data));

  while(1U)
  {
	  /*
	  for(int i = 450; i <= 1050; i += 10)
	  {
		  PWM_SetDutyCycle(&PWM_MOTOR_0, i);
		  busy_waitMs(20);
	  }
	  for(int i = 1049; i > 450; i -= 10)
	  {
		  PWM_SetDutyCycle(&PWM_MOTOR_0, i);
		  busy_waitMs(20);
	  }
		*/
	  busy_waitMs(100);
	  /*UART_Receive(&UART_0, read_data, BUFFERSIZE);
	  if(read_data[0] != '\0') {
		  busy_waitMs(5000);
		  UART_Transmit(&UART_0, read_data, BUFFERSIZE);
		  memset(read_data, 0, BUFFERSIZE);
	  }*/
  }
}
int a = 0;
void DataReceived(void) {
	UART_Receive(&UART_0, read_data, 8);
	read_data[0] = '1';
	read_data[1] = '1';
	UART_Transmit(&UART_0, read_data, 8);
	memset(read_data, 0, BUFFERSIZE);
}

