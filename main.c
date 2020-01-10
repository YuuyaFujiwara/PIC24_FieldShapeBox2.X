/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using MPLAB(c) Code Configurator

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 3.15.0
        Device            :  PIC24FJ64GB002
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC16 1.26
        MPLAB             :  MPLAB X 3.20
*/

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

#include "mcc_generated_files/mcc.h"

#define FCY 4000000UL
#include <libpic30.h>

#include "XBee_Command.h"
#include "mcc_generated_files/mcc.h"
#include "RTCM_Recv.h"
#include "switch.h"
#include "led.h"
#include "FieldShape.h"


#include <stdio.h>
#include <string.h> // for strlen

static bool  flg_data_print = false;

extern void XBeeAPI_Broadcast( const uint8_t *data , const unsigned int data_len );

extern void LED_Test(void);
extern void SW_LED_Test(void);
extern void SW_LED_Test2(void);


/*
                         Main application
 */
int main(void)
{
    // initialize the device
    SYSTEM_Initialize();

//    UART2_WriteBuffer( (uint8_t*)"Hello!\n", 7 );

    
    // 
    LED_Test();

   
    
    // ZigBee親子間の通信が安定するまで待つ
    // 1000msだと短い
    //__delay_ms(10000);


   
        UART2_WriteBuffer( (uint8_t*)"Hello!\n", 7 );
    
        // 処理開始
        FieldShape_start();
        
        //forever
        while(1)
        {
            FieldShape_Proc();
        }

#if false
    // ATNDを発行して、子機アドレスを得る
    for(;;)
    {
        if( XBeeAPI_GetChildrenAddr() )
            break;
        
        __delay_ms(2000);
    }
#endif

#if false
    // 子機にHello送る。
    // ゴミなので後で削除する。
    XBeeAPI_SendToAllChildren( (uint8_t*)"Hello\n", 8 );
#endif
    
    
    //printf("Hello!\n");
//    UART2_WriteBuffer( (uint8_t*)"Hello!\n", 7 );
    
    
//       int count = 0;
//        uint8_t buff[16];
    while (1)
    {
        SW_LED_Test();


        

#if false
        
        // debug用UART2送信
        if( flg_data_print ) 
        {
            flg_data_print = false;
            
            
            // すべての子機に送信する。
          //XBeeAPI_Broadcast( (uint8_t*)"ABC", 3 );
            XBeeAPI_SendToAllChildren( (uint8_t*)"ABCDEFG\n", 8 );

            
        }
#elif false
        // RTCMデータを送信
        unsigned int rtcm_data_num = Recv_RTCM_Data();
        if( rtcm_data_num > 0)
        {
            XBeeAPI_SendToAllChildren( g_rtcm_recv_buff, rtcm_data_num );
        }
#elif false
        
        //if( UART2_TransmitBufferSizeGet() > 30 )
        //if( flg_data_print ) 
        {
            int i;
        //    for( i = 0; i < 50; i++ )
            {
                flg_data_print = false;


                count++;
                sprintf( (char*)buff, "%d\n", count );
                XBeeAPI_SendToAllChildren( buff, strlen((char*)buff) );
                //UART2_WriteBuffer( buff, strlen((char*)buff) );
                //UART2_WriteBuffer( "abcdefg\n", 8 );
            }
            
            
        }
        
        
#endif
        
        
        
    }

    return -1;
}

// LEDを点滅させてみる
#if false
void LED_Test(void)
{
        UART2_WriteBuffer( (uint8_t*)"LED_Test start\n", 15 );
    
    
    LED1_SetLow();
    LED2_SetLow();
    LED3_SetLow();
    __delay_ms(500);

    int i;
    for( i = 0; i < 10; i++ )
    {    
        LED1_SetHigh();
        LED2_SetLow();
        LED3_SetLow();
        __delay_ms(100);

        LED1_SetLow();
        LED2_SetHigh();
        LED3_SetLow();
        __delay_ms(100);

        LED1_SetLow();
        LED2_SetLow();
        LED3_SetHigh();
        __delay_ms(100);
    }
    
    LED1_SetLow();
    LED2_SetLow();
    LED3_SetLow();
    
            
    UART2_WriteBuffer( (uint8_t*)"LED_Test finish\n", 16 );

}
#else
void LED_Test(void)
{
    set_led_status( LED1, LED_STS_OFF );
    set_led_status( LED2, LED_STS_OFF );
    set_led_status( LED3, LED_STS_OFF );
    __delay_ms(500);

    int i;
    for( i = 0; i < 10; i++ )
    {    
        set_led_status( LED1, LED_STS_ON  );
        set_led_status( LED2, LED_STS_OFF );
        set_led_status( LED3, LED_STS_OFF );
        __delay_ms(100);
        
        set_led_status( LED1, LED_STS_OFF );
        set_led_status( LED2, LED_STS_ON  );
        set_led_status( LED3, LED_STS_OFF );
        __delay_ms(100);
        
        set_led_status( LED1, LED_STS_OFF );
        set_led_status( LED2, LED_STS_OFF );
        set_led_status( LED3, LED_STS_ON  );
        __delay_ms(100);
    }
        
    set_led_status( LED1, LED_STS_OFF );
    set_led_status( LED2, LED_STS_OFF );
    set_led_status( LED3, LED_STS_OFF );
}
#endif




// 
void SW_LED_Test(void)
{

    // SW ON = Low = 0
    // SW OFF = High = 1

    if( SW1_GetValue() )
        LED1_SetLow();
    else
        LED1_SetHigh();

    if( SW2_GetValue() )
        LED2_SetLow();
    else
        LED2_SetHigh();

    if( SW3_GetValue() )
        LED3_SetLow();
    else
        LED3_SetHigh();

}


// 
void SW_LED_Test2(void)
{
    int i;
    for( i = 0; i < 3; i++ )
    {
        // 長押しされたらLED状態をすすめる
        if( sw_longpush_sts(i) )      
        {
            led_status_incl(i);
        //    sw_longpush_reset(i);
        }
    }
}





// timer1割り込みより呼び出される
void TMR1_CallBack(void)
{
    // Add your custom callback code here


    
    // 1s interval
    static int interval_counter_1s = 0;
    bool interval_flag_1s = false;
    if (++interval_counter_1s >= 1000)
    {
        interval_counter_1s = 0;
        interval_flag_1s = true;
    }

    // 100ms interval
    static int interval_counter_100ms = 0;
    bool interval_flag_100ms = false;
    if ( ++interval_counter_100ms >= 100 )
    {
        interval_counter_100ms = 0;
        interval_flag_100ms = true;
    }

    // 10ms interval
    static int interval_counter_10ms = 0;
    bool interval_flag_10ms = false;
    if ( ++interval_counter_10ms >= 10 )
    {
        interval_counter_10ms = 0;
        interval_flag_10ms = true;
    }
    
    
    // 2ms interval
    static int interval_counter_2ms = 0;
    bool interval_flag_2ms = false;
    if ( ++interval_counter_2ms >= 2 )
    {
        interval_counter_2ms = 0;
        interval_flag_2ms = true;
    }

    // 各種処理
    
    
   // XBee_Command_1ms_Proc();
   // if( interval_flag_100ms )
   //     MotorDrive_100ms_tick_proc();
    
    
    // 各種要求フラグを立てる
    //（タイミングがシビアでない処理）
    if( interval_flag_10ms )
        flg_data_print = true;


    // 1ms毎の処理呼び出し
    
    FieldShape_1ms_Proc();
    
    switch_1ms_proc();

   led_1ms_proc();
    
    
    
}

/**
 End of File
*/