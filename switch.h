/* 
 * File:   switch.h
 * Author: YuyaFujiwara
 *
 * Created on 2020/01/09, 9:50
 */

#ifndef SWITCH_H
#define	SWITCH_H

#ifdef	__cplusplus
extern "C" {
#endif


    // SW�������Ď��p
    #define SWITCH1 (0)
    #define SWITCH2 (1)
    #define SWITCH3 (2)
    #define SWITHC_COUNT (3)

    
    // SW�Z������Ԏ��o��
    extern inline bool sw_shortpush_sts( int index );
    
    // SW��������Ԏ��o��
    extern inline bool sw_longpush_sts( int index );

    // SW���������Z�b�g
    // ���������o��Ăяo��
  //  extern inline void sw_longpush_reset( int index );
    
    
    // 1ms���Ăяo��
    extern void switch_1ms_proc(void);



#ifdef	__cplusplus
}
#endif

#endif	/* SWITCH_H */

