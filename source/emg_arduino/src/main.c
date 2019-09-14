#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <avr/io.h>
#include <avr/sleep.h>

#include <util/delay_basic.h>


/* Scheduler include files. */
#include "freeRTOS/FreeRTOS.h"
#include "freeRTOS/task.h"
#include "freeRTOS/queue.h"
#include "freeRTOS/semphr.h"

/* serial interface include file. */
#include "freeRTOS/lib_io/serial.h"
#include "freeRTOS/lib_io/digitalAnalog.h"

extern xComPortHandle xSerialPort;

/*-----------------------------------------------------------*/
static void TaskReadADC(void *pvParameters);
static xADCArray adcValues;

/* Create a Semaphore binary flag for the ADC. To ensure only single access. */
static SemaphoreHandle_t xADCSemaphore;
static void ReadADCSensors(void);


/*-----------------------------------------------------------*/

/* Main program loop */
int main(void) __attribute__((OS_main));
int main(void)
{
    xSerialPort = xSerialPortInitMinimal( USART0, 115200, 10, 10);
//    avrSerialxPrint_P( &xSerialPort, PSTR("\r\n\n\nApllication Started Succesfuly!\r\n"));

    vSemaphoreCreateBinary( xADCSemaphore ); // binary semaphore for ADC

    /* Initialize analog inputs */
    enbAnalogInput(analogIn0);
    enbAnalogInput(analogIn1);

    xTaskCreate(TaskReadADC, (const portCHAR *)"READ_ADV",
                256, NULL, 3, NULL );


	vTaskStartScheduler();

    avrSerialxPrint_P( &xSerialPort, PSTR("\r\n\n Application failed... no space for idle task!\r\n"));

}

static void TaskReadADC(void *pvParameters)
{
    (void) pvParameters;
    TickType_t xLastWakeTime;

    xLastWakeTime = xTaskGetTickCount();

    while(1)
    {
        ReadADCSensors();
        vTaskDelayUntil( &xLastWakeTime, ( 30 / portTICK_PERIOD_MS ) );
    }
}


static void ReadADCSensors(void)  // Read ADC Sensors
{

    if( xADCSemaphore != NULL )
    {
        // See if we can obtain the semaphore.  If the semaphore is not available
        // wait 10 ticks to see if it becomes free.
        if( xSemaphoreTake( xADCSemaphore, ( TickType_t ) 10 ) == pdTRUE )
        {
        // We were able to obtain the semaphore and can now access the
        // shared resource.
        // We want to have the ADC for us alone, as it takes some time to sample,
        // so we don't want it getting stolen during the middle of a conversion.
            setAnalogMode(MODE_10_BIT);    // 10-bit analogue-to-digital conversions

            adcValues.adc0 = 0;
            adcValues.adc1 = 0;

            startAnalogConversion(analogIn0, EXTERNAL_REF);   // start next conversion
            while( analogIsConverting() )
                _delay_loop_2(25);     // yield until conversion ready

            adcValues.adc0 = analogConversionResult();

            startAnalogConversion(analogIn1, EXTERNAL_REF);   // start next conversion
            while( analogIsConverting() )
                 _delay_loop_2(25);       // yield until conversion ready

            adcValues.adc1 = analogConversionResult();

            avrSerialPrintf("\r\n%u, %u\r\n", adcValues.adc0, adcValues.adc1);
            xSemaphoreGive( xADCSemaphore );

            return;
        }
    }
}
