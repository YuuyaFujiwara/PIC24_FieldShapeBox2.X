
#include "mcc_generated_files/mcc.h"
#include "switch.h"


// SW押し時間監視用
#define SW_LONGPUSH_MS (2000)   // 長押しms
#define SW_SHORTPUSH_MS (10)    // 短押しms 長押し2000ms

// SW長押し監視用
uint16_t sw_longpush_count[SWITHC_COUNT];

// 内部関数宣言
extern void switch_1ms_proc_sub(int index, int sts );


// SW長押し状態取り出し
inline bool sw_longpush_sts( int index )
{
    bool rslt = (sw_longpush_count[index] == SW_LONGPUSH_MS);
    if( rslt )
    {
        sw_longpush_count[index] = 0xffff;
    }
    return rslt;
}

// SW短押し状態取り出し
inline bool sw_shortpush_sts( int index )
{    
    return( (sw_longpush_count[index] > SW_SHORTPUSH_MS) && (sw_longpush_count[index] != 0xffff ) );
}

#if false
// SW長押しリセット
// 長押し検出後呼び出し
// ※ resetでcounterを0xffffに設定。
//　　SwOffでcounterが0になり、再度長押しカウント開始
inline void sw_longpush_reset( int index )
{
    sw_longpush_count[index] = 0xffff;
}
#endif

// 1ms毎呼び出し
void switch_1ms_proc(void)
{
    switch_1ms_proc_sub( SWITCH1, !SW1_GetValue() );
    switch_1ms_proc_sub( SWITCH2, !SW2_GetValue() );
    switch_1ms_proc_sub( SWITCH3, !SW3_GetValue() );
}


// 1ms毎呼び出し(sub)
void switch_1ms_proc_sub(int index, int sts )
{
    if( !sts )
        sw_longpush_count[index] = 0;  // OFF
    else if( sw_longpush_count[index] < SW_LONGPUSH_MS ) 
    {
        sw_longpush_count[index]++;
    }
    
}    

