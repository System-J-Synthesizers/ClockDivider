#include "kernel.h"
#include "event.h"
#include "task.h"
#include "../hal_chip.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Syscalls */
#define SVC_START_OS 0U
#define SVC_EVTQ_RECEIVE 1U
#define SVC_EVTQ_POST 2U

/* System priorities */
#define SVC_PRIORITY 13U
#define SYSTICK_PRIORITY 14U
#define PENDSV_PRIORITY 15U
#define USER_PRIORITY 1U

/* kernel assembly functions, defined in kernel_asm.S */
void os_start_first_task(os_tcb *t);
void os_start(os_tcb *first);

/**
 * @brief Switch the CPU context to the highest priority task ready to run
 * 
 */
void os_pend_context_switch(void) {
  SCB->ICSR |= (1U << SCB_ICSR_PENDSVSET_Pos);
}

void os_start(os_tcb *first) {
    asm("svc #0\n\t");
}

/**
 * @brief Initialises and starts the OS
 * 
 */
void os_init(void) {
  os_idle_task_init();
  
  chip_disable_isrs();

  NVIC_EnableIRQ(SVCall_IRQn);
  NVIC_SetPriority(SVCall_IRQn, 14);

  NVIC_EnableIRQ(PendSV_IRQn);
  NVIC_SetPriority(PendSV_IRQn, PENDSV_PRIORITY);

  chip_enable_isrs();

  os_set_current_task(os_get_next_task());
  /* raise SVC to start the OS, should never return from here */
  os_start(os_get_current_task());
}

void os_fault(int32_t f) {
  chip_disable_isrs();

  while(1) {
  }
}

void SVC_Handler(void) {
  asm(
    "ldr r1, [r0]\n\t" /* load the stack pointer */
    /* fill r4-r11 with default values, this increments the stack pointer in r1 */
    "ldmia r1!, {r4-r11, lr}\n\t"

    /* set the CPU to use the process stack pointer */
    "mrs r2, control\n\t"
    "orrs r2, r2, #0x2\n\t" /* 0x3 is unpriv 0x2 is priv */
    "msr control, r2\n\t"
    "isb\n\t"

    /* store the new stack pointer back to the TCB, ready for exception return */
    "str r1, [r0]\n\t"
    /* set the psp to the task stack pointer */
    "msr psp, r1\n\t"  

    /* branch to the task with the LR value we just put in */
    "bx lr\n\t"
  );
}

void PendSV_Handler(void) {
  asm(    
    /* put current task pointer into r0 */
    "push {lr}\n\t"
    "bl os_get_current_task  \n\t" 
    "pop {lr}\n\t"

    /* load the stack pointer */
    "ldr r1, [r0]\n\t"

    /* save the context to the tcb */
    "mrs r2, psp\n\t"
    
    //"tst r14, #0x10\n\t"
    //"it eq\n\t"
    //"vstmdbeq r2!, {s16-s31}\n\t"

    "stmdb r2!, {r4-r11, lr}\n\t"
    "str r2, [r0]\n\t"

    /* get the next task to run into r0 */
    "push {lr}\n\t"
    "bl os_get_next_task\n\t"
    "bl os_set_current_task\n\t"
    "pop {lr}\n\t"

    /* load context of next task */
    "ldr r1, [r0]\n\t"
    "ldmia r1!, {r4-r11, lr}\n\t"

    //"tst r14, #0x10\n\t"
    //"it eq\n\t"
    //"vldmiaeq r1!, {s16-s31}\n\t"
    
    "msr psp, r1\n\t"

    /* branch to task */  
    "bx lr\n\t"
  );
}

void HardFault_Handler(void) {
  asm(
    "mov r0, #-1\n\t"
    "b os_fault\n\t"
  );
}