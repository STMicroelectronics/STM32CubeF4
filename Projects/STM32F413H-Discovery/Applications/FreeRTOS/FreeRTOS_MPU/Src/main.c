/**
  ******************************************************************************
  * @file    FreeRTOS/FreeRTOS_MPU/Src/main.c
  * @author  MCD Application Team
  * @brief   Main Program file of the FreeRTOS MPU application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Definitions for the messages that can be sent to the check task. */
#define TEST_1_STILL_EXECUTING ( 0 )
#define TEST_2_STILL_EXECUTING ( 1 )
#define CYCLE_RESET            ( 2 )

#define TEST_TASK_1_PARAMETER	( ( void * ) 0x11112222 )

/* Private macro -------------------------------------------------------------*/
#if defined(__ICCARM__)
#define mainALIGN_TO( x )
#else
#define mainALIGN_TO( x )				__attribute__((aligned(x)))
#endif

/* Private function prototypes -----------------------------------------------*/	
static void SystemClock_Config(void);

/*
 * Prototype for the two test tasks, which execute in User mode.
 * Amongst other things, Both tasks execute at the idle priority so will get 
 * preempted regularly. Each task repeatedly sends a
 * message on a queue to a 'check' task so the check task knows the test task 
 * is still executing and has not detected any errors.  If an error
 * is detected within the task the task is simply deleted so it no longer sends
 * messages.
 *
 * Both tasks obtain access to the queue handle in different ways; 
 * Test1Task() is created in Privileged mode and copies the queue handle to 
 * its local stack before setting itself to User mode, and Test2Task() receives 
 * the task handle using its parameter.
 */
static void Test1Task( void *pvParameters );
static void Test2Task( void *pvParameters );

/*
 * Prototype for the check task.  The check task demonstrates various features
 * of the MPU before entering a loop where it waits for messages to arrive on a
 * queue.
 *
 * Two types of messages can be processes:
 *
 * 1) "I'm Alive" messages sent from the first two test tasks as described above.
 *
 * 2) "reset cycle" sent periodically by the tick hook function (and
 *    therefore from within an interrupt) which commands the check task to verify
 *    the execution of the application
 */
static void CheckTask( void *pvParameters );

/*
 * Prototype for a task created in User mode using the original vTaskCreate()
 * API function.  The task demonstrates the characteristics of such a task,
 * before simply deleting itself.
 */
static void UserModeTask( void *pvParameters );

/*
 * Prototype for a task created in Privileged mode using the original
 * vTaskCreate() API function.  The task demonstrates the characteristics of
 * such a task, before simply deleting itself.
 */
static void PrivilegedModeTask( void *pvParameters );

/*
 * Used by the two test tasks to send messages to the check task.  
 * The message just lets the check task know that the tasks are still functioning 
 * correctly.  If a test task detects an error it will delete itself, 
 * and in so doing prevent itself from sending any more 'I'm Alive' messages 
 * to the check task.
 */
static void SendImAlive( QueueHandle_t xHandle, uint32_t ulTaskNumber );

/*
 * The check task is created with access to three memory regions (plus its
 * stack).  Each memory region is configured with different parameters and
 * TestMemoryRegions() demonstrates what can and cannot be accessed for each
 * region.  It also demonstrates a task that was created
 * as a privileged task settings its own privilege level down to that of a user
 * task.
 */
static void TestMemoryRegions( void );

/* Private variables ---------------------------------------------------------*/
/* The handle of the queue used to communicate between tasks and between tasks
and interrupts.  Note that this is a global scope variable that falls outside of
any MPU region.  As such other techniques have to be used to allow the tasks
to gain access to the queue.  See the comments in the tasks themselves for
further information. */
QueueHandle_t xGlobalScopeCheckQueue = NULL;

#if defined(__CC_ARM) /* !< ARM Compiler */

extern uint32_t Image$$ER_IROM_FREERTOS_SYSTEM_CALLS$$Base;
extern uint32_t Image$$ER_IROM_FREERTOS_SYSTEM_CALLS$$Limit;

/* Memory map needed for MPU setup. Must match the one defined in
 * the scatter-loading file (Project.sct). */
const uint32_t * __FLASH_segment_start__ = ( uint32_t * ) 0x08000000;
const uint32_t * __FLASH_segment_end__ = ( uint32_t * ) 0x080FFFFF;
const uint32_t * __SRAM_segment_start__ = ( uint32_t * ) 0x20000000;
const uint32_t * __SRAM_segment_end__ = ( uint32_t * ) 0x20017FFF;

const uint32_t * __privileged_functions_start__ = ( uint32_t * ) 0x08000000;
const uint32_t * __privileged_functions_end__ = ( uint32_t * ) 0x08007FFF;
const uint32_t * __privileged_data_start__ = ( uint32_t * ) 0x20000000;
const uint32_t * __privileged_data_end__ = ( uint32_t * ) 0x20007FFF;

const uint32_t * __syscalls_flash_start__ = ( uint32_t * ) &( Image$$ER_IROM_FREERTOS_SYSTEM_CALLS$$Base );
const uint32_t * __syscalls_flash_end__ = ( uint32_t * ) &( Image$$ER_IROM_FREERTOS_SYSTEM_CALLS$$Limit );
#else
extern uint32_t __FLASH_segment_start__[];
extern uint32_t __FLASH_segment_end__[];
extern uint32_t __SRAM_segment_end__[];
extern uint32_t __privileged_functions_start__[];
extern uint32_t __privileged_functions_end__[];
extern uint32_t __privileged_data_start__[];
extern uint32_t __privileged_data_end__[];
extern uint32_t __syscalls_flash_start__[];
extern uint32_t __syscalls_flash_end__  [];
#endif


/* Data used by the 'Check' task. ---------------------------*/

/* Define the constants used to allocate the check task stack.  Note that the
stack size is defined in words, not bytes. */
#define CHECK_TASK_STACK_SIZE_WORDS	256
#define CHECK_TASK_STACK_ALIGNMENT ( CHECK_TASK_STACK_SIZE_WORDS * sizeof( portSTACK_TYPE ) )

/* Declare the stack that will be used by the check task.  The kernel will
 automatically create an MPU region for the stack.  The stack alignment must
 match its size, so if 128 words are reserved for the stack then it must be
 aligned to ( 128 * 4 ) bytes. */
#if defined(__ICCARM__)
#pragma data_alignment= CHECK_TASK_STACK_ALIGNMENT
#endif
static portSTACK_TYPE xCheckTaskStack[ CHECK_TASK_STACK_SIZE_WORDS ]  mainALIGN_TO( CHECK_TASK_STACK_ALIGNMENT );

/* Declare three arrays - an MPU region will be created for each array
using the TaskParameters_t structure below.  THIS IS JUST TO DEMONSTRATE THE
MPU FUNCTIONALITY, the data is not used by the check tasks primary function.

Note that the arrays allocate slightly more RAM than is actually assigned to
the MPU region.  This is to permit writes off the end of the array to be
detected even when the arrays are placed in adjacent memory locations (with no
gaps between them).  The align size must be a power of two. */
#define READ_WRITE_ARRAY_SIZE 130
#define READ_WRITE_ALIGN_SIZE 128

#if defined(__ICCARM__)
#pragma data_alignment= READ_WRITE_ALIGN_SIZE 
#endif
char ReadWriteArray[ READ_WRITE_ARRAY_SIZE ] mainALIGN_TO( READ_WRITE_ALIGN_SIZE );

#define READ_ONLY_ARRAY_SIZE 260
#define READ_ONLY_ALIGN_SIZE 256

#if defined(__ICCARM__)
#pragma data_alignment= READ_ONLY_ALIGN_SIZE
#endif
char ReadOnlyArray[ READ_ONLY_ARRAY_SIZE ] mainALIGN_TO( READ_ONLY_ALIGN_SIZE );

#define PRIVILEGED_ONLY_ACCESS_ARRAY_SIZE 130
#define PRIVILEGED_ONLY_ACCESS_ALIGN_SIZE 128

#if defined(__ICCARM__)
#pragma data_alignment= PRIVILEGED_ONLY_ACCESS_ALIGN_SIZE
#endif
char PrivilegedOnlyAccessArray[ PRIVILEGED_ONLY_ACCESS_ALIGN_SIZE ] mainALIGN_TO( PRIVILEGED_ONLY_ACCESS_ALIGN_SIZE );

/* Fill in a TaskParameters_t structure to define the check task - this is the
structure passed to the xTaskCreateRestricted() function. */
static const TaskParameters_t xCheckTaskParameters =
{
  CheckTask,
  "CheckTask",
  CHECK_TASK_STACK_SIZE_WORDS,
  ( void * ) 0x12121212,
  ( tskIDLE_PRIORITY + 1 ) | portPRIVILEGE_BIT,
  xCheckTaskStack,
  {
    /* Base address			Length			                Parameters */
    { ReadWriteArray,			READ_WRITE_ALIGN_SIZE,		portMPU_REGION_READ_WRITE },
    { ReadOnlyArray,			READ_ONLY_ALIGN_SIZE,		portMPU_REGION_READ_ONLY },
    { PrivilegedOnlyAccessArray,	PRIVILEGED_ONLY_ACCESS_ALIGN_SIZE,	portMPU_REGION_PRIVILEGED_READ_WRITE }
  }
};

/* Data used by the 'test' tasks. -----------------------*/

/* Define the constants used to allocate the test task stacks.  Note that
that stack size is defined in words, not bytes. */
#define TEST_STACK_SIZE_WORDS	128
#define TEST_STACK_ALIGNMENT	( TEST_STACK_SIZE_WORDS * sizeof( portSTACK_TYPE ) )

/* Declare the stacks that will be used by the test tasks.  The kernel will
automatically create an MPU region for the stack.  The stack alignment must
match its size, so if 128 words are reserved for the stack then it must be
aligned to ( 128 * 4 ) bytes. */
#if defined(__ICCARM__)
#pragma data_alignment= TEST_STACK_ALIGNMENT
#endif
static portSTACK_TYPE xTest1Stack[ TEST_STACK_SIZE_WORDS ] mainALIGN_TO( TEST_STACK_ALIGNMENT );

#if defined(__ICCARM__)
#pragma data_alignment= TEST_STACK_ALIGNMENT
#endif
static portSTACK_TYPE xTest2Stack[ TEST_STACK_SIZE_WORDS ] mainALIGN_TO( TEST_STACK_ALIGNMENT );

/* Fill in a TaskParameters_t structure per test task to define the tasks. */
static const TaskParameters_t xTest1Parameters =
{
  Test1Task,
  "Test1Task",
  TEST_STACK_SIZE_WORDS,
  ( void * ) TEST_TASK_1_PARAMETER,
  tskIDLE_PRIORITY | portPRIVILEGE_BIT,
  xTest1Stack,
  {
    /* Base address		Length		Parameters */
    { 0x00,				0x00,			0x00 },
    { 0x00,				0x00,			0x00 },
    { 0x00,				0x00,			0x00 }
  }
};


static TaskParameters_t xTest2Parameters =
{
  Test2Task,
  "Test2Task",
  TEST_STACK_SIZE_WORDS,
  ( void * ) NULL,
  tskIDLE_PRIORITY,
  xTest2Stack,
  {
    /* Base address		Length		Parameters */
    { 0x00,				0x00,			0x00 },
    { 0x00,				0x00,			0x00 },
    { 0x00,				0x00,			0x00 }
  }
};

/* Private functions ---------------------------------------------------------*/
int main( void )
{
  
  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();
  
  /* Configure the system clock to 100 MHz */
  SystemClock_Config();	
  
  /* Create the queue used to pass "I'm alive" messages to the check task. */
  xGlobalScopeCheckQueue = xQueueCreate( 1, sizeof( uint32_t ) );
  
  /* One check task uses the task parameter to receive the queue handle.
  This allows the file scope variable to be accessed from within the task.
  The pvParameters member of xTest2Parameters can only be set after the
  queue has been created so is set here. */
  xTest2Parameters.pvParameters = xGlobalScopeCheckQueue;
  
  /* Create three test tasks.  Handles to the created tasks are not required,
  hence the second parameter is NULL. */
  xTaskCreateRestricted( &xTest1Parameters, NULL );
  xTaskCreateRestricted( &xTest2Parameters, NULL );
  xTaskCreateRestricted( &xCheckTaskParameters, NULL );
  
  /* Create the tasks that are created using the original xTaskCreate() API function. */
  xTaskCreate( UserModeTask, "Task1", 100,	NULL, 3, NULL );  
  xTaskCreate( PrivilegedModeTask, "Task2", 100, NULL,( 3 | portPRIVILEGE_BIT ), NULL );
  
  /* Start the scheduler. */
  vTaskStartScheduler();
  
  /* Will only get here if there was insufficient memory to create the idle
  task. */
  for( ;; );
}
/*-----------------------------------------------------------*/

static void CheckTask( void *pvParameters )
{
  /* This task is created in privileged mode so can access the file scope
  queue variable.  Take a stack copy of this before the task is set into user
  mode.  Once that task is in user mode the file scope queue variable will no
  longer be accessible but the stack copy will. */
  QueueHandle_t xQueue = xGlobalScopeCheckQueue;
  int32_t lMessage;
  
  /* Just to remove compiler warning. */
  ( void ) pvParameters;
  
  /* Demonstrate how the various memory regions can and can't be accessed.
  The task privilege level is set down to user mode within this function. */
  TestMemoryRegions();
  
  ReadWriteArray[0] = 0;
  ReadWriteArray[1] = 0;
  ReadWriteArray[2] = 0;
  
  /* This loop performs the main function of the task, which is blocking
  on a message queue then processing each message as it arrives. */
  for( ;; )
  {
    /* Wait for the next message to arrive. */
    xQueueReceive( xQueue, &lMessage, portMAX_DELAY );
    
    switch( lMessage )
    {
    case TEST_1_STILL_EXECUTING	:
      (ReadWriteArray[0])++;
      break;
    case TEST_2_STILL_EXECUTING:
      (ReadWriteArray[1])++;
      break;
    case CYCLE_RESET:
      /* Message from tick hook, to reset the count cycle.  
	  If messages have stopped arriving from either of
      the two test task then the status must be set to fail. */
      if( ( ReadWriteArray[ 0 ] == 0 ) || ( ReadWriteArray[ 1 ] == 0 ) )
      {
		/* One or both of the test tasks are no longer sending
        'still alive' messages. */
        for(;;);
      }
      else
      {
        /* Reset the count of 'still alive' messages. */
        ReadWriteArray[0] = 0;
        ReadWriteArray[1] = 0;

        /* Increment The cycle count  */
        (ReadWriteArray[2])++;
      }

      break;
      
    default :
      /* Something unexpected happened.  Delete this task so the
      error is apparent (no output will be displayed). */
      vTaskDelete( NULL );
      break;
    }
  }
}
/*-----------------------------------------------------------*/

static void TestMemoryRegions( void )
{
  int32_t x;
  char cTemp;
  
  /* The check task (from which this function is called) is created in the
  Privileged mode.  The privileged array can be both read from and written
  to while this task is privileged. */
  PrivilegedOnlyAccessArray[ 0 ] = 'a';
  if( PrivilegedOnlyAccessArray[ 0 ] != 'a' )
  {
    /* Something unexpected happened.  Delete this task so the error is
    apparent (no output will be displayed). */
    vTaskDelete( NULL );
  }
  
  /* Writing off the end of the RAM allocated to this task will *NOT* cause a
  protection fault because the task is still executing in a privileged mode.
  Uncomment the following to test. */
  /*PrivilegedOnlyAccessArray[ PRIVILEGED_ONLY_ACCESS_ALIGN_SIZE ] = 'a';*/
  
  /* Now set the task into user mode. */
  portSWITCH_TO_USER_MODE();
  
  /* Accessing the privileged only array will now cause a fault.  Uncomment
  the following line to test. */
  /*PrivilegedOnlyAccessArray[ 0 ] = 'a';*/
  
  /* The read/write array can still be successfully read and written. */
  for( x = 0; x < READ_WRITE_ALIGN_SIZE; x++ )
  {
    ReadWriteArray[ x ] = 'a';
    if( ReadWriteArray[ x ] != 'a' )
    {
      /* Something unexpected happened.  Delete this task so the error is
      apparent (no output will be displayed). */
      vTaskDelete( NULL );
    }
  }
  
  /* But attempting to read or write off the end of the RAM allocated to this
  task will cause a fault.  Uncomment either of the following two lines to
  test. */
  /* ReadWriteArray[ 0 ] = cReadWriteArray[ -1 ]; */
  /* ReadWriteArray[ READ_WRITE_ALIGN_SIZE ] = 0x00; */
  
  /* The read only array can be successfully read... */
  for( x = 0; x < READ_ONLY_ALIGN_SIZE; x++ )
  {
    cTemp = ReadOnlyArray[ x ];
  }
  
  /* ...but cannot be written.  Uncomment the following line to test. */
  /* ReadOnlyArray[ 0 ] = 'a'; */
  
  /* Writing to the first and last locations in the stack array should not
  cause a protection fault.  Note that doing this will cause the kernel to
  detect a stack overflow if configCHECK_FOR_STACK_OVERFLOW is greater than
  1, hence the test is commented out by default. */
  /* xCheckTaskStack[ 0 ] = 0;
  xCheckTaskStack[ CHECK_TASK_STACK_SIZE_WORDS - 1 ] = 0; */
  
  /* Writing off either end of the stack array should cause a protection
  fault, uncomment either of the following two lines to test. */
  /* xCheckTaskStack[ -1 ] = 0; */
  /* xCheckTaskStack[ CHECK_TASK_STACK_SIZE_WORDS ] = 0; */
  
  ( void ) cTemp;
}

/*-----------------------------------------------------------*/

static void UserModeTask( void *pvParameters )
{
  /*const volatile uint32_t *pulStandardPeripheralRegister = ( volatile uint32_t * ) 0x40000000;*/
  volatile const uint32_t *pul;
  volatile uint32_t ulReadData;
  
  /* The following lines are commented out to prevent the unused variable
  compiler warnings when the tests that use the variable are also commented out. */
  /*extern uint32_t __privileged_functions_start; */
  /*const volatile uint32_t *pulSystemPeripheralRegister = ( volatile uint32_t * ) 0xe000e014; */
  
  ( void ) pvParameters;
  
  /* This task is created in User mode using the original xTaskCreate() API
  function.  It should have access to all Flash and RAM except that marked
  as Privileged access only.  Reading from the start and end of the non-
  privileged RAM should not cause a problem (the privileged RAM is the first
  block at the bottom of the RAM memory). */
  pul = __privileged_data_end__ + 1;
  ulReadData = *pul;
  pul = __SRAM_segment_end__ - 1;
  ulReadData = *pul;
  
  /* Likewise reading from the start and end of the non-privileged Flash
  should not be a problem (the privileged Flash is the first block at the
  bottom of the Flash memory). */
  pul = __privileged_functions_end__ + 1;
  ulReadData = *pul;
  pul = __FLASH_segment_end__ - 1;
  ulReadData = *pul;
  
  /* Standard peripherals are accessible. 
  ulReadData = *pulStandardPeripheralRegister;*/
  
  /* System peripherals are not accessible.  Uncomment the following line
  to test.  Also uncomment the declaration of pulSystemPeripheralRegister
  at the top of this function.
  ulReadData = *pulSystemPeripheralRegister; */
  
  /* Reading from anywhere inside the privileged Flash or RAM should cause a
  fault.  This can be tested by uncommenting any of the following pairs of
  lines.  Also uncomment the declaration of __privileged_functions_start__
  at the top of this function. */
  
  /*pul = __privileged_functions_start__;
  ulReadData = *pul;*/
  
  /*pul = __privileged_functions_end__ - 1;
  ulReadData = *pul;*/
  
  /*pul = __privileged_data_start__;
  ulReadData = *pul;*/
  
  /*pul = __privileged_data_end__ - 1;
  ulReadData = *pul;*/
  
  /* Must not just run off the end of a task function, so delete this task.
  Note that because this task was created using xTaskCreate() the stack was
  allocated dynamically and I have not included any code to free it again. */
  vTaskDelete( NULL );
  
  ( void ) ulReadData;
}
/*-----------------------------------------------------------*/

static void PrivilegedModeTask( void *pvParameters )
{
  volatile const uint32_t *pul;
  volatile uint32_t ulReadData;
  const volatile uint32_t *pulSystemPeripheralRegister = ( volatile uint32_t * ) 0xe000e014; /* Systick */
  const volatile uint32_t *pulStandardPeripheralRegister = ( volatile uint32_t * ) 0x40000000;
  
  ( void ) pvParameters;
  
  /* This task is created in Privileged mode using the original xTaskCreate()
  API	function.  It should have access to all Flash and RAM including that
  marked as Privileged access only.  So reading from the start and end of the
  non-privileged RAM should not cause a problem (the privileged RAM is the
  first block at the bottom of the RAM memory). */
  pul = __privileged_data_end__ + 1;
  ulReadData = *pul;
  pul = __SRAM_segment_end__ - 1;
  ulReadData = *pul;
  
  /* Likewise reading from the start and end of the non-privileged Flash
  should not be a problem (the privileged Flash is the first block at the
  bottom of the Flash memory). */
  pul = __privileged_functions_end__ + 1;
  ulReadData = *pul;
  pul = __FLASH_segment_end__ - 1;
  ulReadData = *pul;
  
  /* Reading from anywhere inside the privileged Flash or RAM should also
  not be a problem. */
  pul = __privileged_functions_start__;
  ulReadData = *pul;
  pul = __privileged_functions_end__ - 1;
  ulReadData = *pul;
  pul = __privileged_data_start__;
  ulReadData = *pul;
  pul = __privileged_data_end__ - 1;
  ulReadData = *pul;
  
  /* Finally, accessing both System and normal peripherals should both be
  possible. */
  ulReadData = *pulSystemPeripheralRegister;
  ulReadData = *pulStandardPeripheralRegister;
  
  /* Must not just run off the end of a task function, so delete this task.
  Note that because this task was created using xTaskCreate() the stack was
  allocated dynamically and I have not included any code to free it again. */
  vTaskDelete( NULL );
  
  ( void ) ulReadData;
}

/*-----------------------------------------------------------*/

static void SendImAlive( QueueHandle_t xHandle, uint32_t ulTaskNumber )
{
  if( xHandle != NULL )
  {
    xQueueSend( xHandle, &ulTaskNumber, 0 );
  }
}

/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
  volatile const uint32_t *pul;
  volatile uint32_t ulReadData;
  
  /* The idle task, and therefore this function, run in Supervisor mode and
  can therefore access all memory.  Try reading from corners of flash and
  RAM to ensure a memory fault does not occur.
  
  Start with the edges of the privileged data area. */
  pul = __privileged_data_start__;
  ulReadData = *pul;
  pul = __privileged_data_end__ - 1;
  ulReadData = *pul;
  
  /* Next the standard SRAM area. */
  pul = __SRAM_segment_end__ - 1;
  ulReadData = *pul;
  
  /* And the standard Flash area - the start of which is marked for
  privileged access only. */
  pul = __FLASH_segment_start__;
  ulReadData = *pul;
  pul = __FLASH_segment_end__ - 1;
  ulReadData = *pul;
  
  /* Reading off the end of Flash or SRAM space should cause a fault.
  Uncomment one of the following two pairs of lines to test. */
  
  /* pul = __FLASH_segment_end__ + 4;
  ulReadData = *pul; */
  
  /* pul = __SRAM_segment_end__ + 1;
  ulReadData = *pul; */
  
  ( void ) ulReadData;
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
  static uint32_t ulCallCount = 0;
  const uint32_t ulCallsBetweenSends = pdMS_TO_TICKS( 5000 );
  const uint32_t ulMessage = CYCLE_RESET;
  portBASE_TYPE xDummy;
  
  /* If configUSE_TICK_HOOK is set to 1 then this function will get called
  from each RTOS tick.  It is called from the tick interrupt and therefore
  will be executing in the privileged state. */
  
  ulCallCount++;
  
  /* Is it time to print out the pass/fail message again? */
  if( ulCallCount >= ulCallsBetweenSends )
  {
    ulCallCount = 0;
    
    /* Send a message to the check task to command it to check that all
    the tasks are still running then print out the status.
    
    This is running in an ISR so has to use the "FromISR" version of
    xQueueSend().  Because it is in an ISR it is running with privileges
    so can access xGlobalScopeCheckQueue directly. */
    xQueueSendFromISR( xGlobalScopeCheckQueue, &ulMessage, &xDummy );
  }
}

/*-----------------------------------------------------------*/

static void Test1Task( void *pvParameters )
{
  /* This task is created in privileged mode so can access the file scope
  queue variable.  Take a stack copy of this before the task is set into user
  mode.  Once this task is in user mode the file scope queue variable will no
  longer be accessible but the stack copy will. */
  QueueHandle_t xQueue = xGlobalScopeCheckQueue;
  const TickType_t xDelayTime = pdMS_TO_TICKS( 100UL );
  
  /* Now the queue handle has been obtained the task can switch to user
  mode.  This is just one method of passing a handle into a protected
  task, the other	reg test task uses the task parameter instead. */
  portSWITCH_TO_USER_MODE();
  
  /* First check that the parameter value is as expected. */
  if( pvParameters != ( void * ) TEST_TASK_1_PARAMETER )
  {
    /* Error detected.  Delete the task so it stops communicating with
    the check task. */
    vTaskDelete(NULL);
  }
  
  for( ;; )
  {
    /* Send TEST_1_STILL_EXECUTING to the check task to indicate that this
    task is still functioning. */
    SendImAlive( xQueue, TEST_1_STILL_EXECUTING );
    vTaskDelay( xDelayTime );
  }
}
/*-----------------------------------------------------------*/

static void Test2Task( void *pvParameters )
{
  /* The queue handle is passed in as the task parameter.  This is one method of
  passing data into a protected task, the other reg test task uses a different
  method. */
  QueueHandle_t xQueue = ( QueueHandle_t ) pvParameters;
  const TickType_t xDelayTime = pdMS_TO_TICKS( 100UL );
  
  for( ;; )
  {
    
    /* Send TEST_2_STILL_EXECUTING to the check task to indicate
    that this task is still functioning. */
    SendImAlive( xQueue, TEST_2_STILL_EXECUTING );
    vTaskDelay( xDelayTime );
  }
}

#if configCHECK_FOR_STACK_OVERFLOW
void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
  /* If configCHECK_FOR_STACK_OVERFLOW is set to either 1 or 2 then this
  function will automatically get called if a task overflows its stack. */
  ( void ) pxTask;
  ( void ) pcTaskName;
  for( ;; );
}
#endif

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 100000000
  *            HCLK(Hz)                       = 100000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 200
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 3
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 200;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  RCC_OscInitStruct.PLL.PLLR = 2;
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  
  if(ret != HAL_OK)
  {
    while(1) { ; } 
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3);
  if(ret != HAL_OK)
  {
    while(1) { ; }  
  }
}
