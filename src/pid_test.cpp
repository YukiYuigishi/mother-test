#include <pid_test.hpp>

#define PID_DEBUG 1;

PidTest::~PidTest() {}

void PidTest::Pid()
{

  static uint32_t time_old = 0;

  uint32_t time_diff = millis() - time_old;
  double deviation_sum = 0;
  for (double *i = deviation_; i != deviation_ + 100; i++)
  {
    deviation_sum += *i;
  }

  double deviation_differential = (double)deviation_[99] / time_diff;

  output_ = kp_ * deviation_[99] + ki_ * deviation_sum + kp_ * deviation_differential;
}

void PidTest::Update()
{

  for (double *i = deviation_; i != deviation_ + 99; i++)
  {
    *i = *(i + 1);
  }

  // devi /  time * 2*M_PI
  //  deviation_[99] = (double)(Driver::lolicon_value[mdId_] - lolicon_tmp_) / 2048 / ((double)(micros() - time_tmp_) / 1000) * 2.0 * M_PI - target_;

  //  constexpr int bunkainou = 2048 * 4;
  //ここをいじる
  //  deviation_[99] = (double)(Driver::lolicon_value[mdId_] - lolicon_tmp_) * mpi2 / bunkainou / ((micros() - time_tmp_) / 1000) - target_;

  deviation_[99] = target_ - Driver::lolicon_speed_value[mdId_];
  time_tmp_ = micros();
  lolicon_tmp_ = Driver::lolicon_value[mdId_];
}

void PidTest::SetTarget(int rad)
{ // target_ = rps * 2 * M_PI;
  target_ = rad;
}

double PidTest::Run()
{
  // 偏差の更新
  Update();

  Pid();

#ifdef PID_DEBUG
  //  Serial.printf("target:%lfrad/s\tnow:%lfrad/s\traw loli:%ld\tout:%lf\n", target_, deviation_[99], Driver::lolicon_value[1], output_);
  Serial.print("motor: ");
  Serial.print(mdId_);
  Serial.print("\ttarget: ");
  Serial.print(target_);
  Serial.print("rad/s");
  Serial.print("\tnow ");
  Serial.print(Driver::lolicon_speed_value[mdId_]);
  Serial.print("\tdevi: ");
  Serial.print(deviation_[99]);
  Serial.print("rad/s ");
  Serial.print("\tpwm ");
  Serial.println(output_);

  Serial.printf("kp: %lf ki: %lf kd: %lf\n", kp_, ki_, kd_);

#endif
  return output_;
  // return 10.0;
}
