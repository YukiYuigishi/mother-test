#include <pid.h>

void Pid::Update(uint8_t loli, double deviation[100], double (*callback)(int32_t))
{

    for (double *i = deviation; i != deviation + 99; i++)
    {
        *i = *(i + 1);
    }

    //    deviation[99] = (double)callback(readenc(loli));
    deviation[99] = (double)callback(Driver::lolicon_value[1]);
}

// PI制御
double Pid::PidLogic(double kp, double ki, double x, double r, double *devi)
{
    // P制御
    //偏差
    devi[99] = r - x;

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
    /*

    for (int i = 0; i < 4; i++)
    {
        Update(i, deviation[i], Pid::Callback);
    }
    for (int i = 0; i < 4; i++)
    {
        Pid(Kp[i], Ki[i], deviation[i][99], output[i], deviation[i]);
    }

    MDx(0, (int)output[0]);
    //   MDx(1, (int)output[1]);
    */
    constexpr int i = 0;
    Update(0, deviation[i], Callback);
    PidLogic(Kp[i], Ki[i], deviation[i][99], output[i], deviation[i]);
    MDx(1, (int)output[0]);
}

double Pid::Callback(int32_t loli)
{

    double diff = (double)(loli - lori_tmp[0]) / lori_resolution;
    lori_tmp[0] = loli;
    // rad/s
    double radps = diff / (micros() - time_tmp[0]) * 2 * M_PI;
    time_tmp[0] = micros();

    return radps;
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
