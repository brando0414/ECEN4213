//use g++ -std=c++11 -o Lab4EX2 Lab4EX2.cpp -lwiringPi


#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <ctime>
#include <ratio>
#include <chrono>
using namespace std;
using namespace std::chrono;

int kobuki;
void turn(int);
float read_sonar();
void movement(int, int);
int SIG = 1;
int count = 0;
float d= 0;

int main(){
	wiringPiSetup();
	kobuki = serialOpen("/dev/kobuki", 115200);

	/*Move from a random point within the area designated "X" to the
	point B as shown on the diagram. Use a sonar sensor to navigate through the channel.
	You can reuse your code from Lab 2 and 3*/
    while (true)
    {
        d = read_sonar();
        if (d < 10 && count == 0 && d > 1)
        {
            turn(-1);
            usleep(500);
            count++;
            movement(0,0);
        }
        else if (d < 10 && count == 1 && d > 1)
        {
            turn(1);
            usleep(500);
            count++;
            movement(0,0);
        }
        else if (d < 10 && count == 2 && d > 1)
        {
            serialClose(kobuki);
        }
        else 
        {
            movement(100,0);
        }
    }


	/*Note: the Kobuki must completely pass point B as shown to receive full credit*/


}

void turn(int direction){
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        double timeElap1 = duration_cast<microseconds>(t2 - t1).count();
        while(timeElap1 < 3500000){
                movement(68, direction);
                t2 = high_resolution_clock::now();
                timeElap1 = duration_cast<microseconds>(t2 - t1).count();
        }
        movement(0,0);
}


float read_sonar(){
	// you can reuse your code from Lab 2
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
    float pulse_width;
    while(digitalRead(SIG))
    {
        // 2. defind a varable to get the current time t2.
        high_resolution_clock::time_point t2 =high_resolution_clock::now();
        // 3. calculate the time duration: t2 - t1
        pulse_width = duration_cast<microseconds>(t2 - t1).count();
        // 4. if the duration is larger than the Pulse Maxium 18.5ms, break the loop.
        if (pulse_width >= 18500 && pulse_width <= 115) break;
    }



    /*Calculate the distance by using the time duration that you just obtained.*/ //Speed of sound is 340m/s
    float distance = 0.034 / 2 * pulse_width;


    /*Print the distance.*/
    printf("Distance: %lf\n", distance);
    return distance;


    /*Delay before next measurement. The actual delay may be a little longer than what is shown is the datasheet.*/
    usleep(2000);
}




void movement(int sp, int r){
	// you can reuse your code from Lab 3
	//Create the byte stream packet with the following format:
	unsigned char b_0 = 0xaa; /*Byte 0: Kobuki Header 0*/
	unsigned char b_1 = 0x55; /*Byte 1: Kobuki Header 1*/
	unsigned char b_2 = 0x0a; /*Byte 2: Length of Payload*/
	unsigned char b_3 = 0x01; /*Byte 3: Sub-Payload Header*/
	unsigned char b_4 = 0x04; /*Byte 4: Length of Sub-Payload*/

	unsigned char b_5 = sp & 0xff;	//Byte 5: Payload Data: Speed(mm/s)
	unsigned char b_6 = (sp >> 8) & 0xff; //Byte 6: Payload Data: Speed(mm/s)
	unsigned char b_7 = r & 0xff;	//Byte 7: Payload Data: Radius(mm)
	unsigned char b_8 = (r >> 8) & 0xff;	//Byte 8: Payload Data: Radius(mm)
	unsigned char checksum = 0;		//Byte 9: Checksum
	
	//Checksum all of the data
	char packet[] = {b_0,b_1,b_2,b_3,b_4,b_5,b_6,b_7,b_8};
	for (unsigned int i = 2; i < 9; i++)
		checksum ^= packet[i];

	/*Send the data (Byte 0 - Byte 8 and checksum) to Kobuki using serialPutchar (kobuki, );*/
	for (int i = 0; i < 9; i++)
	{
		serialPutchar(kobuki, packet[i]);
	}
	/*Pause the script so the data send rate is the
	same as the Kobuki data receive rate*/

	usleep(20000);
	
}
