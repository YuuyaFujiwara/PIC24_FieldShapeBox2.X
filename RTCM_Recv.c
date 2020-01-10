#include "mcc_generated_files/mcc.h"
#include "RTCM_Recv.h"

// �o�b�t�@�T�C�Y
//#define RTCM_RECV_BUFF_SIZE (200)
#define RTCM_RECV_BUFF_SIZE (1)         // ���̃��[�`���s�v�Ȃ̂Ńo�b�t�@�팸
// ��M�o�b�t�@
uint8_t g_rtcm_recv_buff[RTCM_RECV_BUFF_SIZE];

// �p�P�b�g��M�����t���O
bool g_rtcm_packrecv_flag = false;



//
// UART1����RTCM�f�[�^����M����
//
// return : ��M�f�[�^��
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



// �ǉ�
// timer���荞�݂��Ăяo�����
// RTCM��M�p�P�b�g�Ď��p�i 
void TMR2_CallBack(void)
{
    // RTCM�f�[�^��f���ɁATMR2���Z�b�g���ăt���O���Ƃ��B
    // ��莞�Ԏ�M���Ȃ��ꍇ�A���̃��[�`�����Ă΂�ăp�P�b�g��M�t���O�𗧂Ă�B
    //
    g_rtcm_packrecv_flag = true;
}


// RTCM��M�p�P�b�g�Ď��p�^�C�}�N���A
// 1������M���Ăяo��
inline void RTCM_PackRecv_TimerClear()
{
    g_rtcm_packrecv_flag = false;
    TMR2 = 0x0000;
}