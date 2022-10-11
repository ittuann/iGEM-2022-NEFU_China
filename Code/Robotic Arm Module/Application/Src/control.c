/*
 *  control.c
 *
 *  Created on: 2022��04��14��
 *      Author: LBQ
 */

#include "control.h"

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern UART_HandleTypeDef huart2;

#define SERVO_NUM 6
#define ActionGroups1_NodesNum 7		// ������1�ŵĽڵ���

uint8_t NodesNum = 0;					// ��ǰ�����ڵ����
float AngleSetp[SERVO_NUM] = {0};		// �ݴ�ÿ�����ÿ��Ӧת�Ƕ�
uint16_t AnglCont[SERVO_NUM] = {0};		// �ݴ�ÿ���������
bool_t ServoFlag[SERVO_NUM] = {0};		// �ݴ�ÿ������Ƿ����ת����־λ
uint8_t receive_data = 0;

// ���ýǶ� ����ת����ָ���Ƕ�����ʱ��
float ActionGroups1[ActionGroups1_NodesNum][SERVO_NUM][2] = {
	{{10.0f, 0}, {10.0f, 0}, {10.0f, 0}, {10.0f, 0}, {10.0f, 0}, {10.0f, 0}},				// ��ʼֵ
	{{20.0f, 1000}, {10.0f, 100}, {10.0f, 100}, {10.0f, 100}, {10.0f, 100}, {10.0f, 100}},	// �ڵ�1
	{{30.0f, 1000}, {10.0f, 100}, {10.0f, 100}, {10.0f, 100}, {10.0f, 100}, {10.0f, 100}},
	{{40.0f, 1000}, {10.0f, 100}, {10.0f, 100}, {10.0f, 100}, {10.0f, 100}, {10.0f, 100}},
	{{50.0f, 1000}, {10.0f, 100}, {10.0f, 100}, {10.0f, 100}, {10.0f, 100}, {10.0f, 100}},
	{{60.0f, 1000}, {10.0f, 100}, {10.0f, 100}, {10.0f, 100}, {10.0f, 100}, {10.0f, 100}},
	{{70.0f, 1000}, {10.0f, 100}, {10.0f, 100}, {10.0f, 100}, {10.0f, 100}, {10.0f, 100}}
};	// ������1��
float TestGroups[SERVO_NUM] = {50.0f, 60.0f, 70.0f, 80.0f, 90.0f, 100.0f};	// ���Ե����Ƕ���

// ��������
void ServoPWM_Init(void);
void SetServoAngle2PWM(float angle, uint8_t servonum);

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart == &huart2)
	{
		HAL_UART_Receive_IT(&huart2, &receive_data, 1);

		if (receive_data == 49) {
			
		} else if (receive_data == 50) {
			
		} else if (receive_data == 51) {
			
		}
	}
}

void StartDefaultTask(void const * argument)
{
  for(;;)
  {
	  HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
	  osDelay(500);
  }
}

void ControlTaskFunction(void const * argument)
{
	portTickType xLastWakeTime;
	const uint16_t TaskTim = 50;
	const portTickType xFrequency = pdMS_TO_TICKS(TaskTim);	// ������ʱ

	ServoPWM_Init();

	// �õ�ǰtickʱ���ʼ�� pxPreviousWakeTime
	xLastWakeTime = xTaskGetTickCount();

	while (1)
	{
		// ���������ʱ
		vTaskDelayUntil(&xLastWakeTime, xFrequency);

		if (NodesNum < ActionGroups1_NodesNum && NodesNum)
		{
			for (uint8_t s = 0; s < SERVO_NUM; s ++ )
			{
				AngleSetp[s] = ActionGroups1[NodesNum - 1][s][0] + (ActionGroups1[NodesNum][s][0] - ActionGroups1[NodesNum - 1][s][0]) * AnglCont[s] * TaskTim / ActionGroups1[NodesNum][s][1];
				SetServoAngle2PWM(AngleSetp[s], s);
				
				// �ж��Ƿ���ת��ָ���Ƕ�
				if (AnglCont[s] < (uint16_t)(ActionGroups1[NodesNum][s][1]) / TaskTim) {
					AnglCont[s] ++ ;
				} else {
					ServoFlag[s] = 1;
				}
			}
		}
		
		if (!NodesNum)
		{
			for (uint8_t s = 0; s < SERVO_NUM; s ++ )
			{
				SetServoAngle2PWM(ActionGroups1[NodesNum][s][0], s);
			}
			NodesNum ++ ;
		}

		if (ServoFlag[0] && ServoFlag[1] && ServoFlag[2] && ServoFlag[3] && ServoFlag[4] && ServoFlag[5])
		{
			memset(AngleSetp, 0, sizeof(AngleSetp));
			memset(AnglCont, 0, sizeof(AnglCont));
			memset(ServoFlag, 0, sizeof(ServoFlag));
			NodesNum ++ ;										// ����ִ�ж�����
//			NodesNum = (NodesNum + 1) % ActionGroups1_NodesNum;	// ѭ��ִ�ж�����
		}
		
		// ���Ե����Ƕ���
//		for (uint8_t s = 0; s < SERVO_NUM; s ++ )
//		{
//			SetServoAngle2PWM(TestGroups[s], s);
//		}
		
	}
}

// ��������
void ServoPWM_Init(void)
{
	uint16_t InitialValue = 500;
	HAL_TIM_Base_Start(&Servo1_TIM);
	HAL_TIM_Base_Start(&Servo2_TIM);
	HAL_TIM_Base_Start(&Servo3_TIM);
	HAL_TIM_Base_Start(&Servo4_TIM);
	HAL_TIM_Base_Start(&Servo5_TIM);
	HAL_TIM_Base_Start(&Servo6_TIM);
	HAL_TIM_PWM_Start(&Servo1_TIM, Servo1_Channel);
	HAL_TIM_PWM_Start(&Servo2_TIM, Servo2_Channel);
	HAL_TIM_PWM_Start(&Servo3_TIM, Servo3_Channel);
	HAL_TIM_PWM_Start(&Servo4_TIM, Servo4_Channel);
	HAL_TIM_PWM_Start(&Servo5_TIM, Servo5_Channel);
	HAL_TIM_PWM_Start(&Servo6_TIM, Servo6_Channel);
	__HAL_TIM_SetCompare(&Servo1_TIM, Servo1_Channel, InitialValue);
	__HAL_TIM_SetCompare(&Servo2_TIM, Servo2_Channel, InitialValue);
	__HAL_TIM_SetCompare(&Servo3_TIM, Servo3_Channel, InitialValue);
	__HAL_TIM_SetCompare(&Servo4_TIM, Servo4_Channel, InitialValue);
	__HAL_TIM_SetCompare(&Servo5_TIM, Servo5_Channel, InitialValue);
	__HAL_TIM_SetCompare(&Servo6_TIM, Servo6_Channel, InitialValue);
}

void SetServoAngle2PWM(float angle, uint8_t servonum)
{
	int16_t arr = 0;
	switch (servonum) {
		case 0 : arr = __HAL_TIM_GetAutoreload(&Servo1_TIM); break;
		case 1 : arr = __HAL_TIM_GetAutoreload(&Servo2_TIM); break;
		case 2 : arr = __HAL_TIM_GetAutoreload(&Servo3_TIM); break;
		case 3 : arr = __HAL_TIM_GetAutoreload(&Servo4_TIM); break;
		case 4 : arr = __HAL_TIM_GetAutoreload(&Servo5_TIM); break;
		case 5 : arr = __HAL_TIM_GetAutoreload(&Servo6_TIM); break;
		default : break;
	}
	
//	float duty = (0.5f + angle / 90.0f) / 20.0f;
//	float ccr = (float)(arr) * duty + 1.0f;
	float ccr = (float)(arr) * ((0.5f + angle / 90.0f) / 20.0f) + 1.0f;
	
	switch (servonum) {
		case 0 : __HAL_TIM_SetCompare(&Servo1_TIM, Servo1_Channel, (uint16_t)(ccr)); break;
		case 1 : __HAL_TIM_SetCompare(&Servo2_TIM, Servo2_Channel, (uint16_t)(ccr)); break;
		case 2 : __HAL_TIM_SetCompare(&Servo3_TIM, Servo3_Channel, (uint16_t)(ccr)); break;
		case 3 : __HAL_TIM_SetCompare(&Servo4_TIM, Servo4_Channel, (uint16_t)(ccr)); break;
		case 4 : __HAL_TIM_SetCompare(&Servo5_TIM, Servo5_Channel, (uint16_t)(ccr)); break;
		case 5 : __HAL_TIM_SetCompare(&Servo6_TIM, Servo6_Channel, (uint16_t)(ccr)); break;
		default : break;
	}
}

