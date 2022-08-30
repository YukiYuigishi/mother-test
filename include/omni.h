#pragma once
#include <math.h>
#include <Driver.h>
namespace Omni
{
    void run(double volume, double theata)
    {
        Driver::MDsetSpeed(1, (double)volume * sin((theata)*M_PI / 180));
        Driver::MDsetSpeed(2, (double)volume * sin((theata - 90) * M_PI / 180));
        Driver::MDsetSpeed(3, (double)volume * sin((-theata) * M_PI / 180));
        Driver::MDsetSpeed(4, (double)volume * sin((theata + 90) * M_PI / 180));
    }
}