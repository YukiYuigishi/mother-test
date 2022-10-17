#include <Arduino.h>
#include <Driver.h>
#include <pid_test.hpp>
#include <Ps3Decoder.h>
#include <omni.h>
#include <machine.h>
#define DEBUG 1

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

#if DEBUG
// release
void loop()
{

  static Ps3Decoder::Data controller = {0};

  //コントローラ受信
  if (Serial8.available() > 11)
  {
    uint8_t res[12] = {0};
    Serial8.readBytes(res, 12);
    Ps3Decoder::decode(controller, res);
  }

  // push start button machine enable or disable
  static bool machine_enable[40] = {0};
  for (bool *i = machine_enable; i < machine_enable + 39; i++)
  {
    *i = *(i + 1);
  }

  machine_enable[39] = controller.start;

  // avg select
  double machine_enable_avg = 0;
  for (int i = 0; i < 40; i++)
  {
    machine_enable_avg += machine_enable[i];
  }
  machine_enable_avg /= 40;

  static bool status = false;
  if (machine_enable_avg > 0.6)
  {
    status = !status;
    Driver::mainPowerEnable(status);
  }
  Serial.printf("status %d\n", status);

  // IMUを滑らせる
  static double imu_eular_x[10] = {0};
  for (double *i = imu_eular_x; i != imu_eular_x + 9; i++)
  {
    *i = *(i + 1);
  }

  double imu_eular_x_avg = 0;
  for (double *i = imu_eular_x; i != imu_eular_x + 10; i++)
  {
    imu_eular_x_avg += *i;
  }
  imu_eular_x_avg /= 10;

  imu_eular_x[9] = Driver::IMU_eular_x;

  // Serial.println(Driver::IMU_eular_x);

  // 射出機構
  if (controller.circle)
  {
  //  Machine::arrow(8, 6, 2, 1, 3700, 0, 850);
    Serial.println("punish");
  }

  // 足回り駆動
  int controller_offset = 5;
  constexpr int motor_speed = 500;
  // wip
  /*if ((controller.ry > 110 && controller.rx < controller_offset) || (controller.rx > 110 && controller.ry < controller_offset))
  {
    if (controller.ry > 110)
    {
    }
  }*/
  // offset以下の入力を無効化
  Serial.printf("rx:%d\n", controller.rx);
  Serial.printf("ry:%d\n", -controller.ry);
  if ((abs(controller.rx) > controller_offset) || (abs(controller.ry) > controller_offset))
  {
    double theta = atan2(-controller.ry, controller.rx);
    Serial.printf("theta %lf\n", theta);
    Omni::run(motor_speed, theta);
    /*
    // 第１、第４象限
    if (controller.rx > 0)
    {
      double theta = atan((double)(-controller.ry) / controller.rx);
      Serial.printf("theat %lf\n", theta);
      Omni::run(motor_speed, theta);
    }
    else
    {

      //第２、第３象限
      double theta = atan((double)(-controller.ry) / controller.rx) + M_PI;
      Serial.printf("theat %lf\n", theta);
      Omni::run(motor_speed, theta);
    }
    */
  }
  else
  {
    Omni::run(0, 0);
  }
  //  Omni::run(500, M_PI / 4);
  // Serial.println(controller.start);

  Serial.printf("r1 %d\n", controller.r1);
  Serial.printf("l1 %d\n", controller.l1);
  if (controller.r1)
  {
    Omni::rotation(200, true);
  }
  if (controller.l1)
  {
    Omni::rotation(200, false);
  }

  delay(20);
}

#else
// millsec 5000
void arrow(int winding_motor, int weel_motor, int milli_sec, int servo_winding_arg, int servo_stop_arg)
{
  constexpr int power = 1000;
  //事前の回転
  Driver::MDsetSpeed(winding_motor, power);
  //サーボで継手に接続
  Driver::servoSetAngle(2, servo_winding_arg);
  //巻取り delay
  // millisec 5000
  delay(milli_sec);
  Driver::MDsetSpeed(winding_motor, 90);
  // run weel
  /*
  Serial.println(Driver::SW[1]);
  while (!Driver::SW[1])
  {
    Driver::MDsetSpeed(weel_motor, 260);
  }
  Driver::MDsetSpeed(weel_motor, 0);
  */

  Driver::MDsetSpeed(winding_motor, -100);
  Driver::servoSetAngle(2, servo_stop_arg);

  /*
    while (Driver::SW[1])
    {
      Driver::MDsetSpeed(weel_motor, 300);
    }
    Driver::MDsetSpeed(weel_motor, 0);
    */
}
void loop()
{
  static float imu_eular_x[10] = {0};
  for (float *i = imu_eular_x; i != imu_eular_x + 9; i++)
  {
    *i = *(i + 1);
  }

  imu_eular_x[9] = Driver::IMU_eular_x;

  /*
    double imu_eular_x_avg = 0;
    for (float *i = imu_eular_x; i != imu_eular_x + 10; i++)
    {
      imu_eular_x_avg += *i;
    }
    imu_eular_x_avg /= 10;
    */

  static bool status = false;
  Serial.printf("imu yaw: %lf\n", Driver::IMU_eular_x);
  Serial.printf("status: %d\n", status);
  /*
  if (Serial.available() > 0)
  {
    for (int i = 0; i < Serial.available(); i++)
    {
      Serial.read();
    }

    status = !status;
  }
  if (status)
  {
    Omni::run(30, M_PI/2);
  }
  else
  {
    Omni::run(30, 3.0*M_PI/2);
  }
  */

  // Omni::run(50, 0);

  /*
    constexpr int speed = 300;
    if (Driver::SW[1])
    {
      Driver::MDsetSpeed(6, 0);
      delay(1000);
      while (Driver::SW[1])
      {
        Driver::MDsetSpeed(6, speed);
      }
      //  Driver::MDsetSpeed(8, power);

  */

  /*
  Driver::MDsetSpeed(8, power);
  delay(3000);
  Driver::MDsetSpeed(8, 0);
  while (1)
  {
  }
  */

  //  Driver::MDsetSpeed(9, 500);
  // arrow(8, 6, 3000);
  /*
  for (int i = 0; i < 1700; i += 50)
  {
    Driver::servoSetAngle(2, i);
    Serial.printf("arg: %d\n", i);
    delay(500);
  }
  */
  //       en  stop
  // Right 400, 0
  // left  0, 850
  Serial.println(Driver::SW[1]);

  /*
    // wheel
    Serial.println(Driver::SW[1]);
    while (!Driver::SW[1])
    {
      Driver::MDsetSpeed(6, 300);
    }
    Driver::MDsetSpeed(6, 0);

  */

  //  Driver::MDsetSpeed(9, 1000);
  /*
  while (Driver::SW[1])
  {
    Driver::MDsetSpeed(6, 300);
  }

  while (!Driver::SW[1])
  {
    Driver::MDsetSpeed(6, 300);
  }
  delay(100);
  Driver::MDsetSpeed(6, 0);
  delay(3000);

  while (Driver::SW[1])
  {
    Driver::MDsetSpeed(6, 250);
  }
  Driver::MDsetSpeed(6, 0);

*/
  static Ps3Decoder::Data controller = {0};

  //コントローラ受信
  /*
  if (Serial8.available() > 11)
  {
    uint8_t res[12] = {0};
    Serial8.readBytes(res, 12);
    controller = Ps3Decoder::decode(res);
  }

  Serial.printf("circle %d\n", controller.circle);
  if (controller.circle)
  {
    Machine::arrow(8, 6, 2, 1, 3700, 0, 850);
  }

  Serial.printf("cross %d\n", controller.r1);
  if (controller.r1)
  {
    Machine::weel();
    delay(500);
  }
  */
  // Driver::MDsetSpeed(9, 950);

  //  仰角制御 WIP
  // const int offset_gyou = 6;
  /*
  if (abs(controller.ry) > offset_gyou)
  {
    if (controller.ry > offset_gyou)
    {

      Driver::MDsetSpeed(9, -950);
    }
    else
    {
      Driver::MDsetSpeed(9, 950);
    }
  }
  */

  /*
    Serial.printf("l1 %d\n", controller.l1);
    if (controller.l1)
    {
      delay(300);

      while (!controller.l1)
      {
        if (Serial8.available() > 11)
        {
          uint8_t res[12] = {0};
          Serial8.readBytes(res, 12);
          controller = Ps3Decoder::decode(res);
        }
        Machine::weel2();
      }
    }
    */
  //仰角上げ
  /*
  while (!Driver::SW[1])
  {
    Driver::MDsetSpeed(6, 300);
    if (Driver::SW[1])
      break;
  }

  Serial.println("stop");
  Driver::MDsetSpeed(6, 0);
  // 動かす
  while (Driver::SW[1])
  {
    Driver::MDsetSpeed(6, 250);
    if (!Driver::SW[1])
      break;
  }
  delay(300);
  Driver::MDsetSpeed(6, 0);
  */

  delay(20);
}
#endif
