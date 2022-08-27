#pragma once
#include <Encoder.h>
#include <Driver.h>

namespace Pid
{

    extern double Kp[4];
    extern double Ki[4];
    /*
    double r = 0;
    double x = 0;
    double u = 0;
    double e = 0;
    */
    constexpr int lori_resolution = 2048;

    // target speed
    extern double target[4];
    // deviation
    extern double deviation[4][100];
    // old lori tmp
    extern int64_t lori_tmp[4];
    // old time tmp
    extern unsigned long time_tmp[4];
    // output value
    extern double output[4];
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
    void Update(uint8_t loli, double devi[100], double (*callback)(int64_t));
    // Pidのメインロジック
    double PidLogic(double kp, double ki, double x, double r, double *devi);
    // PIDの実行 callbackはロリコンの値の処理用
    void Run(void (*MDx)(int, int));

    // int setValue(int md, double value);

    double Callback(int64_t lori);

} // namespace Pid
