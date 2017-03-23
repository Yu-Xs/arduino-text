//L293D 啟動

const int Motor_1_0 = 7;
const int Motor_1_1 = 8;

void setup(){
	Serial.begin(9600);
	pinMode( Motor_1_0, OUTPUT);
	pinMode( Motor_1_1, OUTPUT);
}

bool a = false;

void loop(){

	if(Serial.available()){
		if(Serial.read() == '0'){
			a = false;
		}else{
			a = true;
		}
	}

	if(a){
		Serial.println(a);
		analogWrite( Motor_1_0, 0);
		// analogWrite( Motor_1_1, 255);
		for(int i = 0; i <= 255; i++){
			analogWrite( Motor_1_1, i);
			delay(4000);
		}
	}else{
		Serial.println(a);
		analogWrite( Motor_1_1, 0);
		// analogWrite( Motor_1_0, 255);
		for(int i = 0; i <= 255; i++){
			analogWrite( Motor_1_0, i);
			delay(4000);
		}
	}
}