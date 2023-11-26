#ifndef EVENT_H
#define EVENT_H

#include "task.h"
#include <stdint.h>

#define INIT_SIG 0U

#define OS_EVTQ_EMPTY 0UL

#define OS_EVTQ_OK 0L
#define OS_EVTQ_FULL -1L

typedef uint32_t os_event_signal;

struct _os_event {
    os_event_signal e_sig;
};

struct _os_event_queue {
    /* DO NOT CHANGE THE ORDER OF THESE VARIABLES. ASM DEPENDANT */
    uint8_t eq_size;
    uint8_t eq_count;
    uint8_t eq_head;
    uint8_t eq_tail;
    os_event **eq_data;
};

os_evtq *os_evtq_init(uint8_t index, uint8_t q_size, os_event **data);

/* all in assembly: event_asm.s */
/* user api */
os_event *os_evtq_receive(os_tcb *t);
int32_t os_evtq_post(os_tcb *t, os_event *evt);

/* called by kernel */
os_event *os_evtq_receive_syscall(os_evtq *q);
int32_t os_evtq_post_syscall(os_evtq *t, os_event *evt);

#endif /* EVENT_H */