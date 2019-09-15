/******************************** INCLUDE FILES *******************************/
/* I/O include files */
#include "freeRTOS/lib_io/serial.h"

#include "adc.h"
#include "rtos.h"

/******************************** LOCAL DEFINES *******************************/


/******************************** GLOBALDATA *******************************/
extern xComPortHandle xSerialPort;

/********************************* LOCAL DATA *********************************/

/******************************* LOCAL FUNCTIONS ******************************/


/***************************** MAIN  ****************************/
int main(void) __attribute__((OS_main));
int main(void)
{
    genericTask_t *adc;

    xSerialPort = xSerialPortInitMinimal( USART0, 115200, 10, 10);
    avrSerialxPrint_P( &xSerialPort, PSTR("\r\n\n\nApllication Started Succesfuly!\r\n"));

    adc = getAdcTask();

    /* Initialize tasks */
    adc->initTask();

    /* Start tasks */
    rtos_createTask(adc);

    /* Start the OS scheduler */
    rtos_start();

    avrSerialxPrint_P( &xSerialPort, PSTR("\r\n\n Application failed... no space for idle task!\r\n"));
}
