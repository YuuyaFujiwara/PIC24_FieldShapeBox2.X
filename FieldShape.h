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

    // 圃場形状設定状態
    typedef enum {
        UNKNOWN = 0,
        NOT_READY,
        READY,
        RUNNING,
        FINISH
    } FIELDSHAPE_STATUS_t;
    
    
    // 圃場形状設定メイン
    extern void FieldShape_Proc(void);
    
    // 処理開始
    extern inline void FieldShape_start(void);

    // 1ms毎呼び出し
    extern inline void FieldShape_1ms_Proc(void);



#ifdef	__cplusplus
}
#endif

#endif	/* FIELDSHAPE_H */

