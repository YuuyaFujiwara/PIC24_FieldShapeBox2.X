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

#include "mcc_generated_files/mcc.h"


#include <stdio.h>
#include <string.h> // for strlen

static bool  flg_data_print = false;



/*
                         Main application
 */
int main(void)
{
    // initialize the device
    SYSTEM_Initialize();




    //printf("Hello!\n");
    UART2_WriteBuffer( (uint8_t*)"Hello!\n", 7 );
    aaa
    
    while (1)
    {
        //---------------------------------
        // コマンド受信チェック
        //
        if( UART2_DelimiterRecieved()  )
        {
//            UART2_WriteBuffer("A",1 );
            // コマンド読み出し
            unsigned char cmndbuff[32];
            unsigned char respbuff[64];
            UART2_ReadLine( cmndbuff, sizeof(cmndbuff));
    //        puts( (char*)cmndbuff );
    //        puts("\n");
    //        
//            UART2_WriteBuffer("B",1 );
    //        // コマンド処理
    //        CommandParser( cmndbuff, respbuff );
//            UART2_WriteBuffer("C",1 );
            // レスポンス
            UART2_WriteBuffer( (uint8_t*)respbuff, strlen((char*)respbuff));

        }  
        
        
//        // debug用UART2送信
//        if( flg_data_print ) 
//        {
//            flg_data_print = false;
//            //uint16_t tmp = IC4_CaptureDataRead();
//            uint16_t tmptmr4 = TMR4;
//
//            
//                //printf("IC4=%u\n", mtrspd_count[0] );
//                //printf("TMR4=%u\n", tmptmr4 );
//                printf("SPD=%u,  TMR4=%u\n", mtrspd_count[0], tmptmr4 );
//                
//             //   TMR4 = 0x1234;
//        }
        
        


    }

    return -1;
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
   // if( interval_flag_100ms )
   //     MotorDrive_100ms_tick_proc();
    
    
    // 各種要求フラグを立てる
    //（タイミングがシビアでない処理）
    if( interval_flag_100ms )
        flg_data_print = true;

    // MotorDrive監視
    
    
    
}

/**
 End of File
*/