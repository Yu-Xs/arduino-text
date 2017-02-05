#include <Wire.h>
 
#define SLAVE_ADDRESS 0x12
 
void setup() {
  Wire.begin(SLAVE_ADDRESS);    // join I2C bus as a slave with address 0x12
}
 
void loop() {
}
