#if 1
////���������Ȃ��A�y��ZigBee���g�p�ɂ��R�����g�A�E�g



/*
 * �R�}���h���
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
// ���[�J���֐��v���g�^�C�v�錾
inline const unsigned char* CommandCompare( const unsigned char* tgt_buff, const  char* src_str );
inline int StrToInt( const unsigned char** ptr_ptr );
long StrToLong( const unsigned char** ptr_ptr );
void uart_send_response_ok( const unsigned char* cmnd );

//*****************************************************************
// �O���[�o���ϐ�
extern bool gpsall_flag;
extern bool gpsonly_flag;

// �R�}���h�z��
// �����������ꍇ�̓J���}�ŏI�����邱�ƁB
// �i�󔒂ŏI�����Ȃ��ꍇ��\r���������Ƃ��`�F�b�N�����j
//


//*****************************************************************


// �R�}���h�����֐��錾

int16_t pid_disp(const unsigned char* param, int16_t cmdlen, unsigned char* respbuff, int16_t* resplen );
int16_t func1(const unsigned char* param, int16_t cmdlen, unsigned char* respbuff, int16_t* resplen );
int16_t func2(const unsigned char* param, int16_t cmdlen, unsigned char* respbuff, int16_t* resplen );
int16_t func3(const unsigned char* param, int16_t cmdlen, unsigned char* respbuff, int16_t* resplen );
int16_t func4(const unsigned char* param, int16_t cmdlen, unsigned char* respbuff, int16_t* resplen );
int16_t func5(const unsigned char* param, int16_t cmdlen, unsigned char* respbuff, int16_t* resplen );
int16_t func6(const unsigned char* param, int16_t cmdlen, unsigned char* respbuff, int16_t* resplen );
int16_t func_pmtk_cmnd(const unsigned char* param, int16_t cmdlen, unsigned char* respbuff, int16_t* resplen );

// �R�}���h������ƑΉ����鏈���֐��̃e�[�u��
typedef struct
{
    const char*         cmnd;
    CMD_CALLBACK_FUNC_t func;
    
} CMND_FUNC_PAIR_T;
// �e�[�u������
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
//#define CMND_ARRAY_SIZE (sizeof(cmnd_array)/sizeof(unsigned char*))'�����������Ȃ��Ȃ�Ƃ��܂������Ȃ��H

const char* comma_ok = ",OK";


//*****************************************************************
int16_t pid_disp(const unsigned char* param, int16_t cmdlen, unsigned char* respbuff, int16_t* resplen )
{
    // ���X�|���X�łȂ����ڕ\��
//   printf( (char*)respbuff, "MTR1:P=%.3f,I=%.3f,D=%.3f\n", PGain[0], IGain[0], DGain[0] );
//   printf( (char*)respbuff, "MTR2:P=%.3f,I=%.3f,D=%.3f\n", PGain[1], IGain[1], DGain[1] );
    sprintf( (char*)respbuff, "P=%.3f,I=%.3f,D=%.3f\nP=%.3f,I=%.3f,D=%.3f\n", PGain[0], IGain[0], DGain[0], PGain[1], IGain[1], DGain[1] );
    *resplen = strlen( (char*)respbuff );
     
    // ����I��
    return 0;
}

//*****************************************************************
int16_t func1(const unsigned char* param, int16_t cmdlen, unsigned char* respbuff, int16_t* resplen )
{   
    // �p�����[�^���o��
    // PID�Q�C���ɐݒ�
    PGain[0] = (double)StrToLong( &param  ) / 1000.0;
    IGain[0] = (double)StrToLong( &param  ) / 1000.0;
    DGain[0] = (double)StrToLong( &param  ) / 1000.0;
    
    //���X�|���X
    sprintf( (char*)respbuff, "MTR1:P=%.3f,I=%.3f,D=%.3f\n", PGain[0], IGain[0], DGain[0] );
    *resplen = strlen( (char*)respbuff );
     
    // ����I��
    return 0;
}

//extern void debug_UART1_Status(void);

//*****************************************************************
int16_t func2(const unsigned char* param, int16_t cmdlen, unsigned char* respbuff, int16_t* resplen )
{
    // �p�����[�^���o��
    // PID�Q�C���ɐݒ�
    PGain[1] = (double)StrToLong( &param  ) / 1000.0;
    IGain[1] = (double)StrToLong( &param  ) / 1000.0;
    DGain[1] = (double)StrToLong( &param  ) / 1000.0;
    
    //���X�|���X
    sprintf( (char*)respbuff, "MTR2:P=%.3f,I=%.3f,D=%.3f\n", PGain[1], IGain[1], DGain[1] );
    *resplen = strlen( (char*)respbuff );
     
    // ����I��
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
//    // �R�}���h�`�F�b�N
//    if( strcmp( cmd, cmdstr) != 0 )
//        return UNKNOWN_COMMAND;
//
//    // �R�}���h����
//    LED = 1;
//    
//    // OK���X�|���X
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
//    // �R�}���h�`�F�b�N
//    if( strcmp( cmd, cmdstr) != 0 )
//        return UNKNOWN_COMMAND;
//
//    // �R�}���h����
//    LED = 1;
//    
//    // OK���X�|���X
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
// GPS���W���[���ɑ΂���PMTK�R�}���h�𔭍s����B
// param : PMTK�R�}���h�̃R�}���h�ԍ�����уp�����[�^ 
//�@�@�@�@�@��"$PMTK", "*", checksum, CR, LF�͊܂܂Ȃ�
//
// �����X�|���X�͑҂��Ȃ��B
int16_t func_pmtk_cmnd(const unsigned char* param, int16_t cmdlen, unsigned char* respbuff, int16_t* resplen )
{
    uint8_t cksum = 0x02;   // 'P' 'M' 'T' 'K'��xor
    char buff[3];
    
    printf("$PMTK");
    printf( (char*)param );

    // checksum���Z
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
// �R�}���h��͂��s���A�������s���B
void CommandParser( const unsigned char* command_buff, unsigned char* resp_buff )
{
    const unsigned char* ptr = command_buff;
    CMD_CALLBACK_FUNC_t func = NULL;
   unsigned int  cmnd_no;
//  char tmpbuff[32];
    int16_t  resplen = 0;
    int16_t ret;

    // cmnd_array�ɊY������R�}���h�����邩�T��
    for( cmnd_no = 0; cmnd_no < CMND_ARRAY_SIZE; cmnd_no++ )
    {
//        puts(cmnd_func_table[cmnd_no].cmnd);
//        puts("\n");
        if( (ptr = CommandCompare( command_buff, cmnd_func_table[cmnd_no].cmnd )) != 0 )
        {
            // �R�}���h�����֐����o��
            func = cmnd_func_table[cmnd_no].func;
            break;
        }
    }
    if( func == NULL ) 
    {
        printf( "Can't find function of command '%s'\n", command_buff );
        return;
    }
    
    // �R�}���h�����֐��Ăяo�����s
    ret = func( ptr,  strlen((const char*)ptr), resp_buff, &resplen );
   
    if( ret )
    {
        printf( "CommandParser error. ret = %d", ret );
    }
            
    return;


}


//*****************************************************************
// �R�}���h������������Ɠ�������r����B
//      tgt_buff : ��r�ΏۃR�}���h�o�b�t�@
//      src_str  : ��r������
//
//   ��v�@�Ftgt_buff�́A��r�Ώە����̎��̃|�C���^��Ԃ��B
//   �s��v�FNULL�|�C���^��Ԃ��B
//
//   ��src_str��','�ŏI���ꍇ�͋󔒂܂ŁA����ȊO��'\r'���������Ƃ��`�F�b�N�����B
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
// �����񂩂琔�������o���B(int�^�j
//�@�ڍׂ�StrToLong�֐����Q��
inline int StrToInt( const unsigned char** ptr_ptr )
{
    return (int)StrToLong( ptr_ptr );
}


//*****************************************************************
// �����񂩂琔�������o���B(long�^�j
//�@�E����������̓|�C���^�̃|�C���^�ŗ^����B
//�@�E�����̃|�C���^�̃|�C���^�́A���o���������̕��i�ށB
//�@�@�i�I�����_�ŁA�����̎��̕������w���j
//  �E��������+-����J�n����B�擪�̖��������͖��������B
//    �i�󔒂�J���}�͖��������B�j
//�@�E�����ȊO�ŏI������B
//
// ptr_ptr : �ϊ���������̃|�C���^�̃|�C���^
//          (�ϊ���|�C���^��i�߂邽�߂Ƀ|�C���^�̃|�C���^�ƂȂ��Ă���̂Œ���)
//
long StrToLong( const unsigned char** ptr_ptr )
{
    long buff = 0;
    char negative_flag = 0;

    // ��������+-�ȊO���X�L�b�v
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

    // �A������������ǂݎ��
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
// �r���h�ʂ����߂̃_�~�[
void CommandParser( const unsigned char* command_buff, unsigned char* resp_buff )
{
    return;
}
#endif
// end of text