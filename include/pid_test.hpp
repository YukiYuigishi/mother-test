#pragma once
#include <Arduino.h>
#include <Driver.h>
class PidTest
{
private:
    /* data */

    const int mdId_;

    int64_t lolicon_tmp_ = 0;
    int32_t time_tmp_ = 0;
    //現在の出力値 x(t) 現在の
    // deviation_[99]
    // double nowValue_;
    // 偏差 e(t)
    double deviation_[100] = {0};
    // 入力値 u(t) PWM
    double output_;
    // 目標値 r(t)
    double target_;

    const double ki_ = 1;
    const double kp_ = 1;
    const double kd_ = 1;

    void Pid();
    void Update();

public:
    PidTest(const int mdId_, const double ki_, const double kp_, const double kd_) : mdId_(mdId_), ki_(ki_), kp_(kp_), kd_(kd_) {}
    ~PidTest();
    double Run();
    void SetTarget(int rotation);
};