#include <iostream>
#include <cmath>
#include "pid.h"

double PID::calculate( double setpoint, double pv, double dur)
{
    // Calculate error
    double error = setpoint - pv;

    // Proportional term
    double Pout = _Kp * error;

    // Integral term
    _integral += error * dur;
    double Iout = _Ki * _integral;

    // Derivative term
    double derivative = (error - _pre_error) / dur;
    double Dout = _Kd * derivative;

    // Calculate total output
    double output = Pout + Iout + Dout;

    // Restrict to max/min
    if( output > _max )
        output = _max;
    else if( output < _min )
        output = _min;

    // Save error to previous error
    _pre_error = error;
    
    if (pv - output < 0) {
        output = 0;
    }

    return output;
}

void PID::reset()
{
    _dt = 0;
    _pre_error = 0;
    _integral = 0;
}
