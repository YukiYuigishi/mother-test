#include <Arduino.h>
#include <Driver.h>
#include <pid_test.hpp>
#include <Ps3Decoder.h>
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
  Serial.println("やあ、アリアルさんだよ。");
}

PidTest moter1(1, 1, 1, 1);

void loop()
{

  static Ps3Decoder::Data controller = {0};

  if (Serial8.available() > 11)
  {
    uint8_t res[12] = {0};
    Serial8.readBytes(res, 12);
    controller = Ps3Decoder::decode(res);
  }

  // read odometor
  static float imu_eular_x[10] = {0};
  for (float *i = imu_eular_x; i != imu_eular_x + 9; i++)
  {

    *i = *(i + 1);
  }

  double imu_eular_x_avg = 0;
  for (float *i = imu_eular_x; i != imu_eular_x + 10; i++)
  {
    imu_eular_x_avg += *i;
  }
  imu_eular_x_avg /= 10;

  imu_eular_x[9] = Driver::IMU_eular_x;

  // Serial.println(Driver::IMU_eular_x);

  /*

    static double odometor_theta = 0;
    static long long int old_time = 0;
    static long long int old_loli = 0;
    */
  Serial.println("------------");
  /*
  Serial.println(Driver::lolicon_value[5]);
  Serial.println(Driver::lolicon_value[6]);
  Serial.println(Driver::lolicon_value[7]);
  Serial.println(Driver::lolicon_value[8]);
  */
  static uint32_t old_time = 0;
  old_time = millis();
  /*
  odometor_theta += Driver::lolicon_speed_value[5] * (double)d_time / 1000.;

  const int d_loli = Driver::lolicon_value[5] - old_loli; old_loli = Driver::lolicon_value[5];

  const double theta = (double)Driver::lolicon_value[5] * 2.0 * M_PI / (500.0);
  const double distance = theta * 24;

  double cos45 = cos(M_PI / 4.);
  int startOdometer = 5;
  */
  Omni::FieldCoordinate field = Omni::getFieldCoordinateSpeed(Driver::IMU_eular_x);
  Serial.printf("rx: %d\n", controller.rx);
  Serial.printf("ry: %d\n", controller.ry);
  Serial.println(field.X);
  Serial.println(field.Y);

  // 足回り駆動
  constexpr int controller_offset = 5;
  constexpr int motor_speed = 1500;
  if ((controller.rx > controller_offset || controller.rx < -controller_offset) && (controller.ry > controller_offset || controller.ry < controller_offset))
  {
    if (controller.rx > 0)
    {
      double theta = atan((double)-controller.ry / controller.rx);
      Serial.printf("theat %lf\n", theta);
      Omni::run(motor_speed, theta);
    }
    else
    {
      double theta = atan((double)-controller.ry / controller.rx) + M_PI;
      Serial.printf("theat %lf\n", theta);
      Omni::run(motor_speed, theta);
    }
  }
  else
  {
    Omni::run(0, 0);
  }

  /*

  static double odo = 0;
  int startOdometer = 5;
  double sin45 = sin(M_PI / 4.0);
  // マシン座標系に変換
  double odo0 = (-Driver::lolicon_speed_value[startOdometer]) * sin45;
  double odo1 = (Driver::lolicon_speed_value[startOdometer + 1]) * sin45;
  double odo2 = (Driver::lolicon_speed_value[startOdometer + 2]) * sin45;
  double odo3 = (-Driver::lolicon_speed_value[startOdometer + 3]) * sin45;

  // 平均値を積分
  odo += (odo0 + odo1 + odo2 + odo3) * (double)d_time / (4. * 1000.);

  // デバッグ
  Serial.printf("time d %d\n", d_time);
  Serial.printf("odo0 raw %d\n", -Driver::lolicon_value[startOdometer]);
  Serial.printf("odo1 raw %d\n", Driver::lolicon_value[startOdometer + 1]);
  Serial.printf("odo2 raw %d\n", Driver::lolicon_value[startOdometer + 2]);
  Serial.printf("odo3 raw %d\n", -Driver::lolicon_value[startOdometer + 3]);

  Serial.printf("odo0 %lf\n", odo0);
  Serial.printf("odo1 %lf\n", odo1);
  Serial.printf("odo2 %lf\n", odo2);
  Serial.printf("odo3 %lf\n", odo3);
  Serial.printf("avg odo %lf\n", odo);
  Serial.printf("distance %lf\n", odo * 24. * 2);

*/

  delay(20);
}
