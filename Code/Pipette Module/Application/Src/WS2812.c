/*
 * 28BYJ48.c
 *
 *  Created on: 2022��05��20��
 *      Author: LBQ
 */

#include "WS2812.h"

extern SPI_HandleTypeDef hspi1;
extern DMA_HandleTypeDef hdma_spi1_tx;

#define LED_NUM		31	// LED�������
#define RESET_HEAD	280	// Rest��λ�����賤��

// ģ��bit��:0x80Ϊ�߼�0, 0xF8Ϊ�߼�1
const uint8_t code[2] = {0xC0, 0xF8};

typedef struct RGBTypeDef {
  uint8_t R;
  uint8_t G;
  uint8_t B;
} RGBTypeDef_t;	//��ɫ�ṹ��

RGBTypeDef_t RGB_Data[LED_NUM] = {0};					// ���õ�����ɫ
uint8_t RGB_BUFFER[LED_NUM * 24 + RESET_HEAD] = {0};	// �������ݻ���

// ������ɫ����
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
  * @brief			����ָ������Ҫ��ʾ����ɫ
  * @param[in]		ID ��ɫ
  */
void Set_LEDColor(uint16_t LedId, RGBTypeDef_t Color)
{
    RGB_Data[LedId].G = Color.G;
    RGB_Data[LedId].R = Color.R;
    RGB_Data[LedId].B = Color.B;
}

/**
  * @brief			ˢ��ȫ��WS2812������ɫ
  * @param[in]		
  */
void RGB_Reflash(void)
{
	uint8_t dat_g = 0, dat_r = 0,dat_b = 0;
	// ��������ɫת��Ϊ24��Ҫ���͵��ֽ�����
    for (uint16_t i = 0; i < LED_NUM; i ++ ) {
		// ������ɫ����
        dat_g = RGB_Data[i].G;
        dat_r = RGB_Data[i].R;
        dat_b = RGB_Data[i].B;
		// ת��Ϊ0/1���ͨ��Э��
        for (uint8_t j = 0; j < 8; j ++ ) {
            RGB_BUFFER[RESET_HEAD + i * 24 + 7 - j] = code[dat_g & 0x01];
            RGB_BUFFER[RESET_HEAD + i * 24 + 15 - j] = code[dat_r & 0x01];
            RGB_BUFFER[RESET_HEAD + i * 24 + 23 - j] = code[dat_b & 0x01];
            dat_g >>= 1;
            dat_r >>= 1;
            dat_b >>= 1;
        }
	}
	// �ȴ��ϴ�DMA�������
	while (HAL_DMA_GetState(&hdma_spi1_tx) != HAL_DMA_STATE_READY);
	// SPI+DMA������������
	HAL_SPI_Transmit_DMA(&hspi1, RGB_BUFFER, LED_NUM * 24 + RESET_HEAD);
}

/**
  * @brief			����WS2812ȫ����ʾΪ��ɫ
  */
void WS2812_Show_RGB(void)
{
	for (uint16_t i = 0; i < LED_NUM; i ++ ) {
		Set_LEDColor(i, WS2812_RGB);
	}
	RGB_Reflash();
}

