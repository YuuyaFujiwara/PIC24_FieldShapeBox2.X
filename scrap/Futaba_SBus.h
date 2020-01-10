/* 
 * File:   Futaba_SBus.h
 * Author: �����T��
 *
 * Created on July 8, 2016, 1:04 PM
 */

#ifndef FUTABA_SBUS_H
#define	FUTABA_SBUS_H

#ifdef	__cplusplus
extern "C" {
#endif
    
typedef enum {
    SBUS_CH1 = 0,
    SBUS_CH2 = 1,
    SBUS_CH3 = 2,
    SBUS_CH4 = 3,
    SBUS_CH5 = 4,
    SBUS_CH6 = 5,
    SBUS_CH7 = 6,
    SBUS_CH8 = 7,
    SBUS_CH9 = 8,
    SBUS_CH10 = 9,
    SBUS_CH11 = 10,
    SBUS_CH12 = 11,
    SBUS_CH13 = 12,
    SBUS_CH14 = 13,
    SBUS_CH15 = 14,
    SBUS_CH16 = 15,
    SBUS_CH_NUM = 16
} SBUS_CHANNELS;    
    
    
    
/*****************************************************************************
 * Public values
 ****************************************************************************/
// �eCH�l�o�b�t�@(���J�p)�FCH1?CH16
extern volatile  uint16_t sbus_ch_buff[];
// �t���O���J�p
extern volatile  uint8_t  sbus_flag_buff;

////extern volatile int tmpflag;
////extern volatile int tmperrflag;


/*
 * ��M�f�[�^����
 */
extern void sbus_ch_data_proc( uint8_t data );

/*
 1ms tick����
 */
extern void FutabaSBus_1ms_tick_proc(void);

#ifdef	__cplusplus
}
#endif

#endif	/* FUTABA_SBUS_H */

