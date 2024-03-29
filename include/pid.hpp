#pragma once
#include <Driver.h>
#include <config.hpp>

namespace Pid
{
    class PidArg
    {
    private:
        const int pid_debug_ = 1;
        const int md_ID_ = 0;
        const int loli_ID_ = 0;

        const double kp_ = 1;
        const double ki_ = 0;
        const double kd_ = 0;

        // 偏差 e(t)
        double deviation_ = 0;
        // 入力値 u(t) PWM
        double output_ = 0;
        // 目標値 r(t)
        double target_ = 0;

        // sum
        double deviation_sum = 0;

        double time_old_ = 0;

        void pid();
        void update();

    public:
        PidArg(MachineConfig::MDID md_ID, MachineConfig::LOLIID loli_ID, MachineConfig::PID::PidConfig &config) : md_ID_(md_ID), loli_ID_(loli_ID), kp_(config.kp), ki_(config.ki), kd_(config.kd) {}
        void SetTarget(double target);
        double Run();
    }; // namespace Pid

} // namespace Pid
