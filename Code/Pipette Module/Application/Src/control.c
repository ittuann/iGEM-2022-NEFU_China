/*
 *  control.c
 *
 *  Created on: 2022年04月14日
 *      Author: LBQ
 */

#include "control.h"

extern TIM_HandleTypeDef htim1;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

uint8_t receive_data = 0;
uint8_t Key_Exit_Flag = 0;		// 使用标志位实现主循环和中断回调函数之间的互斥
								// 保证中断处理函数中的功能只在主循环完成判断之后进行，或者主循环的判断只在中断处理函数运行之后再进行

uint16_t Press_Val_No = 460;	// 不按移液枪
uint16_t Press_Val_Low = 900;	// 轻按移液枪
uint16_t Press_Val_High = 2280;	// 重按移液枪
uint16_t Press_Val_Drop = 1000;	// 打移液枪枪头
uint16_t GunInitial = 500;		// 移液枪满量程初始值
uint8_t Key_Number = 0;			// 按键次数

float GunRange = 5.0f;			// 移液枪量程单位值
uint8_t GunAdjust_Flag = 0;		// 量程调整标志位
uint8_t GunAdjusNum = 0;		// 量程调整数量

uint8_t RGB_CaseFlag = 0;		// 串口调整RGB数值标志位

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
			// 调移液枪量程
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
	const portTickType xTimeIncrement = pdMS_TO_TICKS(10UL);				// 绝对延时

	HAL_TIM_Base_Start(&SG_PRESS_HANDLE);
	HAL_TIM_Base_Start(&SG_ROTATE_HANDLE);
	HAL_TIM_PWM_Start(&SG_PRESS_HANDLE, SG_PRESS_CHANNEL);				// 下压
	HAL_TIM_PWM_Start(&SG_ROTATE_HANDLE, SG_ROTATE_CHANNEL);			// 调量程
	__HAL_TIM_SetCompare(&SG_PRESS_HANDLE, SG_PRESS_CHANNEL, 500);
	__HAL_TIM_SetCompare(&SG_ROTATE_HANDLE, SG_ROTATE_CHANNEL, GunInitial);
	
	// 用当前tick时间初始化 pxPreviousWakeTime
	xLastWakeTime = xTaskGetTickCount();

	while (1)
	{
		// 任务绝对延时
		vTaskDelayUntil(&xLastWakeTime, xTimeIncrement);

		
		// 按键控制下压舵机
		if (Key_Exit_Flag == 1)
		{
//			Key_Exit_Flag = 2;
			vTaskDelay(30);		// 消抖
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

		// 调量程
		if (GunAdjust_Flag == 1) {
			SteppingMotor_TurnAngle(GunAdjusNum * GunRange, 2);
			SteppingMotor_Stop();
			GunAdjust_Flag = 0;
		}
		
//		WS2812_Show_BLUE();
	}
}
