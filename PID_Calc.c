/*
 * PID処理モジュール
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
#define PID_ITEMS_NUM (2)   // PID制御対象数


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
// PID初期化
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
//'''PID出力を求める
//''' </summary>
//'''
//'''<returns>戻り値：出力（前回出力＋操作量</returns> 
//''' <param name="current_val">現在値</param>
//
// タイマー割り込みで呼び出すため?ｔは一定とみなし考慮しない。
long PID_Calc(int index, long current_val, long target_val )
{

    // P
    long p =  target_val - current_val;
    // I
    long integ = 0.9 * IntegralBuff[index] + p; //  積分値が無限に増減しないよう制限
//    if( IntegralBuff[index] > AnsMax[index] ) IntegralBuff[index] = AnsMax[index];
//    if( IntegralBuff[index] < AnsMin[index] ) IntegralBuff[index] = AnsMin[index];
    IntegralBuff[index] = integ;
    // D
    long d = p  - old_P_Buff[index];
    old_P_Buff[index] = p;
    
    // 操作量計算
    double ctrl_val = PGain[index] * p + IGain[index] * integ + DGain[index] * d;

    // 出力値に操作量を反映
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
