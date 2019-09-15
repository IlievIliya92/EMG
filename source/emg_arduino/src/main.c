/******************************** INCLUDE FILES *******************************/
/* I/O include files */
#include "freeRTOS/lib_io/serial.h"

#include "adc.h"
#include "rtos.h"

/******************************** LOCAL DEFINES *******************************/
#define BAUDRATE        115200
#define TX_QUEUE_LEN    10
#define RX_QUEUE_LEN    10

/******************************** GLOBALDATA *******************************/
extern xComPortHandle xSerialPort;

/********************************* LOCAL DATA *********************************/

/******************************* LOCAL FUNCTIONS ******************************/

/***************************** MAIN  ****************************/
int main(void) __attribute__((OS_main));
int main(void)
{
    xSerialPort = xSerialPortInitMinimal(USART0, BAUDRATE, TX_QUEUE_LEN, RX_QUEUE_LEN);
    avrSerialxPrint_P(&xSerialPort, PSTR("\r\n\n\nApllication Started Succesfuly!\r\n"));

    /* Tasks array */
    genericTask_t *rtosTasks[TASKS];

    rtosTasks[0] = getAdcTask();

    /* Starts all the tasks from Tasks array & starts the scheduler */
    rtos_start(rtosTasks);

    avrSerialxPrint_P(&xSerialPort, PSTR("\r\n\n Application failed... no space for idle task!\r\n"));
}