#include <machine.hpp>

namespace Machine
{
    //主砲切り替え
    MachineConfig::Canonn::SELECT canonn_status = MachineConfig::Canonn::RIGHT;

    uint8_t machine_status[100] = {0};
    Pid::PidArg angle_pid_right(MachineConfig::Canonn::ANGLE_MOTOR_RIGHT, MachineConfig::Canonn::ANGLE_LOLI_RIGHT, pidConfig[0]);
    Pid::PidArg angle_pid_left(MachineConfig::Canonn::ANGLE_MOTOR_LEFT, MachineConfig::Canonn::ANGLE_LOLI_LEFT, pidConfig[1]);
    int angle_canonn_right = 5;
    int angle_canonn_left = 5;

    MachineConfig::PID::PidConfig pidConfig[2] = {
        {300, 0.1, 0},
        {3, 0, 0},
    };

    void arrow(const int winding_motor, const int weel_motor, const int servo_motor, const int SW_number, const int milli_sec, const int servo_winding_arg, const int servo_stop_arg)
    {
        constexpr int power = 3000;
        //事前の回転
        Driver::MDsetSpeed(winding_motor, 50);
        // 巻取り用モーターの加速まち
        delay(50);
        //サーボで継手に接続
        Driver::servoSetAngle(servo_motor, servo_winding_arg);
        Driver::MDsetSpeed(winding_motor, power); //巻取り delay
        // millisec 500000

        delay(milli_sec);
        Driver::MDsetSpeed(winding_motor, 200);
        // real
        Serial.printf("sw2 %d\n", Driver::SW[2]);
        Driver::servoSetAngle(MachineConfig::Canonn::WHEEL_LOCK_SERVO_RIGHT, 330);
        delay(400);
        while (!Driver::SW[2])
        {
            Driver::MDsetSpeed(MachineConfig::Canonn::WHEEL_MOTOR_RIGHT, 300);
        }
        Driver::MDsetSpeed(MachineConfig::Canonn::WHEEL_MOTOR_RIGHT, 0);

        //固定
        Driver::servoSetAngle(MachineConfig::Canonn::WHEEL_LOCK_SERVO_RIGHT, 1700);
        delay(400);
        // run weel
        /*
        while (Driver::SW[1])
        {
            Driver::MDsetSpeed(6, 300);
        }
        */
        // weel();

        // 射出
        Serial.println("ban");
        Driver::MDsetSpeed(winding_motor, -100);
        Driver::servoSetAngle(servo_motor, servo_stop_arg);
        Driver::MDsetSpeed(winding_motor, 0);

        // ホイール動かす
        Driver::servoSetAngle(MachineConfig::Canonn::WHEEL_LOCK_SERVO_RIGHT, 400);
        while (Driver::SW[2])
        {
            Driver::MDsetSpeed(MachineConfig::Canonn::WHEEL_MOTOR_RIGHT, 300);
        }
        Driver::MDsetSpeed(MachineConfig::Canonn::WHEEL_MOTOR_RIGHT, 0);
        Driver::servoSetAngle(MachineConfig::Canonn::WHEEL_LOCK_SERVO_RIGHT, 1700);
        delay(500);

        /*
          while (Driver::SW[1])
          {
            Driver::MDsetSpeed(weel_motor, 300);
          }
          Driver::MDsetSpeed(weel_motor, 0);
          */
    }

    void canonnShot(MachineConfig::Canonn::SELECT select, uint16_t timer)
    {
        Serial.println("start canonnn");
        using namespace MachineConfig;

        switch (select)
        {
        case MachineConfig::Canonn::RIGHT:
        {

            //+が巻取り方向
            constexpr int hold_power = 150;
            constexpr int slow_start_power_right = 50;
            constexpr int away_power_right = -100;
            //事前の回転
            Driver::MDsetSpeed(Canonn::WINDING_MOTOR_RIGHT, slow_start_power_right);
            // 巻取り用モーターの加速まち
            delay(50);
            //サーボで継手に接続
            Driver::servoSetAngle(Canonn::WINDING_SERVO_RIGHT, Canonn::WINDING_LOCK_SERVO_ANGLE_RIGHT[0]);
            Driver::MDsetSpeed(Canonn::WINDING_MOTOR_RIGHT, Canonn::WINDING_POWER_RIGHT); //巻取り delay
            // millisec 500000

            delay(timer);
            Driver::MDsetSpeed(Canonn::WINDING_MOTOR_RIGHT, hold_power);
            //装填待ち
            delay(500);
            // run weel
            /*
            while (Driver::SW[1])
            {
                Driver::MDsetSpeed(6, 300);
            }
            */
            // weel();

            // 射出
            Serial.println("right ban");
            Driver::MDsetSpeed(Canonn::WINDING_MOTOR_RIGHT, away_power_right);
            Driver::servoSetAngle(Canonn::WINDING_SERVO_RIGHT, Canonn::WINDING_LOCK_SERVO_ANGLE_RIGHT[1]);
            Driver::MDsetSpeed(Canonn::WINDING_MOTOR_RIGHT, 0);
            delay(500);
            break;
        }
        case MachineConfig::Canonn::LEFT:
        {
            //-が巻取り方向
            constexpr int hold_power_left = -90;
            constexpr int slow_start_power_left = -50;
            constexpr int away_power_left = 100;
            //事前の回転
            Driver::MDsetSpeed(Canonn::WINDING_MOTOR_LEFT, slow_start_power_left);
            // 巻取り用モーターの加速まち
            delay(50);
            //サーボで継手に接続
            Driver::servoSetAngle(Canonn::WINDING_SERVO_LEFT, Canonn::WINDING_LOCK_SERVO_ANGLE_LEFT[0]);
            Driver::MDsetSpeed(Canonn::WINDING_MOTOR_LEFT, Canonn::WINDING_POWER_LEFT); //巻取り delay
            // millisec 500000

            delay(timer);
            Driver::MDsetSpeed(Canonn::WINDING_MOTOR_LEFT, hold_power_left);
            //装填待ち
            delay(500);
            // run weel
            /*
            while (Driver::SW[1])
            {
                Driver::MDsetSpeed(6, 300);
            }
            */
            // weel();

            // 射出
            Serial.println("left ban");
            Driver::MDsetSpeed(Canonn::WINDING_MOTOR_LEFT, away_power_left);
            Driver::servoSetAngle(Canonn::WINDING_SERVO_LEFT, Canonn::WINDING_LOCK_SERVO_ANGLE_LEFT[1]);
            Driver::MDsetSpeed(Canonn::WINDING_MOTOR_LEFT, 0);
            delay(500);
        }
        break;
        default:
            Serial.println("shot error");
            break;
        }
    }
    void wheel()
    {

        while (!Driver::SW[1])
        {
            Driver::MDsetSpeed(6, 250);
        }

        delay(200);
        Serial.println("stop");
        Driver::MDsetSpeed(6, 0);
        // スイッチが反応するまで装填機構を回転させる
        while (Driver::SW[1])
        {
            Driver::MDsetSpeed(6, 250);
        }
        Driver::MDsetSpeed(6, 0);
    }

    void cannonnInit()
    {
        int right_md_port = MachineConfig::Canonn::ANGLE_MOTOR_RIGHT;
        int right_sw_port = MachineConfig::Canonn::ANGLE_LIMIT_SW_RIGHT_F;
        int right_loli_port = MachineConfig::Canonn::ANGLE_LOLI_RIGHT;
        int left_md_port = MachineConfig::Canonn::ANGLE_MOTOR_LEFT;
        int left_sw_port = MachineConfig::Canonn::ANGLE_LIMIT_SW_LEFT_F;
        int left_loli_port = MachineConfig::Canonn::ANGLE_LOLI_LEFT;

        //仰角下げ
        constexpr int right_down_speed = -400;
        constexpr int left_down_speed = -400;

        // 仰角下げ
        Driver::MDsetSpeed(right_md_port, right_down_speed);
        Driver::MDsetSpeed(left_md_port, left_down_speed);

        bool right_status = false;
        bool left_status = false;
        while (!right_status || !left_status)
        {
            if (Driver::SW[right_sw_port])
            {
                right_status = true;
                Driver::lolicon_value[right_loli_port] = 0;
                Driver::MDsetSpeed(right_md_port, 0);
            }

            if (Driver::SW[left_sw_port])
            {
                left_status = true;
                Driver::lolicon_value[left_loli_port] = 0;
                Driver::MDsetSpeed(left_md_port, 0);
            }
        }

        Driver::MDsetSpeed(right_md_port, 400);
        Driver::MDsetSpeed(left_md_port, 400);
        delay(100);
        Driver::MDsetSpeed(right_md_port, 0);
        Driver::MDsetSpeed(left_md_port, 0);
        Serial.println("Canonn Init");
        return;
    }

    //ステータス初期化
    void statusInit(uint8_t enable_number)
    {
        // machine_statusの初期化
        for (int i = 0; i < 100; i++)
        {
            machine_status[i] = enable_number;
        }
        Serial.println("Status Init");
    }
    void canonnAngleSet(MachineConfig::Canonn::SELECT select, int angle)
    {

        using namespace MachineConfig;

        // memo
        /*
        初期位置の角度は水平面に対して０度ではない.
        そのため、そこを０度としたときの角度を考える必要があり
        */

        //入力を切り替え
        double power = 0;
        switch (select)
        {
        case MachineConfig::Canonn::RIGHT:
            while (Driver::lolicon_value[Canonn::ANGLE_LOLI_RIGHT] > (angle + 1) || Driver::lolicon_value[Canonn::ANGLE_LOLI_RIGHT] < (angle - 1))
            {
                angle_pid_right.SetTarget(angle);
                power = angle_pid_right.Run();
                Serial.printf("angle power right: %lf\n", -power);
                if (power > 400)
                {
                    power = 400;
                }
                else if (power < -400)
                {
                    power = -400;
                }

                Driver::MDsetSpeed(MachineConfig::Canonn::ANGLE_MOTOR_RIGHT, -power);
                delay(20);
            }

            /* code */
            break;

        case MachineConfig::Canonn::LEFT:
            angle_pid_left.SetTarget(angle);
            power = angle_pid_left.Run();
            Serial.printf("arg power right: %lf\n", power);
            //  Driver::MDsetSpeed(MachineConfig::Canonn::ANGLE_MOTOR_LEFT, power);
            break;
        }
    }

    // use Serial6
    //  '0' と比較して違うならenable '0'と
    uint8_t checkStatus()
    {
        for (int i = 0; i < 99; i++)
        {
            machine_status[i] = machine_status[i + 1];
        }

        if (Serial8.available() > 0)
        {
            machine_status[99] = (uint8_t)Serial8.read();
            Serial8.flush();
            Serial.println(machine_status[99]);
        }

        int status_avg = 0;
        for (int i = 0; i < 100; i++)
        {
            status_avg += machine_status[i];
        }

        Serial.printf("avg %d\n", status_avg / 100);
        return status_avg / 100;
    }

    void checkCanonnFront()
    {
        using namespace MachineConfig;
        //右 リミットスイッチ確認
        if (Driver::SW[Canonn::ANGLE_LIMIT_SW_RIGHT_F])
        {
            Serial.println("ANGLE_LIMIT_SW_RIGHT_F");
            //定数の300をどうにか白
            Driver::MDsetSpeed(Canonn::ANGLE_MOTOR_RIGHT, 400);
            delay(100);
            Driver::MDsetSpeed(Canonn::ANGLE_MOTOR_RIGHT, 0);
        }
        if (Driver::SW[Canonn::ANGLE_LIMIT_SW_LEFT_F])
        {
            Serial.println("ANGLE_LIMIT_SW_LEFT_F");
            //定数の300をどうにか白
            Driver::MDsetSpeed(Canonn::ANGLE_MOTOR_LEFT, 400);
            delay(100);
            Driver::MDsetSpeed(Canonn::ANGLE_MOTOR_LEFT, 0);
        }
        /*

        // todo
        //リミットスイッチになった場合に要改修
        if (Driver::SW[Canonn::ANGLE_LIMIT_SW_RIGHT_B] || abs(Driver::lolicon_value[Canonn::ANGLE_LOLI_RIGHT]) > Canonn::ANGLE_MOTOR_LIMIT_RIGHT)
        {
            Driver::segDriver(99);
            Driver::MDsetSpeed(Canonn::ANGLE_MOTOR_RIGHT, 0);
        }
        if (Driver::SW[Canonn::ANGLE_LIMIT_SW_LEFT_B] || abs(Driver::lolicon_value[Canonn::ANGLE_LOLI_LEFT]) > Canonn::ANGLE_MOTOR_LIMIT_LEFT)
        {
            Driver::segDriver(99);
            Driver::MDsetSpeed(Canonn::ANGLE_MOTOR_RIGHT, 0);
        }
        */

        return;
    }
    void checkCanonnBack()
    {
        using namespace MachineConfig;
        // todo
        //リミットスイッチになった場合に要改修
        if (Driver::SW[Canonn::ANGLE_LIMIT_SW_RIGHT_B] || abs(Driver::lolicon_value[Canonn::ANGLE_LOLI_RIGHT]) > Canonn::ANGLE_MOTOR_LIMIT_RIGHT)
        {
            Driver::segDriver(99);
            Driver::MDsetSpeed(Canonn::ANGLE_MOTOR_RIGHT, 0);
            Serial.println("RIGHT STOP");
        }
        if (Driver::SW[Canonn::ANGLE_LIMIT_SW_LEFT_B] || abs(Driver::lolicon_value[Canonn::ANGLE_LOLI_LEFT]) > Canonn::ANGLE_MOTOR_LIMIT_LEFT)
        {
            Driver::segDriver(99);
            Driver::MDsetSpeed(Canonn::ANGLE_MOTOR_LEFT, 0);
            Serial.println("LEFT STOP");
        }
    }
    inline void readI2CLoli(int (&data)[2])
    {
        noInterrupts();
        FlexiTimer2::stop();
        Wire.requestFrom(0x0b, 8);
        if (Wire.available() > 0)
        {
            data[0] = Wire.read();
            data[0] += Wire.read() << 8;
            data[0] += Wire.read() << 16;
            data[0] += Wire.read() << 24;
            Serial.printf("read1 %d\n", data[0]);
            data[1] = Wire.read();
            data[1] += Wire.read() << 8;
            data[1] += Wire.read() << 16;
            data[1] += Wire.read() << 24;
            Serial.printf("read2 %d\n", data[1]);
        }
        interrupts();
        FlexiTimer2::start();
    }
}