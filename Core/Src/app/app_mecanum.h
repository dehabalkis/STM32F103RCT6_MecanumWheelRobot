#ifndef __APP_MECANUM_H__
#define __APP_MECANUM_H__

#include "stdint.h"

#define ROBOT_WIDTH            (169.0f)
#define ROBOT_LENGTH           (160.11f)
// #define MECANUM_APB         ((ROBOT_WIDTH + ROBOT_LENGTH)/2.0f)
#define MECANUM_APB            (164.555f)
#define MECANUM_CIRCLE_MM      (204.203f)
#define MECANUM_MINI_CIRCLE_MM (204.203f)
#define MECANUM_MAX_APB        (214.1f)
#define MECANUM_MAX_CIRCLE_MM  (251.327f)
#define MECANUM_MINI_APB       (174.5f)
#define MECANUM_SUNRISE_APB    (143.68f)
#define CAR_X3_PLUS_MAX_SPEED  (700)



void Mecanum_Ctrl(int16_t V_x, int16_t V_y, int16_t V_z, uint8_t adjust);
void Mecanum_State(uint8_t state, uint16_t speed, uint8_t adjust);
void Mecanum_Yaw_Calc(float yaw);

#endif /* __APP_MECANUM_H__ */