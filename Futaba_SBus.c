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
//#define CHANNEL_NUM (16)    // CH数
//#define CH_BIT_NUM  (12)    // CHデータビット数

// s.bus操作用
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
// 各CH値バッファ(公開用)：CH1?CH16
volatile uint16_t sbus_ch_buff[SBUS_CH_NUM] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
// フラグ公開用
volatile uint8_t  sbus_flag_buff = 0xff;

////int tmpflag = 0;
////int tmperrflag = 0;


/*****************************************************************************
 * Private Values
 ****************************************************************************/
// 各CH値バッファ(作業用)：CH1?CH16
#define RECV_BUFF_SIZE (25)
#define SEND_BUFF_SIZE (25)
uint8_t recv_buff[RECV_BUFF_SIZE];
uint8_t send_buff[SEND_BUFF_SIZE];
 
//受信データ数カウント
static int recv_count = 0;
//送信データ数カウント
//static int send_count = 0;

// 受信データ無しTickカウント
static volatile uint16_t nodata_ms_count;


/*****************************************************************************
 * Functions
 ****************************************************************************/
inline uint16_t SBus_Decode( uint8_t* data_ptr, int right_shift );
inline void SBus_Encode( uint8_t* data_ptr, int left_shift, uint16_t ch_data );


/*
 * バッファ初期化
 */
void sbus_buffer_init(void)
{
    memset( recv_buff, 0x00, RECV_BUFF_SIZE  );
    recv_count = 0;
    nodata_ms_count = 0;
}

/*
 UART受信割り込み
 */
void __attribute__ ( ( interrupt, no_auto_psv ) ) _U1RXInterrupt( void )
{

    // * 受信データ処理
    uint8_t data;
    data = U1RXREG;
    
    
    UART1_Write(data);
    
    sbus_ch_data_proc( data );
    nodata_ms_count = 0;    // データ受信したのでカウンタクリア
    
    IFS0bits.U1RXIF = false;

}

/*
 デバッグ
 */
void sbus_debug(void)
{
    
    
    
}


/*
 * 受信データ処理
 */
void sbus_ch_data_proc( uint8_t data )
{
    // 先頭データチェック
    if( (recv_count == 0) && (data != 0x0f) )
        return;
    
    recv_buff[recv_count++] = data;
////    rcv_count++;
    // 全データ受信完了時の処理
    if( recv_count >= RECV_BUFF_SIZE )
    {
        // 各CHデータ取り出し
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

        // フラグ取り出し
        sbus_flag_buff = recv_buff[23];
        
        //バッファ初期化
        sbus_buffer_init();
////        rcv_count2++;
    }
    
//    tmpflag = 1;
}

/*
 * 受信バッファから各CHデータを取り出す。
 *  1) data_ptrから3byte取り出し
 *  2) right_shift分だけ右シフト
 *  3) 下位11bitを返す。
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
 * UARTから送信するS.Busデータを作成する。

 */
inline void SBus_Encode( uint8_t* data_ptr, int left_shift, uint16_t ch_data )
{
    union SBUS_WORK work;
    union SBUS_WORK mask;
//    uint32_t buff = (uint32_t)ch_data;
    work.work = (uint32_t)ch_data;
    mask.work = 0x000007ff;
    work.work &= mask.work; // 念のためMask
    
    // shift
    work.work <<= left_shift;
    mask.work <<= left_shift;
    
    // バッファに格納
    data_ptr[0] = ( data_ptr[0] & ~(mask.bytes[0]) ) | work.bytes[0];
    data_ptr[1] = ( data_ptr[1] & ~(mask.bytes[1]) ) | work.bytes[1];
    data_ptr[2] = ( data_ptr[2] & ~(mask.bytes[2]) ) | work.bytes[2];
}



void sbus_send_start(void)
{
#if false
    // 送信バッファの準備
        // 先頭データチェック
    if( (recv_count == 0) && (data != 0x0f) )
        return;
    
    recv_buff[recv_count++] = data;
////    rcv_count++;
    // 全データ受信完了時の処理
    if( recv_count >= RECV_BUFF_SIZE )
#endif
    
}






/*
 1ms tick処理
 */
void FutabaSBus_1ms_tick_proc(void)
{
     /*
     考慮すべきこと
     * 
     * 　・UARTが途切れた場合（100ms以上データが途切れた場合）、SafeModeに入る。
     * 　・UARTが途切れた場合（1ms以上データが途切れた場合）、バッファクリアする。。
     * 　・UARTが復旧した時に、データを取り違えないこと。
     * 　　※1msデータが途切れた場合に受信バッファをクリアすれば、
     * 　　　復旧後 0x0f(Start)を誤認識したとしてもデータが足りず確定しない。
     * 
     * 　・受信機からのフラグを見て、SafeModeに入る。
     */

    nodata_ms_count++;  // overflowしても大丈夫なはず
    if( (recv_count >0) && (nodata_ms_count>1) )
    {
        // データ受信中に1ms以上データが途切れたら、受信リセット
        recv_count = 0;
    }
    
    if( nodata_ms_count>100 )
    {
        // 100msデータが欠落した場合、SafeModeに入る。
        sbus_flag_buff |= 0xff; // とりあえず、0ｘ00以外ならSafeMode
        
    }
}



/* uart1.cから移動　*/

void __attribute__ ( ( interrupt, no_auto_psv ) ) _U1ErrInterrupt ( void )
{
    if ((U1STAbits.OERR == 1))
    {
        U1STAbits.OERR = 0;
    }

    //バッファ初期化
    sbus_buffer_init();
        
    IFS4bits.U1ERIF = false;
    
//    tmperrflag = 1;
}


/*
 考慮すべきこと
 * 
 * 　・UARTが途切れた場合（100ms以上データが途切れた場合）、SafeModeに入る。
 * 　・UARTが途切れた場合（1ms以上データが途切れた場合）、バッファクリアする。。
 * 　・UARTが復旧した時に、データを取り違えないこと。
 * 　　※1msデータが途切れた場合に受信バッファをクリアすれば、
 * 　　　復旧後 0x0f(Start)を誤認識したとしてもデータが足りず確定しない。
 * 
 * 　・受信機からのフラグを見て、SafeModeに入る。
 */


/* end of text */
