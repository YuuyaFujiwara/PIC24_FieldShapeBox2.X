#if 1
////メモリ少ない、及びZigBee未使用につきコメントアウト



/*
 * コマンド解析
 *
 * File:   CommandParser.c
 * Author: Y.Fujiwara
 *
 * Created on 2016/03/31
 */

#include <xc.h>
#include <stdio.h>
#include <string.h>
#include "CommandParser.h"
//#include "assign.h"

#include "PID_Calc.h"
#include "mcc_generated_files/uart1.h"


//*****************************************************************
// ローカル関数プロトタイプ宣言
inline const unsigned char* CommandCompare( const unsigned char* tgt_buff, const  char* src_str );
inline int StrToInt( const unsigned char** ptr_ptr );
long StrToLong( const unsigned char** ptr_ptr );
void uart_send_response_ok( const unsigned char* cmnd );

//*****************************************************************
// グローバル変数
extern bool gpsall_flag;
extern bool gpsonly_flag;

// コマンド配列
// 引数が続く場合はカンマで終了すること。
// （空白で終了しない場合は\rが続くことがチェックされる）
//


//*****************************************************************


// コマンド処理関数宣言

int16_t pid_disp(const unsigned char* param, int16_t cmdlen, unsigned char* respbuff, int16_t* resplen );
int16_t func1(const unsigned char* param, int16_t cmdlen, unsigned char* respbuff, int16_t* resplen );
int16_t func2(const unsigned char* param, int16_t cmdlen, unsigned char* respbuff, int16_t* resplen );
int16_t func3(const unsigned char* param, int16_t cmdlen, unsigned char* respbuff, int16_t* resplen );
int16_t func4(const unsigned char* param, int16_t cmdlen, unsigned char* respbuff, int16_t* resplen );
int16_t func5(const unsigned char* param, int16_t cmdlen, unsigned char* respbuff, int16_t* resplen );
int16_t func6(const unsigned char* param, int16_t cmdlen, unsigned char* respbuff, int16_t* resplen );
int16_t func_pmtk_cmnd(const unsigned char* param, int16_t cmdlen, unsigned char* respbuff, int16_t* resplen );

// コマンド文字列と対応する処理関数のテーブル
typedef struct
{
    const char*         cmnd;
    CMD_CALLBACK_FUNC_t func;
    
} CMND_FUNC_PAIR_T;
// テーブル実体
const CMND_FUNC_PAIR_T cmnd_func_table[] = {
    { "PID", pid_disp },    // 1
    { "PID1,", func1 },     // 2
    { "PID2,", func2 },     // 3
    { "Hello", func3 },     // 4
//    { "func4", func4 },     // 4
//    { "func5", func5 },    // 5
//    { "func6", func6 },   // 6
//    { "func3,", func_pmtk_cmnd },   // 7
};
#define CMND_ARRAY_SIZE (4)


//const char* cmnd_array[] = {
//    "SVPWM,",               // 0
////    "led_off",              // 1
////    "get_temp",             // 2
//};
//#define CMND_ARRAY_SIZE (3)
//#define CMND_ARRAY_SIZE (sizeof(cmnd_array)/sizeof(unsigned char*))'メモリが少なくなるとうまく動かない？

const char* comma_ok = ",OK";


//*****************************************************************
int16_t pid_disp(const unsigned char* param, int16_t cmdlen, unsigned char* respbuff, int16_t* resplen )
{
    // レスポンスでなく直接表示
//   printf( (char*)respbuff, "MTR1:P=%.3f,I=%.3f,D=%.3f\n", PGain[0], IGain[0], DGain[0] );
//   printf( (char*)respbuff, "MTR2:P=%.3f,I=%.3f,D=%.3f\n", PGain[1], IGain[1], DGain[1] );
    sprintf( (char*)respbuff, "P=%.3f,I=%.3f,D=%.3f\nP=%.3f,I=%.3f,D=%.3f\n", PGain[0], IGain[0], DGain[0], PGain[1], IGain[1], DGain[1] );
    *resplen = strlen( (char*)respbuff );
     
    // 正常終了
    return 0;
}

//*****************************************************************
int16_t func1(const unsigned char* param, int16_t cmdlen, unsigned char* respbuff, int16_t* resplen )
{   
    // パラメータ取り出し
    // PIDゲインに設定
    PGain[0] = (double)StrToLong( &param  ) / 1000.0;
    IGain[0] = (double)StrToLong( &param  ) / 1000.0;
    DGain[0] = (double)StrToLong( &param  ) / 1000.0;
    
    //レスポンス
    sprintf( (char*)respbuff, "MTR1:P=%.3f,I=%.3f,D=%.3f\n", PGain[0], IGain[0], DGain[0] );
    *resplen = strlen( (char*)respbuff );
     
    // 正常終了
    return 0;
}

//extern void debug_UART1_Status(void);

//*****************************************************************
int16_t func2(const unsigned char* param, int16_t cmdlen, unsigned char* respbuff, int16_t* resplen )
{
    // パラメータ取り出し
    // PIDゲインに設定
    PGain[1] = (double)StrToLong( &param  ) / 1000.0;
    IGain[1] = (double)StrToLong( &param  ) / 1000.0;
    DGain[1] = (double)StrToLong( &param  ) / 1000.0;
    
    //レスポンス
    sprintf( (char*)respbuff, "MTR2:P=%.3f,I=%.3f,D=%.3f\n", PGain[1], IGain[1], DGain[1] );
    *resplen = strlen( (char*)respbuff );
     
    // 正常終了
    return 0;
}
//*****************************************************************
int16_t func3(const unsigned char* param, int16_t cmdlen, unsigned char* respbuff, int16_t* resplen )
{
//    strcpy( (char*)respbuff, msg );
//    puts("func3 was called!\n");
//    UART2_WriteBuffer( (unsigned char*)"How are you?\n", 13 );
    strcpy( (char*)respbuff, "How are You?\n" );
    return 0;
}
//*****************************************************************
int16_t func4(const unsigned char* param, int16_t cmdlen, unsigned char* respbuff, int16_t* resplen )
{
    strcpy( (char*)respbuff, "func4() was called.\n" );
    return 0;
//    const char* cmdstr = "led_off";
//    // コマンドチェック
//    if( strcmp( cmd, cmdstr) != 0 )
//        return UNKNOWN_COMMAND;
//
//    // コマンド処理
//    LED = 1;
//    
//    // OKレスポンス
//    int16_t ret = 0;
//    sprintf( respbuff, "%d,%s", ret, cmdstr );
//
//    return ret;
}
//*****************************************************************
int16_t func5(const unsigned char* param, int16_t cmdlen, unsigned char* respbuff, int16_t* resplen )
{
    strcpy( (char*)respbuff, "func5() was called.\n" );
    return 0;
//    puts("gpsall flag was activated.\n");
//    gpsall_flag = true;
//    return 1;
//    const char* cmdstr = "led_on";
//    // コマンドチェック
//    if( strcmp( cmd, cmdstr) != 0 )
//        return UNKNOWN_COMMAND;
//
//    // コマンド処理
//    LED = 1;
//    
//    // OKレスポンス
//    int16_t ret = 0;
//    sprintf( respbuff, "%d,%s", ret, cmdstr );
//
//    return ret;
}

//*****************************************************************
int16_t func6(const unsigned char* param, int16_t cmdlen, unsigned char* respbuff, int16_t* resplen )
{
    strcpy( (char*)respbuff, "func6() was called.\n" );
    return 0;
//    puts("gpsall flag was activated.\n");
//    gpsall_flag = true;
//    gpsonly_flag = true;
//    return 1;
}


//*****************************************************************
// GPSモジュールに対してPMTKコマンドを発行する。
// param : PMTKコマンドのコマンド番号およびパラメータ 
//　　　　　※"$PMTK", "*", checksum, CR, LFは含まない
//
// ※レスポンスは待たない。
int16_t func_pmtk_cmnd(const unsigned char* param, int16_t cmdlen, unsigned char* respbuff, int16_t* resplen )
{
    uint8_t cksum = 0x02;   // 'P' 'M' 'T' 'K'のxor
    char buff[3];
    
    printf("$PMTK");
    printf( (char*)param );

    // checksum演算
    while( *param )
    {
        cksum ^= *param;
        param++;
    }

    sprintf( buff, "%02X", cksum );
    putchar('*');
    putchar( buff[0] );
    putchar( buff[1] );
    putchar( 0x0d );   // cr
    putchar( 0x0a );   // lf

    return 0;
}

//
//static const CMD_CALLBACK_FUNC_t Function_table[] = {
//    func1,
//    func2,
//    func3,
//    func4,
//    func5
//}; 




//*****************************************************************
// コマンド解析を行い、処理を行う。
void CommandParser( const unsigned char* command_buff, unsigned char* resp_buff )
{
    const unsigned char* ptr = command_buff;
    CMD_CALLBACK_FUNC_t func = NULL;
   unsigned int  cmnd_no;
//  char tmpbuff[32];
    int16_t  resplen = 0;
    int16_t ret;

    // cmnd_arrayに該当するコマンドがあるか探す
    for( cmnd_no = 0; cmnd_no < CMND_ARRAY_SIZE; cmnd_no++ )
    {
//        puts(cmnd_func_table[cmnd_no].cmnd);
//        puts("\n");
        if( (ptr = CommandCompare( command_buff, cmnd_func_table[cmnd_no].cmnd )) != 0 )
        {
            // コマンド処理関数取り出し
            func = cmnd_func_table[cmnd_no].func;
            break;
        }
    }
    if( func == NULL ) 
    {
        printf( "Can't find function of command '%s'\n", command_buff );
        return;
    }
    
    // コマンド処理関数呼び出し実行
    ret = func( ptr,  strlen((const char*)ptr), resp_buff, &resplen );
   
    if( ret )
    {
        printf( "CommandParser error. ret = %d", ret );
    }
            
    return;


}


//*****************************************************************
// コマンドが引数文字列と同じか比較する。
//      tgt_buff : 比較対象コマンドバッファ
//      src_str  : 比較文字列
//
//   一致　：tgt_buffの、比較対象文字の次のポインタを返す。
//   不一致：NULLポインタを返す。
//
//   ※src_strが','で終わる場合は空白まで、それ以外は'\r'が続くことがチェックされる。
//
inline const unsigned char* CommandCompare( const unsigned char* tgt_buff, const char* src_str )
{
    while( *src_str )
    {
        if( *tgt_buff != *src_str )
        {
            return (const unsigned char*)0;
        }

        tgt_buff++;
        src_str++;
    }

    if( *tgt_buff == '\0' || *tgt_buff == '\r' ||  *(--src_str)==',' )
        return tgt_buff;
    else
        return (const unsigned char*)0;

}

//*****************************************************************
// 文字列から数字を取り出す。(int型）
//　詳細はStrToLong関数を参照
inline int StrToInt( const unsigned char** ptr_ptr )
{
    return (int)StrToLong( ptr_ptr );
}


//*****************************************************************
// 文字列から数字を取り出す。(long型）
//　・文字列引数はポインタのポインタで与える。
//　・引数のポインタのポインタは、取り出した数字の分進む。
//　　（終了時点で、数字の次の文字を指す）
//  ・数字又は+-から開始する。先頭の無効文字は無視される。
//    （空白やカンマは無視される。）
//　・数字以外で終了する。
//
// ptr_ptr : 変換元文字列のポインタのポインタ
//          (変換後ポインタを進めるためにポインタのポインタとなっているので注意)
//
long StrToLong( const unsigned char** ptr_ptr )
{
    long buff = 0;
    char negative_flag = 0;

    // 数字又は+-以外をスキップ
    while( (**ptr_ptr)<'0' || (**ptr_ptr)>'9' )
    {
        if( (**ptr_ptr)=='-' )
        {
            (*ptr_ptr)++;
            negative_flag = 1;
            break;
        }
        if( (**ptr_ptr)=='+' )
        {
            (*ptr_ptr)++;
            break;
        }
        if( (**ptr_ptr)== 0 ) // NULL
            return buff;

        (*ptr_ptr)++;
    }

    // 連続した数字を読み取る
    while( (**ptr_ptr)>='0' && (**ptr_ptr)<='9' )
    {
        buff = buff * 10 + ((**ptr_ptr)-'0');
        (*ptr_ptr)++;
    }
    if( negative_flag )
        buff *= -1;

    return buff;
}
#else
// ビルド通すためのダミー
void CommandParser( const unsigned char* command_buff, unsigned char* resp_buff )
{
    return;
}
#endif
// end of text