#include "mcc_generated_files/mcc.h"
#include "led.h"


// 点滅管理
bool led_fast_blink_flag = 0;
bool led_slow_blink_flag = 0;
static uint16_t led_fast_blink_count = 0;
static uint16_t led_slow_blink_count = 0;


//
static  uint16_t led_status[LED_NUM] = { LED_STS_OFF, LED_STS_OFF, LED_STS_OFF };


// internal function
static bool led_onoff_sts( int index );


// LED状態を設定する
inline void set_led_status( int index, uint16_t sts )
{
    led_status[index] = sts;
}


// 1ms毎呼び出し
void led_1ms_proc(void)
{
    // fast blink 100ms
    if( led_fast_blink_count < 100 )
        led_fast_blink_count++;
    else
    {
        led_fast_blink_count = 0;
        led_fast_blink_flag = !led_fast_blink_flag;
    }

    // slow blink 500ms
    if( led_slow_blink_count < 500 )
        led_slow_blink_count++;
    else
    {
        led_slow_blink_count = 0;
        led_slow_blink_flag = !led_slow_blink_flag;
    }
    
    // LED1 output
    if( led_onoff_sts(LED1) )
        LED1_SetHigh();
    else
        LED1_SetLow();

    // LED2 output
    if( led_onoff_sts(LED2) )
        LED2_SetHigh();
    else
        LED2_SetLow();

    // LED3 output
    if( led_onoff_sts(LED3) )
        LED3_SetHigh();
    else
        LED3_SetLow();
}


// LEDのON/OFFを決める
static bool led_onoff_sts( int index )
{
    switch( led_status[index] )
    {
        case LED_STS_OFF:
            return false;
        case LED_STS_ON:
            return true;
        case LED_STS_SLOW_BLINK:
            return led_slow_blink_flag;
        case LED_STS_FAST_BLINK:
            return led_fast_blink_flag;
        default:
            return false;            
    }
}

// LEDの状態を1つすすめる（debug用)
void led_status_incl(int index)
{
    if( ++led_status[index] > LED_STS_FAST_BLINK  )
        led_status[index] = 0;
}