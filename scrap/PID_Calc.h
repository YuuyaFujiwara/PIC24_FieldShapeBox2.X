/* 
 * File:   PID_Calc.h
 * Author: 藤原裕也
 *
 * Created on 2017/12/04, 9:45
 */

#ifndef PID_CALC_H
#define	PID_CALC_H

#ifdef	__cplusplus
extern "C" {
#endif

/*****************************************************************************
 * Public values
 ****************************************************************************/
extern double PGain[];
extern double IGain[];
extern double DGain[];
extern long   AnsMax[];
extern long   AnsMin[];   
 //
// PID初期化
//
//
extern void PID_Init(int index );


//'''<summary>
//'''PID出力を求める
//''' </summary>
//'''
//'''<returns>戻り値：出力（前回出力＋操作量</returns> 
//''' <param name="current_val">現在値</param>
//
// タイマー割り込みで呼び出すため?ｔは一定とみなし考慮しない。
extern long PID_Calc(int index, long current_val, long target_val );   
    
    
    



#ifdef	__cplusplus
}
#endif

#endif	/* PID_CALC_H */

