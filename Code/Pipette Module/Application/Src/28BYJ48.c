/*
 * 28BYJ48.c
 *
 *  Created on: 2022年05月20日
 *      Author: LBQ
 */

#include "28BYJ48.h"

#define MOTOR_PORT	GPIOA
#define MOTOR_IN1	GPIO_PIN_4
#define MOTOR_IN2	GPIO_PIN_5
#define MOTOR_IN3	GPIO_PIN_6
#define MOTOR_IN4	GPIO_PIN_7

#define IN1_HIGH	HAL_GPIO_WritePin(MOTOR_PORT, MOTOR_IN1, GPIO_PIN_SET);
#define IN1_LOW		HAL_GPIO_WritePin(MOTOR_PORT, MOTOR_IN1, GPIO_PIN_RESET);
#define IN2_HIGH	HAL_GPIO_WritePin(MOTOR_PORT, MOTOR_IN2, GPIO_PIN_SET);
#define IN2_LOW		HAL_GPIO_WritePin(MOTOR_PORT, MOTOR_IN2, GPIO_PIN_RESET);
#define IN3_HIGH	HAL_GPIO_WritePin(MOTOR_PORT, MOTOR_IN3, GPIO_PIN_SET);
#define IN3_LOW		HAL_GPIO_WritePin(MOTOR_PORT, MOTOR_IN3, GPIO_PIN_RESET);
#define IN4_HIGH	HAL_GPIO_WritePin(MOTOR_PORT, MOTOR_IN4, GPIO_PIN_SET);
#define IN4_LOW		HAL_GPIO_WritePin(MOTOR_PORT, MOTOR_IN4, GPIO_PIN_RESET);

/*
 * @function:   八拍驱动
 * @param:
 * @notion:     delay最小为2ms
 */
static void Phase8_Single(uint8_t step, uint8_t delay)
{
    switch (step) {
		case 0:	IN1_LOW;IN2_HIGH; IN3_HIGH; IN4_HIGH;	break;
		case 1:	IN1_LOW; IN2_LOW; IN3_HIGH; IN4_HIGH;	break;
		case 2:	IN1_HIGH; IN2_LOW; IN3_HIGH; IN4_HIGH;	break;
		case 3:	IN1_HIGH; IN2_LOW; IN3_LOW; IN4_HIGH;	break;
		case 4:	IN1_HIGH; IN2_HIGH; IN3_LOW; IN4_HIGH;	break;
		case 5:	IN1_HIGH; IN2_HIGH; IN3_LOW; IN4_LOW;	break;
		case 6:	IN1_HIGH; IN2_HIGH; IN3_HIGH; IN4_LOW;	break;
		case 7:	IN1_LOW; IN2_HIGH; IN3_HIGH; IN4_LOW;	break;
		default :	break;
	}
	HAL_Delay(delay);
//	vTaskDelay(delay);
}

/*
 * @function:   转动指定角度
 * @param:		dir: 0为正转 1为反转
 * @notion:     delay最小为2ms
 */
void SteppingMotor_TurnAngle(float angle, uint8_t delay)
{
//	float tmp = fabsf(angle) * 283712.0f / 4455.0f * 8.0f / 45.0f;	// 减速比1:63.684
	float tmp = fabsf(angle) * 5440.0f / 720.0f;					// 减速比1:42.5
	uint16_t steps = (uint16_t)tmp;
	
	if (angle > 0) {
		for (uint16_t i = steps; i > 0; i -- ) {
			Phase8_Single(i % 8, delay);
		}
	} else {
		for (uint16_t i = steps; i > 0; i -- ) {
			Phase8_Single(abs(i % 8 - 7), delay);
		}
	}
}

/*
 * @function:   步进电机停止
 */
void SteppingMotor_Stop(void)
{
	IN1_LOW;
	IN2_LOW;
	IN3_LOW;
	IN4_LOW;	
}
