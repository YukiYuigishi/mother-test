#pragma once
#include <Driver.h>
#include <config.hpp>
#include <pid.hpp>
namespace Machine
{
    extern MachineConfig::PID::PidConfig pidConfig[2];
    extern uint8_t machine_status[100];
    //仰角PID
    extern Pid::PidArg angle_pid_right;
    extern Pid::PidArg angle_pid_left;

    // millsec 5000
    void arrow(const int winding_motor, const int weel_motor, const int servo_motor, const int SW_number, const int milli_sec, const int servo_winding_arg, const int servo_stop_arg);

    void wheel();

    //主砲
    void canonnAngleSet(MachineConfig::Canonn::SELECT select, int angle);
    //主砲左用
    // void canonnElevationAngleLeft(int md_port, int canonn_angle) {}

    //主砲初期化
    void cannonnInit(int right_md_port, int right_sw_port, int right_loli_port, int left_md_port, int left_sw_port, int left_loli_port);

    //範囲外検知
    void checkCanonnFront();
    //範囲外検知
    void checkCanonnBack();

    //ステータス初期化
    void statusInit(uint8_t enable_number);

    uint8_t checkStatus();
}