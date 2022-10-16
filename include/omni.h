#pragma once
#include <math.h>
#include <Driver.h>
#include <pid_test.hpp>
namespace Omni
{

    struct Coordinate
    {
        /* data */
        double X;
        double Y;
    };

    typedef enum
    {
        right,
        left
    } Machine;

    typedef struct Coordinate FieldCoordinate;
    typedef struct Coordinate MachineCoordinate;

    PidTest motor1(1, 14, 0, 0);
    PidTest motor2(2, 14, 0, 0);
    PidTest motor3(3, 14, 0, 0);
    PidTest motor4(4, 17.25, 0, 0);

    //半径(mm)
    constexpr int ODOMETOR_OMNI_RADIUS = 24;
    inline __always_inline void run(double volume, double theata_rad)
    {

        motor1.SetTarget(volume * sin(theata_rad));
        motor2.SetTarget(volume * -cos(theata_rad));
        motor3.SetTarget(volume * -sin(theata_rad));
        // 取り付けが逆になってる
        motor4.SetTarget(volume * cos(theata_rad));
        Driver::MDsetSpeed(1, motor1.Run());
        Driver::MDsetSpeed(2, motor2.Run());
        // motor2.Run();
        // Driver::MDsetSpeed(2, 200);

        Driver::MDsetSpeed(3, motor3.Run());

        // Driver::MDsetSpeed(4, 200);
        // motor4.Run();
        Driver::MDsetSpeed(4, -motor4.Run());

        Serial.printf("md2 %d\n", Driver::lolicon_value[2]);
        Serial.printf("md4 %d\n", Driver::lolicon_value[4]);
        Serial.println("---------------------------");
    }

    // theta 0 ~ 360 quotanion
    // 角速度
    FieldCoordinate getFieldCoordinateSpeed(const double arc_degree)
    {
        static FieldCoordinate abusolute_cooordinate = {
            X : 0,
            Y : 0
        };

        static MachineCoordinate dbg_coordinate = {X : 0, Y : 0};

        const double theta = arc_degree * M_PI / 180;
        static uint32_t old_time = 0;

        constexpr uint16_t startOdometer = 5;
        const double cos45 = cos(M_PI / 4);

        MachineCoordinate coordinate = {
            X : ((Driver::lolicon_speed_value[startOdometer]) + (-Driver::lolicon_speed_value[startOdometer + 2]) + (Driver::lolicon_speed_value[startOdometer + 1]) + (-Driver::lolicon_speed_value[startOdometer + 3])) * cos45 / 4,
            Y : ((-Driver::lolicon_speed_value[startOdometer]) + Driver::lolicon_speed_value[startOdometer + 1] + Driver::lolicon_speed_value[startOdometer + 2] + (-Driver::lolicon_speed_value[startOdometer + 3])) * cos45 / 4,
        };

        // double d_time = (double)(millis() - old_time) / 1000.0;
        double time_d = (millis() - old_time) / 1000.0;

        dbg_coordinate.X += coordinate.X * time_d;
        dbg_coordinate.Y += coordinate.Y * time_d;

        Serial.printf("arc: %lf\n", arc_degree);
        Serial.printf("time_d %lf\n", time_d);
        Serial.printf("Raw X: %lf\n", coordinate.X);
        Serial.printf("Raw Y: %lf\n", coordinate.Y);
        Serial.printf("dbg Raw X: %lf\n", dbg_coordinate.X * ODOMETOR_OMNI_RADIUS);
        Serial.printf("dbg Raw Y: %lf\n", dbg_coordinate.Y * ODOMETOR_OMNI_RADIUS);
        old_time = millis();

        //      Serial.print("Machin X:");
        //      Serial.println(dbg_coordinate.X);
        //      Serial.print("Machin Y:");
        //      Serial.println(dbg_coordinate.Y);

        // なんかバグってるので 半径 x2をしている
        abusolute_cooordinate.X += (coordinate.X * cos(theta) - coordinate.Y * sin(theta)) * ODOMETOR_OMNI_RADIUS * 2 * time_d;
        abusolute_cooordinate.Y += (coordinate.X * sin(theta) + coordinate.Y * cos(theta)) * ODOMETOR_OMNI_RADIUS * 2 * time_d;

        Serial.printf("abs X: %lf\n", abusolute_cooordinate.X);
        Serial.printf("abs Y: %lf\n", abusolute_cooordinate.Y);

        return abusolute_cooordinate;
    }

    // direction true is right , false is  left
    inline __always_inline void rotation(int power, bool direction)
    {
        if (direction)
        {
                }
        else
        {
        }
    }
}