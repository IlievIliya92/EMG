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


/******************************** GLOBALDATA *******************************/
extern xComPortHandle xSerialPort;

/********************************* LOCAL DATA *********************************/
static SemaphoreHandle_t xCmdSemaphore;

/******************************* INTERFACE DATA *******************************/

/************************ LOCAL FUNCTIONS PROTOTYPES***************************/

/******************************* LOCAL FUNCTIONS ******************************/

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
