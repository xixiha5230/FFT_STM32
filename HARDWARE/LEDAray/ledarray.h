#ifndef __LEDARRAY_H
#define __LEDARRAY_H

#include "stm32f10x.h"                  // Device header

/*引脚以及端口设置*/
#define PO1 GPIOA
#define A GPIO_Pin_1
#define B GPIO_Pin_2
#define C GPIO_Pin_3
#define D GPIO_Pin_4
#define G1 GPIO_Pin_5
#define G2 GPIO_Pin_6
#define STB GPIO_Pin_7

#define PO2 GPIOB
#define CLK GPIO_Pin_0
#define EN GPIO_Pin_1
#define R1 GPIO_Pin_10
#define R2 GPIO_Pin_11




/*..................*/

/*电平定义*/
#define A_Set GPIO_SetBits(PO1,A)
#define B_Set GPIO_SetBits(PO1,B)
#define C_Set GPIO_SetBits(PO1,C)
#define D_Set GPIO_SetBits(PO1,D)
#define A_ReSet GPIO_ResetBits(PO1,A)
#define B_ReSet GPIO_ResetBits(PO1,B)
#define C_ReSet GPIO_ResetBits(PO1,C)
#define D_ReSet GPIO_ResetBits(PO1,D)

#define R1_Set GPIO_SetBits(PO2,R1)
#define R2_Set GPIO_SetBits(PO2,R2)
#define G1_Set GPIO_SetBits(PO1,G1)
#define G2_Set GPIO_SetBits(PO1,G2)
#define EN_Set GPIO_SetBits(PO2,EN)
#define STB_Set GPIO_SetBits(PO1,STB)
#define CLK_Set GPIO_SetBits(PO2,CLK)

#define R1_ReSet GPIO_ResetBits(PO2,R1)
#define R2_ReSet GPIO_ResetBits(PO2,R2)
#define G1_ReSet GPIO_ResetBits(PO1,G1)
#define G2_ReSet GPIO_ResetBits(PO1,G2)
#define EN_ReSet GPIO_ResetBits(PO2,EN)
#define STB_ReSet GPIO_ResetBits(PO1,STB)
#define CLK_ReSet GPIO_ResetBits(PO2,CLK)
/*......................................*/
/*行选择定义*/




void LedArray_GPIOConfig(void);
void ScanRow(u8 Row);
void LedArray_Clearn(void);
void LedArray_Scan(u8 Row);


#endif
