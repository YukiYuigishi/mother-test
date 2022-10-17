#pragma once
#include <Driver.h>
namespace Machine
{

    // millsec 5000
    void arrow(const int winding_motor, const int weel_motor, const int servo_motor, const int SW_number, const int milli_sec, const int servo_winding_arg, const int servo_stop_arg)
    {
        constexpr int power = 1000;
        //事前の回転
        Driver::MDsetSpeed(winding_motor, power);
        //サーボで継手に接続
        Driver::servoSetAngle(servo_motor, servo_winding_arg);
        //巻取り delay
        // millisec 5000
        delay(milli_sec);
        Driver::MDsetSpeed(winding_motor, 90);
        // run weel
        /*
        while (Driver::SW[1])
        {
            Driver::MDsetSpeed(6, 300);
        }
        */
        //weel();

        // 射出
        Serial.println("ban");
        Driver::MDsetSpeed(winding_motor, -100);
        Driver::servoSetAngle(servo_motor, servo_stop_arg);
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
        // 動かす
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
}