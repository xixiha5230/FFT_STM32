#include "ledarray.h"
#include "delay.h"
#include "usart.h"
void LedArray_GPIOConfig()
{
	GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = A|B|C|D|G1|G2|STB;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(PO1, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = R1|R2|CLK|EN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(PO2, &GPIO_InitStructure);
}
extern long IBufLedArray[];
extern long IBufLedArray_Max[];
void LedArray_Scan(u8 Row)
{
	u8 i=0;
	u8 Up_fla = 0;
	u8 Row_tmp=0;
	Row_tmp = Row;
	if(Row>=16){     				//显示上半屏幕
//		Row_tmp =Row; 
		Row_tmp = 31-Row_tmp; //Row_tmp = 15 ~ 0
		Up_fla = 1;
	}else{                  //显示下半屏幕
//		Row_tmp = Row;
		Row_tmp = 15-Row_tmp;  //Row_tmp = 15 ~ 0
		Up_fla = 0;
	}
	
	switch (Row_tmp){
			case 0:
				A_ReSet;
				B_ReSet;
				C_ReSet;
				D_ReSet;
				break;
			case 1:
				A_Set;
				B_ReSet;
				C_ReSet;
				D_ReSet;
				break;
			case 2:
				A_ReSet;
				B_Set;
				C_ReSet;
				D_ReSet;
				break;
			case 3:
				A_Set;
				B_Set;
				C_ReSet;
				D_ReSet;
				break;
			case 4:
				A_ReSet;
				B_ReSet;
				C_Set;
				D_ReSet;
				break;
			case 5:
				A_Set;
				B_ReSet;
				C_Set;
				D_ReSet;
				break;
			case 6:
				A_ReSet;
				B_Set;
				C_Set;
				D_ReSet;
				break;
			case 7:
				A_Set;
				B_Set;
				C_Set;
				D_ReSet;
				break;
			case 8:
				A_ReSet;
				B_ReSet;
				C_ReSet;
				D_Set;
				break;
			case 9:
				A_Set;
				B_ReSet;
				C_ReSet;
				D_Set;
				break;
			case 10:
				A_ReSet;
				B_Set;
				C_ReSet;
				D_Set;
				break;
			case 11:
				A_Set;
				B_Set;
				C_ReSet;
				D_Set;
				break;
			case 12:
				A_ReSet;
				B_ReSet;
				C_Set;
				D_Set;
				break;
			case 13:
				A_Set;
				B_ReSet;
				C_Set;
				D_Set;
				break;
			case 14:
				A_ReSet;
				B_Set;
				C_Set;
				D_Set;
				break;
			case 15:
				A_Set;
				B_Set;
				C_Set;
				D_Set;
				break;
			default:
				A_ReSet;
				B_ReSet;
				C_ReSet;
				D_ReSet;
				break;
	}
	EN_Set;
	if(Up_fla==0){     												//显示在下半部分
		for(i=0;i<64;i++){
			
			if((int)IBufLedArray[i] > Row){
				R1_Set;
				G1_Set;
				R2_ReSet;
				G2_Set;
			}else{
				R1_Set;
				G1_Set;
				G2_Set;
				R2_Set;
			}
			if((IBufLedArray_Max[i] == Row) | (IBufLedArray_Max[i]-1 == Row)){
				R1_Set;
				G1_Set;
				G2_ReSet;
				R2_Set;
			}
			CLK_ReSet;
			CLK_Set;
		}
	}else{
		for(i=0;i<64;i++){
			if((int)IBufLedArray[i] > Row){
				R1_ReSet;
				G1_Set;
				G2_Set;
				R2_Set;
			}else{
				G1_Set;
				R1_Set;
				G2_Set;
				R2_Set;
			}
			if((IBufLedArray_Max[i] == Row) | (IBufLedArray_Max[i]-1 == Row)){
				G1_ReSet;
				R1_Set;
				G2_Set;
				R2_Set;
			}
			CLK_ReSet;
			CLK_Set;
			
		}
	}
	STB_ReSet;
	STB_Set;
	EN_ReSet;
}

void LedArray_Clearn()
{
	u8 i=0,j=0;
	for(i=0;i<16;i++){
		//LedArray_Scan(i);
		for(j=0;j<64;j++){
			R1_Set;
			R2_Set;
			G1_Set;
			G2_Set;
			CLK_ReSet;
			delay_us(1);
			CLK_Set;
			delay_us(1);
			CLK_ReSet;
		}
		STB_ReSet;
		delay_us(1);
		STB_Set;
		delay_us(1);
		STB_ReSet;
	}
}

