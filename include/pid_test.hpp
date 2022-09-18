#pragma once
#include <Arduino.h>
#include <Driver.h>

class PidTest
{
private:
  /* data */

  //モタドラのID
  const int mdId_;
  //オドメータのID
  // IDと ID + 2 のオドメータがペアとなる
  //  const int odometerId_;
  double (PidTest::*OdometerFunc)() = nullptr;

  int64_t lolicon_tmp_[5] = {0};
  uint32_t time_tmp_ = 0;

  bool skip_ = false;
  //現在の出力値 x(t) 現在の
  // deviation_[99]
  // double nowValue_;
  // 偏差 e(t)
  double deviation_[100] = {0};
  // 入力値 u(t) PWM
  double output_ = 0;
  // 目標値 r(t)
  double target_ = 0;

  const double ki_ = 1;
  const double kp_ = 1;
  const double kd_ = 1;

  double AvgX();
  double AvgY();

  void Pid();
  void Update();
  inline double ReadOdometer()
  {
    return (this->*OdometerFunc)();
  };

public:
  // axis: オドメータの軸 'x' or 'y'
  PidTest(const int mdId_, const char axis, const double ki_, const double kp_, const double kd_)
      : mdId_(mdId_), ki_(ki_), kp_(kp_), kd_(kd_)
  {
    if (axis == 'x')
    {
      OdometerFunc = &PidTest::AvgX;
    }
    else if (axis == 'y')
    {
      OdometerFunc = &PidTest::AvgY;
    }
  }
  ~PidTest();
  double Run();
  //座標制御
  void SetTarget(double target);
};
