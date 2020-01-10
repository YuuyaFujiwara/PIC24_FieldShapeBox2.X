
#include "mcc_generated_files/mcc.h"
#include "FieldShape.h"
#include "switch.h"
#include "led.h"


// UART��M�o�b�t�@
// �o�b�t�@�T�C�Y
#define UART_RECV_BUFF_SIZE (200)
// ��M�o�b�t�@
uint8_t g_uart1_recv_buff[UART_RECV_BUFF_SIZE];
uint8_t g_uart2_recv_buff[UART_RECV_BUFF_SIZE];


// �������t���O
static bool init_flag = false;



// watchdog timer (for debug))
#define WD_LIMIT_NMEA   (1000)  // ms
#define WD_LIMIT_RTCM   (1000)  //ms
uint16_t wd_counter_nmea = 0xffff;   // ��M���Ă��Ȃ����Ԃ̃J�E���g(NMEA)
uint16_t wd_counter_rtcm = 0xffff;   // ��M���Ă��Ȃ����Ԃ̃J�E���g(RTCM)


// ���
static FIELDSHAPE_STATUS_t FieldShape_sts = UNKNOWN;
//static FIELDSHAPE_STATUS_t FieldShape_sts = FIELDSHAPE_STATUS_t.UNKNOWN;
//static FIELDSHAPE_STATUS_t FieldShape_sts = 0;

// Declareare internal functions
static void UART_NMEA_PathThrough(void);
static void UART_RTCM_PathThrough(void);
static void Status_Proc(void);
static void ReadyCheck(void);
static void Status_to_Led(void);




// �����J�n
inline void FieldShape_start(void)
{
    init_flag = true;
}

// �ޏ�`��ݒ胁�C��
void FieldShape_Proc(void)
{
    // �ޏ�`��ݒ��ԊǗ�
    Status_Proc();



    // UART�f�[�^�p�X�X���[
    UART_NMEA_PathThrough();
    UART_RTCM_PathThrough();      
    
}



// 1ms���Ăяo��
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

    // ��MLED����
    if( wd_counter_nmea > 50 ) //ms
        set_led_status( LED2, LED_STS_OFF );
    if( wd_counter_rtcm > 50 ) //ms
        set_led_status( LED3, LED_STS_OFF );
    
    
}



// NMAE�M���p�X�X���[(UART1 -> UART2)
static void UART_NMEA_PathThrough(void)
{
    uint16_t data_num = UART1_ReadBuffer( g_uart1_recv_buff, UART_RECV_BUFF_SIZE );

    // RUNNING��Ԃ̏ꍇ�̂ݑ��M
    if( data_num > 0 )
    {
        // ��M������LED�_��
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


// RTCM�M���p�X�X���[(UART2 -> UART1)
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



// �ޏ�`��ݒ��ԊǗ�
static void Status_Proc(void)
{
    //
    ReadyCheck();
    
    switch( FieldShape_sts )
    {
        case READY:
            // Ready��SW1���������ꂽ ���@�J�n
            if( sw_longpush_sts(SWITCH1))
                FieldShape_sts = RUNNING;
            break;
        case RUNNING:
            // ���s����SW1�Z�������ꂽ �� �I��
            if( sw_shortpush_sts(SWITCH1))
                FieldShape_sts = FINISH;
            // ���s����SW3�Z�������ꂽ �� ���f
            if( sw_shortpush_sts(SWITCH3))
                FieldShape_sts = READY;
            break;
        case FINISH:
            // �I������SW3�Z�������ꂽ �� READY
            if( sw_shortpush_sts(SWITCH3))
                FieldShape_sts = READY;
            break;
        default:
            set_led_status( LED1, LED_STS_OFF );
            break;
    }    
   

    // �ޏ�`��ݒ��Ԃ�LED�ɔ��f����
    Status_to_Led();
}

    

// Ready�y��NotReady��ԑJ��
static void ReadyCheck(void)
{
    // Ready��Ԃ��H
    // �Ƃ肠�����ANMEA�M������M������Ready�ɂ���B
    // ��ňȉ���ǉ�����\��B
    //�@�@�ERTCM�̎�M
    //�@�@�EXBEE��RSSI���x�i�H�j
    //�@�@�EM8P��RTK���( Fix, Float )�i�H�j
    bool rdy_flg = (wd_counter_nmea < WD_LIMIT_NMEA);
    
    // ��ԑJ��
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







// �ޏ�`��ݒ��Ԃ�LED�ɔ��f����
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