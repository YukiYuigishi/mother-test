#include <pid.h>
double Pid::Kp[4] = {1, 1, 1, 1};
double Pid::Ki[4] = {1, 1, 1, 1};
double Pid::target[4] = {0};
double Pid::deviation[4][100] = {{1}, {1}, {1}, {1}};
int64_t Pid::lori_tmp[4] = {0};
unsigned long Pid::time_tmp[4] = {0};
double Pid::output[4] = {0};

void Pid::Update(uint8_t loli, double deviation[100], double (*callback)(int64_t))
{

    for (double *i = deviation; i != deviation + 99; i++)
    {
        *i = *(i + 1);
    }

    //    deviation[99] = (double)callback(readenc(loli));
    deviation[99] = (double)callback(Driver::lolicon_value[1]);
}

// PI制御
double Pid::PidLogic(double kp, double ki, double x, double target, double *devi)
{
    // P制御
    //偏差
    devi[99] = target - x;

    // deviation integral
    double deviation_i = 0;
    for (double *i = devi; i != devi + 100; i++)
    {
        deviation_i += *i;
    }

    return kp * devi[99] + ki * deviation_i;
}

void Pid::Run(void (*MDx)(int, int))
{

    for (int i = 0; i < 4; i++)
    {
        Update(i, deviation[i], Pid::Callback);
    }
    /*
    for (int i = 0; i < 4; i++)
    {
        // output[i] = PidLogic(Kp[i], Ki[i], deviation[i][99], target[i], deviation[i]);
        output[i] = PidLogic(Kp[i], Ki[i], output[i], target[i], deviation[i]);
    }
    */

    //    MDx(1, (int)output[0]);
    //   MDx(1, (int)output[1]);
    Serial.printf("loli:%d \t tmp: %d\tdevi:%lf\n", Driver::lolicon_value[1], lori_tmp[0], deviation[0][99]);

    //    constexpr int i = 0;
    //    Update(0, deviation[i], Callback);
    //    //    PidLogic(Kp[i], Ki[i], deviation[i][99], Pid::output[i], Pid::deviation[i]);
    //    output[0] = 100;
    MDx(1, target[0]);
}

double Pid::Callback(int64_t loli)
{
    /*

    double diff = (double)(loli - lori_tmp[0]) / lori_resolution;
    lori_tmp[0] = loli;
    // rad/s
    double radps = diff / (micros() - time_tmp[0]) * 2 * M_PI;

    time_tmp[0] = micros();

    return radps;
    */
    double diff = (double)(loli - lori_tmp[0]) / lori_resolution * 20;
    lori_tmp[0] = loli;
    return diff;
}
/*
int Pid::setValue(int md, double value)
{
    if (md > 4)
    {
        return 255;
    }
    target[md] = value;
    return 1;
}
*/