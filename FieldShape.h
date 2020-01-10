/* 
 * File:   FieldShape.h
 * Author: YuyaFujiwara
 *
 * Created on January 8, 2020, 5:40 PM
 */

#ifndef FIELDSHAPE_H
#define	FIELDSHAPE_H

#ifdef	__cplusplus
extern "C" {
#endif

    // �ޏ�`��ݒ���
    typedef enum {
        UNKNOWN = 0,
        NOT_READY,
        READY,
        RUNNING,
        FINISH
    } FIELDSHAPE_STATUS_t;
    
    
    // �ޏ�`��ݒ胁�C��
    extern void FieldShape_Proc(void);
    
    // �����J�n
    extern inline void FieldShape_start(void);

    // 1ms���Ăяo��
    extern inline void FieldShape_1ms_Proc(void);



#ifdef	__cplusplus
}
#endif

#endif	/* FIELDSHAPE_H */

