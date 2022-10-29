#include <config.hpp>
#include <Arduino.h>
#include <Driver.h>
#include <Ps3Decoder.h>
//#include <Ps3DecoderNa.h>
#include <machine.hpp>
#include <omni.hpp>
#include <pid_test.hpp>
#include <array>
#define DEBUG 0
constexpr bool undercarriage_en = 1;
constexpr bool canonn_en = 1;
// 0 is dualshock3 bluetooth, 1 is lazurite controller for Nachan protocol, 2 is lazurite controller for Yuki Protocol
constexpr int controller_en = 0;

// void Machine::canonnAngleSet(MachineConfig::Canonn::SELECT select, double angle);

void setup()
{
  // put your setup code here, to run once:
  delay(1000);
  Driver::projectX_Init();
  //  Pid::Init();
  //  Pid::target[0] = 10 * 2 * M_PI;
  //  Pid::target[0] = 20;

  Serial.begin(115200);
  Serial.println("やあ、アリアルさんだよ。");
  Serial.printf("足回り %d\n", (int)undercarriage_en);
  Serial.printf("射出機構 %d\n", (int)canonn_en);
  //  Machine::statusInit(1);
  // 砲 init

  if (canonn_en)
  {
    // Driver::MDsetSpeed(MachineConfig::Canonn::ANGLE_MOTOR_LEFT, 400);
    /*
    Driver::MDsetSpeed(5, -450);

    while (!Driver::SW[1])
    {
    }
    Driver::lolicon_value[8] = 0;
    Driver::MDsetSpeed(5, 400);
    delay(150);
    Driver::MDsetSpeed(5, 0);
    */

    Machine::cannonnInit();
    Driver::servoSetAngle(MachineConfig::Canonn::WHEEL_LOCK_SERVO_RIGHT, 400);
  }
  //固定
  //  Driver::servoSetAngle(MachineConfig::Canonn::WHEEL_LOCK_SERVO_RIGHT, 1700);
}

#if DEBUG
// release
void loop()
{
  // 主砲pid
  // Machine::canonnAngleSet(MachineConfig::Canonn::RIGHT, Machine::angle_canonn_right);
  // Serial.printf("angle %d\n", Machine::angle_canonn_right);
  // static Ps3Decoder::Data controller = {0};
  static Ps3Decoder::Data controller = {0};
  // dualshock3 bluetooth
  if (controller_en == 0)
  {
    //コントローラ受信
    if (Serial7.available() > 12)
    {
      uint8_t res[12] = {0};
      Serial7.readBytes(res, 12);
      Ps3Decoder::decode(controller, res);
    }
  }

  switch (Machine::canonn_status)
  {
  case MachineConfig::Canonn::RIGHT:
  {
    Driver::illumination(0x0, 0xFF, 0x0, 0);
    Serial.println("Right Canonn");
    break;
  }
  case MachineConfig::Canonn::LEFT:
  {
    Driver::illumination(0xFF, 0x0, 0x0, 0);
    Serial.println("Left Canonn");
    break;
  }
  default:
    break;
  }

  // nachan protocol

  // push start button machine enable or disable

  //射出機構//////////////////////////////
  if (canonn_en)
  {
    if (controller.right)
    {
      Machine::canonn_status = MachineConfig::Canonn::RIGHT;
      Serial.println("canonn change to right");
    }
    if (controller.left)
    {
      Machine::canonn_status = MachineConfig::Canonn::LEFT;
      Serial.println("canonn change to left");
    }

    //仰角上げ
    if (controller.up)
    {
      //  Machine::canonnAngleSet(MachineConfig::Canonn::RIGHT, -153);
      //      Machine::angle_canonn_right += 1;
      switch (Machine::canonn_status)
      {
      case MachineConfig::Canonn::RIGHT:
        /* code */
        Driver::MDsetSpeed(MachineConfig::Canonn::ANGLE_MOTOR_RIGHT, 400);
        Serial.println("Right up");
        break;
      case MachineConfig::Canonn::LEFT:
        Driver::MDsetSpeed(MachineConfig::Canonn::ANGLE_MOTOR_LEFT, 400);
        Serial.println("Left up");
        break;
      }
      Machine::checkCanonnBack();
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
      //      Machine::angle_canonn_left -= 1;
      //      Driver::MDsetSpeed(MachineConfig::Canonn::ANGLE_MOTOR_RIGHT, -400);
      switch (Machine::canonn_status)
      {
      case MachineConfig::Canonn::RIGHT:
        /* code */
        Driver::MDsetSpeed(MachineConfig::Canonn::ANGLE_MOTOR_RIGHT, -400);
        Serial.println("Right down");
        break;
      case MachineConfig::Canonn::LEFT:
        Driver::MDsetSpeed(MachineConfig::Canonn::ANGLE_MOTOR_LEFT, -400);
        Serial.println("Left down");
        break;
      }
      Machine::checkCanonnFront();
    }
    else if (controller.cross)
    {
      Serial.println("cross");
      Machine::canonnAngleSet(MachineConfig::Canonn::RIGHT, -153);
    }
    //仰角停止
    else
    {
      Driver::MDsetSpeed(MachineConfig::Canonn::ANGLE_MOTOR_RIGHT, 0);
      Driver::MDsetSpeed(MachineConfig::Canonn::ANGLE_MOTOR_LEFT, 0);
    }
    // Machine::canonnAngleSet(MachineConfig::Canonn::RIGHT, -153);

    Serial.println(Driver::lolicon_value[8]);

    // Machine::checkCanonnFront();

    // Serial.println(Driver::IMU_eular_x);

    // 射出

    Serial.printf("circle %d\n", controller.circle);
    // きーぼーど　入力
    /*
    if (Serial.available() > 0)
    {
      if (Serial.read() == '\n')
      {
        Serial.println("fire");
        /// Machine::arrow(6, 7, 1, 2, 4500, 0, 850);
        Machine::canonnShot(MachineConfig::Canonn::RIGHT);
        Serial8.flush();
        Serial.println("done");
      }
    }
    */
    //押しすぎ対処
    static int circle_check = 0;
    if (controller.circle)
    {
      circle_check += 1;
    }
    if (circle_check > 3)
    {
      // delay(1000);
      Serial.println("fire");
      // Machine::arrow(6, 7, 1, 2, 4300, 0, 850);
      Machine::canonnShot(Machine::canonn_status);
      Serial8.flush();
      circle_check = 0;
      Serial.println("done");
    }
  }
  ////////////////////////////////////

  // 足回り駆動///////////////////////////////////
  // offset以下の入力を無効化

  // constexpr int arrow_key = 0;
  if (undercarriage_en)
  {

    int controller_offset = 11;
    int motor_speed = sqrt(pow(controller.rx, 2) + pow(controller.ry, 2)) * 1.5 / 6;
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
      Omni::rotation(250, true);
    }
    if (controller.l1)
    {
      Omni::rotation(250, false);
    }
    /*
    if (controller.right)
    {
      Omni::run(500, 0);
    }
    else if (controller.up)
    {
      Omni::run(500, M_PI / 2);
    }
    else if (controller.left)
    {

      Omni::run(500, M_PI);
    }
    else if (controller.down)
    {
      Omni::run(500, M_PI / 2 * 3);
    }
    else
    {

      Omni::run(0, 0);
    }
    */
  }
  delay(20);
}
////////////////////////////////////////////////

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
  /*
  static float imu_eular_x[10] = {0};
  for (float *i = imu_eular_x; i != imu_eular_x + 9; i++)
  {
    *i = *(i + 1);
  }

  imu_eular_x[9] = Driver::IMU_eular_x;
  */
  //  double target_speed = 5 * TWO_PI;
  // motor.SetTarget(target_speed);

  // Driver::MDsetSpeed(MOTOR_RIGHT, motor.Run());
  // Serial.println(target_speed);
  // Serial.println(Driver::lolicon_speed_value[MOTOR_RIGHT]);

  // int motor_speed = 500;
  //   Omni::run(motor_speed, M_PI / 2);
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
  //  static Ps3Decoder::Data controller = {0};
  //
  //  //コントローラ受信
  //  if (Serial8.available() > 11)
  //  {
  //    uint8_t res[12] = {0};
  //    Serial8.readBytes(res, 12);
  //    Ps3Decoder::decode(controller, res);
  //  }
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

  //  Driver::MDsetSpeed(3, 300);
  // Driver::MDsetSpeed(1, 300);
  // Driver::MDsetSpeed(2, 300);
  // Driver::MDsetSpeed(3, 300);
  // Driver::MDsetSpeed(4, 300);
  // 右 330  => 850

  //  Driver::servoSetAngle(1, 0);
  // Serial.printf("sw2 %d\n", Driver::SW[2]);
  /*
  while (!Driver::SW[2])
  {
    Driver::MDsetSpeed(MachineConfig::Canonn::WHEEL_MOTOR_RIGHT, 300);
  }
  Driver::MDsetSpeed(MachineConfig::Canonn::WHEEL_MOTOR_RIGHT, 0);

  //固定
  Driver::servoSetAngle(MachineConfig::Canonn::WHEEL_LOCK_SERVO_RIGHT, 1700);
  delay(500);
  Driver::servoSetAngle(MachineConfig::Canonn::WHEEL_LOCK_SERVO_RIGHT, 330);
  while (Driver::SW[2])
  {
    Driver::MDsetSpeed(MachineConfig::Canonn::WHEEL_MOTOR_RIGHT, 300);
  }
  Driver::MDsetSpeed(MachineConfig::Canonn::WHEEL_MOTOR_RIGHT, 0);
  Driver::servoSetAngle(MachineConfig::Canonn::WHEEL_LOCK_SERVO_RIGHT, 330);
  delay(500);
  Driver::servoSetAngle(MachineConfig::Canonn::WHEEL_LOCK_SERVO_RIGHT, 850);
  delay(500);
  Driver::servoSetAngle(MachineConfig::Canonn::WHEEL_LOCK_SERVO_RIGHT, 1700);
  delay(500);
  */

  //  Driver::MDsetSpeed(MachineConfig::Canonn::WINDING_MOTOR_RIGHT, 5000); //巻取り delay

  /*
    int data[2] = {0};
    Machine::readI2CLoli(data);
    Serial.printf("%d\n", data[0]);
    Serial.printf("%d\n", data[1]);
    */

  // 装填機構
  /* i2c loli read
  static uint32_t old_read = 0;
  uint32_t read1 = 0;
  uint32_t read2 = 0;
  noInterrupts();
  FlexiTimer2::stop();
  Wire.requestFrom(0x0b, 8);
  if (Wire.available() > 0)
  {
    Serial.println("test");
    read1 = Wire.read();
    read1 += Wire.read() << 8;
    read1 += Wire.read() << 16;
    read1 += Wire.read() << 24;
    read2 = Wire.read();
    read2 += Wire.read() << 8;
    read2 += Wire.read() << 16;
    read2 += Wire.read() << 24;
  }
  interrupts();
  FlexiTimer2::start();
  Serial.printf("read1 %d\n", read1);
  Serial.printf("read2 %d\n", read2);
  */
  // if (read2 != old_read)
  // {
  ///.println("stop");
  //   Driver::MDsetSpeed(MachineConfig::Canonn::WHEEL_MOTOR_RIGHT, 0);
  //   Driver::servoSetAngle(MachineConfig::Canonn::WHEEL_LOCK_SERVO_RIGHT, 900);
  //   delay(5000);
  //   Driver::servoSetAngle(MachineConfig::Canonn::WHEEL_LOCK_SERVO_RIGHT, 400);
  // }
  // old_read = read2;
  // Driver::MDsetSpeed(MachineConfig::Canonn::WHEEL_MOTOR_RIGHT, 300);

  // Driver::MDsetSpeed(MachineConfig::Canonn::WINDING_MOTOR_RIGHT, 3000);
  //射出機構試し打ち
  /*
  if (Serial.available() > 0)
  {
    if (Serial.read() == '\n')
    {
      Serial.println("fire");
      //      Machine::arrow(6, 7, 1, 2, 4500, 0, 850);
      Machine::canonnShot(MachineConfig::Canonn::LEFT, 1950);
      Serial8.flush();
      Serial.println("done");
    }
  }
  */
  /*
   Driver::servoSetAngle(MachineConfig::Canonn::WINDING_SERVO_LEFT, MachineConfig::Canonn::WHEEL_LOCK_SERVO_ANGLE_LEFT[0]);
   Serial.println("free");
   delay(1000);
   Driver::servoSetAngle(MachineConfig::Canonn::WINDING_SERVO_LEFT, MachineConfig::Canonn::WHEEL_LOCK_SERVO_ANGLE_LEFT[1]);
   Serial.println("lock");
   delay(1000);

 */

  /*
    Driver::MDsetSpeed(MachineConfig::Canonn::ANGLE_MOTOR_RIGHT, 400);
    Serial.printf("loli 8 %d\n", Driver::lolicon_value[MachineConfig::Canonn::ANGLE_LOLI_RIGHT]);
    while (Driver::SW[MachineConfig::Canonn::ANGLE_LIMIT_SW_RIGHT_B])
    {
      Serial.printf("loli 9 %d\n", Driver::lolicon_value[MachineConfig::Canonn::ANGLE_LOLI_RIGHT]);
      Driver::MDsetSpeed(MachineConfig::Canonn::ANGLE_MOTOR_RIGHT, 0);
    }
    */
  /*
  if (Driver::lolicon_value[MachineConfig::Canonn::ANGLE_LOLI_LEFT] > 143)
  {
    Serial.printf("loli 8 %d\n", Driver::lolicon_value[MachineConfig::Canonn::ANGLE_LOLI_RIGHT]);
    Driver::MDsetSpeed(MachineConfig::Canonn::ANGLE_MOTOR_RIGHT, 0);
  }
  */

  /*
  Driver::servoSetAngle(MachineConfig::Canonn::WINDING_SERVO_LEFT, 850);
  Serial.println("850");
  delay(1000);
  Driver::servoSetAngle(MachineConfig::Canonn::WINDING_SERVO_LEFT, 1700);
  Serial.println("1700");
  delay(1000);
  */
  // Driver::MDsetSpeed(MachineConfig::Canonn::WINDING_MOTOR_LEFT, -500);;
  /*
  static Ps3DecoderNa::Data controller = {0};
  if (Serial7.available() > 2)
  {
    uint8_t res[2] = {0};
    Serial7.readBytes(res, 2);
    Ps3DecoderNa::decode(controller, res);
    Serial.printf("%d\n", res[0]);
    Serial.printf("%d\n", res[1]);
  }

*/
  //コントローラ受信
  /*
  static Ps3Decoder::Data controller = {};
  if (Serial7.available() > 12)
  {
    uint8_t res[12] = {0};
    Serial7.readBytes(res, 12);
    Ps3Decoder::decode(controller, res);
  }

  // Serial.printf("rx:%d\n", controller.rx);
  // Serial.printf("ry:%d\n", controller.ry);
  Serial.printf("rx %d\n", controller.rx);
  Serial.printf("ry %d\n", controller.ry);
  Serial.printf("cross %d\n", controller.cross);
  Serial.printf("up %d\n", controller.up);
  Serial.printf("down %d\n", controller.down);
  Serial.printf("r1 %d\n", controller.r1);
  Serial.printf("l1 %d\n", controller.l1);
  // コントローラデバッグ
  int controller_offset = 11;
  Serial.printf("rx:%d\n", controller.rx);
  Serial.printf("ry:%d\n", controller.ry);
  int motor_speed = sqrt(pow(controller.rx, 2) + pow(controller.ry, 2)) * 1.5 / 2;
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

  */
  //   Machine::readI2CLoli()
  /*
   if (Serial7.available() > 0)
   {
     Serial.println(Serial7.read());
   }
   */
  /*
  Driver::servoSetAngle(MachineConfig::Canonn::WINDING_SERVO_RIGHT, MachineConfig::Canonn::WINDING_LOCK_SERVO_ANGLE_RIGHT[0]);
  Serial.println("free");
  delay(1000);
  delay(1000);
  */
  /* 左主砲ガチャガチャ
   Driver::servoSetAngle(MachineConfig::Canonn::WINDING_SERVO_LEFT, MachineConfig::Canonn::WINDING_LOCK_SERVO_ANGLE_LEFT[1]);
   Serial.println("lock");
   Driver::MDsetSpeed(MachineConfig::Canonn::WINDING_MOTOR_LEFT, -2040);
   delay(1000);
   Driver::servoSetAngle(MachineConfig::Canonn::WINDING_SERVO_LEFT, MachineConfig::Canonn::WINDING_LOCK_SERVO_ANGLE_LEFT[0]);
   Serial.println("free");
   Driver::MDsetSpeed(MachineConfig::Canonn::WINDING_MOTOR_LEFT, 1040);
   delay(100);

   */
  // delay(2000);
  // Driver::MDsetSpeed(MachineConfig::Canonn::WINDING_MOTOR_LEFT, -500);
  // delay(1000);

  /*
   Driver::servoSetAngle(MachineConfig::Canonn::WINDING_SERVO_RIGHT,0 );
   Serial.println("0");
   delay(1000);
   Driver::servoSetAngle(MachineConfig::Canonn::WINDING_SERVO_RIGHT,850 );
   Serial.println("850");
   delay(1000);
   Driver::servoSetAngle(MachineConfig::Canonn::WINDING_SERVO_RIGHT, 1700);
   Serial.println("1700");
   delay(1000);
   */
  // readI2C test

  // check readI2Cloli
  //  int data[2] = {0};
  //  Machine::readI2CLoli(data);
  //  Serial.printf("right  %d\n", Driver::SW[MachineConfig::Canonn::WINDING_LIMITSW_RIGHT]);
  //  Serial.printf("left %d\n", Driver::SW[MachineConfig::Canonn::WINDING_LIMITSW_LEFT]);
  //  analogWrite(0, 3900);
//  Machine::canonnShot(MachineConfig::Canonn::RIGHT);
analogWrite(0,5000);

  delay(20);
}
#endif
