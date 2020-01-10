/* 
 * File:   XBee_Command.h
 * Author: YuyaFujiwara
 *
 * Created on 2019/08/08, 16:39
 */

#ifndef XBEE_COMMAND_H
#define	XBEE_COMMAND_H

#ifdef	__cplusplus
extern "C" {
#endif

// 1ms毎呼び出し
extern  void XBee_Command_1ms_Proc(void);

// ATNDを発行して、子機アドレスを得る
extern bool XBeeAPI_GetChildrenAddr(void);

// ATNDコマンドのレスポンスを受信して子機アドレスを得る
extern bool XBeeAPI_GetChildrenAddr_Resp( uint8_t* rslt_buff );

// Transmit Requestを使用してブロードキャストする。
extern void XBeeAPI_Broadcast( const uint8_t *data , const unsigned int data_len );

// すべての子機に送信する。
extern void XBeeAPI_SendToAllChildren( const uint8_t *data , const unsigned int data_len );

// Transmit Requestを使用して指定アドレスに送信する。
extern void XBeeAPI_Transmit_Request( const uint8_t *addr64, const uint8_t *data, const unsigned int data_len );

// ATコマンド'ND'を発行する。
extern bool XBeeAPI_ATND(void);

// パケット受信チェック（受信バッファチェックのみ)
// 0x7E? Length分受信したらtrue
extern bool XBee_PacketRecv_Check(void);

// パケットを受信する
extern bool XBee_PacketRecv( uint8_t *buffer, const unsigned int bufLen );


#ifdef	__cplusplus
}
#endif

#endif	/* XBEE_COMMAND_H */

