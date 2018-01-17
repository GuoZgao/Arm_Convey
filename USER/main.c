#include "stm32f10x.h"
#include "led.h"
#include "sys.h"
#include "usart.h"
#include "light.h"
#include "delay.h"
#include "string.h"
#include "encode.h"


int main(void)
{	
	unsigned char times;
  delay_init();	    	                              //��ʱ������ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);   //�жϷ���   
	uart_init(115200);                                //���ڳ�ʼ��
	LED_Init();		
  LIGHT_Init();                                   	//ͣ��/������ʼ��
	
	
  for(; ;){
		LED0 = LED1 = 0;
		if (WAKEUP){                                    //����Ƿ��Ǹߵ�ƽ
		  times = 5;
			while(times--){                               //������ʱ
				delay_ms(1000);
			}
			if (WAKEUP){
				data_encode("1 1");                         //�������ݣ�������
				delay_ms(200);
				LED0 = LED1 = 1; 
				Enter_StopMode();                           //����ͣ��ģʽ
			} else {
				Enter_Standby();                            //�������ģʽ
			}
		} else {
			data_encode("0 1");                           //���뿪��
			delay_ms(2000);
			Enter_Standby();                              //�������ģʽ
		}
	}
}

