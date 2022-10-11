/*
 * 28BYJ48.c
 *
 *  Created on: 2022年05月20日
 *      Author: LBQ
 */

#include "WS2812.h"

extern SPI_HandleTypeDef hspi1;
extern DMA_HandleTypeDef hdma_spi1_tx;

#define LED_NUM		31	// LED灯珠个数
#define RESET_HEAD	280	// Rest复位码所需长度

// 模拟bit码:0x80为逻辑0, 0xF8为逻辑1
const uint8_t code[2] = {0xC0, 0xF8};

typedef struct RGBTypeDef {
  uint8_t R;
  uint8_t G;
  uint8_t B;
} RGBTypeDef_t;	//颜色结构体

RGBTypeDef_t RGB_Data[LED_NUM] = {0};					// 设置灯珠颜色
uint8_t RGB_BUFFER[LED_NUM * 24 + RESET_HEAD] = {0};	// 发送数据缓存

// 常见颜色定义
const RGBTypeDef_t WS2812_RED = {255, 0, 0};
const RGBTypeDef_t WS2812_GREEN = {0, 255, 0};
const RGBTypeDef_t WS2812_BLUE = {0, 0, 128};
RGBTypeDef_t WS2812_RGB = {10, 0, 10};

void Set_WS2812_RGB(RGBTypeDef_t* RGB, uint8_t num, uint8_t val)
{
	switch (num) {
		case 0 : RGB->R = val; break;
		case 1 : RGB->G = val; break;
		case 2 : RGB->B = val; break;
		default : break;
	}
}

/**
  * @brief			设置指定灯珠要显示的颜色
  * @param[in]		ID 颜色
  */
void Set_LEDColor(uint16_t LedId, RGBTypeDef_t Color)
{
    RGB_Data[LedId].G = Color.G;
    RGB_Data[LedId].R = Color.R;
    RGB_Data[LedId].B = Color.B;
}

/**
  * @brief			刷新全部WS2812灯珠颜色
  * @param[in]		
  */
void RGB_Reflash(void)
{
	uint8_t dat_g = 0, dat_r = 0,dat_b = 0;
	// 将数组颜色转化为24个要发送的字节数据
    for (uint16_t i = 0; i < LED_NUM; i ++ ) {
		// 存入颜色缓存
        dat_g = RGB_Data[i].G;
        dat_r = RGB_Data[i].R;
        dat_b = RGB_Data[i].B;
		// 转换为0/1码的通信协议
        for (uint8_t j = 0; j < 8; j ++ ) {
            RGB_BUFFER[RESET_HEAD + i * 24 + 7 - j] = code[dat_g & 0x01];
            RGB_BUFFER[RESET_HEAD + i * 24 + 15 - j] = code[dat_r & 0x01];
            RGB_BUFFER[RESET_HEAD + i * 24 + 23 - j] = code[dat_b & 0x01];
            dat_g >>= 1;
            dat_r >>= 1;
            dat_b >>= 1;
        }
	}
	// 等待上次DMA传输完成
	while (HAL_DMA_GetState(&hdma_spi1_tx) != HAL_DMA_STATE_READY);
	// SPI+DMA发送所有数据
	HAL_SPI_Transmit_DMA(&hspi1, RGB_BUFFER, LED_NUM * 24 + RESET_HEAD);
}

/**
  * @brief			配置WS2812全部显示为蓝色
  */
void WS2812_Show_RGB(void)
{
	for (uint16_t i = 0; i < LED_NUM; i ++ ) {
		Set_LEDColor(i, WS2812_RGB);
	}
	RGB_Reflash();
}

