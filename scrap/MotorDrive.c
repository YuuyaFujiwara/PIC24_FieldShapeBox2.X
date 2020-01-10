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

uint16_t SBus_to_Target_Value( uint16_t sbus, uint16_t onoff, uint16_t target_max );    // ���� sbus�l�ɉ����A�ŏ��i�O�j?�ő�itarget_max�j�̒l��Ԃ��B
uint16_t Calc_MotorControl_Pwm_PID( int mtr_index, uint16_t spd, uint16_t onoff);    // �v���|��M�@�p���X�����烂�[�^�[�h���C�o�쓮PWM�l�𓾂�B

// �f�f����
bool MotorDrive_Diag_Proc1(int mtr_index, uint16_t tgt_spd, uint16_t mtr_rpm );
bool MotorDrive_Diag_Proc2(int mtr_index, uint16_t tgt_spd, uint16_t mtr_rpm );

void Set_Safe_Mode(void);       // �T�[�{�����S��Ԃɂ���B

uint16_t dbg_motor_spd( uint16_t onoff1, uint16_t onoff2 );


/*****************************************************************************
 * Public functions
 ****************************************************************************/


/*****************************************************************************
 * Public values
 ****************************************************************************/
volatile uint16_t auto_cmnd_pw_buff[] = { 0, 0, 0, 0, 0 }; // ��ʂ���R�}���h�ő���ꂽ�p���X���l
 
int servo_control_mode = 1;         // �T�[�{���䃂�[�h(�v���| CH5�ɂ��R���g���[������)
                                    //   0 : CH5=���@�i�p���X�����j 
                                    //   1 : CH5=���@�i�p���X�����j 
                                    //   2 : CH5=��O�i�p���X����j 

// ���[�^�[���x�J�E���g�l
#define MTRSPD_TGT_NUM (2)
uint16_t mtrspd_count[MTRSPD_TGT_NUM] = { 0xffff, 0xffff };



/*****************************************************************************
 * Private Values
 ****************************************************************************/
    #define PW_MEAS_CH_NUM (5)
#if false
    // �p���X���v�� �o�b�t�@
    static uint16_t rise_buff[PW_MEAS_CH_NUM] = { 0, 0, 0, 0, 0 };
           uint16_t pluse_width[PW_MEAS_CH_NUM] = { 0, 0, 0, 0, 0 };
;
#endif
//static bool     mtrspd_ovflow[MTRSPD_TGT_NUM] = { false, false };
           
int tmptgtrpm;// for debug
int tmprpm;//for debug           
// watch dog�p�J�E���^
static volatile uint16_t wd_count[PW_MEAS_CH_NUM] = { 0, 0, 0, 0, 0 };

// PID�����p




#if false
/*
 ���[�^�[���x�̕��ϒl���Z�o����
 * index : ���[�^�[Index
 * current : ���ݑ��x
 * return : ���ϒl
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
 * ������
 */
void MotorDrive_Init(void)
{
    // ���胂�[�^�[
    PID_Init(0);
    PGain[0] = 0.010;
    IGain[0] = 0.005;
    DGain[0] = 0.010;
    AnsMax[0] = 120;    // ���[�^�[�h���C�oPWM MAX
    AnsMin[0] =     0;  // ���[�^�[�h���C�oPWM Min   
    
    // �g�U���[�^�[
    PID_Init(1);
    PGain[1] = 0.070;
    IGain[1] = 0.010;
    DGain[1] = 0.020;
    AnsMax[1] = 120;    // ���[�^�[�h���C�oPWM MAX
    AnsMin[1] =   0;    // ���[�^�[�h���C�oPWM Min   
}


/*
 100ms tick����
 */
void MotorDrive_100ms_tick_proc(void)
{
    
    
    // for debug
    // UART���M
#if false
    int tmppropo = sbus_ch_buff[8];
    int tgtrpm   = tmptgtrpm;
   // int rpm  = mtrspd_count[0];
    int tmppwm  = OC3R;
    printf( "tgt=%d,rpm=%d,pwm=%d,",  tgtrpm, tmprpm, tmppwm  );
#endif

    // �v���|S.BUS���PID����ɂă��[�^�[�쓮PWM���߂�
    MotorControl_Proc();
    
    
    // Diag����
    
}



/*
 * �v���|�M������M���T�[�{���쓮���郋�[�`��
   1ms���ɌĂяo��
 */
void MotorControl_Proc(void)
{
    // �v���|�M���ɂ�胂�[�^�[�h���C�o�쓮
    if( sbus_flag_buff == 0x00 )
    {
        // SW-D(CH6)�ɂ��PID�����ON/OFF��؂�ւ���
        if( sbus_ch_buff[SBUS_CH6] > 1000 )
        {
            // PID����
#if true
            // �v���|�Z���^�[����5000rpm�ɂȂ�悤�AMAX�l��ݒ�i���@�������j
            OC3R =  SBus_to_Target_Value( sbus_ch_buff[SBUS_CH9], sbus_ch_buff[SBUS_CH8], 36 /*������18����5000rpm*/ );    // CH9, CH8
            OC4R =  SBus_to_Target_Value( sbus_ch_buff[SBUS_CH10], sbus_ch_buff[SBUS_CH7], 54 /*���g�U27����5000rpm*/ );    // CH10, CH7
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
 * ���� sbus�l�ɉ����A�ŏ��i�O�j?�ő�itarget_max�j�̒l��Ԃ��B
 * 
 * sbus : S.Bus�l�B�@�ŏ���1904�@�ő厞144
 * onoff : On/Off SW�̏�Ԃ�\��S.BUS�l�@ON=144�@OFF��1904
 * target_max : S.Bus�l���ő�i144�j�̏ꍇ�ɕԂ��l
 * 
 * return : ���Z��̒l�B�O ? target_max
 */
uint16_t SBus_to_Target_Value( uint16_t sbus, uint16_t onoff, uint16_t target_max )
{
    // sbus, onoff���[���̏ꍇ�AS.BUS��M���Ă��Ȃ��̂Œ�~
    if( sbus==0 || onoff==0 )
        return 0;   
    
    // S.Bus�͈̓`�F�b�N
    if( sbus < 144 ) sbus = 144;
    if( sbus > 1904 ) sbus = 1904;
    
    double tmp = 0;
    if( onoff < 1000 )    // on/off sw��1000�����Ȃ�ON
    {
        tmp = (double)((int)1904 - sbus);
        tmp *= target_max;    // �ő�l
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

    if( onoff1 < 1000 || onoff2<1000 )    // on/off sw��1000�����Ȃ�ON
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
 * �g�U���[�^�[�pPWM����
 * �v���|��M�@ S.BUS�l���烂�[�^�[�h���C�o�쓮PWM�l�𓾂�B
 * �g�U���[�^�[���x����FCH9(DT5)�@1904(�x��)?144(����)
 * �g�U���[�^�[ON/OFF  �FSW-B(CH8)�@ON=144�@OFF��1904
 * 
 */
uint16_t Calc_MotorControl_Pwm_PID(int mtr_index, uint16_t spd, uint16_t onoff)
{ 
    // �^�[�Q�b�gRPM
    uint16_t tgt_motor_rpm = SBus_to_Target_Value( spd, onoff, 10000 /*���ő�rpm*/ );
//    printf("spd=%d,onoff=%d,rpm=%d\n",spd,onoff,tgt_motor_rpm );
//   tmptgtrpm = tgt_motor_rpm;// for debug
    if( tgt_motor_rpm == 0 ) 
    {
        PID_Init(mtr_index);
        return 0;
    }
    
    
    // ����RPM
    // 10,000,000 / �J�E���g = RPM
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
    // PID�v�Z
    long pid_out = PID_Calc( mtr_index, (long)motor_rpm, (long)tgt_motor_rpm );

    
    // PWM�l�ɕϊ�
    // �Ƃ肠�������̂܂�
    return (uint16_t)pid_out;
}


//
// �f�f����
// ���[�^�[����]���Ȃ��A�܂��̓z�[���Z���T�[�ُ�̌��o
//
// return ����Ȃ�true
//
bool MotorDrive_Diag_Proc1(int mtr_index, uint16_t tgt_spd, uint16_t mtr_rpm )
{
    // �o�b�t�@
    static int fail_count[MTRSPD_TGT_NUM];
    
    // �����`�F�b�N
    // tgt_spd���[���ȊO�ŁA���[�^�[����]���Ă��Ȃ��ꍇ�J�E���g
    if( (tgt_spd!=0) && (mtr_rpm==0) )
        fail_count[mtr_index]++;
    else
        fail_count[mtr_index] = 0;
    
    // 1�b�i�@100ms�~10�@�j�ȏ�Ȃ�FAIL
    return ( (fail_count[mtr_index]<10) ? true : false );
}


//
// �f�f����
// �ߕ��ׂŉ�]���オ��Ȃ��ꍇ�̌��o
//
// return ����Ȃ�true
//
bool MotorDrive_Diag_Proc2(int mtr_index, uint16_t tgt_spd, uint16_t mtr_rpm )
{
    // �o�b�t�@
    static int fail_count[MTRSPD_TGT_NUM];
    
    // �����`�F�b�N
    // ���[�^�[��]�����ڕW�l�ɒB���Ȃ��ꍇ�ɃJ�E���g
    if( tgt_spd > mtr_rpm )
        fail_count[mtr_index]++;
    else
        fail_count[mtr_index] = 0;
    
    // 3�b�i�@100ms�~30�@�j�ȏ�Ȃ�FAIL
    return ( (fail_count[mtr_index]<30) ? true : false );
}


// �T�[�{�����S��Ԃɂ���B
// �i�N�����^��M�M�����r�؂ꂽ�ꍇ�ɌĂяo���j
void Set_Safe_Mode(void)
{
    // �ŏ��@�@�@�@�F 970us = 3880 = 0x0f28
    // �j���[�g�����F1520us = 6080 = 0x17c0
    // �ő�@�@�@�@�F2070us = 8280 = 0x2058

    OC1R = 6080;    // �j���[�g����
    OC2R = 3880;    // �ŏ�    
    
    // �o�b�t�@������������B
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
// INT1���荞��
// ���[�^�[1 ���x�p���X�����オ��Ŋ��荞��
// ��TMR4���g�p
//
// 3�p���X���ɃJ�E���^�l��荞�݉�]���x�Z�o
// �i���[�^�[3�ɂȂ̂�3�p���X���̂����肷��j
//
void __attribute__ ( ( interrupt, no_auto_psv ) ) _INT1Interrupt(void)
{
    //***User Area Begin->code: INT1 - External Interrupt 1***
    static uint16_t count = 0;
    
    if( ++count >= 3 )
    {
        mtrspd_count[0] =  TMR4 / 3;
        // �^�C�}�[���Z�b�g�A�ĊJ
        TMR4 = 0x0000;
        IEC1bits.T4IE = true;
        T4CONbits.TON = true;
        ;
        count = 0;
    }
    
    //***User Area End->code: INT1 - External Interrupt 1***
    EX_INT1_InterruptFlagClear();
}


// INT2���荞��
// ���[�^�[2 ���x�p���X�����オ��Ŋ��荞��
// ��TMR5���g�p
//
// 3�p���X���ɃJ�E���^�l��荞�݉�]���x�Z�o
// �i���[�^�[3�ɂȂ̂�3�p���X���̂����肷��j
//
void __attribute__ ( ( interrupt, no_auto_psv ) ) _INT2Interrupt(void)
{
    //***User Area Begin->code: INT2 - External Interrupt 2***
    static uint16_t count = 0;
    
    if( ++count >= 3 )
    {
        mtrspd_count[1] =  TMR5 / 3;
        // �^�C�}�[���Z�b�g�A�ĊJ
        TMR5 = 0x0000;
        IEC1bits.T5IE = true;
        T5CONbits.TON = true;
        ;
        count = 0;
    }
    
    //***User Area End->code: INT2 - External Interrupt 2***
    EX_INT2_InterruptFlagClear();
}

// TMR4�I�[�o�[�t���[���̏���
// TMR4 : ���[�^�[�P���x�p���X�v���p
void  TMR4_CallBack(void)
{
    // ���[�^�[���x�J�E���g���ő�l�ɂ��ă^�C�}�[��~
    T4CONbits.TON = false;
    IEC1bits.T4IE = false;
    TMR4 = 0xffff;
    mtrspd_count[0] =  0xffff;
}

// TMR5�I�[�o�[�t���[���̏���
// TMR5 : ���[�^�[�Q���x�p���X�v���p
void  TMR5_CallBack(void)
{
    // ���[�^�[���x�J�E���g���ő�l�ɂ��ă^�C�}�[��~
    T5CONbits.TON = false;
    IEC1bits.T5IE = false;
    TMR5 = 0xffff;
    mtrspd_count[1] =  0xffff;
}


#if false
// �ȉ��AInputCapture�ɂ��p���X���v��
// �v���|����̐M������M�@�ɂ�PWM�ϊ���PIC���͂Ɏg�p
// ���݂�S.BUS�iUART1)���g�p���AInputCapture�̓��[�^�[��]���v���Ɏg�p���Ă��邽�߃R�����g�A�E�g
//

// �p���X���v�� - �����オ�菈��
inline void meas_pw_rise_proc( int ch, uint16_t timer )
{
    rise_buff[ch] = timer;
}

// �p���X���v�� - ���������菈��
inline void meas_pw_fall_proc( int ch, uint16_t timer )
{
    if( rise_buff[ch] != 0 || pluse_width[ch] != 0 )
    {
        //�p���X���v�����s
        if(  timer > rise_buff[ch] )
            pluse_width[ch] =  timer - rise_buff[ch];
    }
    
    // watch dog ���Z�b�g
    wd_count[ch] = 0;

    
}




// Input Capture�ɂ�銄�荞��
void __attribute__ ( ( interrupt, no_auto_psv ) ) _ISR _IC1Interrupt( void )
{
    uint16_t buff;
    const int index = 0;
    
    // IC1����
    if(IFS0bits.IC1IF)
    {
        IFS0bits.IC1IF = 0;
        
        while( IC1CON1bits.ICBNE )
            buff = IC1BUF;
        
        if( _RB9)   // ���̓s���ς����ꍇ�͂������ύX
            meas_pw_rise_proc( index, buff );
        else
            meas_pw_fall_proc(index, buff );
        

    }
}


// Input Capture�ɂ�銄�荞��
void __attribute__ ( ( interrupt, no_auto_psv ) ) _ISR _IC2Interrupt( void )
{
    uint16_t buff;
     const int index = 1;
   
    // IC1����
    if(IFS0bits.IC2IF)
    {
        IFS0bits.IC2IF = 0;
        
        while( IC2CON1bits.ICBNE )
            buff = IC2BUF;
        
        if( _RB8)   // ���̓s���ς����ꍇ�͂������ύX
            meas_pw_rise_proc( index, buff );
        else
            meas_pw_fall_proc( index, buff );
        

    }
}

// Input Capture�ɂ�銄�荞��
void __attribute__ ( ( interrupt, no_auto_psv ) ) _ISR _IC5Interrupt( void )
{
    uint16_t buff;
    const int index = 2;
    
    // IC1����
    if(IFS2bits.IC5IF)
    {
        IFS2bits.IC5IF = 0;
        
        while( IC5CON1bits.ICBNE )
            buff = IC5BUF;
        
        if( _RB7)   // ���̓s���ς����ꍇ�͂������ύX
            meas_pw_rise_proc( index, buff );
        else
            meas_pw_fall_proc( index, buff );
        

    }
}


void __attribute__ ( ( interrupt, no_auto_psv ) ) _ISR _IC3Interrupt( void )
{
     uint16_t buff;
    const int index = 3;
    
    // IC3����
    if(IFS2bits.IC3IF)
    {
        IFS2bits.IC3IF = 0;
        
        while( IC3CON1bits.ICBNE )
            buff = IC3BUF;
        
        if( _RB11)   // ���̓s���ς����ꍇ�͂������ύX
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
    
    // IC1����
    if(IFS2bits.IC4IF)
    {
        IFS2bits.IC4IF = 0;
        
        while( IC4CON1bits.ICBNE )
            buff = IC4BUF;
        
        if( _RB7)   // ���̓s���ς����ꍇ�͂������ύX
            meas_pw_rise_proc( index, buff );
        else
            meas_pw_fall_proc( index, buff );
        

    }

}
/**/
#endif

// end of text

