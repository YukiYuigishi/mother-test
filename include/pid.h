#pragma once
#include <Encoder.h>
#include <pid.h>
#include <Driver.h>

namespace Pid
{

    double Kp[4] = {1, 1, 1, 1};
    double Ki[4] = {1, 1, 1, 1};
    /*
    double r = 0;
    double x = 0;
    double u = 0;
    double e = 0;
    */
    constexpr int lori_resolution = 1;

    // target speed
    double target[4] = {0};
    // deviation
    double deviation[4][100] = {{1}, {1}, {1}, {1}};
    // old lori tmp
    int32_t lori_tmp[4] = {0};
    // old time tmp
    unsigned long time_tmp[4] = {0};
    // output value
    double output[4] = {0};
//    short (*readenc)(unsigned int enc);

    /*
    // initialize
    void Init(short (*rotenc)(unsigned int))
    {
        readenc = rotenc;
    }
    */

    // ロリコン等のアップデート
    // 引数にcallbackを取り、この関数は偏差の処理を行う ex. ロリコンの値をrpsに変換など
    void Update(uint8_t loli, double devi[100], double (*callback)(int32_t));
    // Pidのメインロジック
    double PidLogic(double kp, double ki, double x, double r, double *devi);
    // PIDの実行 callbackはロリコンの値の処理用
    void Run(void (*MDx)(int, int));

    // int setValue(int md, double value);

    double Callback(int32_t lori);

} // namespace Pid
