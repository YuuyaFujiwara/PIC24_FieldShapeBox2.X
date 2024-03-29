/* 
 * File:   led.h
 * Author: YuyaFujiwara
 *
 * Created on 2020/01/09, 10:17
 */

#ifndef LED_H
#define	LED_H

#ifdef	__cplusplus
extern "C" {
#endif

    // LED状態管理
    #define LED1 (0)
    #define LED2 (1)
    #define LED3 (2)
    #define LED_NUM (3)
    //
    #define LED_STS_OFF (0)
    #define LED_STS_ON  (1)
    #define LED_STS_SLOW_BLINK (2)
    #define LED_STS_FAST_BLINK (3)
    
    // LED状態を設定する
    extern inline void set_led_status( int index, uint16_t sts );

    // 1ms毎呼び出し
    extern void led_1ms_proc(void);

    // LEDの状態を1つすすめる（debug用)
    extern void led_status_incl(int index);

#ifdef	__cplusplus
}
#endif

#endif	/* LED_H */

