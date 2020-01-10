#include "mcc_generated_files/mcc.h"
#include "XBee_Command.h"
#include <string.h>

// Frame ID�����o���B
// �P���C���N�������g�B�i0x01?0xff)
static uint8_t g_frame_id = 0x00;
inline uint8_t FrameID(void)
{
    g_frame_id++;
    if( g_frame_id == 0x00 )
        g_frame_id = 0x01;
    return g_frame_id;
}

// �q�@�A�h���X
#define MAX_CHILD_XBEE_NUM (5)
static uint8_t g_child_xbee_addr[MAX_CHILD_XBEE_NUM][8];
static int g_child_xbee_addr_num = 0;

// �^�C���A�E�g�Ď��p
static int g_timeout_tick = 0;

// 1ms���Ăяo��
inline void XBee_Command_1ms_Proc(void)
{
    g_timeout_tick++;
}




// ATND�𔭍s���āA�q�@�A�h���X�𓾂�
bool XBeeAPI_GetChildrenAddr(void)
{
    g_child_xbee_addr_num = 0;
    
    // ��M�o�b�t�@�N���A
    UART2_ClearReadBuff();
    
    // ND�R�}���h���s
    if( !XBeeAPI_ATND() ) return false;

    // ���X�|���X��M
    int i;
    for( i = 0; i < MAX_CHILD_XBEE_NUM; i++ )
    {
        if( !XBeeAPI_GetChildrenAddr_Resp( g_child_xbee_addr[i] ))
            break;
        g_child_xbee_addr_num = i + 1;
    }
    
    return (bool)( g_child_xbee_addr_num > 0 );
}


// ATND�R�}���h�̃��X�|���X����M���Ďq�@�A�h���X�𓾂�
bool XBeeAPI_GetChildrenAddr_Resp( uint8_t* rslt_buff )
{
    uint8_t recv_buff[32];
    
       
    g_timeout_tick = 0;
    while( g_timeout_tick < 5000 )
    {
        if( XBee_PacketRecv_Check() )
        {
            if( !XBee_PacketRecv( recv_buff, sizeof(recv_buff))) continue;
                    
            if( recv_buff[0] != 0x7e ) continue;
            if( recv_buff[3] != 0x88 ) continue;    // AT command Response
            if( recv_buff[5] != 'N' ) continue;
            if( recv_buff[6] != 'D' ) continue;
            if( recv_buff[7] != 0x00 ) continue;    // status OK
            if( recv_buff[0] != 0x7e ) continue;
           
            memcpy( rslt_buff, recv_buff + 10, 8 );
            
            return true;
            
        }
    }
    
    return false;
}




// Transmit Request���g�p���ău���[�h�L���X�g����B
void XBeeAPI_Broadcast( const uint8_t *data , const unsigned int data_len )
{
    short leng = data_len + 14;
    uint8_t header[17];
    uint8_t cksum;
    int i;
    
    header[0] = 0x7E;                // start delimiter
    header[1] = (uint8_t)(leng >> 8 );
    header[2] = (uint8_t)(leng      );
    header[3] =  0x10;                // Frame Type(Transmit Request)
    header[4] =  0x01;                // Frame ID
    header[5] =  0x00;                // 64-bit dest address 
    header[6] =  0x00;                // 64-bit dest address 
    header[7] =  0x00;                // 64-bit dest address 
    header[8] =  0x00;                // 64-bit dest address 
    header[9] =  0x00;                // 64-bit dest address 
    header[10] = 0x00;                // 64-bit dest address 
    header[11] = 0xff;                // 64-bit dest address 
    header[12] = 0xff;                // 64-bit dest address 
    header[13] = 0xff;                // 16-bit dest address 
    header[14] = 0xfe;                // 16-bit dest address 
    header[15] = 0x01;                // Broadcast radius 
    header[16] = 0x00;                // Options
    
    // calc checksum
    cksum = 0xff;
    for( i = 3; i < sizeof(header); i++ )
        cksum -= header[i];
    for( i = 0; i < data_len; i++ )
        cksum -= data[i];
    
        // check the send buff
    while( UART2_TransmitBufferSizeGet() <= data_len )
        ;

    
    // send
    UART2_WriteBuffer( header, sizeof(header));
    UART2_WriteBuffer( data, data_len );
    UART2_Write( cksum );
}


// ���ׂĂ̎q�@�ɑ��M����B
void XBeeAPI_SendToAllChildren( const uint8_t *data , const unsigned int data_len )
{
    uint8_t recv_buff[16];

    int i;
    for( i = 0; i < g_child_xbee_addr_num; i++ )
    {
        XBeeAPI_Transmit_Request( g_child_xbee_addr[i], data, data_len );
  
#if true
        // ���X�|���X��M
        // ���e�͖�������B
        g_timeout_tick = 0;
        while( g_timeout_tick < 1000 )
        {
            if( XBee_PacketRecv_Check())
            {
                XBee_PacketRecv( recv_buff, sizeof(recv_buff));
                break;
            }
        }
#endif
        
    }
}



// Transmit Request���g�p���Ďw��A�h���X�ɑ��M����B
void XBeeAPI_Transmit_Request( const uint8_t *addr64, const uint8_t *data, const unsigned int data_len )
{
    short leng = data_len + 14;
    uint8_t header[17];
    uint8_t cksum;
    int i;
    
    header[0] = 0x7E;                // start delimiter
    header[1] = (uint8_t)(leng >> 8 );
    header[2] = (uint8_t)(leng      );
    header[3] =  0x10;                // Frame Type(Transmit Request)
    header[4] =  FrameID();           // Frame ID
    header[5] =  addr64[0];                // 64-bit dest address 
    header[6] =  addr64[1];                // 64-bit dest address 
    header[7] =  addr64[2];                // 64-bit dest address 
    header[8] =  addr64[3];                // 64-bit dest address 
    header[9] =  addr64[4];                // 64-bit dest address 
    header[10] = addr64[5];                // 64-bit dest address 
    header[11] = addr64[6];                // 64-bit dest address 
    header[12] = addr64[7];                // 64-bit dest address 
    header[13] = 0xff;                // 16-bit dest address 
    header[14] = 0xfe;                // 16-bit dest address 
    header[15] = 0x00;                // Broadcast radius 
    header[16] = 0x01;                // Options     0x01='Disable ACK'
    
    // calc checksum
    cksum = 0xff;
    for( i = 3; i < sizeof(header); i++ )
        cksum -= header[i];
    for( i = 0; i < data_len; i++ )
        cksum -= data[i];
    
    // check the send buff
    while( UART2_TransmitBufferSizeGet() <= data_len )
        ;
    
#if false
    // CTS��Low�ɂȂ�܂ő҂i�R�}���h���̒��f��h�����߁A�O�ׁ̈j
    while( U2CTS_GetValue() )
        ;
#endif
    
    // send
    UART2_WriteBuffer( header, sizeof(header));
    UART2_WriteBuffer( data, data_len );
    UART2_Write( cksum );    
}


//
// AT�R�}���h'ND'�𔭍s����B
bool XBeeAPI_ATND(void)
{
    uint8_t header[7];
    uint8_t cksum;
    int i;
    
    header[0] = 0x7E;               // start delimiter
    header[1] = 0x00;               // Length(H)
    header[2] = 0x04;               // Length(L)
    header[3] = 0x08;               // Frame Type(AT command)
    header[4] =  FrameID();         // Frame ID
    header[5] =  'N';               // AT Command 
    header[6] =  'D';               // AT Command 
    
    // calc checksum
    cksum = 0xff;
    for( i = 3; i < sizeof(header); i++ )
        cksum -= header[i];
    //for( i = 0; i < data_len; i++ )
    //    cksum -= data[i];
    
    // send
    UART2_WriteBuffer( header, sizeof(header));
    //UART2_WriteBuffer( data, data_len );
    UART2_Write( cksum );    
    
    return true;
}


// �p�P�b�g��M�`�F�b�N�i��M�o�b�t�@�`�F�b�N�̂�)
// 0x7E? Length����M������true
bool XBee_PacketRecv_Check(void)
{
    if( UART2_ReceiveBufferIsEmpty() )
        return false;
    
    if( UART2_Peek(0) != 0x7e )
    {
        UART2_Read();
        return false;
    }
    
    unsigned int rcvlen = UART2_RecvDataNum();
    if( rcvlen < 3 )
        return false;
    
    uint16_t resp_len;
    resp_len = ((UART2_Peek(1) << 8) + UART2_Peek(2));
    if( rcvlen < resp_len + 4 )
        return false;
    
    return true;
}

// �p�P�b�g����M����
bool XBee_PacketRecv( uint8_t *buffer, const unsigned int bufLen )
{
    unsigned int numBytesRead = 0;
    buffer[numBytesRead++] = UART2_Read();
    buffer[numBytesRead++] = UART2_Read();
    buffer[numBytesRead++] = UART2_Read();
    
    uint16_t resp_len;
    resp_len = ((buffer[1] << 8) + buffer[2]);
    
    resp_len += 4;      // 7E, len, len, chksum
    if( resp_len > bufLen)
        resp_len = bufLen;
    
    while (numBytesRead < (resp_len)) 
    {
        buffer[numBytesRead++] = UART2_Read();
    }

    if( numBytesRead < bufLen ) 
        buffer[numBytesRead] = '\0';
    
    return true;
}









// end of text
