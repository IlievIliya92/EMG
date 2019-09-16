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

/******************************** TYPEDEFS ************************************/
typedef eSystemState (*pfEventHandler)(void);

typedef enum {
    Idle_State,
    last_State
}eSystemState;

typedef enum {
    last_Event
} eSystemEvent;

typedef struct
{
    eSystemState eStateMachine;
    eSystemEvent eStateMachineEvent;
    pfEventHandler pfStateMachineEvnentHandler;
} sStateMachine;

/******************************** GLOBALDATA *******************************/
extern xComPortHandle xSerialPort;

/********************************* LOCAL DATA *********************************/
static eSystemState eNextState;
static eSystemEvent eNewEvent;

/******************************* INTERFACE DATA *******************************/

/************************ LOCAL FUNCTIONS PROTOTYPES***************************/

/******************************* LOCAL FUNCTIONS ******************************/
sStateMachine asStateMachine [] = {
    {}
};

static eSystemEvent fsm_readEvent(xADCArray *adcValues)
{
    return Idle_State;
}

static void fsm_Init(void)
{
    eNextState = Idle_State;

    return;
}

static void fsm_Task(void *pvParameters)
{
    (void) pvParameters;
    xADCArray adcValues;
    extern QueueHandle_t xAdcQueue;
    const TickType_t xBlockTime = pdMS_TO_TICKS(200);

    while(1)
    {
        xQueueReceive(xAdcQueue, &adcValues, xBlockTime);
        avrSerialPrintf("\r\n%u, %u\r\n", adcValues.adc0, adcValues.adc1);

        eNewEvent = fsm_readEvent(&adcValues);
        if((eNextState < last_State) && (eNewEvent < last_Event)&& (asStateMachine[eNextState].eStateMachineEvent == eNewEvent) && (asStateMachine[eNextState].pfStateMachineEvnentHandler != NULL))
        {
            eNextState = (*asStateMachine[eNextState].pfStateMachineEvnentHandler)();
        }
        else
        {
            //Invalid
        }
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

genericTask_t *getLedTask(void)
{
    return &led;
}
