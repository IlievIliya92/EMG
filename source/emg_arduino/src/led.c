/******************************** INCLUDE FILES *******************************/
#include <avr/io.h>
#include <util/delay_basic.h>

#include "freeRTOS/FreeRTOS.h"
#include "freeRTOS/semphr.h"
#include "freeRTOS/queue.h"

/* serial interface include file. */
#include "freeRTOS/lib_io/serial.h"
#include "freeRTOS/lib_io/digitalAnalog.h"

#include "led.h"

/******************************** LOCAL DEFINES *******************************/


/******************************** GLOBALDATA *******************************/

/********************************* LOCAL DATA *********************************/

/******************************* INTERFACE DATA *******************************/


/************************ LOCAL FUNCTIONS PROTOTYPES***************************/

/******************************* LOCAL FUNCTIONS ******************************/
static void led_Init(void)
{
    /* Set as output */
    DDRB |= _BV(DDB5);

    return;
}

static void led_Task(void *pvParameters)
{
    (void) pvParameters;
    TickType_t xLastWakeTime;

    extern QueueHandle_t xAdcQueue;
    uint32_t adcDataMsg = 0;

    const TickType_t xBlockTime = pdMS_TO_TICKS(200);

    xLastWakeTime = xTaskGetTickCount();

    while(1)
    {
        xQueueReceive( xAdcQueue, &adcDataMsg, xBlockTime);
        PORTB |=  _BV(PORTB5);

        vTaskDelayUntil( &xLastWakeTime, (100 / portTICK_PERIOD_MS));
        PORTB &= ~_BV(PORTB5);
        vTaskDelayUntil( &xLastWakeTime, (400 / portTICK_PERIOD_MS));
    }

    return;
}

/***************************** INTERFACE FUNCTIONS ****************************/
genericTask_t led = {
    led_Init,
    led_Task,
    "LED TASK",
    256,
    3,
    NULL,
    NULL
};
