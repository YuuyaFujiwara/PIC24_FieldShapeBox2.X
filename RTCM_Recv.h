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

    // ��M�o�b�t�@
extern uint8_t g_rtcm_recv_buff[];

// �p�P�b�g��M�����t���O
extern bool g_rtcm_packrecv_flag;

//
// UART1����RTCM�f�[�^����M����
//
// return : ��M�f�[�^��
extern unsigned int Recv_RTCM_Data( void );

// RTCM��M�p�P�b�g�Ď��p�^�C�}�N���A
// 1������M���Ăяo��
extern inline void RTCM_PackRecv_TimerClear();



#ifdef	__cplusplus
}
#endif

#endif	/* RTCM_RECV_H */

