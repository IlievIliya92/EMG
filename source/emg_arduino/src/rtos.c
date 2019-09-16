/******************************** INCLUDE FILES *******************************/
#include "freeRTOS/FreeRTOS.h"
#include "freeRTOS/task.h"
#include "freeRTOS/queue.h"
#include "freeRTOS/semphr.h"

#include "freeRTOS/lib_io/serial.h"


#include "rtos.h"
/******************************** LOCAL DEFINES *******************************/

/******************************** GLOBALDATA *******************************/
extern xComPortHandle xSerialPort;

/********************************* LOCAL DATA *********************************/

/******************************* INTERFACE DATA *******************************/

/******************************* LOCAL FUNCTIONS ******************************/
static void rtos_createTask(genericTask_t *task)
{
    xTaskCreate(task->runTask, (const portCHAR *)task->name,
                task->stackDepth, task->args, task->priority, NULL );

    return;
}

/***************************** INTERFACE FUNCTIONS ****************************/
void rtos_start(genericTask_t *task[])
{
    int i = 0;
    int n_tasks = sizeof(*task) / sizeof(genericTask_t*);

    avrSerialPrintf("\r\nNumber of tasks: %d \r\n", n_tasks);

    for (i = 0; i < n_tasks; i++) {
        task[i]->initTask();
        rtos_createTask(task[i]);
    }

    vTaskStartScheduler();

    return;
}
