/******************************** INCLUDE FILES *******************************/
/* I/O include files */
#include "freeRTOS/lib_io/serial.h"

#include "adc.h"
#include "led.h"

#include "rtos.h"
#include "generic_t.h"

#include "freeRTOS/lib_io/digitalAnalog.h"

/******************************** LOCAL DEFINES *******************************/
#define BAUDRATE        115200
#define TX_QUEUE_LEN    10
#define RX_QUEUE_LEN    10

#define TASKS           2

/******************************** GLOBALDATA *******************************/
extern xComPortHandle xSerialPort;

/********************************* LOCAL DATA *********************************/
static genericTask_t *rtosTasks[TASKS] =
{
    &adcTask,
    &ledTask
};

/******************************* LOCAL FUNCTIONS ******************************/
// --

/***************************** MAIN  ****************************/
int main(void) __attribute__((OS_main));
int main(void)
{
    xSerialPort = xSerialPortInitMinimal(USART0, BAUDRATE, TX_QUEUE_LEN, RX_QUEUE_LEN);

    /* Starts all the tasks from Tasks array & starts the scheduler */
    rtos_start(rtosTasks);

    avrSerialxPrint_P(&xSerialPort, PSTR("\r\n\n Failed to start the emg application!\r\n"));
}
