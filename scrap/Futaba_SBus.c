#include "mcc_generated_files/mcc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Futaba_SBus.h"


/*****************************************************************************
 * Private functions
 ****************************************************************************/





/*****************************************************************************
 * Define
 ****************************************************************************/
//#define CHANNEL_NUM (16)    // CH��
//#define CH_BIT_NUM  (12)    // CH�f�[�^�r�b�g��

// s.bus����p
union SBUS_WORK {
    uint8_t  bytes[4];
    uint32_t work;
};



/*****************************************************************************
 * Public functions
 ****************************************************************************/


/*****************************************************************************
 * Public values
 ****************************************************************************/
// �eCH�l�o�b�t�@(���J�p)�FCH1?CH16
volatile uint16_t sbus_ch_buff[SBUS_CH_NUM] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
// �t���O���J�p
volatile uint8_t  sbus_flag_buff = 0xff;

////int tmpflag = 0;
////int tmperrflag = 0;


/*****************************************************************************
 * Private Values
 ****************************************************************************/
// �eCH�l�o�b�t�@(��Ɨp)�FCH1?CH16
#define RECV_BUFF_SIZE (25)
#define SEND_BUFF_SIZE (25)
uint8_t recv_buff[RECV_BUFF_SIZE];
uint8_t send_buff[SEND_BUFF_SIZE];
 
//��M�f�[�^���J�E���g
static int recv_count = 0;
//���M�f�[�^���J�E���g
//static int send_count = 0;

// ��M�f�[�^����Tick�J�E���g
static volatile uint16_t nodata_ms_count;


/*****************************************************************************
 * Functions
 ****************************************************************************/
inline uint16_t SBus_Decode( uint8_t* data_ptr, int right_shift );
inline void SBus_Encode( uint8_t* data_ptr, int left_shift, uint16_t ch_data );


/*
 * �o�b�t�@������
 */
void sbus_buffer_init(void)
{
    memset( recv_buff, 0x00, RECV_BUFF_SIZE  );
    recv_count = 0;
    nodata_ms_count = 0;
}

/*
 UART��M���荞��
 */
void __attribute__ ( ( interrupt, no_auto_psv ) ) _U1RXInterrupt( void )
{

    // * ��M�f�[�^����
    uint8_t data;
    data = U1RXREG;
    
    
    UART1_Write(data);
    
    sbus_ch_data_proc( data );
    nodata_ms_count = 0;    // �f�[�^��M�����̂ŃJ�E���^�N���A
    
    IFS0bits.U1RXIF = false;

}

/*
 �f�o�b�O
 */
void sbus_debug(void)
{
    
    
    
}


/*
 * ��M�f�[�^����
 */
void sbus_ch_data_proc( uint8_t data )
{
    // �擪�f�[�^�`�F�b�N
    if( (recv_count == 0) && (data != 0x0f) )
        return;
    
    recv_buff[recv_count++] = data;
////    rcv_count++;
    // �S�f�[�^��M�������̏���
    if( recv_count >= RECV_BUFF_SIZE )
    {
        // �eCH�f�[�^���o��
        sbus_ch_buff[0]  = SBus_Decode( recv_buff +  1, 0 );
        sbus_ch_buff[1]  = SBus_Decode( recv_buff +  2, 3 );
        sbus_ch_buff[2]  = SBus_Decode( recv_buff +  3, 6 );
        sbus_ch_buff[3]  = SBus_Decode( recv_buff +  5, 1 );
        sbus_ch_buff[4]  = SBus_Decode( recv_buff +  6, 4 );
        sbus_ch_buff[5]  = SBus_Decode( recv_buff +  7, 7 );
        sbus_ch_buff[6]  = SBus_Decode( recv_buff +  9, 2 );
        sbus_ch_buff[7]  = SBus_Decode( recv_buff + 10, 5 );
        sbus_ch_buff[8]  = SBus_Decode( recv_buff + 12, 0 );
        sbus_ch_buff[9]  = SBus_Decode( recv_buff + 13, 3 );
        sbus_ch_buff[10] = SBus_Decode( recv_buff + 14, 6 );
        sbus_ch_buff[11] = SBus_Decode( recv_buff + 16, 1 );
        sbus_ch_buff[12] = SBus_Decode( recv_buff + 17, 4 );
        sbus_ch_buff[13] = SBus_Decode( recv_buff + 18, 7 );
        sbus_ch_buff[14] = SBus_Decode( recv_buff + 20, 2 );
        sbus_ch_buff[15] = SBus_Decode( recv_buff + 21, 5 );

        // �t���O���o��
        sbus_flag_buff = recv_buff[23];
        
        //�o�b�t�@������
        sbus_buffer_init();
////        rcv_count2++;
    }
    
//    tmpflag = 1;
}

/*
 * ��M�o�b�t�@����eCH�f�[�^�����o���B
 *  1) data_ptr����3byte���o��
 *  2) right_shift�������E�V�t�g
 *  3) ����11bit��Ԃ��B
 */
inline uint16_t SBus_Decode( uint8_t* data_ptr, int right_shift )
{
    uint32_t buff;
    buff = data_ptr[2];
    buff = (buff<<8) | data_ptr[1];
    buff = (buff<<8) | data_ptr[0];
    buff >>= right_shift;
    buff &= 0x000007ff;
    return (uint16_t)buff;
}



/*
 * UART���瑗�M����S.Bus�f�[�^���쐬����B

 */
inline void SBus_Encode( uint8_t* data_ptr, int left_shift, uint16_t ch_data )
{
    union SBUS_WORK work;
    union SBUS_WORK mask;
//    uint32_t buff = (uint32_t)ch_data;
    work.work = (uint32_t)ch_data;
    mask.work = 0x000007ff;
    work.work &= mask.work; // �O�̂���Mask
    
    // shift
    work.work <<= left_shift;
    mask.work <<= left_shift;
    
    // �o�b�t�@�Ɋi�[
    data_ptr[0] = ( data_ptr[0] & ~(mask.bytes[0]) ) | work.bytes[0];
    data_ptr[1] = ( data_ptr[1] & ~(mask.bytes[1]) ) | work.bytes[1];
    data_ptr[2] = ( data_ptr[2] & ~(mask.bytes[2]) ) | work.bytes[2];
}



void sbus_send_start(void)
{
#if false
    // ���M�o�b�t�@�̏���
        // �擪�f�[�^�`�F�b�N
    if( (recv_count == 0) && (data != 0x0f) )
        return;
    
    recv_buff[recv_count++] = data;
////    rcv_count++;
    // �S�f�[�^��M�������̏���
    if( recv_count >= RECV_BUFF_SIZE )
#endif
    
}






/*
 1ms tick����
 */
void FutabaSBus_1ms_tick_proc(void)
{
     /*
     �l�����ׂ�����
     * 
     * �@�EUART���r�؂ꂽ�ꍇ�i100ms�ȏ�f�[�^���r�؂ꂽ�ꍇ�j�ASafeMode�ɓ���B
     * �@�EUART���r�؂ꂽ�ꍇ�i1ms�ȏ�f�[�^���r�؂ꂽ�ꍇ�j�A�o�b�t�@�N���A����B�B
     * �@�EUART�������������ɁA�f�[�^�����Ⴆ�Ȃ����ƁB
     * �@�@��1ms�f�[�^���r�؂ꂽ�ꍇ�Ɏ�M�o�b�t�@���N���A����΁A
     * �@�@�@������ 0x0f(Start)����F�������Ƃ��Ă��f�[�^�����肸�m�肵�Ȃ��B
     * 
     * �@�E��M�@����̃t���O�����āASafeMode�ɓ���B
     */

    nodata_ms_count++;  // overflow���Ă����v�Ȃ͂�
    if( (recv_count >0) && (nodata_ms_count>1) )
    {
        // �f�[�^��M����1ms�ȏ�f�[�^���r�؂ꂽ��A��M���Z�b�g
        recv_count = 0;
    }
    
    if( nodata_ms_count>100 )
    {
        // 100ms�f�[�^�����������ꍇ�ASafeMode�ɓ���B
        sbus_flag_buff |= 0xff; // �Ƃ肠�����A0��00�ȊO�Ȃ�SafeMode
        
    }
}



/* uart1.c����ړ��@*/

void __attribute__ ( ( interrupt, no_auto_psv ) ) _U1ErrInterrupt ( void )
{
    if ((U1STAbits.OERR == 1))
    {
        U1STAbits.OERR = 0;
    }

    //�o�b�t�@������
    sbus_buffer_init();
        
    IFS4bits.U1ERIF = false;
    
//    tmperrflag = 1;
}


/*
 �l�����ׂ�����
 * 
 * �@�EUART���r�؂ꂽ�ꍇ�i100ms�ȏ�f�[�^���r�؂ꂽ�ꍇ�j�ASafeMode�ɓ���B
 * �@�EUART���r�؂ꂽ�ꍇ�i1ms�ȏ�f�[�^���r�؂ꂽ�ꍇ�j�A�o�b�t�@�N���A����B�B
 * �@�EUART�������������ɁA�f�[�^�����Ⴆ�Ȃ����ƁB
 * �@�@��1ms�f�[�^���r�؂ꂽ�ꍇ�Ɏ�M�o�b�t�@���N���A����΁A
 * �@�@�@������ 0x0f(Start)����F�������Ƃ��Ă��f�[�^�����肸�m�肵�Ȃ��B
 * 
 * �@�E��M�@����̃t���O�����āASafeMode�ɓ���B
 */


/* end of text */
