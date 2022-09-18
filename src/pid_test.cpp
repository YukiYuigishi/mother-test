#include <pid_test.hpp>

#define PID_DEBUG true

PidTest::~PidTest() {}

void PidTest::Pid()
{

  double deviation_sum = 0;
  for (double *i = deviation_; i != deviation_ + 100; i++)
  {
    deviation_sum += *i;
  }
  // Serial.printf("sum:%lf\n", deviation_sum);
  //   output_ = kp_ * deviation_[99] + ki_ * deviation_sum;
  //逆かもしれない
  double ddeviation = deviation_[99] - deviation_[98];
  output_ = kp_ * deviation_[99] + ki_ * deviation_sum + kd_ * ddeviation;
}

void PidTest::Update()
{

  for (double *i = deviation_; i != deviation_ + 99; i++)
  {
    *i = *(i + 1);
  }

  for (int64_t *i = lolicon_tmp_; i != lolicon_tmp_ + 5; i++)
  {
    *i = *(i + 1);
  }
  int64_t lolicon_avg = 0;
  for (int64_t *i = lolicon_tmp_; i != lolicon_tmp_ + 5; i++)
  {
    lolicon_avg += *i;
  }
  lolicon_avg /= 5;

  uint32_t time_deviation = (millis() - time_tmp_);
  time_tmp_ = millis();

  //分解能200
  const double deviation = target_ - Driver::lolicon_speed_value[mdId_];
  //  Serial.printf("time diff: %ld\tdeviation: %lf\n", time_deviation, deviation)  Serial.println( Driver::lolicon_value[mdId_] - lolicon_tmp_);;
  //  Serial.printf("%lf, %lf\n", target_, deviation_);
  deviation_[99] = deviation;
  lolicon_tmp_[4] = Driver::lolicon_speed_value[mdId_];
}

void PidTest::SetTarget(double target)
{

  //分解能 200 x 4
  // target_ = target * 800;

  // motor speed pid;
  target_ = target;
}

double PidTest::Run()
{
  // 偏差の更新
  Update();

  Pid();

#ifdef PID_DEBUG
  //  Serial.printf("target:%lfrad/s\tdeviation:%lfrad/s\tout:%lf\n", target_, deviation_[99], output_);
  // Serial.printf("target:%lf\tdeviation:%lf\touput:%lf\trps:%lf\n", target_, deviation_[99], output_, Driver::lolicon_speed_value[mdId_]);
  Serial.printf("output:%lf\n",output_);
#endif
  return output_;
  // return 10.0;
}

double PidTest::AvgX()
{
  constexpr double cos45 = 1.0 / 1.41421356237309504;
  return (Driver::lolicon_value[5] + Driver::lolicon_value[6] + Driver::lolicon_value[7] + Driver::lolicon_value[8]) * cos45 / 4;
}

double PidTest::AvgY()
{

  // 不具合出るならここ
  constexpr double cos45 = 1.0 / 1.41421356237309504;
  return (Driver::lolicon_value[6] + Driver::lolicon_value[8] + Driver::lolicon_value[5] + Driver::lolicon_value[7]) * cos45 / 4;
}