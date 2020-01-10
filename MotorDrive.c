#include "mcc_generated_files/mcc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#include "Futaba_SBus.h"
#include "PID_Calc.h"


/*****************************************************************************
 * Private functions
 ****************************************************************************/
void MotorControl_Proc(void);

uint16_t SBus_to_Target_Value( uint16_t sbus, uint16_t onoff, uint16_t target_max );    // 引数 sbus値に応じ、最小（０）?最大（target_max）の値を返す。
uint16_t Calc_MotorControl_Pwm_PID( int mtr_index, uint16_t spd, uint16_t onoff);    // プロポ受信機パルス幅からモータードライバ駆動PWM値を得る。

// 診断処理
bool MotorDrive_Diag_Proc1(int mtr_index, uint16_t tgt_spd, uint16_t mtr_rpm );
bool MotorDrive_Diag_Proc2(int mtr_index, uint16_t tgt_spd, uint16_t mtr_rpm );

void Set_Safe_Mode(void);       // サーボを安全状態にする。

uint16_t dbg_motor_spd( uint16_t onoff1, uint16_t onoff2 );


/*****************************************************************************
 * Public functions
 ****************************************************************************/


/*****************************************************************************
 * Public values
 ****************************************************************************/
volatile uint16_t auto_cmnd_pw_buff[] = { 0, 0, 0, 0, 0 }; // 上位からコマンドで送られたパルス幅値
 
int servo_control_mode = 1;         // サーボ制御モード(プロポ CH5によりコントロールする)
                                    //   0 : CH5=奥　（パルス幅小） 
                                    //   1 : CH5=中　（パルス幅中） 
                                    //   2 : CH5=手前（パルス幅大） 

// モーター速度カウント値
#define MTRSPD_TGT_NUM (2)
uint16_t mtrspd_count[MTRSPD_TGT_NUM] = { 0xffff, 0xffff };



/*****************************************************************************
 * Private Values
 ****************************************************************************/
    #define PW_MEAS_CH_NUM (5)
#if false
    // パルス幅計測 バッファ
    static uint16_t rise_buff[PW_MEAS_CH_NUM] = { 0, 0, 0, 0, 0 };
           uint16_t pluse_width[PW_MEAS_CH_NUM] = { 0, 0, 0, 0, 0 };
;
#endif
//static bool     mtrspd_ovflow[MTRSPD_TGT_NUM] = { false, false };
           
int tmptgtrpm;// for debug
int tmprpm;//for debug           
// watch dog用カウンタ
static volatile uint16_t wd_count[PW_MEAS_CH_NUM] = { 0, 0, 0, 0, 0 };

// PID処理用




#if false
/*
 モーター速度の平均値を算出する
 * index : モーターIndex
 * current : 現在速度
 * return : 平均値
 * 
 */
uint16_t MotorSpeed_Average( int index, uint16_t current )
{
    static uint16_t avgbuff[MTRSPD_TGT_NUM][16];
    uint32_t calcbuff;
    int i;
    
    calcbuff = current;
    for( i = 1; i < 16; i++ )
    {
        calcbuff += avgbuff[index][i];
        avgbuff[index][i-1] = avgbuff[index][i];
    }
    
    calcbuff = calcbuff >> 4;
    
    return calcbuff;
    
    
}
#endif

/*
 * 初期化
 */
void MotorDrive_Init(void)
{
    // 送りモーター
    PID_Init(0);
    PGain[0] = 0.010;
    IGain[0] = 0.005;
    DGain[0] = 0.010;
    AnsMax[0] = 120;    // モータードライバPWM MAX
    AnsMin[0] =     0;  // モータードライバPWM Min   
    
    // 拡散モーター
    PID_Init(1);
    PGain[1] = 0.070;
    IGain[1] = 0.010;
    DGain[1] = 0.020;
    AnsMax[1] = 120;    // モータードライバPWM MAX
    AnsMin[1] =   0;    // モータードライバPWM Min   
}


/*
 100ms tick処理
 */
void MotorDrive_100ms_tick_proc(void)
{
    
    
    // for debug
    // UART送信
#if false
    int tmppropo = sbus_ch_buff[8];
    int tgtrpm   = tmptgtrpm;
   // int rpm  = mtrspd_count[0];
    int tmppwm  = OC3R;
    printf( "tgt=%d,rpm=%d,pwm=%d,",  tgtrpm, tmprpm, tmppwm  );
#endif

    // プロポS.BUSよりPID制御にてモーター駆動PWM決める
    MotorControl_Proc();
    
    
    // Diag処理
    
}



/*
 * プロポ信号を受信しサーボを駆動するルーチン
   1ms毎に呼び出し
 */
void MotorControl_Proc(void)
{
    // プロポ信号によりモータードライバ駆動
    if( sbus_flag_buff == 0x00 )
    {
        // SW-D(CH6)によりPID制御のON/OFFを切り替える
        if( sbus_ch_buff[SBUS_CH6] > 1000 )
        {
            // PID無し
#if true
            // プロポセンター時に5000rpmになるよう、MAX値を設定（実機実測より）
            OC3R =  SBus_to_Target_Value( sbus_ch_buff[SBUS_CH9], sbus_ch_buff[SBUS_CH8], 36 /*籾送り18時に5000rpm*/ );    // CH9, CH8
            OC4R =  SBus_to_Target_Value( sbus_ch_buff[SBUS_CH10], sbus_ch_buff[SBUS_CH7], 54 /*籾拡散27時に5000rpm*/ );    // CH10, CH7
#else
            OC3R =  SBus_to_Target_Value( sbus_ch_buff[SBUS_CH9], sbus_ch_buff[SBUS_CH8], OC3RS/4 );    // CH9, CH8
            OC4R =  SBus_to_Target_Value( sbus_ch_buff[SBUS_CH10], sbus_ch_buff[SBUS_CH7], OC4RS/4 );    // CH10, CH7
#endif

            dbg_motor_spd( sbus_ch_buff[SBUS_CH8],sbus_ch_buff[SBUS_CH7] );
        }
        else
        {
            OC3R =  Calc_MotorControl_Pwm_PID( 0, sbus_ch_buff[SBUS_CH9], sbus_ch_buff[SBUS_CH8] );   // CH9, CH8
            OC4R =  Calc_MotorControl_Pwm_PID( 1, sbus_ch_buff[SBUS_CH10], sbus_ch_buff[SBUS_CH7] );   // CH9, CH8

        }
    }
    else
    {
        // safe mode
        OC3R =  0x00;
        OC4R =  0x00;
    }
    
    return;
}


/*
 * 引数 sbus値に応じ、最小（０）?最大（target_max）の値を返す。
 * 
 * sbus : S.Bus値。　最小時1904　最大時144
 * onoff : On/Off SWの状態を表すS.BUS値　ON=144　OFF＝1904
 * target_max : S.Bus値が最大（144）の場合に返す値
 * 
 * return : 演算後の値。０ ? target_max
 */
uint16_t SBus_to_Target_Value( uint16_t sbus, uint16_t onoff, uint16_t target_max )
{
    // sbus, onoffがゼロの場合、S.BUS受信していないので停止
    if( sbus==0 || onoff==0 )
        return 0;   
    
    // S.Bus範囲チェック
    if( sbus < 144 ) sbus = 144;
    if( sbus > 1904 ) sbus = 1904;
    
    double tmp = 0;
    if( onoff < 1000 )    // on/off swが1000未満ならON
    {
        tmp = (double)((int)1904 - sbus);
        tmp *= target_max;    // 最大値
        tmp /= (1904 - 144);
    }

    return (uint16_t)tmp;
}
///
/// for debug
uint16_t dbg_motor_spd( uint16_t onoff1, uint16_t onoff2 )
{
    if( onoff2==0 || onoff1==0 )
        return 0;   

    if( onoff1 < 1000 || onoff2<1000 )    // on/off swが1000未満ならON
    {
        uint16_t motor_rpm1 = 10.0e6 / mtrspd_count[0];
        uint16_t motor_rpm2 = 10.0e6 / mtrspd_count[1];
        
            char  prtbuf[64];
        sprintf( prtbuf,  "pwm1=%u, rpm1=%u, pwm2=%u, rpm2=%u\n",  OC3R, motor_rpm1, OC4R, motor_rpm2  );    
        UART2_WriteBuffer( (uint8_t*)prtbuf, strlen(prtbuf) );
    }
    
    return 0;
}


/*
 * 拡散モーター用PWM生成
 * プロポ受信機 S.BUS値からモータードライバ駆動PWM値を得る。
 * 拡散モーター速度制御：CH9(DT5)　1904(遅い)?144(速い)
 * 拡散モーターON/OFF  ：SW-B(CH8)　ON=144　OFF＝1904
 * 
 */
uint16_t Calc_MotorControl_Pwm_PID(int mtr_index, uint16_t spd, uint16_t onoff)
{ 
    // ターゲットRPM
    uint16_t tgt_motor_rpm = SBus_to_Target_Value( spd, onoff, 10000 /*←最大rpm*/ );
//    printf("spd=%d,onoff=%d,rpm=%d\n",spd,onoff,tgt_motor_rpm );
//   tmptgtrpm = tgt_motor_rpm;// for debug
    if( tgt_motor_rpm == 0 ) 
    {
        PID_Init(mtr_index);
        return 0;
    }
    
    
    // 現在RPM
    // 10,000,000 / カウント = RPM
    uint16_t tmpcount = mtrspd_count[mtr_index];
    uint16_t motor_rpm = 0;
    if( tmpcount < 0xffff )
    {
        motor_rpm = 10.0e6 / tmpcount;
    }
//    tmprpm = motor_rpm;// for debug

    
#if true    
    //printf( "tgt=%u,rpm=%u,",  tgt_motor_rpm, motor_rpm  );    
    char  prtbuf[64];
    sprintf( prtbuf,  "tgt=%u,rpm=%u,",  tgt_motor_rpm, motor_rpm  );    
    UART2_WriteBuffer( (uint8_t*)prtbuf, strlen(prtbuf) );

#endif
    // PID計算
    long pid_out = PID_Calc( mtr_index, (long)motor_rpm, (long)tgt_motor_rpm );

    
    // PWM値に変換
    // とりあえずそのまま
    return (uint16_t)pid_out;
}


//
// 診断処理
// モーターが回転しない、またはホールセンサー異常の検出
//
// return 正常ならtrue
//
bool MotorDrive_Diag_Proc1(int mtr_index, uint16_t tgt_spd, uint16_t mtr_rpm )
{
    // バッファ
    static int fail_count[MTRSPD_TGT_NUM];
    
    // 条件チェック
    // tgt_spdがゼロ以外で、モーターが回転していない場合カウント
    if( (tgt_spd!=0) && (mtr_rpm==0) )
        fail_count[mtr_index]++;
    else
        fail_count[mtr_index] = 0;
    
    // 1秒（　100ms×10　）以上ならFAIL
    return ( (fail_count[mtr_index]<10) ? true : false );
}


//
// 診断処理
// 過負荷で回転が上がらない場合の検出
//
// return 正常ならtrue
//
bool MotorDrive_Diag_Proc2(int mtr_index, uint16_t tgt_spd, uint16_t mtr_rpm )
{
    // バッファ
    static int fail_count[MTRSPD_TGT_NUM];
    
    // 条件チェック
    // モーター回転数が目標値に達しない場合にカウント
    if( tgt_spd > mtr_rpm )
        fail_count[mtr_index]++;
    else
        fail_count[mtr_index] = 0;
    
    // 3秒（　100ms×30　）以上ならFAIL
    return ( (fail_count[mtr_index]<30) ? true : false );
}


// サーボを安全状態にする。
// （起動時／受信信号が途切れた場合に呼び出す）
void Set_Safe_Mode(void)
{
    // 最小　　　　： 970us = 3880 = 0x0f28
    // ニュートラル：1520us = 6080 = 0x17c0
    // 最大　　　　：2070us = 8280 = 0x2058

    OC1R = 6080;    // ニュートラル
    OC2R = 3880;    // 最小    
    
    // バッファを初期化する。
    int i;
    for( i = 0; i < PW_MEAS_CH_NUM; i++ )
    {
//        rise_buff[i] = 0;
//        pluse_width[i] = 0;
    }
    auto_cmnd_pw_buff[0] = 0;
    auto_cmnd_pw_buff[1] = 0;
    auto_cmnd_pw_buff[2] = 0;
    
}

/**
   Section: External Interrupt Handlers
 */
/**
  Interrupt Handler for EX_INT1 - INT1
*/
// INT1割り込み
// モーター1 速度パルス立ち上がりで割り込み
// ※TMR4を使用
//
// 3パルス毎にカウンタ値取り込み回転速度算出
// （モーター3極なので3パルス毎のが安定する）
//
void __attribute__ ( ( interrupt, no_auto_psv ) ) _INT1Interrupt(void)
{
    //***User Area Begin->code: INT1 - External Interrupt 1***
    static uint16_t count = 0;
    
    if( ++count >= 3 )
    {
        mtrspd_count[0] =  TMR4 / 3;
        // タイマーリセット、再開
        TMR4 = 0x0000;
        IEC1bits.T4IE = true;
        T4CONbits.TON = true;
        ;
        count = 0;
    }
    
    //***User Area End->code: INT1 - External Interrupt 1***
    EX_INT1_InterruptFlagClear();
}


// INT2割り込み
// モーター2 速度パルス立ち上がりで割り込み
// ※TMR5を使用
//
// 3パルス毎にカウンタ値取り込み回転速度算出
// （モーター3極なので3パルス毎のが安定する）
//
void __attribute__ ( ( interrupt, no_auto_psv ) ) _INT2Interrupt(void)
{
    //***User Area Begin->code: INT2 - External Interrupt 2***
    static uint16_t count = 0;
    
    if( ++count >= 3 )
    {
        mtrspd_count[1] =  TMR5 / 3;
        // タイマーリセット、再開
        TMR5 = 0x0000;
        IEC1bits.T5IE = true;
        T5CONbits.TON = true;
        ;
        count = 0;
    }
    
    //***User Area End->code: INT2 - External Interrupt 2***
    EX_INT2_InterruptFlagClear();
}

// TMR4オーバーフロー時の処理
// TMR4 : モーター１速度パルス計測用
void  TMR4_CallBack(void)
{
    // モーター速度カウントを最大値にしてタイマー停止
    T4CONbits.TON = false;
    IEC1bits.T4IE = false;
    TMR4 = 0xffff;
    mtrspd_count[0] =  0xffff;
}

// TMR5オーバーフロー時の処理
// TMR5 : モーター２速度パルス計測用
void  TMR5_CallBack(void)
{
    // モーター速度カウントを最大値にしてタイマー停止
    T5CONbits.TON = false;
    IEC1bits.T5IE = false;
    TMR5 = 0xffff;
    mtrspd_count[1] =  0xffff;
}


#if false
// 以下、InputCaptureによるパルス幅計測
// プロポからの信号を受信機にてPWM変換しPIC入力に使用
// 現在はS.BUS（UART1)を使用し、InputCaptureはモーター回転数計測に使用しているためコメントアウト
//

// パルス幅計測 - 立ち上がり処理
inline void meas_pw_rise_proc( int ch, uint16_t timer )
{
    rise_buff[ch] = timer;
}

// パルス幅計測 - 立ち下がり処理
inline void meas_pw_fall_proc( int ch, uint16_t timer )
{
    if( rise_buff[ch] != 0 || pluse_width[ch] != 0 )
    {
        //パルス幅計測実行
        if(  timer > rise_buff[ch] )
            pluse_width[ch] =  timer - rise_buff[ch];
    }
    
    // watch dog リセット
    wd_count[ch] = 0;

    
}




// Input Captureによる割り込み
void __attribute__ ( ( interrupt, no_auto_psv ) ) _ISR _IC1Interrupt( void )
{
    uint16_t buff;
    const int index = 0;
    
    // IC1処理
    if(IFS0bits.IC1IF)
    {
        IFS0bits.IC1IF = 0;
        
        while( IC1CON1bits.ICBNE )
            buff = IC1BUF;
        
        if( _RB9)   // 入力ピン変えた場合はここも変更
            meas_pw_rise_proc( index, buff );
        else
            meas_pw_fall_proc(index, buff );
        

    }
}


// Input Captureによる割り込み
void __attribute__ ( ( interrupt, no_auto_psv ) ) _ISR _IC2Interrupt( void )
{
    uint16_t buff;
     const int index = 1;
   
    // IC1処理
    if(IFS0bits.IC2IF)
    {
        IFS0bits.IC2IF = 0;
        
        while( IC2CON1bits.ICBNE )
            buff = IC2BUF;
        
        if( _RB8)   // 入力ピン変えた場合はここも変更
            meas_pw_rise_proc( index, buff );
        else
            meas_pw_fall_proc( index, buff );
        

    }
}

// Input Captureによる割り込み
void __attribute__ ( ( interrupt, no_auto_psv ) ) _ISR _IC5Interrupt( void )
{
    uint16_t buff;
    const int index = 2;
    
    // IC1処理
    if(IFS2bits.IC5IF)
    {
        IFS2bits.IC5IF = 0;
        
        while( IC5CON1bits.ICBNE )
            buff = IC5BUF;
        
        if( _RB7)   // 入力ピン変えた場合はここも変更
            meas_pw_rise_proc( index, buff );
        else
            meas_pw_fall_proc( index, buff );
        

    }
}


void __attribute__ ( ( interrupt, no_auto_psv ) ) _ISR _IC3Interrupt( void )
{
     uint16_t buff;
    const int index = 3;
    
    // IC3処理
    if(IFS2bits.IC3IF)
    {
        IFS2bits.IC3IF = 0;
        
        while( IC3CON1bits.ICBNE )
            buff = IC3BUF;
        
        if( _RB11)   // 入力ピン変えた場合はここも変更
            meas_pw_rise_proc( index, buff );
        else
            meas_pw_fall_proc( index, buff );
        

    }
}
/**/


void __attribute__ ( ( interrupt, no_auto_psv ) ) _ISR _IC4Interrupt( void )
{
     uint16_t buff;
    const int index = 4;
    
    // IC1処理
    if(IFS2bits.IC4IF)
    {
        IFS2bits.IC4IF = 0;
        
        while( IC4CON1bits.ICBNE )
            buff = IC4BUF;
        
        if( _RB7)   // 入力ピン変えた場合はここも変更
            meas_pw_rise_proc( index, buff );
        else
            meas_pw_fall_proc( index, buff );
        

    }

}
/**/
#endif

// end of text

