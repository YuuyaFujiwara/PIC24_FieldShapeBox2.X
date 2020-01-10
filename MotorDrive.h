/* 
 * File:   Propo_Rcv_Servo.h
 * Author: �����T��
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
 * �v���|�M������M���T�[�{���쓮���郋�[�`��
   1ms���ɌĂяo��
 */
extern void MotorControl_Proc(void);

/*****************************************************************************
 * Public values
 ****************************************************************************/

// ��ʂ���R�}���h�ő���ꂽ�p���X���l
extern volatile uint16_t auto_cmnd_pw_buff[];
 


// �v���|��M�p���X��
//extern uint16_t pluse_width[];

    
extern int servo_control_mode;      // �T�[�{���䃂�[�h(�v���| CH5�ɂ��R���g���[������)
                                    //   0 : CH5=���@�i�p���X�����j 
                                    //   1 : CH5=���@�i�p���X�����j 
                                    //   2 : CH5=��O�i�p���X����j 
     
// ���[�^�[���x�J�E���g�l
extern uint16_t mtrspd_count[];
    
/*
 * ������
 */
extern void MotorDrive_Init(void);
    
/*
 100ms tick����
 */
extern void MotorDrive_100ms_tick_proc(void);

/*
 ���[�^�[���x�̕��ϒl���Z�o����
 * index : ���[�^�[Index
 * current : ���ݑ��x
 * return : ���ϒl
 * 
 */
extern uint16_t MotorSpeed_Average( int index, uint16_t current );   
    


#ifdef	__cplusplus
}
#endif

#endif	/* PROPO_RCV_SERVO_H */

