#ifndef TASK_H
#define TASK_H

#include <stdint.h>
#include <stdbool.h>

#define NUM_TASKS 33U /* includes idle */
#define MAX_USER_TASKS (NUM_TASKS-1U) /* user cannot create task with idle priority */
#define MAX_PRIORITY (MAX_USER_TASKS-1U) /* array indexing */
/* if no tasks are ready then the highest ready priority is the idle task */
#define IDLE_TASK_PRIORTY MAX_USER_TASKS
#define NO_TASKS_READY IDLE_TASK_PRIORTY

#define MIN_STACK_SIZE 64U

#define STACK_DEBUG_VAL 0x55555555U
#define STACK_OVERFLOW_DEBUG_VAL 0xDEADBEEFU
#define INITIAL_XPSR 0x01000000U

typedef void (*os_task_fp)(void);
typedef struct task_control_block os_tcb;
typedef struct _os_event_queue os_evtq;
typedef struct _os_event os_event;

typedef void (*os_evt_dispatch)(os_tcb *task, os_event const *const e);

struct task_control_block {
    uint32_t *sp;
    uint32_t priority;
    os_evtq *evt_q;
    os_evt_dispatch dispatch;
};

void os_task_set_ready_flag(uint8_t priority);
void os_task_clear_ready_flag(uint8_t priority);
uint32_t os_get_highest_priority(void);

os_tcb *os_get_current_task(void);
void os_set_current_task(os_tcb *t);
os_tcb *os_get_next_task(void);

os_tcb *os_task_init(os_evt_dispatch d, uint32_t *const stack, uint32_t s_size, 
                     os_event **evtq, uint32_t q_size, uint8_t priority);

void os_idle_task_init(void);
bool os_task_tick(void);

#endif /* TASK_H */