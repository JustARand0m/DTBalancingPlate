#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <DAVE.h>

#define BUFFERSIZE 13
#define SERVO_OFFSET 450
#define SERVO_DATA_LENGTH 3

#define MIN_SERVO_VALUE 500		// different than SERVO_OFFSET, because of structure of our prototype
#define MAX_SERVO_VALUE 1050

#define MOTOR_0_NEUTRAL 750
#define MOTOR_1_NEUTRAL 750
#define MOTOR_2_NEUTRAL 750

#define MOTOR_0_OFFSET 35
#define MOTOR_1_OFFSET 10
#define MOTOR_2_OFFSET 5

#define MOTOR_0_MIN 600
#define MOTOR_1_MIN 490
#define MOTOR_2_MIN 490

#define MOTOR_0_MAX 1050
#define MOTOR_1_MAX 950
#define MOTOR_2_MAX 950



struct PWM_Servos {
	int servo0;
	int servo1;
	int servo2;
};

uint8_t read_data[BUFFERSIZE];
struct PWM_Servos servos;

#endif /* GLOBALS_H_ */
