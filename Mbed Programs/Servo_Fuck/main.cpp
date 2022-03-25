// Continuously sweep the servo through it's full range
#include "mbed.h"
#include "Servo.h"
#include <iostream>

//Servo myservo(D9);
PwmOut myservo(D9);

int main() {
    float A;
    while(1) {

        myservo.period(0.02f);
        myservo.pulsewidth(0.0005f);
        wait_us(10000000);

        myservo.pulsewidth(0.0015f);
        wait_us(2000000);

        myservo.pulsewidth(0.0025f);
        wait_us(2000000);

        /*
        for(int i=0; i<100; i++) {
            myservo = i/100.0;
            wait_us(100000);
        }
        for(int i=100; i>0; i--) {
            myservo = i/100.0;
            wait_us(100000);
        }
        */

        /*
        myservo.period(0.0002);
        myservo.write(0.5);
        wait_us(5000000);

        myservo.write(0.2);
        wait_us(5000000);

        myservo.write(0.1);
        wait_us(5000000);
        */

        /*
        myservo = 1;
        wait_us(5000000);
        */
    }
    return 0;
}
