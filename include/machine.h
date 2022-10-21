#pragma once
#include <Driver.h>
namespace Machine
{
    uint8_t machine_status[100] = {0};

    // millsec 5000
    void arrow(const int winding_motor, const int weel_motor, const int servo_motor, const int SW_number, const int milli_sec, const int servo_winding_arg, const int servo_stop_arg)
    {
        constexpr int power = 1200;
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

    void weel()
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

    void weel2()
    {
        Driver::MDsetSpeed(6, 250);
    }

    //主砲右用
    void canonnElevationAngleRight(int md_port, int cannonn_angle)
    {
    }

    //主砲左用
    void canonnElevationAngleLeft(int md_port, int canonn_angle)
    {
    }

    void cannonnInit(int right_md_port, int right_sw_port, int right_loli_port, int left_md_port, int left_sw_port, int left_loli_port)
    {
        Driver::MDsetSpeed(right_md_port, 1000);
        Driver::MDsetSpeed(left_md_port, 1000);

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

    void statusInit(uint8_t enable_number)
    {
        // machine_statusの初期化
        for (int i = 0; i < 100; i++)
        {
            machine_status[i] = enable_number;
        }
        Serial.println("Status Init");
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
}