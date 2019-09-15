/******************************** INCLUDE FILES *******************************/
#include "freeRTOS/FreeRTOS.h"
#include "freeRTOS/task.h"
#include "freeRTOS/queue.h"
#include "freeRTOS/semphr.h"

#include "rtos.h"
/******************************** LOCAL DEFINES *******************************/

/******************************** GLOBALDATA *******************************/

/********************************* LOCAL DATA *********************************/

/******************************* INTERFACE DATA *******************************/

/******************************* LOCAL FUNCTIONS ******************************/

/***************************** INTERFACE FUNCTIONS ****************************/
void rtos_start(void)
{
    vTaskStartScheduler();

    return;
}

void rtos_createTask(genericTask_t *task)
{
    xTaskCreate(task->runTask, (const portCHAR *)task->name,
                task->stackDepth, NULL, task->priority, NULL );

    return;
}
