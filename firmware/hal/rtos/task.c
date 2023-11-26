#include "task.h"
#include "event.h"
#include "kernel.h"
#include "os_assert.h"
#include "../hal_chip.h"
#include <cross_studio_io.h>
#include <stddef.h>
#include <stdint.h>

#define IDLE_STACK_SIZE 512U /* size of the idle task stack */

static void os_evt_func(void *params);
static void os_idle_func(void);

static os_tcb task_array[NUM_TASKS];
static os_tcb *current_task;

static uint32_t idle_task_stack[IDLE_STACK_SIZE];

/* task status flags */
volatile uint32_t os_tasks_ready = 0U;

/**
 * @brief      Get highest priority task that is ready.
 *
 * @param[in]  flags  The flags
 *
 * @return     index of highest priority task
 */
uint32_t os_get_highest_priority(void) {
  uint32_t result;
  asm(
    "rbit r0, %1\n\t" /* reverse bit order since lower === higher */
    "clz %0, r0\n\t"  /* count the leading zeroes to the first set bit */
    : "=r" (result) : "r" (os_tasks_ready)
  );
  return result;
}

/**
 * @brief Set the ready bit of a task
 * 
 * @param priority Priority of task bit to set
 */
void os_task_set_ready_flag(uint8_t priority) {
  OS_ASSERT(priority <= IDLE_TASK_PRIORTY);
  os_tasks_ready |= ((uint32_t)1U << priority); /* rule 10.3 */
}

/**
 * @brief Clear the ready bit of a task
 * 
 * @param priority Priority of task bit to clear
 */
void os_task_clear_ready_flag(uint8_t priority) {
  OS_ASSERT(priority <= MAX_PRIORITY);
  os_tasks_ready &= ~((uint32_t)1U << priority); /* rule 10.3 */
}

uint32_t os_task_is_ready(uint8_t priority) {
  OS_ASSERT(priority <= MAX_PRIORITY);
  return os_tasks_ready & ((uint32_t)1U << priority); /* rule 10.3 */
}

/**
 * @brief Get pointer to current task tcb
 * 
 * @return os_tcb* Pointer to current task
 */
os_tcb *os_get_current_task(void) {
  return current_task;
}

/**
 * @brief      Set the current task pointer to some tcb
 *
 * @param      t     task pointer to become the current task
 */
void os_set_current_task(os_tcb *t) {
  OS_ASSERT(t != NULL);
  current_task = t;
}

/**
 * @brief Get the highest priority task that's ready to run.
 *         returns 32 if no tasks ready, which is the idle task priority
 * @return os_tcb* Pointer to the TCB of the next task.
 */
os_tcb *os_get_next_task(void) {
  os_tcb *t;
  
  if(os_tasks_ready == 0U) {
    t = &task_array[IDLE_TASK_PRIORTY];
  } else {
    t = &task_array[os_get_highest_priority()];
  }

  return t;
}

/**
 * @brief Set up a task stack
 * 
 * @param stack Stack of 32 bit words
 * @param s_size Size of stack in words
 * @param func_addr Address of the thread function casted to uint
 * @return uint32_t* stack pointer of the ready stack
 */
static uint32_t *os_task_init_stack(uint32_t *const stack, uint32_t s_size, uint32_t func_addr, uint32_t params) {
  OS_ASSERT(stack != NULL);
  OS_ASSERT(s_size >= MIN_STACK_SIZE);
  OS_ASSERT(func_addr != 0UL);

  uint32_t *stk_ptr = stack;

  stk_ptr[0] = STACK_OVERFLOW_DEBUG_VAL;
  for(uint32_t i = 1U; i < s_size; i++) {
      stk_ptr[i] = STACK_DEBUG_VAL;
  }

  stk_ptr = &stack[s_size - 1U];
 
  /* Put a fake ISR stack frame onto the stack */
  *stk_ptr = INITIAL_XPSR; /* xPSR - Set T bit for thumb state */
  stk_ptr--;
  /* Implementation defined behaviour, but we know the implementation... */
  *stk_ptr = func_addr; /* PC - Thread entry function, the CPU will branch here on exception exit. */
  stk_ptr--;
  *stk_ptr = 0xFFFFFFFDU; /* LR - Return to thread mode using the MSP */
  stk_ptr--;
  /* R12, R3 - R0 */
  *stk_ptr = 0x00000012U;
  stk_ptr--;
  *stk_ptr = 0x00000003U;
  stk_ptr--;
  *stk_ptr = 0x00000002U;
  stk_ptr--;
  *stk_ptr = 0x00000001U;
  stk_ptr--;
  *stk_ptr = params; /* R0 */

  stk_ptr--;
  *stk_ptr = 0xFFFFFFFDU;

  /* R11 - R4 */
  for (uint8_t i = 11U; i >= 4U; i--) {
      stk_ptr--;
      *stk_ptr = i; 
  }

  return stk_ptr;
}

/**
 * @brief Set up a task control block
 * 
 * @param stack pointer to stack top
 * @param s_size size of stack in words
 * @param priority priority of this task
 * @param name name of the task
 * @return os_tcb* const 
 */
os_tcb *os_task_init(os_evt_dispatch d, uint32_t *const stack, uint32_t s_size, 
                     os_event **evtq, uint32_t q_size, uint8_t priority) {
  OS_ASSERT(d != NULL);
  OS_ASSERT(stack != NULL);
  OS_ASSERT(s_size >= MIN_STACK_SIZE);
  OS_ASSERT(priority <= MAX_PRIORITY);
  OS_ASSERT(os_task_is_ready(priority) != (1U << priority));

  os_tcb *const t = &task_array[priority];

  t->dispatch = d;
  t->priority = priority;
  t->sp = os_task_init_stack(stack, s_size, (uint32_t)os_evt_func, (uint32_t)t);
  t->evt_q = os_evtq_init(priority, q_size, evtq);

  os_task_set_ready_flag(priority);

  return t;
}

/**
 * @brief Initialise the idle task
 * 
 */
void os_idle_task_init(void) {
  os_tcb *idle_task = &task_array[IDLE_TASK_PRIORTY];
  os_task_set_ready_flag(IDLE_TASK_PRIORTY);
  idle_task->priority = IDLE_TASK_PRIORTY;
  idle_task->sp = os_task_init_stack(idle_task_stack, IDLE_STACK_SIZE, (uint32_t)os_idle_func, (uint32_t)idle_task);
}

/**
 * @brief      Event driven task function
 *
 * @param      params  This is a pointer to the TCB that will be calling
 */
static void os_evt_func(void *params) {
  OS_ASSERT(params != NULL);
  os_tcb *t = (os_tcb *)params; /* 'this' tcb is passed in at task creation */

  /* send in the initial event */
  static os_event const first = {INIT_SIG};
  (*t->dispatch)(t, &first);

  while(1) {
    os_event *e;
    /* triggers svc and will not return until events are in the queue */
    e = os_evtq_receive(t);
    if (e != (os_event *)OS_EVTQ_EMPTY) {
      (*t->dispatch)(t, e); /* post the event to the dispatch function */
    } else {
      chip_disable_isrs();
      os_task_clear_ready_flag(t->priority);
      os_pend_context_switch();
      chip_enable_isrs();
    }
  }
}

static void os_idle_func(void) {
  while(1) {
  }
}