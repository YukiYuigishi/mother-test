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

// TODO
/*
  ロリコンの入力ではなく測距輪の入力に変更
*/

PidTest x_Axis(1, 1, 1, 1);
PidTest y_Axis(1, 1, 1, 1);

void loop()
{
  //

  x_Axis.SetTarget(4);
  y_Axis.SetTarget(4);

  double x_output = x_Axis.Run();
  double y_output = y_Axis.Run();
  double volume = sqrt(x_output * x_output + y_output * y_output);
  double theta = atan(y_output / x_output);
  Omni::run(volume, theta);

  //
}
