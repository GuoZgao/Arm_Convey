#include "stm32f10x.h"
#include "string.h"
#include "stdio.h"


char carry[50];

/* ���ݴ��͸�ʽ���� */
void data_encode(char *data){
	unsigned char length = strlen(data);
	unsigned char sum = 0x00;
	unsigned char i;
	carry[0] = 0x3C;       //��ʼλ            
  carry[1] = 0x02;       //����λ
	carry[2] = length;     //���͵�����λ��
	
  /* ���͵����� */	
	for (i = 0; i < length; ++i)
		carry[i + 3] = data[i];
	for (i = 0; i < length + 3; ++i)
		sum += carry[i];
	carry[length + 3] = sum;  //У���
	carry[length + 4] = 0x0D; //��β
			
	for (i = 0; i <= length + 4; ++i)
		printf("%c", carry[i]);	
}

