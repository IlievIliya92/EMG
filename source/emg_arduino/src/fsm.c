/******************************** INCLUDE FILES *******************************/
#include <avr/io.h>
#include <util/delay_basic.h>

#include "freeRTOS/FreeRTOS.h"
#include "freeRTOS/semphr.h"
#include "freeRTOS/queue.h"

/* serial interface include file. */
#include "freeRTOS/lib_io/serial.h"
#include "freeRTOS/lib_io/digitalAnalog.h"
#include "freeRTOS/lib_io/servoPWM.h"

#include "fsm.h"

/******************************** LOCAL DEFINES *******************************/
#define SERVO_K 2.841666667

#define MIN_PULSE_WIDTH       300     // the shortest pulse sent to a servo
#define MAX_PULSE_WIDTH      1180     // the longest pulse sent to a servo

#define INTERVAL_PULSE_WIDTH (MAX_PULSE_WIDTH - MIN_PULSE_WIDTH)
#define DEFAULT_PULSE_WIDTH  300     // default pulse width when servo is attached

/******************************** TYPEDEFS ************************************/
typedef enum {
    Idle_State,
    last_State
}eSystemState;

typedef enum {
    last_Event
} eSystemEvent;

typedef eSystemState (*pfEventHandler)(void);

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

    start_PWM_hardware();

    return;
}

static void fsm_Task(void *pvParameters)
{
    (void) pvParameters;

    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    xADCArray adcValues;
    extern QueueHandle_t xAdcQueue;
    const TickType_t xBlockTime = pdMS_TO_TICKS(200);

    uint16_t servoIn = DEFAULT_PULSE_WIDTH;
    uint16_t offset = MIN_PULSE_WIDTH;

    while(1)
    {
        PORTB &= ~_BV(PORTB5);
        xQueueReceive(xAdcQueue, &adcValues, xBlockTime);
        //avrSerialPrintf("\r\n%u, %u\r\n", adcValues.adc0, adcValues.adc1);
        servoIn = offset + adcValues.adc0;
        set_PWM_hardware(servoIn, servoIn);
        vTaskDelayUntil( &xLastWakeTime, (10 / portTICK_PERIOD_MS));

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
genericTask_t fsm = {
    fsm_Init,
    fsm_Task,
    "FSM TASK",
    256,
    2,
    NULL,
    NULL
};

genericTask_t *getFsmTask(void)
{
    return &fsm;
}
