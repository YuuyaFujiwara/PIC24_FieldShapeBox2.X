/* 
 * File:   PID_Calc.h
 * Author: �����T��
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
// PID������
//
//
extern void PID_Init(int index );


//'''<summary>
//'''PID�o�͂����߂�
//''' </summary>
//'''
//'''<returns>�߂�l�F�o�́i�O��o�́{�����</returns> 
//''' <param name="current_val">���ݒl</param>
//
// �^�C�}�[���荞�݂ŌĂяo������?���͈��Ƃ݂Ȃ��l�����Ȃ��B
extern long PID_Calc(int index, long current_val, long target_val );   
    
    
    



#ifdef	__cplusplus
}
#endif

#endif	/* PID_CALC_H */

