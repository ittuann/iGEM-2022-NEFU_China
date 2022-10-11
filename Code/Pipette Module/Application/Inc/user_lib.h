/*
 * user_lib.h
 *
 *  Created on: 2021��10��24��
 *      Author: LBQ
 */
#ifndef USER_LIB_H
#define USER_LIB_H

// Includes
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "struct_typedef.h"
#include "main.h"

//#define DEBUGMODE 1

// Data Define
#define M_PI_F		(3.14159265358979f)
//#define PI			(3.14159265358979f)
#define ONE_PI		(3.14159265358979f)
#define EPS			(1e-6)

// Function Define
#define ABS(x)  (((x) > 0) ? (x) : (-(x)))                                                  // �궨��ʵ�ַ��ؾ���ֵ(x�ﲻ�����Լ��Լ�����䣬�����������)
#define LIMIT_MIN_MAX(x, min, max) ((x) = (((x)<=(min))?(min):(((x)>=(max))?(max):(x))))	// �޷�
#define LIMIT(x, min, max)			\
{									\
        if ((x) > (max)) {			\
            (x) = (max);			\
        } else if ((x) < (min)) {	\
            (x) = (min);			\
        }							\
    }
#define MIN(x, y)   (((x) < (y)) ? (x) : (y))                                               // ȡСֵ
#define MAX(x, y)   (((x) > (y)) ? (x) : (y))                                               // ȡ��ֵ
#define SIGN(x) (((x) > 0) ? 1 :-1) //((int32)(((x)>0?1:-1)*ceil(ABS((x)))))                // ȡ����
#define SWAP(x, y)   do{(x) ^= (y); (y) ^= (x); (x) ^= (y);} while(0)                       // ���� x, y ��ֵ
#define ARR_SIZE(a) ( sizeof( (a) ) / sizeof( ((a)[0]) ) )                                  // ��������Ԫ�صĸ���
#define OFFSET(type, member)    ((uint32_t)(&(((type *)0)->member)))						// ��ȡ�ṹ��ĳ��Աƫ��

// Type Define
typedef struct {
	float OriginData;
	float FilterData;
} LpfRC1st_t;

typedef struct {
	float A[3];
    float B[3];
    float OriginData[3];
    float FilterData[3];
} LpfIIR2nd_t;

typedef struct {
	uint32_t	NowTime;
	uint32_t	LastTime;
	uint8_t		SampleCount;
	float		Freq;
} Frequency_t;

typedef struct {
	int16_t SetVal;
	int16_t NowVal;
} Ramp_t;

// Function
extern	float	invSqrt(float number);
extern	float	LowPassFilterRC1st(LpfRC1st_t* lpf, float coefficient, float rawData);
extern	void	LowPassFilterIIR2nd(LpfIIR2nd_t* lpf_2nd, float rawData);
extern	float	GetFrequency(Frequency_t* ptr);
extern	float	RampCalc(Ramp_t *ptr, int16_t set, int16_t rate);
extern	void	swvPrint(uint8_t port, char *ptr);
//extern	int		U1_fputc(int ch, FILE *f);
extern	void	Delay_us(uint16_t us);

#endif

