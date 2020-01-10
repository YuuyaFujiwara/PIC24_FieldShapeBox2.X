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
    
#define UNKNOWN_COMMAND (1)     // ���m�̃R�}���h    
    
//*****************************************************************
// �R�}���h�����pCallback�֐��^�̒�`
//
// Callback�֐��͈ȉ��̌^�Ƃ���B
// int16_t funcname(const char* param, int16_t cmdlen, char* respbuff, int16_t* resplen );     
//
// �߂�l�F
//      0  :    ����I��
//      1  :    �������������ׂ��łȂ��R�}���h   
//      2  :    �����̐�������Ȃ�
//      3  :    �����̐�������
// 10?19  :    �����̒l���ُ�( 11=1�ԖځA12=2�ԖځA�E�E�E19=9�ԖځA10=����ȊO)
//
// param    : ��ʋ@�킩���M�����R�}���h�����ւ̃|�C���^�B�R�}���h������w���B
// cmdlen   : ��M�����R�}���h�����f�[�^�o�C�g��    
// respbuff : Callback�֐��Ń��X�|���X������o�b�t�@�B��ʋ@��֑��M�����B
// resplen  : ���X�|���X�Ƃ��ĕԂ��f�[�^�o�C�g��������ϐ��ւ̃|�C���^
//
typedef int16_t (*CMD_CALLBACK_FUNC_t)(const unsigned char* param, int16_t cmdlen, unsigned char* respbuff, int16_t* resplen );    
    
//*****************************************************************
// �R�}���h�����pCallback�֐��^�̒�`

    
//*****************************************************************
// �R�}���h��͂��s���A�������s���B
extern void CommandParser( const unsigned char* command_buff, unsigned char* resp_buff );
//extern int16_t CommandParser2(const char* cmd, int16_t cmdlen, char* respbuff, int16_t* resplen );


#ifdef	__cplusplus
}
#endif

#endif	/* COMMANDPARSER_H */

