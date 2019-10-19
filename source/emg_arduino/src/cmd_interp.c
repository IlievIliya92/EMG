/******************************** INCLUDE FILES *******************************/
#include <util/delay_basic.h>

#include "freeRTOS/FreeRTOS.h"
#include "freeRTOS/semphr.h"
#include "freeRTOS/queue.h"

/* serial interface include file. */
#include "freeRTOS/lib_io/serial.h"
#include "freeRTOS/lib_io/digitalAnalog.h"

#include "adc.h"
/******************************** LOCAL DEFINES *******************************/
#define CMD_SIZE 10

/******************************** GLOBALDATA *******************************/
extern xComPortHandle xSerialPort;

/********************************* LOCAL DATA *********************************/
static SemaphoreHandle_t xCmdSemaphore;

/******************************* INTERFACE DATA *******************************/

/************************ LOCAL FUNCTIONS PROTOTYPES***************************/

/******************************* LOCAL FUNCTIONS ******************************/
static void cmd_interpGetCmd(uint8_t *buff, uint8_t len)
{
    uint8_t c;
    uint8_t i = 0;

    for (;;) {
        while (!xSerialGetChar(&xSerialPort, &c))
            vTaskDelay(1);

        if (c == '\r') break;
        if ((c == '\b') && i) {
            --i;
            xSerialPutChar( &xSerialPort, c );
            continue;
        }
        if (c >= ' ' && i < len - 1) {  /* Visible chars */
            buff[i++] = c;
            xSerialPutChar( &xSerialPort, c );
        }
    }
    buff[i] = 0;
    xSerialPrint((uint8_t *)"\r\n");
}

static void cmd_interp_Init(void)
{
    vSemaphoreCreateBinary(xCmdSemaphore);

    return;
}

static void cmd_interp(void)
{
    if( xCmdSemaphore != NULL )
    {
        if( xSemaphoreTake(xCmdSemaphore, (TickType_t)10) == pdTRUE)
        {
            uint8_t cdm_buff[CMD_SIZE];
            cmd_interpGetCmd(cdm_buff, CMD_SIZE);
            xSerialPrint(cdm_buff);

            xSemaphoreGive(xCmdSemaphore);
        }
    }

    return;
}

static void cmd_interp_Task(void *pvParameters)
{
    (void) pvParameters;
    TickType_t xLastWakeTime;

    xLastWakeTime = xTaskGetTickCount();

    while(1)
    {
        cmd_interp();
        vTaskDelayUntil( &xLastWakeTime, (30 / portTICK_PERIOD_MS));
    }

    return;
}

/***************************** INTERFACE FUNCTIONS ****************************/
genericTask_t cmd = {
    cmd_interp_Init,
    cmd_interp_Task,
    "CMD INTERPRET TASK",
    256,
    1,
    NULL,
    NULL
};

genericTask_t *getCmdInterpTask(void)
{
    return &cmd;
}
