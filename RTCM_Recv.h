/* 
 * File:   RTCM_Recv.h
 * Author: YuyaFujiwara
 *
 * Created on 2019/08/19, 16:49
 */

#ifndef RTCM_RECV_H
#define	RTCM_RECV_H

#ifdef	__cplusplus
extern "C" {
#endif

    // 受信バッファ
extern uint8_t g_rtcm_recv_buff[];

// パケット受信完了フラグ
extern bool g_rtcm_packrecv_flag;

//
// UART1からRTCMデータを受信する
//
// return : 受信データ数
extern unsigned int Recv_RTCM_Data( void );

// RTCM受信パケット監視用タイマクリア
// 1文字受信時呼び出し
extern inline void RTCM_PackRecv_TimerClear();



#ifdef	__cplusplus
}
#endif

#endif	/* RTCM_RECV_H */

