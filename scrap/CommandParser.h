/* 
 * File:   CommandParser.h
 * Author: 
 *
 * Created on 2016/03/31
 */

#ifndef COMMANDPARSER_H
#define	COMMANDPARSER_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>
#include <stdint.h>    
    
#define UNKNOWN_COMMAND (1)     // 未知のコマンド    
    
//*****************************************************************
// コマンド処理用Callback関数型の定義
//
// Callback関数は以下の型とする。
// int16_t funcname(const char* param, int16_t cmdlen, char* respbuff, int16_t* resplen );     
//
// 戻り値：
//      0  :    正常終了
//      1  :    自分が処理すべきでないコマンド   
//      2  :    引数の数が足りない
//      3  :    引数の数が多い
// 10?19  :    引数の値が異常( 11=1番目、12=2番目、・・・19=9番目、10=それ以外)
//
// param    : 上位機器から受信したコマンド引数へのポインタ。コマンド直後を指す。
// cmdlen   : 受信したコマンド引数データバイト数    
// respbuff : Callback関数でレスポンスを入れるバッファ。上位機器へ送信される。
// resplen  : レスポンスとして返すデータバイト数を入れる変数へのポインタ
//
typedef int16_t (*CMD_CALLBACK_FUNC_t)(const unsigned char* param, int16_t cmdlen, unsigned char* respbuff, int16_t* resplen );    
    
//*****************************************************************
// コマンド処理用Callback関数型の定義

    
//*****************************************************************
// コマンド解析を行い、処理を行う。
extern void CommandParser( const unsigned char* command_buff, unsigned char* resp_buff );
//extern int16_t CommandParser2(const char* cmd, int16_t cmdlen, char* respbuff, int16_t* resplen );


#ifdef	__cplusplus
}
#endif

#endif	/* COMMANDPARSER_H */

