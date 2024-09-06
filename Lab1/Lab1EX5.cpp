//Group 12 Nathan Fant and Brandon Collings

// g++ -std=c++11 -o Lab1EX5 Lab1EX5.cpp -lwiringPi

#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>
#include <wiringPiI2C.h>
#include <string.h>
#include <iostream>
#include <softPwm.h>
#include <math.h>
#include <stdlib.h>
#include <ctime>
#include <signal.h>
#include <iomanip>
#include <unistd.h>


#define SERVO_MIN_MS 5
#define SERVO_MAX_MS 25
#define SERVO_MIN_ANGLE 0
#define SERVO_MAX_ANGLE 180
using namespace std;

/* signal pin of the servo*/
#define servoPin 1  
#define BUTTON 4 

int adcVal();
int adc;
bool dir;

//Specific a certain rotation angle (0-180) for the servo
void servoWrite(int pin, double angle){ 
    double time = 0;
    time = 10*(0.5 + (angle/90));/* map the desired angle to time*/
    //printf("Servo Time: %f\nServo Angle: %f\n", time, angle);
    softPwmWrite(pin,time);   
}


/* Sefind your callback function to handout the pressing button interrupts. */
void press_button()
{
    //sanitize input and then modify direction flag
    pinMode(BUTTON, INPUT);
    delay(100);
    int but_val = digitalRead(BUTTON);
    if (but_val == 1)
    {
        dir = !dir;
       //cout << "button press"<< endl;
    }
}



int main(void)
{
    dir = true;
    wiringPiSetup();    
    softPwmCreate(servoPin,  0, 200);

    /* Use wiringPiISR() to setup your interrupts. Refer to document WiringPi_ Interrupts.pdf. */
    wiringPiISR(BUTTON, INT_EDGE_RISING, &press_button);



    while(1){

        /* read ADS1015 value */
        double val = adcVal();
        //printf("val: %f\n", val);
        /* convert the obtained ADS1015 value to angle 0 - 180*/
        if (val == 4095) val = 0;
        //val = -1 * (val-1614)*30/269;
        if (dir)
        {
            val = (1614-val)*30/269;
	        //printf("Dir val: %d\n", dir);
            //printf("convert: %f\n", val);
        }
        else
        {
            val = 180 + ((val-1614)*30/269);
	        //printf("Dir val: %d\n", dir);
            //printf("convert: %f\n", val);
        }

        /* use the angle to control the servo motor*/
        servoWrite(servoPin, val);

        usleep(100000);

    }
    return 0;
}

//This function is used to read data from ADS1015
int adcVal(){

	uint16_t low, high, value;
	// Refer to the supplemental documents to find the parameters. In this lab, the ADS1015
	// needs to be set in single conversion, single-end mode, FSR (full-scale range)is 6.144, you can choose 
	// any input pin (A0, A1, A2, A3) you like.
	adc = wiringPiI2CSetup(0x48);
	wiringPiI2CWriteReg16(adc, 0x01, 0xC1C1);//0b0000_0001, 0b1100_0001_1100_0001*/
	usleep(1000);
    uint16_t data = wiringPiI2CReadReg16(adc, 0x00);


    low = (data & 0xFF00) >> 8;
    high = (data & 0x00FF) << 8;
    value = (high | low)>>4;
	return value;

}
