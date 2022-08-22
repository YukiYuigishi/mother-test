#include <Arduino.h>
#include <Driver.h>
#include <pid.h>
void setup()
{
  // put your setup code here, to run once:
  delay(1000);
  Driver::projectX_Init();
  //  Pid::Init();

  Serial.begin(9600);
}

void loop()
{
  //  Driver::MDsetSpeed(1, 128);

  Pid::Run(Driver::MDsetSpeed);
  Serial.println(Pid::output[0]);

  delay(100);
}