/*
************************************************************************************************
* Filename   	: Chip.c
* Programmer : JiangJun
* Description	: Process clock/pins of STM8L151C8 
* Date           : 2014-08-26
************************************************************************************************
*/



/*
*********************************************************************************************************
*                                                                          INCLUDE FILES
*********************************************************************************************************
*/
#include <string.h>
#include "Dbg.h"
#include "stm8l15x_clk.h"
#include "stm8l15x_flash.h"
#include "Chip.h"


/*
*********************************************************************************************************
*                                                                        COMPILE SWITCH
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*                                                                      	MACRO DEFINITION
*********************************************************************************************************
*/
/* Start address of device ID in FLASH */
#define DEV_ID_START_ADDR    0x4926	

#define LED_IOPORT    GPIOD
#define LED_PIN    GPIO_Pin_4

#define LED2_IOPORT    GPIOD
#define LED2_PIN    GPIO_Pin_5

#define LED3_IOPORT    GPIOD
#define LED3_PIN    GPIO_Pin_6

#define LED4_IOPORT    GPIOD
#define LED4_PIN    GPIO_Pin_7

#define iWL880A    1 /* 20 pins */
#define iWL881A    2 /* 8 pins */
#define CUR_PRODUCT    iWL881A


/*
*********************************************************************************************************
*                                            				     DEFINITIONS / TYPEDEFS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                                                   GLOBAL VARIABLE & STRUCTURE
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                                                   LOCAL VARIABLE & STRUCTURE
*********************************************************************************************************
*/
/* Device unique ID */
static DEV_ID    s_stDevID;


/*
*********************************************************************************************************
*                                                                        FUNCTION PROTOTYPE
*********************************************************************************************************
*/


/*---------------------------------------------------------------------------------------------*/
DEV_ID chip_GetDevID(void)
{
    return s_stDevID;
}


/*---------------------------------------------------------------------------------------------*/
void chip_LEDOff(void)
{
#if (iWL880A == CUR_PRODUCT)
    GPIO_ResetBits(LED_IOPORT, LED_PIN);
#else
    GPIO_SetBits(LED_IOPORT, LED_PIN);
#endif

    return;
}

/*---------------------------------------------------------------------------------------------*/
void chip_LEDOn(void)
{
#if (iWL880A == CUR_PRODUCT)
    GPIO_SetBits(LED_IOPORT, LED_PIN);
#else
    GPIO_ResetBits(LED_IOPORT, LED_PIN);
#endif

    return;
}

/*---------------------------------------------------------------------------------------------*/
void chip_LEDToggle(void)
{
    GPIO_ToggleBits(LED_IOPORT, LED_PIN);

    return;
}

/*---------------------------------------------------------------------------------------------*/
void chip_LED2Off(void)
{
#if (iWL880A == CUR_PRODUCT)
    GPIO_ResetBits(LED2_IOPORT, LED2_PIN);
#else
    GPIO_SetBits(LED2_IOPORT, LED2_PIN);
#endif

    return;
}

/*---------------------------------------------------------------------------------------------*/
void chip_LED2On(void)
{
#if (iWL880A == CUR_PRODUCT)
    GPIO_SetBits(LED2_IOPORT, LED2_PIN);
#else
    GPIO_ResetBits(LED2_IOPORT, LED2_PIN);
#endif

    return;
}

/*---------------------------------------------------------------------------------------------*/
void chip_LED2Toggle(void)
{
    GPIO_ToggleBits(LED2_IOPORT, LED2_PIN);

    return;
}


/*-------------------------------------------------------------------------*/
void chip_TwinkleLed(void)
{
    int8_t    chCnt;

    GPIO_ResetBits(LED2_IOPORT, LED2_PIN);
    GPIO_ResetBits(LED3_IOPORT, LED3_PIN);
    GPIO_ResetBits(LED4_IOPORT, LED4_PIN);
    
    for(chCnt = 0; chCnt < 6; ++chCnt)
    {
        GPIO_ToggleBits(LED2_IOPORT, LED2_PIN);
        util_DelayMs(100);

        GPIO_ToggleBits(LED3_IOPORT, LED3_PIN);
        util_DelayMs(100);

        GPIO_ToggleBits(LED4_IOPORT, LED4_PIN);
        util_DelayMs(100);
    }
}


/*
*********************************************************************************************************
*                                                                             Initialize Chip
* Description: Initialize Chip of STM8L151C8
* Arguments : None.
* Returns     : None
* Note(s)     : 
*********************************************************************************************************
*/
void chip_Init(void)
{
    INT16S    nCnt;
    INT8U    *p_byData;

    /* Set high speed internal clock prescaler=1(16MHz) */
    CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);

    /* Enable LSI(Internal Low Speed oscillator) clock for IWDG */
    CLK_LSICmd(ENABLE);

    /* Wait for LSI clock to be ready */
    nCnt = (INT16S)0x7FFF;	
    while ((RESET == CLK_GetFlagStatus(CLK_FLAG_LSIRDY)) && (nCnt > 0))
    {
        --nCnt;
    }
    		
    /* Configure pins for LED: PD7(pin_36)<->LED1, PC7(pin_46)<->LED2 */
#if (iWL880A == CUR_PRODUCT)	
    GPIO_Init(LED_IOPORT, LED_PIN, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(LED2_IOPORT, LED2_PIN, GPIO_Mode_Out_PP_Low_Slow);
#else
    GPIO_Init(LED_IOPORT, LED_PIN, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(LED2_IOPORT, LED2_PIN, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(LED3_IOPORT, LED3_PIN, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(LED4_IOPORT, LED4_PIN, GPIO_Mode_Out_PP_Low_Slow);
#endif

    /* Get device unique ID */
    p_byData = (INT8U *)&s_stDevID;	
    for (nCnt = 0; nCnt < sizeof(DEV_ID); ++nCnt)
    {
        /* unique ID: X co-ordinate=2B, Y co-ordinate=2B, water=1B, lot=7B */
        *p_byData++ = FLASH_ReadByte(DEV_ID_START_ADDR + nCnt);
    }

    return;
}



/*--------------------------------------------------------------------------------------------------------
                   									     0ooo
                   								ooo0     (   )
                								(   )     ) /
                								 \ (     (_/
                								  \_)
----------------------------------------------------------------------------------------------------------*/


