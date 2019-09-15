/******************************** INCLUDE FILES *******************************/
#include <util/delay_basic.h>

#include "freeRTOS/FreeRTOS.h"
#include "freeRTOS/semphr.h"

/* serial interface include file. */
#include "freeRTOS/lib_io/serial.h"
#include "freeRTOS/lib_io/digitalAnalog.h"

#include "adc.h"
/******************************** LOCAL DEFINES *******************************/



/******************************** GLOBALDATA *******************************/
extern xComPortHandle xSerialPort;

/********************************* LOCAL DATA *********************************/
static xADCArray adcValues;
static SemaphoreHandle_t xADCSemaphore;

/******************************* INTERFACE DATA *******************************/


/******************************* LOCAL FUNCTIONS ******************************/
static void adc_Init(void)
{
    /* Initialize analog inputs */
    enbAnalogInput(analogIn0);
    enbAnalogInput(analogIn1);

    vSemaphoreCreateBinary(xADCSemaphore);

    return;
}

static void adc_readSensors(void)
{

    if( xADCSemaphore != NULL )
    {
        if( xSemaphoreTake(xADCSemaphore, (TickType_t)10) == pdTRUE)
        {
            setAnalogMode(MODE_10_BIT);    // 10-bit analogue-to-digital conversions

            adcValues.adc0 = 0;
            adcValues.adc1 = 0;

            startAnalogConversion(analogIn0, EXTERNAL_REF);   // start next conversion
            while(analogIsConverting())
                _delay_loop_2(25);

            adcValues.adc0 = analogConversionResult();

            startAnalogConversion(analogIn1, EXTERNAL_REF);
            while(analogIsConverting())
                 _delay_loop_2(25);

            adcValues.adc1 = analogConversionResult();

            avrSerialPrintf("\r\n%u, %u\r\n", adcValues.adc0, adcValues.adc1);
            xSemaphoreGive(xADCSemaphore);
        }
    }

    return;
}

static void adc_Task(void *pvParameters)
{
    (void) pvParameters;
    TickType_t xLastWakeTime;

    xLastWakeTime = xTaskGetTickCount();

    while(1)
    {
        adc_readSensors();
        vTaskDelayUntil( &xLastWakeTime, (30 / portTICK_PERIOD_MS));
    }

    return;
}

/***************************** INTERFACE FUNCTIONS ****************************/
genericTask_t adc = {
    adc_Init,
    adc_Task,
    "ADC TASK",
    256,
    1
};

genericTask_t *getAdcTask(void)
{
    return &adc;
}
