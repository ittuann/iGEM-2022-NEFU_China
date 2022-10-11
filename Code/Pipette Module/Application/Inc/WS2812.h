/*
 * WS2812.h
 *
 *  Created on: 2022Äê05ÔÂ20ÈÕ
 *      Author: LBQ
 */
 
#ifndef _WS2812_H_
#define _WS2812_H_

#include "main.h"
#include "spi.h"
#include "dma.h"
#include "user_lib.h"

typedef struct RGBTypeDef RGBTypeDef_t;

extern RGBTypeDef_t WS2812_RGB;

extern void WS2812_Show_RGB(void);
extern void Set_WS2812_RGB(RGBTypeDef_t* RGB, uint8_t num, uint8_t val);

#endif

