#include <RobotController.h>

void setup()
{
  RobotBegin();
  Serial.begin(9600);
  calibrateAccelGyros();
}

void loop()
{
  int16_t gx,gy,gz;
  getGyros(&gx,&gy,&gz);
  Serial.print("GX: ");
  Serial.print(gx);
  Serial.print("GY: ");
  Serial.print(gy);
  Serial.print("GZ: ");
  Serial.print(gz);
  Serial.println();
  doDelay(1000);
}
