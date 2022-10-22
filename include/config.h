#pragma once
#define CONFIG_MACRO 0
#if CONFIG_MACRO
#define MOTOR_RIGHT 1
#define MOTOR_FRONT 2
#define MOTOR_LEFT 3
#define MOTOR_BACK 4
#define WINDING_MOTOR_RIGHT
#define WINDING_MOTOR_LEFT
#define WEEL_MOTOR_RIGHT
#define WEEL_MOTOR_LEFT
#else

namespace MachineConfig
{
    typedef int MDID;
    typedef int LOLIID;
    typedef int SWID;
    namespace PID
    {
        typedef struct
        {
            const double kp;
            const double ki;
            const double kd;
        } PidConfig;

    }

    //足回りコンフィグ
    namespace Undercarrige
    {
        //足回りモーター設定
        constexpr MDID MOTOR_RIGHT = 1;
        constexpr MDID MOTOR_FRONT = 2;
        constexpr MDID MOTOR_LEFT = 3;
        constexpr MDID MOTOR_BACK = 4;

        //足回りロリコン設定
        constexpr LOLIID MOTOR_LOLI_RIGHT = 1;
        constexpr LOLIID MOTOR_LOLI_FRONT = 2;
        constexpr LOLIID MOTOR_LOLI_LEFT = 3;
        constexpr LOLIID MOTOR_LOLI_BACK = 4;
    }

    namespace Odometor
    {

    };

    namespace Canonn
    {

        typedef enum
        {
            RIGHT,
            LEFT
        } SELECT;

        constexpr PID::PidConfig PID_CANONN_ANGLE_RIGHT = {1, 0, 0};
        constexpr PID::PidConfig PID_CANONN_ANGLE_LEFT = {1, 0, 0};

        /*仰角*/
        //仰角 右
        constexpr MDID MOTOR_ANGLE_RIGHT = 0;
        //仰角 左
        constexpr MDID MOTOR_ANGLE_LEFT = 0;

        //仰角　ロリコン
        constexpr LOLIID ANGLE_LOLI_RIGHT = 0;
        //仰角　ロリコン
        constexpr LOLIID ANGLE_LOLI_LEFT = 0;

        //仰角 リミットスイッチ前 右
        constexpr SWID ANGLE_LIMIT_SW_RIGHT_F = 0;
        //仰角 リミットスイッチ後 右
        constexpr SWID ANGLE_LIMIT_SW_RIGHT_B = 0;
        //仰角 リミットスイッチ前 左
        constexpr SWID ANGLE_LIMIT_SW_LEFT_F = 0;
        //仰角 リミットスイッチ後 左
        constexpr SWID ANGLE_LIMIT_SW_LEFT_B = 0;

        //仰角上げ上限 右
        constexpr int MOTOR_ANGLE_LIMIT_RIGHT = 168;
        //仰角上げ上限 左
        constexpr int MOTOR_ANGLE_LIMIT_LEFT = 0;

        /*巻取り*/
        //ワイヤーの巻取り 右
        constexpr MDID MOTOR_WINDING_RIGHT = 0;
        //ワイヤーの巻取り 左
        constexpr MDID MOTOR_WINDING_LEFT = 0;

        //正転(巻取り方向) 右
        constexpr int POWER_WINDING_RIGHT = 0;
        //正転(巻取り方向) 左
        constexpr int POWER_WINDING_LEFT = 0;

        /*装填機構*/
        //装填機構回転 右
        constexpr int MOTOR_WHEEL_RIGHT = 0;
        //装填機構回転 左
        constexpr int MOTOR_WHEEL_LEFT = 0;

        //サーボ 右
        constexpr int SERVO_WINDING_RIGHT = 0;
        //サーボ 左
        constexpr int SERVO_WINDING_LEFT = 0;

        //サーボ 右 角度 0 -> 90度 [0, 90]
        constexpr int SERVO_ANGLE_RIGHT[2] = {0};
        //サーボ 左 角度 0 -> 90度 [0, 90]
        constexpr int SERVO_ANGLE_LEFT[2] = {0};
    };
}; // namespace MachineConfig

#endif