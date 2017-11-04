/**
 * \file
 *         Key.c
 * \description
 *         Deal with key input
 * \author
 *         Jiang Jun <jiangjunjie_2005@126.com>
 * \date
 *         2016-07-19 22:22
 * \copyright
 *         (c) RimeLink (www.rimelink.com)  All Rights Reserved.
 */

 
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <string.h>
#include "stm8l15x_gpio.h"
#include "Key.h"

/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
#define KEY_PORT    GPIOA
#define KEY_PIN    GPIO_Pin_2


/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private Constants ---------------------------------------------------------*/
/**
  * @brief  Initialize port for Debug key and Reset key.
  * @note  None.
  * @param  None.
  * @retval  None.
  */
void key_Init(void)
{
    GPIO_Init(KEY_PORT, KEY_PIN, GPIO_Mode_In_FL_IT);

    EXTI_SetPinSensitivity(EXTI_Pin_2, EXTI_Trigger_Rising);

    return;
}

/*---------------------------------------------------------------------------------------------*/
void key_IRQHandler(void)
{
    EXTI_ClearITPendingBit(EXTI_IT_Pin2);

    demo_ProcKey();

    return;
}


/*--------------------------------------------------------------------------------------------------------
                   									  0ooo
                   								ooo0     (   )
                								(   )     ) /
                								 \ (     (_/
                								  \_)
----------------------------------------------------------------------------------------------------------*/



