#include <pid_test.hpp>

#define PID_DEBUG 1;

PidTest::~PidTest() {}

void PidTest::Pid()
{

  double deviation_sum = 0;
  for (double *i = deviation_; i != deviation_ + 100; i++)
  {
    deviation_sum += *i;
  }
  Serial.printf("sum:%lf\n", deviation_sum);
  //  output_ = kp_ * deviation_[99] + ki_ * deviation_sum;
  output_ = kp_ * deviation_[99];
}

void PidTest::Update()
{

  for (double *i = deviation_; i != deviation_ + 99; i++)
  {
    *i = *(i + 1);
  }

  // devi /  time * 2*M_PI
  //  deviation_[99] = (double)(Driver::lolicon_value[mdId_] - lolicon_tmp_) / 2048 / ((double)(micros() - time_tmp_) / 1000) * 2.0 * M_PI - target_;

  constexpr int bunkainou = 2048 * 4;
  constexpr double mpi2 = 2 * M_PI;
  //ここをいじる
  deviation_[99] = (double)(Driver::lolicon_value[mdId_] - lolicon_tmp_) * mpi2 / bunkainou / ((micros() - time_tmp_) / 1000) - target_;

  time_tmp_ = micros();
  lolicon_tmp_ = Driver::lolicon_value[mdId_];
}

void PidTest::SetTarget(int rps) { target_ = rps * 2 * M_PI; }

double PidTest::Run()
{
  // 偏差の更新
  Update();

  Pid();

#ifdef PID_DEBUG
  Serial.printf("target:%lfrad/s\tnow:%lfrad/s\traw loli:%ld\tout:%lf\n",
                target_, deviation_[99], Driver::lolicon_value[1], output_);
#endif
  return output_;
  // return 10.0;
}
