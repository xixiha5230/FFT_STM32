#include "timer.h"
#include "adc.h"
#include "usart.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "ledarray.h"


void AllClock_Init()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3|RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1
	|RCC_APB2Periph_GPIOB, ENABLE);	  //使能USART1，GPIOA, GPIOC时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
}


void ALL_NVIC_Configuration(void)
{
   NVIC_InitTypeDef NVIC_InitStructure;
	 
   /* Enable the TIM2 global Interrupt */
   NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
	
   /* Enable the TIM3 global Interrupt */
   NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	 NVIC_Init(&NVIC_InitStructure);

	   /* Enable the TIM4 global Interrupt */
   NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	 NVIC_Init(&NVIC_InitStructure);
	
			//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=7;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
}

void TIM2_Configuration(unsigned short per,unsigned short sca)
{
   	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_DeInit(TIM2);                              //复位TIM2定时器
        
    /* TIM2 configuration */
    TIM_TimeBaseStructure.TIM_Period = per;        //定时中断频率=72M/71+1/25     
    TIM_TimeBaseStructure.TIM_Prescaler = sca;    // 分频36000       
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  // 时钟分频  
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //计数方向向上计数
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
 
    /* Clear TIM2 update pending flag[清除TIM2溢出中断标志] */
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
 
    /* Enable TIM2 Update interrupt [TIM2溢出中断允许]*/
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); 
 
    /* TIM2 enable counter [允许tim2计数]*/
    TIM_Cmd(TIM2, ENABLE);   
}

volatile unsigned long long FreeRTOSRunTimeTicks;
void ConfigTimerForRunTimeStates(void)
{
	FreeRTOSRunTimeTicks = 0;
	TIM3_Configuration(72-1,500-1);
}

void TIM3_Configuration(unsigned short per,unsigned short sca)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_DeInit(TIM3);                              //复位TIM3定时器
	/* TIM3 configuration */
	TIM_TimeBaseStructure.TIM_Period = per;        //定时中断频率=72M/71+1/25     
	TIM_TimeBaseStructure.TIM_Prescaler = sca;    // 分频36000       
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  // 时钟分频  
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //计数方向向上计数
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	/* Clear TIM3 update pending flag[清除TIM3溢出中断标志] */
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	/* Enable TIM3 Update interrupt [TIM3溢出中断允许]*/
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); 
	/* TIM3 enable counter [允许tim3计数]*/
	TIM_Cmd(TIM3, ENABLE);   
}

void TIM4_Configuration(unsigned short per,unsigned short sca)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_DeInit(TIM4);                              //复位TIM3定时器
	/* TIM4 configuration */
	TIM_TimeBaseStructure.TIM_Period = per;        //定时中断频率=72M/71+1/25     
	TIM_TimeBaseStructure.TIM_Prescaler = sca;    // 分频36000       
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  // 时钟分频  
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //计数方向向上计数
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	/* Clear TIM4 update pending flag[清除TIM4溢出中断标志] */
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	/* Enable TIM4 Update interrupt [TIM4溢出中断允许]*/
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE); 
	/* TIM4 enable counter [允许tim4计数]*/
	TIM_Cmd(TIM4, DISABLE);   
}


extern u16 NPT_Cnt;
extern long lBufInArray[256];
extern SemaphoreHandle_t ADC_Semaphore; //adc二值信号量
extern SemaphoreHandle_t FFT_Semaphore; //fft二值信号量

void TIM2_IRQHandler(void)
{
	BaseType_t PriorityTaskWoken;
	extern __IO uint16_t ADCConvertedValue;
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){
		TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update);	//清中断
		/*采集256个点发送一次队列 队列内容为地址*/
		if(NPT_Cnt==255){
			NPT_Cnt = 0;
			if(FFT_Semaphore!=NULL){
				xSemaphoreGiveFromISR(FFT_Semaphore,&PriorityTaskWoken);
				portYIELD_FROM_ISR(PriorityTaskWoken);
			}     
		}else{		
			/*计数并采集数据*/
			ADC_SoftwareStartConvCmd(ADC1, ENABLE);
			while(!DMA_GetFlagStatus(DMA1_FLAG_TC1)){}
			/* Clear channel1 transfer complete flag */
			ADC_SoftwareStartConvCmd(ADC1, DISABLE);
			DMA_ClearFlag(DMA1_FLAG_TC1);
			lBufInArray[NPT_Cnt] =  ADCConvertedValue<<16;
			NPT_Cnt++;
		}
	}
}

void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){
		TIM_ClearITPendingBit(TIM3,TIM_FLAG_Update);
		FreeRTOSRunTimeTicks++;
	}
}
int fla =0;
extern long IBufLedArray[];
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET){
		TIM_ClearITPendingBit(TIM4,TIM_FLAG_Update);
		if(fla == 31){
			fla = 0;
		}else{
			fla++;
		}
		LedArray_Scan(fla);
	}
}
