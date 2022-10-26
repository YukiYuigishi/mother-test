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
        constexpr LOLIID LOLI_MOTOR_RIGHT = 1;
        constexpr LOLIID LOLI_MOTOR_FRONT = 2;
        constexpr LOLIID LOLI_MOTOR_LEFT = 3;
        constexpr LOLIID LOLI_MOTOR_BACK = 4;
    }

    namespace Odometor
    {
        constexpr LOLIID LOLI_1 = 0;
        constexpr LOLIID LOLI_2 = 0;
        constexpr LOLIID LOLI_3 = 0;
        constexpr LOLIID LOLI_4 = 0;

    };

    namespace Canonn
    {

        /*
                typedef enum
                {
                    RIGHT,
                    LEFT
                } SELECT;
                */
        enum SELECT
        {
            RIGHT,
            LEFT
        };

        constexpr PID::PidConfig PID_CANONN_ANGLE_RIGHT = {1, 0, 0};
        constexpr PID::PidConfig PID_CANONN_ANGLE_LEFT = {1, 0, 0};

        /*仰角*/
        //仰角 右
        constexpr MDID ANGLE_MOTOR_RIGHT = 5;
        //仰角 左
        constexpr MDID ANGLE_MOTOR_LEFT = 0;

        //仰角　ロリコン
        constexpr LOLIID ANGLE_LOLI_RIGHT = 8;
        //仰角　ロリコン
        constexpr LOLIID ANGLE_LOLI_LEFT = 0;

        //仰角 リミットスイッチ前 右
        constexpr SWID ANGLE_LIMIT_SW_RIGHT_F = 1;
        //仰角 リミットスイッチ後 右
        constexpr SWID ANGLE_LIMIT_SW_RIGHT_B = 0;
        //仰角 リミットスイッチ前 左
        constexpr SWID ANGLE_LIMIT_SW_LEFT_F = 0;
        //仰角 リミットスイッチ後 左
        constexpr SWID ANGLE_LIMIT_SW_LEFT_B = 0;

        //仰角上げ上限 右
        constexpr int ANGLE_MOTOR_LIMIT_RIGHT = 168;
        //仰角上げ上限 左
        constexpr int ANGLE_MOTOR_LIMIT_LEFT = 0;

        /*巻取り*/
        //ワイヤーの巻取り 右
        constexpr MDID WINDING_MOTOR_RIGHT = 0;
        //ワイヤーの巻取り 左
        constexpr MDID WINDING_MOTOR_LEFT = 0;

        //正転(巻取り方向) 右
        constexpr int WINDING_POWER_RIGHT = 1400;
        //正転(巻取り方向) 左
        constexpr int WINDING_POWER_LEFT = -1400;

        /*装填機構*/
        //装填機構回転 右
        constexpr int WHEEL_MOTOR_RIGHT = 0;
        //装填機構回転 左
        constexpr int WHEEL_MOTOR_LEFT = 0;

        //サーボ 右
        constexpr int WINDING_SERVO_RIGHT = 0;
        //サーボ 左
        constexpr int WINDING_SERVO_LEFT = 0;

        //サーボ 右 角度 0 -> 90度 [0, 90]
        constexpr int LOCK_SERVO_ANGLE_RIGHT[2] = {0};
        //サーボ 左 角度 0 -> 90度 [0, 90]
        constexpr int LOCK_SERVO_ANGLE_LEFT[2] = {0};
    };
}; // namespace MachineConfig

#endif