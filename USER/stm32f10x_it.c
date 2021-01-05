/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h" 
#include "stdio.h"
#include "delay.h"
#include "usart.h"	  
#include <string.h>
#include <stdlib.h>

void NMI_Handler(void)
{
}
 
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}
 
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

 
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}
 
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}
 
//void SVC_Handler(void)
//{
//}
 
void DebugMon_Handler(void)
{
}
 
//void PendSV_Handler(void)
//{
//}
 
//void SysTick_Handler(void)
//{
//}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/*************************************************
Function: USART1_IRQHandler
Description: ����1�жϺ���
Calls: 
Called By: none
Input: none
Output: none
Return: none
Others: 
*************************************************/
//extern char receivebuf[];
//extern char prerecb[]; 
//void USART1_IRQHandler(void)
//{
//	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET){
//		USART_ReceiveData(USART1);
//		DMA_Cmd(DMA1_Channel5, DISABLE);
//		strcpy(receivebuf,prerecb);
//		memset(prerecb,0,strlen(prerecb));
//		DMA_ClearFlag(DMA1_FLAG_TC5);
//		DMA1_Channel5->CNDTR = (u32)&USART1->DR;
//		DMA_Cmd(DMA1_Channel5, ENABLE);
//	}
//}

/*************************************************
Function: USART2_IRQHandler
Description: ����2�жϺ���
Calls: 
Called By: none
Input: none
Output: none
Return: none
Others: 
*************************************************/
void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET){
		USART_ReceiveData(USART2);
	}
}

/*************************************************
Function: USART2_IRQHandler
Description: ����2�жϺ���
Calls: 
Called By: none
Input: none
Output: none
Return: none
Others: 
*************************************************/
void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET){
		USART_ReceiveData(USART3);
	}
}

/*************************************************
Function: TIM1_UP_IRQHandler
Description: ��ʱ��1�жϴ����� ����ι��
Calls: 
Called By: none
Input: none
Output: none
Return: none
Others: 
*************************************************/
void TIM1_UP_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET){
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);  			//���TIMx�����жϱ�־ 
		IWDG_ReloadCounter();															//ι��
	}	
}

/*************************************************
Function: TIM2_IRQHandler
Description: ��ʱ��2�жϴ����� ����������
Calls:
Called By: none
Input: none
Output: none
Return: none
Others:
*************************************************/
//void TIM2_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){
//	}
//}

/*************************************************
Function: TIM3_IRQHandler
Description: ��ʱ��3�жϴ����� ���ڶ�ʱ���
Calls:
Called By: none
Input: none
Output: none
Return: none
Others:
*************************************************/
//void TIM3_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){
//	}
//}

/*************************************************
Function: TIM4_IRQHandler
Description: ��ʱ��4�жϴ����� �������Ͷ�ʱ
Calls:
Called By: none
Input: none
Output: none
Return: none
Others:
*************************************************/
//void TIM4_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET){
//	}
//}

/*************************************************
Function: TIM5_IRQHandler
Description: ��ʱ��5�жϴ����� ����SD��ⶨʱ
Calls:
Called By: none
Input: none
Output: none
Return: none
Others:
*************************************************/
void TIM5_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET){
	}
}

/*************************************************
Function: EXTI2_IRQHandler
Description: �ⲿ�ж�2������ �������屨��
Calls:
Called By: none
Input: none
Output: none
Return: none
Others:
*************************************************/
void EXTI2_IRQHandler()	 
{
	if(EXTI_GetITStatus(EXTI_Line2)==SET){
		EXTI_ClearITPendingBit(EXTI_Line2);
		
	}
}

/*************************************************
Function: EXTI3_IRQHandler
Description: �ⲿ�ж�3������ ���ڵ�·�屨��
Calls:
Called By: none
Input: none
Output: none
Return: none
Others:
*************************************************/
void EXTI3_IRQHandler()	 
{
	if(EXTI_GetITStatus(EXTI_Line3)==SET){
		EXTI_ClearITPendingBit(EXTI_Line3);

	}
}

/*************************************************
Function: EXTI4_IRQHandler
Description: �ⲿ�ж�4������ ������ʾ������
Calls:
Called By: none
Input: none
Output: none
Return: none
Others:
*************************************************/
void EXTI4_IRQHandler()	 
{
	if(EXTI_GetITStatus(EXTI_Line4)==SET){
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
}

/*************************************************
Function: EXTI9_5_IRQHandler
Description: �ⲿ�ж�9_5������ ������Ǳ���
Calls:
Called By: none
Input: none
Output: none
Return: none
Others:
*************************************************/
void EXTI9_5_IRQHandler()	 
{
	if(EXTI_GetITStatus(EXTI_Line5)==SET){
		EXTI_ClearITPendingBit(EXTI_Line5);
	}
}
