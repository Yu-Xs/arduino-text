#include <Stepper.h>

#define steps 2049
#define pin1 7
#define pin2 8
#define pin3 9
#define pin4 10

Stepper stepper( steps, pin1, pin2, pin3, pin4);

void setup() {
  stepper.setSpeed(5);

  Serial.begin(9600);
}

void loop() {
  Serial.println("status");
  stepper.step(2049);
}
