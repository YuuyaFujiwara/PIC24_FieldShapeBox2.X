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

    // LED��ԊǗ�
    #define LED1 (0)
    #define LED2 (1)
    #define LED3 (2)
    #define LED_NUM (3)
    //
    #define LED_STS_OFF (0)
    #define LED_STS_ON  (1)
    #define LED_STS_SLOW_BLINK (2)
    #define LED_STS_FAST_BLINK (3)
    
    // LED��Ԃ�ݒ肷��
    extern inline void set_led_status( int index, uint16_t sts );

    // 1ms���Ăяo��
    extern void led_1ms_proc(void);

    // LED�̏�Ԃ�1�����߂�idebug�p)
    extern void led_status_incl(int index);

#ifdef	__cplusplus
}
#endif

#endif	/* LED_H */

