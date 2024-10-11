//Use g++ -std=c++11 -o Lab3EX3A Lab3EX3A.cpp -lwiringPi

#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <wiringSerial.h>
#include <wiringPi.h>
#include <chrono>
#define PORT 8080
using namespace std;
using namespace std::chrono;

void movement(int, int);
void createSocket();
void readData();

int kobuki, new_socket;
int sock = 0;
int buffer[1024] = {0};
int bufferlength = 5; 
bool closed = false;
int sp = 0; int r = 0;

/*Create char buffer to store transmitted data*/

int main(){
	//Initialize filestream for the Kobuki
	wiringPiSetup();
	kobuki = serialOpen("/dev/kobuki", 115200);

	//Create connection to client
	createSocket();

	while(true){
		//Read data from client
		readData();
		//cout << read(sock, buffer, sizeof(buffer)) << endl;
	}
	return 0;
}

void movement(int sp, int r){
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

//Creates the connection between the client and
//the server with the Kobuki being the server
void createSocket(){
	int server_fd;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);

	if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	address.sin_family      = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port        = htons(PORT);

	if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0){
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	if(listen(server_fd, 3) < 0){
		perror("listen");
		exit(EXIT_FAILURE);
	}

	if((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0){
		perror("accept");
		exit(EXIT_FAILURE);
	}
}

void readData(){
	/*Read the incoming data stream from the controller*/
	cout << "read.." << endl;
    int valread = read(new_socket, buffer, sizeof(buffer));
	if (valread != 0) cout << "Packet Received" << endl;
    int eventRaised = (int)buffer[0];
    int isButton = (int)buffer[1];
    int isJoystick = (int)buffer[2];
    int inputNumber = (int)buffer[3];
    int inputValue = (int)buffer[4];
    /*Print the data to the terminal*/
    if (inputValue != 0) cout << "input event: " << eventRaised << ", is button: " << isButton << ", is joystick: " << isJoystick << ", input number: " << inputNumber << ", input value: " << inputValue << "\n";
	/*Use the received data to control the Kobuki*/
		/*Create a series of commands to interpret the
		joystick input and use that input to move the Kobuki*/

		//Use the following Key Map:
		//Up     - move the Kobuki forward
		//Down   - move the Kobuki backward
		//Left   - rotate the Kobuki 90 degrees counterclockwise
		//Right  - rotate the Kobuki 90 degrees clockwise
		//Start  - immediately stop the Kobuki's movement
		//Select - exit the script and close the Kobuki's connection cleanly
		if (eventRaised == 1)
		{
			if (isButton == 1)
			{
				printf("isButton: %u | Value: %d\n", inputNumber, inputValue);
				/*Interpret the joystick input and use that input to move the Kobuki*/
				switch(inputNumber){
					case(6):
						if(inputValue > 0) closed = !closed;
						break;
					case(7):
						if(inputValue > 0) 
						{
							sp = 0;
							r = 0;
							break;
						}
				}

			}
			if (isJoystick == 1)
			{
				printf("isAxis: %u | Value: %d\n", inputNumber, inputValue);
				/*Interpret the joystick input and use that input to move the Kobuki*/
				switch(inputNumber){
					case(6):
						if (inputValue > 0)
						{
							high_resolution_clock::time_point t1 = high_resolution_clock::now();
							high_resolution_clock::time_point t2 = high_resolution_clock::now();
							double timeElap1 = duration_cast<microseconds>(t2 - t1).count();
							while(timeElap1 < 3850000){
								movement(68, -1);
								t2 = high_resolution_clock::now();
								timeElap1 = duration_cast<microseconds>(t2 - t1).count();
							}
							sp = 0;
							r = 0;
							break;
						}
						if (inputValue < 0)
						{
							high_resolution_clock::time_point t3 = high_resolution_clock::now();
							high_resolution_clock::time_point t4 = high_resolution_clock::now();
							double timeElap2 = duration_cast<microseconds>(t4 - t3).count();
							while(timeElap2 < 3850000){
								movement(68, 1);
								t4 = high_resolution_clock::now();
								timeElap2 = duration_cast<microseconds>(t4 - t3).count();
							}
							sp = 0; 
							r = 0;
							break;
						}
					case(7):
						if (inputValue < 0) 
						{
							sp = 100;
							r = 0;
							break;
						}
						if (inputValue > 0) 
						{ 
							sp = -100;
							r = 0;
							break;
						}
				}

				
			}
		printf("sp: %d, r: %d\n", sp, r);
		movement(sp,r);
	}

	
	if(closed) {
	/*Closes out of all connections cleanly*/

	//When you need to close out of all connections, please
	//close both the Kobuki and TTP/IP data streams.
	//Not doing so will result in the need to restart
	//the raspberry pi and Kobuki
		close(new_socket);
		serialClose(kobuki);
		exit(0);
	}




	/*Reset the buffer*/
	memset(&buffer, '0', sizeof(buffer));


}
