/**
 * \file
 *         Monitor.c
 * \description
 *         Task for monitoring system
 * \author
 *         Jiang Jun <jiangjunjie_2005@126.com>
 * \date
 *         2016-07-13 22:10
 * \copyright
 *         (c) RimeLink (www.rimelink.com) All Rights Reserved.
 */


/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "process.h"
#include "Dbg.h"
#include "DebugPort.h"


/* Private typedef -----------------------------------------------------------*/
/**
* @brief  Process command received from PC
*/
typedef void (* PFN_CMD)(void);
typedef struct _pc_cmd
{
    char    *p_chCmdStr;
    PFN_CMD    pfnProcCmd;
} PC_CMD;


/* Private macro -------------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
static void CmdHelp(void);
static void CmdVer(void);
static void CmdEnablePrint(void);
static void CmdDisablePrint(void);
static void CmdSampleSHT(void);


/* Private variables ---------------------------------------------------------*/
/**
* @brief  Command RX from PC
*/
#define SIZE_BUF_PC_CMD    48
static INT8U    s_abyPCCmdBuf[SIZE_BUF_PC_CMD];

/**
* @brief  Supported command
*/
const static PC_CMD    s_astPCCmd[] =
{
    {"?", CmdHelp},
    {"version", CmdVer},
    {"open print", CmdEnablePrint},
    {"close print", CmdDisablePrint},
    {"sample sht", CmdSampleSHT},
};

#define SEND_OK_2_PC()    dp_Tx("\r\nOK\r\n", 6)


/* Private Constants ---------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------*/
static void CmdHelp(void)
{
    INT8S    chCnt;

    /* Print all string of command */
    for (chCnt = 0; chCnt < SIZE_OF_ARRAY(s_astPCCmd); ++chCnt)
    {
        dp_Tx(s_astPCCmd[chCnt].p_chCmdStr, strlen(s_astPCCmd[chCnt].p_chCmdStr));
        dp_Tx("\r\n", 2);
    }

    return;
}

/*---------------------------------------------------------------------------------------------*/
static void CmdVer(void)
{
    dp_Tx(CUR_VER, strlen(CUR_VER));
    dp_Tx("\r\n", 2);

    return;
}

/*---------------------------------------------------------------------------------------------*/
static void CmdEnablePrint(void)
{
    SEND_OK_2_PC();
    dbg_EnablePrint();

    return;
}

/*---------------------------------------------------------------------------------------------*/
static void CmdDisablePrint(void)
{
    SEND_OK_2_PC();
    dbg_DisablePrint();

    return;
}

/*---------------------------------------------------------------------------------------------*/
static void CmdSampleSHT(void)
{
    SEND_OK_2_PC();
    demo_SampleSHT();

    return;
}

/*---------------------------------------------------------------------------------------------*/
static void ProcPCCmd(void)
{
    #define UNKNOWN_PROMPT    ("Unknown command, ? for help.\r\n")

    INT8S    chCnt;

    /* Match the command string one by one */
    for (chCnt = 0; chCnt < SIZE_OF_ARRAY(s_astPCCmd); ++chCnt)
    {
        if (0 == strcmp(s_astPCCmd[chCnt].p_chCmdStr, (char *)s_abyPCCmdBuf))    /* Match a command */
        {
            (* s_astPCCmd[chCnt].pfnProcCmd)();    /* Invork corresponding procedure */
            return;    /* Finished job */			
        }
    }
    dp_Tx(UNKNOWN_PROMPT, strlen(UNKNOWN_PROMPT));	

    return;
}


/*---------------------------------------------------------------------------*/
PROCESS(monitor_process, "Monitor process");

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(monitor_process, ev, data)
{
    PROCESS_BEGIN();

    while (1)
    {
        PROCESS_YIELD(); /* Yield process until received a poll or an event */

        if (PROCESS_EVENT_POLL == ev)
        {
            /* Process this received command */
            dp_GetCmd(s_abyPCCmdBuf, SIZE_BUF_PC_CMD);
            ProcPCCmd();
        }
        else
        {
            ASSERT(!"monitor_process(): Bad event!.\r\n");
        }
    }

    PROCESS_END();
}

/*---------------------------------------------------------------------------------------------*/
void monitor_Init(void)
{
    process_start(&monitor_process, NULL);

    return;
}

/*---------------------------------------------------------------------------------------------*/
void monitor_ProcPCCmd(void)
{
    /* Inform monitor_process to deal with this received command */
    process_poll(&monitor_process);	

    return;
}


/*--------------------------------------------------------------------------------------------------------
                   									     0ooo
                   								ooo0     (   )
                								(   )     ) /
                								 \ (     (_/
                								  \_)
----------------------------------------------------------------------------------------------------------*/

