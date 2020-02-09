#include <chrono>
#include <iostream>
#include <thread>
#include <cmath>
#include <vector>
#include <algorithm>
#include "uart/uart_raspi.h"
#include "pid.h"
#include "socketReceiver/socketReceiver.h"
#include <wiringPi.h>
#include <chrono>

const int PIN_BUTTON = 1;
	
const double PI_F = 3.14159265358979;
const double SERVO_ARM_LENGTH = 4.0;
const double SERVO_RADIUS = 10.0;

const double MAX_DEG_INPUT = 15.0;

double RadianToDegree(double radian)
{
	return (radian * 180.0) / PI_F;
}

double DegreeToRadian(double degree)
{
	return (degree * PI_F) / 180.0;
}

int AngleToPWM(double angle)
{
	if(angle < -18.0)
	{
		return 300;
	}
	if(angle > 18.0)
	{
		return 500;
	}
	return ((angle + 18.0) * 5.5) + 300;
}

void ComputeMotorAngles(const double& alpha, const double& beta,
			double& motorAngle0,
			double& motorAngle1,
			double& motorAngle2)
{
	if((alpha > MAX_DEG_INPUT) || (beta > MAX_DEG_INPUT)) 
	{
		/* TODO: Proper error handling */
		std::cout << "Error: exceeded max angles.\n";
		return;
	}	

	double rAlpha = DegreeToRadian(alpha);
	double rBeta  = DegreeToRadian(beta);

	motorAngle0 = asin((-SERVO_RADIUS * sin(rBeta)) / SERVO_ARM_LENGTH);
	motorAngle1 = asin(((SERVO_RADIUS / 2) * sin(rBeta) + ((-SERVO_RADIUS / 2) * sqrt(3)) * sin(rAlpha)) / SERVO_ARM_LENGTH);	
	motorAngle2 = asin(((SERVO_RADIUS / 2) * sin(rBeta) + ((SERVO_RADIUS / 2) * sqrt(3)) * sin(rAlpha)) / SERVO_ARM_LENGTH);	

	motorAngle0 = RadianToDegree(motorAngle0);
	motorAngle1 = RadianToDegree(motorAngle1);
	motorAngle2 = RadianToDegree(motorAngle2);
}

enum socketState{
	XDATA, YDATA, IS_DONE
};


void computeSocketData(socketReceiver* receiver, uartConnection &uartCon) {
	bool isDone = false;
	socketState state = XDATA;
	std::string socketData = receiver->getData();
	std::string xStr;
	std::string yStr;
	messageDecoding msgDec;
	
	if(socketData.length() == 0) {
		return;	
	}

	for(auto dataChar : socketData) {
		switch (state) {
			case XDATA:
				if(dataChar == ';') {
					state = YDATA;
					break;
				}
				xStr.push_back(dataChar);	
				break;
			case YDATA: 
				if(dataChar == ';') {
					state = IS_DONE;
					break;
				}
				yStr.push_back(dataChar);
				break;
			case IS_DONE:
				isDone = true;
				break;
			default:
				state = XDATA;
				break;
		}
	}
	if(isDone){
		double x = std::stod(xStr);
		double y = std::stod(yStr);

		if(x > 15.0){
			x = 15.0;
		}
		if(x < -15.0) {
			x = -15.0;
		}
		if(y > 15.0) {
			y = 15.0;
		}
		if(y < -15.0) {
			y = -15.0;
		}

		double motorAngle0 = 0, motorAngle1 = 0, motorAngle2 = 0;


		ComputeMotorAngles(x, y, motorAngle0, motorAngle1, motorAngle2);
		std::string newData = msgDec.encodeMessage(AngleToPWM(motorAngle0), AngleToPWM(motorAngle1), AngleToPWM(motorAngle2));
		std::cout << newData << std::endl;
		uartCon.writeData(newData);
	}

}


void udpSocket(uartConnection &uartCon, bool &isActive) {
	socketReceiver* receiver = startSocketReceiver();

	try{
		
		while(true) {
			if(isActive) {
				computeSocketData(receiver, uartCon);
			}
		}
	}
	catch(...) {
		std::cout << "error in socket" << std::endl;
	}
}

//constexpr double KP = 0.004;
//constexpr double KI = 0.003;
//constexpr double KD = 0.025;

//constexpr double KP = 0.007;
//constexpr double KI = 0.00001;
//constexpr double KD = 0.00004;

//constexpr double KP = 0.0075;
//constexpr double KI = 0.00002;
//constexpr double KD = 0.00009;
//1.03
//0.14
//0.56

// BEST
//constexpr double KP = 0.0103;
//constexpr double KI = 0.0014;
//constexpr double KD = 0.0025;

constexpr double KP = 0.0103;
constexpr double KI = 0.0019;
constexpr double KD = 0.0025;

void pid_controller(uartConnection &uartCon, bool &isActive) {
    PID pid_x = PID(0.1, 15, -15, KP, KI, KD);
    PID pid_y = PID(0.1, 15, -15, KP, KI, KD);
    messageDecoding msgDec;

    double setpointX = 530;
    double setpointY = 530;

    double alpha = 0;
    double beta  = 0;

    double motorAngle0 = 0;
    double motorAngle1 = 0;
    double motorAngle2 = 0;

    int m1, m2, m3;

    double valX = 0; // getXVal();
    double valY = 0; // getYVal();

    double test = 0;

    char sendBuffer[12] = { 0 };


    auto lastTimePoint = std::chrono::system_clock::now();

    int dataCounter = 0;

    std::vector<int> valX_med(11, 0), valY_med(11, 0);


    while(true) {
	    if(isActive) {
		std::vector<int> data = uartCon.getData();
		    if(data.size() == 0) 
		    {
			   continue;
		    } 
		    
		    valX_med.push_back(data[0]);
		    valY_med.push_back(data[1]);
		    dataCounter++;

		    if(dataCounter == 10)
		    {
			    std::sort(valX_med.begin(), valX_med.end());
			    std::sort(valY_med.begin(), valY_med.end());

			    valX = valX_med[6];
			    valY = valY_med[6];

			    dataCounter = 0;
			    valX_med.clear();
			    valY_med.clear();

			    std::chrono::duration<double> dur = std::chrono::system_clock::now() - lastTimePoint;
			    lastTimePoint = std::chrono::system_clock::now();

			    alpha = -pid_x.calculate(setpointX, valX, dur.count());
			    beta  = pid_y.calculate(setpointY, valY, dur.count());

			    ComputeMotorAngles(alpha, beta, motorAngle0, motorAngle1, motorAngle2);

			    //std::cout << "alpha: " << alpha << " beta: " << beta << std::endl;
			    //std::cout << "X: " << valX << " Y: " << valY << std::endl << std::endl;

			    std::string newData = msgDec.encodeMessage(AngleToPWM(motorAngle0), AngleToPWM(motorAngle1), AngleToPWM(motorAngle2));
			    uartCon.writeData(newData);
			    std::this_thread::sleep_for(std::chrono::milliseconds(10));
		    }

	    }
	    else
	    {
		    pid_x.reset();
		    pid_y.reset();
	    }
    }

}

void switchExecutionMode() {
	int mode = 0;
	bool prevHigh = false;
	bool done = false;
	std::chrono::time_point<std::chrono::high_resolution_clock> start;
	std::chrono::time_point<std::chrono::high_resolution_clock> end;
	uartConnection uartCon("/dev/serial0");
	uartCon.startListening();
	bool controllerActive = false;
	bool appActive = false;

	pinMode(PIN_BUTTON, INPUT);

	std::thread controller_thread = std::thread(pid_controller, std::ref(uartCon), std::ref(controllerActive));
	controller_thread.detach();


	std::thread app_thread =  std::thread(udpSocket, std::ref(uartCon), std::ref(appActive));
	app_thread.detach();
	
	try {
		while(true) {
			while(true) {
				if(digitalRead(PIN_BUTTON) == LOW) {
					prevHigh = true;
					start = std::chrono::high_resolution_clock::now();
				} else {
					if(true == prevHigh) {
						end = std::chrono::high_resolution_clock::now();
						auto duration = end - start;
						if(duration.count() > 1) {
							std::cout << "button high" << std::endl;
							done = true;
							prevHigh = false;
							break;
						}
					}
					prevHigh = false;
				}
			}
			
			if(done) {

				mode++;
				if(mode > 3) {
					mode = 0;
				}
				done = false;
				switch(mode) {
					case 0:
						std::cout << "controller active" << std::endl;
						appActive = false;
						controllerActive = true;
						break;
					case 1:
						std::cout << "app active" << std::endl;
						appActive = true;
						controllerActive = false;
						break;		
					default:
						break;
				}
			}
		}
	}
	catch(...){
		std::cout << "error in switcher" << std::endl;
	}
}


int main() {
	wiringPiSetup();
	switchExecutionMode();
        return 0;
}
