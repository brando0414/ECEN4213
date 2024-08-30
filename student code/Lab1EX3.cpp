//Group ## Nathan Fant and Brandon Collings

//The stator in the Stepper Motor we have supplied has 32 magnetic poles. Therefore, to complete
// one full revolution requires 32 full steps. The rotor (or output shaft) of the Stepper 
//Motor is connected to a speed reduction set of gears and the reduction ratio is 1:64. Therefore, 
//the final output shaft (exiting the Stepper Motor’s housing) requi res 32 X 64 = 2048 
//steps to make one full revolution.

// g++ -std=c++11 -o Lab1EX3 Lab1EX3.cpp -lwiringPi
#include <stdio.h>
#include <wiringPi.h>

#define shortest_time_period_ms 3

void moveOnePeriod(int dir){
    if(dir == 1){
        /* clockwise, there are four steps in one period, set a delay after each step*/
        digitalWrite(6,1);
        digitalWrite(4,0);
        digitalWrite(5,0);
        digitalWrite(1,0);
        delay(3);
        digitalWrite(6,0);
        digitalWrite(4,1);
        digitalWrite(5,0);
        digitalWrite(1,0);
        delay(3);
        digitalWrite(6,0);
        digitalWrite(4,0);
        digitalWrite(5,1);
        digitalWrite(1,0);
        delay(3);
        digitalWrite(6,0);
        digitalWrite(4,0);
        digitalWrite(5,0);
        digitalWrite(1,1);
        delay(3);

    }
    else{
        /* anticlockwise, there are four steps in one period, set a delay after each step*/
        digitalWrite(6,0);
        digitalWrite(4,0);
        digitalWrite(5,0);
        digitalWrite(1,1);
        delay(3);
        digitalWrite(6,0);
        digitalWrite(4,0);
        digitalWrite(5,1);
        digitalWrite(1,0);
        delay(3);
        digitalWrite(6,0);
        digitalWrite(4,1);
        digitalWrite(5,0);
        digitalWrite(1,0);
        delay(3);
        digitalWrite(6,1);
        digitalWrite(4,0);
        digitalWrite(5,0);
        digitalWrite(1,0);
        delay(3);
    }
}
//continuous rotation function, the parameter steps specifies the rotation cycles, every four steps is a cycle
void moveCycles(int dir,int cycles){
    int i;
    for(i=0;i<cycles;i++){
        moveOnePeriod(dir);
    }
}

int main(void){
    wiringPiSetup();
    /* set the pin mode*/
    pinMode(1, OUTPUT)//IN1, GPIO18
    pinMode(4, OUTPUT)//IN2, GPIO23
    pinMode(5, OUTPUT)//IN3, GPIO24
    pinMode(6, OUTPUT)//IN4, GPIO25

    while(1){
        /*rotating 360° clockwise, a total of 2048 steps in one full revolution, namely, 512 cycles.
        use function moveCycles(int dir,int cycles)*/
        moveCycles(1, 512);
        delay(500);

        /*rotating 360° anticlockwise, a total of 2048 steps in one full revolution, namely, 512 cycles.
        use function moveCycles(int dir,int cycles)*/
        moveCycles(0, 512);
        delay(500);
    }
    return 0;
}

