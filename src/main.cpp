#include <Arduino.h>
#include <Driver.h>
#include <Ps3Decoder.h>
#include <machine.hpp>
#include <omni.hpp>
#include <pid_test.hpp>
#include <config.hpp>
#include <array>
#define DEBUG 1
constexpr bool undercarriage_en = 0;

// void Machine::canonnAngleSet(MachineConfig::Canonn::SELECT select, double angle);

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
  Serial.printf("足回り %d\n", (bool)undercarriage_en);
  //  Machine::statusInit(1);
  // 砲 init

  Driver::MDsetSpeed(5, -450);

  while (!Driver::SW[1])
  {
  }
  Driver::lolicon_value[8] = 0;
  Driver::MDsetSpeed(5, 400);
  delay(100);
  Driver::MDsetSpeed(5, 0);
}

#if DEBUG
// release
void loop()
{
  // config
  constexpr bool undercarriage_en = 0;

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

  /*
    // avg select
    double machine_enable_avg = 0; for (int i = 0; i < 40; i++) {
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

  */
  // IMUを滑らせる
  static double imu_eular_x[10] = {0};
  for (int i = 0; i < 9; i++)
  {
    imu_eular_x[i] = imu_eular_x[i + 1];
  }

  double imu_eular_x_avg = 0;
  for (int i = 0; i < 10; i++)
  {
    imu_eular_x_avg += imu_eular_x[i];
  }
  imu_eular_x_avg /= 10;

  imu_eular_x[9] = Driver::IMU_eular_x;
  //射出機構//////////////////////////////

  //仰角上げ
  if (controller.up)
  {
    Serial.println("up");
    Machine::canonnAngleSet(MachineConfig::Canonn::RIGHT, -153);
    Driver::MDsetSpeed(5, 350);
    /*
    if (Driver::lolicon_value[8] < -168)
    {
      Driver::MDsetSpeed(5, 0);
      Driver::segDriver(99);
    }
    */
  }
  //仰角下げ
  else if (controller.down)
  {
    Serial.println("down");
    Driver::MDsetSpeed(5, -430);
  }
  //仰角停止
  else
  {
    Driver::MDsetSpeed(5, 0);
  }

  Serial.println(Driver::lolicon_value[8]);

  Machine::checkCanonn();
  /*
    if (Driver::SW[1])
    {
      Driver::MDsetSpeed(5, 300);
      delay(100);
      Driver::MDsetSpeed(5, 0);
    }
  */

  // Serial.println(Driver::IMU_eular_x);

  // 射出
  if (controller.circle)
  {
    Serial.println("punish");
    Machine::arrow(6, 7, 1, 2, 3700, 0, 850);
    Serial.println("done");
    Serial8.flush();
  }
  ////////////////////////////////////

  // 足回り駆動///////////////////////////////////
  // offset以下の入力を無効化
  if (undercarriage_en)
  {

    int controller_offset = 5;
    constexpr int motor_speed = 30;
    Serial.printf("rx:%d\n", controller.rx);
    Serial.printf("ry:%d\n", -controller.ry);
    if ((abs(controller.rx) > controller_offset) ||
        (abs(controller.ry) > controller_offset))
    {
      //正常系
      // double theta = atan2(-controller.ry, controller.rx);
      //上下左右反転系
      double theta = atan2(controller.ry, -controller.rx);
      Serial.printf("theta %lf\n", theta);
      Omni::run(motor_speed, theta);
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
  }
  ////////////////////////////////////////////////

  delay(20);
}

#else
// millsec 5000
void arrow(int winding_motor, int weel_motor, int milli_sec,
           int servo_winding_arg, int servo_stop_arg)
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

// PidTest motor(MOTOR_RIGHT, 27.0, 1, 0);

void loop()
{
  static float imu_eular_x[10] = {0};
  for (float *i = imu_eular_x; i != imu_eular_x + 9; i++)
  {
    *i = *(i + 1);
  }

  imu_eular_x[9] = Driver::IMU_eular_x;
  //  double target_speed = 5 * TWO_PI;
  // motor.SetTarget(target_speed);

  // Driver::MDsetSpeed(MOTOR_RIGHT, motor.Run());
  // Serial.println(target_speed);
  // Serial.println(Driver::lolicon_speed_value[MOTOR_RIGHT]);

  int motor_speed = 500;
  //  Omni::run(motor_speed, M_PI / 2);
  /*
  Driver::MDsetSpeed(1, 200);
  Driver::MDsetSpeed(2, 200);
  Driver::MDsetSpeed(3, 200);
  Driver::MDsetSpeed(4, 200);

  Serial.printf("1 %d\n", Driver::lolicon_value[1]);
  Serial.printf("2 %d\n", Driver::lolicon_value[2]);
  Serial.printf("3 %d\n", Driver::lolicon_value[3]);
  Serial.printf("4 %d\n", Driver::lolicon_value[4]);
  */

  // PID test

  /*
    double imu_eular_x_avg = 0;
    for (float *i = imu_eular_x; i != imu_eular_x + 10; i++)
    {
      imu_eular_x_avg += *i;
    }
    imu_eular_x_avg /= 10;
    */

  // yaw軸の表示
  // Serial.printf("imu yaw: %lf\n", Driver::IMU_eular_x);
  // Serial.printf("status: %d\n", status);
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
  // Serial.println(Driver::SW[1]);

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
  if (Serial8.available() > 11)
  {
    uint8_t res[12] = {0};
    Serial8.readBytes(res, 12);
    Ps3Decoder::decode(controller, res);
  }
  /*

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

  //  Serial.println(Driver::lolicon_value[8]);

  if (controller.circle)
  {
    Machine::arrow(6, 7, 1, 2, 3400, 0, 850);
    Serial8.flush();
  }

  //仰角上げ
  if (controller.up)
  {
    Serial.println("up");
    Driver::MDsetSpeed(5, 350);
    if (Driver::lolicon_value[8] < -168)
    {
      Driver::MDsetSpeed(5, 0);
      Driver::segDriver(99);
    }
  }
  //仰角下げ
  else if (controller.down)
  {
    Serial.println("down");
    Driver::MDsetSpeed(5, -430);
  }
  //仰角停止
  else
  {
    Driver::MDsetSpeed(5, 0);
  }

  Serial.println(Driver::lolicon_value[8]);

  if (Driver::SW[1])
  {
    Driver::MDsetSpeed(5, 300);
    delay(100);
    Driver::MDsetSpeed(5, 0);
  }
  //仰角下げ 右
  //  Driver::MDsetSpeed(5, -450);
  //仰角上げ
  // Driver::MDsetSpeed(5, 300);
  /*

    if (Driver::SW[1])
    {
      Driver::MDsetSpeed(5, 0);
      Driver::MDsetSpeed(5, 400);
      delay(100);
      Driver::MDsetSpeed(5, 0);
      while (1)
      {
        Driver::segDriver(99);
      }
    };
    */
  //  Machine::arrow(5, 0, 2, 0, 4000, 0, 850);

  //  Serial.println(Driver::lolicon_value[10]);
  static uint8_t status[100] = {0};
  int status_avg = 0;
  for (int i = 0; i < 98; i++)
  {
    status[i] = status[i + 1];
    status_avg += status[i];
  }

  /*
    if (Serial8.available())
    {
      Serial.printf("%d\n", Serial8.read());
      Serial8.flush();
    }
    */

  //  if (0 == Machine::checkStatus())
  //  {
  //    Serial.println("stop");
  //    Driver::safeMode(11);
  //  }
  //  else
  //  {
  //    Serial.println("start");
  //  }

  // uint8_t tmp = Machine::checkStatus();
  // Driver::safeMode(10);
  // Serial.println("panish");
  //  Machine::arrow(6, 7, 1, 2, 4200, 0, 850);
  // Serial.println("test");

  constexpr int controller_offset = 5;
  bool foots = 1;
  Serial.printf("rx:%d\n", controller.rx);
  Serial.printf("ry:%d\n", -controller.ry);
  if (foots)
  {
    if ((abs(controller.rx) > controller_offset) ||
        (abs(controller.ry) > controller_offset))
    {
      //正常系
      // double theta = atan2(-controller.ry, controller.rx);
      //上下左右反転系
      double theta = atan2(controller.ry, -controller.rx);
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
  }
  //  Driver::MDsetSpeed(3, 300);
  // Driver::MDsetSpeed(1, 300);
  // Driver::MDsetSpeed(2, 300);
  // Driver::MDsetSpeed(3, 300);
  // Driver::MDsetSpeed(4, 300);
  delay(20);
}
#endif
