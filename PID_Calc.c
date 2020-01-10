/*
 * PID�������W���[��
 * 
 * 
 * 
 * 
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "mcc_generated_files/mcc.h"
#include "PID_Calc.h"

/*****************************************************************************
 * definition
 ****************************************************************************/
#define PID_ITEMS_NUM (2)   // PID����Ώې�


/*****************************************************************************
 * Private functions
 ****************************************************************************/




/*****************************************************************************
 * Public functions
 ****************************************************************************/


/*****************************************************************************
 * Public values
 ****************************************************************************/
double PGain[PID_ITEMS_NUM];
double IGain[PID_ITEMS_NUM];
double DGain[PID_ITEMS_NUM];
long   AnsMax[PID_ITEMS_NUM];
long   AnsMin[PID_ITEMS_NUM];

/*****************************************************************************
 * Private Values
 ****************************************************************************/
//static bool flg_firsttime[PID_ITEMS_NUM] = { false, false };
static long IntegralBuff[PID_ITEMS_NUM] = { 0, 0 };
static long old_P_Buff[PID_ITEMS_NUM] = { 0, 0 };
static long OutputVal[PID_ITEMS_NUM];

//
// PID������
//
//
void PID_Init(int index )
{
//    flg_firsttime[index] = { false, false };
    IntegralBuff[index] = 0;
    old_P_Buff  [index] = 0;
    OutputVal   [index] = 0;

}

//'''<summary>
//'''PID�o�͂����߂�
//''' </summary>
//'''
//'''<returns>�߂�l�F�o�́i�O��o�́{�����</returns> 
//''' <param name="current_val">���ݒl</param>
//
// �^�C�}�[���荞�݂ŌĂяo������?���͈��Ƃ݂Ȃ��l�����Ȃ��B
long PID_Calc(int index, long current_val, long target_val )
{

    // P
    long p =  target_val - current_val;
    // I
    long integ = 0.9 * IntegralBuff[index] + p; //  �ϕ��l�������ɑ������Ȃ��悤����
//    if( IntegralBuff[index] > AnsMax[index] ) IntegralBuff[index] = AnsMax[index];
//    if( IntegralBuff[index] < AnsMin[index] ) IntegralBuff[index] = AnsMin[index];
    IntegralBuff[index] = integ;
    // D
    long d = p  - old_P_Buff[index];
    old_P_Buff[index] = p;
    
    // ����ʌv�Z
    double ctrl_val = PGain[index] * p + IGain[index] * integ + DGain[index] * d;

    // �o�͒l�ɑ���ʂ𔽉f
    OutputVal[index] += (long)ctrl_val;
    if( OutputVal[index] > AnsMax[index] ) OutputVal[index] = AnsMax[index];
    if( OutputVal[index] < AnsMin[index] ) OutputVal[index] = AnsMin[index];
    
#if false
    printf("P=%ld,I=%ld,D=%ld,PID=%f,out=%ld\n", p, integ, d, ctrl_val, OutputVal[index] );
#else
    char prtbuf[64];
    sprintf( prtbuf, "P=%ld,I=%ld,D=%ld,PID=%f,out=%ld\n", p, integ, d, ctrl_val, OutputVal[index] );
    UART2_WriteBuffer( (uint8_t*)prtbuf, (unsigned int)strlen(prtbuf) );
#endif
    
    return OutputVal[index];
}    
