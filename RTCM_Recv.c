#include "mcc_generated_files/mcc.h"
#include "RTCM_Recv.h"

// バッファサイズ
//#define RTCM_RECV_BUFF_SIZE (200)
#define RTCM_RECV_BUFF_SIZE (1)         // このルーチン不要なのでバッファ削減
// 受信バッファ
uint8_t g_rtcm_recv_buff[RTCM_RECV_BUFF_SIZE];

// パケット受信完了フラグ
bool g_rtcm_packrecv_flag = false;



//
// UART1からRTCMデータを受信する
//
// return : 受信データ数
unsigned int Recv_RTCM_Data( void )
{
    unsigned int rcv_num = UART1_RecvDataNum();
    
    if( rcv_num == 0)
        return 0;
    
    if( !g_rtcm_packrecv_flag && ( rcv_num < RTCM_RECV_BUFF_SIZE ) )
        return 0;

    rcv_num = UART1_ReadBuffer( g_rtcm_recv_buff, RTCM_RECV_BUFF_SIZE );
    
    return rcv_num;
}



// 追加
// timer割り込みより呼び出される
// RTCM受信パケット監視用（ 
void TMR2_CallBack(void)
{
    // RTCMデータ受診時に、TMR2リセットしてフラグ落とす。
    // 一定時間受信しない場合、このルーチンが呼ばれてパケット受信フラグを立てる。
    //
    g_rtcm_packrecv_flag = true;
}


// RTCM受信パケット監視用タイマクリア
// 1文字受信時呼び出し
inline void RTCM_PackRecv_TimerClear()
{
    g_rtcm_packrecv_flag = false;
    TMR2 = 0x0000;
}