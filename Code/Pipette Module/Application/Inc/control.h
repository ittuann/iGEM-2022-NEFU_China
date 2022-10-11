/*
 *  control.h
 *
 *  Created on: 2022Äê04ÔÂ14ÈÕ
 *      Author: LBQ
 */

#ifndef _CONTROL_H_
#define _CONTROL_H_

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "28BYJ48.h"
#include "WS2812.h"

#define SG_PRESS_HANDLE		htim1
#define SG_PRESS_CHANNEL	TIM_CHANNEL_1
#define SG_ROTATE_HANDLE	htim1
#define SG_ROTATE_CHANNEL	TIM_CHANNEL_2

extern uint8_t receive_data;

#endif /* CODE_CONTROL_H_ */
