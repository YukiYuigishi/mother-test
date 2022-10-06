#pragma once
#include <math.h>
#include <Driver.h>
namespace Omni
{

    struct Coordinate
    {
        /* data */
        double X;
        double Y;
    };

    typedef struct Coordinate FieldCoordinate;
    typedef struct Coordinate MachineCoordinate;

    //半径(mm)
    constexpr int ODOMETOR_OMNI_RADIUS = 24;
    inline __always_inline void run(double volume, double theata)

    {
        Driver::MDsetSpeed(1, volume * sin((theata)));
        Driver::MDsetSpeed(2, volume * -cos((theata)));
        Driver::MDsetSpeed(3, volume * -sin((theata)));
        Driver::MDsetSpeed(4, volume * -cos((theata)));
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

    inline __always_inline void rotation(double theta)
    {
    }
}