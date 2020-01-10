/* 
 * File:   Propo_Rcv_Servo.h
 * Author: 藤原裕也
 *
 * Created on 2016/06/29, 11:18
 */

#ifndef PROPO_RCV_SERVO_H
#define	PROPO_RCV_SERVO_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>

/*
 * プロポ信号を受信しサーボを駆動するルーチン
   1ms毎に呼び出し
 */
extern void MotorControl_Proc(void);

/*****************************************************************************
 * Public values
 ****************************************************************************/

// 上位からコマンドで送られたパルス幅値
extern volatile uint16_t auto_cmnd_pw_buff[];
 


// プロポ受信パルス幅
//extern uint16_t pluse_width[];

    
extern int servo_control_mode;      // サーボ制御モード(プロポ CH5によりコントロールする)
                                    //   0 : CH5=奥　（パルス幅小） 
                                    //   1 : CH5=中　（パルス幅中） 
                                    //   2 : CH5=手前（パルス幅大） 
     
// モーター速度カウント値
extern uint16_t mtrspd_count[];
    
/*
 * 初期化
 */
extern void MotorDrive_Init(void);
    
/*
 100ms tick処理
 */
extern void MotorDrive_100ms_tick_proc(void);

/*
 モーター速度の平均値を算出する
 * index : モーターIndex
 * current : 現在速度
 * return : 平均値
 * 
 */
extern uint16_t MotorSpeed_Average( int index, uint16_t current );   
    


#ifdef	__cplusplus
}
#endif

#endif	/* PROPO_RCV_SERVO_H */

