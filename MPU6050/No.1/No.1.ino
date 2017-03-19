//Main

#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include <PID_v1.h>

MPU6050 accelgyro;

/*kalman setting*/

int16_t ax, ay, az;												//MPU6050原始值
int16_t gx, gy, gz;

unsigned long now, lastTime = 0;								//微分時間
float dt;

float aax = 0, aay = 0, aaz = 0;								//角度變量
float agx = 0, agy = 0, agz = 0;

long axo = 0, ayo = 0, azo = 0;									//偏移量
long gxo = 0, gyo = 0, gzo = 0;

float pi = 3.1415926;											//比例係數
float AcceTatio = 16384.0;
float GyroRatio = 131.0;

uint8_t n_sample = 8;											//採樣計數
float aaxs[8] = {0}, aays[8] = {0}, aazs[8] = {0};
long aax_sum, aay_sum, aaz_sum;

float a_x[10] = {0}, a_y[10] = {0}, a_z[10] = {0};				//平方差
float g_x[10] = {0}, g_y[10] = {0}, g_z[10] = {0};

float Px = 1, Rx, Kx, Sx, Vx, Qx;								//軸卡爾曼變量
float Py = 1, Ry, Ky, Sy, Vy, Qy;
float Pz = 1, Rz, Kz, Sz, Vz, Qz;

void kalman();													//卡爾曼濾波


/*PID setting*/

double Setpoint, Input, Output;

double Kp = 2, Ki = 5, Kd = 1;									//未調適

PID carPID( &Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

void setupPID();


/*DC motor*/

const int Motor_1_0 4;											//pin
const int Motor_1_1 7;
const int Motor_2_0 8;
const int Motor_2_1 9;

void setup() {

	Wire.begin();												//初始化
	Serial.begin(38400);
	accelgyro.initialize();

	/*kalman setting*/

	unsigned short times = 200;									//採樣係數

	for(int i = 0; i < times; i++){								//採樣和
		accelgyro.getMotion6( &ax, &ay, &az, &gx, &gy, &gz);

		axo += ax;
		ayo += ay;
		azo += az;

		gxo += gx;
		gyo += gy;
		gzo += gz;
	}

	axo /= times;												//偏移量
	ayo /= times;
	azo /= times;

	gxo /= times;
	gyo /= times;
	gzo /= times;

	/*PID setting*/

	setupPID();


	/*DC motor*/

	pinMode( Motor_1_0, OUTPUT);								//正轉
	pinMode( Motor_2_0, OUTPUT);
	pinMode( Motor_1_1, OUTPUT);								//反轉
	pinMode( Motor_2_1, OUTPUT);
}


void loop() {
	kalman();

	Input = abs(agy);
	carPID.Compute();
	Drive(Output);
}

void kalman(){
	now = millis();												//微分時間
	dt = (now - lastTime) / 1000.0;
	lastTime = now;

	accelgyro.getMotion6( &ax, &ag, &az, &gx, &gy, &gz);

	float accx = ax / AcceTatio;								//加速度換算角度
	float accy = ay / AcceTatio;
	float accz = az / AcceTatio;

	aax = atan( accy / accz ) * (-180) / pi;
	aay = atan( accx / accz ) * 180 / pi;
	aaz = atan( accz / accy ) * 180 / pi;


	aax_sum = 0;												//加速度濾波
	aay_sum = 0;
	aaz_sum = 0;

	for(int i = 1; i < n_sample; i++){
		aaxs[i-1] = aaxs[i];
		aax_sum += aaxs[i];

		aays[i-1] = aays[i];
		aay_sum += aays[i];

		aazs [i-1] = aazs[i];
		aaz_sum += aazs[i];
	}

	aaxs[n_sample-1] = aax;										//角度調整至0-90   not(需要更正至0-180)
	aax_sum += aax * n_sample;
	aax = (aax_sum / (11 * n_sample / 2.0)) * 9 / 7.0;			//係數9/7是由某大神實驗出來的

	aays[n_sample - 1] = aay;
	aay_sum += aay * n_sample;
	aay = (aay_sum / (11 * n_sample / 2.0)) * 9 / 7.0;

	aazs[n_sample - 1] = aaz;
	aaz_sum += aaz * n_sample;
	aaz = (aaz_sum / (11 * n_sample / 2.0)) * 9 / 7.0;


	float gyrox = -(gx - gxo) / GyroRatio * dt;					//角速度 * Time
	float gyroy = -(gy - gyo) / GyroRatio * dt;
	float gyroz = -(gz - gzo) / GyroRatio * dt;

	agx += gyrox;												//角速度積分
	agy += gyroy;
	agy += gyroz;


	Sx = 0;														//計算平均加數度
	Sy = 0;
	Sz = 0;

	Rx = 0;
	Ry = 0;
	Rz = 0;

	for(int i = 1; i < 10; i++){
		a_x[i-1] = a_x[i];
		Sx += a_x[i];

		a_y[i-1] = a_y[i];
		Sy += a_y[i];

		a_z[i-1] = a_z[i];
		Sz += a_z[i];
	}

	a_x[9] = aax;
	Sx += aax;
	Sx /= 10;

	a_y[9] = aay;
	Sy += aay;
	Sy /= 10;

	a_z[9] = aaz;
	Sz += aaz;
	Sz /= 10;


	for(int i = 0; i < 10; i++){
		Rx += sq(a_x[i] - Sx);
		Ry += sq(a_y[i] - Sy);
		Rz += sq(a_z[i] - Sz);
	}

	Rx /= 9;
	Ry /= 9;
	Rz /= 9;

	Px += 0.0025;
	Kx = Px / (Px + Rx);
	agx = agx + Kx * (aax - agx);
	Px = (1 - Kx) * Px;

	Py += 0.0025;
	Ky = Py / (Py + Ry);
	agy = agy + Ky * (aay - agy);
	Py = (1 - ky) * Py;

	Pz += 0.0025;
	Kz = Pz / (Pz + Rz);
	agz = agz + Kz * (aaz - agz);
	Pz = (1 - Kz) * Pz;


	Serial.print(agx);
	Serial.print('\t');
	Serial.print(agy);
	Serial.print('\t');
	Serial.print(agz);
	Serial.println();
}

void setupPID(){
	Input = 0;
	Setpoing = 90;
	carPID.SetSampleTime(100);
	carPID.SetMode(AUTOMATIC);
}

void Drive(double OUT){
	bool O;

	if(agx >= 0){
		O = true;
	}else{
		O = false;
	}

	if(O){
		analogWrite( Motor_1_1, 0);
		analogWrite( Motor_2_1, 0);
		analogWrite( Motor_1_0, OUT);
		analogWrite( Motor_2_0, OUT);
	}else{
		analogWrite( Motor_1_0, 0);
		analogWrite( Motor_2_0, 0);
		analogWrite( Motor_1_1, OUT);
		analogWrite( Motor_2_1, OUT);
	}
}