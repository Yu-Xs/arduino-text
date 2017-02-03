#include <Stepper.h>

#define steps 2048
#define pin1 6
#define pin2 7
#define pin3 8
#define pin4 9

Stepper stepper(steps, pin1, pin2, pin3, pin4 );

void setup() {
  stepper.setSpeed(10);

  Serial.begin(9600);
}

void loop() {
  Serial.println("status");
  stepper.step(2048);
}
