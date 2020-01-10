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

// 1ms���Ăяo��
extern  void XBee_Command_1ms_Proc(void);

// ATND�𔭍s���āA�q�@�A�h���X�𓾂�
extern bool XBeeAPI_GetChildrenAddr(void);

// ATND�R�}���h�̃��X�|���X����M���Ďq�@�A�h���X�𓾂�
extern bool XBeeAPI_GetChildrenAddr_Resp( uint8_t* rslt_buff );

// Transmit Request���g�p���ău���[�h�L���X�g����B
extern void XBeeAPI_Broadcast( const uint8_t *data , const unsigned int data_len );

// ���ׂĂ̎q�@�ɑ��M����B
extern void XBeeAPI_SendToAllChildren( const uint8_t *data , const unsigned int data_len );

// Transmit Request���g�p���Ďw��A�h���X�ɑ��M����B
extern void XBeeAPI_Transmit_Request( const uint8_t *addr64, const uint8_t *data, const unsigned int data_len );

// AT�R�}���h'ND'�𔭍s����B
extern bool XBeeAPI_ATND(void);

// �p�P�b�g��M�`�F�b�N�i��M�o�b�t�@�`�F�b�N�̂�)
// 0x7E? Length����M������true
extern bool XBee_PacketRecv_Check(void);

// �p�P�b�g����M����
extern bool XBee_PacketRecv( uint8_t *buffer, const unsigned int bufLen );


#ifdef	__cplusplus
}
#endif

#endif	/* XBEE_COMMAND_H */

