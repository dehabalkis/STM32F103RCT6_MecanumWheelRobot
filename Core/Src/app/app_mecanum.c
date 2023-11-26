// #include "app_mecanum.h"
// #include "app_motion.h"
// #include "app_bat.h"
// #include "app_pid.h"

// #include "app.h"

// #include "stdint.h"

// #include "bsp_usart.h"
// #include "bsp_motor.h"
//#include "bsp_common.h"

#include "common/common_types.h"

static float speed_lr = 0;
static float speed_fb = 0;
static float speed_spin = 0;

static int speed_L1_setup = 0;
static int speed_L2_setup = 0;
static int speed_R1_setup = 0;
static int speed_R2_setup = 0;

static int g_offset_yaw = 0;
static uint16_t g_speed_setup = 0;

void Mecanum_Control(int16_t V_x, int16_t V_y, int16_t V_z, uint8_t adjust)
{
    float robot_APB = Motion_Get_APB();
    speed_lr = -V_y;
    speed_fb = V_x;
    speed_spin = (-V_z / 1000.0f) * robot_APB;
    if (V_x == 0 && V_y == 0 && V_z == 0)
    {
        Motion_Stop(STOP_BRAKE);
        return;
    }

    speed_L1_setup = speed_fb + speed_lr + speed_spin;
    speed_L2_setup = speed_fb - speed_lr + speed_spin;
    speed_R1_setup = speed_fb - speed_lr - speed_spin;
    speed_R2_setup = speed_fb + speed_lr - speed_spin;

    if (Motion_Get_Car_Type() == CAR_MECANUM_MAX)
    {
        if (speed_L1_setup > CAR_X3_PLUS_MAX_SPEED) speed_L1_setup = CAR_X3_PLUS_MAX_SPEED;
        if (speed_L1_setup < -CAR_X3_PLUS_MAX_SPEED) speed_L1_setup = -CAR_X3_PLUS_MAX_SPEED;
        if (speed_L2_setup > CAR_X3_PLUS_MAX_SPEED) speed_L2_setup = CAR_X3_PLUS_MAX_SPEED;
        if (speed_L2_setup < -CAR_X3_PLUS_MAX_SPEED) speed_L2_setup = -CAR_X3_PLUS_MAX_SPEED;
        if (speed_R1_setup > CAR_X3_PLUS_MAX_SPEED) speed_R1_setup = CAR_X3_PLUS_MAX_SPEED;
        if (speed_R1_setup < -CAR_X3_PLUS_MAX_SPEED) speed_R1_setup = -CAR_X3_PLUS_MAX_SPEED;
        if (speed_R2_setup > CAR_X3_PLUS_MAX_SPEED) speed_R2_setup = CAR_X3_PLUS_MAX_SPEED;
        if (speed_R2_setup < -CAR_X3_PLUS_MAX_SPEED) speed_R2_setup = -CAR_X3_PLUS_MAX_SPEED;
    }
    else
    {
        if (speed_L1_setup > 1000) speed_L1_setup = 1000;
        if (speed_L1_setup < -1000) speed_L1_setup = -1000;
        if (speed_L2_setup > 1000) speed_L2_setup = 1000;
        if (speed_L2_setup < -1000) speed_L2_setup = -1000;
        if (speed_R1_setup > 1000) speed_R1_setup = 1000;
        if (speed_R1_setup < -1000) speed_R1_setup = -1000;
        if (speed_R2_setup > 1000) speed_R2_setup = 1000;
        if (speed_R2_setup < -1000) speed_R2_setup = -1000;
    }

    Motion_Set_Speed(speed_L1_setup, speed_L2_setup, speed_R1_setup, speed_R2_setup);
}

void Mecanum_Yaw_Calc(float yaw)
{
    float yaw_offset = PID_Yaw_Calc(yaw);
    g_offset_yaw = yaw_offset * g_speed_setup;

    #if ENABLE_DEBUG_YAW
    static int aaaaaaaaa = 0;
    aaaaaaaaa++;
    if (aaaaaaaaa > 5)
    {
        aaaaaaaaa = 0;
        printf("Yaw Calc:%.5f, %d", yaw_offset, g_offset_yaw);
    }
    #endif

    int speed_L1 = speed_L1_setup - g_offset_yaw;
    int speed_L2 = speed_L2_setup - g_offset_yaw;
    int speed_R1 = speed_R1_setup + g_offset_yaw;
    int speed_R2 = speed_R2_setup + g_offset_yaw;

    if (Motion_Get_Car_Type() == CAR_MECANUM_MAX)
    {
        if (speed_L1 > CAR_X3_PLUS_MAX_SPEED) speed_L1 = CAR_X3_PLUS_MAX_SPEED;
        if (speed_L1 < -CAR_X3_PLUS_MAX_SPEED) speed_L1 = -CAR_X3_PLUS_MAX_SPEED;
        if (speed_L2 > CAR_X3_PLUS_MAX_SPEED) speed_L2 = CAR_X3_PLUS_MAX_SPEED;
        if (speed_L2 < -CAR_X3_PLUS_MAX_SPEED) speed_L2 = -CAR_X3_PLUS_MAX_SPEED;
        if (speed_R1 > CAR_X3_PLUS_MAX_SPEED) speed_R1 = CAR_X3_PLUS_MAX_SPEED;
        if (speed_R1 < -CAR_X3_PLUS_MAX_SPEED) speed_R1 = -CAR_X3_PLUS_MAX_SPEED;
        if (speed_R2 > CAR_X3_PLUS_MAX_SPEED) speed_R2 = CAR_X3_PLUS_MAX_SPEED;
        if (speed_R2 < -CAR_X3_PLUS_MAX_SPEED) speed_R2 = -CAR_X3_PLUS_MAX_SPEED;
    }
    else
    {
        if (speed_L1 > 1000) speed_L1 = 1000;
        if (speed_L1 < -1000) speed_L1 = -1000;
        if (speed_L2 > 1000) speed_L2 = 1000;
        if (speed_L2 < -1000) speed_L2 = -1000;
        if (speed_R1 > 1000) speed_R1 = 1000;
        if (speed_R1 < -1000) speed_R1 = -1000;
        if (speed_R2 > 1000) speed_R2 = 1000;
        if (speed_R2 < -1000) speed_R2 = -1000;
    }
    
    Motion_Set_Speed(speed_L1, speed_L2, speed_R1, speed_R2);
}

void Mecanum_State(uint8_t state, uint16_t speed, uint8_t adjust)
{
    Motion_Set_Yaw_Adjust(adjust);

    g_speed_setup = speed;

    switch (state)
    {
    case MOTION_STOP:
        g_speed_setup = 0;
        Motion_Stop(speed==0?STOP_FREE:STOP_BRAKE);
        break;
    case MOTION_RUN:
        Mecanum_Control(speed, 0, 0, adjust);
        break;
    case MOTION_BACK:
        Mecanum_Control(-speed, 0, 0, adjust);
        break;
    case MOTION_LEFT:
        Mecanum_Control(0, speed, 0, adjust);
        break;
    case MOTION_RIGHT:
        Mecanum_Control(0, -speed, 0, adjust);
        break;
    case MOTION_SPIN_LEFT:
        Motion_Set_Yaw_Adjust(0);
        Mecanum_Control(0, 0, speed*5, 0);
        break;
    case MOTION_SPIN_RIGHT:
        Motion_Set_Yaw_Adjust(0);
        Mecanum_Control(0, 0, -speed*5, 0);
        break;
    case MOTION_BRAKE:
        Motion_Stop(STOP_BRAKE);
        break;
    default:
        break;
    }
}