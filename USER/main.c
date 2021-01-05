#include "stm32f10x.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "timer.h"
#include "adc.h"
#include "ledarray.h"
#include "fft.h"


#include "stm32_dsp.h"
#include "table_fft.h"

#include "FreeRTOS.h"
#include "task.h"
#include "list.h"
#include "queue.h"
#include "semphr.h"

/*快速傅里叶变换定义*/
#define NPT 256
void GetPowerMag(void);
long lBufInArray[256];
long lBufOutArray[256];
long lBufMagArray[128];
long IBufLedArray[64];
long IBufLedArray_tmp[64];
long IBufLedArray_Max[64];
long Red_Delay_Cnt[64];
long Green_Delay_Cnt[64];
u16 NPT_Cnt=0;
/********************/

//任务优先级
#define START_TASK_PRIO		1    //越高越优先
//任务堆栈大小	
#define START_STK_SIZE 		100  //大小是128*4字节
//任务句柄
TaskHandle_t StartTask_Handler;
//任务函数
void start_task(void *pvParameters);

//任务优先级
#define LED_TASK_PRIO		2
//任务堆栈大小	
#define LED_STK_SIZE 		70  
//任务句柄
TaskHandle_t LEDTask_Handler;
//任务函数 
void LED_task(void *pvParameters);

//任务优先级
#define FFT_TASK_PRIO		3
//任务堆栈大小	
#define FFT_STK_SIZE 		50  
//任务句柄
TaskHandle_t FFTTask_Handler;
//任务函数
void FFT_task(void *pvParameters);

//任务优先级
#define FFTMag_TASK_PRIO		4
//任务堆栈大小	
#define FFTMag_STK_SIZE 		50  
//任务句柄
TaskHandle_t FFTMagTask_Handler;
//任务函数
void FFTMag_task(void *pvParameters);

////任务优先级
//#define ADC_TASK_PRIO		5
////任务堆栈大小	
//#define ADC_STK_SIZE 		50  
////任务句柄
//TaskHandle_t ADCTask_Handler;
////任务函数
//void ADC_task(void *pvParameters);

/*定义二值信号量句柄*/
SemaphoreHandle_t FFT_Semaphore;
//SemaphoreHandle_t ADC_Semaphore;
SemaphoreHandle_t FFTMag_Semaphore;
SemaphoreHandle_t LED_Semaphore;
/*....................................*/

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);   //FreeRTOS推荐优先级4
	AllClock_Init();
	ALL_NVIC_Configuration();
	
	delay_init();
	uart_init(115200);
	ADC_Config();
//	ConfigTimerForRunTimeStates(); //50us 
	TIM2_Configuration(72-1,44-1);//100 10k 50 20k  采用率
	TIM4_Configuration(2500-1,9-1); //10ms 屏幕刷新
	LedArray_GPIOConfig();
	//LedArray_Clearn();
	memset(Green_Delay_Cnt,0,sizeof(Green_Delay_Cnt));
	
	//创建开始任务
  xTaskCreate((TaskFunction_t )start_task,            //任务函数
              (const char*    )"start_task",          //任务名称
              (uint16_t       )START_STK_SIZE,        //任务堆栈大小
              (void*          )NULL,                  //传递给任务函数的参数
              (UBaseType_t    )START_TASK_PRIO,       //任务优先级
              (TaskHandle_t*  )&StartTask_Handler);   //任务句柄           
  vTaskStartScheduler();          //开启任务调度
}


void start_task(void *pvParameters)
{
	taskENTER_CRITICAL();           //进入临界区
	/*初始化信号量*/
	FFT_Semaphore = xSemaphoreCreateBinary();
	if(FFT_Semaphore==NULL){
		printf("FFT_Semaphore Creat err!\r\n");
	}
//	ADC_Semaphore = xSemaphoreCreateBinary();
//	if(ADC_Semaphore==NULL){
//		printf("ADC_Semaphore Creat err!\r\n");
//	}
	FFTMag_Semaphore = xSemaphoreCreateBinary();
	if(FFTMag_Semaphore==NULL){
		printf("FFTMag_Semaphore Creat err!\r\n");
	}
	LED_Semaphore = xSemaphoreCreateBinary();
	if(LED_Semaphore==NULL){
		printf("FFTMag_Semaphore Creat err!\r\n");
	}
	/*........................................*/
	//创建任务
	xTaskCreate((TaskFunction_t )LED_task,     	    //任务函数地址
							(const char*    )"LED_task",   	    //任务名称
							(uint16_t       )LED_STK_SIZE,      //任务堆栈大小 LED_STK_SIZE*4 字节
							(void*          )NULL,				      //传入参数
							(UBaseType_t    )LED_TASK_PRIO,	    //优先级
							(TaskHandle_t*  )&LEDTask_Handler); //句柄
	//创建任务
	xTaskCreate((TaskFunction_t )FFT_task,     	
							(const char*    )"FFT_task",   	
							(uint16_t       )FFT_STK_SIZE, 
							(void*          )NULL,				
							(UBaseType_t    )FFT_TASK_PRIO,	
							(TaskHandle_t*  )&FFTTask_Handler);
							
//	xTaskCreate((TaskFunction_t )ADC_task,     	
//							(const char*    )"ADC_task",   	
//							(uint16_t       )ADC_STK_SIZE, 
//							(void*          )NULL,				
//							(UBaseType_t    )ADC_TASK_PRIO,	
//							(TaskHandle_t*  )&ADCTask_Handler);
							
	xTaskCreate((TaskFunction_t )FFTMag_task,     	
							(const char*    )"FFTMag_task",   	
							(uint16_t       )FFTMag_STK_SIZE, 
							(void*          )NULL,				
							(UBaseType_t    )FFTMag_TASK_PRIO,	
							(TaskHandle_t*  )&FFTMagTask_Handler);
							
	vTaskDelete(StartTask_Handler); //删除开始任务NULL删除自己
	printf("FreeRTOS Start!\r\n");
	taskEXIT_CRITICAL();            //退出临界区	
}
char RUNTIME[150];

void LED_task(void *pvParameters)
{
	int i=0;
	while(1){
		if(LED_Semaphore!=NULL){
			xSemaphoreTake(LED_Semaphore,portMAX_DELAY);
		}else{
			delay_ms(1000);
		}
		lBufMagArray[0] = lBufMagArray[1];
		for(i=0;i<64;i++){
			//IBufLedArray_tmp[i] = (lBufMagArray[2*i]>lBufMagArray[2*i+1] ? lBufMagArray[2*i]:lBufMagArray[2*i+1])*(32/100.0);
			
			IBufLedArray_tmp[i] = (((lBufMagArray[2*i]+lBufMagArray[2*i+1])/2)*(32/100.0));
			if(IBufLedArray_tmp[i]>31)
					IBufLedArray_tmp[i] = 31;
		}
		for(i=0;i<64;i++){
			if(IBufLedArray[i] < IBufLedArray_tmp[i]){
				IBufLedArray[i] = IBufLedArray_tmp[i];
				Red_Delay_Cnt[i] = 0;
			}else if(IBufLedArray[i] > IBufLedArray_tmp[i]){
				Red_Delay_Cnt[i]++;
				if(Red_Delay_Cnt[i]>=14){
					IBufLedArray[i]--;
				}
			}
		}
		
		for(i=0;i<64;i++){
			if(IBufLedArray_Max[i] < IBufLedArray_tmp[i]+1){
				IBufLedArray_Max[i] = IBufLedArray_tmp[i]+1;
				Green_Delay_Cnt[i] = 0;
			}else{
				Green_Delay_Cnt[i]++;
				if(Green_Delay_Cnt[i]>=60){  //绿灯延迟下落时间
					IBufLedArray_Max[i]--;
				}
			}
		}
		//delay_ms(15);
		TIM_Cmd(TIM4, ENABLE);
//		printf("LED_task Staclast= %d!!!\r\n",
//						(int)uxTaskGetStackHighWaterMark(LEDTask_Handler)); //显示剩余内存最少多少
//		printf("FFT_task Staclast= %d!!!\r\n",
//						(int)uxTaskGetStackHighWaterMark(FFTTask_Handler)); //显示剩余内存最少多少
//		printf("FFTMag_Task Staclast= %d!!!\r\n",
//				(int)uxTaskGetStackHighWaterMark(FFTMagTask_Handler)); //显示剩余内存最少多少
//		vTaskGetRunTimeStats(RUNTIME);   //RUNTIME必须全局
//		printf("%s\r\n",RUNTIME);
	}

}

void FFT_task(void *pvParameters)
{
	TickType_t PreTime = 0;
	PreTime = xTaskGetTickCount();
	while(1){
		if(FFT_Semaphore!=NULL){
			xSemaphoreTake(FFT_Semaphore,portMAX_DELAY);  //二值信号量阻塞
		}else{
			vTaskDelayUntil(&PreTime,25);//绝对延时模式
		}
		taskENTER_CRITICAL();           //进入临界区
		cr4_fft_256_stm32(lBufOutArray,lBufInArray,NPT);
		if(FFTMag_Semaphore != NULL){
			xSemaphoreGive(FFTMag_Semaphore);
		}
		taskEXIT_CRITICAL();            //退出临界区					
	}
}

//void ADC_task(void *pvParameters)
//{
//	extern __IO uint16_t ADCConvertedValue;
//	
//	while(1){
//		if(ADC_Semaphore!=NULL){
//			xSemaphoreTake(ADC_Semaphore,portMAX_DELAY);  //二值信号量阻塞
//		}else{
//			delay_ms(1000);
//		}
//		taskENTER_CRITICAL();           //进入临界区
//		
//		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
//		while(!DMA_GetFlagStatus(DMA1_FLAG_TC1)){}
//		/* Clear channel1 transfer complete flag */
//		ADC_SoftwareStartConvCmd(ADC1, DISABLE);
//		DMA_ClearFlag(DMA1_FLAG_TC1);
//		lBufInArray[NPT_Cnt] =  ADCConvertedValue<<16;
//			
//		//printf("%lo",lBufInArray[NPT_Cnt]);
//		taskEXIT_CRITICAL();            //退出临界区		
//	}
//}

void FFTMag_task(void *pvParameters)
{
	signed short lX,lY;
	float Mag;
	unsigned short i;
//	char ch1[] = {0xff,0xf0,0x0f,0x00};
	while(1){
		if(FFTMag_Semaphore!=NULL){
			xSemaphoreTake(FFTMag_Semaphore,portMAX_DELAY);
		}else{
			delay_ms(1000);
		}
		taskENTER_CRITICAL();           //进入临界区
		for(i=0; i<NPT/2; i++){
			lX  = (lBufOutArray[i] << 16) >> 16;
			lY  = (lBufOutArray[i] >> 16);
			Mag = sqrt_fast((float)(lX * lX + lY * lY));
			lBufMagArray[i] = (unsigned long)(Mag);
			//printf("%c,%c,%c,%d,%c",ch1[0],ch1[1],ch1[2],(int)lBufMagArray[i],ch1[3]);
			//printf("P:%d, Mag:%d\r\n",i*10000/NPT, (int)lBufMagArray[i]);
		}
		taskEXIT_CRITICAL();            //退出临界区	
		if(LED_Semaphore != NULL){
			xSemaphoreGive(LED_Semaphore);
		}
			
	}
}
