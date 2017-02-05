#include <SoftwareSerial.h>
#include <Wire.h>

const int RX = 11;
const int TX = 10;
SoftwareSerial BT( TX, RX);

void setup(){
  Serial.begin(9600);
  BT.begin(9600);
}

char val;
int stus = 4;

void loop(){

  if(BT.available()){
    val = BT.read();
    if(val == 7){
      BT.write(val);
      BT.write(stus);
    }else if(val == 1){
      stus = 1;
    }else if(val == 2){
      stus = 2;
    }else if(val == 3){
      stus = 3;
    }else if(val == 4){
      stus = 4;
    }
  }


}