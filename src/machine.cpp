#include <machine.h>
#include <config.h>

namespace Machine
{

    uint8_t machine_status[100] = {0};
    Pid::PidArg arg_pid_right(MachineConfig::Canonn::MOTOR_ANGLE_RIGHT, MachineConfig::Canonn::ANGLE_LOLI_RIGHT, pidConfig[0]);
    Pid::PidArg arg_pid_left(MachineConfig::Canonn::MOTOR_ANGLE_LEFT, MachineConfig::Canonn::ANGLE_LOLI_LEFT, pidConfig[1]);
    MachineConfig::PID::PidConfig pidConfig[2] = {
        {1, 0, 0},
        {1, 0, 0},
    };
    void arrow(const int winding_motor, const int weel_motor, const int servo_motor, const int SW_number, const int milli_sec, const int servo_winding_arg, const int servo_stop_arg)
    {
        constexpr int power = 1400;
        //事前の回転
        Driver::MDsetSpeed(winding_motor, 50);
        // 巻取り用モーターの加速まち
        delay(50);
        //サーボで継手に接続
        Driver::servoSetAngle(servo_motor, servo_winding_arg);
        Driver::MDsetSpeed(winding_motor, power);
        //巻取り delay
        // millisec 5000
        delay(milli_sec);
        Driver::MDsetSpeed(winding_motor, -90);
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
        Serial.println("ban");
        Driver::MDsetSpeed(winding_motor, -100);
        Driver::servoSetAngle(servo_motor, servo_stop_arg);
        Driver::MDsetSpeed(winding_motor, 0);
        delay(500);

        /*
          while (Driver::SW[1])
          {
            Driver::MDsetSpeed(weel_motor, 300);
          }
          Driver::MDsetSpeed(weel_motor, 0);
          */
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

    void cannonnInit(int right_md_port, int right_sw_port, int right_loli_port, int left_md_port, int left_sw_port, int left_loli_port)
    {
        constexpr int right_down_speed = -400;
        constexpr int left_down_speed = 400;

        // 仰角下げ
        Driver::MDsetSpeed(right_md_port, right_down_speed);
        Driver::MDsetSpeed(left_md_port, left_down_speed);

        uint8_t status = 0;
        while (status < 2)
        {
            if (Driver::SW[right_sw_port])
            {
                Driver::MDsetSpeed(right_md_port, 0);
                status++;
            }

            if (Driver::SW[left_sw_port])
            {
                Driver::MDsetSpeed(left_md_port, 0);
                status++;
            }
        }

        Driver::lolicon_value[right_loli_port] = 0;
        Driver::lolicon_value[left_loli_port] = 0;
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
    void canonnAngleSet(MachineConfig::Canonn::SELECT select, double angle)
    {
        // memo
        /*
        初期位置の角度は水平面に対して０度ではない.
        そのため、そこを０度としたときの角度を考える必要があり
        */

        //入力を切り替え
        switch (select)
        {
        case MachineConfig::Canonn::RIGHT:
            arg_pid_right.SetTarget(angle);
            double power = arg_pid_right.Run();
            Serial.printf("arg power right: %lf\n", power);
            Driver::MDsetSpeed(MachineConfig::Canonn::MOTOR_ANGLE_RIGHT, power);

            /* code */
            break;

        case MachineConfig::Canonn::LEFT:
            arg_pid_left.SetTarget(angle);
            double power = arg_pid_left.Run();
            Driver::MDsetSpeed(MachineConfig::Canonn::MOTOR_ANGLE_LEFT, power);
            break;
        default:
            return;
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

    void checkCanonn()
    {
    }
}