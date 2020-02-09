#ifndef __PID_H
#define __PID_H

class PID
{
    public:
        PID(double dt, double max, double min, double Kp, double Kd, double Ki ) :
            _dt(dt),
            _max(max),
            _min(min),
            _Kp(Kp),
            _Kd(Kd),
            _Ki(Ki),
            _pre_error(0),
            _integral(0)
        { }

        ~PID() { }
        double calculate(double setpoint, double pv, double dur);
        void reset();

    private:
        double _dt;
        double _max;
        double _min;
        double _Kp;
        double _Kd;
        double _Ki;
        double _pre_error;
        double _integral;
};

#endif
