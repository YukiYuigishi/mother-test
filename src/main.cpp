#include <Arduino.h>
#include <Driver.h>
#include <pid.h>
void setup()
{
  // put your setup code here, to run once:
  delay(1000);
  Driver::projectX_Init();
  //  Pid::Init();
  Pid::target[0] = 80;

  Serial.begin(9600);
}

short tmp = 0;
void loop()
{
  //  Driver::MDsetSpeed(1, 128);

  Pid::Run(Driver::MDsetSpeed);
  //  Serial.printf("target:%d output: %d deviation %d", Pid::target[0], Pid::output[0], Driver::lolicon_value[1]);
  //  Driver::MDsetSpeed(1, 80);
  //  Serial.printf("lori: %d devi: %lf\n",Driver::lolicon_value[1], Pid::deviation[99]);
  int32_t loli_diff = (Driver::lolicon_value[1] > 0) ? (Driver::lolicon_value[1] - tmp) : (INT16_MAX - Driver::lolicon_value[1] + INT16_MAX - (tmp > 0) ? tmp : -tmp);
  Serial.println(loli_diff);
  tmp = Driver::lolicon_value[1];

  delay(50);
}
