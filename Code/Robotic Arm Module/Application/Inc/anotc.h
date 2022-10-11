/*
 *  anotc.h
 *
 *  Created on: 2021��11��5��
 *      Author: LBQ
 */

#ifndef _ANO_DT_H_
#define _ANO_DT_H_

// Include
#include <string.h>
#include "main.h"
#include "user_lib.h"

// Define
// ���ݲ�ֺ궨��, �ڷ��ʹ���1�ֽڵ���������ʱ, ��Ҫ�����ݲ�ֳɵ����ֽڽ��з���
#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)    ) )   /* !< uint32_t ���ݲ�� byte0 */
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )   /* !< uint32_t ���ݲ�� byte1 */
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )   /* !< uint32_t ���ݲ�� byte2 */
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )   /* !< uint32_t ���ݲ�� byte3 */

// Function
extern  void Wireless_Send(void);

#endif /* CODE_ANOTC_H_ */
