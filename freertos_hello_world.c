
/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
/* Xilinx includes. */
#include "xil_printf.h"
#include "xparameters.h"
#include "xgpio.h"

#define LED_CHANNEL 1


/*-----------------------------------------------------------*/
#define LED_DEVICE_ID XPAR_AXI_GPIO_0_DEVICE_ID



/* The TaskLED, TaskBTN and TaskSW tasks */
static void Task_1( void *pvParameters );
static void Task_2( void *pvParameters );
static void Task_3( void *pvParameters );
static void Task_4( void *pvParameters );
/*-----------------------------------------------------------*/

/* The task handles  */
static TaskHandle_t xTask1;
static TaskHandle_t xTask2;
static TaskHandle_t xTask3;
static TaskHandle_t xTask4;


XGpio LEDInst;


int main( void )
{

	xil_printf( "Hello from Freertos example main\r\n" );

	int status;

	 // Initialize LEDs
	 status = XGpio_Initialize(&LEDInst, LED_DEVICE_ID);
	 if (status != XST_SUCCESS) return XST_FAILURE;

	 // Set LEDs direction to outputs
	 XGpio_SetDataDirection(&LEDInst, 1, 0x00);

	/* Create the three tasks.
	Create  task1 with priority 1 */
	xTaskCreate( 	Task_1, /* The function that implements the task. */
				( const char * ) "Task_1",  /* Text name for the task, provided to assist debugging only. */
				configMINIMAL_STACK_SIZE, 	/* The stack allocated to the task. */
				NULL, 						/* The task parameter is not used, so set to NULL. */
				tskIDLE_PRIORITY + 1,			/* The task runs at the idle priority. */
				&xTask1);

	/* Create  task2 with priority 1 */
	xTaskCreate( Task_2,
				( const char * ) "Task_2",
				configMINIMAL_STACK_SIZE,
				NULL,
				tskIDLE_PRIORITY + 1,
				&xTask2);



	/* Create  task4 with priority 1 */
	xTaskCreate( Task_4,
				 ( const char * ) "Task_4",
				 configMINIMAL_STACK_SIZE,
				 NULL,
				 tskIDLE_PRIORITY + 1,
				 &xTask4);


	/* Start the tasks and timer running. */
	vTaskStartScheduler();

	/* If all is well, the scheduler will now be running, and the following line
	will never be reached.  If the following line does execute, then there was
	insufficient FreeRTOS heap memory available for the idle and/or timer tasks
	to be created.  See the memory management section on the FreeRTOS web site
	for more details. */
	for( ;; );
}


/*-----------------------------------------------------------*/
static void Task_1( void *pvParameters )
{
	//

    const TickType_t x18seconds = pdMS_TO_TICKS( 18000 );  //Convert to tick time for delay
	for( ;; )
	{
		TickType_t xLastWakeTime;  // Define a variable to hold wakeuptime
		vTaskPrioritySet(xTask1, 5);  // Set priority of task1 to 3
		XGpio_DiscreteWrite(&LEDInst, LED_CHANNEL, 0x1);  // Light up LED0
		xil_printf( "Task 1 Running \n" );  // Print the task currently running
		for (int Delay = 0; Delay < 500000000; Delay++);  // Keep lit for approximately 1 sec
		xLastWakeTime = xTaskGetTickCount();  // Initialise the xLastWakeTime variable with the current time.
		vTaskDelayUntil(&xLastWakeTime, x18seconds);   // Wait for 18 seconds
	}
}

/*-----------------------------------------------------------*/
static void Task_2( void *pvParameters )
{
	for( ;; )
	{
		vTaskPrioritySet(xTask2, 4); // Set priority of task2 to 4
		XGpio_DiscreteWrite(&LEDInst, LED_CHANNEL, 0x2);  // Light up LED1
		xil_printf( "Task 2 Running \n" );  // Print the task currently running
		for (int Delay = 0; Delay < 500000000; Delay++);  // Keep lit for approximately 1 sec

		/* Create task3 with priority 3*/
		xTaskCreate( Task_3,
					 ( const char * ) "Task_3",
					 configMINIMAL_STACK_SIZE,
					 NULL,
					 tskIDLE_PRIORITY + 3,
					 &xTask3);
		vTaskSuspend(xTask2);  // Suspend task2
	}

}

/*-----------------------------------------------------------*/
static void Task_3( void *pvParameters )
{
	for( ;; )
	{
		vTaskPrioritySet(xTask3, 4); // Set priority of task3 to 4
		XGpio_DiscreteWrite(&LEDInst, LED_CHANNEL, 0x4); // Light up LED2
		xil_printf( "Task 3 Running \n" );  // Print the task currently running
		for (int Delay = 0; Delay < 500000000; Delay++);  // Keep lit for approximately 1 sec
		vTaskPrioritySet(xTask3, 1);  // Set priority of task3 to 1
		taskYIELD();  // Yield to task with higher priority and ready to run
	}
}

static void Task_4( void *pvParameters )
{
const TickType_t x5seconds = pdMS_TO_TICKS( 5000 );  //Convert to tick time for delay
	for( ;; )
	{
		vTaskPrioritySet(xTask4, 3);  // Set priority of task4 to 3
		XGpio_DiscreteWrite(&LEDInst, LED_CHANNEL, 0x8);  // Light up LED3
		xil_printf( "Task 4 Running \n" );  // Print the task currently running
		vTaskDelete(xTask3);  // Delete task3
		vTaskDelay(x5seconds);  // Delay for 5 seconds
		vTaskResume(xTask2);  // Resume task2
	}
}








