//Use g++ -std=c++11 -o Lab3EX2 Lab3EX2.cpp -lwiringPi

#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <chrono>
#include <cmath>

using namespace std::chrono;
using namespace std;

// functions
void sigroutine(int);
int adcVal();
void PID(float, float, float);
float read_potentionmeter();
float read_sonar();


// variables
float distance_previous_error = 0;
float distance_error;
float obj_value, measured_value; // potentionmeter reading, sonar reading
int adc;
float PID_p,  PID_d, PID_total, PID_i = 0;
int time_inter_ms = 23; // time interval, you can use different time interval

/*set your pin numbers and pid values*/
int FAN = 26;
int SIG = 1;
float kp= ; 
float ki= ; 
float kd= ;




int main(){
	wiringPiSetup();
    adc = wiringPiI2CSetup(0x48);

    /*Set the pinMode (fan pin)*/
    pinMode(FAN, OUTPUT);
    softPwmCreate(FAN,0,100);
    
    // This part is to set a system timer, the function "sigroutine" will be triggered  
    // every time_inter_ms milliseconds. 
    struct itimerval value, ovalue;
    signal(SIGALRM, sigroutine);
    value.it_value.tv_sec = 0;
    value.it_value.tv_usec = time_inter_ms*1000;
    value.it_interval.tv_sec = 0;
    value.it_interval.tv_usec = time_inter_ms*1000;
    setitimer(ITIMER_REAL, &value, &ovalue);    

	while(true){
        cout<<"obj_value: "<<obj_value<<" measured_value: "<<measured_value<<endl;
        cout<<"PID_p: "<<PID_p<<endl;
        cout<<"PID_i: "<<PID_i<<endl;
        cout<<"PID_d: "<<PID_d<<endl;
        cout<<"PID_total: "<<PID_total<<endl;
        delay(20);
	}
}


void sigroutine(int signo){
    PID(kp, ki, kd);
    return;
}


/* based on the obj distance and measured distance, implement a PID control algorithm to 
the speed of the fan so that the Ping-Pang ball can stay in the obj position*/
void PID(float kp, float ki, float kd){
    /*read the objective position/distance of the ball*/
    float obj_value = read_potentiometer();
    /*read the measured position/distance of the ball*/
    float measured_value = read_sonar();
    /*calculate the distance error between the obj and measured distance */
    float distance_error = measured_value - obj_value;
    float cumError += distance_error*time_inter_ms;
    float rateError = (distance_error - distance_previous_error)/time_inter_ms;
    /*calculate the proportional, integral and derivative output */
    PID_p = kp * Error;
    PID_i = ki * cumError;
    PID_d = kd * rateError;
    PID_total = PID_p + PID_d + PID_i; 

    /*assign distance_error to distance_previous_error*/
    distance_previous_error = distance_error;

    /*use PID_total to control your fan*/
    softPemWrite(FAN, PID_total);
}


/* use a sonar sensor to measure the position of the Ping-Pang ball. you may reuse
your code in EX1.*/
float read_sonar()
{
    /*Set the pinMode to output and generate a LOW-HIGH-LOW signal using "digitalWrite" to trigger the sensor. 
    Use a 2 us delay between a LOW-HIGH and then a 5 us delay between HIGH-LOW. You can use
    the function "usleep" to set the delay. The unit of usleep is microsecond. */
    pinMode(SIG, OUTPUT);
    digitalWrite(SIG, LOW);
    usleep(2);
    digitalWrite(SIG, HIGH);
    usleep(5);
    digitalWrite(SIG, LOW);

    /*Echo holdoff delay 750 us*/
    usleep(750);


    /*Switch the pinMode to input*/ 
    pinMode(SIG, INPUT);
    
    
    /*Get the time it takes for signal to leave sensor and come back.*/

    // 1. defind a varable to get the current time t1. Refer to "High_Resolution_Clock_Reference.pdf" for more information
    high_resolution_clock::time_point t1 =high_resolution_clock::now();
    int pulse_width;
    while(digitalRead(SIG))
    {
        // 2. defind a varable to get the current time t2.
        high_resolution_clock::time_point t2 =high_resolution_clock::now();
        // 3. calculate the time duration: t2 - t1
        pulse_width = chrono::duration_cast<chrono::microseconds>(t2 – t1).count();
        // 4. if the duration is larger than the Pulse Maxium 18.5ms, break the loop.
        if (pulse_width >= 18.5) break;
    }



    /*Calculate the distance by using the time duration that you just obtained.*/ //Speed of sound is 340m/s
    float distance = 3.4 *  pulse_width;


    /*Print the distance.*/
    //printf("Distance: %d", distance)
    return distance;


    /*Delay before next measurement. The actual delay may be a little longer than what is shown is the datasheet.*/
    usleep(200);
}

/* use a potentiometer to set an objective position (10 - 90 cm) of the Ping-Pang ball, varying the potentiometer
can change the objective distance. you may reuse your code in Lab 1.*/
float read_potentionmeter()
{

}



int adcVal(){
	uint16_t low, high, value;
	wiringPiI2CWriteReg16(adc, /* */,  /* */);
	usleep(1000);
    uint16_t data = wiringPiI2CReadReg16(adc,  /* */);
    low = (data & 0xFF00) >> 8;
    high = (data & 0x00FF) << 8;
    value = (high | low)>>4;
	return value;
}