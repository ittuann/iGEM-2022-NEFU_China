/*
 *  anotc.h
 *
 *  Created on: 2021��11��5��
 *      Author: LBQ
 */

#include "anotc.h"

#define UserDataLen	4	// ���ݳ���

extern UART_HandleTypeDef huart2;

/*
 * @name:       void Wireless_Send(void)
 * @function:   Wireless_Send
 * @param:      none
 * @return:     none
 * @notion:     V7
 * @notion:     ������ߴ���һ�η���������ò�Ҫ����30�ֽ�
 */
void Wireless_Send(void)
{
    uint8_t waveform[6 + UserDataLen] = {0};// ����֡����
    uint8_t _cnt = 0;

//	memset(waveform, 0, sizeof(waveform));

    waveform[_cnt++] = 0xAA;        		// ֡ͷ
    waveform[_cnt++] = 0xFF;        		// Ŀ���ַ
    waveform[_cnt++] = 0xF1;				// ������ID
//	waveform[_cnt++] = sizeof(waveform) - 6;// ��Ч���ݳ���
//	waveform[_cnt++] = UserDataLen;
    waveform[_cnt++] = 0;

	int16_t UserData_1 = 0;
	int16_t UserData_2 = 0;
//	int16_t UserData_1 = motorData[0].speed_rpm;
//	int16_t UserData_2 = motorData[0].given_current;

    // ������
	// ʹ��С��ģʽ, ���ֽ���ǰ
    waveform[_cnt++] = BYTE0(UserData_1);   // ��������
    waveform[_cnt++] = BYTE1(UserData_1);
    waveform[_cnt++] = BYTE0(UserData_2);
    waveform[_cnt++] = BYTE1(UserData_2);

    waveform[3] = _cnt - 4; 				// д����Ч�����ֽ���

    uint8_t sumcheck = 0;   				// ��У��SC
    uint8_t addcheck = 0;   				// ����У��AC
    for(uint8_t i = 0; i < waveform[3] + 4; i++) {
      sumcheck += waveform[i];      		// ��֡ͷ��ʼ, һֱ�� data ������, ��ÿһ�ֽڽ����ۼӲ���, ֻȡ��8λ
      addcheck += sumcheck;         		// �����У��ʱ, ÿ����һ�ֽڵļӷ�����, ͬʱ����һ�� sumcheck ���ۼӲ���, ֻȡ��8λ
    }
    waveform[_cnt++] = sumcheck;
    waveform[_cnt++] = addcheck;

    // ���ڷ�������
//	for (uint8_t j = 0; j < (6 + UserDataLen); j++) {
//		HAL_UART_Transmit(&huart1, &waveform[j], sizeof(uint8_t), 1);
//	}
	HAL_UART_Transmit(&huart2, (uint8_t*)waveform, _cnt, 2);
}
