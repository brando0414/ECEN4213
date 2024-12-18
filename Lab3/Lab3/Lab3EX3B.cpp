//Use g++ joystick.cc -std=c++11 -o Lab3EX3B Lab3EX3B.cpp

#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include "joystick.hh"
#include <cstdlib>
#define  PORT 8080
using namespace std;

int createSocket();

int sock = 0;

int main(int argc, char const *argv[]){
	
	//Open the file stream for the joystick
	Joystick joystick("/dev/input/js0");
	JoystickEvent event;
	if(!joystick.isFound()){
		cout << "Error opening joystick" << endl;
		exit(1);
	}


	//Create the connection to the server
	createSocket();

	while(true){

		/*Sample the events from the joystick*/
        int eventRaised = (int)joystick.sample(&event);
		if (eventRaised == 0) {
			continue;
		}
        int isButton = (int)event.isButton();
        int isJoystick = (int)event.isAxis();
        int inputNumber = (int)event.number;
        int inputValue = (int)event.value;
		//char test[] = "your mom";
        /*Convert the event to a useable data type so it can be sent*/
        int toSend[5] = {eventRaised, isButton, isJoystick, inputNumber, inputValue};
        /*Print the data stream to the terminal*/
        if (event.value != 0) cout << "input event: " << eventRaised << ", is button: " << isButton << ", is joystick: " << isJoystick << ", input number: " << inputNumber << ", input value: " << inputValue << "\n";
        /*Send the data to the server*/
        if(send(sock, toSend, sizeof(toSend), 0) != -1){
			cout << "Send Successful" << endl;
		}
		else{
			cout << "Send Failed" << endl;
		}
		if(event.isButton() && event.number == 6 && event.value > 0) {
			/*Closes out of all connections cleanly*/
			/*cout << "Close condition"<<endl;
			usleep(20000);
			//When you need to close out of the connection, please
			//close TTP/IP data streams.
			//Not doing so will result in the need to restart
			//the raspberry pi and Kobuki
				close(sock);
				exit(0);

			/*Set a delay
			usleep(20000);*/
		}
		usleep(1000*1000);
	}
	return 0;
}

//Creates the connection between the client and
//the server with the controller being the client
int createSocket(){
	struct sockaddr_in address;
	struct sockaddr_in serv_addr;

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("\nSocket creation error \n");
		return -1;
	}

	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port   = htons(PORT);

	/*Use the IP address of the server you are connecting to*/
	if(inet_pton(AF_INET, "127.0.0.1" , &serv_addr.sin_addr) <= 0){
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
		printf("\nConnection Failed \n");
		return -1;
	}
	return 0;
}

