
#include "mcc_generated_files/mcc.h"
#include "FieldShape.h"
#include "switch.h"
#include "led.h"


// UART受信バッファ
// バッファサイズ
#define UART_RECV_BUFF_SIZE (200)
// 受信バッファ
uint8_t g_uart1_recv_buff[UART_RECV_BUFF_SIZE];
uint8_t g_uart2_recv_buff[UART_RECV_BUFF_SIZE];


// 初期化フラグ
static bool init_flag = false;



// watchdog timer (for debug))
#define WD_LIMIT_NMEA   (1000)  // ms
#define WD_LIMIT_RTCM   (1000)  //ms
uint16_t wd_counter_nmea = 0xffff;   // 受信していない時間のカウント(NMEA)
uint16_t wd_counter_rtcm = 0xffff;   // 受信していない時間のカウント(RTCM)


// 状態
static FIELDSHAPE_STATUS_t FieldShape_sts = UNKNOWN;
//static FIELDSHAPE_STATUS_t FieldShape_sts = FIELDSHAPE_STATUS_t.UNKNOWN;
//static FIELDSHAPE_STATUS_t FieldShape_sts = 0;

// Declareare internal functions
static void UART_NMEA_PathThrough(void);
static void UART_RTCM_PathThrough(void);
static void Status_Proc(void);
static void ReadyCheck(void);
static void Status_to_Led(void);




// 処理開始
inline void FieldShape_start(void)
{
    init_flag = true;
}

// 圃場形状設定メイン
void FieldShape_Proc(void)
{
    // 圃場形状設定状態管理
    Status_Proc();



    // UARTデータパススルー
    UART_NMEA_PathThrough();
    UART_RTCM_PathThrough();      
    
}



// 1ms毎呼び出し
inline void FieldShape_1ms_Proc(void)
{    
    if( !init_flag ) return;

   
    // watchdog timer
    if( wd_counter_nmea < WD_LIMIT_NMEA )
        wd_counter_nmea++;
    else
        set_led_status( LED2, LED_STS_OFF );
                
    if( wd_counter_rtcm < WD_LIMIT_RTCM )
        wd_counter_rtcm++;
    else
        set_led_status( LED3, LED_STS_OFF );

    // 受信LED消す
    if( wd_counter_nmea > 50 ) //ms
        set_led_status( LED2, LED_STS_OFF );
    if( wd_counter_rtcm > 50 ) //ms
        set_led_status( LED3, LED_STS_OFF );
    
    
}



// NMAE信号パススルー(UART1 -> UART2)
static void UART_NMEA_PathThrough(void)
{
    uint16_t data_num = UART1_ReadBuffer( g_uart1_recv_buff, UART_RECV_BUFF_SIZE );

    // RUNNING状態の場合のみ送信
    if( data_num > 0 )
    {
        // 受信したらLED点灯
        wd_counter_nmea = 0;                    // 
        set_led_status( LED2, LED_STS_ON );     // 
        
        if( FieldShape_sts == RUNNING )
        {
            // check the send buff
            while( UART2_TransmitBufferSizeGet() <= data_num )
               ;

            // send
           UART2_WriteBuffer( g_uart1_recv_buff, data_num);
        }
    }
}


// RTCM信号パススルー(UART2 -> UART1)
static void UART_RTCM_PathThrough(void)
{
    uint16_t data_num = UART2_ReadBuffer( g_uart2_recv_buff, UART_RECV_BUFF_SIZE );
    if( data_num > 0 )
    {
        wd_counter_rtcm = 0;                    // 
        set_led_status( LED3, LED_STS_ON );     // 

        // check the send buff
         while( UART1_TransmitBufferSizeGet() <= data_num )
            ;

         // send
        UART1_WriteBuffer( g_uart1_recv_buff, data_num);
    } 
}



// 圃場形状設定状態管理
static void Status_Proc(void)
{
    //
    ReadyCheck();
    
    switch( FieldShape_sts )
    {
        case READY:
            // ReadyでSW1長押しされた →　開始
            if( sw_longpush_sts(SWITCH1))
                FieldShape_sts = RUNNING;
            break;
        case RUNNING:
            // 実行中にSW1短押しされた ⇒ 終了
            if( sw_shortpush_sts(SWITCH1))
                FieldShape_sts = FINISH;
            // 実行中にSW3短押しされた ⇒ 中断
            if( sw_shortpush_sts(SWITCH3))
                FieldShape_sts = READY;
            break;
        case FINISH:
            // 終了時にSW3短押しされた ⇒ READY
            if( sw_shortpush_sts(SWITCH3))
                FieldShape_sts = READY;
            break;
        default:
            set_led_status( LED1, LED_STS_OFF );
            break;
    }    
   

    // 圃場形状設定状態をLEDに反映する
    Status_to_Led();
}

    

// Ready及びNotReady状態遷移
static void ReadyCheck(void)
{
    // Ready常態か？
    // とりあえず、NMEA信号を受信したらReadyにする。
    // 後で以下を追加する予定。
    //　　・RTCMの受信
    //　　・XBEEのRSSI強度（？）
    //　　・M8PのRTK状態( Fix, Float )（？）
    bool rdy_flg = (wd_counter_nmea < WD_LIMIT_NMEA);
    
    // 状態遷移
    if( rdy_flg )
    {
        switch( FieldShape_sts )
        {
            case NOT_READY:
            case UNKNOWN:
                FieldShape_sts = READY;
            default:
                break;
        }
    }
    else
    {
        FieldShape_sts = NOT_READY;
    }
}







// 圃場形状設定状態をLEDに反映する
static void Status_to_Led(void)
{
    switch( FieldShape_sts )
    {
        case READY:
            set_led_status( LED1, LED_STS_ON );
            break;
        case RUNNING:
            set_led_status( LED1, LED_STS_SLOW_BLINK );
            break;
        case FINISH:
            set_led_status( LED1, LED_STS_FAST_BLINK );
            break;
        default:
          //  set_led_status( LED1, LED_STS_OFF );
            set_led_status( LED1, LED_STS_OFF );
            break;
    }
}



// end of text