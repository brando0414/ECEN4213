//Use g++ -std=c++11 -o Lab4EX3 Lab4EX3.cpp -lwiringPi

#include <string>
#include <iostream>
#include <wiringSerial.h>
#include <wiringPi.h>
#include <unistd.h>
#include <cstdlib>
#include <iomanip>
#include <chrono>
using namespace std;
using namespace std::chrono;

int kobuki;

unsigned int bumper;
unsigned int drop;
unsigned int cliff;
unsigned int button;
unsigned int Read; 

int count = 0;
int c;

void turn(int, int);
void movement(int, int);
void readData();

int main(){
	//Create connection to the Kobuki
	wiringPiSetup();
	kobuki = serialOpen("/dev/kobuki", 115200);

	while(serialDataAvail(kobuki) != -1){
		/*Read the initial data. If there are no flags,
		the default condition is forward.*/
		readData();
		//cout << "Read Data Exit: " << bumper << endl;
		if(bumper > 0 || cliff > 0)
		{
			if (bumper > 0)
			{
				high_resolution_clock::time_point t3 = high_resolution_clock::now();
				high_resolution_clock::time_point t4 = high_resolution_clock::now();
				double timeElap2 = duration_cast<microseconds>(t4 - t3).count();
				while(timeElap2 < 1000000){
					movement(-100, 0);
					t4 = high_resolution_clock::now();
					timeElap2 = duration_cast<microseconds>(t4 - t3).count();
				}
				usleep(500);
				turn(30,-1);
			}
			if (cliff > 0)
			{
				high_resolution_clock::time_point t3 = high_resolution_clock::now();
				high_resolution_clock::time_point t4 = high_resolution_clock::now();
				double timeElap2 = duration_cast<microseconds>(t4 - t3).count();
				while(timeElap2 < 1000000){
					movement(-100, 0);
					t4 = high_resolution_clock::now();
					timeElap2 = duration_cast<microseconds>(t4 - t3).count();
				}
				usleep(500);
				turn(30,-1);
			}

		}
		else
		{
			movement(100,0);
		}
		/*Move slowly to give the sensors enough time to read data,
		the recommended speed is 100mm/s*/

		/*Create different states as to satisfy the conditions above.
		Remember, a single press of a bumper may last longer
		than one data cycle.*/

		/*Cleanly close out of all connections using Button 1.*/
		if(button == 0x02) serialClose(kobuki);

		/*Use serialFlush(kobuki) to discard all data received, or waiting to be send down the given device.*/
		serialFlush(kobuki);
	}
}

void turn(int rotation, int direction){
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        double timeElap1 = duration_cast<microseconds>(t2 - t1).count();
        while(timeElap1 < 3500000){
                movement(rotation, direction);
                t2 = high_resolution_clock::now();
                timeElap1 = duration_cast<microseconds>(t2 - t1).count();
        }
        movement(0,0);
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

void readData(){
	// you can reuse your code from EXE1, Lab 4
	while(true){
	//If the bytes are a 1 followed by 15, then we are
	//parsing the basic sensor data packet
		Read = (int)serialGetchar(kobuki);
		if(Read == 1){
			if(serialGetchar(kobuki) == 15) break;
		}
	}

	//Read past the timestamp
	serialGetchar(kobuki);
	serialGetchar(kobuki);

	bumper = (int)serialGetchar(kobuki);
	drop = (int)serialGetchar(kobuki);
	cliff = (int)serialGetchar(kobuki);

	/*Print the data to the screen.*/
	printf("Bumper: %d  Wheel Drop: %d  Cliff: %d\n", bumper, drop, cliff);


	/*Read through 6 bytes between the cliff sensors and
	the button sensors.*/
	serialGetchar(kobuki);
	serialGetchar(kobuki);
	serialGetchar(kobuki);
	serialGetchar(kobuki);
	serialGetchar(kobuki);
	serialGetchar(kobuki);

	/*Read the byte containing the button data.*/
	button = (int)serialGetchar(kobuki);

	//Pause the script so the data read receive rate is the same as the Kobuki send rate.
	usleep(20000);
}
