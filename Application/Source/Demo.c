/**
 * \file
 *         Demo.c
 * \description
 *         Print message per 1 second; deal with KEY and SHT10
 * \author
 *         Jiang Jun <jiangjunjie_2005@126.com>
 * \date
 *         2016-07-13 22:17
 * \copyright
 *         (c) RimeLink (www.rimelink.com) All Rights Reserved.
 */


/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "etimer.h"
#include "process.h"
#include "Chip.h"
#include "Dbg.h"
#include "DebugPort.h"
#include "SHT1x.h"


/* Private typedef -----------------------------------------------------------*/
#define PRINT_SHT    RIME_DBG_ON


/* Private macro -------------------------------------------------------------*/
#define DEMO_EVENT_SAMPLE_SHT    (PROCESS_EVENT_MAX + 1)


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/* Private Constants ---------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
static void SamplePrintSHT(void)
{
    float    dew_point;
    unsigned char    error,checksum;
    sht_value    humi_val,temp_val;

    error = 0;
    error += s_measure((unsigned char*) &humi_val.i, &checksum, HUMI);  //measure humidity
    error += s_measure((unsigned char*) &temp_val.i, &checksum, TEMP);  //measure temperature

    if (0 != error)
    {
        s_connectionreset(); //in case of an error: connection reset
        RIME_DBG(PRINT_SHT | RIME_DBG_LEVEL_WARNING, "SHT10: error=%d.\r\n", error);
        return;
    }

    /* Convert temperature and humidity to human friendly. */
    humi_val.f = (float)humi_val.i; //converts integer to float
    temp_val.f = (float)temp_val.i; //converts integer to float
    calc_sth11(&humi_val.f, &temp_val.f); //calculate humidity, temperature
    dew_point = calc_dewpoint(humi_val.f,temp_val.f); //calculate dew point

    /* Print temperature and humidity to SINK. */
    RIME_DBG( PRINT_SHT, 
                      "SHT10: temp=%f, humi=%f%, dew point=%f.\r\n",
                      temp_val.f, humi_val.f, dew_point );

    return;
}

/*---------------------------------------------------------------------------*/
PROCESS(demo_process, "demo process");

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(demo_process, ev, data)
{
    static struct etimer    timer; 

    PROCESS_BEGIN();

    etimer_set(&timer, CLOCK_CONF_SECOND * 5); /* 5s */ 

    while (1)
    {
        PROCESS_YIELD();

        if (PROCESS_EVENT_TIMER == ev)
        {
            chip_LEDToggle();
            dbg_Tx("Hello, Contiki.\r\n", 17);
            etimer_restart(&timer); /* For next! */
        }
        else if (PROCESS_EVENT_POLL == ev)
        {
            chip_TwinkleLed();
        }
        else if (DEMO_EVENT_SAMPLE_SHT == ev)
        {
            SamplePrintSHT();
        }
        else
        {
            ASSERT(!"demo_process(): Bad event!.\r\n"); 
        }
    }

    PROCESS_END();
}

/*---------------------------------------------------------------------------------------------*/
void demo_Init(void)
{
    process_start(&demo_process, NULL);

    return;
}

/*---------------------------------------------------------------------------------------------*/
void demo_SampleSHT(void)
{
    process_post(&demo_process, DEMO_EVENT_SAMPLE_SHT, NULL);

    return;
}

/*---------------------------------------------------------------------------------------------*/
void demo_ProcKey(void)
{
    process_poll(&demo_process);

    return;
}


/*--------------------------------------------------------------------------
                                                                       0ooo
                                                           ooo0     (   )
                                                            (   )      ) /
                                                             \ (      (_/
                                                              \_)
----------------------------------------------------------------------------*/

