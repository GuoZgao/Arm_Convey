#include "stm32f10x.h"
#include "light.h"
#include "led.h"
#include "delay.h"
#include "encode.h"


/* ����ͣ��״̬ */
void Enter_StopMode(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	   //ʹ��PWR����ʱ��
	PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
}


/* ����ģʽ */
void Standby(void){
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	   //ʹ��PWR����ʱ��
	PWR_WakeUpPinCmd(ENABLE);                              //ʹ�ܻ��ѹܽŹ���
	PWR_EnterSTANDBYMode();	  
}

/* �������״̬ */
void Enter_Standby(void){
	RCC_APB2PeriphResetCmd(0x01FC, DISABLE);	               //��λ����IO��
	Standby();
}


u8 Check_WKUP(void){                //�˿�ɨ��
	if (!WAKEUP) {
		delay_ms(100);
		if(!WAKEUP){
      return 1;			
		} else {
			return 0;
		}
	} else {
		return 0;
	}
}  


/* �жϺ��� */
void EXTI0_IRQHandler(void)
{ 		    		    				     		   
  EXTI_ClearITPendingBit(EXTI_Line0);	
	if(Check_WKUP()){
		SystemInit();                     //ϵͳʱ�ӳ�ʼ��
	}  
} 


/* �жϳ�ʼ�� */
void LIGHT_Init(void){
	GPIO_InitTypeDef  GPIO_InitStructure;  		  
	NVIC_InitTypeDef  NVIC_InitStructure;
	EXTI_InitTypeDef  EXTI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIOA�͸��ù���ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	                      //PA.0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;                   //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);               	          //��ʼ��IO
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);	    //�ж���0����GPIOA.0

  EXTI_InitStructure.EXTI_Line = EXTI_Line0;	                    //���ð������е��ⲿ��·
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			        //�����ⲿ�ж�ģʽ:EXTI��·Ϊ�ж�����
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;         //�½��ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	                                // ��ʼ���ⲿ�ж�

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;                //ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;       //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;              //�����ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                 //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);
	
	if (Check_WKUP())  Standby();                                   //һ��ʼ�������ģʽ
}



