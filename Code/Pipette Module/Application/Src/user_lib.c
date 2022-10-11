/*
 * user_lib.c
 *
 *  Created on: 2021��10��24��
 *      Author: LBQ
 */
#include "user_lib.h"

/**
  * @brief			���ټ���ƽ�����ĵ���
  * @param[in]		number
  * @notice			See: http://en.wikipedia.org/wiki/Fast_inverse_square_root
  */
float invSqrt(float number)
{
	const float x2 = number * 0.5F;
	const float threehalfs = 1.5F;
	union {
		float f;
		uint32_t i;
	} conv  = { .f = number };
	conv.i = 0x5f3759df - ( conv.i >> 1 );
	conv.f *= threehalfs - ( x2 * conv.f * conv.f );
	return conv.f;
}

/**
 * @brief			һ��RC��ͨ�˲�
 * @param[out]		lpf : �˲��ṹ����ָ��
 * @param[in]		coefficient : һ��RC��ͨ�˲�ϵ��(0-1)
 * @param[in]		rawData : ԭʼ����
 */
float LowPassFilterRC1st(LpfRC1st_t* lpf, float coefficient, float rawData)
{
	lpf->OriginData = rawData;
	return lpf->FilterData = coefficient * lpf->OriginData + (1.000f - coefficient) * lpf->FilterData;
}

/**
 * @brief			����IIR��ͨ�˲���
 * @param[out]		lpf : �˲��ṹ����ָ��
 * @param[in]		rawData : ԭʼ����
 * @notion			����ϵ���ǳɶԳ��ֵ� �����Ⱥϲ����������Ч��
 */
void LowPassFilterIIR2nd(LpfIIR2nd_t* lpf, float rawData)
{
	const uint8_t Order = 2;
	uint8_t i = 0;
	for (i = Order; i > 0; i--) {
		lpf->OriginData[i] = lpf->OriginData[i - 1];
		lpf->FilterData[i] = lpf->FilterData[i - 1];
	}
	lpf->OriginData[0] = rawData;
	lpf->FilterData[0] = lpf->B[0] * lpf->OriginData[0];	// NUM ����
	for (i = 1; i <= Order; i++) {
		lpf->FilterData[0] = lpf->FilterData[0] + lpf->B[i] * lpf->OriginData[i] - lpf->A[i] * lpf->FilterData[i];
	}
//	lpf->FilterData[0] = (lpf->B[0] * lpf->OriginData[0] + lpf->B[1] * lpf->OriginData[1] + lpf->B[2] * lpf->OriginData[2] - lpf->A[1] * lpf->FilterData[1] - lpf->A[2] * lpf->FilterData[2]) / lpf->A[0];
}

/**
 * @brief			��������Ƶ��
 * @param[out]		ptr : �ṹ��ָ��
 */
float GetFrequency(Frequency_t* ptr)
{
	if (ptr->SampleCount == 0) {
		ptr->LastTime = HAL_GetTick();
	}
	ptr->SampleCount ++;
	if (ptr->SampleCount > 20) {
		ptr->NowTime = HAL_GetTick();
		ptr->Freq = 1000.0f / ((float)(ptr->NowTime - ptr->LastTime) / (float)(ptr->SampleCount - 1));
		ptr->SampleCount = 0;
	}
	return ptr->Freq;
}

/**
 * @brief			б���������㣨���������ֵ������е���
 * @param[out]		ptr : �ṹ��ָ��
 * @param[in]		set : ����ֵ
 * @param[in]		rate : ÿ�����ӵ�����
 */
float RampCalc(Ramp_t *ptr, int16_t set, int16_t rate)
{
	ptr->SetVal = set;
	if (ptr->SetVal != ptr->NowVal) {
		if ((ptr->SetVal - ptr->NowVal) > 0.0f) {
			ptr->NowVal += rate;
		} else {
			ptr->NowVal -= rate;
		}
	}
	return ptr->NowVal;
}

/**
  * @brief			��SWV Console�����Ϣ
  * @param[in]		port 0~31
  * @param[in]		����
  * @example		swvPrint(1, "Print from CHx\n")
  */
void swvPrint(uint8_t port, char *ptr)
{
    for (uint16_t i = 0; i < strlen(ptr); i++) {
        while (ITM->PORT[port].u32 == 0)
        {
        }
        ITM->PORT[port].u8 = *(ptr + i);
    }
}

/**
  * @brief			printf�ض���
  */
//int U1_fputc (int ch, FILE *f)
//{
//    (void)HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 1000);
//    return ch;
//}

/**
  * @brief			us��ʱ
  * @param[in]		us
  */
void Delay_us(uint16_t us)
{
	const uint8_t fac_us = SystemCoreClock / 1000000;
//	const uint32_t fac_ms = SystemCoreClock / 1000;
	/*** Systick����ʵ�� SYSCLKΪϵͳʱ�� ***/
//    uint32_t ticks = 0;
//    uint32_t told = 0;
//    uint32_t tnow = 0;
//    uint32_t tcnt = 0;
//    uint32_t reload = 0;
//    reload = SysTick->LOAD;
//    ticks = us * fac_us;
//    told = SysTick->VAL;
//    while (1) {
//        tnow = SysTick->VAL;
//        if (tnow != told) {
//            if (tnow < told) {
//                tcnt += told - tnow;
//            } else {
//                tcnt += reload - tnow + told;
//            }
//            told = tnow;
//            if (tcnt >= ticks) {
//                break;
//            }
//        }
//    }

    /*** NOP�����ʵ�� ***/
    uint32_t delay = us * fac_us / 4;
	do {
		__NOP();
	}
	while (delay --);
}
