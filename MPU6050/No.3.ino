//L293D 啟動
//未測試:杜邦線帶不夠XD(3/20)

const int Motor_1_0 7;
const int Motor_1_1 8;

void setup(){
	Serial.begin(9600);
	pinMode( Motor_1_0, OUTPUT);
	pinMode( Motor_1_1, OUTPUT);
}

int a = 0;

void loop(){

	if(Serial.available()){
		a=Serial.read();
	}

	if(a){
		digitalWrite(Motor_1_0, LOW);
		digitalWrite(Motor_1_1, HIGH);
	}else{
		digitalWrite(Motor_1_1, LOW);
		digitalWrite(Motor_1_0, HIGH);
}