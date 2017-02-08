#include <Wire.h>

void setup(){
	Wire.begin();
	Serial.begin(9600);

	Serial.println("Start Wire");
}

void loop(){
	if(Serial.available()){
		int val = Serial.read() - 48;

		Wire.beginTransmission(2);
		Wire.write(val);
		Wire.endTransmission();

		Serial.print("OK ");
		Serial.println(val);
	}
}