#include <Arduino.h>
#include <Driver.h>
#include <pid_test.hpp>
#include <omni.h>
void setup()
{
  // put your setup code here, to run once:
  delay(1000);
  Driver::projectX_Init();
  //  Pid::Init();
  //  Pid::target[0] = 10 * 2 * M_PI;
  //  Pid::target[0] = 20;

  Serial.begin(9600);
}

PidTest moter1(1, 1, 1, 1);
void loop()
{
  //  Driver::MDsetSpeed(1, 128);

  //  Pid::Run(Driver::MDsetSpeed);
  //  Serial.printf("target:%d output: %d deviation %d", Pid::target[0], Pid::output[0], Driver::lolicon_value[1]);
  //  Driver::MDsetSpeed(1, 80);
  //  Serial.printf("lori: %d devi: %lf\n",Driver::lolicon_value[1], Pid::deviation[99]);
  //  int32_t loli_diff = (Driver::lolicon_value[1] > 0) ? (Driver::lolicon_value[1] - tmp) : (INT16_MAX - Driver::lolicon_value[1] + INT16_MAX - (tmp > 0) ? tmp : -tmp);
  //  Serial.println(loli_diff);
  // tmp = Driver::lolicon_value[1];
  //  Serial.printf("%d :\t%d\n", micros() / 1000, Driver::lolicon_value[1]);

  // moter1.SetTarget(3);
  //  Driver::MDsetSpeed(1, (int)(moter1.Run()));
  // Omni::run(20, 0);
  // delay(700);
  /*
  Omni::run(20, 90);
  delay(1000);
  // Omni::run(20, 180);
  // delay(700);
  Omni::run(0, 90);
  delay(10);
  Omni::run(20, 270);
  delay(1000);
  Omni::run(0, 270);
  delay(10);
  // Driver::MDsetSpeed(4, 20);
  // delay(50);
*/
  Driver::MDsetSpeed(4, 20);
}
