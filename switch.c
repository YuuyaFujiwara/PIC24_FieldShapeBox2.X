
#include "mcc_generated_files/mcc.h"
#include "switch.h"


// SW�������ԊĎ��p
#define SW_LONGPUSH_MS (2000)   // ������ms
#define SW_SHORTPUSH_MS (10)    // �Z����ms ������2000ms

// SW�������Ď��p
uint16_t sw_longpush_count[SWITHC_COUNT];

// �����֐��錾
extern void switch_1ms_proc_sub(int index, int sts );


// SW��������Ԏ��o��
inline bool sw_longpush_sts( int index )
{
    bool rslt = (sw_longpush_count[index] == SW_LONGPUSH_MS);
    if( rslt )
    {
        sw_longpush_count[index] = 0xffff;
    }
    return rslt;
}

// SW�Z������Ԏ��o��
inline bool sw_shortpush_sts( int index )
{    
    return( (sw_longpush_count[index] > SW_SHORTPUSH_MS) && (sw_longpush_count[index] != 0xffff ) );
}

#if false
// SW���������Z�b�g
// ���������o��Ăяo��
// �� reset��counter��0xffff�ɐݒ�B
//�@�@SwOff��counter��0�ɂȂ�A�ēx�������J�E���g�J�n
inline void sw_longpush_reset( int index )
{
    sw_longpush_count[index] = 0xffff;
}
#endif

// 1ms���Ăяo��
void switch_1ms_proc(void)
{
    switch_1ms_proc_sub( SWITCH1, !SW1_GetValue() );
    switch_1ms_proc_sub( SWITCH2, !SW2_GetValue() );
    switch_1ms_proc_sub( SWITCH3, !SW3_GetValue() );
}


// 1ms���Ăяo��(sub)
void switch_1ms_proc_sub(int index, int sts )
{
    if( !sts )
        sw_longpush_count[index] = 0;  // OFF
    else if( sw_longpush_count[index] < SW_LONGPUSH_MS ) 
    {
        sw_longpush_count[index]++;
    }
    
}    

