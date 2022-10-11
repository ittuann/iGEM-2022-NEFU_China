/*
 *  control.c
 *
 *  Created on: 2022��04��14��
 *      Author: LBQ
 */

#include "control.h"

extern TIM_HandleTypeDef htim1;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

uint8_t receive_data = 0;
uint8_t Key_Exit_Flag = 0;		// ʹ�ñ�־λʵ����ѭ�����жϻص�����֮��Ļ���
								// ��֤�жϴ������еĹ���ֻ����ѭ������ж�֮����У�������ѭ�����ж�ֻ���жϴ���������֮���ٽ���

uint16_t Press_Val_No = 460;	// ������Һǹ
uint16_t Press_Val_Low = 900;	// �ᰴ��Һǹ
uint16_t Press_Val_High = 2280;	// �ذ���Һǹ
uint16_t Press_Val_Drop = 1000;	// ����Һǹǹͷ
uint16_t GunInitial = 500;		// ��Һǹ�����̳�ʼֵ
uint8_t Key_Number = 0;			// ��������

float GunRange = 5.0f;			// ��Һǹ���̵�λֵ
uint8_t GunAdjust_Flag = 0;		// ���̵�����־λ
uint8_t GunAdjusNum = 0;		// ���̵�������

uint8_t RGB_CaseFlag = 0;		// ���ڵ���RGB��ֵ��־λ

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
//	USART1_IRQHandler
//	if (huart->Instance == USART1)
//	if (huart1.Instance->SR & UART_FLAG_RXNE)
	if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE ) != RESET)
	{
		HAL_UART_Receive_IT(&huart1, &receive_data, 1);
		Set_WS2812_RGB(&WS2812_RGB, RGB_CaseFlag, receive_data);
		RGB_CaseFlag = (RGB_CaseFlag + 1) % 3;
	}

	if (huart == &huart2)
	{
//		receive_data = huart2.Instance->DR;
		HAL_UART_Receive_IT(&huart2, &receive_data, 1);

		if (receive_data == 48) {
			// '0'
			__HAL_TIM_SetCompare(&SG_PRESS_HANDLE, SG_PRESS_CHANNEL, Press_Val_No);
		} else if (receive_data == 49) {
			// '1'
			__HAL_TIM_SetCompare(&SG_PRESS_HANDLE, SG_PRESS_CHANNEL, Press_Val_Low);
		} else if (receive_data == 50) {
			// '2'
			__HAL_TIM_SetCompare(&SG_PRESS_HANDLE, SG_PRESS_CHANNEL, Press_Val_Drop);
		} else if (receive_data == 51) {
			// '3'
			__HAL_TIM_SetCompare(&SG_PRESS_HANDLE, SG_PRESS_CHANNEL, Press_Val_High);
		} else {
			// ����Һǹ����
//			__HAL_TIM_SetCompare(&SG_ROTATE_HANDLE, SG_ROTATE_CHANNEL, GunInitial + GunRange * receive_data);
			if (GunAdjust_Flag == 0) {
				GunAdjust_Flag = 1;
				GunAdjusNum = receive_data;
			}
		}

	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == KEY_Pin)
    {
        if (Key_Exit_Flag == 0)
        {
            Key_Exit_Flag = 1;
        }
    }
}

void StartDefaultTask(void const * argument)
{
  for(;;)
  {
	  HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
	  vTaskDelay(500 / portTICK_RATE_MS);
  }
}

void ControlTaskFunction(void const * argument)
{
	static portTickType xLastWakeTime;
	const portTickType xTimeIncrement = pdMS_TO_TICKS(10UL);				// ������ʱ

	HAL_TIM_Base_Start(&SG_PRESS_HANDLE);
	HAL_TIM_Base_Start(&SG_ROTATE_HANDLE);
	HAL_TIM_PWM_Start(&SG_PRESS_HANDLE, SG_PRESS_CHANNEL);				// ��ѹ
	HAL_TIM_PWM_Start(&SG_ROTATE_HANDLE, SG_ROTATE_CHANNEL);			// ������
	__HAL_TIM_SetCompare(&SG_PRESS_HANDLE, SG_PRESS_CHANNEL, 500);
	__HAL_TIM_SetCompare(&SG_ROTATE_HANDLE, SG_ROTATE_CHANNEL, GunInitial);
	
	// �õ�ǰtickʱ���ʼ�� pxPreviousWakeTime
	xLastWakeTime = xTaskGetTickCount();

	while (1)
	{
		// ���������ʱ
		vTaskDelayUntil(&xLastWakeTime, xTimeIncrement);

		
		// ����������ѹ���
		if (Key_Exit_Flag == 1)
		{
//			Key_Exit_Flag = 2;
			vTaskDelay(30);		// ����
			if (HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin) == GPIO_PIN_RESET) {
				Key_Number ++ ;
				Key_Exit_Flag = 0;
			} else {
				Key_Exit_Flag = 0;
			}
		}
		
		if (Key_Number == 1) {
			__HAL_TIM_SetCompare(&SG_PRESS_HANDLE, SG_PRESS_CHANNEL, Press_Val_Low);
		} else if (Key_Number == 2) {
			__HAL_TIM_SetCompare(&SG_PRESS_HANDLE, SG_PRESS_CHANNEL, Press_Val_High);
		} else if (Key_Number == 3) {
			__HAL_TIM_SetCompare(&SG_PRESS_HANDLE, SG_PRESS_CHANNEL, Press_Val_No);
		} else if (Key_Number == 4) {
			__HAL_TIM_SetCompare(&SG_PRESS_HANDLE, SG_PRESS_CHANNEL, Press_Val_Drop);
			Key_Number = 0;
		}

		// ������
		if (GunAdjust_Flag == 1) {
			SteppingMotor_TurnAngle(GunAdjusNum * GunRange, 2);
			SteppingMotor_Stop();
			GunAdjust_Flag = 0;
		}
		
//		WS2812_Show_BLUE();
	}
}
