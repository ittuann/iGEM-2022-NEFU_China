/*
 * 28BYJ48.h
 *
 *  Created on: 2022Äê05ÔÂ20ÈÕ
 *      Author: LBQ
 */
 
#ifndef _ULN2003_H_
#define _ULN2003_H_

#include <math.h>
#include "main.h"
#include "gpio.h"
#include "stm32f1xx_hal_gpio.h"
#include "user_lib.h"

extern void SteppingMotor_TurnAngle(float angle, uint8_t delay);
extern void SteppingMotor_Stop(void);

#endif
