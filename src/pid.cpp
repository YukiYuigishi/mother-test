#include <pid.hpp>

void Pid::PidArg::pid()
{

    // static uint32_t time_old = millis();
    // uint32_t time_diff = millis() - time_old_;

    double deviation_sum = 0;
    deviation_sum += deviation_;

    // d制御なし
    // double deviation_differential = (double)deviation_[99] / time_diff;

    //    output_ = kp_ * deviation_[99] + ki_ * deviation_sum + kd_ * deviation_differential;
    output_ = kp_ * deviation_ + ki_ * deviation_sum;
}

void Pid::PidArg::update()
{

    deviation_ = target_ - Driver::lolicon_value[loli_ID_];
}

void Pid::PidArg::SetTarget(double target)
{
    target_ = target;
}

double Pid::PidArg::Run()
{
    update();
    pid();
    if (pid_debug_)
    {

        Serial.print("motor: ");
        Serial.print(md_ID_);
        Serial.print("\ttarget: ");
        Serial.print(target_);
        Serial.print("\tnow ");
        Serial.print(Driver::lolicon_value[md_ID_]);
        Serial.print("\tdevi: ");
        Serial.print(deviation_);
        Serial.print("\tpwm ");
        Serial.println(output_);
    }
    return output_;
}
