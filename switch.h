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


    // SW長押し監視用
    #define SWITCH1 (0)
    #define SWITCH2 (1)
    #define SWITCH3 (2)
    #define SWITHC_COUNT (3)

    
    // SW短押し状態取り出し
    extern inline bool sw_shortpush_sts( int index );
    
    // SW長押し状態取り出し
    extern inline bool sw_longpush_sts( int index );

    // SW長押しリセット
    // 長押し検出後呼び出し
  //  extern inline void sw_longpush_reset( int index );
    
    
    // 1ms毎呼び出し
    extern void switch_1ms_proc(void);



#ifdef	__cplusplus
}
#endif

#endif	/* SWITCH_H */

