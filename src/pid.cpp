#include <pid.hpp>

void Pid::PidArg::pid()
{

    //    uint32_t time_diff = millis() - time_old_;

    double deviation_sum = 0;
    for (int i = 0; i < 100; i++)
    {
        deviation_sum += deviation_[i];
    }
    // d制御なし
    // double deviation_differential = (double)deviation_[99] / time_diff;

    //    output_ = kp_ * deviation_[99] + ki_ * deviation_sum + kd_ * deviation_differential;
    output_ = kp_ * deviation_[99] + ki_ * deviation_sum;
}

void Pid::PidArg::update()
{
    for (int i = 0; i < 99; i++)
    {
        deviation_[i] = deviation_[i + 1];
    }

    deviation_[99] = target_ - Driver::lolicon_value[loli_ID_];
}

void Pid::PidArg::SetTarget(double target)
{
}

double Pid::PidArg::Run()
{
    if (pid_debug_)
    {

        Serial.print("motor: ");
        Serial.print(md_ID_);
        Serial.print("\ttarget: ");
        Serial.print(target_);
        Serial.print("\tnow ");
        Serial.print(Driver::lolicon_speed_value[md_ID_]);
        Serial.print("\tdevi: ");
        Serial.print(deviation_[99]);
        Serial.print("rad/s ");
        Serial.print("\tpwm ");
        Serial.println(output_);
    }
    return output_;
}
