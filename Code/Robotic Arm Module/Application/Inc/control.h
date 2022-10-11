/*
 *  control.h
 *
 *  Created on: 2022Äê04ÔÂ14ÈÕ
 *      Author: LBQ
 */

#ifndef _CONTROL_H_
#define _CONTROL_H_

#include "main.h"
#include "user_lib.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "task.h"

#define Servo1_TIM		htim2
#define Servo1_Channel	TIM_CHANNEL_2
#define Servo2_TIM		htim4
#define Servo2_Channel	TIM_CHANNEL_3
#define Servo3_TIM		htim4
#define Servo3_Channel	TIM_CHANNEL_4
#define Servo4_TIM		htim4
#define Servo4_Channel	TIM_CHANNEL_1
#define Servo5_TIM		htim4
#define Servo5_Channel	TIM_CHANNEL_2
#define Servo6_TIM		htim3
#define Servo6_Channel	TIM_CHANNEL_1

extern uint8_t receive_data;

#endif /* CODE_CONTROL_H_ */
