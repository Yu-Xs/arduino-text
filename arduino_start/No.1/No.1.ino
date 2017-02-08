#include <SoftwareSerial.h>
#include <Wire.h>

const int TX = 7;
const int RX = 8;
SoftwareSerial BT( TX, RX);

void setup(){
	Serial.begin(9600);
	BT.begin(9600);
	Wire.begin();

	Serial.println("Start Xs");
	BT.write("Start Xs");
}

char val;
int stus = 4;

void loop(){

	if(BT.available()){				//android to arduino status
		val = BT.read();

		if(val == 7){
			BT.write(val);
			BT.write(stus);
		}else{
			stus = val;
		}
	}

	if(stus == 1){							//arduino to arduino
		val = 1;						//arduino left address 1
										//arduino right address 2
		Wire.beginTransmission(1);
		Wire.write(val);
		Wire.endTransmission();

		Wire.beginTransmission(2);
		Wire.write(val);
		Wire.endTransmission();
	}else if(stus == 2){
		val = 2;

		Wire.beginTransmission(1);
		Wire.write(val);
		Wire.endTransmission();
	}else if(stus == 3){
		val = 2;

		Wire.beginTransmission(2);
		Wire.write(val);
		Wire.endTransmission();
	}else if(stus == 4){
		val = 2;

		Wire.beginTransmission(1);
		Wire.write(val);
		Wire.endTransmission();

		Wire.beginTransmission(2);
		Wire.write(val);
		Wire.endTransmission();
	}
}