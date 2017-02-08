#include <Stepper.h>
#include <Wire.h>

const int setp = 2049;
const int pin1 = 7;
const int pin2 = 8;
const int pin3 = 9;
const int pin4 = 10;
Stepper left_stepper( setp, pin1, pin2, pin3, pin4);

void setup(){
	Serial.begin(9600);

	Wire.begin(1);
	Wire.onReceive(status);

	left_stepper.setSpeed(7);

	Serial.println("Start left_stepper");
}

char val;
int stus = 2;

void loop(){
	if(stus == 1){
		Serial.println("start");
		left_stepper.step(2049);
	}else{
		Serial.println("stop");
	}
}

void status(){

	if(Wire.available()){
		val = Wire.read();

		stus = val;
	}
}