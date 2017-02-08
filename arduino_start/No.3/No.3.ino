#include <Stepper.h>
#include <Wire.h>

const int setp = 2049;
const int pin1 = 7;
const int pin2 = 8;
const int pin3 = 9;
const int pin4 = 10;
Stepper right_stepper( setp, pin1, pin2, pin3, pin4);

void setup(){
	Serial.begin(9600);

	Wire.begin(2);
	Wire.onReceive(status);

	right_stepper.setSpeed(7);

	Serial.println("Start right_stepper");
}

int val;
int stus = 2;

void loop(){
	if(stus == 1){
		Serial.println("start");
		right_stepper.step(-77);
	}else{
		Serial.println("stop");
	}
}

void status(){

	if(Wire.available()){
		val = Wire.read();
		Serial.println("OK");
		delay(5000);
		stus = val;
	}
}